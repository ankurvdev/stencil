#pragma once
#include <CommonMacros.h>

SUPPRESS_WARNINGS_START
#pragma warning(disable : 4866)    // left to right evaluation not guaranteed
#include <chrono>
SUPPRESS_WARNINGS_END

#include <bit>
#include <cmath>
#include <cstdint>

struct Primitives64Bit
{
    struct Type
    {
        enum class Category : uint8_t
        {
            Unknown,
            Unsigned,
            Signed,
            Float
        };

        enum class Width
        {
            W_0,
            W_1,
            W_2,
            W_4,
            W_8
        };

        uint8_t  width : 3;       // 00: 0 , 01: 1, 10: 2, 11: 4, 100: 8
        Category category : 2;    // 00: unknown,  01: unsigned , 2: signed, 3: float

        private:
        static constexpr unsigned _GetWidth(unsigned x) { return x < 2 ? x : 1 + _GetWidth(x >> 1); }

        template <uint8_t W, typename T> static constexpr Type _Create()
        {
            if constexpr (std::is_floating_point<T>::value) { return Type{.width = W, .category = Primitives64Bit::Type::Category::Float}; }
            else if constexpr (std::is_unsigned<T>::value)
            {
                return Type{.width = W, .category = Primitives64Bit::Type::Category::Unsigned};
            }
            else if constexpr (std::is_signed<T>::value)
            {
                return Type{.width = W, .category = Primitives64Bit::Type::Category::Signed};
            }
            else
            {
                return Type{.width = 0, .category = Primitives64Bit::Type::Category::Unknown};
            }
        }

        public:
        template <typename T> constexpr static Type Of() { return _Create<_GetWidth(sizeof(T)), T>(); }

        static constexpr Type Unknown() { return Type{.width = 0, .category = Category::Unknown}; }
        static constexpr bool IsUnknown(Type t) { return t.category == Category::Unknown; }
        static constexpr bool IsFloat(Type t) { return t.category == Category::Float; }
        static constexpr bool IsSigned(Type t) { return t.category == Category::Signed; }
        static constexpr bool IsUnsigned(Type t) { return t.category == Category::Unsigned; }

        constexpr bool operator==(Type const& t) { return width == t.width && category == t.category; }
        constexpr bool operator!=(Type const& t) { return width != t.width || category != t.category; }

        static constexpr Type Signed(unsigned n) { return Type{.width = static_cast<uint8_t>(_GetWidth(n)), .category = Category::Signed}; }
        static constexpr Type Unsigned(unsigned n)
        {
            return Type{.width = static_cast<uint8_t>(_GetWidth(n)), .category = Category::Unsigned};
        }
        static constexpr Type Float(unsigned n) { return Type{.width = static_cast<uint8_t>(_GetWidth(n)), .category = Category::Float}; }
    };

    static_assert(sizeof(Type) == 1);

    template <typename TValue> struct Traits
    {
        static constexpr auto Type() { return Type::Unknown(); }
    };

    Type     _type{Type::Unknown()};
    uint64_t _uVal{0};
    int64_t  _iVal{0};
    double   _dVal{0.0};

