#pragma once
#include "CommonMacros.h"
#include "comparator.h"
#include "mutatorsaccessors.h"
#include "serdes.h"
#include "visitor.h"

#include <algorithm>
#include <iterator>
#include <unordered_map>

namespace Stencil
{

template <typename TElem, typename TContainer = void> struct Transaction;

template <typename TElem, typename TContainer = void> struct TransactionView;

// template <typename T> struct Transaction<T, T>
//{};

template <typename T> constexpr bool                          IsTransaction                                 = false;
template <typename TElem, typename TContainer> constexpr bool IsTransaction<Transaction<TElem, TContainer>> = true;
template <typename TElem> constexpr bool                      IsTransaction<Transaction<TElem>>             = true;

template <typename T> constexpr bool                          IsTransactionView                                     = false;
template <typename TElem, typename TContainer> constexpr bool IsTransactionView<TransactionView<TElem, TContainer>> = true;
template <typename TElem> constexpr bool                      IsTransactionView<TransactionView<TElem>>             = true;

struct RootTransaction
{
    using View = RootTransaction;
    using Txn  = RootTransaction;
    struct TxnState
    {};
    struct ElemTxnState
    {};
    void NotifyElementEdited(ElemTxnState const& /*elemTxnState*/) {}
};

using RootTransactionView = RootTransaction;

struct RootTransactionView1
{
    using Txn          = RootTransaction;
    using TxnState     = Txn::TxnState;
    using ElemTxnState = Txn::ElemTxnState;
};

template <typename TTxn>
concept ConceptTransaction = IsTransaction<TTxn> || std::is_same_v<TTxn, RootTransaction>;

template <typename TTxn>
concept ConceptTransactionView = IsTransactionView<TTxn> || std::is_same_v<TTxn, RootTransactionView>;

template <typename TTxn> constexpr bool IsPrimitiveTransaction = false;
template <typename TTxn> constexpr bool IsIterableTransaction  = false;
template <typename TTxn> constexpr bool IsIndexableTransaction = false;

template <typename TTxn> constexpr bool IsPrimitiveTransactionView = false;
template <typename TTxn> constexpr bool IsIterableTransactionView  = false;
template <typename TTxn> constexpr bool IsIndexableTransactionView = false;

template <ConceptPreferPrimitive TElem, typename TContainer> constexpr bool IsPrimitiveTransaction<Transaction<TElem, TContainer>> = true;
template <ConceptPreferIterable TElem, typename TContainer> constexpr bool  IsIterableTransaction<Transaction<TElem, TContainer>>  = true;
template <ConceptPreferIndexable TElem, typename TContainer> constexpr bool IsIndexableTransaction<Transaction<TElem, TContainer>> = true;

template <ConceptPreferPrimitive TElem, typename TContainer>
constexpr bool IsPrimitiveTransactionView<TransactionView<TElem, TContainer>> = true;
template <ConceptPreferIterable TElem, typename TContainer>
constexpr bool IsIterableTransactionView<TransactionView<TElem, TContainer>> = true;
template <ConceptPreferIndexable TElem, typename TContainer>
constexpr bool IsIndexableTransactionView<TransactionView<TElem, TContainer>> = true;

template <typename TTxn>
concept ConceptTransactionForPrimitive = IsPrimitiveTransaction<TTxn>;
template <typename TTxn>
concept ConceptTransactionForIterable = IsIterableTransaction<TTxn>;
template <typename TTxn>
concept ConceptTransactionForIndexable = IsIndexableTransaction<TTxn>;

template <typename TTxn>
concept ConceptTransactionViewForPrimitive = IsPrimitiveTransactionView<TTxn>;
template <typename TTxn>
concept ConceptTransactionViewForIterable = IsIterableTransactionView<TTxn>;
template <typename TTxn>
concept ConceptTransactionViewForIndexable = IsIndexableTransactionView<TTxn>;

template <ConceptTransaction TTxn> struct TransactionTraits;

template <typename TElem, typename TContainer> struct TransactionTraits<Transaction<TElem, TContainer>>
{
    using ElemType      = TElem;
    using ContainerType = TContainer;
    // using View          = TransactionView<TElem, typename TContainer::View>;
};

template <ConceptTransaction TTxn, ConceptTransaction TContainer, typename TElem>
TTxn CreateTransaction(typename TTxn::TxnState& elemState                LFTBND,
                       typename TContainer::ElemTxnState& containerState LFTBND,
                       TContainer& container                             LFTBND,
                       TElem& elem                                       LFTBND)
{ return TTxn(elemState, containerState, container, elem); }

template <ConceptTransactionView TTxn, ConceptTransactionView TContainer, typename TElem>
TTxn CreateTransactionView(typename TTxn::TxnState const& elemState                LFTBND,
                           typename TContainer::ElemTxnState const& containerState LFTBND,
                           TContainer const& container                             LFTBND,
                           TElem const& elem                                       LFTBND)
{ return TTxn(elemState, containerState, container, elem); }

template <typename TElem> struct Transaction<TElem, void> : Transaction<TElem, RootTransaction>
{
    using Txn  = Transaction<TElem, RootTransaction>;
    using View = TransactionView<TElem, RootTransactionView>;

    Transaction(TElem& elemIn LFTBND) : Txn(elemState, containerState, container, elemIn), elem(elemIn) {} //NOLINT
    ~Transaction() = default;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);
    operator View() const LFTBND { return View(elemState, containerState, container, elem); } //NOLINT

