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
    return std::equal(std::begin(a), std::end(a), std::begin(b), std::end(b), [](auto a1, auto b1) { return tolower(a1) == tolower(b1); });
}
}    // namespace impl

template <typename T>
concept WebInterfaceImpl = requires(T t)
{
    typename T::Interface;
    typename WebServiceHandlerTraits<typename T::Interface>;
};

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
        _server.Get("/.*", [this](httplib::Request const& req, httplib::Response& res) { return this->_HandleRequest(req, res); });
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
            _HandleRequest<TImpls...>(req, res, ifname, path.substr(index));
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

    template <typename TArgsStruct> auto _CreateArgStruct(httplib::Request const& req)
    {
        using StateTracker = ReflectionServices::StateTraker<TArgsStruct, void*>;
        TArgsStruct  args;
        StateTracker tracker(&args, nullptr);
        for (auto const& [key, value] : req.params)
        {
            if (tracker.TryObjKey(Value(shared_string::make(key)), nullptr))
            {
                tracker.HandleValue(Value(shared_string::make(value)), nullptr);
            }
        }
        return args;
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
        throw std::logic_error("Not implemented");
    }

    template <typename TTup, size_t TTupIndex, WebInterfaceImpl T>
    bool _TryHandleObjectStore(T&                      obj,
                               httplib::Request const& req,
                               httplib::Response&      res,
                               std::string_view const& ifname,
                               std::string_view const& path)
    {
        if constexpr (TTupIndex == 0) { return false; }
        else
        {
            using SelectedTup = std::tuple_element_t<TTupIndex - 1, TTup>;
            if (!impl::iequal(ReflectionBase::InterfaceObjectTraits<SelectedTup>::Name(), ifname))
            {
                return _TryHandleObjectStore<TTup, TTupIndex - 1, T>(obj, req, res, ifname, path);
            }
            if (path == "/create")
            {
                auto lock       = obj.objects.LockForEdit();
                auto [id, obj1] = obj.objects.template Create<SelectedTup>(lock, _CreateArgStruct<SelectedTup>(req));
                uint32_t idint  = ((uint32_t{id.page} << 16) | uint32_t{id.slot});
                auto     rslt   = Stencil::Json::Stringify<decltype(idint)>(idint);
                res.set_content(rslt, "application/json");
                return true;
            }
            else if (path == "/get")
            {
                auto              ids = req.params.find("ids")->second;
                std::stringstream rslt;
                rslt << '[';
                bool   first;
                size_t sindex = 0;
                do {
                    auto eindex = ids.find(',', sindex);
                    if (eindex == std::string_view::npos) eindex = ids.size();
                    if (!first) { rslt << ','; }
                    auto idstr = ids.substr(sindex, eindex - sindex);
                    auto id    = std::stoul(idstr);
                    auto lock  = obj.objects.LockForEdit();
                    auto obj1  = obj.objects.template Get<SelectedTup>(lock, Database2::impl::Ref::FromUInt(id));
                    auto jsobj = Stencil::Json::Stringify<SelectedTup>(obj1);
                    rslt << jsobj;
                    sindex = eindex + 1;
                } while (sindex < ids.size());
                rslt << ']';
                res.set_content(rslt.str(), "application/json");
                return true;
            }
            else
            {
                throw std::logic_error("Not implemented");
            }
        }
    }

    template <WebInterfaceImpl T>
    bool _HandleObjectStore(T&                      obj,
                            httplib::Request const& req,
                            httplib::Response&      res,
                            std::string_view const& ifname,
                            std::string_view const& path)
    {
        using ObjectsTup = typename ReflectionBase::InterfaceTraits<typename T::Interface>::Objects;
        return _TryHandleObjectStore<ObjectsTup, std::tuple_size_v<ObjectsTup>, T>(obj, req, res, ifname, path);
    }

    template <typename TTup, size_t TTupIndex, WebInterfaceImpl T>
    bool _TryHandleFunction(T&                      obj,
                            httplib::Request const& req,
                            httplib::Response&      res,
                            std::string_view const& ifname,
                            std::string_view const& path)
    {
        if constexpr (TTupIndex == 0) { return false; }
        else
        {
            using SelectedTup = std::tuple_element_t<TTupIndex - 1, TTup>;
            if (!impl::iequal(ReflectionBase::InterfaceApiTraits<SelectedTup>::Name(), ifname))
            {
                return _TryHandleFunction<TTup, TTupIndex - 1, T>(obj, req, res, ifname, path);
            }
            using Traits = ::ReflectionBase::InterfaceApiTraits<SelectedTup>;
            auto args    = _CreateArgStruct<typename Traits::ArgsStruct>(req);
            if constexpr (std::is_same_v<void, decltype(Traits::Invoke(args))>) { Traits::Invoke(args); }
            else
            {
                auto retval = Traits::Invoke(args);
#ifdef USE_NLOHMANN_JSON
                auto rslt = Stencil::Json::Stringify<decltype(retval)>(retval);
                res.set_content(rslt, "application/json");
#else
                throw std::logic_error("No json stringifier defined");
#endif
            }
            return true;
        }
    }

    template <WebInterfaceImpl T>
    bool _HandleFunction(T&                      obj,
                         httplib::Request const& req,
                         httplib::Response&      res,
                         std::string_view const& ifname,
                         std::string_view const& path)
    {
        using Tup = typename ReflectionBase::InterfaceTraits<typename T::Interface>::Apis;
        /* Func API
         *      Condition: path matches api-name
         *      Action:
         *          Convert args into Func API arg-struct
         *          Traits are used to translate arg-struct to virtual function args for the given name
         *
         * */
        return _TryHandleFunction<Tup, std::tuple_size_v<Tup>, T>(obj, req, res, ifname, path);
    }

    template <WebInterfaceImpl T>
    void _HandleRequest(T& obj, httplib::Request const& req, httplib::Response& res, std::string_view const& path)
    {
        if (path.empty() || path[0] != '/') throw std::logic_error("Invalid path");
        auto index = path.find('/', 1);
        if (index == path.npos) index = path.size();
        auto ifname    = path.substr(1, index - 1);
        auto remaining = path.substr(index);

        bool found = _HandleEventSource(obj, req, res, ifname, remaining) || _HandleObjectStore(obj, req, res, ifname, remaining)
                     || _HandleFunction(obj, req, res, ifname, remaining);

        if (!found) { throw std::runtime_error(fmt::format("No Matching api found for {}", path)); }
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
