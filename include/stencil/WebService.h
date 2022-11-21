#include "protocol_json.h"

#pragma warning(push, 3)
#pragma warning(disable : 4365)    // conversion from 'const char' to 'unsigned char', signed/unsigned mismatch
#pragma warning(disable : 4355)    //'this' : used in base member initializer list
#pragma warning(disable : 4855)    // implicit capture of 'this' via '[=]' is deprecated
#pragma warning(disable : 4548)    // expression before comma has no effect; expected expression with side - effect
#pragma warning(disable : 4702)    // Unreachable code
#pragma warning(disable : 4668)    // not defined as a preprocessor macro
#pragma warning(disable : 5039)    // pointer or reference to potentially throwing function passed to 'extern)
#pragma warning(disable : 4191)    // type cast': unsafe conversion

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostics  push
#pragma GCC diagnostic   ignored "-Wmaybe-uninitialized"

#include <limits.h>

#include <httplib.h>

#pragma GCC diagnostic   pop
#pragma clang diagnostic pop
#pragma warning(pop)

#include <algorithm>
#include <string_view>
#include <unordered_set>

#define STENCIL_USING_WEBSERVICE 1
namespace Stencil
{
template <typename T> struct InterfaceTraits;          // TODO1
template <typename T> struct InterfaceApiTraits;       // TODO1
template <typename T> struct InterfaceObjectTraits;    // TODO1

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

struct SSEListenerManager
{
    SSEListenerManager() = default;
    CLASS_DELETE_COPY_AND_MOVE(SSEListenerManager);

    struct Instance : std::enable_shared_from_this<Instance>
    {

        SSEListenerManager*     _manager{nullptr};
        httplib::DataSink*      _sink{nullptr};
        std::condition_variable _dataAvailable{};
        bool                    _stopRequested = false;

        Instance() = default;
        CLASS_DELETE_COPY_AND_MOVE(Instance);

        void Start(httplib::DataSink& sink, std::span<const char> const& msg)
        {
            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
                if (_sink != nullptr) { throw std::logic_error("Duplicate sink"); }
                _sink = &sink;
                _sink->write(msg.data(), msg.size());
            }
            do {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                if (_manager->_stopRequested) return;
                _dataAvailable.wait(lock, [&](...) { return _sink != nullptr || _manager->_stopRequested; });
                if (_manager->_stopRequested) return;
                if (!_sink->is_writable())
                {
                    _manager->Remove(lock, shared_from_this());
                    _sink->done();
                    _sink = nullptr;
                    return;
                }
            } while (true);
        }

        void Release()
        {
            auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
            _manager->Remove(lock, shared_from_this());
        }
    };

    void Send(std::span<const char> const& msg)
    {
        auto lock = std::unique_lock<std::mutex>(_mutex);
        for (auto const& inst : _sseListeners)
        {
            if (_stopRequested) return;
            if (inst->_stopRequested) continue;
            try
            {
                if (!inst->_sink->is_writable()) { continue; }
                inst->_sink->write(msg.data(), msg.size());
                inst->_dataAvailable.notify_all();
            } catch (...)
            {}
        }
    }

    std::shared_ptr<Instance> CreateInstance()
    {
        auto inst      = std::make_shared<Instance>();
        inst->_manager = this;
        return inst;
    }

    void Register(std::unique_lock<std::mutex> const& /* lock */, std::shared_ptr<Instance> instance) { _sseListeners.insert(instance); }

    void Remove(std::unique_lock<std::mutex> const& /* lock */, std::shared_ptr<Instance> instance) { _sseListeners.erase(instance); }

    void Stop()
    {
        auto lock      = std::unique_lock<std::mutex>(_mutex);
        _stopRequested = true;

        for (auto const& inst : _sseListeners)
        {
            inst->_stopRequested = true;
            inst->_dataAvailable.notify_all();
        }
    }
    std::unordered_set<std::shared_ptr<Instance>> _sseListeners;
    bool                                          _stopRequested = false;
    std::mutex                                    _mutex;
};

}    // namespace impl

template <typename T>
concept ConceptInterface = requires { typename Stencil::InterfaceTraits<T>; };

