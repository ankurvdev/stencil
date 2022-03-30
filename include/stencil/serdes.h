#pragma once
#include "shared_string.h"
#include "visitor.h"

#include <span>

namespace Stencil
{

template <typename T, typename TProt> struct SerDes;
#if 0
{
    template <typename T, typename TIn> static void  Read(T& obj, TIn& in);
    template <typename T, typename TOut> static void Write(TOut& out, T const& obj);
};
#endif

template <typename T, typename TProt> auto Serialize(T const& obj)
{
    typename TProt::OutType out;
    Stencil::SerDes<T, TProt>::Write(out, obj);
    return out;
}

template <typename T, typename TProt> T Deserialize(typename TProt::InType const& in)
{
    T obj;
    Stencil::SerDes<T, TProt>::Read(obj, in);
    return obj;
}

}    // namespace Stencil
