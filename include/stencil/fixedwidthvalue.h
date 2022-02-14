#pragma once
#include <CommonMacros.h>
SUPPRESS_WARNINGS_START
#pragma warning(disable : 4866)    // left to right evaluation not guaranteed
#include <chrono>
SUPPRESS_WARNINGS_END

#include <bit>
#include <cmath>
#include <cstdint>

struct Value
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
            if constexpr (std::is_floating_point<T>::value) { return Type{.width = W, .category = Value::Type::Category::Float}; }
            else if constexpr (std::is_unsigned<T>::value)
            {
                return Type{.width = W, .category = Value::Type::Category::Unsigned};
            }
            else if constexpr (std::is_signed<T>::value)
            {
                return Type{.width = W, .category = Value::Type::Category::Signed};
            }
            else
            {
                return Type{.width = 0, .category = Value::Type::Category::Unknown};
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

    template <typename TValue> struct ValueTraits
    {
        static constexpr auto ValueType() { return Type::Unknown(); }
    };

    Type     _type{Type::Unknown()};
    uint64_t _uVal{0};
    int64_t  _iVal{0};
    double   _dVal{0.0};

    template <typename T> struct SignedTraits
    {
        static constexpr auto ValueType() { return Type::Of<T>(); }
        static void           Assign(Value& obj, T const& val) { obj._iVal = val; }
        static const auto&    Get(const Value& obj) { return obj._iVal; }
        static T              Convert(int64_t val) { return static_cast<T>(val); }
        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct UnsignedTraits
    {
        static constexpr auto ValueType() { return Type::Of<T>(); }
        static void           Assign(Value& obj, T const& val) { obj._uVal = val; }
        static const auto&    Get(const Value& obj) { return obj._uVal; }
        static T              Convert(uint64_t val) { return static_cast<T>(val); }

        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct DoubleTraits
    {
        static constexpr auto ValueType() { return Type::Of<T>(); }
        static void           Assign(Value& obj, T const& val) { obj._dVal = val; }
        static const auto&    Get(const Value& obj) { return obj._dVal; }
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

    operator size_t() const;

    Type GetType() const { return _type; }

    template <typename T> explicit Value(T const& val) : _type(ValueTraits<T>::ValueType()) { ValueTraits<T>::Assign(*this, val); }

    Value() = default;
    Value(std::nullptr_t) {}
#pragma warning(push, 3)    // TODO : Remove the warning
#pragma warning(disable : 4244)
    template <typename T> T cast() const
    {
        if constexpr (Value::Type::IsFloat(ValueTraits<T>::ValueType()))
        {
            switch (_type.category)
            {
            case Value::Type::Category::Float: return ValueTraits<T>::Convert(_dVal);
            case Value::Type::Category::Signed: return ValueTraits<T>::Convert(static_cast<double>(_iVal));
            case Value::Type::Category::Unsigned: return ValueTraits<T>::Convert(static_cast<double>(_uVal));
            case Value::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Value::Type::IsSigned(ValueTraits<T>::ValueType()))
        {
            switch (_type.category)
            {
            case Value::Type::Category::Float: return ValueTraits<T>::Convert(static_cast<int64_t>(_dVal));
            case Value::Type::Category::Unsigned: return ValueTraits<T>::Convert(static_cast<int64_t>(_uVal));
            case Value::Type::Category::Signed: return ValueTraits<T>::Convert(_iVal);
            case Value::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else if constexpr (Value::Type::IsUnsigned(ValueTraits<T>::ValueType()))
        {
            switch (_type.category)
            {
            case Value::Type::Category::Float: return ValueTraits<T>::Convert(static_cast<uint64_t>(_dVal));
            case Value::Type::Category::Signed: return ValueTraits<T>::Convert(static_cast<uint64_t>(_iVal));
            case Value::Type::Category::Unsigned: return ValueTraits<T>::Convert(_uVal);
            case Value::Type::Category::Unknown: [[fallthrough]];
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else
        {
            throw std::logic_error("Unknown type");
        }
    }

#pragma warning(pop)

#if 0
    static int _doubletoint(double val) { return static_cast<int>(std::round(val)); }

    Value cast_signed() const
    {
        switch (_type)
        {
        case Type::Empty: throw UnsupportedCast();
        case Type::Signed: return Value(_iVal);
        case Type::Unsigned: return Value(static_cast<int64_t>(_uVal));
        case Type::Double: return Value(_doubletoint(_dVal));
        case Type::Unknown: [[fallthrough]];

        default: throw UnsupportedCast();
        }
    }

    Value cast_unsigned() const
    {
        switch (_type)
        {
        case Type::Empty: throw UnsupportedCast();
        case Type::Signed: return Value(static_cast<uint64_t>(_iVal));
        case Type::Unsigned: return Value(_uVal);
        case Type::Double: return Value(_doubletoint(_dVal));
        case Type::Unknown: [[fallthrough]];

        default: throw UnsupportedCast();
        }
    }

    Value cast_double() const
    {
        switch (_type)
        {
        case Type::Empty: throw UnsupportedCast();
        case Type::Signed: return Value(static_cast<int64_t>(round(_dVal)));
        case Type::Unsigned: return Value(static_cast<uint64_t>(round(_dVal)));
        case Type::Double: return Value(_dVal);
        case Type::Unknown: [[fallthrough]];

        default: throw UnsupportedCast();
        }
    }

    Value cast(Type type) const
    {
        switch (type)
        {
        case Type::Empty: return Value();
        case Type::Signed: return cast_signed();
        case Type::Unsigned: return cast_unsigned();
        case Type::Double: return cast_double();
        case Type::Unknown: [[fallthrough]];

        default: throw UnsupportedCast();
        }
    }

    template <typename T> T convert() const
    {
        auto casted = (*this).cast(ValueTraits<T>::ValueType());
        return static_cast<T>(ValueTraits<T>::Get(casted));
    }
#endif
};

template <> struct Value::ValueTraits<uint64_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct Value::ValueTraits<uint32_t> : public Value::UnsignedTraits<uint32_t>
{};
template <> struct Value::ValueTraits<uint16_t> : public Value::UnsignedTraits<uint16_t>
{};
template <> struct Value::ValueTraits<uint8_t> : public Value::UnsignedTraits<uint8_t>
{};
template <> struct Value::ValueTraits<int64_t> : public Value::SignedTraits<int64_t>
{};
template <> struct Value::ValueTraits<int32_t> : public Value::SignedTraits<int32_t>
{};
template <> struct Value::ValueTraits<int16_t> : public Value::SignedTraits<int16_t>
{};
template <> struct Value::ValueTraits<int8_t> : public Value::SignedTraits<int8_t>
{};

template <> struct Value::ValueTraits<char> : public Value::SignedTraits<char>
{};

template <> struct Value::ValueTraits<bool> : public Value::UnsignedTraits<bool>
{};
template <> struct Value::ValueTraits<double> : public Value::DoubleTraits<double>
{};
template <> struct Value::ValueTraits<float> : public Value::DoubleTraits<float>
{};

template <typename TClock> struct Value::ValueTraits<std::chrono::time_point<TClock>>
{
    using time_point = std::chrono::time_point<TClock>;
    static constexpr auto ValueType() { return Value::Type::Unsigned(4); }
    static void           Assign(Value& obj, time_point& val) { obj._iVal = val.time_since_epoch().count(); }
    static auto           Get(Value const& obj) { return time_point(typename time_point::duration(obj._iVal)); }
    static auto           Convert(uint64_t val) { return time_point(typename time_point::duration(val)); }
};

#if 0
template <size_t N>
requires(N <= 8) struct Value::ValueTraits<std::array<char, N>>
{
    static constexpr auto      ValueType() { return Value::Type::Signed(N); }
    static void                Assign(Value& /*obj*/, std::array<char, N>& /*val*/) { TODO(""); }
    static std::array<char, N> Get(Value const& /*obj*/) { TODO(""); }
    static std::array<char, N> Convert(int64_t /*val*/) { TODO(""); }
};
#endif
template <size_t N>
requires(N <= 4) struct Value::ValueTraits<std::array<int16_t, N>>
{
    static constexpr auto         ValueType() { return Value::Type::Signed(N * 2); }
    static void                   Assign(Value& /*obj*/, std::array<int16_t, N>& /*val*/) { TODO(""); }
    static std::array<int16_t, N> Get(Value const& /*obj*/) { TODO(""); }
    static std::array<int16_t, N> Convert(int64_t /*val*/) { TODO(""); }
};

template <size_t N>
requires(N <= 8) struct Value::ValueTraits<std::array<uint8_t, N>>
{
    static constexpr auto         ValueType() { return Value::Type::Unsigned(N); }
    static void                   Assign(Value& /*obj*/, std::array<uint8_t, N>& /*val*/) { TODO(""); }
    static std::array<uint8_t, N> Get(Value const& /*obj*/) { TODO(""); }
    static std::array<uint8_t, N> Convert(int64_t /*val*/) { TODO(""); }
};

template <size_t N>
requires(N <= 4) struct Value::ValueTraits<std::array<uint16_t, N>>
{
    static constexpr auto          ValueType() { return Value::Type::Unsigned(N * 2); }
    static void                    Assign(Value& /*obj*/, std::array<uint16_t, N>& /*val*/) { TODO(""); }
    static std::array<uint16_t, N> Get(Value const& /*obj*/) { TODO(""); }
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

inline Value::operator ::size_t() const
{
    _check(ValueTraits<uint64_t>::ValueType());
    return static_cast<size_t>(ValueTraits<uint64_t>::Get(*this));
}
template <typename T>
concept ConceptValue = !Value::Type::IsUnknown(Value::ValueTraits<T>::ValueType());

template <typename T>
concept ConceptValueFloat = Value::Type::IsFloat(Value::ValueTraits<T>::ValueType());

template <typename T>
concept ConceptValueSigned = Value::Type::IsSigned(Value::ValueTraits<T>::ValueType());

template <typename T>
concept ConceptValueUnsigned = Value::Type::IsUnsigned(Value::ValueTraits<T>::ValueType());

static_assert(ConceptValue<double>);
static_assert(ConceptValue<float>);
static_assert(ConceptValue<char>);
static_assert(ConceptValue<uint16_t>);
static_assert(ConceptValue<bool>);
static_assert(!ConceptValueSigned<float>);
static_assert(ConceptValueFloat<float>);
static_assert(ConceptValue<uint64_t>);
