#pragma once
#include "protocol.h"
#include "tuple_utils.h"
#include "typetraits_builtins.h"
#include "visitor.h"

#include <span>
#include <sstream>
#include <string>

namespace Stencil
{

using ByteIt = std::span<const uint8_t>::iterator;

template <typename TVal, typename = typename std::enable_if<std::is_trivial<TVal>::value>::type>
static std::span<const uint8_t> AsCSpan(TVal const& val)
{
    return std::span(reinterpret_cast<uint8_t const*>(&val), sizeof(TVal));
}

template <typename TVal, typename = typename std::enable_if<std::is_trivial<TVal>::value>::type> static std::span<uint8_t> AsSpan(TVal& val)
{
    return std::span(reinterpret_cast<uint8_t*>(&val), sizeof(TVal));
}

struct Writer
{
    Writer() = default;

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> Writer& operator<<(TVal const& val)
    {
        auto spn = AsCSpan(val);
        std::copy(spn.begin(), spn.end(), back_inserter(_buffer));
        return *this;
    }

    Writer& operator<<(std::span<const std::byte> const& bytespn)
    {
        std::span<uint8_t const> spn(reinterpret_cast<uint8_t const*>(bytespn.data()), bytespn.size());
        std::copy(spn.begin(), spn.end(), back_inserter(_buffer));
        return *this;
    }

    Writer& operator<<(std::span<const uint8_t> const& spn)
    {
        std::copy(spn.begin(), spn.end(), back_inserter(_buffer));
        return *this;
    }
    template <typename TChar, typename TStr> Writer& _WriteStr(TStr const& str)
    {
        uint32_t bytesize = static_cast<uint32_t>(str.size() * sizeof(TChar));
        *this << bytesize;
        std::span<uint8_t const> spn(reinterpret_cast<uint8_t const*>(str.data()), bytesize);
        *this << spn;
        return *this;
    }

    Writer& operator<<(std::string const& str) { return _WriteStr<char>(str); }
    Writer& operator<<(std::wstring const& str) { return _WriteStr<wchar_t>(str); }
    Writer& operator<<(shared_string const& str) { return _WriteStr<char>(str); }
    Writer& operator<<(shared_wstring const& str) { return _WriteStr<wchar_t>(str); }

    std::vector<uint8_t> Reset() { return std::move(_buffer); }

    std::vector<uint8_t> _buffer;
};

struct Reader
{
    Reader(std::span<const uint8_t> const& w) : _it(w.begin()) {}
    Reader(ByteIt const& itbeg) : _it(itbeg) {}

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> TVal read()
    {
        TVal val;
        auto endIt = _it + sizeof(TVal);
        std::copy(_it, endIt, AsSpan(val).begin());
        _it = endIt;
        return val;
    }

    template <typename TChar, typename TStr> TStr _ReadStr()
    {
        size_t bytesize = read<uint32_t>();
        TStr   str;
        str.resize(bytesize / sizeof(TChar));
        std::span<uint8_t> spn(reinterpret_cast<uint8_t*>(str.data()), bytesize);
        auto               endIt = _it + static_cast<ByteIt::difference_type>(bytesize);
        std::copy(_it, endIt, spn.begin());
        _it = endIt;
        return str;
    }

    shared_string  read_shared_string() { return _ReadStr<char, shared_string>(); }
    shared_wstring read_shared_wstring() { return _ReadStr<wchar_t, shared_wstring>(); }
    std::string    read_string() { return _ReadStr<char, std::string>(); }
    std::wstring   read_wstring() { return _ReadStr<wchar_t, std::wstring>(); }

