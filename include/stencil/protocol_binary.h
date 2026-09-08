#pragma once
#include "enums.h"
#include "serdes.h"
#include "shared_string.h"
#include "visitor.h"

#include <span>
#include <string>
#include <type_traits>

namespace Stencil
{

using ByteIt = std::span<uint8_t const>::iterator;

template <typename TVal>
static std::span<uint8_t const> AsCSpan(TVal const& val)
requires std::is_trivially_default_constructible<TVal>::value {
    return {reinterpret_cast<uint8_t const*>(&val), sizeof(TVal)};
}

template <typename TVal>
static std::span<uint8_t> AsSpan(TVal& val)
requires std::is_trivially_default_constructible<TVal>::value {
    return {reinterpret_cast<uint8_t*>(&val), sizeof(TVal)};
}

struct Writer
{
    Writer() = default;

    template <typename TVal>
     requires std::is_trivially_default_constructible_v<TVal> 
    Writer& operator<<(TVal const& val) LFTBND
   {
        auto spn = AsCSpan(val);
        std::copy(spn.begin(), spn.end(), back_inserter(buffer));
        return *this;
    }

    Writer& operator<<(std::span<std::byte const> const& bytespn) LFTBND
    {
        std::span<uint8_t const> spn(reinterpret_cast<uint8_t const*>(bytespn.data()), bytespn.size());
        std::copy(spn.begin(), spn.end(), back_inserter(buffer));
        return *this;
    }

    Writer& operator<<(std::span<uint8_t const> const& spn) LFTBND
    {
        std::copy(spn.begin(), spn.end(), back_inserter(buffer));
        return *this;
    }
    template <typename TChar, typename TStr> Writer& _WriteStr(TStr const& str) LFTBND
    {
        auto bytesize = static_cast<uint32_t>(str.size() * sizeof(TChar));
        *this << bytesize;
        std::span<uint8_t const> spn(reinterpret_cast<uint8_t const*>(str.data()), bytesize);
        *this << spn;
        return *this;
    }

    Writer& operator<<(std::string const& str)  LFTBND { return _WriteStr<char>(str); }
    Writer& operator<<(std::wstring const& str) LFTBND { return _WriteStr<wchar_t>(str); }
    Writer& operator<<(shared_string const& str) LFTBND { return _WriteStr<char>(str); }
    Writer& operator<<(shared_wstring const& str) LFTBND { return _WriteStr<wchar_t>(str); }

    std::vector<uint8_t> Reset() { return std::move(buffer); }

    std::vector<uint8_t> buffer;
};

struct Reader
{
    explicit Reader(std::span<uint8_t const> const& w) : it(w.begin()) {}
    explicit Reader(ByteIt const& itbeg) : it(itbeg) {}

    template <typename TVal> TVal Read()
    requires std::is_trivially_default_constructible_v<TVal> {
        TVal val;
        auto endIt = it + sizeof(TVal);
        std::copy(it, endIt, AsSpan(val).begin());
        it = endIt;
        return val;
    }

    template <typename TChar, typename TStr> TStr _ReadStr()
    {
        size_t bytesize = Read<uint32_t>();
        TStr   str;
        str.resize(bytesize / sizeof(TChar));
        std::span<uint8_t> spn(reinterpret_cast<uint8_t*>(str.data()), bytesize);
        auto               endIt = it + static_cast<ByteIt::difference_type>(bytesize);
        std::copy(it, endIt, spn.begin());
        it = endIt;
        return str;
    }

    shared_string  ReadSharedString() { return _ReadStr<char, shared_string>(); }
    shared_wstring ReadSharedWstring() { return _ReadStr<wchar_t, shared_wstring>(); }
    std::string    ReadString() { return _ReadStr<char, std::string>(); }
    std::wstring   ReadWstring() { return _ReadStr<wchar_t, std::wstring>(); }

    auto   GetIterator() const { return it; }
    ByteIt it;
};

struct ProtocolBinary
{
    using InType  = Reader;
    using OutType = Writer;
};

template <ConceptPreferIndexable T> struct SerDes<T, ProtocolBinary>
{
    using TKey = Stencil::TypeTraitsForIndexable<T>::Key;
    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        Visitor<T>::VisitAll(obj, [&](auto const& key, auto const& val) {
            ctx << uint8_t{1};
            SerDes<std::remove_cvref_t<decltype(key)>, ProtocolBinary>::Write(ctx, key);
            SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Write(ctx, val);
        });
        ctx << uint8_t{0};
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {
        while (true)
        {
            auto marker = ctx.template read<uint8_t>();
            if (marker == 0) return;
            if (marker != 1) throw std::logic_error("Invalid marker");
            TKey key;
            SerDes<TKey, ProtocolBinary>::Read(key, ctx);
            Visitor<T>::VisitKey(obj, key, [&](auto& val) {    //
                SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Read(val, ctx);
            });
        }
    }
};

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4702) /*Unreachable code*/    // Seems to only work in global scope

