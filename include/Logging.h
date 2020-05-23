#pragma once
#include <shared_string.h>

#include <cstring>
#include <iostream>
#include <array>

struct CorrelationVector
{
    static constexpr size_t StringSize = 129;
    struct
    {
        char Vector[StringSize];
    } _cv = {0};
    CorrelationVector() {}

    CorrelationVector(const CorrelationVector&) = delete;
    CorrelationVector& operator=(const CorrelationVector&) = delete;

    CorrelationVector(CorrelationVector&& cv) noexcept { *this = std::move(cv); }

    CorrelationVector& operator=(CorrelationVector&& cv) noexcept
    {
        std::swap(_cv, cv._cv);
        return *this;
    }

    static CorrelationVector Empty() { return CorrelationVector(); }

    static CorrelationVector Create() { throw std::logic_error("TODO"); }
    CorrelationVector&       Increment();
    CorrelationVector        Extend() const;

    operator shared_wstring() const { return shared_wstring(std::wstring(&_cv.Vector[0], &_cv.Vector[StringSize])); }

    operator shared_string() const { return shared_string(_cv.Vector); }

    operator const char*() const { return _cv.Vector; }

    const char* c_str() const { return _cv.Vector; }
};

namespace Logging
{
enum class Severity : uint16_t
{
    Fatal   = 0,      // Unrecoverable errors. FailFast
    Error   = 16,     // Recoverable errors but unexpected
    Warning = 64,     // Wierd but sometimes expected
    Info    = 128,    // May be helpful to debug further errors
    Verbose = 256,    // Success
    Trace   = 512
};

template <std::size_t N> struct memstreambuf : public std::streambuf
{
    std::array<char, N> buf;

    public:
    memstreambuf() { setp(buf.data(), buf.data() + buf.size()); }
    //    std::streambuf* setbuf(char_type* const s, std::streamsize const n) final { TODO(""); }
    //    pos_type seekpos(pos_type const pos, std::ios_base::openmode const which = std::ios_base::in | std::ios_base::out) final {
    //    TODO(""); } std::streamsize xsgetn(char_type* const s, std::streamsize const count) final { TODO(""); } std::streamsize
    //    xsputn(char_type const* s, std::streamsize const count) final { TODO(""); }
};

template <std::size_t N = 1024> class memstream : public memstreambuf<N>, public std::ostream
{
    public:
    memstream() : std::ostream(this) {}
    memstream(memstream const& stream) { this->buf = stream.buf; }
    memstream& operator=(memstream const& stream)
    {
        this->buf = stream.buf;
        return *this;
    }
};
using PrettyPrintStream = memstream<1024>;

template <typename T, typename = void> struct PrettyPrinter;

struct PrettyPrint
{
    template <typename T>
    static void PrintOnePropertyPair(PrettyPrintStream& buffer, const char* context, const char* name, const T& obj) noexcept(true)
    {
        PrettyPrinter<T>::Print(buffer, (name == nullptr ? context : name), obj);
    }

    template <typename T, typename... TArgs>
    static void
    PrintProperties(PrettyPrintStream& buffer, const char* context, const char* name, const T& obj, TArgs&&... args) noexcept(true)
    {
        PrintOnePropertyPair(buffer, context, name, obj);
        PrintProperties(buffer, context, std::forward<TArgs>(args)...);
    }

