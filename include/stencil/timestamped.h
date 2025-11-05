#pragma once
#include "typetraits.h"

#include <chrono>
#include <tuple>

namespace Stencil
{
using Timestamp = decltype(std::chrono::steady_clock::now());

template <typename T> struct TimestampedT
{
    public:
    static void UpdateTimestamp(T& elem)
    {
        if constexpr (std::is_base_of_v<TimestampedT<T>, T>) { elem.lastmodified = Timestamp::clock::now(); }
    }

    Timestamp lastmodified;
};

}    // namespace Stencil

template <typename T> struct Stencil::TypeTraits<Stencil::TimestampedT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
