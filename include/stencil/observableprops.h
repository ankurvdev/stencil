#pragma once
#include "mutatorsaccessors.h"
#include "optionalprops.h"

#include <bitset>
namespace Stencil
{

struct Observable
{
};

template <typename T> struct ObservablePropsT : Observable
{
#if 0
    template <typename TFieldEnum, typename TField>
    static void OnChangeRequested(Trab<T>& obj, TFieldEnum fieldType, TField const& currentVal, TField const& requestedVal)
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
    static void OnMutationRequested(Transaction<T>& obj, TFieldEnum fieldType, uint8_t fieldId, TField const& fieldVal, TArgs const& args)
    {
        if constexpr (std::is_base_of_v<ObservablePropsT<T>, T>)
        {
            obj.MarkFieldChanged(fieldType);
            obj.Visit(fieldType, [&](auto& subctx) { subctx.NotifyMutation(fieldType, fieldId, fieldVal, args); });
        }
    }
#endif
};
}    // namespace Stencil
