#pragma once
#include "protocol_string.h"
#include "serdes.h"
#include "stackvisitor.h"
#include "visitor.h"

// #if defined USE_SIMDJSON
SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4061) /* Not all labels are EXPLICITLY handled in switch */
#include <rapidjson/memorystream.h>
#include <rapidjson/reader.h>
SUPPRESS_WARNINGS_END

#include <array>
#include <string>

namespace Stencil
{
struct ProtocolJsonVal
{
    using InType  = std::string_view;
    using OutType = std::stringstream;
};
}    // namespace Stencil

#define RAPIDJSON_CHECK(...)        \
    try                             \
    {                               \
        __VA_ARGS__;                \
        return true;                \
    } catch (std::exception const&) \
    {                               \
        return false;               \
    }

// #include "json_parse_simdjson.h"
namespace Stencil::impl::rapidjson_
{
template <typename T> struct Tokenizer : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, Tokenizer<T>>
{
    Tokenizer(Stencil::StackVisitor<Stencil::ProtocolJsonVal, T>& stackvisitor) : _stackvisitor(stackvisitor) {}
    CLASS_DELETE_COPY_AND_MOVE(Tokenizer);

    void Parse(T& obj, std::string_view const& ctx)
    {
        _modes.push_back(Mode::Indexable);
        _obj = &obj;
        _stackvisitor.Start(obj);
        rapidjson::Reader       reader;
        rapidjson::MemoryStream ss(ctx.data(), ctx.size());
        auto                    rslt = reader.Parse(ss, *this);
        if (rslt.IsError())
        {
            throw std::logic_error(fmt::format("Json parse error : Code:{} Offset:{}", static_cast<uint32_t>(rslt.Code()), rslt.Offset()));
        }
        if (_modes.size() != 1) { throw std::logic_error("Something is wrong"); }
    }

    enum class Mode
    {
        Primitive,
        Indexable,
        Iterable,
        IndexableValue
    };

    void _Start(Mode mode)
    {
        if (_modes.back() == Mode::Iterable) { _stackvisitor.Add(); }
        _modes.push_back(mode);
    }

    void _End()
    {
        _modes.pop_back();
        if (_modes.back() == Mode::Iterable || _modes.back() == Mode::IndexableValue) _stackvisitor.Pop();
        if (_modes.back() == Mode::IndexableValue) _modes.pop_back();
    }

    template <typename T1> void _Handle(T1 const& val)
    {
        _Start(Mode::Primitive);
        _stackvisitor.Assign(val);
        _End();
    }

    void _StartArray() { _Start(Mode::Iterable); }
    void _EndArray() { _End(); }

    void _StartObject() { _Start(Mode::Indexable); }
    void _EndObject() { _End(); }

    void _AddKey(std::string_view const& key)
    {
        _stackvisitor.AddKey();
        _stackvisitor.Assign(key);
        _stackvisitor.AddValue();
        _Start(Mode::IndexableValue);
    }

    // Rapidjson apis
    bool Null() { RAPIDJSON_CHECK(_Handle(std::string_view{})) }
    bool Bool(bool b) { RAPIDJSON_CHECK(_Handle(b)) }
    bool Int(int i) { RAPIDJSON_CHECK(_Handle(i)) }
    bool Uint(unsigned u) { RAPIDJSON_CHECK(_Handle(u)) }
    bool Int64(int64_t i) { RAPIDJSON_CHECK(_Handle(i)) }
    bool Uint64(uint64_t u) { RAPIDJSON_CHECK(_Handle(u)) }
    bool Double(double d) { RAPIDJSON_CHECK(_Handle(d)) }
    bool String(const char* str, rapidjson::SizeType length, bool /* copy */) { RAPIDJSON_CHECK(_Handle(std::string_view(str, length))) }
    bool StartObject() { RAPIDJSON_CHECK(_StartObject()) }
    bool Key(const char* str, rapidjson::SizeType length, bool /* copy */) { RAPIDJSON_CHECK(_AddKey(std::string_view(str, length))) }
    bool EndObject(rapidjson::SizeType /* memberCount */) { RAPIDJSON_CHECK(_EndObject()) }
    bool StartArray() { RAPIDJSON_CHECK(_StartArray()) }
    bool EndArray(rapidjson::SizeType /* elementCount */){RAPIDJSON_CHECK(_EndArray())}

    std::vector<Mode> _modes;
    StackVisitor<Stencil::ProtocolJsonVal, T>& _stackvisitor;
    T*                                         _obj{};
};    // namespace Stencil::impl::rapidjson_

}    // namespace Stencil::impl::rapidjson_

