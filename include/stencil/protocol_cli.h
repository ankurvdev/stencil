#pragma once
#include "protocol_string.h"
#include "stencil/typetraits.h"
#include "visitor.h"

#include <chrono>
#include <fmt/format.h>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace Stencil
{
template <typename T> struct Attribute<Stencil::AttributeType::Description, T>
{
    static std::string_view Value()
    {
#if defined DEBUG
        return "<unspecified>";
#else
        return "";
#endif
    }
};

template <typename T> struct Attribute_ShortName
{
    static char Value() { return typeid(T).name()[0]; }
};

/*
 * Key Points
 * Context
 *      A 'dash' moves up the context when inside iterable or indexable
 *      A 'double-dash' moves the context to root
 *      For Iterables Context auto slides to the Iterable subtype and keeps sliding until a non-iterable is encountered.
 *      For Indexable Context slides on named field or --field
 * Primitives
 *      --field=value Only used for primitives inside indexables
 *      After parsing context automatically goes up
 *      If in a list parent context look for comma and split
 * Iterables:
 *      Primitives:
            Can be clubbed into a single arg using comma separator 1,2,3,4
            Can be specified as individual args 1 2 3 4
            Use dash to move up the context
 *      Iterables
 *          Auto context slide down
 *      Indexable
 *          Switch to indexable context
 * Indexable
 *
 */

template <typename T> struct ArgsIterator
{
    ArgsIterator(T&& ctx) : _ctx(ctx) {}

    std::string_view move_next()
    {
        std::string_view retval(_ctx.at(_current));
        _current++;
        return retval;
    }

    void move_back() { _current--; }

    std::string_view operator*() const { return std::string_view(_ctx.at(_current)); }

    bool   valid() const { return _current < _ctx.count(); }
    bool   root_ctx_requested() const { return _root_ctx_requested; }
    void   clear_root_ctx_requested() { _root_ctx_requested = false; }
    void   mark_root_ctx_requested() { _root_ctx_requested = true; }
    bool   _root_ctx_requested{false};
    bool   help_requested{false};
    size_t _current{0};
    T      _ctx;
};

struct ProtocolCLI
{
    using InType  = ArgsIterator<std::string_view>;
    using OutType = std::vector<std::string>;
};

template <typename, typename = void> struct is_specialized : std::false_type
{};

template <typename T> struct is_specialized<T, std::void_t<decltype(T{})>> : std::true_type
{};

template <typename T>
concept ConceptHasProtocolString = is_specialized<Stencil::SerDes<T, Stencil::ProtocolString>>::value;

static_assert(ConceptHasProtocolString<std::chrono::time_point<std::chrono::system_clock>>, "Chrono should be defined");
static_assert(ConceptHasProtocolString<uint64_t>, "uint64_t should be defined");
static_assert(!ConceptHasProtocolString<std::vector<std::string>>, "void");
static_assert(ConceptHasProtocolString<shared_string>, "shared_string");
static_assert(ConceptHasProtocolString<shared_wstring>, "shared_wstring");
static_assert(!ConceptIterable<shared_string>, "shared_string");

struct IterableValuesVisitors
{
    virtual ~IterableValuesVisitors() = default;
};

template <Stencil::ConceptIndexable T> struct SerDes<T, ProtocolCLI>
{
    template <typename TKey, typename TVal, typename TObj, typename TContext>
    static void _WriteForKeyValue(TKey const& k, TVal const& v, TObj const& /*obj*/, TContext& ctx)
    {
        if constexpr (ConceptHasProtocolString<TVal>)
        {
            std::stringstream ss;
            fmt::print(ss, "--");
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
            fmt::print(ss, "=");
            SerDes<TVal, ProtocolString>::Write(ss, v);
            ctx.push_back(ss.str());
            return;
        }

        else if constexpr (ConceptIndexable<TVal>)
        {
            std::stringstream ss;
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
            ctx.push_back(ss.str());
            SerDes<TVal, ProtocolCLI>::Write(ctx, v);
            // ctx.push_back("-");
            return;
        }

        else if constexpr (ConceptIterable<TVal>)
        {
            int               type = -1;
            std::stringstream ss;

            Visitor<TVal>::VisitAll(v, [&](auto, auto& v1) {
                if constexpr (ConceptHasProtocolString<std::remove_cvref_t<decltype(v1)>>)
                {
                    // Iterable of Values. Use shorthand
                    if (type != -1 && type != 0) throw std::logic_error("Iterable of mixed types unsupported");
                    if (type == -1)
                    {
                        fmt::print(ss, "--");
                        SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                        fmt::print(ss, "=");
                    }
                    else { fmt::print(ss, ","); }
                    type = 0;
                    SerDes<std::remove_cvref_t<decltype(v1)>, ProtocolString>::Write(ss, v1);
                }
                else
                {
                    if (type != -1 && type != 1) throw std::logic_error("Iterable of mixed types unsupported");
                    // For Iterable of Iterable and Iterable of Indexables.
                    // Just use the recursive Iterable
                    if (type == -1)
                    {
                        SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
                        ctx.push_back(ss.str());
                    }

                    SerDes<std::remove_cvref_t<decltype(v1)>, ProtocolCLI>::Write(ctx, v1);
                    type = 1;
                }
            });
            if (type == 0) ctx.push_back(ss.str());
            if (type != -1) ctx.push_back("-");
            return;
        }
        else
        {
            // std::string str = typeid(TVal).name();
            TODO("Unknown");
        }
    }

    [[noreturn]] static void _Error() { throw std::logic_error("Unsupported type"); }

    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        Visitor<T>::VisitAll(obj, [&](auto const& k, auto const& v) { _WriteForKeyValue(k, v, obj, ctx); });
        ctx.push_back("-");
    }

    static bool _IsBooleanValue(std::string_view const& value, bool& boolval)
    {
        if (value == "1" || value == "on" || value == "On" || value == "ON" || value == "true" || value == "True" || value == "TRUE"
            || value == "y" || value == "yes" || value == "Yes" || value == "YES")
        {
            boolval = true;
            return true;
        }
        else if (value == "0" || value == "off" || value == "Off" || value == "OFF" || value == "false" || value == "False"
                 || value == "FALSE" || value == "n" || value == "no" || value == "NO" || value == "No")
        {
            boolval = false;
            return true;
        }
        else { return false; }
    }

    template <typename TVal, typename TRhs>
    static void
    _ReadForIterableValue(std::unordered_map<void*, std::unique_ptr<IterableValuesVisitors>>& visitorState, TVal& val, TRhs const& value)
    {
        size_t si = 0;
        size_t ei = value.find(',');

        if (ei == std::string_view::npos)
        {
            if constexpr (ConceptHasProtocolString<TVal>)
            {
                SerDes<TVal, ProtocolString>::Read(val, value);
                return;
            }
            else if constexpr (ConceptIterable<TVal>)
            {
                if constexpr (ConceptIterable<TVal>)
                {

                    struct CustomVisitor : IterableValuesVisitors
                    {
                        virtual ~CustomVisitor() override = default;
                        typename Visitor<TVal>::Iterator it;
                    };
                    CustomVisitor* visitor{nullptr};

                    auto it = visitorState.find(&val);
                    if (it == visitorState.end())
                    {
                        auto newvisitor    = std::make_unique<CustomVisitor>();
                        visitor            = newvisitor.get();
                        visitorState[&val] = std::move(newvisitor);
                        Visitor<TVal>::IteratorBegin(visitor->it, val);
                    }
                    else
                    {
                        visitor = static_cast<CustomVisitor*>(it->second.get());
                        Visitor<TVal>::IteratorMoveNext(visitor->it, val);
                    }
                    if (!Visitor<TVal>::IteratorValid(visitor->it, val))
                    {
                        throw std::runtime_error("Cannot Visit Next Item on the iterable");
                    }

                    Visitor<TVal>::Visit(visitor->it, val, [&](auto& val2) {
                        using SubTVal = std::remove_cvref_t<decltype(val2)>;
                        if constexpr (ConceptHasProtocolString<SubTVal>) { SerDes<SubTVal, ProtocolString>::Read(val2, value); }
                        else { throw std::logic_error("Unsupported type"); }
                    });
                    // Just insert at the last
                }
            }
            else { throw std::logic_error("Cannot read into value"); }
        }
        else
        {
            if constexpr (ConceptIterable<TVal>)
            {
                typename Visitor<TVal>::Iterator it;

                bool valid = false;
                while (true)
                {
                    if (!valid)
                    {
                        Visitor<TVal>::IteratorBegin(it, val);
                        valid = true;
                    }
                    else { Visitor<TVal>::IteratorMoveNext(it, val); }

                    if (!Visitor<TVal>::IteratorValid(it, val)) { throw std::runtime_error("Cannot Visit Next Item on the iterable"); }

                    auto subval = value.substr(si, ei - si);

                    Visitor<TVal>::Visit(it, val, [&](auto& val2) {
                        using SubTVal = std::remove_cvref_t<decltype(val2)>;
                        if constexpr (ConceptHasProtocolString<SubTVal>) { SerDes<SubTVal, ProtocolString>::Read(val2, subval); }
                        else { throw std::logic_error("Unsupported type"); }
                    });

                    if (ei == std::string_view::npos) break;
                    si = ei + 1;
                    ei = value.find(',', si);
                }
            }
            else if constexpr (ConceptHasProtocolString<TVal>)
            {
                SerDes<TVal, ProtocolString>::Read(val, value);
                return;
            }
            else { throw std::logic_error("Cannot read into value"); }
        }
        return;
    }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        using Traits    = typename Stencil::TypeTraitsForIndexable<T>;
        using TraitsKey = typename Traits::Key;
        std::unordered_map<void*, std::unique_ptr<IterableValuesVisitors>> iterableVistors;
        while (ctx.valid() && !ctx.root_ctx_requested())
        {
            std::string_view token = ctx.move_next();
            if (token.size() == 0) continue;
            if (token[0] == '-')
            {
                if (token.size() == 1) return;
                if (token[1] != '-') { TODO("handle short args"); }
                if (token.size() == 2)
                {
                    ctx.mark_root_ctx_requested();
                    return;
                }
                if (token == "--help")
                {
                    ctx.help_requested = true;
                    return;
                }
                auto index = token.find('=');
                if (index == std::string_view::npos)
                {
                    // If type is bool
                    // if type is not bool then the next argument has to be the value

                    if (token.starts_with("--no-"))
                    {
                        auto keystr = std::string(token.substr(5));
                        for (auto& c : keystr) { c = (c == '-') ? '_' : c; }
                        TraitsKey key;
                        SerDes<TraitsKey, ProtocolString>::Read(key, keystr);

                        // Has to be a boolean
                        Visitor<T>::VisitKey(obj, key, [&](auto& val) {
                            if constexpr (std::is_same_v<decltype(val), bool&>) { val = false; }
                            else { throw std::logic_error(fmt::format("Expected Boolean type for key: {}", keystr)); }
                        });
                    }
                    else
                    {
                        auto keystr = std::string(token.substr(2));
                        for (auto& c : keystr) { c = (c == '-') ? '_' : c; }
                        TraitsKey key;
                        SerDes<TraitsKey, ProtocolString>::Read(key, keystr);

                        // Has to be a boolean
                        Visitor<T>::VisitKey(obj, key, [&](auto& val) {
                            if constexpr (std::is_same_v<decltype(val), bool&>)
                            {
                                val = true;
                                // check if we need to eat up the next token
                                if (ctx.valid())
                                {
                                    auto next = ctx.move_next();
                                    if (!_IsBooleanValue(next, val)) { ctx.move_back(); }
                                }
                            }
                            else
                            {
                                if (!ctx.valid()) { throw std::invalid_argument(fmt::format("Missing value for {}", keystr)); }
                                auto next = ctx.move_next();
                                _ReadForIterableValue(iterableVistors, val, next);
                            }
                        });
                    }
                }
                else
                {
                    auto keystr = std::string(token.substr(2, index - 2));
                    for (auto& c : keystr) { c = (c == '-') ? '_' : c; }
                    auto value = token.substr(index + 1);

                    TraitsKey key;
                    SerDes<TraitsKey, ProtocolString>::Read(key, keystr);
                    Visitor<T>::VisitKey(obj, key, [&](auto& val) { _ReadForIterableValue(iterableVistors, val, value); });
                }
            }
            else
            {
                // Should we suppport key=val ?
                TraitsKey key;
                SerDes<TraitsKey, ProtocolString>::Read(key, token);

                Visitor<T>::VisitKey(obj, key, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolCLI>::Read(val, ctx); });
            }
        }
    }
};

