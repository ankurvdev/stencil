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

template <typename TObj, typename TOwner> struct Transaction;

template <typename TObj> struct RootTransactionOwner
{
    RootTransactionOwner(TObj& objIn) : obj(objIn) {}
    ~RootTransactionOwner() = default;
    CLASS_DELETE_COPY_AND_MOVE(RootTransactionOwner);

    struct State
    {};
    struct ContainerState
    {};
    TObj& obj;
};

template <typename T> constexpr bool                  IsTransaction                              = false;
template <typename T, typename TOwner> constexpr bool IsTransaction<Transaction<T, TOwner>>      = true;
template <typename T> constexpr bool                  IsRootTransaction                          = false;
template <typename T> constexpr bool                  IsRootTransaction<RootTransactionOwner<T>> = true;

template <typename T>
concept ConceptTransaction = IsTransaction<T> || IsRootTransaction<T>;

template <typename T> constexpr bool IsPrimitiveTransaction = false;
template <typename T> constexpr bool IsIterableTransaction  = false;
template <typename T> constexpr bool IsIndexableTransaction = false;

template <ConceptPreferPrimitive T, typename TOwner> constexpr bool IsPrimitiveTransaction<Transaction<T, TOwner>> = true;
template <ConceptPreferIterable T, typename TOwner> constexpr bool  IsIterableTransaction<Transaction<T, TOwner>>  = true;
template <ConceptPreferIndexable T, typename TOwner> constexpr bool IsIndexableTransaction<Transaction<T, TOwner>> = true;

template <typename T>
concept ConceptTransactionForPrimitive = IsPrimitiveTransaction<T>;
template <typename T>
concept ConceptTransactionForIterable = IsIterableTransaction<T>;
template <typename T>
concept ConceptTransactionForIndexable = IsIndexableTransaction<T>;

template <typename T> struct TransactionTraits;

template <typename T, typename TOwner> struct TransactionTraits<Transaction<T, TOwner>>
{
    using Obj   = T;
    using Owner = TOwner;
};

template <typename T, typename TOwner> using StructMemberTransactionState = typename Transaction<T, TOwner>::State;

template <typename T> struct StructTransactionT
{
    using TObj   = typename TransactionTraits<T>::Obj;
    using TOwner = typename TransactionTraits<T>::Owner;
    using TEnum  = typename Stencil::TypeTraitsForIndexable<TObj>::Fields;

    private:
    auto& _GetTransactionObj()
    {
        static_assert(std::is_base_of_v<StructTransactionT<T>, T>, "Transaction<TObj> must derive TransactionT<TObj>");
        return *static_cast<T*>(this);
    }

    public:
    bool IsChanged() const { return (_assigntracker | _edittracker).any(); }

    bool IsFieldAssigned(TEnum field) const { return _assigntracker.test(static_cast<uint8_t>(field)); }
    bool IsFieldEdited(TEnum field) const { return _edittracker.test(static_cast<uint8_t>(field)); }
    bool IsFieldChanged(TEnum field) const { return IsFieldAssigned(field) || IsFieldEdited(field); }

    size_t CountFieldsChanged() const { return (_assigntracker | _edittracker).count(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        _GetTransactionObj().VisitAll([&](auto const& key, auto& subtxn, auto& /* obj */) {
            if (IsFieldAssigned(key)) lambda(key, 0, 0, subtxn);
            if (IsFieldEdited(key)) lambda(key, 3, 0, subtxn);
        });
    }

    void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    protected:
    // template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    template <typename TVal> void MarkFieldAssigned_(TEnum field, TVal const& curval, TVal const& newval)
    {
        if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<TObj>, TObj>)
        {
            if (!_GetTransactionObj().Obj().IsValid(field))
            {
                _assigntracker.set(static_cast<uint8_t>(field));
                return;
            }
        }
        if (!Stencil::AreEqual(curval, newval)) { _assigntracker.set(static_cast<uint8_t>(field)); }
    }

    void MarkFieldEdited_(TEnum field) { _edittracker.set(static_cast<uint8_t>(field)); }

    void Flush_()
    {
        if constexpr (std::is_base_of_v<Stencil::TimestampedT<TObj>, TObj>) { T::Obj().UpdateTimestamp_(); }
    }

    std::bitset<64> _assigntracker;    // TODO1
    std::bitset<64> _edittracker;      // TODO1
};

