#pragma once
#include "base.h"
#include "visitor.h"

#include <deque>
#include <format>
class JsonDataModel;

#ifdef USE_NLOHMANN_JSON
#pragma warning(push, 3)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"    // Wall doesnt work
#include <nlohmann/json.hpp>
#pragma GCC diagnostic pop
#pragma clang diagnostic pop
#pragma warning(pop)

#define SAFEEXEC(stmt)    \
    do                    \
    {                     \
        try               \
        {                 \
            stmt;         \
            return true;  \
        }                 \
        catch (...)       \
        {                 \
            return false; \
        }                 \
    } while (false)
struct Json
{
    using number_float_t    = double;
    using number_integer_t  = int;
    using number_unsigned_t = unsigned;
    using string_t          = std::string;
    using binary_t          = std::string;

    template <typename TStruct> struct Reader : public nlohmann::json_sax<nlohmann::json>
    {
        using sax = typename nlohmann::json_sax<nlohmann::json>;

        Reader(TStruct& obj) : _visitor(obj) { _stack.push_back(std::numeric_limits<size_t>::max()); }

        bool IsArray() { return _stack.back() != std::numeric_limits<size_t>::max(); }

        template <typename TArg> bool _HandleValue(TArg const& arg)
        try
        {
            if (IsArray())
            {
                _visitor.Select(Value(_stack.back()++));
            }
            _visitor.SetValue(Value(arg));
            _visitor.GoBackUp();
            return true;
        }
        catch (std::exception const& /*ex*/)
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
            if (IsArray())
            {
                _visitor.Select(Value(_stack.back()++));
            }
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
            if (IsArray())
            {
                _visitor.Select(Value(_stack.back()++));
            }
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
        Stencil::Visitor<TStruct> _visitor;
        std::vector<size_t>       _stack;
        // ReflectionServices::StateTraker<TStruct, void*> _tracker;
    };

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

            ss << "\"" << Writer<decltype(key)>::Stringify(key) << "\":";
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

    template <typename TClk> struct Writer<std::chrono::time_point<TClk>>
    {
        static std::string Stringify(const std::chrono::time_point<TClk>& obj)
        {
            // return std::format("\"{:%FT%TZ}\"", std::chrono::time_point_cast<std::chrono::milliseconds>(obj));
            return std::format("\"{:%FT%TZ}\"", obj);
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

    template <size_t N> struct Writer<char const (&)[N]>
    {
        static std::string Stringify(char const (&obj)[N]) { return std::string(obj); }
    };

    template <typename T> struct Writer<T, std::enable_if_t<Value::Supported<T>::value>>
    {
        static std::string Stringify(const T& obj) { return Value(obj).convert<shared_string>().str(); }
    };

    template <typename T> struct Writer<T, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const T& obj) { return obj.Id(); }
    };

    template <typename T> struct Writer<std::unique_ptr<T>, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const std::unique_ptr<T>& obj) { return std::string(obj->GetObjectUuid().ToString()); }
    };

    template <typename TStruct> static void Load(TStruct& obj, const std::string_view& str)
    {
        Reader<TStruct> handler(obj);
        nlohmann::json::sax_parse(str, &handler);
    }

    template <typename TStruct> static std::unique_ptr<TStruct> Parse(const std::string_view& str)
    {
        std::unique_ptr<TStruct> ptr(new TStruct());
        Load(*ptr.get(), str);
        return ptr;
    }

    template <typename T> static std::string Stringify(const T& obj) { return Writer<T>::Stringify(obj); }
};
#endif

// TODO : Fork it out into cliserdes
#include "CommandLineArgsReader.h"

enum HelpFormats
{
    Union,
    Enum
};
struct HelpFormat;
struct HelpFormatUnion
{
    std::string              UnionName;
    std::vector<std::string> Types;
};
struct HelpFormatEnum
{
    std::string              EnumName;
    std::vector<std::string> ValidTypes;
};

