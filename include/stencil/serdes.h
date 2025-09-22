#pragma once

#include "CommonMacros.h"
namespace Stencil
{

template <typename T>
concept ConceptProtocol = requires {
    typename T::InType;
    typename T::OutType;
};

template <typename T, ConceptProtocol TProt> struct SerDes;

SUPPRESS_WARNINGS_START
SUPPRESS_CLANG_WARNING("-Wnrvo")
template <ConceptProtocol TProt, typename T> auto Serialize(T const& obj)
{
    typename TProt::OutType out;
    Stencil::SerDes<T, TProt>::Write(out, obj);
    return out;
}
SUPPRESS_WARNINGS_END

template <typename T, ConceptProtocol TProt, typename TIn> T Deserialize(TIn& input)
{
    typename TProt::InType in{input};
    T                      obj;
    Stencil::SerDes<T, TProt>::Read(obj, in);
    return obj;
}

template <ConceptProtocol TProt, typename TObj, typename TVal> void SerDesRead(TObj& obj, TVal& val)
{
    SerDes<TObj, TProt>::Read(obj, val);
}

template <ConceptProtocol TProt, typename TObj, typename TVal> void SerDesWrite(TObj& obj, TVal& val)
{
    SerDes<TObj, TProt>::Write(obj, val);
}

}    // namespace Stencil