template <typename TTransaction, typename TOwner, typename TObj>
TTransaction CreateTransaction(typename TTransaction::State& state, TOwner& owner, TObj& obj)
{
    return TTransaction(state, owner, obj);
}

}    // namespace Stencil

template <Stencil::ConceptPreferPrimitive T, typename TOwner> struct Stencil::Transaction<T, TOwner>
{
    using TObj                 = T;
    using TOwnerContainerState = typename TOwner::ContainerState;

    struct State
    {
        TOwnerContainerState owner;
        bool                 changed = false;
    };

    Transaction(State& state, TOwner& owner, TObj& obj) : _state(state), _owner(owner), _obj(obj) {}
    ~Transaction() = default;

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    TObj const& Obj() const { return _obj; }

    template <typename TKey, typename TLambda> auto Edit(TKey const& /*key*/, TLambda const& /*lambda*/)
    {
        throw std::logic_error("Obj Not supported on Transaction");
    }

    void MarkEdited() { _state.changed = true; }

    State&  _state;
    TOwner& _owner;
    TObj&   _obj;
};

template <typename TOwner, typename TKey, typename TVal> struct Stencil::Transaction<std::unordered_map<TKey, TVal>, TOwner>
{
    struct ContainerState
    {
        std::remove_cvref_t<TKey> key;
    };

    using TOwnerContainerState = typename TOwner::ContainerState;

    struct State
    {
        TOwnerContainerState owner;
    };

    using TObj                 = std::unordered_map<TKey, TVal>;
    using TThis                = Stencil::Transaction<TObj, TOwner>;
    using TValTransaction      = Stencil::Transaction<TVal, TThis>;
    using TValTransactionState = typename TValTransaction::State;

    Transaction(State& state, TOwner& owner, TObj& obj) : _state(state), _owner(owner), _obj(obj) {}
    ~Transaction() = default;

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    TObj const& Obj() const { return _obj; }
    bool        IsChanged() const { TODO("DoNotCommit"); }

    template <typename TLambda> void Edit(TKey const& key, TLambda&& lambda)
    {
        auto txn = Stencil::CreateTransaction<TValTransaction>(_edit_txn_at(key), *this, _obj[key]);
        lambda(txn);
    }

    template <typename TLambda> auto Assign(TKey const& key, TVal&& val)
    {
        TObj& obj = Ref();
        obj[key]  = std::move(val);
        return _assign_txn_at(key);
    }

    template <typename TLambda> auto Visit(TKey const& key, TLambda&& lambda) { lambda(key, at(key)); }
    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda) {}

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        // for (auto const& removed : _removed_keys) { lambda(nullptr, nullptr, 0, 0, this, Obj()); }
        for (auto& [k, v] : _add_subtxns)
        {
            auto txn = Stencil::CreateTransaction<TValTransaction>(*v.get(), *this, _obj[k]);
            lambda(k, 0, 0, txn);
        }

        for (auto& [k, v] : _edit_subtxns)
        {
            auto txn = Stencil::CreateTransaction<TValTransaction>(*v.get(), *this, _obj[k]);
            lambda(k, 0, 0, txn);
        }

        // lambda(auto const& name, auto const& /* type */, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj)
    }

    private:
    TObj& Ref() { return _obj; }

    auto& _edit_txn_at(TKey const& key)
    {
        {
            auto it = _add_subtxns.find(key);
            if (it != _add_subtxns.end()) { return *it->second.get(); }
        }
        {
            auto it = _edit_subtxns.find(key);
            if (it != _edit_subtxns.end()) { return *it->second.get(); }
        }
        TObj& obj = Ref();
        auto  it  = obj.find(key);
        if (it == obj.end()) { throw std::logic_error("Object not found"); }
        auto txnstate       = std::make_unique<TValTransactionState>();
        txnstate->owner.key = key;
        auto txnptr         = txnstate.get();
        _edit_subtxns[key]  = std::move(txnstate);
        return *txnptr;
    }

    auto& _assign_txn_at(TKey const& key)
    {
        {
            auto it = _edit_subtxns.find(key);
            if (it != _edit_subtxns.end()) { _edit_subtxns.erase(it); }
        }
        {
            auto it = _add_subtxns.find(key);
            if (it != _add_subtxns.end()) { return *it.second.get(); }
        }
        TObj& obj = Ref();
        auto  it  = obj.find(key);
        if (it == obj.end()) { throw std::logic_error("Object not found"); }
        auto txn           = std::make_unique<TValTransaction>(it.second);
        auto txnptr        = txn.get();
        _edit_subtxns[key] = std::move(txn);
        return *txnptr;
    }
    std::unordered_map<TKey, std::unique_ptr<TValTransactionState>> _edit_subtxns;
    std::unordered_map<TKey, std::unique_ptr<TValTransactionState>> _add_subtxns;

    std::vector<TKey> _removed_keys;
    State&            _state;
    TOwner&           _owner;
    TObj&             _obj;
};

