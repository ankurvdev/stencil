#include "typetraits.h"

#include <memory>
#include <array>
#include <vector>
#include <string>

template <typename T> struct Stencil::TypeTraits<std::shared_ptr<T>>
{
    using Categories = typename Stencil::TypeTraits<T>::Categories;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<std::shared_ptr<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::ConceptIterable T> struct Stencil::TypeTraitsForIterable<std::shared_ptr<T>>
{};

template <Stencil::ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<std::shared_ptr<T>>
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

template <> struct Stencil::TypeTraits<std::string>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<std::wstring>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
