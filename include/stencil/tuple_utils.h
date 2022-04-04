#pragma once
#include <tuple>

namespace Stencil
{

template <typename TTup, typename TLambda, size_t TIndex = 0> constexpr void TupleVisitAt(TTup& tup, const size_t idx, TLambda&& lambda)
{
    if constexpr (TIndex == std::tuple_size<TTup>::value) { throw std::out_of_range("Exceeded maximum tuple range"); }
    else
    {
        if (TIndex == idx)
        {
            lambda(std::get<TIndex>(tup));
            return;
        }
        return TupleVisitAt<TTup, TLambda, TIndex + 1>(tup, idx, std::forward<TLambda>(lambda));
    }
}

template <typename TI, typename T, typename... Ts> constexpr size_t TupleIndexOf(size_t index = 0)
{
    if constexpr (std::is_same<TI, T>::value) { return index; }
    else
    {
        if constexpr (sizeof...(Ts) == 0) { static_assert(sizeof...(Ts) != 0, "Cannot Match anything in tuple"); }
        else
        {
            return TupleIndexOf<TI, Ts...>(index + 1);
        }
    }
}
}    // namespace Stencil
