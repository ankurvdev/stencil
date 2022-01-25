#include <stencil/stencil.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

struct FixedSize
{
    int64_t  intVal1;
    int16_t  intVal2;
    uint64_t uintVal;
    char     charVal;
    double   dVal;
    float    fVal;
    bool     bVal;

    std::chrono::time_point<std::chrono::system_clock>          tVal1;
    std::chrono::time_point<std::chrono::high_resolution_clock> tVal2;

    std::array<double, 8> darr;
};

struct WithBlobs
{
    std::string              str;
    std::wstring             wstr;
    std::vector<double>      dvec;
    std::vector<std::string> strvec;
};

struct Nested
{
    FixedSize strctf;
    WithBlobs strctb;

    // std::unique_ptr<FixedSize> uptr;
    std::shared_ptr<FixedSize> sptr;

    std::vector<WithBlobs> vec;
};

struct SingleAttributed : Stencil::TimestampedT<SingleAttributed>
{
    Nested obj;
};

struct MultiAttributed : Stencil::TimestampedT<MultiAttributed>, UuidBasedId<MultiAttributed>
{
    SingleAttributed obj;
};

struct TestObj
{
    MultiAttributed obj;
};

template <> struct Stencil::TypeTraits<FixedSize>
{};

template <> struct Stencil::TypeTraits<WithBlobs>
{};

template <> struct Stencil::TypeTraits<Nested>
{};

template <> struct Stencil::TypeTraits<SingleAttributed>
{};

template <> struct Stencil::TypeTraits<MultiAttributed>
{};

template <> struct Stencil::TypeTraits<TestObj>
{};
