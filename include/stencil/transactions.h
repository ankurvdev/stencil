#pragma once
#include "comparator.h"
#include "mutatorsaccessors.h"
#include "optionalprops.h"
#include "typetraits_shared_string.h"
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

template <typename TObj, typename TEnable = void> struct TransactionT
{
    TransactionT(TObj& /*obj*/) {}

    template <typename TLambda> auto Visit(size_t /* fieldIndex */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> auto Visit(std::string_view const& /* fieldName */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }

    TObj& Obj() { throw std::logic_error("Obj Not supported on Transaction"); }

    void Flush_() const { throw std::logic_error("Flush not supported on transaction"); }

    void Flush() const {}
    bool IsChanged() const { return false; }
};

template <typename TObj, typename TEnable = void> struct Transaction : Stencil::TransactionT<TObj>
{
    Transaction(TObj& obj) : Stencil::TransactionT<TObj>(obj) {}
    CLASS_DELETE_COPY_AND_MOVE(Transaction);
};

}    // namespace Stencil

template <typename T> struct Stencil::Transaction<T, std::enable_if_t<Stencil::ConceptPrimitive<T>>> : Stencil::TransactionT<T>
{
    Transaction(T& obj) : Stencil::TransactionT<T>(obj) {}
    void Flush() const {}
    bool IsChanged() const { return false; }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
};

template <> struct Stencil::Transaction<shared_string> : Stencil::TransactionT<shared_string>
{
    Transaction(shared_string& obj) : Stencil::TransactionT<shared_string>(obj) {}
    void Flush() const {}
    bool IsChanged() const { return false; }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);
};

template <typename TObj> struct Stencil::TransactionT<TObj, std::enable_if_t<Stencil::ConceptIndexable<TObj>>>
{
    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}

    CLASS_DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    private:
    using TTransaction = Transaction<TObj>;

    auto& _GetTransactionObj()
    {
        static_assert(std::is_base_of_v<TransactionT<TObj>, TTransaction>, "Transaction<TObj> must derive TransactionT<TObj>");
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
        _GetTransactionObj().VisitAll([&](auto const& name, auto const& type, auto& subtxn, auto& obj) {
            if (IsFieldAssigned(type)) lambda(name, type, 0, 0, subtxn, obj);
            if (IsFieldEdited(type)) lambda(name, type, 3, 0, subtxn, obj);
        });
    }

    template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    protected:
    // template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

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

template <typename ListObjType> struct Stencil::TransactionT<std::vector<ListObjType>>
{
    using TObj = std::vector<ListObjType>;

    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}
    ~TransactionT() {}

    CLASS_DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    template <typename TArg> void RecordMutation_add_(TArg&) { _changes.push_back({1u, Obj().size()}); }

    void RecordMutation_remove_(size_t index) { _changes.push_back({2u, index}); }
    void RecordMutation_edit_(size_t index) { _changes.push_back({3u, index}); }
    void RecordMutation_assign_(size_t index) { _changes.push_back({0u, index}); }

    template <typename T> void add(T&& obj)
    {
        RecordMutation_add_(obj);
        Stencil::Mutators<TObj>::add(Obj(), std::move(obj));
    }

    void remove(size_t index)
    {
        RecordMutation_remove_(index);
        Stencil::Mutators<TObj>::remove(Obj(), index);
    }

    auto& edit(size_t index)
    {
        Transaction<ListObjType>* ptr = nullptr;
        Visit(index, [&](auto /* fieldIndex */, auto& txn) { ptr = &txn; });
        return *ptr;
    }

    template <typename TLambda> auto Visit(size_t fieldIndex, TLambda&& lambda)
    {
        Visitor<TObj>::VisitKey(Obj(), fieldIndex, [&](auto& obj) {
            RecordMutation_edit_(fieldIndex);
            auto subtxnptr = std::make_unique<Transaction<std::remove_cvref_t<decltype(obj)>>>(obj);
            lambda(fieldIndex, *subtxnptr);
            // TODO : only do it if there was a change;
            _edited.insert(std::make_pair(fieldIndex, std::move(subtxnptr)));
        });
    }

    template <typename TLambda> auto Visit(std::string_view const& /* fieldName */, TLambda&& /* lambda */)
    {
        TODO("TODO1");
        // return Visit(Value(fieldName).convert<size_t>(), std::forward<TLambda>(lambda));
    }

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
            auto& obj = Obj()[index];
            auto  it  = _edited.find(index);
            if (it == _edited.end())
            {
                it = _edited.insert(std::make_pair(c.index, std::make_unique<Transaction<ListObjType>>(obj))).first;
            }
            lambda(nullptr, nullptr, c.mutationtype, c.index, *it->second, obj);
        }
    }

    void Flush() {}
    bool IsChanged() { return _changes.size() > 0; }

    struct _Record
    {
        uint8_t mutationtype;
        size_t  index;
    };

    std::unordered_map<size_t, std::unique_ptr<Transaction<ListObjType>>> _edited;

    std::vector<_Record>         _changes;
    std::reference_wrapper<TObj> _ref;
};

// Transaction Mutators Accessors
template <typename T> struct Stencil::Mutators<Stencil::Transaction<T>, std::enable_if_t<Stencil::ConceptIterable<T>>>
{
    template <typename TVal> static void add(Stencil::Transaction<T>& txn, TVal&& obj) { txn.add(std::move(obj)); }

    static void  remove(Stencil::Transaction<T>& txn, size_t index) { txn.remove(index); }
    static auto& edit(Stencil::Transaction<T>& txn, size_t index) { return txn.edit(index); }
};
