#pragma once
#include "base.h"

#include <string_view>

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

template <Stencil::ConceptEnum T> struct Stencil::TypeTraits<T>
{
    /* Primitive*/
    using Types = std::tuple<Type::Primitive>;
};
