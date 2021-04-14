#pragma once
#include <cassert>
#include <functional>
#include <sstream>
namespace IDLDebug
{
using Str = Binding::Str;

struct ErrorAggregator
{
    void AddContextInfo(size_t indent, Str::View const& what, Str::View const& str)
    {
        std::wstringstream stream;
        for (size_t i = 0; i < indent; i++)
        {
            stream << '\t';
        }
        stream << '[' << what << ']' << '\t' << str.data();
        _lines.push_back(stream.str());
    }
    void Clear() { _lines.clear(); }

    Str::Type GetErrors()
    {
        std::wstringstream str;
        for (auto& err : _lines)
        {
            str << err << std::endl;
        }
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
        if (std::uncaught_exceptions() > 0)
        {
            std::wcerr << GetErrors();
        }
    }

    private:
    static ErrorAggregator*& GetPtr()
    {
        thread_local ErrorAggregator* ptr = nullptr;
        return ptr;
    }

    static ErrorAggregator& Get() { return *GetPtr(); }

    size_t                 _indent = 0;
    std::vector<Str::Type> _lines;
    friend struct ThreadActionContextImpl;
};

struct ThreadActionContextImpl
{
    ThreadActionContextImpl(Str::View const& what, std::function<Str::Type()>&& func) : _what(what), _func(std::move(func)) {}
    ~ThreadActionContextImpl()
    {
        if (std::uncaught_exceptions() > 0)
        {
            ErrorAggregator::Get().AddContextInfo(_indent, _what, _func());
        }
        ErrorAggregator::Get().Decr();
    }

    DELETE_MOVE_AND_COPY_ASSIGNMENT(ThreadActionContextImpl);

    size_t                     _indent = ErrorAggregator::Get().Incr();
    Str::View                  _what;
    std::function<Str::Type()> _func;
};

inline auto ThreadActionContext(Str::View const& what, std::function<Str::Type()>&& func)
{
    return ThreadActionContextImpl(what, std::move(func));
}

struct DebugContext
{
    Str::Type context{};
    Str::Type filename{};
    size_t    row{}, col{};

    Str::Type str() const
    {
        std::wstringstream strm;
        strm << context.c_str() << L" -- " << filename.c_str() << L" [" << row << L":" << col << L"]";
        return Str::Type(strm.str());
    }
};
#if 0
struct ParserContext
{
    typedef size_t (&RowGetter)(void* ptr);
    typedef size_t (&ColGetter)(void* ptr);

    static inline std::vector<ParserContext&> s_current;
    Str::Type                               _filename;
    RowGetter                                 _rowGetter;
    ColGetter                                 _colGetter;
    void*                                     _ptr;
    ParserContext(Str::Type const& filename, void* ptr, RowGetter rowGetter, ColGetter colGetter) :
        _filename(filename), _ptr(ptr), _rowGetter(rowGetter), _colGetter(colGetter)
    {
        s_current.push_back(this);
    }

    ~ParserContext() { s_current.pop_back(); }

    static DebugContext GetCurrentDebugContext(Str::Type const& context)
    {
        if (s_current.size() == 0) return DebugContext();
        ParserContext& parsercontext = s_current.back();
        return DebugContext{context,
                            parsercontext._filename,
                            parsercontext._rowGetter(parsercontext._ptr),
                            parsercontext._colGetter(parsercontext._ptr)};
    }
};

struct Exception : public std::exception
{

    template <typename... TArgs> Exception(const DebugContext& debugcontext, const char& fmt, TArgs&&... args)
    {
        snprintf(_msg, 511, fmt, std::forward<TArgs>(args)...);
        snprintf(_buffer,
                 1023,
                 "Error: %s %s [%zu:%zu]\n\t%s",
                 debugcontext.context.c_str(),
                 debugcontext.filename.c_str(),
                 debugcontext.row,
                 debugcontext.col,
                 _msg);
    }

    template <typename... TArgs>
    Exception(const char& fmt, TArgs... args) : Exception(ParserContext::GetCurrentDebugContext(L""), fmt, args...)
    {
    }

    virtual const char& what() const noexcept(true) override { return _buffer; }

    char         _context[512] = {};
    char         _msg[512]     = {};
    mutable char _buffer[1024] = {};
};
}    // namespace IDLDebug
#endif
}    // namespace IDLDebug

#define ACTION_CONTEXT_IMPL1(line, file, fnname, fn) auto actionctxvar_##line = IDLDebug::ThreadActionContextImpl(L##file fnname, (fn))
#define ACTION_CONTEXT_IMPL2(line, file, fnname, fn) ACTION_CONTEXT_IMPL1(line, file, fnname, fn)
#ifdef _WIN32
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, __FUNCTION__, fn)
#else
#define ACTION_CONTEXT(fn) ACTION_CONTEXT_IMPL2(__LINE__, __FILE__, "", fn)
#endif