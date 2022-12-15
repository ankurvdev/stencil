#pragma once
#include "comparator.h"
#include "mutatorsaccessors.h"
#include "optionalprops.h"
#include "typetraits_builtins.h"
#include "visitor.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <span>
#include <unordered_map>

namespace Stencil
{

template <typename TElem, typename TContainer = TElem> struct Transaction;

// template <typename T> struct Transaction<T, T>
//{};

template <typename T> constexpr bool                          IsTransaction                                 = false;
template <typename TElem, typename TContainer> constexpr bool IsTransaction<Transaction<TElem, TContainer>> = true;

struct RootTransaction
{
    struct TxnState
    {};
    struct ElemTxnState
    {};
    void NotifyElementEdited_(ElemTxnState const& /*elemTxnState*/) {}
};

template <typename TTxn>
concept ConceptTransaction = IsTransaction<TTxn> || std::is_same_v<TTxn, RootTransaction>;

template <typename TTxn> constexpr bool IsPrimitiveTransaction = false;
template <typename TTxn> constexpr bool IsIterableTransaction  = false;
template <typename TTxn> constexpr bool IsIndexableTransaction = false;

template <ConceptPreferPrimitive TElem, typename TContainer> constexpr bool IsPrimitiveTransaction<Transaction<TElem, TContainer>> = true;
template <ConceptPreferIterable TElem, typename TContainer> constexpr bool  IsIterableTransaction<Transaction<TElem, TContainer>>  = true;
template <ConceptPreferIndexable TElem, typename TContainer> constexpr bool IsIndexableTransaction<Transaction<TElem, TContainer>> = true;

template <typename TTxn>
concept ConceptTransactionForPrimitive = IsPrimitiveTransaction<TTxn>;
template <typename TTxn>
concept ConceptTransactionForIterable = IsIterableTransaction<TTxn>;
template <typename TTxn>
concept ConceptTransactionForIndexable = IsIndexableTransaction<TTxn>;

template <ConceptTransaction TTxn> struct TransactionTraits;

template <typename TElem, typename TContainer> struct TransactionTraits<Transaction<TElem, TContainer>>
{
    using ElemType      = TElem;
    using ContainerType = TContainer;
};

template <typename TTxn, typename TContainer, typename TElem>
TTxn CreateTransaction(typename TTxn::TxnState&           elemState,
                       typename TContainer::ElemTxnState& containerState,
                       TContainer&                        container,
                       TElem&                             elem)
{
    return TTxn(elemState, containerState, container, elem);
}

template <typename TElem> struct Transaction<TElem, void> : Transaction<TElem, RootTransaction>
{
    using Txn = Transaction<TElem, RootTransaction>;

    Transaction(TElem& elem) : Txn(elemState, containerState, container, elem) {}
    ~Transaction() = default;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    RootTransaction               container;
    typename Txn::TxnState        elemState;
    RootTransaction::ElemTxnState containerState{};
};

template <typename TElem> Transaction<TElem, void> CreateRootTransaction(TElem& elem)
{
    return Transaction<TElem, void>(elem);
}

}    // namespace Stencil

template <Stencil::ConceptPreferPrimitive TElem, typename TContainer> struct Stencil::Transaction<TElem, TContainer>
{
    using ElemType          = TElem;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    // struct ElemTxnState
    //{};

    struct TxnState
    {};

    Transaction(TxnState& elemState, ContainerTxnState& containerState, TContainer& container, ElemType& elem) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction() {}

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    ElemType const& Elem() const { return _elem; }

    template <typename TKey, typename TLambda> auto Edit(TKey const& /*key*/, TLambda const& /*lambda*/)
    {
        throw std::logic_error("Elem Not supported on Transaction");
    }

    bool IsChanged() { return false; }    // What about doing AreEqual in the destructor ?

    void Assign(ElemType&& elem)
    {
        if (Stencil::AreEqual(_elem, elem)) return;
        _elem = elem;
        _container.NotifyElementAssigned_(_containerState);
    }

    void Assign(ElemType const& elem)
    {
        auto elem1 = elem;
        Assign(std::move(elem1));
    }
    void Add(ElemType&& /* elem */) { std::logic_error("Invalid operation"); }
    void Remove(size_t /* key */) { std::logic_error("Invalid operation"); }

    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    TElem&             _elem;
};

