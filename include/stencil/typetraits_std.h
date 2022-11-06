#pragma once
#include "typetraits.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
SUPPRESS_WARNINGS_END

template <typename T> struct Stencil::TypeTraits<std::shared_ptr<T>>
{
    using Categories = typename Stencil::TypeTraits<T>::Categories;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<std::shared_ptr<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::ConceptIterable T> struct Stencil::TypeTraitsForIterable<std::shared_ptr<T>>
{
    using ElementType = typename Stencil::TypeTraitsForIndexable<T>::ElementType;
};

template <Stencil::ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<std::shared_ptr<T>>
{};

template <typename T> struct Stencil::TypeTraits<T, typename std::enable_if_t<std::is_arithmetic_v<T>>>
{
    using Categories = std::tuple<Category::Primitive>;
};

template <typename T, size_t N>
requires(!ConceptPrimitives64Bit<std::array<T, N>>) struct Stencil::TypeTraits<std::array<T, N>>
{
    using Categories = std::tuple<Stencil::Category::Iterable>;
};

template <typename T, size_t N> struct Stencil::TypeTraits<std::array<T, N>, std::enable_if_t<ConceptPrimitives64Bit<std::array<T, N>>>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <typename T> struct Stencil::TypeTraits<std::vector<T>>
{
    using Categories = std::tuple<Stencil::Category::Iterable>;
};
template <typename T> struct Stencil::TypeTraitsForIterable<std::vector<T>>
{
    using ElementType = T;
};

template <size_t N> struct Stencil::TypeTraits<std::array<char, N>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <typename T, size_t N> struct Stencil::TypeTraitsForIterable<std::array<T, N>>
{
    using ElementType = T;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 2>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraits<std::array<uint16_t, 4>>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraits<std::string>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<std::wstring>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

// 2 options

// "type" : ....
// "value" : "..."
// or
// "..." : "..."

// How to get a typename. /TypeTraits ?
template <typename... Ts> struct Stencil::TypeTraits<std::variant<Ts...>>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <typename... Ts> struct Stencil::TypeTraitsForIndexable<std::variant<Ts...>>
{
    using Key = size_t;
};

template <typename K, typename V> struct Stencil::TypeTraits<std::unordered_map<K, V>>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};
template <typename K, typename V> struct Stencil::TypeTraitsForIndexable<std::unordered_map<K, V>>
{
    using Key = K;
};
