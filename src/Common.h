#pragma once
#include "Value.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include <exception>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define NOCOPY(Type)            \
    Type(const Type&) = delete; \
    Type& operator=(const Type&) = delete;

template <typename> struct is_tuple : std::false_type
{
};

template <typename... T> struct is_tuple<std::tuple<T...>> : std::true_type
{
};

struct NotImplException : public std::exception
{
    const char* what() const noexcept(true) override { return "Not implemented: " __FILE__ " ::  __FUNCTION__"; }
};
#if !defined TODO
#define TODO(...) throw NotImplException{};
#endif

#define TODOFUNC \
    todo();      \
    return nullptr;

static inline void todo()
{
    throw std::logic_error("Not implemented");
}

template <typename TChar, size_t N> struct static_stringT : std::array<TChar, N>
{
    constexpr static_stringT(const TChar (&str)[N]) : std::array<TChar, N>(str) {}

    template <size_t N2> constexpr static_stringT<TChar, N + N2> operator+(const static_stringT<TChar, N2>& str2);
};

template <size_t N> using static_string = static_stringT<char, N>;

template <size_t N> using static_wstring = static_stringT<wchar_t, N>;

template <size_t N> auto create_sstr(const char (&str)[N])
{
    return static_string<N>(str);
}

template <size_t N> auto create_wstr(const wchar_t (&str)[N])
{
    return static_wstring<N>(str);
}

struct shared_lock
{
    private:
    shared_lock(std::shared_mutex* mutex) : m_mutex(mutex){};

    public:
    shared_lock(std::shared_mutex& mutex) : m_mutex(&mutex) { lockacquire(); }
    ~shared_lock() { lockrelease(); }

    void lockacquire()
    {
        if (m_mutex) m_rlock = std::shared_lock(*m_mutex);
    }
    void lockrelease()
    {
        if (m_mutex) m_rlock.unlock();
    }

    std::shared_mutex* m_mutex{nullptr};

    friend struct exclusive_lock;
    std::shared_lock<std::shared_mutex> m_rlock;

    friend struct exclusive_lock;
};

struct exclusive_lock
{
    exclusive_lock(std::shared_mutex& mutex) : m_mutex(&mutex) { lockacquire(); }
    ~exclusive_lock() { lockrelease(); }

    exclusive_lock(const exclusive_lock& lock) = delete;
   // exclusive_lock(exclusive_lock&& lock)      = default;
   // exclusive_lock& operator=(exclusive_lock&& lock) = default;
    exclusive_lock& operator=(const exclusive_lock& lock) = delete;

    void lockacquire() { m_wlock = std::unique_lock<std::shared_mutex>(*m_mutex); }
    void lockrelease() { m_wlock = std::unique_lock<std::shared_mutex>{}; }

    shared_lock const& shared() const { return m_slock; }

    std::shared_mutex* m_mutex{nullptr};

    shared_lock                         m_slock{nullptr};
    std::unique_lock<std::shared_mutex> m_wlock{};
};

template <typename T> struct shared_bufferT : private std::shared_ptr<std::vector<T>>
{
};

typedef shared_bufferT<std::uint8_t> shared_buffer;

using timestamp = decltype(std::chrono::system_clock::now());

