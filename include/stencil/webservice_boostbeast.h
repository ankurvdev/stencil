#include "CommonMacros.h"

#if !defined HAVE_BOOSTBEAST
#error "Need boost::beast to be linked"
#endif

#include "database.h"
#include "interfaces.h"
#include "protocol_json.h"
#include "serdes.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_MSVC_WARNING(4242)
SUPPRESS_MSVC_WARNING(4702)
SUPPRESS_MSVC_WARNING(5219)
SUPPRESS_MSVC_WARNING(5262)    // implicit fall-through occurs here;
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/url.hpp>

SUPPRESS_WARNINGS_END

#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#if !defined(BOOST_ASIO_HAS_CO_AWAIT)
#error Need co await
#endif
namespace Stencil::impl
{
using Request    = boost::beast::http::request<boost::beast::http::string_body>;
using Response   = boost::beast::http::response<boost::beast::http::buffer_body>;
using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
    boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;

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

template <typename... Types> struct Selector
{
    template <typename T, typename... TArgs> static bool InvokeIfMatch(TArgs&&... args)
    {
        if (T::Matches(std::forward<TArgs>(args)...))
        {
            T::Invoke(std::forward<TArgs>(args)...);
            return true;
        }
        return false;
    }
    template <typename... TArgs> static auto Invoke(TArgs&&... args)
    {
        if constexpr (sizeof...(Types) == 0) { throw std::runtime_error("Cannot match any"); }
        else
        {
            auto result = (InvokeIfMatch<Types>(std::forward<TArgs>(args)...) || ...);
            if (!result) throw std::runtime_error("No match found");
        }
    }
};

struct SSEListenerManager
{
    SSEListenerManager() = default;
    CLASS_DELETE_COPY_AND_MOVE(SSEListenerManager);

    struct Instance : std::enable_shared_from_this<Instance>
    {
        struct SSEContext
        {
            using ResponseSerializer = boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields>;
            tcp_stream&         stream;
            Response&           res;
            ResponseSerializer& sr;
        };

        SSEListenerManager*     _manager{nullptr};
        SSEContext*             _ctx{nullptr};
        std::condition_variable _dataAvailable{};
        bool                    _stopRequested = false;

        Instance() = default;
        CLASS_DELETE_COPY_AND_MOVE(Instance);
        bool _streamEnded() const { return _ctx == nullptr; }
        void Start(SSEContext& ctx, std::span<const char> const& msg)
        {
            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
                ctx.res.body().data = nullptr;
                ctx.res.body().size = 0;
                ctx.res.body().more = true;
                ctx.res.set(boost::beast::http::field::transfer_encoding, "chunked");
                boost::beast::http::write_header(ctx.stream, ctx.sr);
                _ctx = &ctx;

                Send(msg);
            }
            do {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                if (_manager->_stopRequested) return;
                _dataAvailable.wait(lock, [&](...) { return _streamEnded() || _manager->_stopRequested; });
                if (_manager->_stopRequested) return;
                if (_streamEnded())
                {
                    _manager->Remove(lock, shared_from_this());
                    // TODO : _stream.Close() ?
                    return;
                }
            } while (true);
        }

        void Release()
        {
            auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
            _manager->Remove(lock, shared_from_this());
            _ctx = nullptr;
        }

