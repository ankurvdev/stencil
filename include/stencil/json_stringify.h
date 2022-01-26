#pragma once
#include "visitor.h"

#include <deque>

// Stringify
namespace Stencil::Json
{

template <typename TContext, typename T> void _WriteTo(TContext, T const& obj)
{
    throw std::logic_error("Not Implemented");
}
template <typename T> struct _Stringifier;

template <Stencil::ConceptIndexable T> struct _Stringifier<T>
{
    template <typename Context, /*Stencil::ConceptRVisitor<T>*/ typename TRVisitor> auto Write(Context& ctx, TRVisitor const& rvisitor)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        rvisitor.VisitAllIndicies([&](auto& k, auto& v) {
            _StringifyForVisitor(ctx, k);
            _WriteTo(ctx, ':');
            _StringifyForVisitor(ctx, v);
            first = false;
        });
        WriteTo(ctx, '}');
    }
};

template <Stencil::ConceptIterableNotIndexable T> struct _Stringifier<T>
{
    template <typename Context, /*Stencil::ConceptRVisitor<T>*/ typename TRVisitor> auto Write(Context& ctx, TRVisitor const& rvisitor)
    {
        _WriteTo(ctx, '[');
        bool first = true;

        rvisitor.VisitAll([&](auto& v) {
            if (!first) WriteTo(ctx, ',');
            _StringifyForVisitor(ctx, v);
            first = false;
        });

        _WriteTo(ctx, ']');
    }
};

template <Stencil::ConceptPrimitiveOnly T> struct _Stringifier<T>
{
    template <typename Context, /*Stencil::ConceptRVisitor<T>*/ typename TRVisitor>
    auto Write(Context& /*ctx*/, TRVisitor const& /*rvisitor*/)
    {
        throw std::logic_error("Not Implemented");
    }
};

template <typename T> static std::string Stringify(T const& /*obj*/)
{
    throw std::logic_error("Not Implemented");
}

template <typename T, typename TOutCtx> static void StringifyTo(T const& /*obj*/, TOutCtx& /*octx*/)
{
    throw std::logic_error("Not Implemented");
}
}    // namespace Stencil::Json
