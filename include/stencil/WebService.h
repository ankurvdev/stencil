#include "jsonserdes.h"

#pragma warning(push, 3)
#pragma warning(disable : 4365)    // conversion from 'const char' to 'unsigned char', signed/unsigned mismatch
#pragma warning(disable : 4355)    //'this' : used in base member initializer list
#pragma warning(disable : 4855)    // implicit capture of 'this' via '[=]' is deprecated
#pragma warning(disable : 4548)    // expression before comma has no effect; expected expression with side - effect
#pragma warning(disable : 4702)    // Unreachable code
#pragma warning(disable : 4668)    // not defined as a preprocessor macro
#pragma warning(disable : 5039)    // pointer or reference to potentially throwing function passed to 'extern)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostics  push
#pragma GCC diagnostic   ignored "-Wmaybe-uninitialized"
#include <fmt/format.h>

#include <limits.h>

#include <httplib.h>

#pragma GCC diagnostic   pop
#pragma clang diagnostic pop
#pragma warning(pop)

#include <algorithm>
#include <string_view>

#define STENCIL_USING_WEBSERVICE 1
namespace Stencil
{

template <typename TInterface> struct WebServiceHandlerTraits
{
    static constexpr const std::string_view Url()
    {
        static_assert(std::is_same<TInterface, TInterface>::value, "Please specialize WebServiceHandlerTraits and provide Url");
        return std::string_view("", 0);
    }

    static void _HandleRequest(httplib::Request const& request, httplib::Response& res, std::string_view const& path);

    // static void HandleRequest(httplib::Response& res) {}
};

namespace impl
{
template <typename T1, typename T2> inline bool iequal(T1 const& a, T2 const& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](auto a1, auto b1) { return tolower(a1) == tolower(b1); });
}
}    // namespace impl

template <typename T>
concept WebInterfaceImpl = std::is_default_constructible_v<WebServiceHandlerTraits<T>>;

template <WebInterfaceImpl... TImpls> struct WebService
{
    struct UnableToStartDaemonException
    {};
    struct InvalidUrlException
    {};
    struct EmptyUrlException
    {};
    struct HandlerNotFound
    {};

    WebService() = default;
    DELETE_COPY_AND_MOVE(WebService);

    void StartOnPort(uint16_t port)
    {
        auto guardscope = std::unique_lock<std::mutex>();
        _server.Get("/.*", [this](auto... args) { return this->_HandleRequest(args...); });
        _server.set_error_handler([this](auto... args) { return this->_HandleRequest(args...); });
        _port         = port;
        _listenthread = std::thread([this]() { _server.listen("localhost", _port); });

        for (int i = 0; i < 100 && !_server.is_running(); i++) std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!_server.is_running()) throw std::runtime_error("Unable to start server");
    }

    void StopDaemon()
    {
        {
            auto guardscope = std::unique_lock<std::mutex>();
            _server.stop();
        }
        _cond.notify_all();
    }

    void WaitForStop() { _listenthread.join(); }

    private:
    void _HandleRequest(const httplib::Request& req, httplib::Response& res)
    {
        try
        {
            std::string_view const& path = req.path;
            if (path.empty() || path[0] != '/') throw std::logic_error("Invalid path");
            auto index = path.find('/', 1);
            if (index == path.npos) throw std::runtime_error("Invalid path. Cannot detect interface");
            auto ifname = path.substr(1, index - 1);
            _HandleRequest<TImpls...>(req, res, ifname, path.substr(index + 1));
            res.status = 200;
        } catch (std::exception const& ex)
        {
            res.status = 500;
            res.body   = ex.what();
        }
    }

    void _HandleError(const httplib::Request& /* req */, httplib::Response& res)
    {
        res.set_content(fmt::format("<p>Error Status: <span style='color:red;'>{}</span></p>", res.status), "text/html");
    }

    template <WebInterfaceImpl T>
    bool _HandleEventSource(T& /* obj */,
                            httplib::Request const& /* req */,
                            httplib::Response& /* res */,
                            std::string_view const& ifname,
                            std::string_view const& /* path */)
    {
        if (!impl::iequal("eventsource", ifname)) { return false; }

        /* Event Source
         *      Condition: path begins with /eventsource.
         *      Action: Add the req to the SSR messaging queue
         *      Explanation:
         *          WebService(this) must register itself (via Traits) as the notification client for all events in the interface
         *          Once RaiseEvent_event(int arg1, string arg2 ) is called, the event handlers are invoked
         *          The Traits handler function transform args1, arg2 into a argstruct
         *          The argstruct and the event name are plumbed here (WebService) which serializes that to a JSON object
         *          And pumps the data into all the clients (req here)
         */
        return false;
    }
    template <WebInterfaceImpl T>
    bool _HandleObjectStore(T& /* obj */,
                            httplib::Request const& /* req */,
                            httplib::Response& /* res */,
                            std::string_view const& ifname,
                            std::string_view const& /* path */)
    {
        /*
           * Object Store
           *      Condition: path matches datastore-name (traits)
           *      Action:
           *          Based on the first path-name identified we consult the traits to pass control onto the respective Object-Store
           *          The next keyword dictates the Object-Store action. Each action interacts with central-DataStore (Database2) based on
           the
           * args. Explanation: Object Store apis can be
           *          1. create: Create
           *          2. get: Get-Multiple
           *          3. edit: GetSingle
           *          4. delete: Delete (multiple)
           *          5. all: Search/filter
           *          The Traits converts URL-Encoded args (GET/POST) to data-structs if needed
           *          Each object-store gets a specific wrapper-type (over field-type) to handle situation where a single-type is used for
           * multiple object stores The Interface contains the datamember Database<....> based on all Data-Store members of the interface.
           */

        throw std::logic_error("Not implemented");
    }

    template <WebInterfaceImpl T>
    bool _HandleFunction(T& /* obj */,
                         httplib::Request const& /* req */,
                         httplib::Response& /* res */,
                         std::string_view const& ifname,
                         std::string_view const& /* path */)
    {
        /* Func API
         *      Condition: path matches api-name
         *      Action:
         *          Convert args into Func API arg-struct
         *          Traits are used to translate arg-struct to virtual function args for the given name
         *
         * */
        throw std::logic_error("Not implemented");
    }

    template <WebInterfaceImpl T>
    void _HandleRequest(T& obj, httplib::Request const& req, httplib::Response& res, std::string_view const& path)
    {
        if (path.empty() || path[0] != '/') throw std::logic_error("Invalid path");
        auto index = path.find('/', 1);
        if (index == path.npos) throw std::runtime_error("Invalid api name. Cannot detect api name");
        auto ifname    = path.substr(1, index - 1);
        auto remaining = path.substr(index);

        _HandleEventSource(obj, req, res, ifname, remaining) || _HandleObjectStore(obj, req, ifname, remaining)
            || _HandleFunction(obj, req, res, ifname, remaining);
    }

    template <WebInterfaceImpl T, WebInterfaceImpl... Ts>
    void _HandleRequest(httplib::Request const& req, httplib::Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        if (!impl::iequal(WebServiceHandlerTraits<typename T::Interface>::Url(), ifname))
        {
            if constexpr (sizeof...(Ts) > 0) { return _HandleRequest<Ts...>(req, res, ifname, path); }
            else
            {
                throw std::runtime_error(fmt::format("No matching interface found for {}", ifname));
            }
        }
        return _HandleRequest(std::get<T>(_impls), req, res, path);
    }

    std::thread             _listenthread;
    std::condition_variable _cond;
    std::mutex              _mutex;

    int                   _port;
    httplib::Server       _server;
    std::tuple<TImpls...> _impls;
};
}    // namespace Stencil

