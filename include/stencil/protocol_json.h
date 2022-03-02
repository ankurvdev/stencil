#pragma once
#include "protocol.h"
#include "protocol_string.h"

namespace Stencil
{
template <size_t N> struct SerDes<std::array<char, N>, ProtocolJsonVal>
{
    using TObj = std::array<char, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        if (obj[0] == 0) { fmt::print(ctx, "null"); }
        else
        {
            fmt::print(ctx, "\"");
            SerDes<TObj, ProtocolString>::Write(ctx, obj);
            fmt::print(ctx, "\"");
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
        fmt::print(ctx, "\"");
        SerDes<TObj, ProtocolString>::Write(ctx, obj);
        fmt::print(ctx, "\"");
    }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { SerDes<TObj, ProtocolString>::Read(obj, ctx); }
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
        fmt::print(ctx, "\"");
        SerDes<TObj, ProtocolString>::Write(ctx, obj);
        fmt::print(ctx, "\"");
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx) { SerDes<TObj, ProtocolString>::Read(obj, ctx); }
};

template <> struct SerDes<std::wstring_view, ProtocolJsonVal>
{
    using TObj = std::wstring_view;

    template <typename Context> static auto Write(Context& /*ctx*/, TObj const& /*obj*/) { TODO(""); }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { return SerDes<TObj, ProtocolString>::Read(obj, ctx); }
};
}    // namespace Stencil