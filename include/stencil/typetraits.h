#pragma once
#include <tuple>
#include <type_traits>

namespace Stencil
{

// Requires typename Categories = std::tuple<...>
// TODO : Try again with concepts
// template <typename T> struct TypeTraits;
template <typename T> struct TypeTraits;

template <typename T> struct TypeTraitsForPrimitive
{};
template <typename T> struct TypeTraitsForIterable
{};
template <typename T> struct TypeTraitsForIndexable
{};
template <typename T> struct TypeTraitsForVariant
{};

struct Category
{
    struct Primitive
    {};

    struct Iterable
    {};

    struct Indexable
    {};

    struct Variant
    {};

    template <typename T, typename Tuple> struct _HasType;

    template <typename T, typename... Us> struct _HasType<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...>
    {};

    template <typename T> static constexpr bool IsIndexable() { return _HasType<Indexable, typename TypeTraits<T>::Categories>::value; }
    template <typename T> static constexpr bool IsIterable() { return _HasType<Iterable, typename TypeTraits<T>::Categories>::value; }
    template <typename T> static constexpr bool IsPrimitive() { return _HasType<Primitive, typename TypeTraits<T>::Categories>::value; }
    template <typename T> static constexpr bool IsVariant() { return _HasType<Variant, typename TypeTraits<T>::Categories>::value; }

    template <typename T> static constexpr bool PreferIndexable()
    {
        return std::is_same_v<Category::Indexable, std::tuple_element_t<0, typename TypeTraits<T>::Categories>>;
    }

    template <typename T> static constexpr bool PreferIterable()
    {
        return std::is_same_v<Category::Iterable, std::tuple_element_t<0, typename TypeTraits<T>::Categories>>;
    }

    template <typename T> static constexpr bool PreferPrimitive()
    {
        return std::is_same_v<Category::Primitive, std::tuple_element_t<0, typename TypeTraits<T>::Categories>>;
    }

    template <typename T> static constexpr bool PreferVariant()
    {
        return std::is_same_v<Category::Variant, std::tuple_element_t<0, typename TypeTraits<T>::Categories>>;
    }
};

template <typename T>
concept ConceptIndexable = requires {
    requires Category::IsIndexable<T>();
    typename TypeTraitsForIndexable<T>::Key;
};

template <typename T>
concept ConceptNamedTuple = requires(T t, typename TypeTraitsForIndexable<T>::Key k) {
    requires ConceptIndexable<T>;
    { TypeTraitsForIndexable<T>::HasDefaultValueForKey(t, k) } -> std::same_as<bool>;
};

template <typename T>
concept ConceptIterable = /* Category::IsIterable<T>() &&  */ requires {
    requires Category::IsIterable<T>();
    typename TypeTraitsForIterable<T>::ElementType;
};

template <typename T>
concept ConceptPrimitive = requires {
    requires Category::IsPrimitive<T>();
    typename TypeTraitsForPrimitive<T>;
};

template <typename T>
concept ConceptVariant = requires {
    Category::IsVariant<T>();
    typename TypeTraitsForVariant<T>::AlternativeTuple;
};

template <typename T>
concept ConceptPreferIndexable = Category::PreferIndexable<T>();

template <typename T>
concept ConceptPreferIterable = Category::PreferIterable<T>();

template <typename T>
concept ConceptPreferPrimitive = Category::PreferPrimitive<T>();

template <typename T>
concept ConceptPreferVariant = Category::PreferVariant<T>();

template <typename T> struct StructT
{};

template <typename T> struct VariantT
{};

template <typename TAttr, typename TObj> struct Attribute;

namespace AttributeType
{
struct Description
{};
struct Name1
{};
};    // namespace AttributeType
}    // namespace Stencil
