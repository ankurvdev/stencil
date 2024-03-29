#pragma once
#include "protocol.h"
#include "serdes.h"
#include "visitor.h"

namespace Stencil
{
template <typename T, typename _Ts = void> struct Mutators;

template <typename T, typename _Ts = void> struct Accessors;

template <typename T> struct Mutators<std::vector<T>>
{
    using ListObj = T;
    static void add(std::vector<T>& arr, T&& obj) { arr.push_back(std::move(obj)); }

    static void remove(std::vector<T>& arr, size_t index)
    {
        arr.erase(arr.begin() + static_cast<typename std::vector<T>::difference_type>(index));
    }

    static auto& edit(std::vector<T>& arr, size_t index) { return arr[index]; }

    template <typename TLambda> static void remove_matching(std::vector<T>& arr, TLambda&& lambda)
    {
        for (auto it = arr.begin(); it != arr.end(); ++it)
        {
            if (lambda(*it))
            {
                arr.erase(it);
                return;
            }
        }
    }

    template <typename TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t> GenerateMutationData(uint8_t /* mutationIndex */, std::vector<T> const& /*fieldVal*/, T const& /* val */)
    {
#ifdef TODO1
        if (mutationIndex == 1)    // add
        {
            SerDes<T, TProtocol>::Writer writer;
            SerDes<T, TProtocol>::Write(val, writer);
            return writer.Reset();
        }
#endif

        throw std::logic_error("Whats a mutation");
    }

    template <typename TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t>
    GenerateMutationData(uint8_t /* mutationIndex */, std::vector<T> const& /*fieldVal*/, size_t const& /* val */)
    {
#ifdef TODO1
        if (mutationIndex == 2)    // add
        {
            SerDes<T, TProtocol>::Writer writer;
            SerDes<T, TProtocol>::Write(val, writer);
            return writer.Reset();
        }
#endif
        throw std::logic_error("Whats a mutation");
    }
};

template <typename T, size_t N> struct Mutators<std::array<T, N>>
{
    using TData   = std::array<T, N>;
    using ListObj = T;

    template <typename... TArgs> static void add(TArgs&&...) { throw std::logic_error("Invalid"); }

    template <typename... TArgs> static void remove(TArgs&&...) { throw std::logic_error("Invalid"); }

    template <typename... TArgs> static void edit(TArgs&&...) { throw std::logic_error("Invalid"); }

    template <typename TLambda> static void remove_matching(TData& /*arr*/, TLambda&& /*lambda*/) { throw std::logic_error("Invalid"); }

    template <typename TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t> GenerateMutationData(uint8_t /*mutationIndex*/, TData const& /*fieldVal*/, T const& /*val*/)
    {
        throw std::logic_error("Invalid");
    }

    template <typename TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t> GenerateMutationData(uint8_t /*mutationIndex*/, TData const& /*fieldVal*/, size_t const& /*val*/)
    {
        throw std::logic_error("Invalid");
    }
};

template <typename T> struct Accessors<std::vector<T>>
{
    template <typename TTransaction> static T const& at(TTransaction const& /*transaction*/, std::vector<T> const& arr, size_t index)
    {
        return arr.at(index);
    }
};

template <typename T, size_t N> struct Accessors<std::array<T, N>>
{
    template <typename TTransaction> static T const& at(TTransaction const& /*transaction*/, std::array<T, N> const& arr, size_t index)
    {
        return arr.at(index);
    }
};

}    // namespace Stencil