    auto   GetIterator() { return _it; }
    ByteIt _it;
};

struct ProtocolBinary
{
    using InType  = Reader;
    using OutType = Writer;
};

template <ConceptPreferIndexable T> struct SerDes<T, ProtocolBinary>
{
    using TKey = typename Stencil::TypeTraitsForIndexable<T>::Key;
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        Visitor<T>::VisitAll(obj, [&](auto const& key, auto const& val) {
            ctx << uint8_t{1};
            SerDes<std::remove_cvref_t<decltype(key)>, ProtocolBinary>::Write(ctx, key);
            SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Write(ctx, val);
        });
        ctx << uint8_t{0};
    }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        while (true)
        {
            auto marker = ctx.template read<uint8_t>();
            if (marker == 0) return;
            if (marker != 1) throw std::logic_error("Invalid marker");
            TKey key;
            SerDes<TKey, ProtocolBinary>::Read(key, ctx);
            Visitor<T>::VisitKey(obj, key, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Read(val, ctx); });
        }
    }
};
SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4702) /*Unreachable code*/    // Seems to only work in global scope

template <ConceptPreferIterable T> struct SerDes<T, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        // Some iterables can be primitives
        Visitor<T>::VisitAll(obj, [&](auto& /*key*/, auto& val) {
            ctx << uint8_t{1};
            SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Write(ctx, val);
        });
        ctx << uint8_t{0};
    }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        {
            auto marker = ctx.template read<uint8_t>();
            if (marker == 0) return;
            if (marker != 1) throw std::logic_error("Invalid marker");
        }
        typename Visitor<T>::Iterator it;
        Visitor<T>::IteratorBegin(it, obj);
        while (true)
        {

            if (!Visitor<T>::IteratorValid(it, obj)) { throw std::runtime_error("Cannot Visit Next Item on the iterable"); }
            Visitor<T>::Visit(it, obj, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Read(val, ctx); });
            auto marker = ctx.template read<uint8_t>();
            if (marker == 0) return;
            if (marker != 1) throw std::logic_error("Invalid marker");

            Visitor<T>::IteratorMoveNext(it, obj);
        }
    }
};
SUPPRESS_WARNINGS_END

template <ConceptPrimitives64Bit T> struct SerDes<T, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { ctx << Primitives64Bit::Traits<T>::Repr(obj); }
    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        obj = Primitives64Bit::Traits<T>::Convert(ctx.template read<decltype(Primitives64Bit::Traits<T>::Repr(obj))>());
    }
};

template <ConceptEnum T> struct SerDes<T, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { ctx << static_cast<uint32_t>(obj); }

    template <typename Context> static auto Read(T& obj, Context& ctx) { obj = static_cast<T>(ctx.template read<uint32_t>()); }
};

template <ConceptEnumPack T> struct SerDes<T, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj) { ctx << T::CastToInt(obj); }
    template <typename Context> static auto Read(T& obj, Context& ctx) { obj = T::CastFromInt(ctx.template read<uint32_t>()); }
};

template <> struct SerDes<shared_string, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, shared_string const& obj) { ctx << obj; }
    template <typename Context> static auto Read(shared_string& obj, Context& ctx) { obj = ctx.read_shared_string(); }
};

template <> struct SerDes<shared_wstring, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, shared_wstring const& obj) { ctx << obj; }
    template <typename Context> static auto Read(shared_wstring& obj, Context& ctx) { obj = ctx.read_shared_wstring(); }
};

template <> struct SerDes<std::wstring, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, std::wstring const& obj) { ctx << obj; }
    template <typename Context> static auto Read(std::wstring& obj, Context& ctx) { obj = ctx.read_wstring(); }
};

template <> struct SerDes<std::string, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, std::string const& obj) { ctx << obj; }
    template <typename Context> static auto Read(std::string& obj, Context& ctx) { obj = ctx.read_string(); }
};

template <size_t N> struct SerDes<std::array<char, N>, ProtocolBinary>
{
    using TObj = std::array<char, N>;
    template <typename Context> static auto Write(Context& ctx, TObj const& obj) { ctx << obj; }
    template <typename Context> static auto Read(TObj& obj, Context& ctx) { obj = ctx.template read<TObj>(); }
};

template <> struct SerDes<uuids::uuid, ProtocolBinary>
{
    template <typename Context> static auto Write(Context& ctx, uuids::uuid const& obj) { ctx << obj.as_bytes(); }

    template <typename Context> static auto Read(uuids::uuid& obj, Context& ctx)
    {
        obj = uuids::uuid{ctx.template read<std::array<uint8_t, 16>>()};
    }
};
}    // namespace Stencil
