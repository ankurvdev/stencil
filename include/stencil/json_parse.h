#pragma once
#include "stackvisitor.h"

//#if defined USE_SIMDJSON

#pragma warning(push, 3)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <fmt/format.h>
#include <rapidjson/reader.h>
#pragma clang diagnostic pop
#pragma warning(pop)
#define RAPIDJSON_CHECK(...) \
    try                      \
    {                        \
        __VA_ARGS__;         \
        return true;         \
    } catch (std::exception) \
    {                        \
        return false;        \
    }
//#include "json_parse_simdjson.h"
namespace Stencil::impl::rapidjson_
{
#pragma warning(push, 3)

template <typename T> struct Tokenizer : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, Tokenizer<T>>
{
    Tokenizer(StackVisitor<T>& stackvisitor) : _stackvisitor(stackvisitor) {}
    CLASS_DELETE_COPY_AND_MOVE(Tokenizer);

    void Parse(T& obj, std::string_view const& ctx)
    {
        _obj = &obj;
        _stackvisitor.Start(obj);
        rapidjson::Reader       reader;
        rapidjson::StringStream ss(ctx.data());
        auto                    rslt = reader.Parse(ss, *this);
        if (rslt.IsError()) { throw std::logic_error(fmt::format("Json parse error : Code:{} Offset:{}", rslt.Code(), rslt.Offset())); }
    }

    template <typename T> void _Handle(T const& val)
    {
        if (_modes.back() == Mode::Array)
        {
            _modes.push_back(Mode::Array);
            _stackvisitor.Add();
        }
        _stackvisitor.Assign(val);
        _stackvisitor.Pop();
    }

    void _StartArray() { _modes.push_back(Mode::Array); }
    void _EndArray()
    {
        _stackvisitor.Pop();
        _modes.pop_back();
    }

    void _StartObject() { _modes.push_back(Mode::Object); }
    void _EndObject()
    {
        _stackvisitor.Pop();
        _modes.pop_back();
    }

    void _AddKey(std::string_view const& key)
    {
        _stackvisitor.AddKey();
        _stackvisitor.Assign(key);
        _stackvisitor.Pop();
    }

    // Rapidjson apis
    bool Null() { RAPIDJSON_CHECK(_Handle(0)); }
    bool Bool(bool b) { RAPIDJSON_CHECK(_Handle(b)); }
    bool Int(int i) { RAPIDJSON_CHECK(_Handle(i)); }
    bool Uint(unsigned u) { RAPIDJSON_CHECK(_Handle(u)); }
    bool Int64(int64_t i) { RAPIDJSON_CHECK(_Handle(i)); }
    bool Uint64(uint64_t u) { RAPIDJSON_CHECK(_Handle(u)); }
    bool Double(double d) { RAPIDJSON_CHECK(_Handle(d)); }
    bool String(const char* str, rapidjson::SizeType length, bool copy) { RAPIDJSON_CHECK(_Handle(std::string_view(str, length))); }
    bool StartObject() { RAPIDJSON_CHECK(_StartObject()); }
    bool Key(const char* str, rapidjson::SizeType length, bool copy) { RAPIDJSON_CHECK(_AddKey(std::string_view(str, length))); }
    bool EndObject(rapidjson::SizeType memberCount) { RAPIDJSON_CHECK(_EndObject()); }
    bool StartArray() { RAPIDJSON_CHECK(_StartArray()); }
    bool EndArray(rapidjson::SizeType elementCount) { RAPIDJSON_CHECK(_EndArray()); }

    enum class Mode
    {
        Primitive,
        Array,
        Object
    };

    std::vector<Mode> _modes;
    StackVisitor<T>&  _stackvisitor;
    T*                _obj{};
};    // namespace Stencil::impl::rapidjson_

#pragma warning(pop)

}    // namespace Stencil::impl::rapidjson_

//#endif

// Parse
namespace Stencil::Json
{
template <typename T> using Tokenizer = Stencil::impl::rapidjson_::Tokenizer<T>;

template <typename T, typename TInCtx> static T Parse(TInCtx const& ictx)
{
    T                        obj{};
    Stencil::StackVisitor<T> stack;
    Tokenizer<T>(stack).Parse(obj, ictx);
    return obj;
}

}    // namespace Stencil::Json
