#pragma once
#include "typetraits.h"

#include <string_view>
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

}    // namespace Stencil

template <typename T> struct Stencil::TypeTraits<T, std::enable_if_t<Stencil::ConceptEnum<T>>>
//template <Stencil::ConceptEnum T> struct Stencil::TypeTraits<T>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};

template <Stencil::ConceptEnum... Ts> struct Stencil::TypeTraits<std::variant<Ts...>>
{
    /* Primitive*/
    using Categories = std::tuple<Category::Primitive>;
};