// Parse
namespace Stencil::Json
{
template <typename T> using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;

template <typename T, typename TInCtx> inline T Parse(TInCtx const& ictx)
{
    T                                         obj{};
    Stencil::StackVisitor<ProtocolJsonVal, T> stack;
    Tokenizer<T>(stack).Parse(obj, ictx);
    return obj;
}

}    // namespace Stencil::Json

namespace Stencil
{
template <typename T, typename TContext> auto _WriteQuotedString(TContext& ctx, T const& obj)
{
    fmt::print(ctx, "\"");
    SerDes<T, ProtocolString>::Write(ctx, obj);
    fmt::print(ctx, "\"");
}

// template <typename T, typename TContext> void _ReadQuotedString(T& obj, TContext const& ctx);
SUPPRESS_WARNINGS_START
SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
template <typename T> void _ReadQuotedString(T& obj, std::string_view const& ctx)
{
    if (ctx.size() == 0 || ctx.data()[0] != '\"') { return SerDes<T, ProtocolString>::Read(obj, ctx); }
    if (ctx.data()[ctx.size() - 1] != '\"') throw std::logic_error("String does not end with quotes");
    auto ctx1 = ctx.substr(1, ctx.size() - 2);
    SerDes<T, ProtocolString>::Read(obj, ctx1);
}
SUPPRESS_WARNINGS_END
template <size_t N> struct SerDes<std::array<char, N>, ProtocolJsonVal>
{
    using TObj = std::array<char, N>;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj[0] == 0) { fmt::print(ctx, "null"); }
        else { _WriteQuotedString(ctx, obj); }
    }

    template <typename TContext> static auto Read(TObj& obj, TContext& ctx)
    {
        obj = {};
        if (ctx == "null") return;
        _ReadQuotedString(obj, ctx);
    }
};

template <size_t N> struct SerDes<std::array<uint16_t, N>, ProtocolJsonVal>
{
    using TObj = std::array<uint16_t, N>;
    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if constexpr (N <= 4)
        {
            uint64_t val = 0;
            for (size_t i = N; i > 0; i--) { val = (val << 16) | obj.at(i - 1); }
            SerDes<uint64_t, ProtocolString>::Write(ctx, val);
        }
        else
        {
            fmt::print(ctx, "[");
            bool first = true;
            Visitor<std::array<uint16_t, N>>::VisitAll(obj, [&](auto, auto& v) {
                using ValueType = std::remove_cvref_t<decltype(v)>;
                if (!first) fmt::print(ctx, ",");
                SerDes<ValueType, ProtocolString>::Write(ctx, v);
                first = false;
            });
            fmt::print(ctx, "]");
        }
    }
    template <typename TContext> static auto Read(TObj& /*obj*/, TContext& /*ctx*/) { TODO(""); }
};

template <> struct SerDes<uuids::uuid, ProtocolJsonVal>
{
    using TObj = uuids::uuid;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj) { _WriteQuotedString(ctx, obj); }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <typename TClock> struct SerDes<std::chrono::time_point<TClock>, ProtocolJsonVal>
{
    using TObj = std::chrono::time_point<TClock>;
    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj) { _WriteQuotedString(ctx, obj); }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <> struct SerDes<char, ProtocolJsonVal>
{
    using TObj = char;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj == 0)
            fmt::print(ctx, "null");
        else
            fmt::print(ctx, "\"{}\"", obj);
    }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx)
    {
        if (ctx.empty()) { obj = {}; }
        else
        {
            if (ctx.size() > 1) { throw std::logic_error("Invalid value"); }
            obj = ctx[0];

            // SerDes<TObj, ProtocolString>::Read(obj, ctx);
        }
    }
};

template <ConceptEnumPack T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { _WriteQuotedString(ctx, obj); }
    template <typename TContext> static auto Read(T& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <ConceptEnum T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { _WriteQuotedString(ctx, obj); }
    template <typename TContext> static auto Read(T& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <ConceptPrimitives64Bit T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj) { SerDes<T, ProtocolString>::Write(ctx, obj); }
    template <typename TContext> static auto Read(T& obj, TContext& ctx) { SerDes<T, ProtocolString>::Read(obj, ctx); }
};