template <typename TContainer, typename TKey, typename TVal> struct Stencil::Transaction<std::unordered_map<TKey, TVal>, TContainer>
{
    struct ElemTxnState
    {
        TKey key;
    };

    using Txn               = Transaction<std::unordered_map<TKey, TVal>, TContainer>;
    using ElemType          = std::unordered_map<TKey, TVal>;
    using ValTxn            = Transaction<TVal, Txn>;
    using ValTxnState       = typename ValTxn::TxnState;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    struct TxnState
    {
        struct Delta
        {
            enum class Type
            {
                New,
                Edit,
                Delete
            } type;

            ElemTxnState container;
            ValTxnState  elem;
            static Delta New(TKey const& key)
            {
                Delta state;
                state.type          = Type::New;
                state.container.key = key;
                return state;
            }

            static Delta Edit(TKey const& key)
            {
                Delta state;
                state.type          = Type::Edit;
                state.container.key = key;
                return state;
            }

            static Delta Delete(TKey const& key)
            {
                Delta state;
                state.type          = Type::Delete;
                state.container.key = key;
                return state;
            }
        };
        std::unordered_map<TKey, Delta> deltas;
    };

    Transaction(TxnState& elemState, ContainerTxnState& containerState, TContainer& container, ElemType& elem) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction()
    {
        if (IsChanged()) _container.NotifyElementEdited_(_containerState);
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    ElemType const& Elem() const { return _elem; }

    bool IsChanged() const { return !_elemState.deltas.empty(); }

    template <typename TLambda> void Edit(TKey const& key, TLambda&& lambda)
    {
        auto& state = _edit_txn_at(key);
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[key]);
        lambda(txn);
    }
    template <typename TLambda> void Assign(TKey const& key, TLambda&& lambda)
    {
        auto& state = _assign_txn_at(key);
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[key]);
        lambda(txn);
    }

    auto Assign(TKey const& key, TVal&& val)
    {
        _elem[key] = std::move(val);
        return _assign_txn_at(key);
    }

    void Assign(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }
    void Remove(TKey const& key)
    {
        _elem.erase(key);
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::Delete(key)}).first; }
        else { it->second.type = TxnState::Delta::Type::Delete; }
    }

    template <typename TLambda> auto Visit(TKey const& key, TLambda&& lambda) { lambda(key, at(key)); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        for (auto& [k, v] : _elemState.deltas)
        {
            switch (v.type)
            {
            case TxnState::Delta::Type::New:
            {
                auto txn = Stencil::CreateTransaction<ValTxn>(v.elem, v.container, *this, _elem[k]);
                lambda(k, uint8_t{0u}, uint32_t{0u}, txn);
            }
            break;
            case TxnState::Delta::Type::Edit:
            {
                auto txn = Stencil::CreateTransaction<ValTxn>(v.elem, v.container, *this, _elem[k]);
                lambda(k, uint8_t{3u}, uint32_t{0u}, txn);
            }
            break;

            case TxnState::Delta::Type::Delete:
            {
                lambda(k, uint8_t{2u}, uint32_t{0u}, *this);
            }
            break;
            default: throw std::logic_error("Invalid State");
            }
        }
    }

    void NotifyElementEdited_(ElemTxnState const& elemTxnState) { _edit_txn_at(elemTxnState.key); }
    void NotifyElementAssigned_(ElemTxnState const& elemTxnState) { _assign_txn_at(elemTxnState.key); }

    private:
    auto& _edit_txn_at(TKey const& key)
    {
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::Edit(key)}).first; }
        else
        {
            if (it->second.type == TxnState::Delta::Type::Delete) throw std::logic_error("Invalid State");
        }
        return it->second;
    }

    auto& _assign_txn_at(TKey const& key)
    {
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::New(key)}).first; }
        else { it->second.type = TxnState::Delta::Type::New; }
        return it->second;
    }

    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;
};

