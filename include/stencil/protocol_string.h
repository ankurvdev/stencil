#pragma once
#include "primitives64bit.h"
#include "protocol.h"

SUPPRESS_WARNINGS_START
#include <fmt/chrono.h>
#include <fmt/ostream.h>
SUPPRESS_WARNINGS_END

SUPPRESS_WARNINGS_START
#pragma warning(disable : 4866)    // left to right evaluation not guaranteed
#pragma warning(disable : 4623)    // default constructor was implicitly defined as deleted
#pragma warning(disable : 4626)    // assignment operator was implicitly defined as deleted
#pragma warning(disable : 5027)    // move assignment operator was implicitly defined as deleted
#include <charconv>
#include <chrono>
#include <date/date.h>

SUPPRESS_WARNINGS_END

namespace Stencil
{
struct ProtocolString
{
    using InType  = std::string_view;
    using OutType = std::string;
};

template <typename T> struct SerDes<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& /*ctx*/, T const& /*obj*/) { TODO(""); }
    template <typename Context> static auto Read(T& /*obj*/, Context& /*ctx*/) { TODO(""); }
};

template <ConceptEnumPack T> struct SerDes<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& /*ctx*/, T const& /*obj*/) { TODO(""); }
    template <typename Context> static auto Read(T& obj, Context& ctx) { obj = T::FromString(ctx); }
};

template <ConceptEnum T> struct SerDes<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "{}", EnumTraits<T>::ToString(obj)); }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {

        using EnumTrait = typename Stencil::EnumTraits<T>;

        for (size_t i = 0; i < std::size(EnumTrait::Names); i++)
        {
            auto name = EnumTrait::Names[i];
            if (std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                    return std::tolower(l) == std::tolower(r);
                }))
            {
                obj = EnumTrait::ForIndex(i);
                return;
            }
        }
        throw std::invalid_argument("Invalid");

        if constexpr (ConceptEnumPack<T>) {}
    }
};

template <ConceptPrimitives64Bit T> struct SerDes<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        if constexpr (std::is_default_constructible_v<fmt::formatter<T>>) { fmt::print(ctx, "{}", obj); }
        else
        {
            fmt::print(ctx, "{}", Primitives64Bit::Traits<T>::Repr(obj));
        }
    }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            obj = false;
            if (ctx.size() == 0) return;
            if (ctx[0] == '1' || (ctx[0] == 'o' && ctx[1] == 'n') || (ctx[0] == 'O' && ctx[1] == 'N') || ctx[0] == 't' || ctx[0] == 'T'
                || ctx[0] == 'y' || ctx[0] == 'Y')
            {
                obj = true;
                return;
            }
        }
        else
        {
            using TRepr = decltype(Primitives64Bit::Traits<T>::Repr(T{}));
            TRepr ival;
            auto  result = std::from_chars(ctx.data(), ctx.data() + ctx.size(), ival);
            if (result.ec == std::errc::invalid_argument) throw std::logic_error("Cannot convert");
            // if (ival > std::numeric_limits<T>::max() || ival < std::numeric_limits<T>::min()) { throw std::logic_error("Out of range"); }
            obj = Primitives64Bit::Traits<T>::Convert(ival);
            return;
        }
    }
};

template <> struct SerDes<std::string, ProtocolString>
{
    using TObj = std::string;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj) { fmt::print(ctx, "{}", obj); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { obj = ctx; }
};

template <> struct SerDes<std::string_view, ProtocolString>
{
    using TObj = std::string;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj) { fmt::print(ctx, "{}", obj); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { obj = ctx; }
};

template <> struct SerDes<uuids::uuid, ProtocolString>
{
    using TObj = uuids::uuid;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj) { fmt::print(ctx, "{}", obj); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        auto opuuid = uuids::uuid::from_string(ctx);
        if (!opuuid.has_value()) { throw std::runtime_error("Invalid Uuid. Cannot parse"); }
        obj = opuuid.value();
    }
};

template <size_t N> struct SerDes<std::array<char, N>, ProtocolString>
{
    using TObj = std::array<char, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        auto str       = std::string_view(obj.data(), obj.size());
        auto nullindex = str.find(char{0}, 0);
        if (nullindex == str.npos) { fmt::print(ctx, "{}", str); }
        else if (nullindex == 0)
        {
            fmt::print(ctx, "null");
        }
        else
        {
            fmt::print(ctx, "{}", str.substr(0, nullindex));
        }
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        obj = {};
        if (!ctx.empty()) { std::copy(std::begin(ctx), std::end(ctx), std::begin(obj)); }
    }
};

template <> struct SerDes<std::wstring_view, ProtocolString>
{
    using TObj = std::wstring_view;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::string str;
        str.resize(obj.size());
        std::transform(obj.begin(), obj.end(), str.begin(), [](auto l) { return static_cast<char>(l); });
        SerDes<std::string, ProtocolString>::Write(ctx, str);
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::string str;
        SerDes<std::string, ProtocolString>::Read(str, ctx);
        if (obj.size() < str.size()) { throw std::logic_error("Not enough room"); }
        std::transform(str.begin(), str.end(), obj.begin(), [](auto l) { return static_cast<char>(l); });
    }
};

template <> struct SerDes<std::wstring, ProtocolString>
{
    using TObj = std::wstring;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        SerDes<std::wstring_view, ProtocolString>::Write(ctx, obj);
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::string str;
        SerDes<std::string, ProtocolString>::Read(str, ctx);
        obj.resize(str.size());
        std::transform(str.begin(), str.end(), obj.begin(), [](auto l) { return static_cast<char>(l); });
    }
};

template <typename TClock> struct SerDes<std::chrono::time_point<TClock>, ProtocolString>
{
    using TObj = std::chrono::time_point<TClock>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        fmt::print(ctx, "{:%FT%T%z}", obj);
        //    fmt::print(ctx, "{}", obj.time_since_epoch().count());
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::istringstream ss(ctx.data());
        ss >> date::parse("%FT%T%z", obj);
        // return std::mktime(&tm);
        // TODO("");
        //  uint64_t val;
        //  SerDes<uint64_t, ProtocolJsonVal>::Read(val, ctx);
        //  obj = std::chrono::time_point<TClock>(typename std::chrono::time_point<TClock>::duration(val));
    }
};

template <> struct SerDes<char, ProtocolString>
{
    using TObj = char;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        if (obj == 0)
            fmt::print(ctx, "0");
        else
            fmt::print(ctx, "{}", obj);
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        if (ctx.empty()) { obj = {}; }
        else
        {
            if (ctx.size() > 1) { throw std::logic_error("Invalid value"); }
            obj = ctx[0];
        }
    }
};

template <size_t N>
requires(N <= 4) struct SerDes<std::array<uint16_t, N>, ProtocolString>
{
    using TObj = std::array<uint16_t, N>;
    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        uint64_t val = 0;
        for (size_t i = N; i > 0; i--) { val = (val << 16) | obj.at(i - 1); }
        SerDes<uint64_t, ProtocolString>::Write(ctx, val);
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        uint64_t val = 0;
        SerDes<uint64_t, ProtocolString>::Read(val, ctx);
        for (size_t i = 0; i < N; i++)
        {
            obj.at(i) = static_cast<uint16_t>(val & 0xff);
            val       = (val >> 16);
        }
    }
};

}    // namespace Stencil