        void Send(std::span<const char> const& msg)
        {
            if (msg.size() == 0) return;
            auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();
            if (msgSize == 0) return;
            boost::asio::const_buffer b{msg.data(), msgSize};
            boost::beast::net::write(_ctx->stream, boost::beast::http::make_chunk(b));
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
                inst->Send(msg);
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

template <typename TImpl, ConceptInterface TInterface> struct WebRequestContext
{
    using Interface = TInterface;
    using Impl      = TImpl;
    impl::SSEListenerManager& sse;
    TImpl&                    impl;

    tcp_stream&                           stream;
    Request const&                        req;
    Response&                             res;
    boost::urls::url_view&                url;
    boost::urls::segments_base::iterator& url_seg_it;

    std::ostringstream rslt;
};

template <typename TContext, typename TEventStructs> struct RequestHandlerForEvents
{
    static bool Matches(TContext& ctx) { return impl::iequal(Stencil::InterfaceApiTraits<TEventStructs>::Name(), *ctx.url_seg_it); }
    static auto Invoke(TContext& ctx)
    {
        auto instance = ctx.sse.CreateInstance();
        boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields> sr{ctx.res};

        SSEListenerManager::Instance::SSEContext ctx1{ctx.stream, ctx.res, sr};

        // ctx1.res    = ctx.res;
        // ctx1.sr     = ctx.sr;
        // ctx1.stream = ctx.stream;

        instance->Start(ctx1, "event: init\ndata: \n\n");
    }
};

template <typename TContext, typename TObjectStoreObj> struct RequestHandlerForObjectStore
{
    static bool Matches(TContext& ctx) { return impl::iequal(Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name(), *ctx.url_seg_it); }

    template <typename TLambda> static auto _ForeachObjId(TContext& ctx, TLambda&& lambda)
    {
        auto subpath = *(++ctx.url_seg_it);
        if (subpath.empty())
        {
            auto it = ctx.req.find("ids");
            if (it == ctx.req.end()) { throw std::invalid_argument("No Id specified for read"); }
            auto ids = it->value();
            ctx.rslt << '{';
            bool   first  = true;
            size_t sindex = 0;
            do {
                auto eindex = ids.find(',', sindex);
                if (eindex == std::string_view::npos) eindex = ids.size();
                auto idstr = ids.substr(sindex, eindex - sindex);
                if (!first) { ctx.rslt << ','; }
                ctx.rslt << idstr;
                first       = false;
                uint32_t id = static_cast<uint32_t>(std::stoul(idstr));
                lambda(id);
                sindex = eindex + 1;
            } while (sindex < ids.size());
            ctx.rslt << '}';
        }
        else
        {
            uint32_t id = static_cast<uint32_t>(std::stoul(subpath));
            lambda(id);
        }
        return ctx.rslt.str();
    }

    template <typename TArgsStruct> static auto _CreateArgStruct(TContext& ctx)
    {
        TArgsStruct args{};

        for (auto const& param : ctx.url.params())
        {
            using TKey = typename Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
            TKey key{};
            Stencil::SerDesRead<Stencil::ProtocolString>(key, param.key);
            auto& jsonval = param.value;    // Clang complains about capturing localbinding variables
            Visitor<TArgsStruct>::VisitKey(args, key, [&](auto& val) { Stencil::SerDesRead<Stencil::ProtocolJsonVal>(val, jsonval); });
        }
        return args;
    }

    static auto Invoke(TContext& ctx)
    {
        // auto [action, subpath] = Split(path);
        auto action = *(++ctx.url_seg_it);
        Handle(ctx, action);
        auto msg            = ctx.rslt.str();
        ctx.res.body().data = msg.data();
        ctx.res.body().size = msg.size();
        ctx.res.body().more = false;
        boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields> sr{ctx.res};
        boost::beast::http::write(ctx.stream, sr);
    }

    static auto Handle(TContext& ctx, std::string_view action)
    {
        auto& ifobj   = ctx.impl.template GetInterface<typename TContext::Interface>();
        auto& objects = ifobj.objects;
        if (action == "create")
        {
            auto lock       = objects.LockForEdit();
            auto [id, obj1] = objects.template Create<TObjectStoreObj>(lock, _CreateArgStruct<TObjectStoreObj>(ctx));
            uint32_t idint  = id.id;
            fmt::print(ctx.rslt, "{}", Stencil::Json::Stringify(idint));
            ctx.sse.Send(fmt::format("event: objectstore_create\ndata: {{\"{}\": {{\"{}\": {}}}}}\n\n",
                                     Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name(),
                                     idint,
                                     Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, id, obj1))));
        }
        else if (action == "all")
        {
            auto lock = objects.LockForRead();
            ctx.rslt << '{';
            bool first = true;
            for (auto const& [ref, obj1] : objects.template Items<TObjectStoreObj>(lock))
            {
                if (!first) { ctx.rslt << ','; }
                ctx.rslt << '\"' << ref.id << '\"' << ':'
                         << Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, ref, obj1));
                first = false;
            }
            ctx.rslt << '}';
        }
        else if (action == "read")
        {
            auto lock = objects.LockForRead();
            _ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                ctx.rslt << jsobj;
            });
        }
        else if (action == "edit")
        {
            ctx.sse.Send(
                fmt::format("event: objectstore_edit\ndata: {{\'{}\': {{", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            _ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                ctx.rslt << jsobj;
                ctx.sse.Send(fmt::format("{}\'{}\': {}", (first ? ' ' : ','), id, jsobj));
                first = false;
            });
            ctx.sse.Send("}}\n\n");
        }
        else if (action == "delete")
        {
            ctx.sse.Send(
                fmt::format("event: objectstore_delete\ndata: {{\'{}\': [", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            _ForeachObjId(ctx, [&](uint32_t id) {
                try
                {
                    objects.template Delete<TObjectStoreObj>(lock, {id});
                    ctx.rslt << "true";
                    ctx.sse.Send(fmt::format("{}{}", (first ? ' ' : ','), id));
                    first = false;
                } catch (std::exception const& /*ex*/)
                {
                    ctx.rslt << "false";
                }
            });
            ctx.sse.Send("]}\n\n");
        }
        else { throw std::logic_error("Not implemented"); }
    }
};