    TElem&                        elem;
    RootTransaction               container{};
    Txn::TxnState                 elemState{};
    RootTransaction::ElemTxnState containerState{};
};

template <typename TElem> struct TransactionView<TElem, void> : TransactionView<TElem, RootTransactionView>
{
    using Txn = Transaction<TElem, RootTransaction>;
    explicit TransactionView(TElem const& elemIn LFTBND) :
        TransactionView<TElem, RootTransactionView>(elemState, containerState, container, elemIn)
    {}
    ~TransactionView() = default;
    CLASS_DELETE_COPY_AND_MOVE(TransactionView);

    RootTransactionView const               container{};
    Txn::TxnState const                     elemState{};
    RootTransactionView::ElemTxnState const containerState{};
};

template <typename TElem> Transaction<TElem, void> CreateRootTransaction(TElem& elem LFTBND)
{ return Transaction<TElem, void>(elem); }

template <typename TElem> Transaction<TElem, void> CreateRootTransactionView(TElem const& elem)
{ return TransactionView<TElem, void>(elem); }

}    // namespace Stencil

template <Stencil::ConceptPreferPrimitive TElem, Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<TElem, TContainer>
{
    using ElemType          = TElem;
    using ContainerTxnState = TContainer::ElemTxnState;
    using View              = Stencil::TransactionView<TElem, typename TContainer::View>;

    // struct ElemTxnState
    //{};

    struct TxnState
    {};

    Transaction(TxnState& elemStateIn               LFTBND,
                ContainerTxnState& containerStateIn LFTBND,
                TContainer& containerIn             LFTBND,
                ElemType& elemIn                    LFTBND) :
        elemState(elemStateIn), containerState(containerStateIn), container(containerIn), elem(elemIn)
    {}

    ~Transaction() = default;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    [[nodiscard]] ElemType const& Elem() const { return elem; }

    explicit operator View() const { return CreateTransactionView<View>(elemState, containerState, container, elem); }

    template <typename TKey, typename TLambda> auto Edit(TKey const& /*key*/, TLambda const& /*lambda*/)
    { throw std::logic_error("Elem Not supported on Transaction"); }

    bool IsChanged() { return container.IsElementChanged(containerState); }

    void Assign(ElemType&& elemIn)
    {
        if (Stencil::AreEqual(elem, elemIn)) return;
        elem = std::move(elemIn);
        container.NotifyElementAssigned(containerState);
    }

    void Assign(ElemType const& elemIn)
    {
        auto elem1 = elemIn;
        Assign(std::move(elem1));
    }

    void Add(ElemType&& elemIn)
    {
        RecordMutation_add_(elemIn);
        Stencil::Mutators<ElemType>::add(elem, std::move(elemIn));
    }

    [[noreturn]] void Remove(size_t /* key */) { throw std::logic_error("Invalid operation"); }

    TxnState&          elemState;
    ContainerTxnState& containerState;
    TContainer&        container;
    TElem&             elem;
};

