#pragma once
#include "Logging.h"
#include "fixedwidthvalue.h"
#include "shared_tree.h"
#include "uuidobject.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <charconv>
#include <climits>
#include <ctype.h>
#include <list>
#include <span>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace Stencil
{
struct StructMarker
{};
struct VariantMarker
{};
struct InterfaceMarker
{};

template <typename T> struct TypeTraits;

struct Type
{
    struct Primitive
    {};

    struct Iterable
    {};

    struct Indexable
    {};

    template <typename U, typename... T> static constexpr bool _TypeTupleContains(std::tuple<T...>)
    {
        return (std::is_same_v<U, T> || ...);
    }

    template <typename T> static constexpr bool IsIndexable() { return _TypeTupleContains<Indexable>(typename TypeTraits<T>::Types{}); }
    template <typename T> static constexpr bool IsIterable() { return _TypeTupleContains<Iterable>(typename TypeTraits<T>::Types{}); }
    template <typename T> static constexpr bool IsPrimitive() { return _TypeTupleContains<Primitive>(typename TypeTraits<T>::Types{}); }
};

/*
enum class Type
{
    Invalid,
    Primitive,    // Fixed size or blobs
    Iterable,     // Multiple values can only iterate on values
    Indexable,    // Indexing of values via Atomics
};
*/

template <typename T>
concept ConceptIndexable = Type::IsIndexable<T>();

template <typename T>
concept ConceptIterableOnly = Type::IsIterable<T>();

template <typename T>
concept ConceptPrimitiveOnly = Type::IsPrimitive<T>();

template <typename T>
concept ConceptIterableNotIndexable = Type::IsIterable<T>() && !Type::IsIndexable<T>();

template <typename T> struct Stencil::TypeTraits<std::shared_ptr<T>>
{
    using Types = typename Stencil::TypeTraits<T>::Types;
};

template <typename T, size_t N> struct Stencil::TypeTraits<std::array<T, N>>
{
    using Types = std::tuple<Stencil::Type::Iterable>;
};

template <typename T> struct Stencil::TypeTraits<std::vector<T>>
{
    using Types = std::tuple<Stencil::Type::Iterable>;
};

template <size_t N> struct Stencil::TypeTraits<std::array<char, N>>
{
    using Types = std::tuple<Stencil::Type::Primitive, Stencil::Type::Iterable>;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 2>>
{
    using Types = std::tuple<Stencil::Type::Primitive, Stencil::Type::Iterable>;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 4>>
{
    using Types = std::tuple<Stencil::Type::Primitive, Stencil::Type::Iterable>;
};

template <> struct Stencil::TypeTraits<uuids::uuid>
{
    using Types = std::tuple<Stencil::Type::Primitive>;
};

template <> struct Stencil::TypeTraits<std::string>
{
    using Types = std::tuple<Stencil::Type::Primitive>;
};

template <> struct Stencil::TypeTraits<std::wstring>
{
    using Types = std::tuple<Stencil::Type::Primitive>;
};

}    // namespace Stencil
