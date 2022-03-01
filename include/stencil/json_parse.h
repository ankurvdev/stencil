#pragma once
#include "protocol.h"
#include "stackvisitor.h"
//#if defined USE_SIMDJSON

#pragma warning(push, 3)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <fmt/format.h>
#include <rapidjson/reader.h>
#pragma clang diagnostic pop
#pragma warning(pop)
#define RAPIDJSON_CHECK(...)        \
    try                             \
    {                               \
        __VA_ARGS__;                \
        return true;                \
    } catch (std::exception const&) \
    {                               \
        return false;               \
    }
//#include "json_parse_simdjson.h"
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
        rapidjson::StringStream ss(ctx.data());
        auto                    rslt = reader.Parse(ss, *this);
        if (rslt.IsError()) { throw std::logic_error(fmt::format("Json parse error : Code:{} Offset:{}", rslt.Code(), rslt.Offset())); }
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

//#endif

// Parse
namespace Stencil::Json
{
template <typename T> using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;

template <typename T, typename TInCtx> static T Parse(TInCtx const& ictx)
{
    T                                         obj{};
    Stencil::StackVisitor<ProtocolJsonVal, T> stack;
    Tokenizer<T>(stack).Parse(obj, ictx);
    return obj;
}

}    // namespace Stencil::Json