template <Stencil::ConceptIterable T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        Visitor<T>::VisitAll(obj, [&](auto, auto& v) {
            SerDes<std::remove_cvref_t<decltype(v)>, ProtocolCLI>::Write(ctx, v);
            ctx.push_back("-");
        });
    }

    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        typename Visitor<T>::Iterator it;
        Visitor<T>::IteratorBegin(it, obj);
        bool valid = false;

        while (ctx.valid() && !ctx.root_ctx_requested())
        {
            auto token = ctx.move_next();
            if (token.size() == 0) continue;
            if (token[0] == '-')
            {
                if (token.size() == 1) return;
                if (token[1] != '-') { TODO("handle short args"); }
                if (token == "--help")
                {
                    ctx.help_requested = true;
                    return;
                }
                if (token.size() == 2)
                {
                    ctx.mark_root_ctx_requested();
                    return;
                }
            }
            if (!valid)
            {
                Visitor<T>::IteratorBegin(it, obj);
                valid = true;
            }
            else { Visitor<T>::IteratorMoveNext(it, obj); }
            if (!Visitor<T>::IteratorValid(it, obj)) throw std::runtime_error("Cannot Visit Next Item on the iterable");

            Visitor<T>::Visit(it, obj, [&](auto& val) { SerDes<std::remove_cvref_t<decltype(val)>, ProtocolCLI>::Read(val, ctx); });
        }
    }
};

