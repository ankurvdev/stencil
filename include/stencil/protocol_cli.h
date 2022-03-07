#pragma once
#include "protocol.h"
#include "protocol_string.h"

#include <chrono>
#include <span>
#include <string>
#include <string_view>
#include <vector>
namespace Stencil
{

/*
 * Key Points
 * Context
 *      A 'dash' moves up the context when inside iterable or indexable
 *      A 'double-dash' moves the context to root
 *      For Iterables Context auto slides to the Iterable subtype and keeps sliding until a non-iterable is encountered.
 *      For Indexable Context slides on named field or --field
 * Primitives
 *      --field=value Only used for primitives inside indexables
 *      After parsing context automatically goes up
 *      If in a list parent context look for comma and split
 * Iterables:
 *      Primitives:
            Can be clubbed into a single arg using comma separator 1,2,3,4
            Can be specified as individual args 1 2 3 4
            Use dash to move up the context
 *      Iterables
 *          Auto context slide down
 *      Indexable
 *          Switch to indexable context
 * Indexable
 *
 */
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
                ctx.push_back("-");
                return;
            }

            else if constexpr (ConceptIterable<ValType>)
            {
                int               type = -1;
                std::stringstream ss;

                Visitor<ValType>::VisitAllIndicies(v, [&](auto, auto& v1) {
                    if constexpr (ConceptHasProtocolString<std::remove_cvref_t<decltype(v1)>>)
                    {
                        // Iterable of Values. Use shorthand
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
                    else
                    {
                        if (type != -1 && type != 1) throw std::logic_error("Iterable of mixed types unsupported");
                        // For Iterable of Iterable and Iterable of Indexables.
                        // Just use the recursive Iterable
                        if (type == -1)
                        {
                            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                            ctx.push_back(ss.str());
                        }

                        SerDes<std::remove_cvref_t<decltype(v1)>, ProtocolCLI>::Write(ctx, v1);
                        ctx.push_back("-");
                        type = 1;
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

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        using Traits = typename Stencil::TypeTraitsForIndexable<T>;

        while (ctx.valid() && !ctx.root_ctx_requested())
        {
            auto token = ctx.move_next();
            if (token.size() == 0) continue;
            if (token[0] == '-')
            {
                if (token.size() == 1) return;
                if (token[1] != '-') { TODO("handle short args"); }
                if (token.size() == 2)
                {
                    ctx.mark_root_ctx_requested();
                    return;
                }
                auto index = token.find('=');
                if (index == std::string_view::npos) { TODO("Check if bool"); }
                auto                 keystr = token.substr(2, index - 2);
                auto                 value  = token.substr(index + 1);
                typename Traits::Key key;
                SerDes<Traits::Key, ProtocolString>::Read(key, keystr);

                // Visitor<T>::VisitKey(obj, key, [](auto& val) { TODO("list vs primitive handling"); });
            }
            else
            {
                // Should we suppport key=val ?
                typename Traits::Key key;
                SerDes<Traits::Key, ProtocolString>::Read(key, token);

                Visitor<T>::VisitKey(obj, key, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolCLI>::Read(val, ctx); });
            }
        }
    }
};

template <Stencil::ConceptIterable T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        Visitor<T>::VisitAllIndicies(obj, [&](auto, auto& v) {
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolCLI>::Write(ctx, v);
            ctx.push_back("-");
        });
    }
    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        typename Visitor<T>::Iterator it;
        Visitor<T>::IteratorBegin(it, obj);
        bool valid = false;

        while (ctx.valid() && !ctx.root_ctx_requested())
        {
            auto token = ctx.move_next();
            if (token.size() == 0) continue;
            if (token[0] == '-')
            {
                if (token.size() == 1) return;
                if (token[1] != '-') { TODO("handle short args"); }
                if (token.size() == 2)
                {
                    ctx.mark_root_ctx_requested();
                    return;
                }
            }
            if (!valid)
            {
                Visitor<T>::IteratorBegin(it, obj);
                valid = true;
            }
            else
            {
                Visitor<T>::IteratorMoveNext(it, obj);
            }
            if (!Visitor<T>::IteratorValid(it, obj)) throw std::runtime_error("Cannot Visit Next Item on the iterable");

            Visitor<T>::Visit(it, obj, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolCLI>::Read(val, ctx); });
        }
    }
};

template <Stencil::ConceptPrimitive T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        std::stringstream ss;
        SerDes<T, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }
    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        if (!ctx.valid() || ctx.root_ctx_requested()) return;
        auto token = ctx.move_next();
        if (token.size() == 0) return;
        if (token[0] == '-')
        {
            if (token.size() == 1) return;
            if (token[1] != '-') { TODO("handle short args"); }
            if (token.size() == 2)
            {
                ctx.mark_root_ctx_requested();
                return;
            }
        }
        SerDes<T, ProtocolString>::Read(obj, token);
    }
};

template <size_t N> struct SerDes<std::array<char, N>, ProtocolCLI>
{
    using TObj = std::array<char, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::stringstream ss;
        SerDes<TObj, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }

    template <typename Context> static auto Read(TObj& /*obj*/, Context& /*ctx*/) { TODO(""); }
};

template <size_t N>
requires(N <= 4) struct SerDes<std::array<uint16_t, N>, ProtocolCLI>
{
    using TObj = std::array<uint16_t, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::stringstream ss;
        SerDes<TObj, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }

    template <typename Context> static auto Read(TObj& /*obj*/, Context& /*ctx*/) { TODO(""); }
};
}    // namespace Stencil

namespace Stencil::CLI
{
struct ArgcArgv
{
    int                _argc;
    char const* const* _argv;

    auto   at(size_t index) const { return std::string_view(_argv[index]); }
    size_t count() const { return static_cast<size_t>(_argc); }
};

template <typename TStrArr> struct SpanStr
{
    TStrArr const* _strList;

    const auto& at(size_t index) const { return (*_strList)[index]; }
    size_t      count() const { return (*_strList).size(); }
};

template <typename T> struct ArgsIterator
{
    ArgsIterator(T&& ctx) : _ctx(ctx) {}

    std::string_view move_next()
    {
        std::string_view retval(_ctx.at(_current));
        _current++;
        return retval;
    }

    bool valid() const { return _current < _ctx.count(); }
    bool root_ctx_requested() const { return _root_ctx_requested; }
    void clear_root_ctx_requested() { _root_ctx_requested = false; }
    void mark_root_ctx_requested() { _root_ctx_requested = true; }

    bool   _root_ctx_requested{false};
    size_t _current{0};
    T      _ctx;
};

template <typename T, typename TInCtx> inline T _Parse(ArgsIterator<TInCtx>&& argsIt)
{
    T obj{};
    while (argsIt.valid())
    {
        argsIt.clear_root_ctx_requested();
        SerDes<T, ProtocolCLI>::Read(obj, argsIt);
    }
    return obj;
}

template <typename T> inline T Parse(int argc, char const* const* const argv)
{
    return _Parse<T>(ArgsIterator<ArgcArgv>(ArgcArgv{argc, argv}));
}

template <typename T, typename TStrArr> inline T Parse(TStrArr const& args)
{
    return _Parse<T>(ArgsIterator<SpanStr<TStrArr>>(SpanStr<TStrArr>{&args}));
}

template <typename T> inline std::vector<std::string> Stringify(T const& obj)
{
    std::vector<std::string> args;
    SerDes<T, ProtocolCLI>::Write(args, obj);
    return args;
}

}    // namespace Stencil::CLI