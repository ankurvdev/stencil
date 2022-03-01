#pragma once
#include "typetraits.h"

#include <string_view>
#include <type_traits>
#include <variant>

namespace Stencil
{
template <typename T> struct EnumTraits;

/*
{
    // Convert To and front string_view
    // Convert To and from index
    // Convert To and from value.
};*/

template <typename T>
concept ConceptEnum = (std::is_enum_v<T> && std::is_default_constructible_v<EnumTraits<T>>);

template <typename... Ts> struct EnumPack : std::variant<Ts...>
{

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
                lhs = EnumPack<Ts...>{EnumTrait::ForIndex(i)};
                return true;
            }
        }
        return false;
    }

    template <typename TContext> static Stencil::EnumPack<Ts...> FromString(TContext const& rhs)
    {
        Stencil::EnumPack<Ts...> lhs;
        bool                     found = (_TryMatch<Ts>(lhs, rhs) || ...);
        if (!found) throw std::invalid_argument("Invalid");
        return lhs;
    }
};

template <typename T> constexpr bool     IsEnumPack                  = false;
template <typename... Ts> constexpr bool IsEnumPack<EnumPack<Ts...>> = true;
template <typename T>
concept ConceptEnumPack = IsEnumPack<T>;

}    // namespace Stencil

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