template <Stencil::ConceptPreferPrimitive TElem, Stencil::ConceptTransactionView TContainer>
struct Stencil::TransactionView<TElem, TContainer>
{
    using ElemType          = TElem;
    using Txn               = Stencil::Transaction<TElem, typename TContainer::Txn>;
    using ContainerTxnState = TContainer::ElemTxnState;

    // struct ElemTxnState
    //{};
    using TxnState = Txn::TxnState;

    TransactionView(TxnState const& elemStateIn               LFTBND,
                    ContainerTxnState const& containerStateIn LFTBND,
                    TContainer const& containerIn             LFTBND,
                    ElemType const& elemIn                    LFTBND) :
        elemState(elemStateIn), containerState(containerStateIn), container(containerIn), elem(elemIn)
    {}
    ~TransactionView() = default;
    CLASS_DELETE_COPY_AND_MOVE(TransactionView);

    [[nodiscard]] ElemType const& Elem() const { return elem; }

    bool IsChanged() { return container.IsElementChanged(containerState); }

    TxnState const&          elemState;
    ContainerTxnState const& containerState;
    TContainer const&        container;
    TElem const&             elem;
};

template <typename TVal, Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<std::vector<TVal>, TContainer>
{
    struct ElemTxnState
    { size_t index; };

    using Txn               = Transaction<std::vector<TVal>, TContainer>;
    using View              = TransactionView<std::vector<TVal>, typename TContainer::View>;
    using ElemType          = std::vector<TVal>;
    using ValTxn            = Transaction<TVal, Txn>;
    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;

    struct TxnState
    {
        struct Delta
        {
            enum class Type
            {
                Assign,
                ListAdd,
                ListDelete,
                Edit,
            } type;

            size_t       index{};
            ElemTxnState container{};
            ValTxnState  elem;
            static Delta Assign(size_t const& index)
            {
                Delta state{};
                state.index           = index;
                state.type            = Type::Assign;
                state.container.index = index;
                return state;
            }

            static Delta ListAdd(size_t const& index)
            {
                Delta state{};
                state.index           = index;
                state.type            = Type::ListAdd;
                state.container.index = index;
                return state;
            }

            static Delta ListDelete(size_t const& index)
            {
                Delta state{};
                state.index           = index;
                state.type            = Type::ListDelete;
                state.container.index = index;
                return state;
            }

            static Delta Edit(size_t const& index)
            {
                Delta state{};
                state.index           = index;
                state.type            = Type::Edit;
                state.container.index = index;
                return state;
            }
        };
        std::vector<Delta> deltas;
        int                dummy = -1;
    };

    Transaction(TxnState& elemState LFTBND, ContainerTxnState& containerState LFTBND, TContainer& container LFTBND, ElemType& elem LFTBND) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction()
    {
        if (IsChanged()) _container.NotifyElementEdited(_containerState);
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
    explicit        operator View() const { return CreateTransactionView<View>(_elemState, _containerState, _container, _elem); }
    [[nodiscard]] ElemType const& Elem() const { return _elem; }

    [[nodiscard]] bool IsChanged() const { return !_elemState.deltas.empty(); }

    template <typename TLambda> void Edit(size_t const& index, TLambda&& lambda)
    {
        _elemState.deltas.push_back(TxnState::Delta::Edit(index));
        auto& state = _elemState.deltas.back();
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[index]);
        lambda(txn);
    }

    auto Edit(size_t const& index) LFTBND
    {
        _elemState.deltas.push_back(TxnState::Delta::Edit(index));
        auto& state = _elemState.deltas.back();
        return Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[index]);
    }

    template <typename TLambda> void Assign(size_t const& index, TLambda&& lambda)
    {
        _elemState.deltas.push_back(TxnState::Delta::Assign(index));
        auto& state = _elemState.deltas.back();
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[index]);
        lambda(txn);
    }

    auto Assign(size_t const& index, TVal&& val)
    {
        _elem[index] = std::move(val);
        return _elemState.deltas.insert(TxnState::Delta::Assign(index));
    }

    [[noreturn]] void Assign(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    void Add(TVal&& elem)
    {
        auto index = _elem.size();
        _elemState.deltas.push_back(TxnState::Delta::ListAdd(index));
        Stencil::Mutators<ElemType>::add(_elem, std::move(elem));
    }

    void Remove(size_t const& index)
    {
        _elem.erase(_elem.begin() + static_cast<long>(index));
        _elemState.deltas.push_back(TxnState::Delta::ListDelete(index));
    }

    template <typename TLambda> auto Visit(size_t const& index, TLambda&& lambda) { lambda(index, _elem.at(index)); }

    void NotifyElementEdited(ElemTxnState const& elemTxnState) { _elemState.deltas.push_back(TxnState::Delta::Edit(elemTxnState.index)); }
    void NotifyElementAssigned(ElemTxnState const& elemTxnState)
    { _elemState.deltas.push_back(TxnState::Delta::Assign(elemTxnState.index)); }

    private:
    TxnState&          _elemState{};
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;
};

