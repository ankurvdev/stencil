#pragma once
#include <array>
#include <chrono>

struct UuidStr
{
    static constexpr size_t Size           = 38;
    char                    str[Size + 1]  = {};
    wchar_t                 wstr[Size + 1] = {};
    constexpr UuidStr() {}
    constexpr UuidStr(const char strin[Size]) : str()
    {
        for (size_t i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }
    constexpr UuidStr(const wchar_t strin[Size])
    {
        for (size_t i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }

    constexpr UuidStr(const std::string_view strin)
    {
        for (size_t i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }
    constexpr UuidStr(const std::wstring_view strin)
    {
        for (size_t i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }

    operator std::wstring_view() const { return wstr; }
    operator std::string_view() const { return str; }
    // operator wchar_t*() { return wstr; }
    // operator char*() { return str; }
};

struct Uuid
{
    constexpr Uuid(UuidStr const& str);
    constexpr Uuid() noexcept {}
    constexpr Uuid(Uuid const& obj) noexcept { data = obj.data; }
    constexpr Uuid(Uuid&& obj) noexcept { data = std::move(obj.data); }

    Uuid& operator=(Uuid const& uuid)
    {
        data = uuid.data;
        return *this;
    }
#define COMPARISONOP(op) \
    inline bool operator op(Uuid const& r) const { return data op r.data; }
    COMPARISONOP(==)
    COMPARISONOP(!=)
    COMPARISONOP(<=)
    COMPARISONOP(>=)
    COMPARISONOP(<)
    COMPARISONOP(>)

    UuidStr constexpr ToString() const;
    static Uuid constexpr FromString(UuidStr const& str) { return Uuid(str); }
    static Uuid Create();

    std::array<std::uint8_t, 16> data{};
};

template <typename T> struct UuidObjectT;

template <typename T> struct UuidBasedId
{
    static UuidBasedId<T>           Create() { return UuidBasedId<T>(Uuid::Create()); }
    static constexpr UuidBasedId<T> FromString(UuidStr str) { return UuidBasedId<T>(Uuid::FromString(str)); }

    inline bool operator==(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) == (const Uuid&)r; }
    inline bool operator!=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) != (const Uuid&)r; }
    inline bool operator<=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) <= (const Uuid&)r; }
    inline bool operator>=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) >= (const Uuid&)r; }
    inline bool operator<(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) < (const Uuid&)r; }
    inline bool operator>(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) > (const Uuid&)r; }

    bool Empty() const { return _guid == Uuid(); }
    bool Valid() const { return !Empty(); }
    void Validate()
    {
        if (!Valid())
        {
            _guid = Uuid::Create();
        }
    }
    operator UuidStr() { return _guid.ToString(); }

    UuidStr ToString() const { return _guid.ToString(); }
    UuidBasedId(const UuidBasedId<T>& obj) : _guid(obj._guid) {}
    UuidBasedId<T>& operator=(const UuidBasedId<T>& obj)
    {
        _guid = obj._guid;
        return *this;
    }
    constexpr UuidBasedId(UuidStr str) : _guid(str) {}

    private:
    UuidBasedId(Uuid guid) : _guid(guid) {}
    constexpr UuidBasedId() {}
    Uuid _guid;
         operator const Uuid&() const { return _guid; }

    public:
    static constexpr UuidBasedId<T> Invalid() { return UuidBasedId<T>(); }
    friend UuidObjectT<T>;

    friend std::hash<UuidBasedId<T>>;
};

namespace std
{
template <typename T> struct hash<UuidBasedId<T>>
{
    // TODO: Do a better job here
    std::size_t operator()(UuidBasedId<T> const& s) const noexcept { return std::hash<std::string_view>{}(s.ToString()); }
};
}    // namespace std

template <typename T> struct UuidObjectT
{
    using Id = UuidBasedId<T>;
    UuidObjectT() {}
    Id id = Id::Create();
};