template <Stencil::ConceptPrimitive T> struct SerDes<T, ProtocolCLI>
{
    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        std::stringstream ss;
        SerDes<T, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }
    template <typename Context> static auto Read(T& obj, Context& ctx)
    {
        if (!ctx.valid() || ctx.root_ctx_requested()) return;
        auto token = ctx.move_next();
        if (token.size() == 0) return;
        if (token[0] == '-')
        {
            if (token.size() == 1) return;
            if (token[1] != '-') { TODO("handle short args"); }
            if (token.size() == 2)
            {
                ctx.mark_root_ctx_requested();
                return;
            }
            if (token == "--help")
            {
                ctx.help_requested = true;
                return;
            }
        }
        SerDes<T, ProtocolString>::Read(obj, token);
    }
};

template <size_t N> struct SerDes<std::array<char, N>, ProtocolCLI>
{
    using TObj = std::array<char, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::stringstream ss;
        SerDes<TObj, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }

    template <typename Context> static auto Read(TObj& /*obj*/, Context& /*ctx*/) { TODO(""); }
};

template <size_t N>
    requires(N <= 4)
struct SerDes<std::array<uint16_t, N>, ProtocolCLI>
{
    using TObj = std::array<uint16_t, N>;

    template <typename Context> static auto Write(Context& ctx, TObj const& obj)
    {
        std::stringstream ss;
        SerDes<TObj, ProtocolString>::Write(ss, obj);
        ctx.push_back(ss.str());
    }

