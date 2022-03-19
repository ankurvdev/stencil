#pragma once
#include <chrono>

namespace Stencil
{
using Timestamp = decltype(std::chrono::system_clock::now());

template <typename T> struct TimestampedT
{
#if 0

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

    void UpdateTimestamp_() { lastmodified = std::chrono::system_clock::now(); }
    void SetLastModified(time_point const& t) { lastmodified = t; }

    protected:
    time_point created = std::chrono::system_clock::now();
#endif
    public:
    Timestamp lastmodified;
};

}    // namespace Stencil