template <Stencil::ConceptPreferIterable TObj, typename TOwner> struct Stencil::Transaction<TObj, TOwner>
{

    struct ContainerState
    {
        uint8_t mutationtype;
        size_t  index;
    };

    using TOwnerContainerState = typename TOwner::ContainerState;

    struct State
    {
        TOwnerContainerState owner;
    };

    using TThis                = Stencil::Transaction<TObj, TOwner>;
    using TVal                 = typename TObj::value_type;
    using TValTransaction      = Stencil::Transaction<TVal, TThis>;
    using TValTransactionState = typename TValTransaction::State;

    Transaction(State& state, TOwner& owner, TObj& obj) : _state(state), _owner(owner), _obj(obj) {}
    ~Transaction() = default;

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    TObj const& Obj() const { return _obj; }

    template <typename TArg> void RecordMutation_add_(TArg&) { _changes.push_back(TValTransactionState{.owner{1u, Obj().size()}}); }

    void RecordMutation_remove_(size_t index) { _changes.push_back(TValTransactionState{.owner{2u, index}}); }
    void RecordMutation_edit_(size_t /* index */) {}
    void RecordMutation_assign_(size_t index) { _changes.push_back(TValTransactionState{.owner{1u, Obj().size()}}); }

    template <typename T> void add(T&& obj)
    {
        RecordMutation_add_(obj);
        Stencil::Mutators<TObj>::add(_obj, std::move(obj));
    }

    void Remove(size_t index)
    {
        RecordMutation_remove_(index);
        Stencil::Mutators<TObj>::remove(_obj, index);
    }

    auto edit(size_t index)
    {
        TValTransactionState ptr{};
        // Edit(index, [&](auto /* fieldIndex */, auto& txn) { ptr = &txn; });
        return CreateTransaction<TValTransaction>(ptr, *this, _obj[index]);
    }

    template <typename TLambda> auto Edit(size_t fieldIndex, TLambda&& lambda)
    {
        TObj& obj = _obj;
        Visitor<TObj>::VisitKey(obj, fieldIndex, [&](auto& /*obj*/) {
            RecordMutation_edit_(fieldIndex);
            auto subtxnptr = std::make_unique<TValTransactionState>();
            lambda(fieldIndex, *subtxnptr);
            // TODO : only do it if there was a change;
            _changes.push_back({3u, fieldIndex, std::move(subtxnptr)});
        });
    }

    template <typename TLambda> auto Edit(std::string_view const& fieldName, TLambda&& lambda)
    {
        size_t key;
        SerDes<size_t, ProtocolString>::Read(key, fieldName);
        return Edit(key, lambda);
    }

    void Assign(TObj&& /* obj */) { throw std::logic_error("Self-Assignment not allowed"); }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }
    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        if (_changes.size() == 0) return;

        for (size_t i = 0; i < _changes.size(); i++)
        {
            auto& c     = _changes[i];
            auto  index = c.owner.index;
            for (size_t j = i + 1; j < _changes.size(); j++)
            {
                auto& c1 = _changes[j];
                if (c1.owner.mutationtype == 2u && c1.owner.index < index) { index--; }
                if (c1.owner.mutationtype == 1u && c1.owner.index < index) { index++; }
            }
            // TODO : Fix me. This is horrible
            //  auto& obj = (*_fn(_owner))[index];
            // if (c.txn.get() == nullptr) { c.txn = std::make_unique<TValTransaction>(this, nullptr); }
            auto txn = CreateTransaction<TValTransaction>(c, *this, _obj[index]);
            lambda(c.owner.index, c.owner.mutationtype, 0, txn);
        }
    }

    bool IsChanged() { return _changes.size() > 0; }

    std::vector<TValTransactionState> _changes;
    State&                            _state;
    TOwner&                           _owner;
    TObj&                             _obj;
};

