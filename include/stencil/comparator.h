#pragma once
#include "CommonMacros.h"

#include "shared_string.h"
#include "shared_tree.h"
#include <unordered_map>
#include <vector>

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

template <typename T> struct Comparator<std::vector<T>, std::vector<T>>
{
    static bool AreEqual(std::vector<T> const& /* t1 */, std::vector<T> const& /* t2 */) { throw std::logic_error("Not implemented"); }
};

template <typename T> struct Comparator<shared_tree<T>, shared_tree<T>>
{
    static bool AreEqual(shared_tree<T> const& /* t1 */, shared_tree<T> const& /* t2 */) { throw std::logic_error("Not implemented"); }
};

template <typename K, typename V> struct Comparator<std::unordered_map<K, V>, std::unordered_map<K, V>>
{
    static bool AreEqual(std::unordered_map<K, V> const& /* t1 */, std::unordered_map<K, V> const& /* t2 */) { throw std::logic_error("Not implemented"); }
};

template <typename T1, typename T2> bool AreEqual(T1 const& t1, T2 const& t2)
{
    return Comparator<T1, T2>::AreEqual(t1, t2);
}

};    // namespace Stencil
