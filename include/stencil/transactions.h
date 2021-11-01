#pragma once
#include "optionalprops.h"
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

template <typename TObj, typename _Ts = void> struct Transaction;

template <typename TObj, typename _Ts = void> struct TransactionT
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
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Object>>
{
    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}

    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    private:
    using TTransaction = Transaction<TObj>;

    auto& _GetTransactionObj()
    {
        static_assert(std::is_base_of_v<TransactionT<TObj>, TTransaction>, "Transaction<TObj> must derive TransactionT<TObj>");
        return *static_cast<TTransaction*>(this);
    }

    public:
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
        if (!ReflectionBase::AreEqual(curval, newval)) { _assigntracker.set(static_cast<uint8_t>(field)); }
    }

    template <typename TEnum> void MarkFieldEdited_(TEnum field) { _edittracker.set(static_cast<uint8_t>(field)); }

    std::reference_wrapper<TObj>        _ref;
    std::bitset<TObj::FieldCount() + 1> _assigntracker;
    std::bitset<TObj::FieldCount() + 1> _edittracker;
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::List>>
{
    using ListObjType = typename ReflectionBase::TypeTraits<TObj&>::ListObjType;

    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}
    ~TransactionT() {}

    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    template <typename TArg> void RecordMutation_add_(TArg&) { _changes.push_back({1u, Obj().size()}); }

    void RecordMutation_remove_(size_t index) { _changes.push_back({2u, index}); }
    void RecordMutation_edit_(size_t index) { _changes.push_back({3u, index}); }
    void RecordMutation_assign_(size_t index) { _changes.push_back({0u, index}); }

    void add(ListObjType&& obj)
    {
        RecordMutation_add_(obj);
        Mutators<TObj>::add(Obj(), std::move(obj));
    }

    void remove(size_t index)
    {
        RecordMutation_remove_(index);
        Mutators<TObj>::remove(Obj(), index);
    }

    template <typename TLambda> auto Visit(size_t fieldIndex, TLambda&& lambda)
    {
        Visitor<TObj> visitor(_ref.get());
        visitor.Visit(fieldIndex, [&](auto index, auto& obj) {
            RecordMutation_edit_(index);
            auto subtxnptr = std::make_unique<Transaction<ListObjType>>(obj);
            lambda(index, *subtxnptr);
            // TODO : only do it if there was a change;
            _edited.insert(std::make_pair(index, std::move(subtxnptr)));
        });
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        return Visit(Value(fieldName).convert<size_t>(), std::forward<TLambda>(lambda));
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

    struct _Record
    {
        uint8_t mutationtype;
        size_t  index;
    };

    std::unordered_map<size_t, std::unique_ptr<Transaction<ListObjType>>> _edited;

    std::vector<_Record>         _changes;
    std::reference_wrapper<TObj> _ref;
};

}    // namespace Stencil

template <typename T, typename _Ts> struct Stencil::Transaction : Stencil::TransactionT<T>
{
    Transaction(T& obj) : Stencil::TransactionT<T>(obj) {}
    DELETE_COPY_AND_MOVE(Transaction);
};

template <typename T> struct Stencil::Transaction<T, std::enable_if_t<Value::Supported<T>::value>> : Stencil::TransactionT<T>
{
    Transaction(T& obj) : Stencil::TransactionT<T>(obj) {}
    DELETE_COPY_AND_MOVE(Transaction);
};
