#pragma once
#include "typetraits.h"

#include <variant>

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

template <typename... Ts> struct Stencil::Visitor<std::variant<Ts...>>
{

    template <size_t N, typename TObj, typename TLambda> static void _SetAndVisit(TObj& obj, size_t const& key, TLambda&& lambda)
    {
        if constexpr (N == sizeof...(Ts)) { throw std::runtime_error("Index out of bounds"); }
        else
        {
            if (N == key)
            {
                std::get<N>(obj) = {};
                lambda(obj);
            }
            else
            {
                _SetAndVisit<N + 1>(obj, key, lambda);
            }
        }
    }
    template <typename T1, typename TLambda> static void VisitKey(T1& obj, size_t const& key, TLambda&& lambda)
    {
        if (obj.index() == key)
        {
            std::visit([&](auto&& arg) { lambda(arg); }, obj);
        }
        else
        {
            _SetAndVisit<0>(obj, key, lambda);
        }
    }
    template <typename T1, typename TLambda> static void VisitAllIndicies(T1& obj, TLambda&& lambda)
    {
        std::visit([&](auto&& arg) { lambda(obj.index(), arg); }, obj);
    }
};
