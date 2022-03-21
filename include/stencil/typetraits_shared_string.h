#pragma once
#include "protocol_string.h"
#include "shared_string.h"
#include "typetraits.h"

template <typename T> struct Stencil::TypeTraits<shared_stringT<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::SerDes<shared_wstring, Stencil::ProtocolString>
{
    using TObj = shared_wstring;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        return SerDes<std::wstring, ProtocolString>::Write(ctx, obj.str());
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::wstring str;
        SerDes<std::wstring, ProtocolString>::Read(str, ctx);
        obj = shared_wstring::make(std::move(str));
    }
};

template <> struct Stencil::SerDes<shared_string, Stencil::ProtocolString>
{
    using TObj = shared_string;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        return SerDes<std::string, ProtocolString>::Write(ctx, obj.str());
    }

    template <typename Context> static auto Read(TObj& obj, Context& ctx)
    {
        std::string str;
        SerDes<std::string, ProtocolString>::Read(str, ctx);
        obj = shared_string::make(std::move(str));
    }
};