#if 0

inline auto tokenizer(std::string_view view, char sep)
{
    struct substr
    {
        size_t           index = 0;
        size_t           size  = 0;
        std::string_view view;
    };
    struct iterator
    {
        iterator() {}
        iterator(std::string_view view, char sep) : _view(view), _sep(sep) { _validatecurrent(); }
        iterator& operator++()
        {
            _validatecurrent();
            return *this;
        }

        const auto& operator*() const { return _current; }

        bool operator==(const iterator& it) const { return _current == it._current && _sep == it._sep; }
        bool operator!=(const iterator& it) const { return !(*this == it); }

        void _validatecurrent()
        {
            assert(!_view.empty());
            if (_index == _view.size())
            {
                (*this = iterator());
                return;
            }

            auto curptr    = &_view.at(_index);
            auto nextptr   = strchr(curptr, _sep);
            _current.index = _index;
            _current.size  = nextptr == nullptr ? (_view.size() - _index) : static_cast<size_t>(nextptr - curptr);
            _current.view  = _view;
            _index         = _current.index + _current.size;
        }

        struct substr
        {
            size_t           index = 0;
            size_t           size  = 0;
            std::string_view view{};
            bool             operator==(const substr& it) const { return index == it.index && size == it.size && view == it.view; }
        } _current;

        size_t           _index = 0;
        std::string_view _view;
        char             _sep{};
    };

    struct range
    {
        iterator _begin;
        iterator _end;
        auto     begin() { return _begin; }
        auto     end() { return _end; }
    } range = {iterator(view, sep), iterator()};
    return range;
}

template <typename TInterface, typename TInterfaceApi, typename... TInterfaceApis>
inline auto WebServiceRequestExecuteApis(httplib::Request const& request, const std::string_view& apiname, const std::string_view& moreurl)
{
    constexpr const std::string_view name = ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::Name();
    if (!iequal(name, apiname))
    {
        if constexpr (sizeof...(TInterfaceApis) != 0)
        {
            return WebServiceRequestExecuteApis<TInterface, TInterfaceApis...>(request, apiname, moreurl);
        }
        else
        {
            throw std::runtime_error("Api not found");
        }
    }

    using ArgsStruct   = typename ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::ArgsStruct;
    using StateTracker = ReflectionServices::StateTraker<ArgsStruct, void*>;
    ArgsStruct args;

    if constexpr (!::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::IsStatic())
    {
        if (!moreurl.empty()) { args.instance = TInterface::FindObjectById(moreurl); }
    }

    StateTracker tracker(&args, nullptr);
    for (auto const& [key, value] : request.params)
    {
        if (tracker.TryObjKey(Value(shared_string::make(key)), nullptr))
        {
            tracker.HandleValue(Value(shared_string::make(value)), nullptr);
        }
    }

    auto retval = ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::Invoke(args);
#ifdef USE_NLOHMANN_JSON
    return Stencil::Json::Stringify<decltype(retval)>(retval);
#else
    throw std::logic_error("No json stringifier defined");
#endif
}

template <typename TInterface, typename... TApis>
inline auto ProcessWebServiceRequestForInterface(httplib::Request const& request,
                                                 const std::string_view& apiname,
                                                 const std::string_view& moreurl,
                                                 ::ReflectionBase::InterfaceApiPack<TApis...>)
{
    return WebServiceRequestExecuteApis<TInterface, TApis...>(request, apiname, moreurl);
}
#endif