struct Table
{
    struct ColumnSpan
    {
        size_t      column  = 0;
        size_t      colspan = 0;
        std::string text;
    };

    struct Row
    {
        std::vector<ColumnSpan> columns;
    };

    std::vector<Row> rows;

    void AddRow() { rows.push_back(Row{}); }

    void AddColumn(size_t col, size_t span, std::string text)
    {
        ColumnSpan col1{col, span, text};
        rows.back().columns.push_back(std::move(col1));
    }

    void AddTable(size_t colindx, size_t span, const Table& table)
    {
        auto lines = table.PrintAsLines();
        for (auto& l : lines)
        {
            AddRowColumn(colindx, span, l);
        }
    }

    void AddRowColumn(size_t col, size_t span, std::string text)
    {
        AddRow();
        AddColumn(col, span, text);
    }

    size_t _FindColumnWidth(const ColumnSpan& col) const { return static_cast<size_t>(col.text.length() + 4u); }

    void _PrintColumnTextToBuffer(char* buffer, size_t /*available*/, const ColumnSpan& col) const
    {
        if (col.text.empty()) return;
        std::copy(col.text.begin(), col.text.end(), buffer);
    }

    size_t _FindTableWidth() const
    {
        size_t width = 0;
        for (auto& colwidth : _FindColumnWidths())
        {
            width += colwidth;
        }
        return width;
    }

    std::vector<size_t> _FindColumnWidths() const
    {
        std::vector<size_t> columnwidths;
        for (const auto& row : rows)
        {
            for (const auto& col : row.columns)
            {
                if (columnwidths.size() <= col.column)
                {
                    columnwidths.resize(col.column + 1, 0);
                }
                if (col.colspan == 0)
                {
                    auto neededwidth         = _FindColumnWidth(col);
                    columnwidths[col.column] = std::max(columnwidths[col.column], neededwidth);
                }
            }
        }

        for (auto& row : rows)
        {
            for (auto& col : row.columns)
            {
                auto farright    = std::min(static_cast<size_t>(col.colspan) + col.column, columnwidths.size() - 1);
                auto neededwidth = _FindColumnWidth(col);
                auto remaining   = neededwidth;
                for (size_t i = col.column; i < farright && remaining > columnwidths[i]; i++)
                {
                    remaining -= columnwidths[i];
                }
                columnwidths[farright] = std::max(columnwidths[farright], remaining);
            }
        }

        return columnwidths;
    }

    std::vector<std::string> PrintAsLines() const
    {
        auto                widths = _FindColumnWidths();
        std::vector<size_t> offsets{0};
        offsets.reserve(widths.size());
        size_t bufferwidth = 1;
        for (size_t i = 0; i < widths.size(); i++)
        {
            bufferwidth += widths[i];
            offsets.push_back(bufferwidth - 1);
        }
        if (bufferwidth == 1) return {};

        std::unique_ptr<char[]>  buffer(new char[bufferwidth]());
        std::vector<std::string> lines;
        for (auto& row : rows)
        {
            std::fill(buffer.get(), buffer.get() + (bufferwidth), ' ');
            buffer[bufferwidth - 1] = 0;
            for (auto& col : row.columns)
            {
                auto offset = offsets[col.column];
                _PrintColumnTextToBuffer(&buffer[offset], bufferwidth - offset, col);
            }
            lines.push_back(buffer.get());
        }
        return lines;
    }
};

