#pragma once
#include "CommonMacros.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_MSVC_WARNING(4866)    // left to right evaluation not guaranteed
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

SUPPRESS_WARNINGS_END

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access,bugprone-branch-clone,readability-magic-numbers)

struct Primitives64Bit
{
    struct Type
    {
        enum class Category : uint8_t
        {
            Unknown,
            Unsigned,
            Signed,
            Float,
        };

        enum class Width
        {
            W0,
            W1,
            W2,
            W4,
            W8,
        };

        uint8_t  width : 3;       // 00: 0 , 01: 1, 10: 2, 11: 4, 100: 8
        Category category : 2;    // 00: unknown,  01: unsigned , 2: signed, 3: float

        private:
        static constexpr unsigned GetWidth_(unsigned x) { return x < 2 ? x : 1 + GetWidth_(x >> 1u); }

        template <uint8_t W, typename T> static constexpr Type Create_()
        {
            // Apparently chars can be signed or unsigned
            if constexpr (std::is_same_v<T, char>) { return Type{.width = W, .category = Primitives64Bit::Type::Category::Signed}; }
            else if constexpr (std::is_floating_point_v<T>) { return Type{.width = W, .category = Primitives64Bit::Type::Category::Float}; }
            else if constexpr (std::is_unsigned_v<T>) { return Type{.width = W, .category = Primitives64Bit::Type::Category::Unsigned}; }
            else if constexpr (std::is_signed_v<T>) { return Type{.width = W, .category = Primitives64Bit::Type::Category::Signed}; }
            else
            {
                return Type{.width = 0, .category = Primitives64Bit::Type::Category::Unknown};
            }
        }

        public:
        template <typename T> constexpr static Type Of() { return Create_<GetWidth_(sizeof(T)), T>(); }

        static constexpr Type Unknown() { return Type{.width = 0, .category = Category::Unknown}; }
        static constexpr bool IsUnknown(Type t) { return t.category == Category::Unknown; }
        static constexpr bool IsFloat(Type t) { return t.category == Category::Float; }
        static constexpr bool IsSigned(Type t) { return t.category == Category::Signed; }
        static constexpr bool IsUnsigned(Type t) { return t.category == Category::Unsigned; }

        constexpr bool operator==(Type const& t) const { return width == t.width && category == t.category; }
        constexpr bool operator!=(Type const& t) const { return width != t.width || category != t.category; }

        SUPPRESS_WARNINGS_START
        SUPPRESS_GCC_WARNING("-Wconversion")
        SUPPRESS_CLANG_WARNING("-Wconversion")

        static constexpr Type Signed(unsigned n) { return Type{.width = static_cast<uint8_t>(GetWidth_(n)), .category = Category::Signed}; }
        static constexpr Type Unsigned(unsigned n)
        { return Type{.width = static_cast<uint8_t>(GetWidth_(n)), .category = Category::Unsigned}; }
        static constexpr Type Float(unsigned n) { return Type{.width = static_cast<uint8_t>(GetWidth_(n)), .category = Category::Float}; }
        SUPPRESS_WARNINGS_END
    };

    static_assert(sizeof(Type) == 1);

    template <typename TValue> struct Traits
    {
        static constexpr auto Type() { return Type::Unknown(); }
    };

    private:
    Type _type{Type::Unknown()};
    union
    {
        uint64_t u{};
        int64_t  i;
        double   d;
    } _val{};