template <typename TVal, Stencil::ConceptTransactionView TContainer> struct Stencil::TransactionView<std::vector<TVal>, TContainer>
{
    using Txn               = Transaction<std::vector<TVal>, typename TContainer::Txn>;
    using View              = TransactionView<std::vector<TVal>, TContainer>;
    using TxnState          = Txn::TxnState;
    using ElemTxnState      = Txn::ElemTxnState;
    using ElemType          = std::vector<TVal>;
    using ValTxn            = Transaction<TVal, Txn>;
    using ValTxnView        = TransactionView<TVal, View>;
    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;

    TransactionView(TxnState const& elemState               LFTBND,
                    ContainerTxnState const& containerState LFTBND,
                    TContainer const& container             LFTBND,
                    ElemType const& elem                    LFTBND) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~TransactionView() = default;

    CLASS_DELETE_COPY_AND_MOVE(TransactionView);

    [[nodiscard]] ElemType const& Elem() const { return _elem; }

    [[nodiscard]] bool IsChanged() const { return !_elemState.deltas.empty(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda) const
    {
        using DeltaType   = TxnState::Delta::Type;
        auto adjustedElem = [&](auto it) -> auto& {
            auto index = it->index;
            for (auto it1 = it; it1 != _elemState.deltas.end(); ++it1)
            {
                if (it1->type == DeltaType::ListDelete && it1->index < index)
                {    //
                    index--;
                }
            }
            return _elem[index];
        };
        for (auto it = _elemState.deltas.begin(); it != _elemState.deltas.end(); ++it)
        {
            auto& v = *it;

            switch (v.type)
            {
            case DeltaType::Assign:
            {
                auto const txn = Stencil::CreateTransactionView<ValTxnView>(v.elem, v.container, *this, adjustedElem(it));
                lambda(v.index, uint8_t{0u}, uint32_t{0u}, txn);
            }
            break;
            case DeltaType::ListAdd:
            {
                auto const txn = Stencil::CreateTransactionView<ValTxnView>(v.elem, v.container, *this, adjustedElem(it));
                lambda(v.index, uint8_t{1u}, uint32_t{0u}, txn);
            }
            break;
            case DeltaType::Edit:
            {
                auto const txn = Stencil::CreateTransactionView<ValTxnView>(v.elem, v.container, *this, adjustedElem(it));
                lambda(v.index, uint8_t{3u}, uint32_t{0u}, txn);
            }
            break;

            case DeltaType::ListDelete:
            {
                auto const* ptr = this;
                lambda(v.index, uint8_t{2u}, uint32_t{0u}, *ptr);
            }
            break;
            default: throw std::logic_error("Invalid State");
            }
        }
    }

    private:
    TxnState const&          _elemState;
    ContainerTxnState const& _containerState;
    TContainer const&        _container;
    ElemType const&          _elem;
};

template <Stencil::ConceptTransaction TContainer, typename TKey, typename TVal>
struct Stencil::Transaction<std::unordered_map<TKey, TVal>, TContainer>
{
    struct ElemTxnState
    { TKey key; };

    using Txn               = Transaction<std::unordered_map<TKey, TVal>, TContainer>;
    using View              = TransactionView<std::unordered_map<TKey, TVal, typename TContainer::View>>;
    using ElemType          = std::unordered_map<TKey, TVal>;
    using ValTxn            = Transaction<TVal, Txn>;
    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;

    struct TxnState
    {
        struct Delta
        {
            enum class Type
            {
                New,
                Edit,
                Delete,
            } type;

            ElemTxnState container{};
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

    Transaction(TxnState& elemState LFTBND, ContainerTxnState& containerState LFTBND, TContainer& container LFTBND, ElemType& elem LFTBND) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction()
    {
        if (IsChanged()) _container.NotifyElementEdited(_containerState);
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    explicit        operator View() const { return CreateTransactionView<View>(_elemState, _containerState, _container, _elem); }
    [[nodiscard]] ElemType const& Elem() const { return _elem; }

    [[nodiscard]] bool IsChanged() const { return !_elemState.deltas.empty(); }

    template <typename TLambda> void Edit(TKey const& key, TLambda&& lambda)
    {
        auto& state = edit_txn_at_(key);
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[key]);
        lambda(txn);
    }
    template <typename TLambda> void Assign(TKey const& key, TLambda&& lambda)
    {
        auto& state = assign_txn_at_(key);
        auto  txn   = Stencil::CreateTransaction<ValTxn>(state.elem, state.container, *this, _elem[key]);
        lambda(txn);
    }

    auto Assign(TKey const& key, TVal&& val)
    {
        _elem[key] = std::move(val);
        return assign_txn_at_(key);
    }

    [[noreturn]] void Assign(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }
    [[noreturn]] void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    void Remove(TKey const& key)
    {
        _elem.erase(key);
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::Delete(key)}).first; }
        else
        {
            it->second.type = TxnState::Delta::Type::Delete;
        }
    }

    template <typename TLambda> auto Visit(TKey const& key, TLambda&& lambda) { lambda(key, _elem.at(key)); }

    void NotifyElementEdited(ElemTxnState const& elemTxnState) { edit_txn_at_(elemTxnState.key); }
    void NotifyElementAssigned(ElemTxnState const& elemTxnState) { assign_txn_at_(elemTxnState.key); }

    private:
    auto& edit_txn_at_(TKey const& key)
    {
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::Edit(key)}).first; }
        else
        {
            if (it->second.type == TxnState::Delta::Type::Delete) throw std::logic_error("Invalid State");
        }
        return it->second;
    }

    auto& assign_txn_at_(TKey const& key)
    {
        auto it = _elemState.deltas.find(key);
        if (it == _elemState.deltas.end()) { it = _elemState.deltas.insert({key, TxnState::Delta::New(key)}).first; }
        else
        {
            it->second.type = TxnState::Delta::Type::New;
        }
        return it->second;
    }

    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;
};

