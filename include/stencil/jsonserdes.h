#pragma once
#include "visitor.h"

#include <deque>
#include <sstream>

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

    template <typename T, typename = void> struct Writer;

    template <typename TStruct> static void StringifyStruct(TStruct& obj, std::stringstream& ss)
    {
        Stencil::Visitor<TStruct> visitor(obj);
        ss << "{";
        bool first = true;
        visitor.VisitAll([&](auto& key, auto& value) {
            if (!first)
                ss << ",";
            else
                first = false;

            ss << Writer<decltype(key)>::Stringify(key) << ":";
            ss << Writer<std::remove_cvref_t<decltype(value)>>::Stringify(value);
        });

        ss << "}";
    }

    template <typename TArr> static void StringifyArray(TArr& obj, std::stringstream& ss)
    {
        ss << "[";
        bool first = true;
        for (auto it = std::begin(obj); it != std::end(obj); ++it)
        {
            if (!first)
                ss << ",";
            else
                first = false;
            ss << Writer<std::remove_cvref_t<decltype(*it)>>::Stringify(*it);
        }
        ss << "]";
    }

    template <typename T> struct Writer<T, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
    {
        static std::string Stringify(const T& obj)
        {
            std::stringstream ss;
            StringifyStruct<T const>(obj, ss);
            return ss.str();
        }
    };

    template <typename T> struct Writer<std::unique_ptr<T>, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
    {
        static std::string Stringify(const std::unique_ptr<T>& obj)
        {
            std::stringstream ss;
            StringifyStruct<T>(*obj.get(), ss);
            return ss.str();
        }
    };

    template <typename T> struct Writer<std::shared_ptr<T>, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
    {
        static std::string Stringify(const std::shared_ptr<T>& obj)
        {
            std::stringstream ss;
            StringifyStruct<T>(*obj.get(), ss);
            return ss.str();
        }
    };

    template <typename T> struct Writer<std::vector<T>>
    {
        static std::string Stringify(const std::vector<T>& obj)
        {
            std::stringstream ss;
            StringifyArray(obj, ss);
            return ss.str();
        }
    };

    template <typename T, size_t N> struct Writer<std::array<T, N>>
    {
        static std::string Stringify(const std::array<T, N>& obj)
        {
            std::stringstream ss;
            StringifyArray(obj, ss);
            return ss.str();
        }
    };

    template <size_t N> struct Writer<std::array<char, N>>
    {
        static std::string Stringify(const std::array<char, N>& obj)
        {
            auto str  = std::string(obj.data(), obj.size());
            auto term = str.find(char{0}, 0);
            if (term != std::string::npos) { str.resize(term); }
            return "\"" + str + "\"";
        }
    };

    template <size_t N> struct Writer<char const (&)[N]>
    {
        static std::string Stringify(char const (&obj)[N]) { return "\"" + std::string(obj) + "\""; }
    };

    template <typename T> struct Writer<T, std::enable_if_t<Value::Supported<T>::value>>
    {
        static std::string Stringify(const T& obj)
        {
            auto const& sval = Value(obj).convert<shared_string>();
            return sval.empty() ? "null" : sval.str();
        }
    };

    template <typename T> struct Writer<T, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const T& obj) { return obj.Id(); }
    };

    template <typename T> struct Writer<std::unique_ptr<T>, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const std::unique_ptr<T>& obj) { return std::string(obj->GetObjectUuid().ToString()); }
    };

    template <typename T> struct Writer<UuidBasedId<T>>
    {
        static std::string Stringify(const UuidBasedId<T>& obj) { return std::string(obj.ToString()); }
    };

    template <typename TStruct> static void Load([[maybe_unused]] Visitor<TStruct>& visitor, [[maybe_unused]] std::istream& strm)
    {
#ifdef USE_NLOHMANN_JSON
        Reader<Visitor<TStruct>> handler(visitor);
        nlohmann::json::sax_parse(strm, &handler);
#else
        throw std::logic_error("json parser disabled at compile time");
#endif
    }

    template <typename TStruct> static void Load([[maybe_unused]] TStruct& obj, std::istream& strm)
    {
        Visitor<TStruct> visitor(obj);
        Load(visitor, strm);
    }

    template <typename TStruct> static std::unique_ptr<TStruct> Parse(const std::string_view& strv)
    {
        std::unique_ptr<TStruct> ptr(new TStruct());
        // TODO : Avoid stringing
        std::string        str(strv);
        std::istringstream istr(str);
        Load(*ptr.get(), istr);
        return ptr;
    }

    template <typename T> static std::string Stringify(const T& obj) { return Writer<T>::Stringify(obj); }
};

struct JsonSerDes
{
    template <typename TVisitor> static void Serialize(TVisitor& visitor, std::ostream& strm)
    {
        auto& ss = strm;
        ss << "{";
        bool first = true;
        visitor.VisitAll([&](auto& key, auto& value) {
            if (!first)
                ss << ",";
            else
                first = false;

            ss << Json::Writer<decltype(key)>::Stringify(key) << ":";
            ss << Json::Writer<std::remove_cvref_t<decltype(value)>>::Stringify(value);
        });

        ss << "}";
    }

    template <typename TVisitor> static void Deserialize(TVisitor& visitor, std::istream& strm)
    {
        switch (visitor.GetDataTypeHint())
        {
        case ReflectionBase::DataType::Value:
        {
            std::ostringstream sstrm;
            strm.peek();
            while (strm.good())
            {
                char c;
                strm >> c;
                sstrm << c;
                strm.peek();
            }
            visitor.SetValue(Value{sstrm.str()});
        }
        break;
        case ReflectionBase::DataType::List:
        case ReflectionBase::DataType::Object: Json::Load(visitor, strm); break;
        case ReflectionBase::DataType::Enum: TODO();
        case ReflectionBase::DataType::Union: TODO();
        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: throw std::runtime_error("Unsupported Data Type");
        }
    }

    template <typename TVisitor> static void Deserialize(TVisitor& visitor, std::string_view const& strv)
    {
        // TODO : Avoid stringing
        std::string        str(strv);
        std::istringstream istr(str);
        Deserialize(visitor, istr);
    }
};

}    // namespace Stencil
