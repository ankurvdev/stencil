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
concept ConceptEnum = requires(T t)
{
    typename std::is_enum<T>::type;
    typename EnumTraits<T>;
};

}    // namespace Stencil

template <Stencil::ConceptEnum T> struct Stencil::TypeTraits<T>
{
    /* Primitive*/
    constexpr static bool IsIndexable() { return false; }
    constexpr static bool IsIterable() { return false; }
    constexpr static bool IsPrimitive() { return true; }
};

template <Stencil::ConceptEnum TOut, typename T> struct Stencil::Assign<TOut, std::basic_string_view<T>>
{
    using EnumTrait = typename EnumTraits<TOut>;
    void operator()(TOut& lhs, std::basic_string_view<T> const& rhs)
    {
        for (size_t i = 0; i < std::size(EnumTrait::Names); i++)
        {
            auto name = EnumTrait::Names[i];
            if (std::equal(std::begin(rhs), std::end(rhs), std::begin(name), std::end(name), [](auto l, auto r) {
                    return std::tolower(l) == std::tolower(r);
                }))
            {
                lhs = EnumTrait::ForIndex(i);
                return;
            }
        }
        throw std::invalid_argument("Invalid");
    }
};

template <Stencil::ConceptEnum TOut> struct Stencil::Assign<TOut, Value>
{
    using EnumTrait = typename EnumTraits<TOut>;
    void operator()(TOut& lhs, Value const& rhs) { lhs = EnumTrait::ForIndex(rhs); }
};