template <Stencil::ConceptTransactionView TContainer, typename TKey, typename TVal>
struct Stencil::TransactionView<std::unordered_map<TKey, TVal>, TContainer>
{
    using Txn               = Transaction<std::unordered_map<TKey, TVal>, typename TContainer::Txn>;
    using View              = TransactionView<std::unordered_map<TKey, TVal>, TContainer>;
    using TxnState          = Txn::TxnState;
    using ElemTxnState      = Txn::ElemTxnState;
    using ElemType          = std::unordered_map<TKey, TVal>;
    using ValTxn            = Transaction<TVal, Txn>;
    using ValTxnView        = TransactionView<TVal, View>;
    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;

    TransactionView(TxnState const& elemStateIn               LFTBND,
                    ContainerTxnState const& containerStateIn LFTBND,
                    TContainer const& containerIn             LFTBND,
                    ElemType const& elemIn                    LFTBND) :
        elemState(elemStateIn), containerState(containerStateIn), container(containerIn), elem(elemIn)
    {}

    ~TransactionView() = default;
    CLASS_DELETE_COPY_AND_MOVE(TransactionView);
    [[nodiscard]] ElemType const& Elem() const { return elem; }

    [[nodiscard]] bool IsChanged() const { return !elemState.deltas.empty(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda) const
    {
        for (auto& [k, v] : elemState.deltas)
        {
            switch (v.type)
            {
            case TxnState::Delta::Type::New:
            {
                auto const txn = Stencil::CreateTransactionView<ValTxnView>(v.elem, v.container, *this, elem.at(k));
                lambda(k, uint8_t{0u}, uint32_t{0u}, txn);
            }
            break;
            case TxnState::Delta::Type::Edit:
            {
                auto const txn = Stencil::CreateTransactionView<ValTxnView>(v.elem, v.container, *this, elem.at(k));
                lambda(k, uint8_t{3u}, uint32_t{0u}, txn);
            }
            break;

            case TxnState::Delta::Type::Delete:
            {
                auto const* ptr = this;
                lambda(k, uint8_t{2u}, uint32_t{0u}, *ptr);
            }
            break;
            default: throw std::logic_error("Invalid State");
            }
        }
    }

    TxnState const&          elemState;
    ContainerTxnState const& containerState;
    TContainer const&        container;
    ElemType const&          elem;
};

template <Stencil::ConceptPreferIterable TElem, Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<TElem, TContainer>
{
    struct ElemTxnState
    {
        uint8_t  mutationtype;
        uint32_t index;
    };

    using ElemType          = TElem;
    using Txn               = Stencil::Transaction<TElem, TContainer>;
    using View              = Stencil::TransactionView<TElem, typename TContainer::View>;
    using ValType           = Stencil::TypeTraitsForIterable<TElem>::ElementType;
    using ValTxn            = Transaction<ValType, Txn>;
    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;
    using IteratorType      = uint32_t;

    struct CombinedTxnState
    {
        ElemTxnState elemState;
        ValTxnState  valState;
    };

    struct TxnState
    { std::vector<CombinedTxnState> changes; };

    using TContainerTxnState = TContainer::ElemTxnState;

    Transaction(TxnState& elemStateIn, ContainerTxnState& containerStateIn, TContainer& containerIn, ElemType& elemIn) :
        elemState(elemStateIn), containerState(containerStateIn), container(containerIn), elem(elemIn)
    {}

    ~Transaction()
    {
        if (IsChanged()) container.NotifyElementEdited(containerState);
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
    explicit        operator View() const { return CreateTransactionView<View>(elemState, containerState, container, elem); }
    ElemType const& Elem() const { return elem; }

    template <typename TArg> void RecordMutationAdd(TArg& /*unused*/)
    { elemState.changes.push_back(CombinedTxnState{.elemState{1u, static_cast<uint32_t>(Elem().size())}, .valState{}}); }

    void RecordMutationRemove(IteratorType it) { elemState.changes.push_back(CombinedTxnState{.elemState{2u, it}, .valState{}}); }
    void RecordMutationEdit(IteratorType /* index */) {}
    void RecordMutationAssign(IteratorType it) { elemState.changes.push_back(CombinedTxnState{.elemState{1u, it}, .valState{}}); }

    void Add(ValType&& val)
    {
        RecordMutation_add_(val);
        Stencil::Mutators<ElemType>::add(elem, std::move(val));
    }

    void Remove(uint32_t key)
    {
        RecordMutationRemove(key);
        Stencil::Mutators<ElemType>::remove(elem, key);
    }

    auto Edit(IteratorType it)
    {
        // TODO: DoNotCommit  Use NotifyElementEdited to store it permanently
        elemState.changes.push_back(CombinedTxnState{.elemState{3u, it}, .valState{}});
        auto& change = elemState.changes.back();
        return CreateTransaction<ValTxn>(change.valState, change.elemState, *this, elem.at(it));
    }

    template <typename TLambda> auto Edit(IteratorType it, TLambda&& lambda)
    {
        Stencil::Visitor<TElem>::VisitKey(elem, it, [&](auto& /*val*/) {
            RecordMutationEdit(it);
            elemState.changes.push_back(CombinedTxnState{.elemState{3u, it}, .valState{}});
            auto& change = elemState.changes.back();
            auto  txn    = CreateTransaction<ValTxn>(change.valState, change.elemState, *this, elem.at(it));
            lambda(txn);
        });
    }

    void NotifyElementEdited(ElemTxnState const& /*elemTxnState*/) { /* Do nothing for now. The change is already pushed at 328
                                                                        TODO("DoNotCommit");*/
    }

    [[noreturn]] void Assign(ElemType&& /* elem */) { throw std::logic_error("Self-Assignment not allowed"); }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }

    bool IsChanged() { return elemState.changes.size() > 0; }

    TxnState&          elemState;
    ContainerTxnState& containerState;
    TContainer&        container;
    ElemType&          elem;
};

template <Stencil::ConceptPreferIterable TElem, Stencil::ConceptTransactionView TContainer>
struct Stencil::TransactionView<TElem, TContainer>
{
    using ElemType = TElem;
    using Txn      = Stencil::Transaction<TElem, typename TContainer::Txn>;
    using View     = Stencil::TransactionView<TElem, TContainer>;