template <typename TStruct> struct CommandLineArgs
{
    struct Exception
    {
    };

    struct HelpExceptionTraits
    {
        static void ConstructMessage(Logging::PrettyPrintStream& buffer, std::string helpstr)
        {
            Logging::PrettyPrint::PrintProperties(buffer, {}, "help", helpstr);
        }
    };

    struct InvalidArgsExceptionTraits
    {
        static void ConstructMessage(Logging::PrettyPrintStream& buffer, std::string helpstr)
        {
            Logging::PrettyPrint::PrintProperties(buffer, {}, "help", helpstr);
        }
    };

    struct HelpException : Logging::Exception<HelpExceptionTraits>, Exception
    {
        HelpException(std::vector<std::string>&& str) :
            Logging::Exception<HelpExceptionTraits>(CorrelationVector::Create(), ""), helpinfolines(std::move(str))
        {
        }
        std::vector<std::string> helpinfolines;
    };

    // struct InvalidArgsException : Logging::Exception<InvalidArgsExceptionTraits>, Exception {};

    struct ReaderHandler : public CommandLineArgsReader::Handler
    {
        ReaderHandler(TStruct& obj) : _visitor(obj) {}

        virtual void HandleValue(bool value) override
        {
            _visitor.SetValue(value);
            _visitor.GoBackUp();
        }

        virtual void HandleValue(std::string_view const& str) override
        {
            _visitor.SetValue(str);
            _visitor.GoBackUp();
        }

        virtual void HandleEnum(std::string_view const& str) override
        {
            _visitor.SetValue(str);
            _visitor.GoBackUp();
        }

        virtual void UnionType(std::string_view const& str) override { _visitor.SetValue(str); }

        virtual void ListStart() override {}
        virtual void ListEnd() override { _visitor.GoBackUp(); }
        virtual void ObjStart() override {}
        virtual void ObjEnd() override { _visitor.GoBackUp(); }

        virtual void ObjKey(std::string_view const& key) override
        {
            while (!_visitor.TrySelect(Value{key}))
            {
                _visitor.GoBackUp();
            }
        }

        virtual void ObjKey(size_t index) override { _visitor.Select(Value{index}); }
        virtual void AddKey(size_t index) override { _visitor.Select(Value{index}); }

        virtual std::shared_ptr<CommandLineArgsReader::Definition> GetCurrentContext() override
        {
            CommandLineArgsReader::Definition::Type type;
            switch (_visitor.GetDataTypeHint())
            {
            case ReflectionBase::DataType::List: type = CommandLineArgsReader::Definition::Type::List; break;
            case ReflectionBase::DataType::Object: type = CommandLineArgsReader::Definition::Type::Object; break;
            case ReflectionBase::DataType::Value: type = CommandLineArgsReader::Definition::Type::Value; break;
            case ReflectionBase::DataType::Enum: type = CommandLineArgsReader::Definition::Type::Enum; break;
            case ReflectionBase::DataType::Union: type = CommandLineArgsReader::Definition::Type::Union; break;
            case ReflectionBase::DataType::Invalid: type = CommandLineArgsReader::Definition::Type::Invalid; break;
            case ReflectionBase::DataType::Unknown: [[fallthrough]];    // TODO
            default:
                throw std::logic_error("Unknown Data Type");
                // TODO: case ReflectionBase::DataType::Unknown: type = CommandLineArgsReader::Definition::Type::Unknown; break;
            }

            return std::make_shared<CommandLineArgsReader::Definition>("empty", type);
        }

        std::unique_ptr<Table> _PrintUnion(const ::ReflectionBase::DataInfo& info)
        {
            std::unique_ptr<Table> table(new Table());
            table->AddRowColumn(0, 1, "List of available options:");
            assert(info.acceptablevalues.size() == info.children.size());
            for (size_t i = 0; i < info.acceptablevalues.size(); i++)
            {
                table->AddRowColumn(0, 0, (static_cast<shared_string>(info.acceptablevalues[i])).str());
                table->AddColumn(1, 0, info.children[i]->description);
            }
            return table;
        }

        std::unique_ptr<Table> _PrintUsage(const ::ReflectionBase::DataInfo& info)
        {
            std::unique_ptr<Table>   table(new Table());
            std::vector<std::string> args;

            // print usage
            switch (info.datatype)
            {
            case ::ReflectionBase::DataType::Union:
                args.push_back("<" + info.name.str() + ">");
                args.push_back("...");
                break;
            case ::ReflectionBase::DataType::Object:
                for (const auto& c : info.children)
                {
                    if (!c->required()) break;
                    args.push_back(("<" + c->name.str() + ">"));
                }
                break;
            case ::ReflectionBase::DataType::Value: [[fallthrough]];
            case ::ReflectionBase::DataType::Enum: [[fallthrough]];
            case ::ReflectionBase::DataType::List: [[fallthrough]];

            case ::ReflectionBase::DataType::Unknown: [[fallthrough]];
            case ::ReflectionBase::DataType::Invalid: [[fallthrough]];

            default: throw std::logic_error("TODO");
            }

            table->AddRowColumn(0, 0, "Usage:");
            std::stringstream sstr;
            for (auto& s : args) sstr << s;
            table->AddColumn(1, 255, sstr.str());

            table->AddRowColumn(0, 255, "Description:");
            table->AddRowColumn(0, 255, info.description);
            table->AddRowColumn(0, 255, "");

            switch (info.datatype)
            {
            case ::ReflectionBase::DataType::Union: table->AddTable(0, 255, *_PrintUnion(info)); break;
            case ::ReflectionBase::DataType::Object:
            {
                bool cont = true;
                for (const auto& c : info.children)
                {
                    cont = cont && c->required();
                    if (cont) continue;
                    table->AddRowColumn(0, 0, "--" + c->name.str());
                    table->AddColumn(1, 255, c->description);
                }
            }
            break;
            case ::ReflectionBase::DataType::Value: [[fallthrough]];
            case ::ReflectionBase::DataType::Enum: [[fallthrough]];
            case ::ReflectionBase::DataType::List: [[fallthrough]];

            case ::ReflectionBase::DataType::Unknown: [[fallthrough]];
            case ::ReflectionBase::DataType::Invalid: [[fallthrough]];

            default: throw std::logic_error("TODO");
            }

            return table;
        }

        void _RecursivelyAddHelp(std::vector<std::string>&                                lines,
                                 std::deque<std::shared_ptr<::ReflectionBase::DataInfo>>& pending,
                                 int /*depth*/)
        {
            auto& info      = pending.front();
            auto  infolines = _PrintUsage(*info)->PrintAsLines();
            for (auto& l : infolines)
            {
                lines.push_back(l);
            }
            pending.pop_front();
        }

        std::vector<std::string> _GenerateContextHelp()
        {
            std::vector<std::string>                                lines;
            std::deque<std::shared_ptr<::ReflectionBase::DataInfo>> pending;
            pending.push_back(_visitor.GetDataInfo());
            for (int i = 0; pending.size() > 0; i++)
            {
                _RecursivelyAddHelp(lines, pending, i);
            }
            return lines;
        }

        virtual std::string GenerateHelp() override
        {
            std::stringstream ss;
            _helpInfo = _GenerateContextHelp();
            for (auto& l : _helpInfo)
            {
                ss << std::move(l);
            }
            return ss.str();

            // Print Current Context

            // Print Error

            // Print context help

            // throw HelpException(std::move());
        }

        Stencil::Visitor<TStruct> _visitor;

        std::vector<std::string> _helpInfo;
    };

    // TODO: Get rid of pointers
    template <typename TStr> void Load(TStruct& obj, std::span<TStr> const& args)
    {
        ReaderHandler handler(obj);
        CommandLineArgsReader(&handler).Parse(args);
        std::swap(_helpInfo, handler._helpInfo);
    }

    template <typename TStr> std::unique_ptr<TStruct> Parse(std::span<TStr> const& args)
    {
        std::unique_ptr<TStruct> ptr(new TStruct());
        Load(ptr.get(), args);
        return ptr;
    }

    std::vector<std::string> const& HelpInfo() const { return _helpInfo; }

    std::vector<std::string> _helpInfo;
};
