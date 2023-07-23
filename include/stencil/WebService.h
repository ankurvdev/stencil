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
template <typename T> struct InterfaceTraits;
template <typename T> struct InterfaceApiTraits;
template <typename T> struct InterfaceObjectTraits;

template <typename T>
concept ConceptInterface = requires { typename Stencil::InterfaceTraits<T>; };

namespace impl
{
template <typename T1, typename T2> inline bool iequal(T1 const& a, T2 const& b)
{
    return std::equal(std::begin(a), std::end(a), std::begin(b), std::end(b), [](auto a1, auto b1) { return tolower(a1) == tolower(b1); });
}

inline std::tuple<std::string_view, std::string_view> Split(std::string_view const& path, char token = '/')
{
    if (path.empty()) throw std::logic_error("Invalid path");
    size_t start = path[0] == token ? 1u : 0u;
    size_t index = path.find(token, start);
    auto   str1  = path.substr(start, index - start);
    if (index == path.npos) return {str1, {}};
    return {str1, path.substr(index)};
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
            auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();
            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
                if (_sink != nullptr) { throw std::logic_error("Duplicate sink"); }
                _sink = &sink;
                _sink->write(msg.data(), msgSize);
            }
            do {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                if (_manager->_stopRequested) return;
                _dataAvailable.wait(lock, [&](...) { return _sink != nullptr || _manager->_stopRequested; });
                if (_manager->_stopRequested) return;
                if (_sink->os.fail())
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
        auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();

        auto lock = std::unique_lock<std::mutex>(_mutex);
        for (auto const& inst : _sseListeners)
        {
            if (_stopRequested) return;
            if (inst->_stopRequested) continue;
            try
            {
                inst->_sink->write(msg.data(), msgSize);
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

template <ConceptInterface T> struct WebRequestHandler
{

    T&                        obj;
    httplib::Request const&   req;
    httplib::Response&        res;
    impl::SSEListenerManager& sse;
    std::ostringstream        rslt;

    WebRequestHandler(T& objIn, httplib::Request const& reqIn, httplib::Response& resIn, impl::SSEListenerManager& sseIn) :
        obj(objIn), req(reqIn), res(resIn), sse(sseIn)
    {}
    ~WebRequestHandler() = default;
    CLASS_DELETE_COPY_AND_MOVE(WebRequestHandler);

    template <typename TArgsStruct> auto _CreateArgStruct()
    {
        TArgsStruct args{};
        for (auto const& [keystr, valjson] : req.params)
        {
            using TKey = typename Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
            TKey key{};
            Stencil::SerDesRead<Stencil::ProtocolString>(key, keystr);
            auto& jsonval = valjson;    // Clang complains about capturing localbinding variables
            Visitor<TArgsStruct>::VisitKey(args, key, [&](auto& val) { Stencil::SerDesRead<Stencil::ProtocolJsonVal>(val, jsonval); });
        }
        return args;
    }

    template <typename TLambda> auto _ForeachObjId(std::string_view const& subpath, TLambda&& lambda)
    {
        if (subpath.empty())
        {
            auto it = req.params.find("ids");
            if (it == req.params.end()) { throw std::invalid_argument("No Id specified for read"); }
            auto ids = it->second;
            rslt << '{';
            bool   first  = true;
            size_t sindex = 0;
            do {
                auto eindex = ids.find(',', sindex);
                if (eindex == std::string_view::npos) eindex = ids.size();
                auto idstr = ids.substr(sindex, eindex - sindex);
                if (!first) { rslt << ','; }
                rslt << idstr;
                first       = false;
                uint32_t id = static_cast<uint32_t>(std::stoul(idstr));
                lambda(id);
                sindex = eindex + 1;
            } while (sindex < ids.size());
            rslt << '}';
        }
        else
        {
            uint32_t id = static_cast<uint32_t>(std::stoul(std::string(subpath.substr(1))));
            lambda(id);
        }
        return rslt.str();
    }

    template <typename TEventStructs> bool TryHandleEvents(std::string_view const& ifname, std::string_view const& /*path*/)
    {
        if (!impl::iequal(Stencil::InterfaceApiTraits<TEventStructs>::Name(), ifname)) { return false; }
        auto instance = sse.CreateInstance();
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

    template <typename TInterfaceObj> bool TryHandleObjectStore(std::string_view const& ifname, std::string_view const& path)
    {
        if (!impl::iequal(Stencil::InterfaceObjectTraits<TInterfaceObj>::Name(), ifname)) { return false; }

        auto [action, subpath] = Split(path);
        if (action == "create")
        {
            auto lock       = obj.objects.LockForEdit();
            auto [id, obj1] = obj.objects.template Create<TInterfaceObj>(lock, _CreateArgStruct<TInterfaceObj>());
            uint32_t idint  = id.id;
            fmt::print(rslt, "{}", Stencil::Json::Stringify(idint));
            sse.Send(fmt::format("event: objectstore_create\ndata: {{\"{}\": {{\"{}\": {}}}}}\n\n",
                                 Stencil::InterfaceObjectTraits<TInterfaceObj>::Name(),
                                 idint,
                                 Stencil::Json::Stringify(Stencil::Database::CreateRecordView(obj.objects, lock, id, obj1))));
        }
        else if (action == "all")
        {
            auto lock = obj.objects.LockForRead();
            rslt << '{';
            bool first = true;
            for (auto const& [ref, obj1] : obj.objects.template Items<TInterfaceObj>(lock))
            {
                if (!first) { rslt << ','; }
                rslt << '\"' << ref.id << '\"' << ':'
                     << Stencil::Json::Stringify(Stencil::Database::CreateRecordView(obj.objects, lock, ref, obj1));
                first = false;
            }
            rslt << '}';
        }
        else if (action == "read")
        {
            auto lock = obj.objects.LockForRead();
            _ForeachObjId(subpath, [&](uint32_t id) {
                auto obj1  = obj.objects.template Get<TInterfaceObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(obj.objects, lock, {id}, obj1));
                rslt << jsobj;
            });
        }
        else if (action == "edit")
        {
            sse.Send(fmt::format("event: objectstore_edit\ndata: {{\'{}\': {{", Stencil::InterfaceObjectTraits<TInterfaceObj>::Name()));
            auto lock  = obj.objects.LockForEdit();
            bool first = true;
            _ForeachObjId(subpath, [&](uint32_t id) {
                auto obj1  = obj.objects.template Get<TInterfaceObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(obj.objects, lock, {id}, obj1));
                rslt << jsobj;
                sse.Send(fmt::format("{}\'{}\': {}", (first ? ' ' : ','), id, jsobj));
                first = false;
            });
            sse.Send("}}\n\n");
        }
        else if (action == "delete")
        {
            sse.Send(fmt::format("event: objectstore_delete\ndata: {{\'{}\': [", Stencil::InterfaceObjectTraits<TInterfaceObj>::Name()));
            auto lock  = obj.objects.LockForEdit();
            bool first = true;
            _ForeachObjId(subpath, [&](uint32_t id) {
                try
                {
                    obj.objects.template Delete<TInterfaceObj>(lock, {id});
                    rslt << "true";
                    sse.Send(fmt::format("{}{}", (first ? ' ' : ','), id));
                    first = false;
                } catch (std::exception const& /*ex*/)
                {
                    rslt << "false";
                }
            });
            sse.Send("]}\n\n");
        }
        else { throw std::logic_error("Not implemented"); }
        return true;
    }

    template <typename TArgsStruct> bool TryHandleFunction(std::string_view const& ifname, std::string_view const& /*path*/)
    {
        if (!impl::iequal(Stencil::InterfaceApiTraits<TArgsStruct>::Name(), ifname)) { return false; }

        using Traits = ::Stencil::InterfaceApiTraits<TArgsStruct>;
        auto args    = _CreateArgStruct<TArgsStruct>();
        if constexpr (std::is_same_v<void, decltype(Traits::Invoke(obj, args))>) { Traits::Invoke(obj, args); }
        else
        {
            auto retval = Traits::Invoke(obj, args);
            rslt << Stencil::Json::Stringify<decltype(retval)>(retval);
        }
        return true;
    }

    bool HandleObjectStore(std::string_view const& ifname, std::string_view const& path)
    {
        bool found = false;
        std::apply([&](auto... tup) { found = found || (TryHandleObjectStore<std::remove_cvref_t<decltype(tup)>>(ifname, path) || ...); },
                   typename Stencil::InterfaceTraits<T>::Objects{});
        return found;
    }

    bool HandleFunction(std::string_view const& ifname, std::string_view const& path)
    {
        bool found = false;
        std::apply([&](auto... tup) { found = found || (TryHandleFunction<std::remove_cvref_t<decltype(tup)>>(ifname, path) || ...); },
                   typename Stencil::InterfaceTraits<T>::ApiStructs{});
        return found;
    }

    bool HandleEvents(std::string_view const& ifname, std::string_view const& path)
    {
        bool found = false;
        std::apply([&](auto... tup) { found = found || (TryHandleEvents<std::remove_cvref_t<decltype(tup)>>(ifname, path) || ...); },
                   typename Stencil::InterfaceTraits<T>::EventStructs{});
        return found;
    }

    void HandleRequest(std::string_view const& path)
    {
        res.set_header("Access-Control-Allow-Origin", "*");

        auto [ifname, subpath] = Split(path);
        if (ifname == "objectstore")
        {
            auto instance = sse.CreateInstance();
            auto it       = req.params.find("query");
            rslt << '[';
            bool first = true;
            if (it != req.params.end())
            {
                std::string_view query = it->second;
                do {
                    rslt << (first ? ' ' : ',');
                    first                    = false;
                    auto [query1, remaining] = Split(query, ',');
                    auto [qifname, qsubpath] = Split(query1);
                    if (!HandleObjectStore(qifname, qsubpath)) throw std::runtime_error(fmt::format("Cannot handle query : {}", query1));
                    query = remaining;
                } while (!query.empty());
            }
            rslt << ']';
            auto rsltstr = rslt.str();
            res.set_chunked_content_provider(
                "text/event-stream",
                [instance, rsltstr](size_t /*offset*/, httplib::DataSink& sink) {
                    instance->Start(sink, fmt::format("event: init\ndata: {}\n\n", rsltstr));

                    return true;
                },
                [instance](bool /*success*/) { instance->Release(); });
            res.status = 200;
            return;
        }

        bool found = HandleEvents(ifname, subpath) || HandleObjectStore(ifname, subpath) || HandleFunction(ifname, subpath);

        if (!found) { throw std::runtime_error(fmt::format("No Matching api found for {}", path)); }
        res.set_content(rslt.str(), "application/json");
    }
};

}    // namespace impl

template <ConceptInterface... Ts> struct WebService : public Stencil::impl::Interface::InterfaceEventHandlerT<WebService<Ts...>, Ts>...
{

    WebService() = default;
    ~WebService() override { StopDaemon(); }
    CLASS_DELETE_COPY_AND_MOVE(WebService);

    auto& Server() { return _server; }

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
            if (_server.is_running())
            {
                std::apply([](auto& impl) { impl->handler = nullptr; }, _impls);
                _server.stop();
                _sseManager.Stop();
                _cond.notify_all();
            }
        }
        if (_listenthread.joinable()) _listenthread.join();
    }

