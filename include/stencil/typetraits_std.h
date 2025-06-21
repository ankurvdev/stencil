#pragma once
#include "primitives64bit.h"
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

template <Stencil::ConceptIndexable T>
    requires(!Stencil::ConceptNamedTuple<T>)
struct Stencil::TypeTraitsForIndexable<std::shared_ptr<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::ConceptNamedTuple T> struct Stencil::TypeTraitsForIndexable<std::shared_ptr<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
    static bool HasDefaultValueForKey(std::shared_ptr<T> const& obj, Key key)
    {
        return Stencil::TypeTraitsForIndexable<T>::HasDefaultValueForKey(*obj.get(), key);
    }
    template <typename TKey> static bool HasDefaultValueForKey(std::shared_ptr<T> const& obj, TKey key)
    {
        return Stencil::TypeTraitsForIndexable<T>::HasDefaultValueForKey(*obj.get(), key);
    }
};

template <Stencil::ConceptIterable T> struct Stencil::TypeTraitsForIterable<std::shared_ptr<T>>
{
    using ElementType = typename Stencil::TypeTraitsForIndexable<T>::ElementType;
};

template <Stencil::ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<std::shared_ptr<T>>
{};

template <typename T, size_t N>
    requires(!ConceptPrimitives64Bit<std::array<T, N>>)
struct Stencil::TypeTraits<std::array<T, N>>
{
    using Categories = std::tuple<Stencil::Category::Iterable>;
};

template <ConceptPrimitives64Bit T> struct Stencil::TypeTraits<T>
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

template <typename T> struct Stencil::TypeTraits<std::basic_string_view<T>>
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
    using Categories = std::tuple<Stencil::Category::Variant>;
};

template <typename... Ts> struct Stencil::TypeTraitsForVariant<std::variant<Ts...>>
{
    using AlternativeTuple = std::tuple<Ts...>;
};

static_assert(Stencil::ConceptVariant<std::variant<std::string, int>>);

template <typename T>
concept ConceptVariantTType = std::is_base_of<Stencil::VariantT<T>, T>::value;

template <ConceptVariantTType T> struct Stencil::TypeTraitsForVariant<T>
{
    using AlternativeTuple = Stencil::TypeTraitsForVariant<decltype(T{}._variant)>::AlternativeTuple;
};

template <typename K, typename V> struct Stencil::TypeTraits<std::unordered_map<K, V>>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};
template <typename K, typename V> struct Stencil::TypeTraitsForIndexable<std::unordered_map<K, V>>
{
    using Key = K;
};

static_assert(Stencil::ConceptPrimitive<std::array<uint16_t, 2>>);
static_assert(Stencil::Category::IsIterable<std::array<uint16_t, 2>>());
static_assert(Stencil::ConceptIterable<std::array<uint16_t, 2>>);
static_assert(!Stencil::ConceptIndexable<std::array<uint16_t, 2>>);
static_assert(!Stencil::ConceptVariant<std::array<uint16_t, 2>>);
