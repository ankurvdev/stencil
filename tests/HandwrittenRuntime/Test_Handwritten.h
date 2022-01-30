#include <stencil/stencil.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

struct FixedSize
{
    int64_t  f1;
    int16_t  f2;
    uint64_t f3;
    char     f4;
    double   f5;
    float    f6;
    bool     f7;

    std::chrono::time_point<std::chrono::system_clock>          f8;
    std::chrono::time_point<std::chrono::high_resolution_clock> f9;

    std::array<double, 8>   f10;    // Iterable only
    std::array<char, 8>     f11;    // Iterable, blob (string) and Value (64-bit)
    std::array<uint16_t, 4> f12;    // Iterable and by value (64 bit)

    uuids::uuid f13;    // iterable , blob (string) or FixedSize buffer
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
    std::vector<double>    vec2;
};

struct MultiAttributed : Stencil::TimestampedT<MultiAttributed>, UuidBasedId<MultiAttributed>
{
    Nested f1;
};

struct TestObj
{
    MultiAttributed f1;
};

template <> struct Stencil::TypeTraits<FixedSize>
{};

template <> struct Stencil::TypeTraits<WithBlobs>
{};

template <> struct Stencil::TypeTraits<Nested>
{};

template <> struct Stencil::TypeTraits<MultiAttributed>
{
    enum class Fields
    {
        Invalid  = 0,
        Field_f1 = 1
    };

    using Key        = Fields;
    using ValueTypes = std::tuple<>;

    template <typename TLambda> static void VisitKey(MultiAttributed& /*obj*/, Fields f, TLambda&& /*lambda*/)
    {
        switch (f)
        {
        case Fields::Field_f1:; break;
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid field");
        }
    }

    constexpr static bool IsIndexable() { return true; }
    constexpr static bool IsIterable() { return true; }
    constexpr static bool IsPrimitive() { return false; }
};

template <> struct Stencil::TypeTraits<TestObj>
{
    enum class Fields
    {
        Invalid  = 0,
        Field_f1 = 1
    };

    using Key        = Fields;
    using ValueTypes = std::tuple<MultiAttributed>;

    template <typename TLambda> static void VisitKey(TestObj& obj, Fields f, TLambda&& lambda)
    {
        switch (f)
        {
        case Fields::Field_f1: lambda(obj.f1); break;
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid field");
        }
    }

    constexpr static bool IsIndexable() { return true; }
    constexpr static bool IsIterable() { return true; }
    constexpr static bool IsPrimitive() { return false; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<TestObj>::Fields>
{
    static constexpr std::string_view           Names[] = {"Invalid", "f1"};
    static Stencil::TypeTraits<TestObj>::Fields ForIndex(size_t index) { return static_cast<Stencil::TypeTraits<TestObj>::Fields>(index); }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<MultiAttributed>::Fields>
{
    static constexpr std::string_view                   Names[] = {"Invalid", "f1"};
    static Stencil::TypeTraits<MultiAttributed>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<MultiAttributed>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<MultiAttributed> : Stencil::VisitorT<MultiAttributed>
{
    Visitor() = default;
    // Visitor(MultiAttributed& obj) : _ref(&obj) {}
    CLASS_DELETE_COPY_AND_MOVE(Visitor);

    Stencil::StructFieldEnumVisitor<MultiAttributed> _visitor_key;

    Stencil::VisitorWithParent<MultiAttributed, Stencil::TimestampedT<MultiAttributed>> _visitor_base_b1;
    Stencil::VisitorWithParent<MultiAttributed, UuidBasedId<MultiAttributed>>           _visitor_base_b2;

    // Stencil::VisitorWithParent<MultiAttributed, decltype(MultiAttributed{}.f1)> _visitor_mem_f1;

    // MultiAttributed* _ref{};
};

template <> struct Stencil::Visitor<TestObj> : Stencil::VisitorT<TestObj>
{
    Visitor() = default;
    // Visitor(TestObj& obj) : _ref(&obj) {}
    CLASS_DELETE_COPY_AND_MOVE(Visitor);

    Stencil::StructFieldEnumVisitor<TestObj> _visitor_key;

    Stencil::VisitorWithParent<TestObj, decltype(TestObj{}.f1)> _visitor_mem_f1;

    // TestObj* _ref{};
};
