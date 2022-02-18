#include <stencil/stencil.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>

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
    std::string              f1;
    std::wstring             f2;
    std::vector<double>      f3;
    std::vector<std::string> f4;
};

struct Nested
{
    Primitives64Bit                  f1;
    WithBlobs                        f2;
    std::shared_ptr<Primitives64Bit> f3;
    std::vector<WithBlobs>           f4;
};

struct MultiAttributed : Stencil::TimestampedT<MultiAttributed>, UuidBasedId<MultiAttributed>
{
    Primitives64Bit                f1;
    std::shared_ptr<WithBlobs>     f2;
    std::array<Primitives64Bit, 4> f3;
};

struct WithVariantAndMaps
{
    std::variant<MultiAttributed, Nested, WithBlobs, Primitives64Bit> f1;
    std::variant<int, char, std::string, uuids::uuid>                 f2;
    std::unordered_map<size_t, std::string>                           f3;
    std::unordered_map<std::string, Primitives64Bit>                  f4;
};

struct TestObj
{
    MultiAttributed f1;
};

template <> struct Stencil::TypeTraits<Primitives64Bit>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Primitives64Bit>
{
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Primitives64Bit>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Primitives64Bit>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Primitives64Bit>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Primitives64Bit>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<Primitives64Bit> : Stencil::VisitorT<Primitives64Bit>
{
    using Fields = TypeTraitsForIndexable<Primitives64Bit>::Fields;

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
    using Categories = std::tuple<Stencil::Category::Indexable, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraitsForIndexable<ComplexPrimitives>
{
    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<ComplexPrimitives>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<ComplexPrimitives>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<ComplexPrimitives>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<ComplexPrimitives>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<ComplexPrimitives> : Stencil::VisitorT<ComplexPrimitives>
{
    using Fields = TypeTraitsForIndexable<ComplexPrimitives>::Fields;

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
    using Categories = std::tuple<Stencil::Category::Indexable, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraitsForIndexable<LargePrimitives>
{

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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<LargePrimitives>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<LargePrimitives>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<LargePrimitives>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<LargePrimitives>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<LargePrimitives> : Stencil::VisitorT<LargePrimitives>
{
    using Fields = TypeTraitsForIndexable<LargePrimitives>::Fields;

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

template <> struct Stencil::TypeTraits<WithBlobs>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<WithBlobs>
{
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<WithBlobs>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<WithBlobs>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<WithBlobs>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<WithBlobs>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<WithBlobs> : Stencil::VisitorT<WithBlobs>
{
    using Fields = TypeTraitsForIndexable<WithBlobs>::Fields;

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

template <> struct Stencil::TypeTraits<Nested>
{
    using Categories = std::tuple<Stencil::Category::Indexable, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Nested>
{
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Nested>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Nested>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Nested>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Nested>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<Nested> : Stencil::VisitorT<Nested>
{
    using Fields = TypeTraitsForIndexable<Nested>::Fields;

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
    using Categories = std::tuple<Stencil::Category::Indexable, Stencil::Category::Iterable>;
};

enum class Timestamp_Fields
{
    Invalid,
    Field_timestamp
};

enum class UuidBasedId_Fields
{
    Invalid,
    Field_uuid
};

template <typename T> struct Stencil::TypeTraitsForIndexable<Stencil::TimestampedT<T>>
{
    using Fields = Timestamp_Fields;
};

template <typename T> struct Stencil::TypeTraitsForIndexable<UuidBasedId<T>>
{
    using Fields = UuidBasedId_Fields;
};

template <> struct Stencil::EnumTraits<UuidBasedId_Fields>
{
    using Enum = UuidBasedId_Fields;

    static constexpr std::string_view Names[] = {"Invalid", "uuid"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Enum ForIndex(size_t index) { return static_cast<Enum>(index); }
};

template <> struct Stencil::EnumTraits<Timestamp_Fields>
{
    using Enum = Timestamp_Fields;

    static constexpr std::string_view Names[] = {"Invalid", "timestamp"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Enum ForIndex(size_t index) { return static_cast<Enum>(index); }
};

template <> struct Stencil::TypeTraitsForIndexable<MultiAttributed>
{
    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3
    };

    using Timestamp_Fields   = typename Stencil::TypeTraitsForIndexable<Stencil::TimestampedT<MultiAttributed>>::Fields;
    using UuidBasedId_Feilds = typename Stencil::TypeTraitsForIndexable<UuidBasedId<MultiAttributed>>::Fields;

    using Key = std::variant<Fields, Timestamp_Fields, UuidBasedId_Feilds>;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<MultiAttributed>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<MultiAttributed>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<MultiAttributed>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<MultiAttributed>::Fields>(index);
    }
};

template <> struct Stencil::StructFieldsVisitor<MultiAttributed>
{
    using Fields = TypeTraitsForIndexable<MultiAttributed>::Fields;
    template <typename T, typename TLambda> static bool VisitKey(T& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::Field_f1: lambda(obj.f1); return true;
        case Fields::Field_f2: lambda(obj.f2); return true;
        case Fields::Field_f3: lambda(obj.f3); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_f1, obj.f1);
        lambda(Fields::Field_f2, obj.f2);
        lambda(Fields::Field_f3, obj.f3);
    }
};

template <>
struct Stencil::Visitor<MultiAttributed>
    : Stencil::StructVisitor<MultiAttributed, Stencil::TimestampedT<MultiAttributed>, UuidBasedId<MultiAttributed>>
{};

template <> struct Stencil::TypeTraits<WithVariantAndMaps>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<WithVariantAndMaps>
{
    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
        Field_f4
    };

    using Timestamp_Fields   = typename Stencil::TypeTraitsForIndexable<Stencil::TimestampedT<WithVariantAndMaps>>::Fields;
    using UuidBasedId_Feilds = typename Stencil::TypeTraitsForIndexable<UuidBasedId<WithVariantAndMaps>>::Fields;

    using Key = std::variant<Fields, Timestamp_Fields, UuidBasedId_Feilds>;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<WithVariantAndMaps>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<WithVariantAndMaps>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<WithVariantAndMaps>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<WithVariantAndMaps>::Fields>(index);
    }
};

template <> struct Stencil::StructFieldsVisitor<WithVariantAndMaps>
{
    using Fields = TypeTraitsForIndexable<WithVariantAndMaps>::Fields;
    template <typename T, typename TLambda> static bool VisitKey(T& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::Field_f1: lambda(obj.f1); return true;
        case Fields::Field_f2: lambda(obj.f2); return true;
        case Fields::Field_f3: lambda(obj.f3); return true;
        case Fields::Field_f4: lambda(obj.f4); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
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

template <> struct Stencil::Visitor<WithVariantAndMaps> : Stencil::StructVisitor<WithVariantAndMaps>
{};

template <> struct Stencil::TypeTraits<TestObj>
{
    using Categories = std::tuple<Stencil::Category::Indexable, Stencil::Category::Iterable>;
};

template <> struct Stencil::TypeTraitsForIndexable<TestObj>
{
    enum class Fields
    {
        Invalid  = 0,
        Field_f1 = 1
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<TestObj>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<TestObj>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<TestObj>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<TestObj>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<TestObj> : Stencil::VisitorT<TestObj>
{
    using Fields = TypeTraitsForIndexable<TestObj>::Fields;

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
