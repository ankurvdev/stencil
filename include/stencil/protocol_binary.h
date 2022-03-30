#pragma once
#include <protocol.h>
#include <typetraits.h>

#include <span>
#include <sstream>
#include <string>
#include <tuple>

namespace Stencil
{

template <typename TTup, typename TLambda, size_t TIndex = 0> constexpr void VisitAt(TTup& tup, const size_t idx, TLambda&& lambda)
{
    if constexpr (TIndex == std::tuple_size<TTup>::value) { throw std::out_of_range("Exceeded maximum tuple range"); }
    else
    {
        if (TIndex == idx)
        {
            lambda(std::get<TIndex>(tup));
            return;
        }
        return VisitAt<TTup, TLambda, TIndex + 1>(tup, idx, std::forward<TLambda>(lambda));
    }
}

template <typename TI, typename T, typename... Ts> constexpr size_t IndexOf(size_t index = 0)
{
    if constexpr (std::is_same<TI, T>::value) { return index; }
    else
    {
        if constexpr (sizeof...(Ts) == 0) { static_assert(sizeof...(Ts) != 0, "Cannot Match anything in tuple"); }
        else
        {
            return IndexOf<TI, Ts...>(index + 1);
        }
    }
}

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
struct OStrmWriter
{
    OStrmWriter(std::ostream& ostr) : _ostr(ostr) {}
    CLASS_DELETE_COPY_AND_MOVE(OStrmWriter);

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> auto& operator<<(TVal const& val)
    {
        auto spn = AsCSpan(val);
        _ostr.write(reinterpret_cast<char const*>(spn.data()), static_cast<std::streamsize>(spn.size()));
        return *this;
    }

    auto& strm() { return _ostr; }

    auto& operator<<(shared_string const& val)
    {
        *this << val.size();
        if (val.size() > 0) _ostr.write(reinterpret_cast<char const*>(val.data()), static_cast<std::streamsize>(val.size()));
        return *this;
    }

    std::ostream& _ostr;
};

struct IStrmReader
{
    IStrmReader(std::istream& istrm) : _istrm(istrm) {}
    CLASS_DELETE_COPY_AND_MOVE(IStrmReader);

    bool  isEof() { return !_istrm.good(); }
    auto& strm() { return _istrm; }

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> TVal read()
    {
        TVal val;
        auto spn = AsSpan(val);
        _istrm.read(reinterpret_cast<char*>(spn.data()), static_cast<std::streamsize>(spn.size()));
        return val;
    }

    shared_string read_shared_string()
    {
        size_t      size = read<size_t>();
        std::string str(size, 0);
        _istrm.read(reinterpret_cast<char*>(str.data()), static_cast<std::streamsize>(size));
        return shared_string::make(std::move(str));
    }

    std::istream& _istrm;
};

struct Writer
{
    Writer() = default;

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> Writer& operator<<(TVal const& val)
    {
        auto spn = AsCSpan(val);
        std::copy(spn.begin(), spn.end(), back_inserter(_buffer));
        return *this;
    }

    Writer& operator<<(shared_string const& val)
    {
        *this << val.size();
        auto cursize = _buffer.size();
        _buffer.resize(_buffer.size() + val.size());
        auto bufferPos = _buffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(cursize);
        std::transform(val.begin(), val.end(), bufferPos, [](auto c) { return static_cast<uint8_t>(c); });
        return *this;
    }

    std::vector<uint8_t> Reset() { return std::move(_buffer); }

    std::vector<uint8_t> _buffer;
};

struct Reader
{
    Reader(ByteIt const& itbeg) : _it(itbeg) {}

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> TVal read()
    {
        TVal val;
        auto endIt = _it + sizeof(TVal);
        std::copy(_it, endIt, AsSpan(val).begin());
        _it = endIt;
        return val;
    }

    shared_string read_shared_string()
    {
        size_t        size  = read<size_t>();
        shared_string str   = shared_string::make(std::string(size, 0));
        auto          endIt = _it + static_cast<ByteIt::difference_type>(size);
        std::transform(_it, endIt, str.begin(), [](auto c) { return static_cast<char>(c); });
        _it = endIt;
        return str;
    }

    auto   GetIterator() { return _it; }
    ByteIt _it;
};

struct ProtocolBinary
{
    using InType  = std::span<const uint8_t>;
    using OutType = std::vector<uint8_t>;
};

}    // namespace Stencil
