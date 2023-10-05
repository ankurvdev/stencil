#include "CommonMacros.h"
#include "interfaces.h"
#include "protocol_json.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
SUPPRESS_WARNINGS_END

#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#define STENCIL_USING_WEBSERVICE

#if !defined(BOOST_ASIO_HAS_CO_AWAIT)
#error Need co await
#endif
namespace Stencil::impl
{
using Request  = boost::beast::http::request<boost::beast::http::string_body>;
using Response = boost::beast::http::response<boost::beast::http::string_body>;

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

        SSEListenerManager*           _manager{nullptr};
        void* /* httplib::DataSink**/ _sink{nullptr};
        std::condition_variable       _dataAvailable{};
        bool                          _stopRequested = false;

        Instance() = default;
        CLASS_DELETE_COPY_AND_MOVE(Instance);

        void Start(void* /* httplib::DataSink& */ sink, std::span<const char> const& msg)
        {
            auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();
            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
                if (_sink != nullptr) { throw std::logic_error("Duplicate sink"); }
                _sink = &sink;
                std::cout << msg.data() << msgSize;
                // _sink->write(msg.data(), msgSize);
            }
            do {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                if (_manager->_stopRequested) return;
                _dataAvailable.wait(lock, [&](...) { return _sink != nullptr || _manager->_stopRequested; });
                if (_manager->_stopRequested) return;
                /*if (_sink->os.fail())
                {
                    _manager->Remove(lock, shared_from_this());
                    _sink->done();
                    _sink = nullptr;
                    return;
                }*/
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
                std::cout << msg.data() << msgSize;
                // inst->_sink->write(msg.data(), msgSize);
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
    Request const&            req;
    Response&                 res;
    impl::SSEListenerManager& sse;
    std::ostringstream        rslt;

    WebRequestHandler(T& objIn, Request const& reqIn, Response& resIn, impl::SSEListenerManager& sseIn) :
        obj(objIn), req(reqIn), res(resIn), sse(sseIn)
    {}
    ~WebRequestHandler() = default;
    CLASS_DELETE_COPY_AND_MOVE(WebRequestHandler);

    template <typename TArgsStruct> auto _CreateArgStruct()
    {
        TArgsStruct args{};
        for (auto const& param : req)
        {
            auto keystr  = param.name_string();
            auto valjson = param.value();
            using TKey   = typename Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
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
            auto it = req.find("ids");
            if (it == req.end()) { throw std::invalid_argument("No Id specified for read"); }
            auto ids = it->value();
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
        TODO("Chunked");
#if 0
        res.set_chunked_content_provider(
            "text/event-stream",
            [instance](size_t /*offset*/, httplib::DataSink& sink) {
                // This function doesnt return so its important to release the lock before entering start
                instance->Start(sink, "event: init\ndata: \n\n");    // Empty data
                return true;
            },
            [instance](bool /*success*/) { instance->Release(); });
        res.status = 200;
#endif
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
        // res.set_header("Access-Control-Allow-Origin", "*");

        auto [ifname, subpath] = impl::Split(path);
        if (ifname == "objectstore")
        {
            auto instance = sse.CreateInstance();
            auto it       = req.find("query");
            rslt << '[';
            bool first = true;
            if (it != req.end())
            {
                std::string_view query = it->value();
                do {
                    rslt << (first ? ' ' : ',');
                    first                    = false;
                    auto [query1, remaining] = impl::Split(query, ',');
                    auto [qifname, qsubpath] = impl::Split(query1);
                    if (!HandleObjectStore(qifname, qsubpath)) throw std::runtime_error(fmt::format("Cannot handle query : {}", query1));
                    query = remaining;
                } while (!query.empty());
            }
            rslt << ']';
            auto rsltstr = rslt.str();
#if 0
            res.set_chunked_content_provider(
                "text/event-stream",
                [instance, rsltstr](size_t /*offset*/, httplib::DataSink& sink) {
                    instance->Start(sink, fmt::format("event: init\ndata: {}\n\n", rsltstr));

                    return true;
                },
                [instance](bool /*success*/) { instance->Release(); });
            res.status = 200;
#endif
            return;
        }

        bool found = HandleEvents(ifname, subpath) || HandleObjectStore(ifname, subpath) || HandleFunction(ifname, subpath);

        if (!found) { throw std::runtime_error(fmt::format("No Matching api found for {}", path)); }
        // res.set_content(rslt.str(), "application/json");
    }
};

}    // namespace Stencil::impl
namespace Stencil
{

template <ConceptInterface... Ts> struct WebService : public Stencil::impl::Interface::InterfaceEventHandlerT<WebService<Ts...>, Ts>...
{
    using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
    using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
        boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;

    using Request  = boost::beast::http::request<boost::beast::http::string_body>;
    using Response = boost::beast::http::response<boost::beast::http::string_body>;
    WebService()   = default;
    ~WebService() override { StopDaemon(); }
    CLASS_DELETE_COPY_AND_MOVE(WebService);

    template <typename T1, typename T2> inline bool iequal(T1 const& a, T2 const& b)
    {
        return std::equal(
            std::begin(a), std::end(a), std::begin(b), std::end(b), [](auto a1, auto b1) { return tolower(a1) == tolower(b1); });
    }

    // auto& Server() { return *this; }

    void StartOnPort(uint16_t port)
    {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        boost::asio::co_spawn(ioc, _do_listen(tcp::endpoint{address, port}), [](std::exception_ptr e) {
            if (e) try
                {
                    std::rethrow_exception(e);
                } catch (std::exception& e)
                {
                    std::cerr << "Error in acceptor: " << e.what() << "\n";
                }
        });

        _listenthread = std::thread([this]() { ioc.run(); });

        std::apply([&](auto& impl) { impl->handler = this; }, _impls);
    }

    void StopDaemon() { ioc.stop(); }

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
    // Return a reasonable mime type based on the extension of a file.
    static boost::beast::string_view mime_type(boost::beast::string_view path)
    {
        using boost::beast::iequals;
        auto const ext = [&path] {
            auto const pos = path.rfind(".");
            if (pos == boost::beast::string_view::npos) return boost::beast::string_view{};
            return path.substr(pos);
        }();
        if (iequals(ext, ".htm")) return "text/html";
        if (iequals(ext, ".html")) return "text/html";
        if (iequals(ext, ".php")) return "text/html";
        if (iequals(ext, ".css")) return "text/css";
        if (iequals(ext, ".txt")) return "text/plain";
        if (iequals(ext, ".js")) return "application/javascript";
        if (iequals(ext, ".json")) return "application/json";
        if (iequals(ext, ".xml")) return "application/xml";
        if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
        if (iequals(ext, ".flv")) return "video/x-flv";
        if (iequals(ext, ".png")) return "image/png";
        if (iequals(ext, ".jpe")) return "image/jpeg";
        if (iequals(ext, ".jpeg")) return "image/jpeg";
        if (iequals(ext, ".jpg")) return "image/jpeg";
        if (iequals(ext, ".gif")) return "image/gif";
        if (iequals(ext, ".bmp")) return "image/bmp";
        if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
        if (iequals(ext, ".tiff")) return "image/tiff";
        if (iequals(ext, ".tif")) return "image/tiff";
        if (iequals(ext, ".svg")) return "image/svg+xml";
        if (iequals(ext, ".svgz")) return "image/svg+xml";
        return "application/text";
    }

    // Return a response for the given request.
    //
    // The concrete type of the response message (which depends on the
    // request), is type-erased in message_generator.
    template <class Body, class Allocator>
    boost::beast::http::message_generator
    _handle_request(boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req)
    {
        // Returns a bad request response
        auto const bad_request = [&req](boost::beast::string_view why) {
            boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, req.version()};
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(why);
            res.prepare_payload();
            return res;
        };

#if 0
        // Returns a not found response
        auto const not_found = [&req](boost::beast::string_view target) {
            boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::not_found, req.version()};
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "The resource '" + std::string(target) + "' was not found.";
            res.prepare_payload();
            return res;
        };