struct UuidStr
{
    static constexpr size_t Size           = 38;
    char                    str[Size + 1]  = {};
    wchar_t                 wstr[Size + 1] = {};
    constexpr UuidStr() {}
    constexpr UuidStr(const char strin[Size]) : str()
    {
        for (int i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }
    constexpr UuidStr(const wchar_t strin[Size])
    {
        for (int i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }

    constexpr UuidStr(const std::string_view strin)
    {
        for (int i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }
    constexpr UuidStr(const std::wstring_view strin)
    {
        for (int i = 0; i < Size; i++)
        {
            str[i]  = (char)strin[i];
            wstr[i] = (wchar_t)strin[i];
        }
    }

    operator const wchar_t*() const { return wstr; }
    operator const char*() const { return str; }
    operator wchar_t*() { return wstr; }
    operator char*() { return str; }
};

struct Uuid
{
    constexpr Uuid(UuidStr const& str);
    constexpr Uuid() noexcept {}
    constexpr Uuid(Uuid const& obj) noexcept { data = obj.data; }
    constexpr Uuid(Uuid&& obj) noexcept { data = std::move(obj.data); }

    Uuid& operator=(Uuid const& uuid)
    {
        data = uuid.data;
        return *this;
    }
#define COMPARISONOP(op) \
    inline bool operator op(Uuid const& r) const { return data op r.data; }
    COMPARISONOP(==)
    COMPARISONOP(!=)
    COMPARISONOP(<=)
    COMPARISONOP(>=)
    COMPARISONOP(<)
    COMPARISONOP(>)

    UuidStr constexpr ToString() const;
    static Uuid constexpr FromString(UuidStr const& str) { return Uuid(str); }
    static Uuid Create();

    std::array<std::uint8_t, 16> data{};
};

template <typename T> struct UuidObjectT;

template <typename T> struct UuidBasedId
{
    static UuidBasedId<T>           Create() { return UuidBasedId<T>(Uuid::Create()); }
    static constexpr UuidBasedId<T> FromString(UuidStr str) { return UuidBasedId<T>(Uuid::FromString(str)); }

    inline bool operator==(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) == (const Uuid&)r; }
    inline bool operator!=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) != (const Uuid&)r; }
    inline bool operator<=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) <= (const Uuid&)r; }
    inline bool operator>=(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) >= (const Uuid&)r; }
    inline bool operator<(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) < (const Uuid&)r; }
    inline bool operator>(const UuidBasedId<T>& r) const { return (const Uuid&)(*this) > (const Uuid&)r; }

    bool Empty() const { return _guid == Uuid(); }
    bool Valid() const { return !Empty(); }
    void Validate()
    {
        if (!Valid())
        {
            _guid = Uuid::Create();
        }
    }
    operator UuidStr() { return _guid.ToString(); }

    UuidStr ToString() const { return _guid.ToString(); }
    UuidBasedId(const UuidBasedId<T>& obj) : _guid(obj._guid) {}
    UuidBasedId<T>& operator=(const UuidBasedId<T>& obj)
    {
        _guid = obj._guid;
        return *this;
    }
    constexpr UuidBasedId(UuidStr str) : _guid(str) {}

    private:
    UuidBasedId(Uuid guid) : _guid(guid) {}
    constexpr UuidBasedId() {}
    Uuid _guid;
         operator const Uuid&() const { return _guid; }

    public:
    static constexpr UuidBasedId<T> Invalid() { return UuidBasedId<T>(); }
    friend UuidObjectT<T>;
};

template <typename T> struct UuidObjectT
{
    using Id = UuidBasedId<T>;
    UuidObjectT() {}
    Id id = Id::Create();
};

template <typename T> struct TimestampedT
{
    using time_point = decltype(std::chrono::system_clock::now());
    struct InitArgs
    {
    };

    TimestampedT<T>()
    {
        creation     = std::chrono::system_clock::now();
        lastmodified = creation;
    }

    time_point creation;
    time_point lastmodified;
};

struct TDefaultForProcess
{
};

// template <typename TMutex = TDefaultForProcess>
struct SingletonManager
{
    struct Mutex
    {
        struct Scope
        {
            Scope(Mutex* obj) : _obj(obj) { obj->_mutex.lock(); }
            ~Scope() { _obj->_mutex.unlock(); }

            Mutex* _obj;
        };

        auto       LockShared() { return Scope(this); }
        auto       LockExclusive() { return Scope(this); }
        std::mutex _mutex;
    };

    static inline Mutex _mutex;

    template <typename TCreator, typename TReturnType> struct Singleton
    {
        static inline TReturnType _value = nullptr;
        TReturnType               Get()
        {
            bool _create = false;
            {
                auto scope = _mutex.LockShared();
                if (_value != nullptr)
                {
                    return (_value);
                }
            }

            auto scope = _mutex.LockExclusive();
            if (_value == nullptr)
            {
                _value = TCreator::Create();
            }

            return _value;
        }
    };

