#pragma once
#include "typetraits.h"

#include <bitset>
#include <tuple>

namespace Stencil
{
struct OptionalProps
{};
template <typename T> struct OptionalPropsT : OptionalProps
{
    std::bitset<32> _fieldtracker;

    template <typename TField> static void MarkValid([[maybe_unused]] T& elem, [[maybe_unused]] TField fieldIndex)
    {
        if constexpr (std::is_base_of_v<OptionalPropsT<T>, T>) { elem._fieldtracker.set(static_cast<uint32_t>(fieldIndex) - 1); }
        else {}
    }

    template <typename TField> void MarkInvalid(TField fieldIndex) { _fieldtracker.reset(static_cast<uint32_t>(fieldIndex) - 1); }
    template <typename TField> [[nodiscard]] bool IsValid(TField fieldIndex) const
    {
        return _fieldtracker.test(static_cast<uint32_t>(fieldIndex) - 1);
    }

    template <typename TFieldEnum, typename TField>
    static void OnChangeRequested(T& obj, TFieldEnum fieldType, TField const& /* currentVal */, TField const& /* requestedVal */)
    {
        if constexpr (std::is_base_of_v<OptionalPropsT<T>, T>) { obj.MarkValid(fieldType); }
    }

    template <typename TFieldEnum> static bool IsSet(T const& obj, TFieldEnum fieldType)
    {
        if constexpr (std::is_base_of_v<OptionalPropsT<T>, T>) { return obj.IsValid(fieldType); }
        return true;
    }
};

}    // namespace Stencil

template <typename T> struct Stencil::TypeTraits<Stencil::OptionalPropsT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
