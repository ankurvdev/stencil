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

template <typename TObj, typename TEnable = void> struct Transaction
{
    Transaction(TObj& /*obj*/) {}

    template <typename TLambda> auto Visit(size_t /* fieldIndex */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> auto Visit(std::string_view const& /* fieldName */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }

    TObj const& Obj() const { throw std::logic_error("Obj Not supported on Transaction"); }

    bool IsChanged() const { return false; }
};

template <typename TObj, typename TEnable = void> struct StructTransactionT;

}    // namespace Stencil

template <typename T> struct Stencil::Transaction<T, std::enable_if_t<Stencil::ConceptPreferPrimitive<T>>>
{
    Transaction(T& /*obj*/) {}

    template <typename TKey, typename TLambda> auto Edit(TKey const& /*key*/, TLambda const& /*lambda*/)
    {
        throw std::logic_error("Obj Not supported on Transaction");
    }

    T const& Obj() const { throw std::logic_error("Obj Not supported on Transaction"); }

    bool IsChanged() const { return false; }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
};

template <> struct Stencil::Transaction<shared_string>
{
    Transaction(shared_string& /*obj*/) {}
    bool                 IsChanged() const { return false; }
    shared_string const& Obj() const { throw std::logic_error("Obj Not supported on Transaction"); }

    template <typename TKey, typename TLambda> auto Edit(TKey const& /*key*/, TLambda const& /*lambda*/)
    {
        throw std::logic_error("Obj Not supported on Transaction");
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
};

template <typename TObj> struct Stencil::StructTransactionT<TObj, std::enable_if_t<Stencil::ConceptPreferIndexable<TObj>>>
{
    StructTransactionT(TObj& obj) : _ref(std::ref(obj)) {}

    CLASS_DELETE_COPY_AND_MOVE(StructTransactionT);

    TObj const& Obj() const { return _ref; }

    private:
    using TTransaction = Transaction<TObj>;

    auto& _GetTransactionObj()
    {
        static_assert(std::is_base_of_v<StructTransactionT<TObj>, TTransaction>, "Transaction<TObj> must derive TransactionT<TObj>");
        return *static_cast<TTransaction*>(this);
    }

    public:
    bool IsChanged() const { return (_assigntracker | _edittracker).any(); }

    template <typename TEnum> bool IsFieldAssigned(TEnum field) const { return _assigntracker.test(static_cast<uint8_t>(field)); }
    template <typename TEnum> bool IsFieldEdited(TEnum field) const { return _edittracker.test(static_cast<uint8_t>(field)); }
    template <typename TEnum> bool IsFieldChanged(TEnum field) const { return IsFieldAssigned(field) || IsFieldEdited(field); }

    size_t CountFieldsChanged() const { return (_assigntracker | _edittracker).count(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        _GetTransactionObj().VisitAll([&](auto const& /* name */, auto const& type, auto& subtxn, auto& /* obj */) {
            if (IsFieldAssigned(type)) lambda(type, 0, 0, subtxn);
            if (IsFieldEdited(type)) lambda(type, 3, 0, subtxn);
        });
    }

    template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    protected:
    // template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }
    TObj& StructObj_() { return _ref; }

    template <typename TEnum, typename TVal> void MarkFieldAssigned_(TEnum field, TVal const& curval, TVal const& newval)
    {
        if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<TObj>, TObj>)
        {
            if (!Obj().IsValid(field))
            {
                _assigntracker.set(static_cast<uint8_t>(field));
                return;
            }
        }
        if (!Stencil::AreEqual(curval, newval)) { _assigntracker.set(static_cast<uint8_t>(field)); }
    }

    template <typename TEnum> void MarkFieldEdited_(TEnum field) { _edittracker.set(static_cast<uint8_t>(field)); }

    void Flush_()
    {
        if constexpr (std::is_base_of_v<Stencil::TimestampedT<TObj>, TObj>) { Obj().UpdateTimestamp_(); }
    }

    std::reference_wrapper<TObj> _ref;
    std::bitset<64>              _assigntracker;    // TODO1
    std::bitset<64>              _edittracker;      // TODO1
};

template <typename TKey, typename TVal> struct Stencil::Transaction<std::unordered_map<TKey, TVal>>
{
    using TObj = std::unordered_map<TKey, TVal>;

    Transaction(TObj& obj) : _ref(std::ref(obj)) {}
    TObj const& Obj() const { return _ref; }
    bool        IsChanged() const { TODO("DoNotCommit"); }

    template <typename TLambda> void Edit(TKey const& key, TLambda&& lambda) { lambda(_edit_txn_at(key)); }
    template <typename TLambda> auto Assign(TKey const& key, TVal&& val)
    {
        TObj& obj = _ref;
        obj[key]  = std::move(val);
        return _assign_txn_at(key);
    }

    Stencil::Transaction<TVal>& _edit_txn_at(TKey const& key)
    {
        {
            auto it = _add_subtxns.find(key);
            if (it != _add_subtxns.end()) { return *it->second.get(); }
        }
        {
            auto it = _edit_subtxns.find(key);
            if (it != _edit_subtxns.end()) { return *it->second.get(); }
        }
        TObj& obj = _ref;
        auto  it  = obj.find(key);
        if (it == obj.end()) { throw std::logic_error("Object not found"); }
        auto txn           = std::make_unique<Stencil::Transaction<TVal>>(it->second);
        auto txnptr        = txn.get();
        _edit_subtxns[key] = std::move(txn);
        return *txnptr;
    }

    Stencil::Transaction<TVal>& _assign_txn_at(TKey const& key)
    {
        {
            auto it = _edit_subtxns.find(key);
            if (it != _edit_subtxns.end()) { _edit_subtxns.erase(it); }
        }
        {
            auto it = _add_subtxns.find(key);
            if (it != _add_subtxns.end()) { return *it.second.get(); }
        }
        TObj& obj = _ref;
        auto  it  = obj.find(key);
        if (it == obj.end()) { throw std::logic_error("Object not found"); }
        auto txn           = std::make_unique<Stencil::Transaction<TVal>>(it.second);
        auto txnptr        = txn.get();
        _edit_subtxns[key] = std::move(txn);
        return *txnptr;
    }

    template <typename TLambda> auto Visit(TKey const& key, TLambda&& lambda) { lambda(key, at(key)); }
    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda) {}

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        // for (auto const& removed : _removed_keys) { lambda(nullptr, nullptr, 0, 0, this, Obj()); }
        for (auto& [k, v] : _add_subtxns) { lambda(k, 0, 0, *v.get()); }
        for (auto& [k, v] : _edit_subtxns) { lambda(k, 0, 0, *v.get()); }

        // lambda(auto const& name, auto const& /* type */, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj)
    }

    std::unordered_map<TKey, std::unique_ptr<Stencil::Transaction<TVal>>> _edit_subtxns;
    std::unordered_map<TKey, std::unique_ptr<Stencil::Transaction<TVal>>> _add_subtxns;
    std::vector<TKey>                                                     _removed_keys;
    std::reference_wrapper<TObj>                                          _ref;
};

