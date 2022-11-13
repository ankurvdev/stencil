#pragma once

namespace Stencil
{

template <typename T1, typename T2> struct Comparator
{
    static bool AreEqual(T1 const& t1, T2 const& t2) { return t1 == t2; }
};

template <> struct Comparator<float, float>
{
    static bool AreEqual(float const& t1, float const& t2) { return std::fabs(t1 - t2) <= std::numeric_limits<float>::epsilon(); }
};

template <> struct Comparator<double, double>
{
    static bool AreEqual(double const& t1, double const& t2) { return std::fabs(t1 - t2) <= std::numeric_limits<double>::epsilon(); }
};

template <typename T1, typename T2> bool AreEqual(T1 const& t1, T2 const& t2)
{
    return Comparator<T1, T2>::AreEqual(t1, t2);
}

};    // namespace Stencil