    template <typename T>
    static void PrintProperties(PrettyPrintStream& buffer, const char* context, const char* name, const T& obj) noexcept(true)
    {
        PrintOnePropertyPair(buffer, context, name, obj);
    }
};

typedef void TraceCallback(void* cbData, Severity level, const char* context, const char* message);

struct SingletonLogger
{
    void*          callbackData   = nullptr;
    TraceCallback* callback       = nullptr;
    Severity       verbosityLevel = Severity::Fatal;
};

static auto& GetLogger()
{
    static SingletonLogger logger;
    return logger;
}

template <typename TraceTraits, typename... TArgs> void Log(const CorrelationVector& /*cv*/, TArgs&&... args)
{
    auto& logger = GetLogger();

    if (TraceTraits::VerbosityLevel > logger.verbosityLevel || logger.callback == nullptr)
    {
        return;
    }

    Logging::PrettyPrintStream buffer;
    TraceTraits::ConstructMessage(buffer, std::forward<TArgs>(args)...);
    logger.callback(logger.callbackData, TraceTraits::VerbosityLevel, strrchr(typeid(TraceTraits).name(), ':') + 1, buffer.buf.data());
}

inline void AddTraceCallback(Severity level, void* cbData, TraceCallback* callback)
{
    auto& logger          = GetLogger();
    logger.verbosityLevel = level;
    logger.callback       = callback;
    logger.callbackData   = cbData;
}

template <typename TraceTraits> struct Exception : public std::exception
{
    template <typename... TArgs> Exception(const CorrelationVector& /*cv*/, TArgs&&... args) : std::exception()
    {
        auto name      = typeid(TraceTraits).name();
        auto classname = strchr(name, ':');
        buffer << (classname != nullptr ? classname + 1 : name) << " :: ";
        TraceTraits::ConstructMessage(buffer, std::forward<TArgs>(args)...);
        auto& logger = GetLogger();
        if (logger.callback)
        {
            logger.callback(logger.callbackData, Severity::Error, strrchr(typeid(TraceTraits).name(), ':') + 1, buffer.buf.data());
        }
    }

    Exception(Exception const& ex) { buffer = ex.buffer; }
    const char*       what() const noexcept(true) override { return buffer.buf.data(); }
    PrettyPrintStream buffer;
};

struct GenericException
{
    static constexpr Logging::Severity VerbosityLevel = Logging::Severity::Error;
    static void                        ConstructMessage(Logging::PrettyPrintStream& /*buffer*/) {}
};

struct TODOCreateExceptionTraits
{
    static constexpr Logging::Severity VerbosityLevel = Logging::Severity::Fatal;
    static void                        ConstructMessage(Logging::PrettyPrintStream& buffer, std::string_view const& msg)
    {
        // std::ostream stream(buffer.ptr(), buffer.remaining());
        buffer << "message = " << msg.data();
    }
};

struct TODOCreateException : public Exception<TODOCreateExceptionTraits>
{
    TODOCreateException(std::string_view const& msg) : Exception(CorrelationVector::Create(), msg) {}
};

template <> struct PrettyPrinter<std::string_view>
{
    static void Print(PrettyPrintStream& buffer, const char* context, std::string_view const& obj)
    {
        buffer << context << " = " << (obj.empty() ? "(empty)" : obj.data());
    }
};

template <> struct PrettyPrinter<std::string>
{
    static void Print(PrettyPrintStream& buffer, const char* context, std::string const& obj)
    {
        buffer << context << " = " << (obj.empty() ? "(empty)" : obj.data());
    }
};

template <> struct PrettyPrinter<shared_string>
{
    static void Print(PrettyPrintStream& buffer, const char* context, const shared_string& obj)
    {
        buffer << context << " = " << (obj.empty() ? "(empty)" : obj.data());
    }
};

template <> struct PrettyPrinter<shared_wstring>
{
    static void Print(PrettyPrintStream& buffer, const char* context, const shared_wstring& obj)
    {
        throw std::logic_error("Not Implemented");
        // buffer << context << " = " << (obj.empty() ? L"(empty)" : obj.data());
    }
};

template <> struct PrettyPrinter<int>
{
    static void Print(PrettyPrintStream& buffer, const char* context, int obj) { buffer << context << " = " << obj; }
};

template <> struct PrettyPrinter<size_t>
{
    static void Print(PrettyPrintStream& buffer, const char* context, size_t obj) { buffer << context << " = " << obj; }
};

template <> struct PrettyPrinter<CorrelationVector>
{
    static void Print(PrettyPrintStream& buffer, const char* /*context*/, const CorrelationVector& cv) { buffer << "cv = " << cv.c_str(); }
};
}    // namespace Logging
