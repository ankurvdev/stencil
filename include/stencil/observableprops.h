#pragma once
#include "base.h"
#include "optionalprops.h"

#include <bitset>
namespace Stencil
{

struct Observable
{
};

template <typename T, typename _Ts = void> struct DeltaTracker;

template <typename T> struct GenericDeltaTracker
{
    T const* const _ptr;
    bool const     _changed;

    DELETE_COPY_AND_MOVE(GenericDeltaTracker);

    GenericDeltaTracker(T const* ptr, bool changed) : _ptr(ptr), _changed(changed){};

    using TData = T;
    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    bool IsChanged() const { return _changed; }

    size_t NumFields() const { return 0; }

    bool   OnlyHasDefaultMutator() const { return true; }
    size_t CountFieldsChanged() const { return 0; }

    uint8_t MutatorIndex() const;

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct DeltaTracker<T, std::enable_if_t<Value::Supported<T>::value>> : GenericDeltaTracker<T>
{
    DELETE_COPY_AND_MOVE(DeltaTracker);
    template <typename... TArgs> DeltaTracker(TArgs&&... args) : GenericDeltaTracker<T>(std::forward<TArgs>(args)...) {}
};

template <typename TClock> struct DeltaTracker<std::chrono::time_point<TClock>> : GenericDeltaTracker<std::chrono::time_point<TClock>>
{
    DELETE_COPY_AND_MOVE(DeltaTracker);
    template <typename... TArgs>
    DeltaTracker(TArgs&&... args) : GenericDeltaTracker<std::chrono::time_point<TClock>>(std::forward<TArgs>(args)...)
    {
    }
};

template <typename T, size_t N> struct DeltaTracker<std::array<T, N>>
{
    using TData = std::array<T, N>;

    TData const* const _ptr;
    bool const         _changed{false};

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed) {}

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    bool IsChanged() const { return _changed; }

    size_t NumFields() const { return 0; }

    bool   OnlyHasDefaultMutator() const { return true; }
    size_t CountFieldsChanged() const { return 0; }

    uint8_t MutatorIndex() const
    {
        TODO("Whats a MutatorIndex");
        return 0;
    }

    size_t ListIndex() const
    {
        TODO("Whats a ListIndex");
        return 0;
    }

    DeltaTracker<T> GetSubObjectTracker(size_t index) const { return DeltaTracker<T>(&_ptr->at(index), false); }

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct DeltaTracker<std::vector<T>>
{
    std::vector<T> const* const _ptr;
    bool const                  _changed{false};

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(std::vector<T> const* ptr, bool changed) : _ptr(ptr), _changed(changed) {}

    using TData = std::vector<T>;
    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    bool IsChanged() const { return _changed; }

    size_t NumFields() const { return 0; }

    bool   OnlyHasDefaultMutator() const { return true; }
    size_t CountFieldsChanged() const { return 0; }

    uint8_t MutatorIndex() const
    {
        TODO("Whats a MutatorIndex");
        return 0;
    }
    size_t ListIndex() const
    {
        TODO("Whats a ListIndex");
        return 0;
    }
    DeltaTracker<T> GetSubObjectTracker(size_t index) const { return DeltaTracker<T>(&_ptr->at(index), false); }

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct ObservablePropsT : Observable
{
    std::bitset<32> _changetracker;

    template <typename TFieldEnum> void NotifyChanged(TFieldEnum fieldIndex) { _changetracker.set(static_cast<size_t>(fieldIndex)); }
    template <typename TFieldEnum, typename TFieldType, typename TValueType>
    void NotifyMutation(TFieldEnum /*fieldIndex*/, uint8_t /*mutationIndex*/, TFieldType const& /*fieldType*/, TValueType const& /*val*/)
    {
        TODO("Whats a mutation");
    }

    DeltaTracker<T> Edit()
    {
        _changetracker.reset();
        return DeltaTracker<T>(static_cast<T*>(this), false);
    }

    template <typename TFieldEnum, typename TField>
    static void OnChangeRequested(T& obj, TFieldEnum fieldType, TField const& currentVal, TField const& requestedVal)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<T>, T>)
            {
                if (!obj.IsValid(fieldType))
                {
                    obj.NotifyChanged(fieldType);
                    return;
                }
            }
            if (!ReflectionBase::AreEqual(currentVal, requestedVal))
            {
                obj.NotifyChanged(fieldType);
            }
        }
    }

    template <typename TFieldEnum, typename TField, typename TArgs>
    static void OnMutationRequested(T& obj, TFieldEnum fieldType, uint8_t fieldId, TField const& fieldVal, TArgs const& args)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            obj.NotifyMutation(fieldType, fieldId, fieldVal, args);
        }
    }
};
}    // namespace Stencil