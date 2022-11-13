#include <stencil/stencil.h>

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
SUPPRESS_WARNINGS_END

struct WithPrimitives64Bit
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
    WithPrimitives64Bit                  f1;
    WithBlobs                            f2;
    std::shared_ptr<WithPrimitives64Bit> f3;
    std::vector<WithBlobs>               f4;
};

struct MultiAttributed : Stencil::TimestampedT<MultiAttributed>, UuidBasedId<MultiAttributed>
{
    WithPrimitives64Bit                f1;
    std::shared_ptr<WithBlobs>         f2;
    std::array<WithPrimitives64Bit, 4> f3;
};

struct WithVariant
{
    std::variant<MultiAttributed, Nested, WithBlobs, WithPrimitives64Bit>     f1;
    std::variant<int, char, std::string, uuids::uuid>                         f2;
    std::variant<std::variant<double, float>, char, std::string, uuids::uuid> f3;
    std::variant<double, std::shared_ptr<MultiAttributed>>                    f4;

    // std::unordered_map<size_t, std::string>                           f3;
    // std::unordered_map<std::string, WithPrimitives64Bit>                  f4;
};

struct NamedVariant
{
    std::variant<std::monostate, MultiAttributed, Nested, WithBlobs, WithPrimitives64Bit, double, std::string> _variant;

    MultiAttributed&     f1() { return std::get<MultiAttributed>(_variant); }
    Nested&              f2() { return std::get<Nested>(_variant); }
    WithBlobs&           f3() { return std::get<WithBlobs>(_variant); }
    WithPrimitives64Bit& f4() { return std::get<WithPrimitives64Bit>(_variant); }
    double&              f5() { return std::get<double>(_variant); }
    std::string&         f6() { return std::get<std::string>(_variant); }
};

struct TestObj
{
    MultiAttributed f1;
};

template <> struct Stencil::TypeTraits<WithPrimitives64Bit>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<WithPrimitives64Bit>
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<WithPrimitives64Bit>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<WithPrimitives64Bit>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<WithPrimitives64Bit>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<WithPrimitives64Bit>::Fields>(index);
    }
};

template <> struct Stencil::Visitor<WithPrimitives64Bit> : Stencil::VisitorT<WithPrimitives64Bit>
{
    using Fields = TypeTraitsForIndexable<WithPrimitives64Bit>::Fields;

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
    using Categories = std::tuple<Stencil::Category::Indexable>;
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
    using Categories = std::tuple<Stencil::Category::Indexable>;
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
    using Categories = std::tuple<Stencil::Category::Indexable>;
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
    using Categories = std::tuple<Stencil::Category::Indexable>;
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

    using Key = Stencil::EnumPack<Fields, Timestamp_Fields, UuidBasedId_Feilds>;
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
    template <typename T, typename TLambda> static bool VisitField(T& obj, Fields fields, TLambda&& lambda)
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

    template <typename T, typename TLambda> static void VisitAllFields(T& obj, TLambda&& lambda)
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

template <> struct Stencil::TypeTraits<WithVariant>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<WithVariant>
{
    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
        Field_f4
    };

    using Key = Stencil::EnumPack<Fields>;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<WithVariant>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<WithVariant>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<WithVariant>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<WithVariant>::Fields>(index);
    }
};

template <> struct Stencil::StructFieldsVisitor<WithVariant>
{
    using Fields = TypeTraitsForIndexable<WithVariant>::Fields;
    template <typename T, typename TLambda> static bool VisitField(T& obj, Fields fields, TLambda&& lambda)
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

    template <typename T, typename TLambda> static void VisitAllFields(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_f1, obj.f1);
        lambda(Fields::Field_f2, obj.f2);
        lambda(Fields::Field_f3, obj.f3);
        lambda(Fields::Field_f4, obj.f4);
    }
};

template <> struct Stencil::Visitor<WithVariant> : Stencil::StructVisitor<WithVariant>
{};

template <> struct Stencil::TypeTraits<NamedVariant>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<NamedVariant>
{
    enum class Fields
    {
        Invalid,
        Field_f1,
        Field_f2,
        Field_f3,
        Field_f4,
        Field_f5,
        Field_f6

    };

    using Key = Stencil::EnumPack<Fields>;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<NamedVariant>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<NamedVariant>::Fields;

    static constexpr std::string_view Names[] = {"Invalid", "f1", "f2", "f3", "f4", "f5", "f6"};

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<NamedVariant>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<NamedVariant>::Fields>(index);
    }
};

template <> struct Stencil::StructFieldsVisitor<NamedVariant>
{
    using Fields = TypeTraitsForIndexable<NamedVariant>::Fields;
    template <typename TType, typename TObj, typename TLambda> static void _SetAndVisit(TObj& obj, TLambda&& lambda)
    {
        using Type = std::remove_cvref_t<TType>;
        obj        = Type{};
        lambda(std::get<Type>(obj));
    }

    template <typename T, typename TLambda> static bool VisitField(T& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::Field_f1: _SetAndVisit<decltype(obj.f1())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Field_f2: _SetAndVisit<decltype(obj.f2())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Field_f3: _SetAndVisit<decltype(obj.f3())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Field_f4: _SetAndVisit<decltype(obj.f4())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Field_f5: _SetAndVisit<decltype(obj.f5())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Field_f6: _SetAndVisit<decltype(obj.f6())>(obj._variant, std::forward<TLambda>(lambda)); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T, typename TLambda> static void VisitAllFields(T& obj, TLambda&& lambda)
    {
        auto fieldType = static_cast<Fields>(obj._variant.index());
        std::visit(
            [&](auto&& arg) {
                if constexpr (!std::is_same_v<std::remove_cvref_t<decltype(arg)>, std::monostate>) { lambda(fieldType, arg); }
            },
            obj._variant);
    }
};

template <> struct Stencil::Visitor<NamedVariant> : Stencil::StructVisitor<NamedVariant>
{};

template <> struct Stencil::TypeTraits<TestObj>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
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
