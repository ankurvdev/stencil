#pragma once
#include "typetraits_std.h"

SUPPRESS_WARNINGS_START
#include <fmt/chrono.h>
#include <fmt/ostream.h>
SUPPRESS_WARNINGS_END

#include <charconv>

namespace Stencil
{

struct ProtocolString
{};

struct ProtocolJsonVal
{};

struct ProtocolBinary
{};

template <typename T, typename TProtocol> struct SerDes;

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
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "{}", obj); }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {

        if constexpr (std::is_same_v<T, bool>) { TODO(""); }
        else
        {
            T    ival;
            auto result = std::from_chars(ctx.data(), ctx.data() + ctx.size(), ival);
            if (result.ec == std::errc::invalid_argument) throw std::logic_error("Cannot convert");
            if (ival > std::numeric_limits<T>::max() || ival < std::numeric_limits<T>::min()) { throw std::logic_error("Out of range"); }
            obj = static_cast<T>(ival);
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

template <size_t N> struct SerDes<std::array<char, N>, ProtocolJsonVal>
{
    using TObj = std::array<char, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        auto str       = std::string_view(obj.data(), obj.size());
        auto nullindex = str.find(char{0}, 0);
        if (nullindex == str.npos) { fmt::print(ctx, "\"{}\"", str); }
        else if (nullindex == 0)
        {
            fmt::print(ctx, "null");
        }
        else
        {
            fmt::print(ctx, "\"{}\"", str.substr(0, nullindex));
        }
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        obj = {};
        if (!ctx.empty()) { std::copy(std::begin(ctx), std::end(ctx), std::begin(obj)); }
    }
};

template <size_t N> struct SerDes<std::array<uint16_t, N>, ProtocolJsonVal>
{
    using TObj = std::array<uint16_t, N>;
    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        if constexpr (N <= 4)
        {
            uint64_t val = 0;
            for (size_t i = N; i > 0; i--) { val = (val << 16) | obj.at(i - 1); }
            SerDes<uint64_t, ProtocolJsonVal>::Write(ctx, val);
        }
        else
        {
            fmt::print(ctx, "[");
            bool first = true;
            Visitor<std::array<uint16_t, N>>::VisitAllIndicies(obj, [&](auto, auto& v) {
                using ValueType = std::remove_cvref_t<decltype(v)>;
                if (!first) fmt::print(ctx, ",");
                SerDes<ValueType, ProtocolJsonVal>::Write(ctx, v);
                first = false;
            });
            fmt::print(ctx, "]");
        }
    }
    template <typename Context> static auto Read(TObj& /*obj*/, Context& /*ctx*/) { TODO(""); }
};

template <> struct SerDes<uuids::uuid, ProtocolJsonVal>
{
    using TObj = uuids::uuid;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj) { fmt::print(ctx, "\"{}\"", obj); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { SerDes<uuids::uuid, ProtocolString>::Read(obj, ctx); }
};

template <typename TClock> struct SerDes<std::chrono::time_point<TClock>, ProtocolJsonVal>
{
    using TObj = std::chrono::time_point<TClock>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        fmt::print(ctx, "\"{:%FT%T%z}\"", obj);
        //    fmt::print(ctx, "{}", obj.time_since_epoch().count());
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::istringstream ss(ctx.data());
        ss >> std::chrono::parse("%FT%T%z", obj);
        // return std::mktime(&tm);
        // TODO("");
        //  uint64_t val;
        //  SerDes<uint64_t, ProtocolJsonVal>::Read(val, ctx);
        //  obj = std::chrono::time_point<TClock>(typename std::chrono::time_point<TClock>::duration(val));
    }
};

template <> struct SerDes<char, ProtocolJsonVal>
{
    using TObj = char;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        if (obj == 0)
            fmt::print(ctx, "null");
        else
            fmt::print(ctx, "\"{}\"", obj);
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        if (ctx.empty()) { obj = {}; }
        else
        {
            if (ctx.size() > 1) { throw std::logic_error("Invalid value"); }
            obj = ctx[0];

            // SerDes<TObj, ProtocolString>::Read(obj, ctx);
        }
    }
};

template <ConceptEnumPack T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"");
        SerDes<T, ProtocolString>::Write(ctx, obj);
        fmt::print(ctx, "\"");
    }
    template <typename Context> static auto Read(T& obj, Context& ctx) { SerDes<T, ProtocolString>::Read(obj, ctx); }
};

template <ConceptEnum T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"");
        SerDes<T, ProtocolString>::Write(ctx, obj);
        fmt::print(ctx, "\"");
    }
    template <typename Context> static auto Read(T& obj, Context& ctx) { SerDes<T, ProtocolString>::Read(obj, ctx); }
};

template <ConceptPrimitives64Bit T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { SerDes<T, ProtocolString>::Write(ctx, obj); }
    template <typename Context> static auto Read(T& obj, Context& ctx) { SerDes<T, ProtocolString>::Read(obj, ctx); }
};

template <> struct SerDes<std::string, ProtocolJsonVal>
{
    using TObj = std::string;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        fmt::print(ctx, "\"");
        SerDes<TObj, ProtocolString>::Write(ctx, obj);
        fmt::print(ctx, "\"");
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { SerDes<TObj, ProtocolString>::Read(obj, ctx); }
};

template <> struct SerDes<std::wstring, ProtocolJsonVal>
{
    using TObj = std::wstring;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::string str;
        str.resize(obj.size());
        std::transform(obj.begin(), obj.end(), str.begin(), [](auto l) { return static_cast<char>(l); });
        SerDes<std::string, ProtocolJsonVal>::Write(ctx, str);
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::string str;
        SerDes<std::string, ProtocolJsonVal>::Read(str, ctx);
        obj.resize(str.size());
        std::transform(str.begin(), str.end(), obj.begin(), [](auto l) { return static_cast<char>(l); });
    }
};

template <> struct SerDes<std::wstring_view, ProtocolJsonVal>
{
    using TObj = std::wstring_view;

    template <typename Context> static auto Write(Context& /*ctx*/, TObj const& /*obj*/) { TODO(""); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { return SerDes<TObj, ProtocolString>::Read(obj, ctx); }
};

// Protocol String

// Protocol JsonVal

// Protocol Binary

}    // namespace Stencil