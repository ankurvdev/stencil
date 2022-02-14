#pragma once
#include "visitor.h"

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

template <typename T> struct _Stringifier;

template <Stencil::ConceptIndexable T> struct _Stringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto const& k, auto const& v) {
            if (!first) _WriteTo(ctx, ',');
            _Stringifier<std::remove_cvref_t<decltype(k)>>::Write(ctx, k);
            _WriteTo(ctx, ':');
            _Stringifier<std::remove_cvref_t<decltype(v)>>::Write(ctx, v);
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
        Visitor<T>::VisitAllIndicies(obj, [&](auto, auto& v) {
            if (!first) _WriteTo(ctx, ',');
            _Stringifier<std::remove_cvref_t<decltype(v)>>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, ']');
    }
};

template <typename T> struct _PrimitiveStringifier
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { fmt::print(ctx, "\"{}\"", obj); }
};

template <Stencil::ConceptPrimitive T> struct _Stringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { return _PrimitiveStringifier<T>::Write(ctx, obj); }
};

template <size_t N> struct _Stringifier<std::array<char, N>>
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

template <size_t N> struct _Stringifier<std::array<uint16_t, N>>
{
    template <typename Context> static auto Write(Context& ctx, std::array<uint16_t, N> const& obj)
    {
        if constexpr (N <= 4)
        {
            uint64_t val = 0;
            for (size_t i = N; i > 0; i--) { val = (val << 16) | obj.at(i - 1); }
            _Stringifier<uint64_t>::Write(ctx, val);
        }
        else
        {
            _WriteTo(ctx, '[');
            bool first = true;
            Visitor<std::array<uint16_t, N>>::VisitAllIndicies(obj, [&](auto, auto& v) {
                using ValueType = std::remove_cvref_t<decltype(v)>;
                if (!first) _WriteTo(ctx, ',');
                _Stringifier<ValueType>::Write(ctx, v);
                first = false;
            });
            _WriteTo(ctx, ']');
        }
    }
};

template <Stencil::ConceptEnum T> struct _PrimitiveStringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        fmt::print(ctx, "\"{}\"", Stencil::EnumTraits<T>::ToString(obj));
    }
};
template <ConceptValue T> struct _PrimitiveStringifier<T>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        //        if constexpr (std::is_same_v<T, std::chrono::time_point<std::chrono::system_clock>>) { fmt::print(ctx, "\"{:%FT%T%z}\"",
        //        obj); }
        //       else
        //{
        fmt::print(ctx, "{}", obj);
        //}
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

template <> struct _PrimitiveStringifier<std::wstring>
{
    template <typename Context> static auto Write(Context& ctx, std::wstring const& obj)
    {
        std::string str;
        str.resize(obj.size());
        std::transform(obj.begin(), obj.end(), str.begin(), [](auto l) { return static_cast<char>(l); });
        _PrimitiveStringifier<std::string>::Write(ctx, str);
    }
};

template <> struct _PrimitiveStringifier<std::wstring_view>
{
    template <typename Context> static auto Write(Context& /*ctx*/, std::wstring_view const& /*obj*/) { TODO(""); }
};

template <typename T> static std::string Stringify(T const& obj)
{
    std::stringstream sstr;
    _Stringifier<T>::Write(sstr, obj);
    return sstr.str();
}

//template <typename T, typename TOutCtx> static void StringifyTo(T const& /*obj*/, TOutCtx& /*octx*/)
//{
//    throw std::logic_error("Not Implemented");
//}
}    // namespace Stencil::Json