// Transaction Mutators Accessors
template <Stencil::ConceptTransactionForIterable T> struct Stencil::Mutators<T>
{
    using ListObj = typename T::TVal;

    template <typename TVal> static void add(T& txn, TVal&& obj) { txn.add(std::move(obj)); }

    static void remove(T& txn, size_t index) { txn.Remove(index); }
    static auto edit(T& txn, size_t index) { return txn.edit(index); }
};

template <Stencil::ConceptTransaction T> struct Stencil::TypeTraits<T>
{
    using Categories = typename Stencil::TypeTraits<typename TransactionTraits<T>::Obj>::Categories;
};

template <Stencil::ConceptTransactionForIndexable T> struct Stencil::TypeTraitsForIndexable<T>
{
    using Key = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::Obj>::Key;
};

template <Stencil::ConceptTransactionForIterable T> struct Stencil::TypeTraitsForIterable<T>
{};

template <Stencil::ConceptTransactionForPrimitive T> struct Stencil::TypeTraitsForPrimitive<T>
{};

template <Stencil::ConceptTransactionForIterable T> struct Stencil::VisitorForIterable<T>
{
    using ObjType  = typename TransactionTraits<T>::Obj;
    using Iterator = typename Stencil::Visitor<ObjType>::Iterator;

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, T>
    static void IteratorBegin(Iterator& /*it*/, T1& /*obj*/) { TODO("DoNotCommit: Stencil::Visitor<T>::IteratorBegin(it, *obj.get());"); }

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, T>
    static void IteratorMoveNext(Iterator& /*it*/, T1& /*obj*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::IteratorMoveNext(it, *obj.get());");
    }

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, T>
    static bool IteratorValid(Iterator& /*it*/, T1& /*obj*/)
    {
        TODO("DoNotCommit: return Stencil::Visitor<T>::IteratorValid(it, *obj.get());");
    }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, T>
    static void Visit(Iterator& /*it*/, T1& /*obj*/, TLambda&& /*lambda*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::Visit(it, *obj.get(), std::forward<TLambda>(lambda));");
    }
};

template <Stencil::ConceptTransactionForIndexable T> struct Stencil::VisitorForIndexable<T>
{};

template <Stencil::ConceptTransaction T>
struct Stencil::Visitor<T> : Stencil::VisitorT<T>, Stencil::VisitorForIterable<T>, Stencil::VisitorForIndexable<T>
{
    using ObjType = typename TransactionTraits<T>::Obj;

    // So that this works for both const and non-const
    template <typename TKey, typename TLambda> static void VisitKey(T& obj, TKey&& key, TLambda&& lambda)
    {
        obj.Edit(std::forward<TKey>(key), std::forward<TLambda>(lambda));
    }

    template <typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda) { obj.VisitAll(std::forward<TLambda>(lambda)); }
};

template <Stencil::ConceptTransactionForPrimitive T, typename TProtocol> struct Stencil::SerDes<T, TProtocol>
{
    using TObj = typename TransactionTraits<T>::Obj;

    template <typename Context> static auto Write(Context& ctx, T const& txn) { Stencil::SerDes<TObj, TProtocol>::Write(ctx, txn.Obj()); }

    template <typename Context> static auto Read(T& txn, Context& ctx)
    {
        auto val = txn.Obj();
        Stencil::SerDes<TObj, TProtocol>::Read(val, ctx);
        txn.MarkEdited();
    }
};
