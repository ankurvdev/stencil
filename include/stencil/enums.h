#pragma once
#include "CommonMacros.h"
#include "typetraits.h"
#include <algorithm>

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <variant>
SUPPRESS_WARNINGS_END

namespace Stencil
{
template <typename T> struct EnumTraits;

template <typename T>
concept ConceptEnum = (std::is_enum_v<T> && std::is_default_constructible_v<EnumTraits<T>>);

template <typename... Ts> struct EnumPack
{
    using PackType                   = EnumPack<Ts...>;
    static constexpr size_t PackSize = sizeof...(Ts);

    template <typename T1, typename T2, typename TCtx> static bool TryMatch(T2& lhs, TCtx const& rhs)
    {
        using EnumTrait = Stencil::EnumTraits<T1>;
        for (size_t i = 0; i < std::size(EnumTrait::Names); i++)
        {
            auto name = EnumTrait::Names[i];
            if (std::equal(std::begin(rhs), std::end(rhs), std::begin(name), std::end(name), [](auto l, auto r) {
                    return std::tolower(l) == std::tolower(r);
                }))
            {
                lhs.variants = EnumTrait::ForIndex(i);
                return true;
            }
        }
        return false;
    }

    template <typename TContext> static PackType CastFromString(TContext const& rhs)
    {
        Stencil::EnumPack<Ts...> lhs;
        bool                     found = (TryMatch<Ts>(lhs, rhs) || ...);
        if (!found) throw std::invalid_argument("Invalid");
        return lhs;
    }

    static std::string_view CastToString(PackType const& rhs)
    {
        std::string_view out = "Invalid";
        std::visit([&](auto arg) { out = Stencil::EnumTraits<decltype(arg)>::Names[static_cast<size_t>(arg)]; }, rhs.variants);
        return out;
    }

    template <size_t TIndex = 0> static PackType CastFromInt(size_t const idx, uint32_t val)
    {
        if constexpr (TIndex == PackSize) { throw std::out_of_range("Exceeded maximum tuple range"); }
        else
        {
            if (TIndex == idx) { return PackType{static_cast<std::tuple_element_t<TIndex, std::tuple<Ts...>>>(val)}; }
            return CastFromInt<TIndex + 1>(idx, val);
        }
    }

    static PackType CastFromInt(uint32_t val)
    {
        auto index = static_cast<uint8_t>(val & 0xffu);    // NOLINT
        val        = val >> 8u;
        return CastFromInt(index, val);
    }

    static uint32_t CastToInt(PackType const& rhs)
    {
        uint32_t out  = 0;
        auto     type = static_cast<uint8_t>(rhs.variants.index());
        std::visit([&](auto&& arg) { out = static_cast<uint32_t>(arg); }, rhs.variants);
        return (out << 8u | type);
    }

    std::variant<Ts...> variants;
};

template <typename T> constexpr bool     IsEnumPack                  = false;
template <typename... Ts> constexpr bool IsEnumPack<EnumPack<Ts...>> = true;
template <typename T>
concept ConceptEnumPack = IsEnumPack<T>;

template <typename T> uint32_t        EnumPackCastToInt(T const& rhs);
template <ConceptEnumPack T> uint32_t EnumPackCastToInt(T const& rhs)
{ return T::CastToInt(rhs); }
}    // namespace Stencil

namespace std
{
template <typename T, typename... Ts> constexpr bool holds_alternative(Stencil::EnumPack<Ts...> const& pack) noexcept    // NOLINT
{ return std::holds_alternative<T>(pack.variants); }

template <typename T, typename... Ts> constexpr auto get(Stencil::EnumPack<Ts...> const& pack) noexcept    // NOLINT
{ return std::get<T>(pack.variants); }
}    // namespace std

template <Stencil::ConceptEnum T> struct Stencil::TypeTraits<T>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};

template <typename... Ts> struct Stencil::TypeTraits<Stencil::EnumPack<Ts...>>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};