        // Returns a server error response
        auto const server_error = [&req](boost::beast::string_view what) {
            boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::internal_server_error,
                                                                              req.version()};
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();
            return res;
        };
#endif
        // Make sure we can handle the method
        if (req.method() != boost::beast::http::verb::get && req.method() != boost::beast::http::verb::head)
            return bad_request("Unknown HTTP-method");

        Response res{};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "application/json");
        // res.content_length(size);
        res.keep_alive(req.keep_alive());

        // Respond to HEAD request
        if (req.method() == boost::beast::http::verb::head)
        {
            TODO("HEAD");
#if 0
            boost::beast::http::response<boost::beast::http::empty_body> res{boost::beast::http::status::ok, req.version()};
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return res;
#endif
        }
        _HandleRequest(req, res);

        // Respond to GET request
        return res;
    }

    //------------------------------------------------------------------------------

    // Handles an HTTP server connection
    boost::asio::awaitable<void> _do_session(tcp_stream stream)
    {
        // This buffer is required to persist across reads
        boost::beast::flat_buffer buffer;

        // This lambda is used to send messages
        try
        {
            for (;;)
            {
                // Set the timeout.
                stream.expires_after(std::chrono::seconds(30));

                // Read a request
                boost::beast::http::request<boost::beast::http::string_body> req;
                co_await boost::beast::http::async_read(stream, buffer, req);

                // Handle the request
                boost::beast::http::message_generator msg = _handle_request(std::move(req));

                // Determine if we should close the connection
                bool keep_alive = msg.keep_alive();

                // Send the response
                co_await boost::beast::async_write(stream, std::move(msg), boost::asio::use_awaitable);

                if (!keep_alive)
                {
                    // This means we should close the connection, usually because
                    // the response indicated the "Connection: close" semantic.
                    break;
                }
            }
        } catch (boost::system::system_error& se)
        {
            if (se.code() != boost::beast::http::error::end_of_stream) throw;
        }

        // Send a TCP shutdown
        boost::beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
        // we ignore the error because the client might have
        // dropped the connection already.
    }

    //------------------------------------------------------------------------------

    // Accepts incoming connections and launches the sessions
    boost::asio::awaitable<void> _do_listen(tcp::endpoint endpoint)
    {
        // Open the acceptor
        auto acceptor = boost::asio::use_awaitable.as_default_on(tcp::acceptor(co_await boost::asio::this_coro::executor));
        acceptor.open(endpoint.protocol());

        // Allow address reuse
        acceptor.set_option(boost::asio::socket_base::reuse_address(true));

        // Bind to the server address
        acceptor.bind(endpoint);

        // Start listening for connections
        acceptor.listen(boost::asio::socket_base::max_listen_connections);

        for (;;)
            boost::asio::co_spawn(
                acceptor.get_executor(), _do_session(tcp_stream(co_await acceptor.async_accept())), [](std::exception_ptr e) {
                    if (e) try
                        {
                            std::rethrow_exception(e);
                        } catch (std::exception& e)
                        {
                            std::cerr << "Error in session: " << e.what() << "\n";
                        }
                });
    }

    bool _HandleRequest(const Request& req, Response& res)
    {
        std::string_view const& path = req.target();
        auto [ifname, subpath]       = impl::Split(path);
        if (ifname != "api" || subpath.empty()) return false;
        try
        {
            auto [ifname1, subpath1] = impl::Split(subpath);
            bool found               = (_TryHandleRequest<Ts>(req, res, ifname1, subpath1) || ...);
            if (!found) throw std::runtime_error(fmt::format("No matching interface found for {}", ifname1));
        } catch (std::exception const& ex)
        {
            res.result(500);
            res.body() = ex.what();
        }
        return true;
    }

    void _HandleError(const Request& /* req */, Response& /* res */)
    {
        TODO("_HANDLE_ERROR");
        // res.set_content(fmt::format("<p>Error Status: <span style='color:red;'>{}</span></p>", res.status), "text/html");
    }

    template <ConceptInterface T>
    bool _TryHandleRequest(Request const& req, Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        if (!iequal(Stencil::InterfaceTraits<T>::Name(), ifname)) { return false; }
        impl::WebRequestHandler<T>{*std::get<std::unique_ptr<T>>(_impls).get(), req, res, _sseManager}.HandleRequest(path);
        return true;
    }

    boost::asio::io_context ioc{1};

    std::thread             _listenthread;
    std::condition_variable _cond;
    std::mutex              _mutex;

    int                                _port;
    impl::SSEListenerManager           _sseManager;
    std::tuple<std::unique_ptr<Ts>...> _impls = {Ts::Create()...};
};
}    // namespace Stencil
