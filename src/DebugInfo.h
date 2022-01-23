#pragma once
#pragma warning(push, 3)
#include <fmt/format.h>

#include <fmt/ostream.h>
#include <fmt/xchar.h>
#pragma warning(pop)

#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>

namespace IDLDebug
{

struct ErrorAggregator
{
    void AddContextInfo(size_t indent, std::wstring_view const& what, std::wstring_view const& str)
    {
        _lines.push_back(fmt::format(L"{:\t>{}}[{}]\t{}", L"", indent, what, str));
    }
    void Clear() { _lines.clear(); }

    std::wstring GetErrors()
    {
        std::wstringstream str;
        for (auto& err : _lines) { str << err << std::endl; }
        return str.str();
    }

    ErrorAggregator()
    {
        assert(GetPtr() == nullptr);
        GetPtr() = this;
    }

    size_t Incr() { return _indent++; }
    size_t Decr()
    {
        assert(_indent > 0);
        return _indent--;
    }

    ~ErrorAggregator()
    {
        GetPtr() = nullptr;
        if (std::uncaught_exceptions() > 0) { std::wcerr << GetErrors(); }
    }

    private:
    static ErrorAggregator*& GetPtr()
    {
        thread_local ErrorAggregator* ptr = nullptr;
        return ptr;
    }

    static ErrorAggregator& Get() { return *GetPtr(); }

    size_t                    _indent = 0;
    std::vector<std::wstring> _lines;
    friend struct ThreadActionContextImpl;
};

struct ThreadActionContextImpl
{
    ThreadActionContextImpl(std::wstring_view const& what, std::function<std::wstring()>&& func) : _what(what), _func(std::move(func)) {}
    ~ThreadActionContextImpl()
    {
        if (std::uncaught_exceptions() > 0) { ErrorAggregator::Get().AddContextInfo(_indent, _what, _func()); }
        ErrorAggregator::Get().Decr();
    }

    size_t                        _indent = ErrorAggregator::Get().Incr();
    std::wstring_view             _what;
    std::function<std::wstring()> _func;
};

inline auto ThreadActionContext(std::wstring_view const& what, std::function<std::wstring()>&& func)
{
    return ThreadActionContextImpl(what, std::move(func));
}

struct DebugContext
{
    std::wstring context{};
    std::wstring filename{};
    size_t       row{}, col{};

    std::wstring str() const
    {
        std::wstringstream strm;
        strm << context.c_str() << L" -- " << filename.c_str() << L" [" << row << L":" << col << L"]";
        return std::wstring(strm.str());
    }
};

}    // namespace IDLDebug

#define ACTION_CONTEXT_IMPL1(line, file, fnname, fn) auto actionctxvar_##line = IDLDebug::ThreadActionContextImpl(L##file fnname, (fn))
#define ACTION_CONTEXT_IMPL2(line, file, fnname, fn) ACTION_CONTEXT_IMPL1(line, file, fnname, fn)
#ifdef _WIN32
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, __FUNCTION__, fn)
#else
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, "", fn)
#endif
