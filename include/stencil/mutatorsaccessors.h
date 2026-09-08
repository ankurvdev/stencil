#pragma once
#include "protocol_binary.h"
#include "serdes.h"

namespace Stencil
{
template <typename T, typename Ts = void> struct Mutators;

template <typename T, typename Ts = void> struct Accessors;

template <typename T> struct Mutators<std::vector<T>>
{
    using ListObj = T;
    static void add(std::vector<T>& arr, T&& obj) { arr.push_back(std::move(obj)); } //NOLINT

    static void remove(std::vector<T>& arr, size_t index) //NOLINT
    {
        arr.erase(arr.begin() + static_cast<std::vector<T>::difference_type>(index));
    }

    static auto& edit(std::vector<T>& arr, size_t index) { return arr[index]; } //NOLINT

    template <typename TLambda> static void RemoveMatching(std::vector<T>& arr, TLambda&& lambda)
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

    template <ConceptProtocol TProtocol = Stencil::ProtocolBinary>
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

    template <ConceptProtocol TProtocol = Stencil::ProtocolBinary>
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

    template <typename... TArgs> static void add(TArgs&&... /*unused*/) { throw std::logic_error("Invalid"); }// NOLINT
    template <typename... TArgs> static void remove(TArgs&&... /*unused*/) { throw std::logic_error("Invalid"); } //NOLINT
    template <typename... TArgs> static void edit(TArgs&&... /*unused*/) { throw std::logic_error("Invalid"); } //NOLINT

    template <typename TLambda> static void RemoveMatching(TData& /*arr*/, TLambda&& /*lambda*/) { throw std::logic_error("Invalid"); }

    template <ConceptProtocol TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t> GenerateMutationData(uint8_t /*mutationIndex*/, TData const& /*fieldVal*/, T const& /*val*/)
    {
        throw std::logic_error("Invalid");
    }

    template <ConceptProtocol TProtocol = Stencil::ProtocolBinary>
    static std::vector<uint8_t> GenerateMutationData(uint8_t /*mutationIndex*/, TData const& /*fieldVal*/, size_t const& /*val*/)
    {
        throw std::logic_error("Invalid");
    }
};

template <typename T> struct Accessors<std::vector<T>>
{
    template <typename TTransaction> static T const& At(TTransaction const& /*transaction*/, std::vector<T> const& arr, size_t index)
    {
        return arr.at(index);
    }
};

template <typename T, size_t N> struct Accessors<std::array<T, N>>
{
    template <typename TTransaction> static T const& At(TTransaction const& /*transaction*/, std::array<T, N> const& arr, size_t index)
    {
        return arr.at(index);
    }
};

}    // namespace Stencil
