#pragma once
#include "enums.h"
#include "shared_string.h"
#include "timestamped.h"
#include "typetraits.h"
#include "typetraits_std.h"
#include "uuidobject.h"

template <typename T> struct Stencil::TypeTraits<shared_stringT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<Stencil::Timestamp>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