template <typename TContext, typename TArgsStruct> struct RequestHandlerForFunctions
{
    static bool Matches(TContext& ctx) { return impl::iequal(Stencil::InterfaceApiTraits<TArgsStruct>::Name(), *ctx.url_seg_it); }
    static auto _CreateArgStruct(TContext& ctx)
    {
        TArgsStruct args{};

        for (auto const& param : ctx.url.params())
        {
            using TKey = typename Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
            TKey key{};
            Stencil::SerDesRead<Stencil::ProtocolString>(key, param.key);
            auto& jsonval = param.value;    // Clang complains about capturing localbinding variables
            Visitor<TArgsStruct>::VisitKey(args, key, [&](auto& val) { Stencil::SerDesRead<Stencil::ProtocolJsonVal>(val, jsonval); });
        }
        return args;
    }

    static auto Invoke(TContext& ctx)
    {
        using Traits = ::Stencil::InterfaceApiTraits<TArgsStruct>;
        auto args    = _CreateArgStruct(ctx);
        if constexpr (std::is_same_v<void, decltype(Traits::Invoke(ctx.impl, args))>) { return Traits::Invoke(ctx, ctx.impl, args); }
        else
        {
            auto retval = Traits::Invoke(ctx.impl, args);
            ctx.rslt << Stencil::Json::Stringify<decltype(retval)>(retval);
        }
        auto msg            = ctx.rslt.str();
        ctx.res.body().data = msg.data();
        ctx.res.body().size = msg.size();
        ctx.res.body().more = false;
        boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields> sr{ctx.res};
        boost::beast::http::write(ctx.stream, sr);
    }
};

template <typename TContext> struct RequestHandlerFallback
{
    static bool Matches(TContext& /* ctx */) { return true; }
    static auto Invoke(TContext& ctx)
    {
        if (!ctx.impl.HandleRequest(ctx.stream, ctx.req, ctx.res)) throw std::runtime_error("url not found");
        /*auto instance = ctx.sse.CreateInstance();
        // auto instance = ctx.sse.CreateInstance();

        // https://github.com/pgit/cppcoro-devcontainer/blob/master/http_echo_awaitable.cpp
        // https://github.com/fantasy-peak/cpp-freegpt-webui/blob/main/src/main.cpp#L241
        // https://github.com/chimaoshu/CatPlusPlus/blob/main/src/http.cpp#L230

        // (no co_await )
        // https://github.com/jamestiotio/PhotonLibOS/blob/main/net/http/server.cpp#L321

        // (sse)
        // https://github.com/openbmc/bmcweb/blob/master/http/server_sent_event.hpp#L95
        // https://github.com/jgaa/mobile-events/blob/main/eventsd/lib/HttpServer.cpp

        ctx.res.result(boost::beast::http::status::not_found);
        ctx.res.set(boost::beast::http::field::content_type, "text/html");
        ctx.res.body().data = nullptr;
        ctx.res.body().more = false;

        boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields> sr{ctx.res};
        boost::beast::http::write(ctx.stream, sr);
        */
    }
};