template <> struct SerDes<std::string, ProtocolJsonVal>
{
    using TObj = std::string;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj.empty()) { fmt::print(ctx, "null"); }
        else
            _WriteQuotedString(ctx, obj);
    }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <typename T> struct SerDes<std::basic_string_view<T>, ProtocolJsonVal>
{
    using TObj = std::basic_string_view<T>;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj.empty()) { fmt::print(ctx, "null"); }
        else
            _WriteQuotedString(ctx, obj);
    }

    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) = delete;
};

template <> struct SerDes<std::wstring, ProtocolJsonVal>
{
    using TObj = std::wstring;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj.empty()) { fmt::print(ctx, "null"); }
        else
            _WriteQuotedString(ctx, obj);
    }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <> struct SerDes<std::wstring_view, ProtocolJsonVal>
{
    using TObj = std::wstring_view;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj.empty()) { fmt::print(ctx, "null"); }
        else
            _WriteQuotedString(ctx, obj);
    }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};

template <typename T> struct SerDes<shared_stringT<T>, ProtocolJsonVal>
{
    using TObj = shared_stringT<T>;

    template <typename TContext> static auto Write(TContext& ctx, TObj const& obj)
    {
        if (obj.empty()) { fmt::print(ctx, "null"); }
        else
            _WriteQuotedString(ctx, obj);
    }
    template <typename TContext> static auto Read(TObj& obj, TContext& ctx) { _ReadQuotedString(obj, ctx); }
};
}    // namespace Stencil

// Stringify
namespace Stencil
{

template <typename TContext, typename T> void _WriteTo(TContext& ctx, T const& obj)
{
    std::ostream& ostr = ctx;
    fmt::print(ostr, "{}", obj);
}

template <typename T, ConceptProtocol TProt> struct SerDes;

static_assert(!Stencil::Category::IsIndexable<uint64_t>());

template <ConceptPreferVariant T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        VisitorForVariant<T>::VisitActiveAlternative(obj, [&](auto const& k, auto const& v) {
            _WriteTo(ctx, '\"');
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ctx, k);
            _WriteTo(ctx, '\"');
            _WriteTo(ctx, ':');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
        });
        _WriteTo(ctx, '}');
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {
        using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;
        Stencil::StackVisitor<ProtocolJsonVal, T> stack;
        Tokenizer(stack).Parse(obj, ctx);
    }
};

template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolJsonVal>
{

    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        _WriteTo(ctx, '{');
        bool first = true;
        Visitor<T>::VisitAll(obj, [&](auto const& k, auto const& v) {
            if (!first) _WriteTo(ctx, ',');
            _WriteTo(ctx, '\"');
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ctx, k);
            _WriteTo(ctx, '\"');
            _WriteTo(ctx, ':');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, '}');
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {
        using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;
        Stencil::StackVisitor<ProtocolJsonVal, T> stack;
        Tokenizer(stack).Parse(obj, ctx);
    }
};

// template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolString>
//{
//     template <typename TContext> static auto Write(TContext& /*ctx*/, T const& /*obj*/) { TODO(""); }
// };

template <Stencil::ConceptPreferIterable T> struct SerDes<T, ProtocolJsonVal>
{
    template <typename TContext> static auto Write(TContext& ctx, T const& obj)
    {
        _WriteTo(ctx, '[');
        bool first = true;
        Visitor<T>::VisitAll(obj, [&](auto, auto& v) {
            if (!first) _WriteTo(ctx, ',');
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolJsonVal>::Write(ctx, v);
            first = false;
        });
        _WriteTo(ctx, ']');
    }

    template <typename TContext> static auto Read(T& obj, TContext& ctx)
    {
        using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;
        Stencil::StackVisitor<ProtocolJsonVal, T> stack;
        Tokenizer(stack).Parse(obj, ctx);
    }
};

namespace Json
{
template <typename T> inline std::string Stringify(T const& obj)
{
    std::stringstream sstr;
    SerDes<T, ProtocolJsonVal>::Write(sstr, obj);
    return sstr.str();
}
}    // namespace Json
}    // namespace Stencil