template <Stencil::ConceptPreferIterable TElem, typename TContainer> struct Stencil::Transaction<TElem, TContainer>
{
    struct ElemTxnState
    {
        uint8_t  mutationtype;
        uint32_t index;
    };

    using ElemType          = TElem;
    using Txn               = Stencil::Transaction<TElem, TContainer>;
    using ValType           = typename Stencil::TypeTraitsForIterable<TElem>::ElementType;
    using ValTxn            = Transaction<ValType, Txn>;
    using ValTxnState       = typename ValTxn::TxnState;
    using ContainerTxnState = typename TContainer::ElemTxnState;
    using IteratorType      = uint32_t;    // TODO: Use Iterators ?

    struct CombinedTxnState
    {
        ElemTxnState elemState;
        ValTxnState  valState;
    };

    struct TxnState
    {
        std::vector<CombinedTxnState> changes;
    };

    using TContainerTxnState = typename TContainer::ElemTxnState;

    Transaction(TxnState& elemState, ContainerTxnState& containerState, TContainer& container, ElemType& elem) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction()
    {
        if (IsChanged()) _container.NotifyElementEdited_(_containerState);
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    ElemType const& Elem() const { return _elem; }

    template <typename TArg> void RecordMutation_add_(TArg&)
    {
        _elemState.changes.push_back(CombinedTxnState{.elemState{1u, static_cast<uint32_t>(Elem().size())}, .valState{}});
    }

    void RecordMutation_remove_(IteratorType it) { _elemState.changes.push_back(CombinedTxnState{.elemState{2u, it}, .valState{}}); }
    void RecordMutation_edit_(IteratorType /* index */) {}
    void RecordMutation_assign_(IteratorType it) { _elemState.changes.push_back(CombinedTxnState{.elemState{1u, it}, .valState{}}); }

    void Add(ValType&& val)
    {
        RecordMutation_add_(val);
        Stencil::Mutators<ElemType>::add(_elem, std::move(val));
    }

    void Remove(uint32_t key)
    {
        RecordMutation_remove_(key);
        Stencil::Mutators<ElemType>::remove(_elem, key);
    }

    auto edit(IteratorType it)
    {
        // TODO: DoNotCommit  Use NotifyElementEdited to store it permanently
        _elemState.changes.push_back(CombinedTxnState{.elemState{3u, it}, .valState{}});
        auto& change = _elemState.changes.back();
        return CreateTransaction<ValTxn>(change.valState, change.elemState, *this, _elem.at(it));
    }

    template <typename TLambda> auto Edit(IteratorType it, TLambda&& lambda)
    {
        Stencil::Visitor<TElem>::VisitKey(_elem, it, [&](auto& /*val*/) {
            RecordMutation_edit_(it);
            _elemState.changes.push_back(CombinedTxnState{.elemState{3u, it}, .valState{}});
            auto& change = _elemState.changes.back();
            auto  txn    = CreateTransaction<ValTxn>(change.valState, change.elemState, *this, _elem.at(it));
            lambda(txn);
        });
    }

    void NotifyElementEdited_(ElemTxnState const& /*elemTxnState*/)
    { /* Do nothing for now. The change is already pushed at 328
        TODO("DoNotCommit");*/
    }

    void Assign(ElemType&& /* elem */) { throw std::logic_error("Self-Assignment not allowed"); }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }
    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        if (_elemState.changes.size() == 0) return;

        for (size_t i = 0; i < _elemState.changes.size(); i++)
        {
            auto& c     = _elemState.changes[i];
            auto  index = c.elemState.index;
            for (size_t j = i + 1; j < _elemState.changes.size(); j++)
            {
                auto& c1 = _elemState.changes[j];
                if (c1.elemState.mutationtype == 2u && c1.elemState.index < index) { index--; }
                if (c1.elemState.mutationtype == 1u && c1.elemState.index < index) { index++; }
            }
            // TODO : Fix me. This is horrible
            //  auto& elem = (*_fn(_container))[index];
            // if (c.txn.get() == nullptr) { c.txn = std::make_unique<TValTransaction>(this, nullptr); }
            auto txn = CreateTransaction<ValTxn>(c.valState, c.elemState, *this, _elem.at(index));
            lambda(c.elemState.index, c.elemState.mutationtype, uint32_t{0u}, txn);
        }
    }

    bool IsChanged() { return _elemState.changes.size() > 0; }

    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;
};

