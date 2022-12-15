#pragma once
#include "typetraits.h"

#include <chrono>
#include <tuple>

namespace Stencil
{
using Timestamp = decltype(std::chrono::system_clock::now());

template <typename T> struct TimestampedT
{
#ifdef TODO1

    struct InitArgs
    {};

    TimestampedT()
    {
        created;
        lastmodified = created;
    }

    public:
    time_point const& LastModified() const { return lastmodified; }
    time_point const& Created() const { return created; }

    void SetLastModified(time_point const& t) { lastmodified = t; }

    protected:
    time_point created = std::chrono::system_clock::now();
#endif
    public:
    static void UpdateTimestamp(T& elem)
    {
        if constexpr (std::is_base_of_v<TimestampedT<T>, T>) { elem.lastmodified = std::chrono::system_clock::now(); }
    }

    Timestamp lastmodified;
};

}    // namespace Stencil

template <typename T> struct Stencil::TypeTraits<Stencil::TimestampedT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
