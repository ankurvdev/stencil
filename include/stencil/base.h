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

// Requires typename Categories = std::tuple<...>
template <typename T> struct TypeTraits;

template <typename T> struct TypeTraitsForPrimitive
{
    // Nothing as of yet.
};

template <typename T> struct TypeTraitsForIterable
{
    // Nothing as of yet.
};

template <typename T> struct TypeTraitsForIndexable
{
    // typename Key
};

struct Category
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

    template <typename T> static constexpr bool IsIndexable()
    {
        return _TypeTupleContains<Indexable>(typename TypeTraits<T>::Categories{});
    }
    template <typename T> static constexpr bool IsIterable() { return _TypeTupleContains<Iterable>(typename TypeTraits<T>::Categories{}); }
    template <typename T> static constexpr bool IsPrimitive()
    {
        return _TypeTupleContains<Primitive>(typename TypeTraits<T>::Categories{});
    }
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
concept ConceptIndexable = Category::IsIndexable<T>();

template <typename T>
concept ConceptIterable = Category::IsIterable<T>();

template <typename T>
concept ConceptPrimitive = Category::IsPrimitive<T>();

template <typename T>
concept ConceptIterableOnly = Category::IsIterable<T>();

template <typename T>
concept ConceptIterableNotIndexable = Category::IsIterable<T>() && !Category::IsIndexable<T>();

template <typename T> struct Stencil::TypeTraits<std::shared_ptr<T>>
{
    using Categories = typename Stencil::TypeTraits<T>::Categories;
};

template <ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<std::shared_ptr<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <ConceptIterable T> struct Stencil::TypeTraitsForIterable<std::shared_ptr<T>>
{};

template <ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<std::shared_ptr<T>>
{};

template <typename T, size_t N> struct Stencil::TypeTraits<std::array<T, N>>
{
    using Categories = std::tuple<Stencil::Category::Iterable>;
};

template <typename T> struct Stencil::TypeTraits<std::vector<T>>
{
    using Categories = std::tuple<Stencil::Category::Iterable>;
};

template <size_t N> struct Stencil::TypeTraits<std::array<char, N>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 2>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 4>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraits<uuids::uuid>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<std::string>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<std::wstring>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

}    // namespace Stencil