// Transaction Mutators Accessors
template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::Mutators<TTxn>
{
    using ElemType = typename Stencil::TransactionTraits<TTxn>::ElemType;

    // TODO: DoNotCommit
    template <typename TVal> static void add(TTxn& txn, TVal&& elem) { txn.Add(std::move(elem)); }

    static void remove(TTxn& txn, size_t index) { txn.Remove(static_cast<uint32_t>(index)); }
    static auto edit(TTxn& txn, size_t index) { return txn.edit(static_cast<uint32_t>(index)); }
};

template <Stencil::ConceptTransaction TTxn> struct Stencil::TypeTraits<TTxn>
{
    using Categories = typename Stencil::TypeTraits<typename TransactionTraits<TTxn>::ElemType>::Categories;
};

template <Stencil::ConceptTransactionForIndexable TTxn> struct Stencil::TypeTraitsForIndexable<TTxn>
{
    using Key = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<TTxn>::ElemType>::Key;
};

template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::TypeTraitsForIterable<TTxn>
{
    using ElementType = typename Stencil::TypeTraitsForIterable<typename TransactionTraits<TTxn>::ElemType>::ElementType;
};

template <Stencil::ConceptTransactionForPrimitive TTxn> struct Stencil::TypeTraitsForPrimitive<TTxn>
{};

template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::VisitorForIterable<TTxn>
{
    using ElemType = typename TransactionTraits<TTxn>::ElemType;
    using Iterator = typename Stencil::Visitor<ElemType>::Iterator;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    static void IteratorBegin(Iterator& /*it*/, T1& /*elem*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::IteratorBegin(it, *elem.get());");
    }

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    static void IteratorMoveNext(Iterator& /*it*/, T1& /*elem*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::IteratorMoveNext(it, *elem.get());");
    }

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    static bool IteratorValid(Iterator& /*it*/, T1& /*elem*/)
    {
        TODO("DoNotCommit: return Stencil::Visitor<T>::IteratorValid(it, *elem.get());");
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    static void Visit(Iterator& /*it*/, T1& /*elem*/, TLambda&& /*lambda*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::Visit(it, *elem.get(), std::forward<TLambda>(lambda));");
    }
};

template <Stencil::ConceptTransactionForIndexable T> struct Stencil::VisitorForIndexable<T>
{};

template <Stencil::ConceptTransaction TTxn>
struct Stencil::Visitor<TTxn> : Stencil::VisitorT<TTxn>, Stencil::VisitorForIterable<TTxn>, Stencil::VisitorForIndexable<TTxn>
{
    using ElemType = typename TransactionTraits<TTxn>::ElemType;

    // So that this works for both const and non-const
    template <typename TKey, typename TLambda> static void VisitKey(TTxn& txn, TKey&& key, TLambda&& lambda)
    {
        txn.Edit(std::forward<TKey>(key), std::forward<TLambda>(lambda));
    }

    template <typename TLambda> static void VisitAll(TTxn& txn, TLambda&& lambda) { txn.VisitAll(std::forward<TLambda>(lambda)); }
};

template <Stencil::ConceptTransactionForPrimitive TTxn, typename TProtocol> struct Stencil::SerDes<TTxn, TProtocol>
{
    using ElemType = typename TransactionTraits<TTxn>::ElemType;

    template <typename TContext> static auto Write(TContext& ctx, TTxn const& txn)
    {
        Stencil::SerDes<ElemType, TProtocol>::Write(ctx, txn.Elem());
    }

    template <typename TContext> static auto Read(TTxn& txn, TContext& ctx)
    {
        auto val = txn.Elem();
        Stencil::SerDes<ElemType, TProtocol>::Read(val, ctx);
        txn.Assign(val);
        // txn.MarkEdited();
    }
};
