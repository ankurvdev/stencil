#pragma once
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
        static consteval unsigned _GetWidth(unsigned x) { return x < 2 ? x : 1 + _GetWidth(x >> 1); }

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
        static constexpr bool IsFloat(Type t) { return t.category == Category::Float; }
        static constexpr bool IsSigned(Type t) { return t.category == Category::Signed; }
        static constexpr bool IsUnsigned(Type t) { return t.category == Category::Unsigned; }

        constexpr bool operator==(Type const& t) { return width == t.width && category == t.category; }
        constexpr bool operator!=(Type const& t) { return width != t.width || category != t.category; }
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
        static constexpr auto ValueType() { return Type::Of<T>(); };
        static void           Assign(Value& obj, T const& val) { obj._iVal = val; }
        static const auto&    Get(const Value& obj) { return obj._iVal; }
        static T              Convert(int64_t val) { return static_cast<T>(val); }
        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct UnsignedTraits
    {
        static constexpr auto ValueType() { return Type::Of<T>(); };
        static void           Assign(Value& obj, T const& val) { obj._uVal = val; }
        static const auto&    Get(const Value& obj) { return obj._uVal; }
        static T              Convert(uint64_t val) { return static_cast<T>(val); }

        // static void Check() { if (obj._iVal < std::numeric_limits<T>::min() || obj.iVal >
        // std::numeric_limits<T>::max()); throw 1; }
    };

    template <typename T> struct DoubleTraits
    {
        static constexpr auto ValueType() { return Type::Of<T>(); };
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
        if (_type.category == ValueTraits<T>::ValueType().category)
        {
            switch (_type.category)
            {
            case Value::Type::Category::Float: return ValueTraits<T>::Convert(_dVal);
            case Value::Type::Category::Signed: return ValueTraits<T>::Convert(_iVal);
            case Value::Type::Category::Unsigned: return ValueTraits<T>::Convert(_uVal);
            default: throw std::logic_error("Unsupported Cast");
            }
        }
        else
        {
            switch (ValueTraits<T>::ValueType().category)
            {
            case Value::Type::Category::Float:
            {
                switch (_type.category)
                {
                case Value::Type::Category::Signed: ValueTraits<T>::Convert(static_cast<double>(_iVal));
                case Value::Type::Category::Unsigned: ValueTraits<T>::Convert(static_cast<double>(_uVal));
                default: throw std::logic_error("Unsupported Cast");
                }
            }
            break;
            case Value::Type::Category::Signed:
            {
                switch (_type.category)
                {
                case Value::Type::Category::Float: ValueTraits<T>::Convert(static_cast<int64_t>(_dVal));
                case Value::Type::Category::Unsigned: ValueTraits<T>::Convert(static_cast<int64_t>(_uVal));
                default: throw std::logic_error("Unsupported Cast");
                }
            }
            break;
            case Value::Type::Category::Unsigned:
            {
                switch (_type.category)
                {
                case Value::Type::Category::Float: ValueTraits<T>::Convert(static_cast<uint64_t>(_dVal));
                case Value::Type::Category::Signed: ValueTraits<T>::Convert(static_cast<uint64_t>(_iVal));
                default: throw std::logic_error("Unsupported Cast");
                }
            }
            break;
            default: throw std::logic_error("Unsupported Cast");
            }
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
template <> struct Value::ValueTraits<uint32_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct Value::ValueTraits<uint16_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct Value::ValueTraits<uint8_t> : public Value::UnsignedTraits<uint64_t>
{};
template <> struct Value::ValueTraits<int64_t> : public Value::SignedTraits<int64_t>
{};
template <> struct Value::ValueTraits<int32_t> : public Value::SignedTraits<int64_t>
{};
template <> struct Value::ValueTraits<int16_t> : public Value::SignedTraits<int64_t>
{};
template <> struct Value::ValueTraits<int8_t> : public Value::SignedTraits<int64_t>
{};

template <> struct Value::ValueTraits<char> : public Value::SignedTraits<int64_t>
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
    static constexpr auto ValueType() { return Value::Type{.width = 4, .category = Value::Type::Category::Unsigned}; }
    static void           Assign(Value& obj, time_point& val) { obj._iVal = val.time_since_epoch().count(); }
    static auto           Get(Value const& obj) { return time_point(time_point::duration(obj._iVal)); }
    static auto           Convert(uint64_t val) { return time_point(time_point::duration(val)); }
};

inline Value::operator ::size_t() const
{
    _check(ValueTraits<uint64_t>::ValueType());
    return static_cast<size_t>(ValueTraits<uint64_t>::Get(*this));
}
template <typename T>
concept ConceptValue = (Value::ValueTraits<T>::ValueType() != Value::Type::Unknown());

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