    template <typename TCreator> static decltype(TCreator::Create()) Get()
    {
        return Singleton<TCreator, decltype(TCreator::Create())>().Get();
    }
};

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

    static CorrelationVector Create();
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

template <typename T> struct PrettyPrinter<UuidBasedId<T>>
{
    static void Print(PrettyPrintStream& buffer, const char* context, const UuidBasedId<T>& obj)
    {
        buffer << context << " = " << (const char*)obj.ToString();
    }
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
        buffer << context << " = " << (obj.empty() ? L"(empty)" : obj.data());
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

namespace Common
{
using Uuid = ::Uuid;
struct PointF
{
    float x, y;
};
struct Point
{
    size_t x, y;
};
struct Size
{
    size_t width, height;
};
struct SizeF
{
    float width, height;
};

typedef const std::string_view&  cstrref;
typedef const std::wstring_view& cwcsref;

// template <typename EventTraits> struct EventSource;
template <typename EventTraits> struct IEventHandler;

template <typename EventTraits> struct TEvent
{
    using Traits     = EventTraits;
    using Handler    = IEventHandler<EventTraits>;
    using HandlerPtr = IEventHandler<EventTraits>*;
    using Args       = typename EventTraits::Args;
    using Sender     = typename EventTraits::Sender;
    struct HandlerData
    {
        Sender*     sender;
        const Args& args;
    };
};

struct EventRegistrationTracker
{
    bool* m_cookie = nullptr;
    EventRegistrationTracker() {}
    EventRegistrationTracker(const EventRegistrationTracker&) = delete;
    EventRegistrationTracker(EventRegistrationTracker&&)      = delete;
    EventRegistrationTracker& operator=(EventRegistrationTracker&&) = delete;
    EventRegistrationTracker& operator=(const EventRegistrationTracker&) = delete;

    void Clear()
    {
        *m_cookie = false;
        m_cookie  = nullptr;
    }

    bool Valid() { return m_cookie != nullptr && *m_cookie; }

    ~EventRegistrationTracker()
    {
        if (m_cookie != nullptr)
        {
            Clear();
        }
    }
};

template <typename EventTraits> struct IEventHandler;

template <typename... EventTraits> class EventSource
{
    public:
    EventSource()          = default;
    virtual ~EventSource() = default;

    template <typename TEventTraits> void Add(IEventHandler<TEventTraits>* handler, EventRegistrationTracker& tracker)
    {
        auto& handlers = _gethandlers<TEventTraits>();
        handlers.push_back(std::make_unique<HandlerData<TEventTraits>>(handler, &tracker));
    }

    void Remove(EventRegistrationTracker& token) { token.Clear(); }

    template <typename TTrait> struct HandlerData
    {
        using HandlerPtr = typename TEvent<TTrait>::HandlerPtr;
        HandlerData(HandlerPtr ptrIn, EventRegistrationTracker* trackerIn) : ptr(ptrIn), tracker(trackerIn) { tracker->m_cookie = &valid; }

        HandlerPtr                ptr   = nullptr;
        bool                      valid = true;
        EventRegistrationTracker* tracker;

        virtual ~HandlerData()
        {
            ptr = nullptr;
            if (valid && tracker->Valid())
            {
                tracker->Clear();
            }
        }
    };

    template <typename TEventTrait> using HandlerList = std::vector<std::unique_ptr<HandlerData<TEventTrait>>>;

    std::tuple<HandlerList<EventTraits>...> _handlers;

    private:
    template <typename TEventTraits> auto& _gethandlers() { return std::get<HandlerList<TEventTraits>>(_handlers); }

    protected:
    template <typename TEvent, typename TSender> void InvokeAll(TSender* sender, typename TEvent::Args const& data)
    {
        using InvokeEventTraits = typename TEvent::Traits;
        auto& handlers          = _gethandlers<InvokeEventTraits>();
        for (auto& handler : handlers)
        {
            typename TEvent::HandlerData handlerdata{sender, data};
            if (handler->ptr != nullptr && handler->valid && handler->tracker->Valid()) handler->ptr->HandleEvent(handlerdata);
        }
    }
};

template <typename EventTraits> struct IEventHandler
{
    using Sender      = typename EventTraits::Sender;
    using HandlerData = typename TEvent<EventTraits>::HandlerData;

    void Initialize(typename EventTraits::Sender* ptr)
    {
        Clear();
        _ptr = ptr;
        _ptr->Add(this, _event_token);
    }

    void Clear()
    {
        if (_ptr && _event_token.Valid())
        {
            _ptr->Remove(_event_token);
            _ptr = nullptr;
        }
    }
    IEventHandler() {}

    IEventHandler(Sender* ptr) { Initialize(ptr); }

    ~IEventHandler() { Clear(); }

    virtual void HandleEvent(HandlerData& data) = 0;

    private:
    EventRegistrationTracker      _event_token;
    typename EventTraits::Sender* _ptr = nullptr;
};

struct Exception : public std::exception
{
    Exception(long /*code*/, const wchar_t* /*fmt*/, ...)
    {
        //        va_list ap;
        //        va_start(ap, fmt);
        //        vswprintf_s(_buffer, fmt, ap);
    }

    const wchar_t* message() const { return _buffer; }

    private:
    wchar_t _buffer[256]{};
};

struct ISuspendable
{
    struct SuspendScope
    {
        SuspendScope(ISuspendable* obj) : _obj(obj) { obj->SuspendImpl(); }
        ~SuspendScope() { _obj->ResumeImpl(); }
        SuspendScope(const SuspendScope&) = delete;
        SuspendScope& operator=(const SuspendScope&) = delete;
        SuspendScope(SuspendScope&& obj) noexcept { *this = std::move(obj); }
        SuspendScope& operator=(SuspendScope&& obj) noexcept
        {
            std::swap(_obj, obj._obj);
            return *this;
        }

        ISuspendable* _obj;
    };

    virtual bool IsSuspended() = 0;
    virtual void SuspendImpl() = 0;
    virtual void ResumeImpl()  = 0;

    SuspendScope Suspend() { return SuspendScope(this); }
};

struct IAsyncInitialize
{
    public:
    IAsyncInitialize()                        = default;
    IAsyncInitialize(const IAsyncInitialize&) = delete;
    IAsyncInitialize& operator=(const IAsyncInitialize&) = delete;

    virtual void InitializeAsyncImpl() = 0;

    void BlockForInitializeComplete()
    {
        BeginInitialize();
        _future.wait();
    }

    std::shared_future<void> InitializeAsync()
    {
        BeginInitialize();
        return _future;
    }

    void BeginInitialize()
    {
        std::lock_guard<std::mutex> guard_scope(_mutex);
        if (!_future.valid())
        {
            _future = std::async([this] { this->InitializeAsyncImpl(); }).share();
        }
    }

    private:
    std::mutex               _mutex;
    std::shared_future<void> _future;
};

struct IAsyncWorker
{
    void Schedule(int /*delay*/)
    {
        if (_suspendable != nullptr && _suspendable->IsSuspended())
        {
            return;
        }
        std::unique_lock<std::mutex> scope(_lock);
        _lastRequestTick = std::chrono::steady_clock::now();
        if (!_thread.joinable())
        {
            _thread = std::thread(s_Worker, this);
        }
        // return scope;
    }

    void Suspend()
    {
        while (true)
        {
            {
                std::lock_guard<std::mutex> scope(_lock);
                if (_lastRequestTick == std::chrono::steady_clock::time_point())
                {
                    return;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    virtual void DoWork() = 0;

    virtual ~IAsyncWorker()
    {
        std::lock_guard<std::mutex> scope(_lock);
        if (_thread.joinable())
        {
            _thread.join();
        }
    }
    IAsyncWorker(ISuspendable* obj = nullptr) : _suspendable(obj){};

    IAsyncWorker(const IAsyncWorker&) = delete;
    IAsyncWorker& operator=(const IAsyncWorker&) = delete;

    IAsyncWorker(IAsyncWorker&& obj) noexcept { *this = std::move(obj); }
    IAsyncWorker& operator=(IAsyncWorker&& obj) noexcept
    {
        std::lock_guard<std::mutex> scope1(obj._lock);
        std::lock_guard<std::mutex> scope2(_lock);
        std::swap(_lastRequestTick, obj._lastRequestTick);
        std::swap(_delay, obj._delay);
        // std::swap(_lock, obj._lock);
        std::swap(_thread, obj._thread);
        return *this;
    };

    private:
    static void s_Worker(IAsyncWorker* that) { that->_Worker(); }

    void _Worker()
    {
        std::chrono::steady_clock::time_point lastTick = std::chrono::steady_clock::time_point();
        do
        {
            {
                std::lock_guard<std::mutex> scope(_lock);
                if (lastTick == _lastRequestTick)
                {
                    _lastRequestTick = std::chrono::steady_clock::time_point();
                    _thread.detach();
                    return;
                }
                else
                {
                    lastTick = _lastRequestTick;
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(_delay));
            {
                //   std::lock_guard<std::mutex> scope(_lock);
                DoWork();
            }
        } while (true);
    }

    std::chrono::steady_clock::time_point _lastRequestTick = std::chrono::steady_clock::time_point();
    int                                   _delay           = 0;
    std::mutex                            _lock;
    std::thread                           _thread;

    ISuspendable* _suspendable;
};

template <typename T> struct IVector
{
    T&     GetAt(size_t index) const;
    size_t GetCount() const;
};

template <typename TOwner, typename TObject, typename TIndexerFunc> struct ConstIterator
{
    size_t        _count       = 0;
    size_t        _index       = 0;
    const TOwner* _ptr         = nullptr;
    TIndexerFunc  _indexerfunc = nullptr;

    ConstIterator() {}
    ConstIterator(const TOwner* ptr, size_t count, TIndexerFunc indexerfunc) : _ptr(ptr), _count(count), _indexerfunc(indexerfunc) {}

    void _checkend()
    {
        if (_index >= _count)
        {
            _ptr         = nullptr;
            _count       = 0;
            _index       = 0;
            _indexerfunc = nullptr;
        }
    }
    ConstIterator& operator++(int n)
    {
        _index += n;
        _checkend();
        return *this;
    }
    ConstIterator& operator++()
    {
        _index += 1;
        _checkend();
        return *this;
    }

    const TObject& operator*() { return (_ptr->*_indexerfunc)(_index); }
    bool           operator==(const ConstIterator& rhs) const { return _ptr == rhs._ptr && _index == rhs._index; }
    bool           operator!=(const ConstIterator& rhs) const { return !(*this == rhs); }
};

template <typename TOwner, typename TObject, typename TIndexerFunc> struct Iterator
{
    size_t        _count       = 0;
    size_t        _index       = 0;
    TOwner*       _ptr         = nullptr;
    TIndexerFunc* _indexerfunc = nullptr;

    Iterator() {}
    Iterator(TOwner* ptr, size_t count, TIndexerFunc indexerfunc) : _ptr(ptr), _count(count), _indexerfunc(indexerfunc) {}

    Iterator& operator++(int n)
    {
        _index += n;
        this->_checkend();
        return *this;
    }
    Iterator& operator++()
    {
        _index += 1;
        this->_checkend();
        return *this;
    }

    TObject& operator*() { return (_ptr->*_indexerfunc)(_index); }
    bool     operator==(const Iterator& rhs) const { return _ptr == rhs._ptr && _index == rhs._index; }
    bool     operator!=(const Iterator& rhs) const { return !(*this == rhs); }
};

template <typename TBeginIterator, typename TEndIterator> struct Range
{
    Range(TBeginIterator& begin, TEndIterator& end) : _begin(begin), _end(end) {}
    TBeginIterator  _begin;
    TEndIterator    _end;
    TBeginIterator& begin() { return _begin; }
    TEndIterator&   end() { return _end; }
};

template <typename TObj, typename TOwner, typename TSizeFunc, typename TIndexerFunc>
auto GetRange(TOwner* owner, TSizeFunc sizefunc, TIndexerFunc indexerfunc)
{
    size_t                               size = (owner->*sizefunc)();
    Iterator<TOwner, TObj, TIndexerFunc> begin(owner, size, 0), end;
    return Range<decltype(begin), decltype(end)>{begin, end};
};

template <typename TObj, typename TOwner, typename TSizeFunc, typename TIndexerFunc>
auto GetRange(const TOwner* owner, TSizeFunc sizefunc, TIndexerFunc indexerfunc)
{
    size_t                                    size = (owner->*sizefunc)();
    ConstIterator<TOwner, TObj, TIndexerFunc> begin(owner, size, indexerfunc);
    ConstIterator<TOwner, TObj, TIndexerFunc> end;
    return Range<decltype(begin), decltype(end)>{begin, end};
}

template <typename T> struct CyclicBuffer
{
    CyclicBuffer(size_t n) : _N(n) {}

    bool push_back(T&& obj)
    {
        if (_count == _N)
        {
            std::swap(obj, _buffer[_head]);
            _head = (_head + 1) % _N;
            return true;
        }
        _count++;
        _buffer.push_back(std::move(obj));
        return false;
    }

    T& operator[](size_t n)
    {
        if (n > _count)
        {
            throw ::Logging::Exception<::Logging::GenericException>(CorrelationVector::Empty());
            //_buffer.resize(n + 1);
            //_tail = n + 1;
        }
        return _buffer[(_head + n) % _N];
    }

    T pop()
    {
        if (_count == 0) throw ::Logging::Exception<::Logging::GenericException>(CorrelationVector::Empty());
        _count--;
        auto oldhead = _head;
        _head        = (_head + 1) % _N;
        return std::move(_buffer[oldhead]);
    }

    size_t count() const { return _count; }
    size_t capacity() const { return _N; }
    bool   Full() const { return _count == _N; }

    void buffersize(size_t n)
    {
        _N = n;
        _buffer.clear();
        _head  = 0;
        _count = 0;
    }
    void clear()
    {
        _buffer.clear();
        _head = _count = 0;
    }

    private:
    size_t         _head  = 0;
    size_t         _count = 0;
    size_t         _N     = 0;
    std::vector<T> _buffer;
};

}    // namespace Common

inline bool stricompare(const char* str1, const char* str2)
{
    auto len1 = strlen(str1);
    auto len2 = strlen(str2);
    return std::equal(str1, str1 + len1, str2, str2 + len2, [](char a, char b) { return tolower(a) == tolower(b); });
}
template <typename T1, typename T2> inline bool iequal(T1 const& a, T2 const& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](auto a, auto b) { return tolower(a) == tolower(b); });
}

inline bool stricompare(std::string_view const& str1, std::string_view const& str2)
{
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

inline bool operator==(const ::Common::Size& l, const ::Common::Size& r)
{
    return l.width == r.width && l.height == r.height;
}
inline bool operator!=(const ::Common::Size& l, const ::Common::Size& r)
{
    return !(l == r);
}

#include <time.h>
namespace TimeHelpers
{

using time_point   = std::chrono::time_point<std::chrono::system_clock>;
using system_clock = std::chrono::system_clock;

struct time_point_str
{
    time_point_str(const time_point& t) : _time(t)
    {
        auto tt = std::chrono::system_clock::to_time_t(_time);
#ifdef _WIN32
        localtime_s(&_tmobj, &tt);
#else
        localtime_r(&tt, &_tmobj);
#endif
    }

    operator const wchar_t*() { return wcs().data(); }

    operator const char*() { return str().data(); }

    operator std::string_view() { return str(); }

    operator std::wstring_view() { return wcsv(); }

    const std::wstring_view wcsv()
    {
        auto len = wcsftime(_wcsbuffer, std::size(_wcsbuffer), L"%T", &_tmobj);
        return std::wstring_view(_wcsbuffer, len);
    }

    std::wstring wcs()
    {
        auto len = wcsftime(_wcsbuffer, std::size(_wcsbuffer), L"%T", &_tmobj);
        return std::wstring(_wcsbuffer, len);
    }

    const std::string_view str()
    {
        auto len = strftime(_strbuffer, std::size(_strbuffer), "%T", &_tmobj);
        return std::string_view(_strbuffer, len);
    }

    wchar_t    _wcsbuffer[16]{};
    char       _strbuffer[16]{};
    std::tm    _tmobj{};
    time_point _time{};
};
}    // namespace TimeHelpers

#if false 
defined WIN32
#define WIN32_LEAN_AND_MEAN
#include <combaseapi.h>
#include <guiddef.h>
#include <winnt.h>

#if !defined RTL_CORRELATION_VECTOR_STRING_LENGTH
#define RTL_CORRELATION_VECTOR_STRING_LENGTH 129
#define RTL_CORRELATION_VECTOR_VERSION_1 ((CHAR)1)
#define RTL_CORRELATION_VECTOR_VERSION_2 ((CHAR)2)
#define RTL_CORRELATION_VECTOR_VERSION_CURRENT RTL_CORRELATION_VECTOR_VERSION_2

#define RTL_CORRELATION_VECTOR_V1_PREFIX_LENGTH (16)
#define RTL_CORRELATION_VECTOR_V1_LENGTH (64)

#define RTL_CORRELATION_VECTOR_V2_PREFIX_LENGTH (22)
#define RTL_CORRELATION_VECTOR_V2_LENGTH (128)

typedef struct CORRELATION_VECTOR
{
    CHAR Version;
    CHAR Vector[RTL_CORRELATION_VECTOR_STRING_LENGTH];
} CORRELATION_VECTOR;

typedef CORRELATION_VECTOR* PCORRELATION_VECTOR;

#define TraceLoggingCORRELATION_VECTOR(cv) TraceLoggingString((cv).Vector, "__TlgCV__")

NTSYSAPI
DWORD
NTAPI
RtlInitializeCorrelationVector(_Out_ PCORRELATION_VECTOR CorrelationVector, _In_ int Version, _In_opt_ const GUID* Guid);

NTSYSAPI
DWORD
NTAPI
RtlIncrementCorrelationVector(_Inout_ PCORRELATION_VECTOR CorrelationVector);

NTSYSAPI
DWORD
NTAPI
RtlExtendCorrelationVector(_Inout_ PCORRELATION_VECTOR CorrelationVector);

NTSYSAPI
DWORD
NTAPI
RtlValidateCorrelationVector(_In_ PCORRELATION_VECTOR Vector);

#endif

extern "C" __declspec(dllimport) long __stdcall UuidCreate(GUID* Uuid);

inline CorrelationVector CorrelationVector::Create()
{
    CorrelationVector cv;
    GUID              guid;
    if (UuidCreate(&guid) != 0) throw std::exception("Unable to generate GUID");
    CORRELATION_VECTOR wincv;
    RtlInitializeCorrelationVector(&wincv, RTL_CORRELATION_VECTOR_VERSION_CURRENT, &guid);
    static_assert(sizeof(cv._cv) == sizeof(wincv.Vector));
    memcpy(cv._cv.Vector, wincv.Vector, sizeof(wincv.Vector));
    return cv;
}

inline CorrelationVector& CorrelationVector::Increment()
{
    return *this;
}
inline CorrelationVector CorrelationVector::Extend() const
{
    return CorrelationVector::Create();
}
#else

inline CorrelationVector CorrelationVector::Create()
{
    return {};
}

inline CorrelationVector& CorrelationVector::Increment()
{
    return *this;
}
inline CorrelationVector CorrelationVector::Extend() const
{
    return CorrelationVector::Create();
}
#endif

#include <random>
inline Uuid Uuid::Create()
{
    Uuid                                    uuid;
    std::random_device                      r;
    std::default_random_engine              e1(r());
    std::uniform_int_distribution<uint16_t> dist(0, 255);

    for (size_t i = 0; i < uuid.data.size(); i++)
    {
        uuid.data[i] = (uint8_t)dist(e1);
    }

    return uuid;
}

inline UuidStr constexpr UuidToString(Uuid const& uuid)
{
    struct tohex
    {
        static constexpr char convert(uint8_t c) { return c < 10 ? ('0' + c) : ('a' + (c - 10)); }
        static constexpr char l(uint8_t c) { return convert(c & 0xf); }
        static constexpr char h(uint8_t c) { return convert(c >> 4); }
    };

    char str[] = {'{',
                  tohex::h(uuid.data[0]),
                  tohex::l(uuid.data[0]),
                  tohex::h(uuid.data[1]),
                  tohex::l(uuid.data[1]),
                  tohex::h(uuid.data[2]),
                  tohex::l(uuid.data[2]),
                  tohex::h(uuid.data[3]),
                  tohex::l(uuid.data[3]),
                  '-',
                  tohex::h(uuid.data[4]),
                  tohex::l(uuid.data[4]),
                  tohex::h(uuid.data[5]),
                  tohex::l(uuid.data[5]),
                  '-',
                  tohex::h(uuid.data[6]),
                  tohex::l(uuid.data[6]),
                  tohex::h(uuid.data[7]),
                  tohex::l(uuid.data[7]),
                  '-',
                  tohex::h(uuid.data[8]),
                  tohex::l(uuid.data[8]),
                  tohex::h(uuid.data[9]),
                  tohex::l(uuid.data[9]),
                  '-',
                  tohex::h(uuid.data[10]),
                  tohex::l(uuid.data[10]),
                  tohex::h(uuid.data[11]),
                  tohex::l(uuid.data[11]),
                  tohex::h(uuid.data[12]),
                  tohex::l(uuid.data[12]),
                  tohex::h(uuid.data[13]),
                  tohex::l(uuid.data[13]),
                  tohex::h(uuid.data[14]),
                  tohex::l(uuid.data[14]),
                  tohex::h(uuid.data[15]),
                  tohex::l(uuid.data[15]),
                  '}',
                  0};

    return UuidStr(str);
}

inline UuidStr constexpr Uuid::ToString() const
{
    return UuidToString(*this);
}

inline constexpr Uuid::Uuid(UuidStr const& str)
{
    struct tobyte
    {
        static constexpr uint8_t convert(char c) { return (c < 'a' ? (c - '0') : (c - 'a' + 10)) & 0xf; }
        static constexpr uint8_t join(char h, char l) { return convert(h) << 4 | convert(l); }
    };

    data[0x0] = tobyte::join(str.str[1], str.str[2]);
    data[0x1] = tobyte::join(str.str[3], str.str[4]);
    data[0x2] = tobyte::join(str.str[5], str.str[6]);
    data[0x3] = tobyte::join(str.str[7], str.str[8]);

    data[0x4] = tobyte::join(str.str[10], str.str[11]);
    data[0x5] = tobyte::join(str.str[12], str.str[13]);

    data[0x6] = tobyte::join(str.str[15], str.str[16]);
    data[0x7] = tobyte::join(str.str[17], str.str[18]);

    data[0x8] = tobyte::join(str.str[20], str.str[21]);
    data[0x9] = tobyte::join(str.str[22], str.str[23]);

    data[0xa] = tobyte::join(str.str[25], str.str[26]);
    data[0xb] = tobyte::join(str.str[27], str.str[28]);
    data[0xc] = tobyte::join(str.str[29], str.str[30]);
    data[0xd] = tobyte::join(str.str[31], str.str[32]);
    data[0xe] = tobyte::join(str.str[33], str.str[34]);
    data[0xf] = tobyte::join(str.str[35], str.str[36]);
}

template <typename TOwner, typename... Ts> struct IterableContainer
{
    template <typename T> struct Iterator
    {
        TOwner* owner = nullptr;
        size_t  index = std::numeric_limits<size_t>::max();
        Iterator()    = default;
        Iterator(TOwner* ownerin) : owner(ownerin), index(0)
        {
            if (!owner->template ValidAt<T>(0))
            {
                *this = {};
            }
        }

        bool operator==(Iterator<T> const& it) const { return index == it.index; }
        bool operator!=(Iterator<T> const& it) const { return index != it.index; }

        Iterator<T>& operator++()
        {
            index++;
            if (!owner->template ValidAt<T>(index))
            {
                *this = {};
            }
            return *this;
        }
        T& operator*() { return owner->template GetAt<T>(index); }
    };

    template <typename T> struct Range
    {
        auto begin() { return _begin; }
        auto end() { return _end; }

        Range(TOwner* owner) : _begin(owner) {}

        Iterator<T> _begin{};
        Iterator<T> _end{};
    };

    private:
};
