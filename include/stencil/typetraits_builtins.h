#pragma once
#include "shared_string.h"
#include "timestamped.h"
#include "typetraits.h"

template <typename T> struct Stencil::TypeTraits<shared_stringT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<Stencil::Timestamp>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

static_assert(Stencil::ConceptPrimitive<shared_stringT<char>>);
static_assert(!Stencil::ConceptIndexable<shared_stringT<char>>);
static_assert(!Stencil::ConceptIterable<shared_stringT<char>>);
static_assert(!Stencil::ConceptVariant<shared_stringT<char>>);