template <typename TObj> struct Stencil::Transaction<TObj, std::enable_if_t<Stencil::ConceptPreferIterable<TObj>>>
{
    using ListObjType = typename TObj::value_type;

    Transaction(TObj& obj) : _ref(std::ref(obj)) {}
    ~Transaction() {}

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    TObj const& Obj() const { return _ref; }

    template <typename TArg> void RecordMutation_add_(TArg&) { _changes.push_back({1u, _ref.get().size(), nullptr}); }

    void RecordMutation_remove_(size_t index) { _changes.push_back({2u, index, nullptr}); }
    void RecordMutation_edit_(size_t /* index */) {}
    void RecordMutation_assign_(size_t index) { _changes.push_back({0u, index, nullptr}); }

    template <typename T> void add(T&& obj)
    {
        RecordMutation_add_(obj);
        Stencil::Mutators<TObj>::add(_ref, std::move(obj));
    }

    void Remove(size_t index)
    {
        RecordMutation_remove_(index);
        Stencil::Mutators<TObj>::remove(_ref, index);
    }

    auto& edit(size_t index)
    {
        Transaction<ListObjType>* ptr = nullptr;
        Edit(index, [&](auto /* fieldIndex */, auto& txn) { ptr = &txn; });
        return *ptr;
    }

    template <typename TLambda> auto Edit(size_t fieldIndex, TLambda&& lambda)
    {
        Visitor<TObj>::VisitKey(_ref.get(), fieldIndex, [&](auto& obj) {
            RecordMutation_edit_(fieldIndex);
            auto subtxnptr = std::make_unique<Transaction<std::remove_cvref_t<decltype(obj)>>>(obj);
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
            auto  index = c.index;
            for (size_t j = i + 1; j < _changes.size(); j++)
            {
                auto& c1 = _changes[j];
                if (c1.mutationtype == 2u && c1.index < index) { index--; }
                if (c1.mutationtype == 1u && c1.index < index) { index++; }
            }
            // TODO : Fix me. This is horrible
            auto& obj = _ref.get()[index];
            if (c.txn.get() == nullptr) { c.txn = std::make_unique<Transaction<ListObjType>>(obj); }
            lambda(c.index, c.mutationtype, 0, *c.txn.get());
        }
    }

    bool IsChanged() { return _changes.size() > 0; }

    struct _Record
    {
        uint8_t mutationtype;
        size_t  index;

        std::unique_ptr<Transaction<ListObjType>> txn;
    };

    std::vector<_Record>         _changes;
    std::reference_wrapper<TObj> _ref;
};

// Transaction Mutators Accessors
template <typename T> struct Stencil::Mutators<Stencil::Transaction<T>, std::enable_if_t<Stencil::ConceptIterable<T>>>
{
    template <typename TVal> static void add(Stencil::Transaction<T>& txn, TVal&& obj) { txn.add(std::move(obj)); }

    static void  remove(Stencil::Transaction<T>& txn, size_t index) { txn.Remove(index); }
    static auto& edit(Stencil::Transaction<T>& txn, size_t index) { return txn.edit(index); }
};

template <typename T> struct Stencil::TypeTraits<Stencil::Transaction<T>>
{
    using Categories = typename Stencil::TypeTraits<T>::Categories;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<Stencil::Transaction<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::ConceptIterable T> struct Stencil::TypeTraitsForIterable<Stencil::Transaction<T>>
{};

template <Stencil::ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<Stencil::Transaction<T>>
{};

template <Stencil::ConceptIterable T> struct Stencil::VisitorForIterable<Stencil::Transaction<T>>
{
    using Iterator = typename Stencil::Visitor<T>::Iterator;
    using ThisType = Stencil::Transaction<T>;

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorBegin(Iterator& /*it*/, T1& /*obj*/) { TODO("DoNotCommit: Stencil::Visitor<T>::IteratorBegin(it, *obj.get());"); }

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorMoveNext(Iterator& /*it*/, T1& /*obj*/) { TODO("DoNotCommit: Stencil::Visitor<T>::IteratorMoveNext(it, *obj.get());"); }

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static bool IteratorValid(Iterator& /*it*/, T1& /*obj*/) { TODO("DoNotCommit: return Stencil::Visitor<T>::IteratorValid(it, *obj.get());"); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void Visit(Iterator& /*it*/, T1& /*obj*/, TLambda&& /*lambda*/)
    {
        TODO("DoNotCommit: Stencil::Visitor<T>::Visit(it, *obj.get(), std::forward<TLambda>(lambda));");
    }
};

template <Stencil::ConceptIndexable T> struct Stencil::VisitorForIndexable<Stencil::Transaction<T>>
{
    using ThisType = Stencil::Transaction<T>;
};

template <typename T>
struct Stencil::Visitor<Stencil::Transaction<T>> : Stencil::VisitorT<Stencil::Transaction<T>>,
                                                   Stencil::VisitorForIterable<Stencil::Transaction<T>>,
                                                   Stencil::VisitorForIndexable<Stencil::Transaction<T>>
{
    using ThisType = Stencil::Transaction<T>;

    // So that this works for both const and non-const
    template <typename TKey, typename TLambda> static void VisitKey(ThisType& obj, TKey&& key, TLambda&& lambda)
    {
        obj.Edit(std::forward<TKey>(key), std::forward<TLambda>(lambda));
    }

    template <typename TLambda> static void VisitAllIndicies(ThisType& obj, TLambda&& lambda)
    {
        obj.VisitAll(std::forward<TLambda>(lambda));
    }
};

template <ConceptPrimitives64Bit T, typename TProtocol> struct Stencil::SerDes<Stencil::Transaction<T>, TProtocol>
{
    template <typename Context> static auto Write(Context& ctx, Stencil::Transaction<T> const& txn)
    {
        Stencil::SerDes<T, TProtocol>::Write(ctx, txn.Obj());
    }

    template <typename Context> static auto Read(Stencil::Transaction<T>& /*obj*/, Context& /*ctx*/)
    {
        TODO("DoNotCommit");
        // TODO: DoNotCommit
    }
};

template <typename TProtocol> struct Stencil::SerDes<Stencil::Transaction<shared_string>, TProtocol>
{
    template <typename Context> static auto Write(Context& /*ctx*/, Stencil::Transaction<shared_string> const& /*txn*/)
    {
        TODO("DoNotCommit");
    }

    template <typename Context> static auto Read(Stencil::Transaction<shared_string>& /*obj*/, Context& /*ctx*/)
    {
        TODO("DoNotCommit");
        // TODO: DoNotCommit
    }
};