template <ConceptInterface... Ts> struct WebService : public Stencil::impl::Interface::InterfaceEventHandlerT<WebService<Ts...>, Ts>...
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
    ~WebService() override
    {
        _sseManager.Stop();
        StopDaemon();
        WaitForStop();
    }
    CLASS_DELETE_COPY_AND_MOVE(WebService);

    void StartOnPort(uint16_t port)
    {
        auto guardscope = std::unique_lock<std::mutex>();
        _server.set_pre_routing_handler([this](httplib::Request const& req, httplib::Response& res) {
            return this->_HandleRequest(req, res) ? httplib::Server::HandlerResponse::Handled : httplib::Server::HandlerResponse::Unhandled;
        });
        //_server.set_error_handler([this](auto... args) { return this->_HandleRequest(args...); });
        _port         = port;
        _listenthread = std::thread([this]() { _server.listen("localhost", _port); });

        for (int i = 0; i < 100 && !_server.is_running(); i++) std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!_server.is_running()) throw std::runtime_error("Unable to start server");

        std::apply([&](auto& impl) { impl->handler = this; }, _impls);
    }

    void StopDaemon()
    {
        {
            auto guardscope = std::unique_lock<std::mutex>();
            std::apply([](auto& impl) { impl->handler = nullptr; }, _impls);
            _server.stop();
        }
        _cond.notify_all();
    }

    void WaitForStop() { _listenthread.join(); }

    template <typename TEventArgs> void OnEvent(TEventArgs const& args)
    {
        auto msg = fmt::format("event: init\ndata: {}\n\n", Stencil::Json::Stringify(args));
        _sseManager.Send(msg);    // Empty data
    }

    template <ConceptInterface T> auto& GetInterface() { return *std::get<std::unique_ptr<T>>(_impls).get(); }

    private:
    bool _HandleRequest(const httplib::Request& req, httplib::Response& res)
    {
        std::string_view const& path = req.path;
        if (path.empty() || path[0] != '/') throw std::logic_error("Invalid path");
        auto stindex = std::string_view("/api/").length();
        auto index   = path.find('/', stindex);
        if (index == path.npos) return false;
        try
        {

            auto ifname = path.substr(stindex, index - stindex);
            _HandleRequest<Ts...>(req, res, ifname, path.substr(index));
        } catch (std::exception const& ex)
        {
            res.status = 500;
            res.body   = ex.what();
        }
        return true;
    }

    void _HandleError(const httplib::Request& /* req */, httplib::Response& res)
    {
        res.set_content(fmt::format("<p>Error Status: <span style='color:red;'>{}</span></p>", res.status), "text/html");
    }

    template <typename TArgsStruct> auto _CreateArgStruct(httplib::Request const& req)
    {
        TArgsStruct args{};
        for (auto const& [keystr, valjson] : req.params)
        {
            using TKey = Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
            TKey key{};
            Stencil::SerDesRead<Stencil::ProtocolString>(key, keystr);
            Visitor<TArgsStruct>::VisitKey(args, key, [&](auto& val) { Stencil::SerDesRead<Stencil::ProtocolJsonVal>(val, valjson); });
        }
        return args;
    }

    template <typename TTup, size_t TTupIndex, ConceptInterface T>
    bool _TryHandleEvents(T&                      obj,
                          httplib::Request const& req,
                          httplib::Response&      res,
                          std::string_view const& ifname,
                          std::string_view const& path)
    {
        if constexpr (TTupIndex == 0) { return false; }
        else
        {
            using SelectedTup = std::tuple_element_t<TTupIndex - 1, TTup>;
            if (!impl::iequal(Stencil::InterfaceApiTraits<SelectedTup>::Name(), ifname))
            {
                return _TryHandleEvents<TTup, TTupIndex - 1, T>(obj, req, res, ifname, path);
            }
            res.set_header("Access-Control-Allow-Origin", "*");
            auto instance = _sseManager.CreateInstance();
            res.set_chunked_content_provider(
                "text/event-stream",
                [instance](size_t /*offset*/, httplib::DataSink& sink) {
                    // This function doesnt return so its important to release the lock before entering start
                    instance->Start(sink, "event: init\ndata: \n\n");    // Empty data
                    return true;
                },
                [instance](bool /*success*/) { instance->Release(); });
            res.status = 200;
            return true;
        }
    }

    template <ConceptInterface T>
    bool
    _HandleEvents(T& obj, httplib::Request const& req, httplib::Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        using Tup = typename Stencil::InterfaceTraits<T>::EventStructs;
        return _TryHandleEvents<Tup, std::tuple_size_v<Tup>, T>(obj, req, res, ifname, path);
    }

    template <typename TTup, ConceptInterface T, size_t TTupIndex = std::tuple_size_v<TTup>>
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
            if (!impl::iequal(Stencil::InterfaceObjectTraits<SelectedTup>::Name(), ifname))
            {
                return _TryHandleObjectStore<TTup, T, TTupIndex - 1>(obj, req, res, ifname, path);
            }
            if (path == "/create")
            {
                auto lock       = obj.objects.LockForEdit();
                auto [id, obj1] = obj.objects.template Create<SelectedTup>(lock, _CreateArgStruct<SelectedTup>(req));
                uint32_t idint  = id.objId;
                auto     rslt   = Stencil::Json::Stringify<decltype(idint)>(idint);
                res.set_content(rslt, "application/json");
                return true;
            }
            else if (path == "/get")
            {
                auto              ids = req.params.find("ids")->second;
                std::stringstream rslt;
                rslt << '[';
                bool   first  = true;
                size_t sindex = 0;
                do {
                    auto eindex = ids.find(',', sindex);
                    if (eindex == std::string_view::npos) eindex = ids.size();
                    if (!first) { rslt << ','; }
                    first          = false;
                    auto     idstr = ids.substr(sindex, eindex - sindex);
                    uint32_t id    = static_cast<uint32_t>(std::stoul(idstr));
                    auto     lock  = obj.objects.LockForEdit();
                    auto     obj1  = obj.objects.template Get<SelectedTup>(lock, Database2::impl::Ref::FromUInt(id));
                    // auto jsobj = Stencil::Json::Stringify<SelectedTup>(obj1);
                    // rslt << jsobj;
                    sindex = eindex + 1;
                } while (sindex < ids.size());
                rslt << ']';
                res.set_content(rslt.str(), "application/json");
                return true;
            }
            else { throw std::logic_error("Not implemented"); }
        }
    }

    template <ConceptInterface T>
    bool _HandleObjectStore(T&                      obj,
                            httplib::Request const& req,
                            httplib::Response&      res,
                            std::string_view const& ifname,
                            std::string_view const& path)
    {
        if (ifname == "objectstore")
        {
            res.set_header("Access-Control-Allow-Origin", "*");
            auto instance = _sseManager.CreateInstance();
            res.set_chunked_content_provider(
                "text/event-stream",
                [instance](size_t /*offset*/, httplib::DataSink& sink) {
                    // This function doesnt return so its important to release the lock before entering start
                    instance->Start(sink, "event: init\ndata: \n\n");    // Empty data
                    return true;
                },
                [instance](bool /*success*/) { instance->Release(); });
            res.status = 200;
            return true;
        }

        return _TryHandleObjectStore<typename Stencil::InterfaceTraits<T>::Objects, T>(obj, req, res, ifname, path);
    }

    template <typename TTup, size_t TTupIndex, ConceptInterface T>
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
            if (!impl::iequal(Stencil::InterfaceApiTraits<SelectedTup>::Name(), ifname))
            {
                return _TryHandleFunction<TTup, TTupIndex - 1, T>(obj, req, res, ifname, path);
            }
            using Traits = ::Stencil::InterfaceApiTraits<SelectedTup>;
            auto args    = _CreateArgStruct<typename Traits::ArgsStruct>(req);
            if constexpr (std::is_same_v<void, decltype(Traits::Invoke(obj, args))>) { Traits::Invoke(obj, args); }
            else
            {
                auto retval = Traits::Invoke(obj, args);
                auto rslt   = Stencil::Json::Stringify<decltype(retval)>(retval);
                res.set_content(rslt, "application/json");
            }
            return true;
        }
    }

    template <ConceptInterface T>
    bool _HandleFunction(T&                      obj,
                         httplib::Request const& req,
                         httplib::Response&      res,
                         std::string_view const& ifname,
                         std::string_view const& path)
    {
        using Tup = typename Stencil::InterfaceTraits<T>::ApiStructs;
        return _TryHandleFunction<Tup, std::tuple_size_v<Tup>, T>(obj, req, res, ifname, path);
    }

    template <ConceptInterface T>
    void _HandleRequest(T& obj, httplib::Request const& req, httplib::Response& res, std::string_view const& path)
    {
        if (path.empty() || path[0] != '/') throw std::logic_error("Invalid path");
        auto index = path.find('/', 1);
        if (index == path.npos) index = path.size();
        auto ifname    = path.substr(1, index - 1);
        auto remaining = path.substr(index);

        bool found = _HandleEvents(obj, req, res, ifname, remaining) || _HandleObjectStore(obj, req, res, ifname, remaining)
                     || _HandleFunction(obj, req, res, ifname, remaining);

        if (!found) { throw std::runtime_error(fmt::format("No Matching api found for {}", path)); }
    }

    template <ConceptInterface T1, ConceptInterface... T1s>
    void _HandleRequest(httplib::Request const& req, httplib::Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        if (!impl::iequal(Stencil::InterfaceTraits<T1>::Name(), ifname))
        {
            if constexpr (sizeof...(T1s) > 0) { return _HandleRequest<T1s...>(req, res, ifname, path); }
            else { throw std::runtime_error(fmt::format("No matching interface found for {}", ifname)); }
        }
        return _HandleRequest(*std::get<std::unique_ptr<T1>>(_impls).get(), req, res, path);
    }

    std::thread             _listenthread;
    std::condition_variable _cond;
    std::mutex              _mutex;

    int                                _port;
    httplib::Server                    _server;
    impl::SSEListenerManager           _sseManager;
    std::tuple<std::unique_ptr<Ts>...> _impls = {Ts::Create()...};
};
}    // namespace Stencil