#include <random>
inline Uuid Uuid::Create()
{
    Uuid                                    uuid;
    std::random_device                      r;
    std::default_random_engine              e1(r());
    std::uniform_int_distribution<uint16_t> dist(0, 255);

    for (size_t i = 0; i < uuid.data.size(); i++)
    {
        uuid.data[i] = (uint8_t)dist(e1);
    }

    return uuid;
}

inline UuidStr constexpr UuidToString(Uuid const& uuid)
{
    struct tohex
    {
        static constexpr char convert(uint8_t c) { return c < 10 ? ('0' + c) : ('a' + (c - 10)); }
        static constexpr char l(uint8_t c) { return convert(c & 0xfu); }
        static constexpr char h(uint8_t c) { return convert(static_cast<uint8_t>(c >> 4u)); }
    };

    char str[] = {'{',
                  tohex::h(uuid.data[0]),
                  tohex::l(uuid.data[0]),
                  tohex::h(uuid.data[1]),
                  tohex::l(uuid.data[1]),
                  tohex::h(uuid.data[2]),
                  tohex::l(uuid.data[2]),
                  tohex::h(uuid.data[3]),
                  tohex::l(uuid.data[3]),
                  '-',
                  tohex::h(uuid.data[4]),
                  tohex::l(uuid.data[4]),
                  tohex::h(uuid.data[5]),
                  tohex::l(uuid.data[5]),
                  '-',
                  tohex::h(uuid.data[6]),
                  tohex::l(uuid.data[6]),
                  tohex::h(uuid.data[7]),
                  tohex::l(uuid.data[7]),
                  '-',
                  tohex::h(uuid.data[8]),
                  tohex::l(uuid.data[8]),
                  tohex::h(uuid.data[9]),
                  tohex::l(uuid.data[9]),
                  '-',
                  tohex::h(uuid.data[10]),
                  tohex::l(uuid.data[10]),
                  tohex::h(uuid.data[11]),
                  tohex::l(uuid.data[11]),
                  tohex::h(uuid.data[12]),
                  tohex::l(uuid.data[12]),
                  tohex::h(uuid.data[13]),
                  tohex::l(uuid.data[13]),
                  tohex::h(uuid.data[14]),
                  tohex::l(uuid.data[14]),
                  tohex::h(uuid.data[15]),
                  tohex::l(uuid.data[15]),
                  '}',
                  0};

    return UuidStr(str);
}

inline UuidStr constexpr Uuid::ToString() const
{
    return UuidToString(*this);
}

inline constexpr Uuid::Uuid(UuidStr const& str)
{
    struct tobyte
    {
        static constexpr uint8_t convert(char c) { return (c < 'a' ? (c - '0') : (c - 'a' + 10)) & 0xfu; }
        static constexpr uint8_t join(char h, char l) { return static_cast<uint8_t>(convert(h) << 4u | convert(l)); }
    };

    data[0x0] = tobyte::join(str.str[1], str.str[2]);
    data[0x1] = tobyte::join(str.str[3], str.str[4]);
    data[0x2] = tobyte::join(str.str[5], str.str[6]);
    data[0x3] = tobyte::join(str.str[7], str.str[8]);

    data[0x4] = tobyte::join(str.str[10], str.str[11]);
    data[0x5] = tobyte::join(str.str[12], str.str[13]);

    data[0x6] = tobyte::join(str.str[15], str.str[16]);
    data[0x7] = tobyte::join(str.str[17], str.str[18]);

    data[0x8] = tobyte::join(str.str[20], str.str[21]);
    data[0x9] = tobyte::join(str.str[22], str.str[23]);

    data[0xa] = tobyte::join(str.str[25], str.str[26]);
    data[0xb] = tobyte::join(str.str[27], str.str[28]);
    data[0xc] = tobyte::join(str.str[29], str.str[30]);
    data[0xd] = tobyte::join(str.str[31], str.str[32]);
    data[0xe] = tobyte::join(str.str[33], str.str[34]);
    data[0xf] = tobyte::join(str.str[35], str.str[36]);
}