    public:
    template <typename T> struct SignedTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._val.i = val; }    // NOLINT(bugprone-signed-char-misuse)
        static auto const&    Get(Primitives64Bit const& obj) { return obj._val.i; }
        static T              Convert(int64_t val) { return static_cast<T>(val); }
        static int64_t        Repr(T const& val)
        {
            Primitives64Bit obj;
            Assign(obj, val);
            return obj._val.i;
        }
        // static void Check() { if (obj._val.i < std::numeric_limits<T>::min() || obj._val.i > std::numeric_limits<T>::max()); throw 1; }
    };
    // std::numeric_limits<T>::max()); throw 1; }

    template <typename T> struct UnsignedTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._val.u = val; }
        static auto const&    Get(Primitives64Bit const& obj) { return obj._val.u; }
        static T              Convert(uint64_t val) { return static_cast<T>(val); }
        static uint64_t       Repr(T const& val)
        {
            Primitives64Bit obj;
            Assign(obj, val);
            return obj._val.u;
        }
        // static void Check() { if (obj._val.u < std::numeric_limits<T>::min() || obj._val.u > std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct DoubleTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._val.d = static_cast<double>(val); }
        static auto const&    Get(Primitives64Bit const& obj) { return obj._val.d; }
        static void           Check() {}
        static T              Convert(double val) { return static_cast<T>(val); }
        static double         Repr(T const& val)
        {
            Primitives64Bit obj;
            Assign(obj, val);
            return obj._val.d;
        }
    };

    public:
    struct UnsupportedCast
    {};

    [[nodiscard]] Type GetType() const { return _type; }

    constexpr bool operator==(Primitives64Bit const& rhs) const { return _type == rhs._type && _val.u == rhs._val.u; }

    template <typename T> explicit Primitives64Bit(T const& val) : _type(Traits<T>::Type()) { Traits<T>::Assign(*this, val); }

    Primitives64Bit() = default;
    explicit Primitives64Bit(std::nullptr_t) {}

    template <typename T> [[nodiscard]] T Cast() const
    {
        if constexpr (Primitives64Bit::Type::IsFloat(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(_val.d);
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(static_cast<double>(_val.i));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(static_cast<double>(_val.u));
            case Primitives64Bit::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Primitives64Bit::Type::IsSigned(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(static_cast<int64_t>(_val.d));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(static_cast<int64_t>(_val.u));
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(_val.i);
            case Primitives64Bit::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Primitives64Bit::Type::IsUnsigned(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(static_cast<uint64_t>(_val.d));
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(static_cast<uint64_t>(_val.i));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(_val.u);
            case Primitives64Bit::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else
        {
            throw std::logic_error("Unknown type");
        }
    }
};
template <typename T>
concept ConceptSignedIntegral = std::integral<T> && std::is_signed_v<T>;
template <typename T>
concept ConceptUnsignedIntegral = std::integral<T> && !std::is_signed_v<T>;

template <ConceptUnsignedIntegral T> struct Primitives64Bit::Traits<T> : public Primitives64Bit::UnsignedTraits<T>
{};
template <ConceptSignedIntegral T> struct Primitives64Bit::Traits<T> : public Primitives64Bit::SignedTraits<T>
{};

template <> struct Primitives64Bit::Traits<double> : public Primitives64Bit::DoubleTraits<double>
{};
template <> struct Primitives64Bit::Traits<float> : public Primitives64Bit::DoubleTraits<float>
{};

template <typename TClock> struct Primitives64Bit::Traits<std::chrono::time_point<TClock>>
{
    using time_point = std::chrono::time_point<TClock>;
    static constexpr auto Type() { return Primitives64Bit::Type::Unsigned(4); }
    static void           Assign(Primitives64Bit& obj, time_point& val) { obj._val.u = Repr(val); }
    static auto           Get(Primitives64Bit const& obj) { return time_point(typename time_point::duration(obj._val.u)); }
    static auto           Convert(uint64_t val) { return time_point(std::chrono::microseconds(val)); }
    static uint64_t       Repr(time_point const& val)
    { return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(val.time_since_epoch()).count()); }
};

template <size_t N>
    requires(N <= 4)
struct Primitives64Bit::Traits<std::array<int16_t, N>>
{
    using TObj = std::array<int16_t, N>;
    static constexpr auto                      Type() { return Primitives64Bit::Type::Signed(N * 2); }
    [[noreturn]] static void                   Assign(Primitives64Bit& /*obj*/, std::array<int16_t, N>& /*val*/) { TODO(""); }
    [[noreturn]] static std::array<int16_t, N> Get(Primitives64Bit const& /*obj*/) { TODO(""); }
    [[noreturn]] static std::array<int16_t, N> Convert(int64_t /*val*/) { TODO(""); }
    static int64_t                             Repr(TObj const& val)
    {
        Primitives64Bit obj;
        Assign(obj, val);
        return obj._val.i;
    }
};

template <size_t N>
    requires(N <= 8)
struct Primitives64Bit::Traits<std::array<uint8_t, N>>
{
    using TObj = std::array<uint8_t, N>;

    static constexpr auto Type() { return Primitives64Bit::Type::Unsigned(N); }
    static void           Assign(Primitives64Bit& obj, TObj const& val) { obj._val.u = Repr(val); }
    static constexpr TObj Get(Primitives64Bit const& obj) { return Convert(obj._val.u); }
    static constexpr TObj Convert(uint64_t val)
    {
        TObj out;
        for (size_t i = 0; i < N; i++)
        {
            out[i] = static_cast<uint8_t>(val & 0xffu);
            val    = val >> 8u;
        }
        return out;
    }
    static constexpr uint64_t Repr(TObj const& val)
    {
        uint64_t out = 0;
        for (size_t i = N; i > 0; i--) { out = (out << 8u) | val.at(i - 1); }
        return out;
    }
};

template <size_t N>
    requires(N <= 4)
struct Primitives64Bit::Traits<std::array<uint16_t, N>>
{
    using TObj = std::array<uint16_t, N>;

    static constexpr auto Type() { return Primitives64Bit::Type::Unsigned(N * 2); }
    static void           Assign(Primitives64Bit& obj, TObj const& val) { obj._val.u = Repr(val); }
    static constexpr TObj Get(Primitives64Bit const& obj) { return Convert(obj._val.u); }
    static constexpr TObj Convert(uint64_t val)
    {
        TObj out;
        for (size_t i = 0; i < N; i++)
        {
            out[i] = static_cast<uint16_t>(val & 0xffffu);
            val    = val >> 16u;
        }
        return out;
    }
    static uint64_t Repr(TObj const& val)
    {
        uint64_t out = 0;
        for (size_t i = N; i > 0; i--) { out = (out << 16u) | val.at(i - 1); }
        return out;
    }
};
// NOLINTEND(cppcoreguidelines-pro-type-union-access,bugprone-branch-clone,readability-magic-numbers)

template <typename T>
concept ConceptPrimitives64Bit = !Primitives64Bit::Type::IsUnknown(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitFloat = Primitives64Bit::Type::IsFloat(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitSigned = Primitives64Bit::Type::IsSigned(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitUnsigned = Primitives64Bit::Type::IsUnsigned(Primitives64Bit::Traits<T>::Type());

template <typename T, size_t N>
concept ConceptPrimitive64BitArray = ConceptPrimitives64Bit<std::array<T, N>>;

static_assert(ConceptPrimitives64Bit<double>);
static_assert(ConceptPrimitives64Bit<float>);
static_assert(ConceptPrimitives64Bit<char>);
static_assert(ConceptPrimitives64Bit<uint16_t>);
static_assert(ConceptPrimitives64Bit<bool>);
static_assert(!ConceptPrimitives64BitSigned<float>);
static_assert(ConceptPrimitives64BitFloat<float>);
static_assert(ConceptPrimitives64Bit<uint64_t>);
static_assert(ConceptPrimitives64BitSigned<char>);    // Apparently chars can be signed or unsigned. We treat then as signed
static_assert(ConceptPrimitives64Bit<unsigned long>);
static_assert(ConceptPrimitives64Bit<std::chrono::time_point<std::chrono::system_clock>>);
static_assert(ConceptPrimitives64Bit<std::chrono::time_point<std::chrono::steady_clock>>);
static_assert(ConceptPrimitives64Bit<std::chrono::time_point<std::chrono::high_resolution_clock>>);
