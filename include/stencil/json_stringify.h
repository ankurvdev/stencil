#pragma once
#include "protocol.h"
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
namespace Stencil
{

template <typename TContext, typename T> void _WriteTo(TContext& ctx, T const& obj)
{
    std::ostream& ostr = ctx;
    fmt::print(ostr, "{}", obj);
}

template <typename T, typename TProt> struct SerDes;

static_assert(!Stencil::Category::IsIndexable<uint64_t>());

template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto const& k, auto const& v) {
            if (!first) _WriteTo(ctx, ',');
            _WriteTo(ctx, '\"');
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ctx, k);
            _WriteTo(ctx, '\"');
            _WriteTo(ctx, ':');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, '}');
    }
};

template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolString>
{
    template <typename Context> static auto Write(Context& /*ctx*/, T const& /*obj*/) { TODO(""); }
};

template <Stencil::ConceptIterable T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        _WriteTo(ctx, '[');
        bool first = true;
        Visitor<T>::VisitAllIndicies(obj, [&](auto, auto& v) {
            if (!first) _WriteTo(ctx, ',');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, ']');
    }
};

namespace Json
{
template <typename T> static std::string Stringify(T const& obj)
{
    std::stringstream sstr;
    SerDes<T, ProtocolJsonVal>::Write(sstr, obj);
    return sstr.str();
}
}    // namespace Json
}    // namespace Stencil
