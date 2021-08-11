#include <chrono>

namespace Stencil
{

template <typename T> struct TimestampedT
{
    using time_point = decltype(std::chrono::system_clock::now());
    struct InitArgs
    {};

    TimestampedT()
    {
        created      = std::chrono::system_clock::now();
        lastmodified = created;
    }

    public:
    time_point const& LastModified() const { return lastmodified; }
    time_point const& Created() const { return created; }

    protected:
    void MarkModified() { lastmodified = std::chrono::system_clock::now(); }
    void SetLastModified(time_point const& t) { lastmodified = t; }

    time_point created;
    time_point lastmodified;
};

}    // namespace Stencil