    template <typename Context> static auto Read(TObj& /*obj*/, Context& /*ctx*/) { TODO(""); }
};
}    // namespace Stencil

namespace Stencil::CLI
{
SUPPRESS_WARNINGS_START
SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")

struct ArgcArgv
{
    int                _argc;
    char const* const* _argv;

    auto   at(size_t index) const { return std::string_view(_argv[index]); }
    size_t count() const { return static_cast<size_t>(_argc); }
};

template <typename TStrArr> struct SpanStr
{
    TStrArr const* _strList;

    const auto& at(size_t index) const { return (*_strList)[index]; }
    size_t      count() const { return std::size(*_strList); }
};

template <typename T, typename TInCtx> inline auto _Parse(ArgsIterator<TInCtx>&& argsIt)
{
    struct ParseResult
    {
        T    obj{};
        bool helpRequested = false;
        bool success       = true;
    } parseResult;

    while (argsIt.valid())
    {
        argsIt.clear_root_ctx_requested();

        Stencil::SerDes<T, ProtocolCLI>::Read(parseResult.obj, argsIt);
        if (argsIt.help_requested)
        {
            parseResult.helpRequested = true;
            parseResult.success       = false;
            return parseResult;
        }
    }
    return parseResult;
}

template <typename T> inline auto Parse(int argc, char const* const* const argv)
{
    return _Parse<T>(ArgsIterator<ArgcArgv>(ArgcArgv{argc, argv}));
}

template <typename T, typename TStrArr> inline auto Parse(TStrArr const& args)
{
    return _Parse<T>(ArgsIterator<SpanStr<TStrArr>>(SpanStr<TStrArr>{&args}));
}
SUPPRESS_WARNINGS_END
template <typename T> inline std::vector<std::string> Stringify(T const& obj)
{
    std::vector<std::string> args;
    Stencil::SerDes<T, ProtocolCLI>::Write(args, obj);
    return args;
}

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