template <typename TImpl, typename TInterface> struct RequestHandler
{
    template <typename TTup>
    static bool Matches(SSEListenerManager& /* sseMgr */,
                        TTup& /* impls */,
                        tcp_stream& /* stream */,
                        Request const& /* req */,
                        Response& /* res */,
                        boost::urls::url_view& /*url*/,
                        boost::urls::segments_base::iterator& it)
    {
        return iequal(Stencil::InterfaceTraits<TInterface>::Name(), *it);
    }

    template <typename T1> struct EventTransform;
    template <typename... T1s> struct EventTransform<std::tuple<T1s...>>
    {
        using Handler = std::tuple<RequestHandlerForEvents<WebRequestContext<TImpl, TInterface>, T1s>...>;
    };

    template <typename T1> struct ApiStructTransform;
    template <typename... T1s> struct ApiStructTransform<std::tuple<T1s...>>
    {
        using Handler = std::tuple<RequestHandlerForFunctions<WebRequestContext<TImpl, TInterface>, T1s>...>;
    };

    template <typename T1> struct ObjectStoreTransform;
    template <typename... T1s> struct ObjectStoreTransform<std::tuple<T1s...>>
    {
        using Handler = std::tuple<RequestHandlerForObjectStore<WebRequestContext<TImpl, TInterface>, T1s>...>;
    };

    template <typename T1> struct SelectorTransform;
    template <typename... T1s> struct SelectorTransform<std::tuple<T1s...>>
    {
        using SelectorT = Selector<T1s...>;
    };

    template <typename... Tuples> using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

    template <typename TContext> struct RequestHandlerForObjectStoreListener
    {
        static bool Matches(TContext& ctx) { return impl::iequal(*ctx.url_seg_it, std::string_view("objectstore")); }

        static auto Invoke(TContext& ctx)
        {
            auto instance = ctx.sse.CreateInstance();
            boost::beast::http::response_serializer<boost::beast::http::buffer_body, boost::beast::http::fields> sr{ctx.res};

            SSEListenerManager::Instance::SSEContext ctx1{ctx.stream, ctx.res, sr};
            ctx.rslt << '[';

            auto it    = ctx.url.params().find("query");
            bool first = true;
            if (it != ctx.url.params().end())
            {
                std::string_view query = (*it).value;
                do {
                    ctx.rslt << (first ? ' ' : ',');
                    first                    = false;
                    auto [query1, remaining] = Split(query, ',');
                    // auto [qifname, qsubpath] = Split(query1);
                    ctx.url         = query1;
                    ctx.url_seg_it  = ctx.url.segments().begin();
                    using TypesT    = typename ObjectStoreTransform<typename Stencil::InterfaceTraits<TInterface>::Objects>::Handler;
                    using SelectorT = typename SelectorTransform<TypesT>::SelectorT;
                    SelectorT::Invoke(ctx);
                    query = remaining;
                } while (!query.empty());
            }
            ctx.rslt << ']';
            auto rsltstr = ctx.rslt.str();
            instance->Start(ctx1, fmt::format("event: init\ndata: {}\n\n", rsltstr));
        }
    };
    template <typename TImpl>
    static void Invoke(SSEListenerManager&                   sseMgr,
                       TImpl&                                impl,
                       tcp_stream&                           stream,
                       Request const&                        req,
                       Response&                             res,
                       boost::urls::url_view&                url,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        WebRequestContext<TImpl, TInterface> ctx{sseMgr, impl, stream, req, res, url, it, {}};
        using RequestHandlerForEventsT    = typename EventTransform<typename Stencil::InterfaceTraits<TInterface>::EventStructs>::Handler;
        using RequestHandlerForFunctionsT = typename ApiStructTransform<typename Stencil::InterfaceTraits<TInterface>::ApiStructs>::Handler;
        using RequestHandlerForObjectStoreT =
            typename ObjectStoreTransform<typename Stencil::InterfaceTraits<TInterface>::Objects>::Handler;

        ctx.res.set(boost::beast::http::field::server, Stencil::InterfaceTraits<TInterface>::Name());

        using TypesT    = tuple_cat_t<RequestHandlerForEventsT,
                                      RequestHandlerForObjectStoreT,
                                      std::tuple<RequestHandlerForObjectStoreListener<WebRequestContext<TImpl, TInterface>>>,
                                      RequestHandlerForFunctionsT,
                                      std::tuple<RequestHandlerFallback<WebRequestContext<TImpl, TInterface>>>>;
        using SelectorT = typename SelectorTransform<TypesT>::SelectorT;
        return SelectorT::Invoke(ctx);
    }
};

}    // namespace Stencil::impl

namespace Stencil
{
SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4626)
SUPPRESS_MSVC_WARNING(5027)
SUPPRESS_MSVC_WARNING(4623)
SUPPRESS_MSVC_WARNING(4625)
SUPPRESS_MSVC_WARNING(4583)
SUPPRESS_MSVC_WARNING(4582)
SUPPRESS_MSVC_WARNING(4702)

