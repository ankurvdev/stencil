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

template <typename T, typename _Ts = void> struct DeltaTracker
{
    using TData = T;

    DeltaTracker(T* /*ptr*/){};

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    bool    IsChanged() const { return true; }
    size_t  NumFields() const { return 0; }
    bool    OnlyHasDefaultMutator() const { return true; }
    size_t  CountFieldsChanged() const { return 0; }
    uint8_t MutatorIndex() const;

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};
template <typename T, size_t N> struct DeltaTracker<std::array<T, N>>
{
    using TData = std::array<T, N>;
    bool _changed{true};

    TData* const _ptr;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) : _ptr(ptr) {}

    TData& Obj() { return *_ptr; }

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
    using TData = std::vector<T>;

    std::vector<T>* const _ptr;
    bool                  _changed{true};

    struct MutationInfo
    {
        uint8_t              fieldIndex;
        uint8_t              mutationIndex;
        size_t               listIndex;
        std::vector<uint8_t> mutationData;
    };

    std::vector<MutationInfo> mutations;

    template <typename TFieldEnum, typename TFieldType, typename TValueType>

    void NotifyMutation(TFieldEnum fieldIndex, uint8_t mutationIndex, TFieldType const& fieldType, TValueType const& val)
    {
        if (mutationIndex == 1)    // Add
        {
            MutationInfo info{static_cast<uint8_t>(fieldIndex),
                              mutationIndex,
                              _ptr->size(),
                              Mutators<TFieldType>::GenerateMutationData(mutationIndex, fieldType, val)};

            mutations.push_back(std::move(info));
        }
        else if (mutationIndex == 2)    // Remove
        {
            if constexpr (std::is_same_v<TValueType, size_t>)
            {
                MutationInfo info{static_cast<uint8_t>(fieldIndex), mutationIndex, val, {}};
                mutations.push_back(std::move(info));
            }
            else
            {
                throw std::logic_error("Invalid Operation");
            }
        }
    }

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(std::vector<T>* ptr) : _ptr(ptr) {}

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    bool IsChanged() const { return _changed; }

    size_t NumFields() const { return 0; }

    bool   OnlyHasDefaultMutator() const { return true; }
    size_t CountFieldsChanged() const { return 0; }

    size_t MutationCount() const { return mutations.size(); }
    auto&  Mutations() const { return mutations; }

    DeltaTracker<T> GetSubObjectTracker(size_t index) const { return DeltaTracker<T>(&_ptr->at(index)); }

    template <typename TLambda, typename TFieldIndex> void Visit(TFieldIndex /*index*/, TLambda&& /*lambda*/) const
    {
        throw std::logic_error("Illegal Visit");
    }
};

template <typename T> struct ObservablePropsT : Observable
{
    DeltaTracker<T> Edit() { return DeltaTracker<T>(static_cast<T*>(this)); }

    template <typename TFieldEnum, typename TField>
    static void OnChangeRequested(DeltaTracker<T>& obj, TFieldEnum fieldType, TField const& currentVal, TField const& requestedVal)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<T>, T>)
            {
                if (!obj.Obj().IsValid(fieldType))
                {
                    obj.MarkFieldChanged(fieldType);
                    return;
                }
            }
            if (!ReflectionBase::AreEqual(currentVal, requestedVal))
            {

                obj.MarkFieldChanged(fieldType);
            }
        }
    }

    template <typename TFieldEnum, typename TField, typename TArgs>
    static void OnMutationRequested(DeltaTracker<T>& obj, TFieldEnum fieldType, uint8_t fieldId, TField const& fieldVal, TArgs const& args)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            obj.MarkFieldChanged(fieldType);
            obj.Visit(fieldType, [&](auto& subctx) { subctx.NotifyMutation(fieldType, fieldId, fieldVal, args); });
        }
    }
};
}    // namespace Stencil