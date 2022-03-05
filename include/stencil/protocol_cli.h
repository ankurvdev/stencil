#pragma once
#include "protocol.h"
#include "protocol_string.h"

#include <chrono>
#include <string_view>
namespace Stencil
{
struct ProtocolCLI
{};

template <typename, typename = void> struct is_specialized : std::false_type
{};

template <typename T> struct is_specialized<T, std::void_t<decltype(T{})>> : std::true_type
{};

template <typename T>
concept ConceptHasProtocolString = is_specialized<Stencil::SerDes<T, Stencil::ProtocolString>>::value;

static_assert(ConceptHasProtocolString<std::chrono::time_point<std::chrono::system_clock>>, "Chrono should be defined");
static_assert(ConceptHasProtocolString<uint64_t>, "uint64_t should be defined");
static_assert(!ConceptHasProtocolString<std::vector<std::string>>, "void");

template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        Visitor<T>::VisitAllIndicies(obj, [&](auto const& k, auto const& v) {
            using ValType = std::remove_cvref_t<decltype(v)>;
            if constexpr (ConceptHasProtocolString<ValType>)
            {
                std::stringstream ss;
                fmt::print(ss, "--");
                SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                fmt::print(ss, "=");
                SerDes<ValType, ProtocolString>::Write(ss, v);
                ctx.push_back(ss.str());
                return;
            }

            else if constexpr (ConceptIndexable<ValType>)
            {
                std::stringstream ss;
                SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                ctx.push_back(ss.str());
                SerDes<ValType, ProtocolCLI>::Write(ctx, v);
                return;
            }

            else if constexpr (ConceptIterable<ValType>)
            {
                int               type = -1;
                std::stringstream ss;

                Visitor<ValType>::VisitAllIndicies(v, [&](auto, auto& v1) {
                    if constexpr (ConceptHasProtocolString<std::remove_cvref_t<decltype(v1)>>)
                    {
                        if (type != -1 && type != 0) throw std::logic_error("Iterable of mixed types unsupported");
                        if (type == -1)
                        {
                            fmt::print(ss, "--");
                            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                            fmt::print(ss, "=");
                        }
                        else
                        {
                            fmt::print(ss, ",");
                        }
                        type = 0;
                        SerDes<std::remove_cvref_t<decltype(v1)>, ProtocolString>::Write(ss, v1);
                    }
                    else if constexpr (ConceptIndexable<std::remove_cvref_t<decltype(v1)>>)
                    {
                        // list of indexables
                        if (type != -1 && type != 1) throw std::logic_error("Iterable of mixed types unsupported");
                        // Iterable of indexable. Use
                        SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                        ctx.push_back(ss.str());
                        SerDes<std::remove_cvref_t<decltype(v1)>, ProtocolCLI>::Write(ctx, v1);
                    }
                    else if constexpr (ConceptIterable<std::remove_cvref_t<decltype(v1)>>)
                    {
                        // Iterables of iterables unsupported
                        TODO("Iterable of Iterable");
                    }
                    else
                    {
                        TODO("Unknown");
                    }
                });
                if (type == 0) ctx.push_back(ss.str());
                return;
            }
            else
            {
                // std::string str = typeid(ValType).name();
                TODO("Unknown");
            }
        });
    }
};

template <Stencil::ConceptIterable T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        bool              first = true;
        std::stringstream ss;
        Visitor<T>::VisitAllIndicies(obj, [&](auto, auto& v) {
            if (!first) fmt::print(ctx, ',');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolString>::Write(ss, v);
            first = false;
        });
        ctx.push_back(ss.str());
    }
};

}    // namespace Stencil

namespace Stencil::CLI
{
template <typename T> struct Args;

template <typename TStr> struct Args<std::span<TStr>>
{
    Args(std::span<TStr> const& args) : _args(args) {}

    std::span<TStr> _args{};
};

template <typename T, typename TInCtx> inline T Parse(TInCtx const& args)
{
    Args<std::span<const std::string>> a(args);
    T                                  obj{};
    // Stencil::StackVisitor<ProtocolJsonVal, T> stack;
    // Tokenizer<T>(stack).Parse(obj, ictx);
    return obj;
}

template <typename T> inline std::vector<std::string> Stringify(T const& obj)
{
    std::vector<std::string> args;
    SerDes<T, ProtocolCLI>::Write(args, obj);
    return args;
}

}    // namespace Stencil::CLI