template <typename TImpl, ConceptInterface TInterface> struct WebServiceInterfaceImplT : TInterface
{
    WebServiceInterfaceImplT()          = default;
    virtual ~WebServiceInterfaceImplT() = default;
    CLASS_DELETE_COPY_AND_MOVE(WebServiceInterfaceImplT);
};

template <typename TImpl, ConceptInterface... TInterfaces>
struct WebServiceT : public WebServiceInterfaceImplT<TImpl, TInterfaces>...,
                     public Stencil::impl::Interface::InterfaceEventHandlerT<TImpl, TInterfaces>...
{
    using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
    using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
        boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;

    using Request  = impl::Request;
    using Response = impl::Response;
    WebServiceT()  = default;
    virtual ~WebServiceT() { StopDaemon(); }

    CLASS_DELETE_COPY_AND_MOVE(WebServiceT);

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
                    fmt::print(stderr, "Error in acceptor: {}\n", e.what());
                }
        });

        for (size_t i = 0; i < 4; i++)
        {
            _listenthreads.emplace_back([this]() { ioc.run(); });
        }
    }

    void StopDaemon()
    {
        _sseManager.Stop();
        ioc.stop();
        WaitForStop();
    }

    void WaitForStop()
    {
        for (auto& thrd : _listenthreads)
            if (thrd.joinable()) thrd.join();
    }

    template <typename TEventArgs> void OnEvent(TEventArgs const& args)
    {
        auto msg = fmt::format("event: {}\ndata: {}\n\n", Stencil::InterfaceApiTraits<TEventArgs>::Name(), Stencil::Json::Stringify(args));
        _sseManager.Send(msg);
    }

    template <ConceptInterface T> auto& GetInterface() { return *static_cast<T*>(this); }

    virtual bool HandleRequest(tcp_stream& /* stream */, Request const& /* req */, Response& /* res */) { return false; }

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
    auto _handle_request(tcp_stream& stream, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req)
    {
        Response res{};
        res.result(boost::beast::http::status::ok);
        res.version(req.version());
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "application/json");

        res.keep_alive(req.keep_alive());

        // Respond to HEAD request
        if (req.method() == boost::beast::http::verb::head) { throw std::runtime_error("Not implemented"); }
        {
            auto target = req.target();
            auto url    = boost::urls::parse_origin_form(target).value();
            auto segs   = url.segments();

            auto it = segs.begin();
            if (it == segs.end() || *it != "api")
            {
                if (!HandleRequest(stream, req, res)) throw std::runtime_error("Not found");
            }
            ++it;
            auto& impl = *static_cast<TImpl*>(this);
            return impl::Selector<impl::RequestHandler<TImpl, TInterfaces>...>::Invoke(_sseManager, impl, stream, req, res, url, it);
        }
    }

    // Handles an HTTP server connection
    boost::asio::awaitable<void> _do_session(boost::asio::ip::tcp::socket&& socket)
    {
        tcp_stream stream(std::move(socket));
        // This buffer is required to persist across reads
        boost::beast::flat_buffer buffer;

        // This lambda is used to send messages
        try
        {
            //            for (;;)
            //            {
            // Set the timeout.
            stream.expires_after(std::chrono::seconds(30000));

            // Read a request
            boost::beast::http::request<boost::beast::http::string_body> req;
            co_await boost::beast::http::async_read(stream, buffer, req, boost::asio::use_awaitable);

            // Handle the request
            // boost::beast::http::message_generator msg =
            _handle_request(stream, std::move(req));
            //        }
        } catch (boost::system::system_error& se)
        {
            if (se.code() != boost::beast::http::error::end_of_stream) throw;
        }

        stream.socket().shutdown(tcp::socket::shutdown_send);

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
            boost::asio::co_spawn(acceptor.get_executor(), _do_session(co_await acceptor.async_accept()), [](std::exception_ptr e) {
                if (e) try
                    {
                        std::rethrow_exception(e);
                    } catch (std::exception& e)
                    {
                        fmt::print(stderr, "Error in session:  {}\n", e.what());
                    }
            });
    }

    boost::asio::io_context ioc{8};

    std::vector<std::thread> _listenthreads;
    std::condition_variable  _cond;
    std::mutex               _mutex;

    int                      _port;
    impl::SSEListenerManager _sseManager;
};
SUPPRESS_WARNINGS_END
}    // namespace Stencil
