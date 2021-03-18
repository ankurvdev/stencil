#pragma once
#include "base.h"
#include "mutatorsaccessors.h"
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
    T* const   _ptr;
    bool const _changed{false};

    DELETE_COPY_AND_MOVE(GenericDeltaTracker);

    GenericDeltaTracker(T* ptr) : _ptr(ptr){};

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
    bool _changed{false};

    TData* const _ptr;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) : _ptr(ptr) {}

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

    DeltaTracker<T> GetSubObjectTracker(size_t index) const { return DeltaTracker<T>(&_ptr->at(index)); }

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct DeltaTracker<std::vector<T>>
{
    std::vector<T>* const _ptr;
    bool                  _changed{false};

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(std::vector<T>* ptr) : _ptr(ptr) {}

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
    DeltaTracker<T> GetSubObjectTracker(size_t index) const { return DeltaTracker<T>(&_ptr->at(index)); }

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct ObservablePropsT : Observable
{
    struct MutationInfo
    {
        uint8_t              fieldIndex;
        uint8_t              mutationIndex;
        std::vector<uint8_t> mutationData;
    };

    std::vector<MutationInfo> mutations;

    template <typename TFieldEnum, typename TFieldType, typename TValueType>

    void NotifyMutation(TFieldEnum fieldIndex, uint8_t mutationIndex, TFieldType const& fieldType, TValueType const& val)
    {
        //NotifyChanged(fieldIndex);
        MutationInfo info{
            static_cast<uint8_t>(fieldIndex), mutationIndex, Mutators<TFieldType>::GenerateMutationData(mutationIndex, fieldType, val)};
        mutations.push_back(std::move(info));
    }

    DeltaTracker<T> Edit() { return DeltaTracker<T>(static_cast<T*>(this)); }

    template <typename TFieldEnum, typename TField>
    static void OnChangeRequested(T& obj, TFieldEnum fieldType, TField const& currentVal, TField const& requestedVal)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<T>, T>)
            {
                if (!obj.IsValid(fieldType))
                {
                    //                    obj.NotifyChanged(fieldType);
                    return;
                }
            }
            if (!ReflectionBase::AreEqual(currentVal, requestedVal))
            {
                //              obj.NotifyChanged(fieldType);
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