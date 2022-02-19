#pragma once

#include <tuple>

namespace Stencil
{

// Requires typename Categories = std::tuple<...>
// TODO : Try again with concepts
// template <typename T> struct TypeTraits;
template <typename T, class Enable = void> struct TypeTraits;

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
    // TODO static_assert to check if T has specialization for TypeTraitsForIndexable

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

    template <typename T, typename Tuple> struct _HasType;

    template <typename T, typename... Us> struct _HasType<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...>
    {};

    template <typename T> static constexpr bool IsIndexable() { return _HasType<Indexable, typename TypeTraits<T>::Categories>::value; }
    template <typename T> static constexpr bool IsIterable() { return _HasType<Iterable, typename TypeTraits<T>::Categories>::value; }
    template <typename T> static constexpr bool IsPrimitive() { return _HasType<Primitive, typename TypeTraits<T>::Categories>::value; }
};

template <typename T>
concept ConceptIndexable = Category::IsIndexable<T>();

template <typename T>
concept ConceptIterable = Category::IsIterable<T>();

template <typename T>
concept ConceptPrimitive = Category::IsPrimitive<T>();

}    // namespace Stencil