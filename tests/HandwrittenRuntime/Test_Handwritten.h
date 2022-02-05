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

    std::chrono::time_point<std::chrono::system_clock> f8;
    std::chrono::time_point<std::chrono::system_clock> f9;    // TODO High performance clock
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
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<Primitives64Bit>::Fields>
{
    using Enum = Stencil::TypeTraits<Primitives64Bit>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraits<Primitives64Bit>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<Primitives64Bit>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<Primitives64Bit> : Stencil::VisitorT<Primitives64Bit>
{
    using Fields = TypeTraits<Primitives64Bit>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
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
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_f1, obj.f1);
        lambda(Fields::Field_f2, obj.f2);
        lambda(Fields::Field_f3, obj.f3);
        lambda(Fields::Field_f4, obj.f4);
        lambda(Fields::Field_f5, obj.f5);
        lambda(Fields::Field_f6, obj.f6);
        lambda(Fields::Field_f7, obj.f7);
        lambda(Fields::Field_f8, obj.f8);
        lambda(Fields::Field_f9, obj.f9);
    }
};

template <> struct Stencil::TypeTraits<ComplexPrimitives>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;

    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<ComplexPrimitives>::Fields>
{
    using Enum = Stencil::TypeTraits<ComplexPrimitives>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraits<ComplexPrimitives>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<ComplexPrimitives>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<ComplexPrimitives> : Stencil::VisitorT<ComplexPrimitives>
{
    using Fields = TypeTraits<ComplexPrimitives>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_f1: return lambda(obj.f1);
        case Fields::Field_f2: return lambda(obj.f2);
        case Fields::Field_f3: return lambda(obj.f3);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_f1, obj.f1);
        lambda(Fields::Field_f2, obj.f2);
        lambda(Fields::Field_f3, obj.f3);
    }
};

template <> struct Stencil::TypeTraits<LargePrimitives>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;

    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
        Field_f4,
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<LargePrimitives>::Fields>
{
    using Enum = Stencil::TypeTraits<LargePrimitives>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraits<LargePrimitives>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<LargePrimitives>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<LargePrimitives> : Stencil::VisitorT<LargePrimitives>
{
    using Fields = TypeTraits<LargePrimitives>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_f1: return lambda(obj.f1);
        case Fields::Field_f2: return lambda(obj.f2);
        case Fields::Field_f3: return lambda(obj.f3);
        case Fields::Field_f4: return lambda(obj.f4);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_f1, obj.f1);
        lambda(Fields::Field_f2, obj.f2);
        lambda(Fields::Field_f3, obj.f3);
        lambda(Fields::Field_f4, obj.f4);
    }
};

template <> struct Stencil::TypeTraits<MultiAttributed>
{
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable>;
    enum class Fields
    {
        Invalid  = 0,
        Field_f1 = 1
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<MultiAttributed>::Fields>
{
    using Enum = Stencil::TypeTraits<MultiAttributed>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraits<MultiAttributed>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraits<MultiAttributed>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<MultiAttributed> : Stencil::VisitorT<MultiAttributed>
{
    using Fields = TypeTraits<MultiAttributed>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& /*obj*/, Fields field, TLambda&& /*lambda*/)
    {
        switch (field)
        {
        case Fields::Field_f1:    // return lambda(obj.f1);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& /*obj*/, TLambda&& /*lambda*/)
    {
        // lambda(Fields::Field_f1, obj.f1);
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

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraits<TestObj>::Fields>
{
    using Enum = Stencil::TypeTraits<TestObj>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraits<TestObj>::Fields ForIndex(size_t index) { return static_cast<Stencil::TypeTraits<TestObj>::Fields>(index); }
};

template <> struct Stencil::Visitor<TestObj> : Stencil::VisitorT<TestObj>
{
    using Fields = TypeTraits<TestObj>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_f1: return lambda(obj.f1);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda) { lambda(Fields::Field_f1, obj.f1); }
};
