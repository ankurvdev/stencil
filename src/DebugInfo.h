#pragma once
#include <CommonMacros.h>

SUPPRESS_WARNINGS_START
SUPPRESS_FMT_WARNINGS
SUPPRESS_STL_WARNINGS
#include <fmt/ostream.h>
#include <fmt/xchar.h>

#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

SUPPRESS_WARNINGS_END

namespace IDLDebug
{

struct ErrorAggregator
{
    void AddContextInfo(size_t indent, std::wstring_view const& what, std::wstring_view const& str)
    { _lines.push_back(fmt::format(L"{:\t>{}}[{}]\t{}", L"", indent, what, str)); }
    void Clear() { _lines.clear(); }

    std::wstring GetErrors()
    {
        std::wstringstream str;
        for (auto& err : _lines) { str << err << '\n'; }
        return str.str();
    }

    ErrorAggregator()
    {
        assert(GetPtr_() == nullptr);
        GetPtr_() = this;
    }

    size_t Incr() { return _indent++; }
    size_t Decr()
    {
        assert(_indent > 0);
        return _indent--;
    }

    ~ErrorAggregator()
    {
        GetPtr_() = nullptr;
        if (std::uncaught_exceptions() > 0) { std::wcerr << GetErrors(); }
    }
    CLASS_DELETE_COPY_AND_MOVE(ErrorAggregator);

    private:
    static ErrorAggregator*& GetPtr_()
    {
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wunique-object-duplication")
        thread_local ErrorAggregator* Ptr = nullptr;
        SUPPRESS_WARNINGS_END
        return Ptr;
    }

    static ErrorAggregator& Get_() { return *GetPtr_(); }

    size_t                    _indent = 0;
    std::vector<std::wstring> _lines;
    friend struct ThreadActionContextImpl;
};

struct ThreadActionContextImpl
{
    ThreadActionContextImpl(std::wstring_view const& whatIn, std::function<std::wstring()>&& funcIn) : what(whatIn), func(std::move(funcIn))
    {}
    ~ThreadActionContextImpl()
    {
        if (std::uncaught_exceptions() > 0) { ErrorAggregator::Get_().AddContextInfo(indent, what, func()); }
        ErrorAggregator::Get_().Decr();
    }
    CLASS_DELETE_COPY_AND_MOVE(ThreadActionContextImpl);

    size_t                        indent = ErrorAggregator::Get_().Incr();
    std::wstring_view             what;
    std::function<std::wstring()> func;
};

inline auto ThreadActionContext(std::wstring_view const& what, std::function<std::wstring()>&& func)
{ return ThreadActionContextImpl(what, std::move(func)); }

struct DebugContext
{
    std::wstring context;
    std::wstring filename;
    size_t       row{}, col{};

    [[nodiscard]] std::wstring Str() const
    {
        std::wstringstream strm;
        strm << context.c_str() << L" -- " << filename.c_str() << L" [" << row << L":" << col << L"]";
        return std::wstring(strm.str());
    }
};

}    // namespace IDLDebug

#define ACTION_CONTEXT_IMPL1(line, file, fnname, fn) auto actionctxvar_##line = IDLDebug::ThreadActionContextImpl(L##file fnname, (fn)) //NOLINT
#define ACTION_CONTEXT_IMPL2(line, file, fnname, fn) ACTION_CONTEXT_IMPL1(line, file, fnname, fn) //NOLINT
#ifdef _MSC_VER
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, __FUNCTION__, fn)
#else
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, "", fn) //NOLINT
#endif