    using ValType    = Stencil::TypeTraitsForIterable<TElem>::ElementType;
    using ValTxn     = Transaction<ValType, Txn>;
    using ValTxnView = Transaction<ValType, View>;

    using ValTxnState       = ValTxn::TxnState;
    using ContainerTxnState = TContainer::ElemTxnState;
    using IteratorType      = uint32_t;

    using CombineddTxnState = Txn::CombinedTxnState;
    using ElemTxnState      = Txn::ElemTxnState;
    using TxnState          = Txn::TxnState;

    using TContainerTxnState = TContainer::ElemTxnState;

    TransactionView(TxnState& elemStateIn, ContainerTxnState& containerStateIn, TContainer& containerIn, ElemType& elemIn) :
        elemState(elemStateIn), containerState(containerStateIn), container(containerIn), elem(elemIn)
    {}

    ~TransactionView() = default;
    CLASS_DELETE_COPY_AND_MOVE(TransactionView);

    ElemType const& Elem() const { return elem; }

    template <typename TLambda> void VisitChanges(TLambda&& lambda) const
    {
        if (elemState.changes.size() == 0) return;

        for (size_t i = 0; i < elemState.changes.size(); i++)
        {
            auto& c     = elemState.changes[i];
            auto  index = c.elemState.index;
            for (size_t j = i + 1; j < elemState.changes.size(); j++)
            {
                auto& c1 = elemState.changes[j];
                if (c1.elemState.mutationtype == 2u && c1.elemState.index < index) { index--; }
                if (c1.elemState.mutationtype == 1u && c1.elemState.index < index) { index++; }
            }
            // TODO : Fix me. This is horrible
            //  auto& elem = (*_fn(_container))[index];
            // if (c.txn.get() == nullptr) { c.txn = std::make_unique<TValTransaction>(this, nullptr); }
            auto txn = CreateTransactionView<ValTxnView>(c.valState, c.elemState, *this, elem.at(index));
            lambda(c.elemState.index, c.elemState.mutationtype, uint32_t{0u}, txn);
        }
    }

