#pragma once
#include <DataModel.h>
#include <deque>
class JsonDataModel;

#ifdef USE_RAPIDJSON

#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

struct Json
{
    template <typename TStruct> struct RapidJsonReader : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, RapidJsonReader<TStruct>>
    {
        RapidJsonReader(TStruct* ptr) : _tracker(ptr, nullptr) {}

#define SAFEEXEC(stmt) \
    try                \
    {                  \
        stmt;          \
        return true;   \
    }                  \
    catch (...)        \
    {                  \
        return false;  \
    }

        bool Default()
        {
            throw std::logic_error("TODO");
            return false;
        }

        bool Null() { SAFEEXEC(_tracker.HandleValue(Value(nullptr), nullptr)); }
        bool Bool(bool val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool Int(int val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool Uint(unsigned val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool Int64(int64_t val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool Uint64(uint64_t val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }
        bool Double(double val) { SAFEEXEC(_tracker.HandleValue(Value(val), nullptr)); }

        bool String(const char* str, rapidjson::SizeType length, bool /*copy*/)
        {
            SAFEEXEC(_tracker.HandleValue(Value(shared_string::make(std::string_view(str, length))), nullptr));
        }

        bool Key(const char* str, rapidjson::SizeType length, bool /*copy*/)
        {
            SAFEEXEC(_tracker.ObjKey(Value(shared_string::make(std::string_view(str, length))), nullptr));
        }

        bool StartObject() { SAFEEXEC(_tracker.ObjStart(nullptr)); }
        bool EndObject(rapidjson::SizeType /*memberCount*/) { SAFEEXEC(_tracker.ObjEnd()); }
        bool StartArray()
        {
            {
                SAFEEXEC(_tracker.ListStart(nullptr));
            }
        }
        bool EndArray(rapidjson::SizeType /*elementCount*/) { SAFEEXEC(_tracker.ListEnd()); }

        ReflectionServices::StateTraker<TStruct, void*> _tracker;
    };

    template <typename T, typename = void> struct RapidJsonWriter
    {
        static std::string Stringify(const T& obj)
        {
            static_assert(std::is_same<T, T>::value, "Dont know how to stringify");
            throw std::logic_error("");
        }
    };

    template <typename T> struct RapidJsonWriter<T, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
    {
        static std::string Stringify(const T& obj)
        {
            static_assert(std::is_same<T, T>::value, "Dont know how to stringify");
            throw std::logic_error("");
        }
    };

    template <typename T> struct RapidJsonWriter<T, std::enable_if_t<Value::Supported<T>::value>>
    {
        static std::string Stringify(const T& obj) { return Value(obj).convert<shared_string>().str(); }
    };

    template <typename T> struct RapidJsonWriter<T, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const T& obj) { return obj.Id(); }
    };

    template <typename T>
    struct RapidJsonWriter<std::unique_ptr<T>, std::enable_if_t<std::is_base_of<::ReflectionBase::InterfaceMarker, T>::value>>
    {
        static std::string Stringify(const std::unique_ptr<T>& obj) { return std::string(obj->GetObjectUuid().ToString()); }
    };

    template <typename TStruct> static void Load(TStruct& obj, const std::string& str)
    {
        RapidJsonReader<TStruct> handler(&obj);
        rapidjson::Reader        reader;
        rapidjson::StringStream  ss(str.c_str());
        reader.Parse(ss, handler);
    }

    template <typename TStruct> static std::unique_ptr<TStruct> Parse(const std::string& str)
    {
        std::unique_ptr<TStruct> ptr(new TStruct());
        Load(*ptr.get(), str);
        return ptr;
    }

    template <typename T> static std::string Stringify(const T& obj) { return RapidJsonWriter<T>::Stringify(obj); }
};

#endif

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

    size_t _FindColumnWidth(const ColumnSpan& col) const { return (size_t)col.text.length() + 4; }

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
                auto farright    = std::min((size_t)(col.colspan) + col.column, columnwidths.size() - 1);
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
            Logging::PrettyPrint::PrintProperties(buffer, nullptr, "help", helpstr);
        }
    };

    struct InvalidArgsExceptionTraits
    {
        static void ConstructMessage(Logging::PrettyPrintStream& buffer, std::string helpstr)
        {
            Logging::PrettyPrint::PrintProperties(buffer, nullptr, "help", helpstr);
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
        ReaderHandler(TStruct* obj) : _tracker(obj, nullptr) {}
        virtual void HandleValue(bool value) override { _tracker.HandleValue(Value(value), nullptr); }

        virtual void HandleValue(std::string_view const& str) override { _tracker.HandleValue(Value(str), nullptr); }

        virtual void HandleEnum(std::string_view const& str) override { _tracker.HandleEnum(Value(str), nullptr); }

        virtual void UnionType(std::string_view const& str) override { _tracker.UnionType(Value(str), nullptr); }

        virtual void ListStart() override
        {
            // auto sub = FindComponent(0);
            // assert(sub != nullptr);
            _tracker.ListStart(nullptr);
        }
        virtual void ListEnd() override { _tracker.ListEnd(); }

        virtual void ObjStart() override { _tracker.ObjStart(nullptr); }
        virtual void ObjEnd() override { _tracker.ObjEnd(); }
        virtual void ObjKey(std::string_view const& key) override { _tracker.ObjKey(Value{key}, nullptr); }
        virtual void ObjKey(size_t index) override { _tracker.ObjKey(Value{index}, nullptr); }

        virtual std::shared_ptr<CommandLineArgsReader::Definition> GetCurrentContext() override
        {
            CommandLineArgsReader::Definition::Type type;
            switch (_tracker.GetDataTypeHint())
            {
            case ReflectionBase::DataType::List: type = CommandLineArgsReader::Definition::Type::List; break;
            case ReflectionBase::DataType::Object: type = CommandLineArgsReader::Definition::Type::Object; break;
            case ReflectionBase::DataType::Value: type = CommandLineArgsReader::Definition::Type::Value; break;
            case ReflectionBase::DataType::Enum: type = CommandLineArgsReader::Definition::Type::Enum; break;
            case ReflectionBase::DataType::Union: type = CommandLineArgsReader::Definition::Type::Union; break;
            case ReflectionBase::DataType::Invalid: type = CommandLineArgsReader::Definition::Type::Invalid; break;
            case ReflectionBase::DataType::Unknown: type = CommandLineArgsReader::Definition::Type::Unknown; break;
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
                table->AddRowColumn(0, 0, ((shared_string)info.acceptablevalues[i]).str());
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
            default: throw std::logic_error("TODO");
            }

            return table;
        }

        void _RecursivelyAddHelp(std::vector<std::string>&                                lines,
                                 std::deque<std::unique_ptr<::ReflectionBase::DataInfo>>& pending,
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
            std::deque<std::unique_ptr<::ReflectionBase::DataInfo>> pending;
            pending.push_back(std::move(_tracker.GetHandler()->GetDataInfo()));
            for (int i = 0; pending.size() > 0; i++)
            {
                _RecursivelyAddHelp(lines, pending, i);
            }
            return lines;
        }

        virtual std::string GenerateHelp()
        {
            // Print Current Context

            // Print Error

            // Print context help

            throw HelpException(std::move(_GenerateContextHelp()));
        }

        ReflectionServices::StateTraker<TStruct, std::shared_ptr<CommandLineArgsReader::Definition>> _tracker;
    };

    void Load(TStruct* obj, size_t argc, const char* argv[])
    {
        ReaderHandler handler(obj);
        CommandLineArgsReader(&handler).Parse(argc, argv);
    }

    std::unique_ptr<TStruct> Parse(size_t argc, const char* argv[])
    {
        std::unique_ptr<TStruct> ptr(new TStruct());
        Load(ptr.get(), argc, argv);
        return ptr;
    }
};
