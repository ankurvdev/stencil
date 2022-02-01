#include <stencil/stencil.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

struct Primitives64Bit
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
};

struct ComplexPrimitives
{
    std::array<char, 8>     f1;    // Iterable, blob (string) and Value (64-bit)
    std::array<uint16_t, 4> f2;    // Iterable and by value (64 bit)
    std::array<float, 2>    f3;    // Iterable and by value (64 bit)
};

struct LargePrimitives
{
    std::array<char, 16>    f1;    // Iterable, blob (string) and Value (64-bit)
    std::array<uint16_t, 8> f2;    // Iterable and by value (64 bit)
    std::array<float, 4>    f3;    // Iterable and by value (64 bit)
    uuids::uuid             f4;
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
    Primitives64Bit strctf;
    WithBlobs       strctb;

    // std::unique_ptr<Primitives> uptr;
    std::shared_ptr<Primitives64Bit> sptr;

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

template <> struct Stencil::TypeTraits<Primitives64Bit>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;

    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
        Field_f4,
        Field_f5,
        Field_f6,
        Field_f7,
        Field_f8,
        Field_f9,
    };

    using Key = Fields;

    // TODO: Move to visitor
    template <typename TLambda> static void VisitKey(Primitives64Bit& obj, Fields f, TLambda&& lambda)
    {
        switch (f)
        {
        case Fields::Field_f1: return lambda(obj.f1);
        case Fields::Field_f2: return lambda(obj.f2);
        case Fields::Field_f3: return lambda(obj.f3);
        case Fields::Field_f4: return lambda(obj.f4);
        case Fields::Field_f5: return lambda(obj.f5);
        case Fields::Field_f6: return lambda(obj.f6);
        case Fields::Field_f7: return lambda(obj.f7);
        case Fields::Field_f8: return lambda(obj.f8);
        case Fields::Field_f9: return lambda(obj.f9);

        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid field");
        }
    }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<Primitives64Bit>::Fields>
{
    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"};

    static Stencil::TypeTraits<Primitives64Bit>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<Primitives64Bit>::Fields>(index);
    }
};

static_assert(Stencil::Type::IsIndexable<Primitives64Bit>(), "indexable");

template <> struct Stencil::TypeTraits<WithBlobs>
{};

template <> struct Stencil::TypeTraits<Nested>
{};

template <> struct Stencil::TypeTraits<MultiAttributed>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;
    enum class Fields
    {
        Invalid  = 0,
        Field_f1 = 1
    };

    using Key = Fields;

    // TODO:
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
};

template <> struct Stencil::TypeTraits<TestObj>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;

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
