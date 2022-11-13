#pragma once
#include "typetraits.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include <cctype>
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

    template <typename T1, typename T2, typename TCtx> static bool _TryMatch(T2& lhs, TCtx const& rhs)
    {
        using EnumTrait = typename Stencil::EnumTraits<T1>;
        for (size_t i = 0; i < std::size(EnumTrait::Names); i++)
        {
            auto name = EnumTrait::Names[i];
            if (std::equal(std::begin(rhs), std::end(rhs), std::begin(name), std::end(name), [](auto l, auto r) {
                    return std::tolower(l) == std::tolower(r);
                }))
            {
                lhs._variant = EnumTrait::ForIndex(i);
                return true;
            }
        }
        return false;
    }

    template <typename TContext> static PackType CastFromString(TContext const& rhs)
    {
        Stencil::EnumPack<Ts...> lhs;
        bool                     found = (_TryMatch<Ts>(lhs, rhs) || ...);
        if (!found) throw std::invalid_argument("Invalid");
        return lhs;
    }

    static std::string_view CastToString(PackType const& rhs)
    {
        std::string_view out = "Invalid";
        std::visit([&](auto arg) { out = Stencil::EnumTraits<decltype(arg)>::Names[static_cast<size_t>(arg)]; }, rhs._variant);
        return out;
    }

    template <size_t TIndex = 0> static PackType _CastFromInt(const size_t idx, uint32_t val)
    {
        if constexpr (TIndex == PackSize) { throw std::out_of_range("Exceeded maximum tuple range"); }
        else
        {
            if (TIndex == idx) { return PackType{static_cast<std::tuple_element_t<TIndex, std::tuple<Ts...>>>(val)}; }
            return _CastFromInt<TIndex + 1>(idx, val);
        }
    }

    static PackType CastFromInt(uint32_t val)
    {
        uint8_t index = static_cast<uint8_t>(val & 0xff);
        val           = val >> 8;
        return _CastFromInt(index, val);
    }

    static uint32_t CastToInt(PackType const& rhs)
    {
        uint32_t out;
        uint8_t  type = static_cast<uint8_t>(rhs._variant.index());
        std::visit([&](auto&& arg) { out = static_cast<uint32_t>(arg); }, rhs._variant);
        return uint32_t{out << 8 | type};
    }

    std::variant<Ts...> _variant;
};

template <typename T> constexpr bool     IsEnumPack                  = false;
template <typename... Ts> constexpr bool IsEnumPack<EnumPack<Ts...>> = true;
template <typename T>
concept ConceptEnumPack = IsEnumPack<T>;

}    // namespace Stencil

namespace std
{
template <typename T, typename... Ts> constexpr bool holds_alternative(const Stencil::EnumPack<Ts...>& pack) noexcept
{
    return std::holds_alternative<T>(pack._variant);
}

template <typename T, typename... Ts> constexpr auto get(const Stencil::EnumPack<Ts...>& pack) noexcept
{
    return std::get<T>(pack._variant);
}
}    // namespace std

template <typename T> struct Stencil::TypeTraits<T, std::enable_if_t<Stencil::ConceptEnum<T>>>
// template <Stencil::ConceptEnum T> struct Stencil::TypeTraits<T>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};

template <typename... Ts> struct Stencil::TypeTraits<Stencil::EnumPack<Ts...>>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};
