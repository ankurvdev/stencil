#pragma once
#include "typetraits_std.h"
#include "typetraits_variant.h"
#include "visitor.h"

#include <array>
#include <deque>

SUPPRESS_WARNINGS_START
#include <fmt/chrono.h>
#include <fmt/ostream.h>
SUPPRESS_WARNINGS_END

// Stringify
namespace Stencil::Json
{

template <typename TContext, typename T> void _WriteTo(TContext& ctx, T const& obj)
{
    std::ostream& ostr = ctx;
    fmt::print(ostr, "{}", obj);
}

struct ProtocolString
{};

struct ProtocolJsonVal
{};

struct ProtocolBinary
{};

template <typename T, typename TProt> struct Serializer;

template <Stencil::ConceptIndexable T> struct Serializer<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto const& k, auto const& v) {
            if (!first) _WriteTo(ctx, ',');
            Serializer<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ctx, k);
            _WriteTo(ctx, ':');
            Serializer<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, '}');
    }
};
template <Stencil::ConceptIndexable T> struct Serializer<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { TODO(""); };
};

template <Stencil::ConceptIterableNotIndexable T> struct Serializer<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '[');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto, auto& v) {
            if (!first) _WriteTo(ctx, ',');
            Serializer<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, ']');
    }
};

template <typename T, typename TProt> struct _PrimitiveStringifier
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "\"{}\"", obj); }
};

template <Stencil::ConceptPrimitive T> struct Serializer<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        return _PrimitiveStringifier<T, ProtocolJsonVal>::Write(ctx, obj);
    }
};

template <Stencil::ConceptPrimitive T> struct Serializer<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        return _PrimitiveStringifier<T, ProtocolString>::Write(ctx, obj);
    }
};

template <size_t N> struct Serializer<std::array<char, N>, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, std::array<char, N> const& obj)
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
};

template <size_t N> struct Serializer<std::array<uint16_t, N>, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, std::array<uint16_t, N> const& obj)
    {
        if constexpr (N <= 4)
        {
            uint64_t val = 0;
            for (size_t i = N; i > 0; i--) { val = (val << 16) | obj.at(i - 1); }
            Serializer<uint64_t, ProtocolJsonVal>::Write(ctx, val);
        }
        else
        {
            _WriteTo(ctx, '[');
            bool first = true;
            Visitor<std::array<uint16_t, N>>::VisitAllIndicies(obj, [&](auto, auto& v) {
                using ValueType = std::remove_cvref_t<decltype(v)>;
                if (!first) _WriteTo(ctx, ',');
                Serializer<ValueType, ProtocolJsonVal>::Write(ctx, v);
                first = false;
            });
            _WriteTo(ctx, ']');
        }
    }
};

template <Stencil::ConceptEnum T> struct _PrimitiveStringifier<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"{}\"", Stencil::EnumTraits<T>::ToString(obj));
    }
};

template <Stencil::ConceptEnum T> struct _PrimitiveStringifier<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"{}\"", Stencil::EnumTraits<T>::ToString(obj));
    }
};

template <ConceptPrimitives64Bit T> struct _PrimitiveStringifier<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "{}", obj); }
};

template <typename TClock> struct _PrimitiveStringifier<std::chrono::time_point<TClock>, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, std::chrono::time_point<TClock> const& obj)
    {
        fmt::print(ctx, "\"{:%FT%T%z}\"", obj);
    }
};

template <> struct _PrimitiveStringifier<char, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, char const& obj)
    {
        if (obj == 0)
            fmt::print(ctx, "null");
        else
            fmt::print(ctx, "\"{}\"", obj);
    }
};

template <> struct _PrimitiveStringifier<std::wstring, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, std::wstring const& obj)
    {
        std::string str;
        str.resize(obj.size());
        std::transform(obj.begin(), obj.end(), str.begin(), [](auto l) { return static_cast<char>(l); });
        _PrimitiveStringifier<std::string, ProtocolJsonVal>::Write(ctx, str);
    }
};

template <> struct _PrimitiveStringifier<std::wstring_view, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& /*ctx*/, std::wstring_view const& /*obj*/) { TODO(""); }
};

template <typename T> static std::string Stringify(T const& obj)
{
    std::stringstream sstr;
    Serializer<T, ProtocolJsonVal>::Write(sstr, obj);
    return sstr.str();
}

}    // namespace Stencil::Json