template <ConceptPreferIterable T> struct SerDes<T, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        // Some iterables can be primitives
        Visitor<T>::VisitAll(obj, [&](auto& /*key*/, auto& val) {
            ctx << uint8_t{1};
            SerDes<std::remove_cvref_t<decltype(val)>, ProtocolBinary>::Write(ctx, val);
        });
        ctx << uint8_t{0};
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
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
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { ctx << Primitives64Bit::Traits<T>::Repr(obj); }
    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {
        obj = Primitives64Bit::Traits<T>::Convert(ctx.template read<decltype(Primitives64Bit::Traits<T>::Repr(obj))>());
    }
};

template <ConceptPreferVariant T> struct SerDes<T, ProtocolBinary>
{
    using TKey = uint8_t;
    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        if (VisitorForVariant<T>::IsMonostate(obj))
        {
            SerDes<TKey, ProtocolBinary>::Write(ctx, TKey{0});
            return;
        }
        VisitorForVariant<T>::VisitActiveAlternative(obj, [&](auto const& k, auto const& v) {
            SerDes<TKey, ProtocolBinary>::Write(ctx, static_cast<TKey>(k));
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolBinary>::Write(ctx, v);
        });
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {

        TKey key = 0;
        SerDes<TKey, ProtocolBinary>::Read(key, ctx);
        bool done = false;
        VisitorForVariant<T>::VisitAlternatives(obj, [&](auto const& k, auto& v) {
            if (done) { return; }
            // using TKey1 = std::remove_cvref_t<decltype(k)>;
            using TVal = std::remove_cvref_t<decltype(v)>;
            if (static_cast<TKey>(k) != key) { return; }
            SerDes<TVal, ProtocolBinary>::Read(v, ctx);
            obj  = v;
            done = true;
        });
    }
};

template <ConceptEnum T> struct SerDes<T, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { ctx << static_cast<uint32_t>(obj); }

    template <typename TContext> static auto Read(T& obj, TContext& ctx) { obj = static_cast<T>(ctx.template read<uint32_t>()); }
};

template <ConceptEnumPack T> struct SerDes<T, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { ctx << T::CastToInt(obj); }
    template <typename TContext> static auto Read(T& obj, TContext& ctx) { obj = T::CastFromInt(ctx.template read<uint32_t>()); }
};

template <> struct SerDes<shared_string, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, shared_string const& obj) { ctx << obj; }
    template <typename TContext> static auto Read(shared_string& obj, TContext& ctx) { obj = ctx.read_shared_string(); }
};

template <> struct SerDes<shared_wstring, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, shared_wstring const& obj) { ctx << obj; }
    template <typename TContext> static auto Read(shared_wstring& obj, TContext& ctx) { obj = ctx.read_shared_wstring(); }
};

template <> struct SerDes<std::wstring, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, std::wstring const& obj) { ctx << obj; }
    template <typename TContext> static auto Read(std::wstring& obj, TContext& ctx) { obj = ctx.read_wstring(); }
};

template <> struct SerDes<std::string, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, std::string const& obj) { ctx << obj; }
    template <typename TContext> static auto Read(std::string& obj, TContext& ctx) { obj = ctx.read_string(); }
};

template <size_t N> struct SerDes<std::array<char, N>, ProtocolBinary>
{
    using TObj = std::array<char, N>;
    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj) { ctx << obj; }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { obj = ctx.template read<TObj>(); }
};

template <> struct SerDes<uuids::uuid, ProtocolBinary>
{
    template <typename TContext> static auto Write(TContext& ctx, uuids::uuid const& obj) { ctx << obj.as_bytes(); }

    template <typename TContext> static auto Read(uuids::uuid& obj, TContext& ctx)
    {
        obj = uuids::uuid{ctx.template read<std::array<uint8_t, 16>>()};
    }
};
}    // namespace Stencil