    void WaitForStop()
    {
        if (_listenthread.joinable()) _listenthread.join();
    }

    template <typename TEventArgs> void OnEvent(TEventArgs const& args)
    {
        auto msg = fmt::format("event: {}\ndata: {}\n\n", Stencil::InterfaceApiTraits<TEventArgs>::Name(), Stencil::Json::Stringify(args));
        _sseManager.Send(msg);
    }

    template <ConceptInterface T> auto& GetInterface() { return *std::get<std::unique_ptr<T>>(_impls).get(); }

    private:
    bool _HandleRequest(const httplib::Request& req, httplib::Response& res)
    {
        std::string_view const& path = req.path;
        auto [ifname, subpath]       = impl::Split(path);
        if (ifname != "api" || subpath.empty()) return false;
        try
        {
            auto [ifname1, subpath1] = impl::Split(subpath);
            bool found               = (_TryHandleRequest<Ts>(req, res, ifname1, subpath1) || ...);
            if (!found) throw std::runtime_error(fmt::format("No matching interface found for {}", ifname1));
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

    template <ConceptInterface T>
    bool
    _TryHandleRequest(httplib::Request const& req, httplib::Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        if (!impl::iequal(Stencil::InterfaceTraits<T>::Name(), ifname)) { return false; }
        impl::WebRequestHandler<T>{*std::get<std::unique_ptr<T>>(_impls).get(), req, res, _sseManager}.HandleRequest(path);
        return true;
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
