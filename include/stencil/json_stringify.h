#pragma once
#include "visitor.h"

#include <deque>

#pragma warning(push, 3)
#include <fmt/chrono.h>
#include <fmt/ostream.h>
#pragma warning(pop)

// Stringify
namespace Stencil::Json
{

template <typename TContext, typename T> void _WriteTo(TContext& ctx, T const& obj)
{
    std::ostream& ostr = ctx;
    fmt::print(ostr, "{}", obj);
}

template <typename T> struct _Stringifier;

template <Stencil::ConceptIndexable T> struct _Stringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto const& k, auto const& v) {
            using KeyType   = std::remove_cvref_t<decltype(k)>;
            using ValueType = std::remove_cvref_t<decltype(v)>;
            if (!first) _WriteTo(ctx, ',');
            _Stringifier<KeyType>::Write(ctx, k);
            _WriteTo(ctx, ':');
            _Stringifier<ValueType>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, '}');
    }
};

template <Stencil::ConceptIterableNotIndexable T> struct _Stringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '[');
        bool first = true;
        Visitor<T>::VisitAll(obj, [&](auto& v) {
            using ValueType = std::remove_cvref_t<decltype(v)>;
            if (!first) WriteTo(ctx, ',');
            _Stringifier<ValueType>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, ']');
    }
};

template <typename T> struct _PrimitiveStringifier
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "\"{}\"", obj); }
};

template <Stencil::ConceptPrimitiveOnly T> struct _Stringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { return _PrimitiveStringifier<T>::Write(ctx, obj); }
};

template <Stencil::ConceptEnum T> struct _PrimitiveStringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"{}\"", Stencil::EnumTraits<T>::ToString(obj));
    }
};

template <typename TClock> struct _PrimitiveStringifier<std::chrono::time_point<TClock>>
{
    template <typename Context> static auto Write(Context& ctx, std::chrono::time_point<TClock> const& obj)
    {
        fmt::print(ctx, "\"{:%FT%T%z}\"", obj);
    }
};

template <> struct _PrimitiveStringifier<char>
{
    template <typename Context> static auto Write(Context& ctx, char const& obj)
    {
        if (obj == 0)
            fmt::print(ctx, "null");
        else
            fmt::print(ctx, "\"{}\"", obj);
    }
};

template <typename T> static std::string Stringify(T const& obj)
{
    std::stringstream sstr;
    _Stringifier<T>::Write(sstr, obj);
    return sstr.str();
}

template <typename T, typename TOutCtx> static void StringifyTo(T const& /*obj*/, TOutCtx& /*octx*/)
{
    throw std::logic_error("Not Implemented");
}
}    // namespace Stencil::Json
