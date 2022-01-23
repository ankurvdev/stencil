#pragma once
SUPPRESS_WARNINGS_START
#pragma warning(disable : 4866)    // left to right evaluation not guaranteed
#include <chrono>
SUPPRESS_WARNINGS_END

#include <chrono>
#include <cmath>
#include <cstdint>

template <typename TValue> struct ValueTraits;
template <> struct ValueTraits<size_t>;
template <> struct ValueTraits<uint64_t>;
template <> struct ValueTraits<uint32_t>;
template <> struct ValueTraits<uint16_t>;
template <> struct ValueTraits<uint8_t>;

template <> struct ValueTraits<int64_t>;
template <> struct ValueTraits<int32_t>;
template <> struct ValueTraits<int16_t>;
template <> struct ValueTraits<int8_t>;
template <> struct ValueTraits<bool>;
template <> struct ValueTraits<double>;
template <> struct ValueTraits<std::chrono::time_point<std::chrono::system_clock>>;

struct Value
{
    enum class Type
    {
        Empty,
        Signed,
        Unsigned,
        Double,
        Unknown
    } _type
        = Type::Empty;
    uint64_t _uVal{0};
    int64_t  _iVal{0};
    double   _dVal{0.0};
    template <typename T> struct UnknownTraits
    {
        static constexpr auto ValueType() { return Type::Unknown; }
        static void           Assign(Value& /* obj */, T const& /* val */) { throw std::logic_error("Unknown Value Type"); }
        static void           Get(const Value& /* obj */) {}
        static void           Check() { throw std::logic_error("Unknown Value Type"); }
    };

    template <typename T> struct SignedTraits
    {
        static constexpr auto ValueType() { return Type::Signed; }
        static void           Assign(Value& obj, T const& val) { obj._iVal = val; }
        static const auto&    Get(const Value& obj) { return obj._iVal; }
        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct UnsignedTraits
    {
        static constexpr auto ValueType() { return Type::Unsigned; }
        static void           Assign(Value& obj, T const& val) { obj._uVal = val; }
        static const auto&    Get(const Value& obj) { return obj._uVal; }
        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct DoubleTraits
    {
        static constexpr auto ValueType() { return Type::Double; }
        static void           Assign(Value& obj, T const& val) { obj._dVal = val; }
        static const auto&    Get(const Value& obj) { return obj._dVal; }
        static void           Check() {}
    };

    template <typename T> struct Supported
    {
        static constexpr bool value = ValueTraits<T>::ValueType() != Type::Unknown;
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
    template <typename T> Value(T val) : _type(ValueTraits<T>::ValueType()) { ValueTraits<T>::Assign(*this, val); }

    Value() = default;
    Value(std::nullptr_t) {}

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
};

template <typename T> struct ValueTraits : public Value::UnknownTraits<T>
{};

template <> struct ValueTraits<uint64_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct ValueTraits<uint32_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct ValueTraits<uint16_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct ValueTraits<uint8_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct ValueTraits<int64_t> : public Value::SignedTraits<int64_t>
{};
template <> struct ValueTraits<int32_t> : public Value::SignedTraits<int64_t>
{};
template <> struct ValueTraits<int16_t> : public Value::SignedTraits<int64_t>
{};
template <> struct ValueTraits<int8_t> : public Value::SignedTraits<int64_t>
{};

template <> struct ValueTraits<char> : public Value::SignedTraits<int64_t>
{};

template <> struct ValueTraits<bool> : public Value::UnsignedTraits<bool>
{};
template <> struct ValueTraits<double> : public Value::DoubleTraits<double>
{};

template <> struct ValueTraits<std::chrono::time_point<std::chrono::system_clock>>
{
    using time_point = std::chrono::time_point<std::chrono::system_clock>;
    static constexpr auto ValueType() { return Value::Type::Signed; }
    static void           Assign(Value& obj, time_point& val) { obj._iVal = val.time_since_epoch().count(); }
    static auto           Get(Value const& obj) { return time_point(time_point::duration(obj._iVal)); }
};

inline Value::operator ::size_t() const
{
    _check(ValueTraits<uint64_t>::ValueType());
    return static_cast<size_t>(ValueTraits<uint64_t>::Get(*this));
}
