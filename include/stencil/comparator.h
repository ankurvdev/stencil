#pragma once

namespace Stencil
{

template <typename T1, typename T2> struct Comparator;

template <typename T1, typename T2> bool AreEqual(T1 const& t1, T2 const& t2)
{
    return Comparator<T1, T2>::AreEqual(t1, t2);
}

};    // namespace Stencil
