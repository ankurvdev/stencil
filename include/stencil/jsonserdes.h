#pragma once
#include "visitor.h"

#include <deque>

#ifdef USE_NLOHMANN_JSON
#pragma warning(push, 3)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic   push
#pragma GCC diagnostic   ignored "-Wimplicit-fallthrough"    // Wall doesnt work
#include <nlohmann/json.hpp>
#pragma GCC diagnostic   pop
#pragma clang diagnostic pop
#pragma warning(pop)
#endif
namespace Stencil
{
class JsonDataModel;
struct Json
{
#ifdef USE_NLOHMANN_JSON

#define SAFEEXEC(stmt)    \
    do {                  \
        try               \
        {                 \
            stmt;         \
            return true;  \
        } catch (...)     \
        {                 \
            return false; \
        }                 \
    } while (false)

    using number_float_t    = double;
    using number_integer_t  = int;
    using number_unsigned_t = unsigned;
    using string_t          = std::string;
    using binary_t          = std::string;

    template <typename TVisitor> struct Reader : public nlohmann::json_sax<nlohmann::json>
    {
        using sax = typename nlohmann::json_sax<nlohmann::json>;

        Reader(TVisitor& obj) : _visitor(obj) { _stack.push_back(std::numeric_limits<size_t>::max()); }
        CLASS_DELETE_COPY_AND_MOVE(Reader);

        bool IsArray() { return _stack.back() != std::numeric_limits<size_t>::max(); }

        template <typename TArg> bool _HandleValue(TArg const& arg)
        try
        {
            if (IsArray()) { _visitor.Select(Value(_stack.back()++)); }
            _visitor.SetValue(Value(arg));
            _visitor.GoBackUp();
            return true;
        } catch (std::exception const& /*ex*/)
        {
            return false;
        }

        bool null() override { return _HandleValue(Value()); }
        bool boolean(bool val) override { return _HandleValue(val); }
        bool number_integer(sax::number_integer_t val) override { return _HandleValue(val); }
        bool number_unsigned(sax::number_unsigned_t val) override { return _HandleValue(val); }
        bool number_float(sax::number_float_t val, const string_t& /*s*/) override { return _HandleValue(val); }
        bool string(sax::string_t& val) override { return _HandleValue(val); }
        bool binary(sax::binary_t& /*val*/) override { throw std::logic_error("TODO"); }

        bool start_object(std::size_t /*elements*/) override
        {
            if (IsArray()) { _visitor.Select(Value(_stack.back()++)); }
            _stack.push_back(std::numeric_limits<size_t>::max());
            return true;
        }
        bool end_object() override
        {
            _stack.pop_back();
            _visitor.GoBackUp();
            return true;
        }
        bool start_array(std::size_t /*elements*/) override
        {
            if (IsArray()) { _visitor.Select(Value(_stack.back()++)); }
            _stack.push_back(0);
            return true;
        }
        bool end_array() override
        {
            _stack.pop_back();
            _visitor.GoBackUp();
            return true;
        }
        // called when an object key is parsed; value is passed and can be safely moved away
        bool key(string_t& val) override { SAFEEXEC(_visitor.Select(Value(shared_string::make(val)))); }

        // called when a parse error occurs; byte position, the last token, and an exception is passed
        bool parse_error(std::size_t /*position*/, const std::string& /*last_token*/, const nlohmann::json::exception& ex) override
        {
            throw ex;
        }
#if 0
        bool null() override { SAFEEXEC(_tracker.HandleValue(Value(nullptr), nullptr)); }
        bool boolean(bool val) override { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool number_integer(sax::number_integer_t val) override { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool number_unsigned(sax::number_unsigned_t val) override { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool number_float(sax::number_float_t val, const string_t& /*s*/) override { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }

        bool string(sax::string_t& val) override { SAFEEXEC(_tracker.HandleValue(Value(shared_string::make(val)), nullptr)); }
        bool binary(sax::binary_t& /*val*/) override { throw std::logic_error("TODO"); }

        bool start_object(std::size_t /*elements*/) override { SAFEEXEC(_tracker.ObjStart(nullptr)); }
        bool end_object() override { SAFEEXEC(_tracker.ObjEnd()); }
        bool start_array(std::size_t /*elements*/) override { SAFEEXEC(_tracker.ListStart(nullptr)); }
        bool end_array() override { SAFEEXEC(_tracker.ListEnd()); }
        // called when an object key is parsed; value is passed and can be safely moved away
        bool key(string_t& val) override { SAFEEXEC(_tracker.ObjKey(Value(shared_string::make(val)), nullptr)); }

        // called when a parse error occurs; byte position, the last token, and an exception is passed
        bool parse_error(std::size_t /*position*/, const std::string& /*last_token*/, const nlohmann::json::exception& ex) override
        {
            throw ex;
        }
#endif
        TVisitor&           _visitor;
        std::vector<size_t> _stack;
        // ReflectionServices::StateTraker<TStruct, void*> _tracker;
    };
#endif

    template <typename TContext, typename T> void _WriteTo(TContext, T const& obj) { throw std::logic_error("Not Implemented"); }
    template <typename T> struct _Stringifier;

    template <Stencil::ConceptIndexable T> struct _Stringifier<T>
    {
        template <typename Context, Stencil::ConceptRVisitor<T> TRVisitor> auto Write(Context& ctx, TRVisitor const& rvisitor)
        {
            _WriteTo(ctx, '{');
            bool first = true;
            rvisitor.VisitAllIndicies([&](auto& k, auto& v) {
                _StringifyForVisitor(ctx, k);
                _WriteTo(ctx, ':');
                _StringifyForVisitor(ctx, v);
                first = false;
            });
            WriteTo(ctx, '}');
        }
    };

    template <Stencil::ConceptIterableNotIndexable T> struct _Stringifier<T>
    {
        template <typename Context, Stencil::ConceptRVisitor<T> TRVisitor> auto Write(Context& ctx, TRVisitor const& rvisitor)
        {
            _WriteTo(ctx, '[');
            bool first = true;

            rvisitor.VisitAll([&](auto& v) {
                if (!first) WriteTo(ctx, ',');
                _StringifyForVisitor(ctx, v);
                first = false;
            });

            _WriteTo(ctx, ']');
        }
    };

    template <Stencil::ConceptAtomicOnly T> struct _Stringifier<T>
    {
        template <typename Context, Stencil::ConceptRVisitor<T> TRVisitor> auto Write(Context& /*ctx*/, TRVisitor const& /*rvisitor*/)
        {
            throw std::logic_error("Not Implemented");
        }
    };

    template <typename T, typename TInCtx> static T Parse(TInCtx const& /*ictx*/) { throw std::logic_error("Not Implemented"); }

    template <typename T> static std::string Stringify(T const& /*obj*/) { throw std::logic_error("Not Implemented"); }

    template <typename T, typename TOutCtx> static void StringifyTo(T const& /*obj*/, TOutCtx& /*octx*/)
    {
        throw std::logic_error("Not Implemented");
    }
};

}    // namespace Stencil