    [[nodiscard]] bool IsChanged() const { return elemState.changes.size() > 0; }

    TxnState const&          elemState;
    ContainerTxnState const& containerState;
    TContainer const&        container;
    ElemType const&          elem;
};
// Transaction Mutators Accessors
template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::Mutators<TTxn>
{
    using ElemType = Stencil::TransactionTraits<TTxn>::ElemType;

    // TODO: DoNotCommit
    template <typename TVal> static void add(TTxn& txn, TVal&& elem) { txn.Add(std::forward<TVal>(elem)); } //NOLINT
    static void remove(TTxn& txn, size_t index) { txn.Remove(static_cast<uint32_t>(index)); } //NOLINT
    static auto edit(TTxn& txn, size_t index) { return txn.Edit(static_cast<uint32_t>(index)); } //NOLINT
};

template <Stencil::ConceptTransaction TTxn> struct Stencil::TypeTraits<TTxn>
{
    using Categories = Stencil::TypeTraits<typename TransactionTraits<TTxn>::ElemType>::Categories;
};

template <Stencil::ConceptTransactionForIndexable TTxn> struct Stencil::TypeTraitsForIndexable<TTxn>
{
    using Key = Stencil::TypeTraitsForIndexable<typename TransactionTraits<TTxn>::ElemType>::Key;
};

template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::TypeTraitsForIterable<TTxn>
{
    using ElementType = Stencil::TypeTraitsForIterable<typename TransactionTraits<TTxn>::ElemType>::ElementType;
};

template <Stencil::ConceptTransactionForPrimitive TTxn> struct Stencil::TypeTraitsForPrimitive<TTxn>
{};

template <Stencil::ConceptTransactionForIterable TTxn> struct Stencil::VisitorForIterable<TTxn>
{
    using ElemType = TransactionTraits<TTxn>::ElemType;
    using Iterator = Stencil::Visitor<ElemType>::Iterator;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    [[noreturn]] static void IteratorBegin(Iterator& /*it*/, T1& /*elem*/)
    { TODO("DoNotCommit: Stencil::Visitor<T>::IteratorBegin(it, *elem.get());"); }

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    [[noreturn]] static void IteratorMoveNext(Iterator& /*it*/, T1& /*elem*/)
    { TODO("DoNotCommit: Stencil::Visitor<T>::IteratorMoveNext(it, *elem.get());"); }

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    [[noreturn]] static bool IteratorValid(Iterator& /*it*/, T1& /*elem*/)
    { TODO("DoNotCommit: return Stencil::Visitor<T>::IteratorValid(it, *elem.get());"); }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TTxn>
    [[noreturn]] static void Visit(Iterator& /*it*/, T1& /*elem*/, TLambda&& /*lambda*/)
    { TODO("DoNotCommit: Stencil::Visitor<T>::Visit(it, *elem.get(), std::forward<TLambda>(lambda));"); }
};

template <Stencil::ConceptTransactionForIndexable T> struct Stencil::VisitorForIndexable<T>
{};

template <Stencil::ConceptTransaction TTxn>
struct Stencil::Visitor<TTxn> : Stencil::VisitorT<TTxn>, Stencil::VisitorForIterable<TTxn>, Stencil::VisitorForIndexable<TTxn>
{
    using ElemType = TransactionTraits<TTxn>::ElemType;

    // So that this works for both const and non-const
    template <typename TKey, typename TLambda> static void VisitKey(TTxn& txn, TKey&& key, TLambda&& lambda)
    { txn.Edit(std::forward<TKey>(key), std::forward<TLambda>(lambda)); }

    template <typename TLambda> static void VisitAll(TTxn& txn, TLambda&& lambda) { txn.VisitAll(std::forward<TLambda>(lambda)); }
};

template <Stencil::ConceptTransactionForPrimitive TTxn, Stencil::ConceptProtocol TProtocol> struct Stencil::SerDes<TTxn, TProtocol>
{
    using ElemType = TransactionTraits<TTxn>::ElemType;

    template <typename TContext> static auto Write(TContext& ctx, TTxn const& txn)
    { Stencil::SerDes<ElemType, TProtocol>::Write(ctx, txn.Elem()); }

    template <typename TContext> static auto Read(TTxn& txn, TContext& ctx)
    {
        auto val = txn.Elem();
        Stencil::SerDes<ElemType, TProtocol>::Read(val, ctx);
        txn.Assign(val);
        // txn.MarkEdited();
    }
};