    template <typename T> struct SignedTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._iVal = val; }
        static const auto&    Get(const Primitives64Bit& obj) { return obj._iVal; }
        static T              Convert(int64_t val) { return static_cast<T>(val); }
        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct UnsignedTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._uVal = val; }
        static const auto&    Get(const Primitives64Bit& obj) { return obj._uVal; }
        static T              Convert(uint64_t val) { return static_cast<T>(val); }

        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct DoubleTraits
    {
        static constexpr auto Type() { return Type::Of<T>(); }
        static void           Assign(Primitives64Bit& obj, T const& val) { obj._dVal = val; }
        static const auto&    Get(const Primitives64Bit& obj) { return obj._dVal; }
        static void           Check() {}
        static T              Convert(double val) { return static_cast<T>(val); }
    };

    public:
    struct UnsupportedCast
    {};
    void _check(Type type) const
    {
        if (_type != type) throw 1;
    }

    Type GetType() const { return _type; }

    template <typename T> explicit Primitives64Bit(T const& val) : _type(Traits<T>::Type()) { Traits<T>::Assign(*this, val); }

    Primitives64Bit() = default;
    Primitives64Bit(std::nullptr_t) {}

    template <typename T> T cast() const
    {
        if constexpr (Primitives64Bit::Type::IsFloat(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(_dVal);
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(static_cast<double>(_iVal));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(static_cast<double>(_uVal));
            case Primitives64Bit::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Primitives64Bit::Type::IsSigned(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(static_cast<int64_t>(_dVal));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(static_cast<int64_t>(_uVal));
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(_iVal);
            case Primitives64Bit::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Primitives64Bit::Type::IsUnsigned(Traits<T>::Type()))
        {
            switch (_type.category)
            {
            case Primitives64Bit::Type::Category::Float: return Traits<T>::Convert(static_cast<uint64_t>(_dVal));
            case Primitives64Bit::Type::Category::Signed: return Traits<T>::Convert(static_cast<uint64_t>(_iVal));
            case Primitives64Bit::Type::Category::Unsigned: return Traits<T>::Convert(_uVal);
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

template <> struct Primitives64Bit::Traits<uint64_t> : public Primitives64Bit::UnsignedTraits<uint64_t>
{};
template <> struct Primitives64Bit::Traits<uint32_t> : public Primitives64Bit::UnsignedTraits<uint32_t>
{};
template <> struct Primitives64Bit::Traits<uint16_t> : public Primitives64Bit::UnsignedTraits<uint16_t>
{};
template <> struct Primitives64Bit::Traits<uint8_t> : public Primitives64Bit::UnsignedTraits<uint8_t>
{};
template <> struct Primitives64Bit::Traits<int64_t> : public Primitives64Bit::SignedTraits<int64_t>
{};
template <> struct Primitives64Bit::Traits<int32_t> : public Primitives64Bit::SignedTraits<int32_t>
{};
template <> struct Primitives64Bit::Traits<int16_t> : public Primitives64Bit::SignedTraits<int16_t>
{};
template <> struct Primitives64Bit::Traits<int8_t> : public Primitives64Bit::SignedTraits<int8_t>
{};

template <> struct Primitives64Bit::Traits<char> : public Primitives64Bit::SignedTraits<char>
{};

template <> struct Primitives64Bit::Traits<bool> : public Primitives64Bit::UnsignedTraits<bool>
{};
template <> struct Primitives64Bit::Traits<double> : public Primitives64Bit::DoubleTraits<double>
{};
template <> struct Primitives64Bit::Traits<float> : public Primitives64Bit::DoubleTraits<float>
{};

template <typename TClock> struct Primitives64Bit::Traits<std::chrono::time_point<TClock>>
{
    using time_point = std::chrono::time_point<TClock>;
    static constexpr auto Type() { return Primitives64Bit::Type::Unsigned(4); }
    static void           Assign(Primitives64Bit& obj, time_point& val) { obj._iVal = val.time_since_epoch().count(); }
    static auto           Get(Primitives64Bit const& obj) { return time_point(typename time_point::duration(obj._iVal)); }
    static auto           Convert(uint64_t val) { return time_point(typename time_point::duration(val)); }
};

#if 0
template <size_t N>
requires(N <= 8) struct Primitives64Bit::Traits<std::array<char, N>>
{
    static constexpr auto      Type() { return Primitives64Bit::Type::Signed(N); }
    static void                Assign(Primitives64Bit& /*obj*/, std::array<char, N>& /*val*/) { TODO(""); }
    static std::array<char, N> Get(Primitives64Bit const& /*obj*/) { TODO(""); }
    static std::array<char, N> Convert(int64_t /*val*/) { TODO(""); }
};
#endif
template <size_t N>
requires(N <= 4) struct Primitives64Bit::Traits<std::array<int16_t, N>>
{
    static constexpr auto         Type() { return Primitives64Bit::Type::Signed(N * 2); }
    static void                   Assign(Primitives64Bit& /*obj*/, std::array<int16_t, N>& /*val*/) { TODO(""); }
    static std::array<int16_t, N> Get(Primitives64Bit const& /*obj*/) { TODO(""); }
    static std::array<int16_t, N> Convert(int64_t /*val*/) { TODO(""); }
};

template <size_t N>
requires(N <= 8) struct Primitives64Bit::Traits<std::array<uint8_t, N>>
{
    static constexpr auto         Type() { return Primitives64Bit::Type::Unsigned(N); }
    static void                   Assign(Primitives64Bit& /*obj*/, std::array<uint8_t, N>& /*val*/) { TODO(""); }
    static std::array<uint8_t, N> Get(Primitives64Bit const& /*obj*/) { TODO(""); }
    static std::array<uint8_t, N> Convert(int64_t /*val*/) { TODO(""); }
};

template <size_t N>
requires(N <= 4) struct Primitives64Bit::Traits<std::array<uint16_t, N>>
{
    static constexpr auto          Type() { return Primitives64Bit::Type::Unsigned(N * 2); }
    static void                    Assign(Primitives64Bit& /*obj*/, std::array<uint16_t, N>& /*val*/) { TODO(""); }
    static std::array<uint16_t, N> Get(Primitives64Bit const& /*obj*/) { TODO(""); }
    static std::array<uint16_t, N> Convert(uint64_t val)
    {
        std::array<uint16_t, N> out;
        for (size_t i = 0; i < N; i++)
        {
            out[i] = static_cast<uint16_t>(val & 0xffff);
            val    = val >> 16;
        }
        return out;
    }
};

template <typename T>
concept ConceptPrimitives64Bit = !Primitives64Bit::Type::IsUnknown(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitFloat = Primitives64Bit::Type::IsFloat(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitSigned = Primitives64Bit::Type::IsSigned(Primitives64Bit::Traits<T>::Type());

template <typename T>
concept ConceptPrimitives64BitUnsigned = Primitives64Bit::Type::IsUnsigned(Primitives64Bit::Traits<T>::Type());

static_assert(ConceptPrimitives64Bit<double>);
static_assert(ConceptPrimitives64Bit<float>);
static_assert(ConceptPrimitives64Bit<char>);
static_assert(ConceptPrimitives64Bit<uint16_t>);
static_assert(ConceptPrimitives64Bit<bool>);
static_assert(!ConceptPrimitives64BitSigned<float>);
static_assert(ConceptPrimitives64BitFloat<float>);
static_assert(ConceptPrimitives64Bit<uint64_t>);