#pragma once
#include "shared_string.h"
#include "visitor.h"

#include <span>

namespace Stencil
{

template <typename T, typename TProt> struct SerDes;

template <typename T, typename TProt> auto Serialize(T const& obj)
{
    typename TProt::OutType out;
    Stencil::SerDes<T, TProt>::Write(out, obj);
    return out;
}

template <typename T, typename TProt, typename TIn> T Deserialize(typename TIn& input)
{
    typename TProt::InType in{input};
    T                      obj;
    Stencil::SerDes<T, TProt>::Read(obj, in);
    return obj;
}

}    // namespace Stencil