    void AddColumn(size_t col, size_t span, std::string_view text)
    {
        ColumnSpan col1{col, span, std::string(text)};
        rows.back().columns.push_back(std::move(col1));
    }

    void AddTable(size_t colindx, size_t span, const Table& table)
    {
        auto lines = table.PrintAsLines();
        for (auto& l : lines) { AddRowColumn(colindx, span, l); }
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
        for (auto& colwidth : _FindColumnWidths()) { width += colwidth; }
        return width;
    }

    std::vector<size_t> _FindColumnWidths() const
    {
        std::vector<size_t> columnwidths;
        for (const auto& row : rows)
        {
            for (const auto& col : row.columns)
            {
                if (columnwidths.size() <= col.column) { columnwidths.resize(col.column + 1, 0); }
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
                for (size_t i = col.column; i < farright && remaining > columnwidths[i]; i++) { remaining -= columnwidths[i]; }
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

template <typename T> inline auto GenerateHelp(T const& obj)
{
    std::vector<std::string> args;
    std::unique_ptr<Table>   table(new Table());

    table->AddRowColumn(0, 0, "Usage:");
    table->AddColumn(1, 255, fmt::format("{}", "name"));

    table->AddRowColumn(0, 255, "Description:");
    table->AddRowColumn(0, 255, fmt::format("{}", Stencil::Attribute<Stencil::AttributeType::Description, T>::Value()));
    table->AddRowColumn(0, 255, "");

    if constexpr (ConceptIndexable<T>)
    {
        Visitor<T>::VisitAll(obj, [&](auto k, auto&) {
            std::stringstream ss;
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
            table->AddRowColumn(0, 0, fmt::format("--{}", ss.str()));
            table->AddColumn(1, 255, Stencil::Attribute<Stencil::AttributeType::Description, std::remove_cvref_t<decltype(k)>>::Value());
        });
    }
    return table->PrintAsLines();
}

}    // namespace Stencil::CLI
