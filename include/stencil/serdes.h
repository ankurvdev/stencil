#pragma once
#include "shared_string.h"
#include "visitor.h"

#include <span>

namespace Stencil
{

template <typename T, typename TProt> struct SerDes;

template <typename TProt, typename T> auto Serialize(T const& obj)
{
    typename TProt::OutType out;
    Stencil::SerDes<T, TProt>::Write(out, obj);
    return out;
}

template <typename T, typename TProt, typename TIn> T Deserialize(TIn& input)
{
    typename TProt::InType in{input};
    T                      obj;
    Stencil::SerDes<T, TProt>::Read(obj, in);
    return obj;
}

template <typename TProt, typename TObj, typename TVal> void SerDesRead(TObj& obj, TVal& val)
{
    SerDes<TObj, TProt>::Read(obj, val);
}

template <typename TProt, typename TObj, typename TVal> void SerDesWrite(TObj& obj, TVal& val)
{
    SerDes<TObj, TProt>::Write(obj, val);
}

}    // namespace Stencil
