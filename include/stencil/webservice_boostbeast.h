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

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <condition_variable>
#include <cstdlib>
#include <filesystem>
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
using Request = boost::beast::http::request<boost::beast::http::string_body>;

template <typename T> auto create_response(Request const& req, std::string_view const& content_type)
{
    boost::beast::http::response<T, boost::beast::http::fields> res;
    res.result(boost::beast::http::status::ok);
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, content_type);
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::server, "stencil_webserver");
    return res;
}
using boost::beast::iequals;

using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
    boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;

inline std::tuple<std::string_view, std::string_view> Split(std::string_view const& path, char token = '/')
{
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
        if constexpr (sizeof...(Types) == 0) {}
        else
        {
            auto result = (InvokeIfMatch<Types>(std::forward<TArgs>(args)...) || ...);
            if (result) return;
        }
        throw std::logic_error("Unexpected error. Unreachable code encountered. Did not match any selector");
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
            SSEContext(tcp_stream& streamIn, Request const& reqIn) : stream(streamIn)
            {
                auto res = impl::create_response<boost::beast::http::buffer_body>(reqIn, "text/event-stream");
                boost::beast::http::response_serializer<boost::beast::http::buffer_body> sr(res);
                res.set(boost::beast::http::field::transfer_encoding, "chunked");
                res.body().data = nullptr;
                res.body().size = 0;
                res.body().more = true;
                boost::beast::http::write_header(stream, sr);
            }

            tcp_stream& stream;
        };

        SSEListenerManager*     _manager{nullptr};
        SSEContext*             _ctx{nullptr};
        std::condition_variable _dataAvailable{};
        bool                    _stopRequested = false;

        Instance()  = default;
        ~Instance() = default;
        CLASS_DELETE_COPY_AND_MOVE(Instance);

        bool _streamEnded() const { return _ctx == nullptr; }
        void Start(SSEContext& ctx, std::span<const char> const& msg)
        {
            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
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

    impl::SSEListenerManager&             sse;
    TImpl&                                impl;
    tcp_stream&                           stream;
    Request const&                        req;
    boost::urls::url_view&                url;
    boost::urls::segments_base::iterator& url_seg_it;
};

template <typename TContext, typename TEventStructs> struct RequestHandlerForEvents
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TEventStructs>::Name(), *ctx.url_seg_it); }
    static auto Invoke(TContext& ctx)
    {
        SSEListenerManager::Instance::SSEContext ctx1(ctx.stream, ctx.req);
        ctx.sse.CreateInstance()->Start(ctx1, "event: init\ndata: \n\n");
    }
};

template <typename TContext, typename TObjectStoreObj> struct RequestHandlerForObjectStore
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name(), *ctx.url_seg_it); }

    template <typename TLambda> static auto _ForeachObjId(TContext& ctx, TLambda&& lambda)
    {
        std::ostringstream rslt;

        auto subpath = *(++ctx.url_seg_it);
        if (subpath.empty())
        {
            auto it = ctx.req.find("ids");
            if (it == ctx.req.end())
            {
                throw std::invalid_argument(
                    fmt::format("Missing Param: \"ids\" for object-store request: {}", std::string_view(ctx.req.target())));
            }
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
            uint32_t id = static_cast<uint32_t>(std::stoul(subpath));
            lambda(id);
        }
        return rslt.str();
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
        auto action = *(++ctx.url_seg_it);
        auto msg    = Handle(ctx, action);
        auto res    = impl::create_response<boost::beast::http::string_body>(ctx.req, "application/json");
        res.body()  = msg;
        boost::beast::http::response_serializer<boost::beast::http::string_body, boost::beast::http::fields> sr{res};
        boost::beast::http::write(ctx.stream, sr);
    }

    static auto Handle(TContext& ctx, std::string_view action)
    {
        std::ostringstream rslt;

        auto& ifobj   = ctx.impl.template GetInterface<typename TContext::Interface>();
        auto& objects = ifobj.objects;
        if (action == "create")
        {
            auto lock       = objects.LockForEdit();
            auto [id, obj1] = objects.template Create<TObjectStoreObj>(lock, _CreateArgStruct<TObjectStoreObj>(ctx));
            uint32_t idint  = id.id;
            fmt::print(rslt, "{}", Stencil::Json::Stringify(idint));
            ctx.sse.Send(fmt::format("event: objectstore_create\ndata: {{\"{}\": {{\"{}\": {}}}}}\n\n",
                                     Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name(),
                                     idint,
                                     Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, id, obj1))));
        }
        else if (action == "all")
        {
            auto lock = objects.LockForRead();
            rslt << '{';
            bool first = true;
            for (auto const& [ref, obj1] : objects.template Items<TObjectStoreObj>(lock))
            {
                if (!first) { rslt << ','; }
                rslt << '\"' << ref.id << '\"' << ':'
                     << Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, ref, obj1));
                first = false;
            }
            rslt << '}';
        }
        else if (action == "read")
        {
            auto lock = objects.LockForRead();
            _ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                rslt << jsobj;
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
                rslt << jsobj;
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
                    rslt << "true";
                    ctx.sse.Send(fmt::format("{}{}", (first ? ' ' : ','), id));
                    first = false;
                } catch (std::exception const& /*ex*/)
                {
                    rslt << "false";
                }
            });
            ctx.sse.Send("]}\n\n");
        }
        else { throw std::invalid_argument(fmt::format("Unknown object store action: {}", action)); }
        return rslt.str();
    }
};

template <typename TContext, typename TArgsStruct> struct RequestHandlerForFunctions
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TArgsStruct>::Name(), *ctx.url_seg_it); }
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
        std::ostringstream rslt;

        auto args = _CreateArgStruct(ctx);
        if constexpr (std::is_same_v<void, decltype(Traits::Invoke(ctx.impl, args))>) { return Traits::Invoke(ctx, ctx.impl, args); }
        else
        {
            auto retval = Traits::Invoke(ctx.impl, args);
            rslt << Stencil::Json::Stringify<decltype(retval)>(retval);
        }
        auto msg   = rslt.str();
        auto res   = impl::create_response<boost::beast::http::string_body>(ctx.req, "application/json");
        res.body() = msg;
        boost::beast::http::response_serializer<boost::beast::http::string_body, boost::beast::http::fields> sr{res};
        boost::beast::http::write(ctx.stream, sr);
    }
};

template <typename TContext> struct RequestHandlerFallback
{
    static bool Matches(TContext& /* ctx */) { return true; }
    static auto Invoke(TContext& ctx)
    {
        if (!ctx.impl.HandleRequest(ctx.stream, ctx.req))
        {
            throw std::invalid_argument(fmt::format("Cannot determine handler for api : {}", std::string_view(ctx.req.target())));
        }
    }
};

template <typename TImpl, typename TInterface> struct RequestHandler
{
    template <typename TTup>
    static bool Matches(SSEListenerManager& /* sseMgr */,
                        TTup& /* impls */,
                        tcp_stream& /* stream */,
                        Request const& /* req */,
                        boost::urls::url_view& /*url*/,
                        boost::urls::segments_base::iterator& it)
    {
        return iequals(Stencil::InterfaceTraits<TInterface>::Name(), *it);
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
        static bool Matches(TContext& ctx) { return impl::iequals(*ctx.url_seg_it, std::string_view("objectstore")); }

        static auto Invoke(TContext& ctx)
        {
            SSEListenerManager::Instance::SSEContext ctx1(ctx.stream, ctx.req);
            std::ostringstream                       rslt;

            rslt << '[';

            auto it    = ctx.url.params().find("query");
            bool first = true;
            if (it != ctx.url.params().end())
            {
                std::string_view query = (*it).value;
                while (!query.empty())
                {
                    rslt << (first ? ' ' : ',');
                    first                    = false;
                    auto [query1, remaining] = Split(query, ',');
                    ctx.url                  = query1;
                    ctx.url_seg_it           = ctx.url.segments().begin();
                    using TypesT    = typename ObjectStoreTransform<typename Stencil::InterfaceTraits<TInterface>::Objects>::Handler;
                    using SelectorT = typename SelectorTransform<TypesT>::SelectorT;
                    SelectorT::Invoke(ctx);
                    query = remaining;
                }
            }
            rslt << ']';
            auto rsltstr = rslt.str();
            ctx.sse.CreateInstance()->Start(ctx1, fmt::format("event: init\ndata: {}\n\n", rsltstr));
        }
    };

    static void Invoke(SSEListenerManager&                   sseMgr,
                       TImpl&                                impl,
                       tcp_stream&                           stream,
                       Request const&                        req,
                       boost::urls::url_view&                url,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        WebRequestContext<TImpl, TInterface> ctx{sseMgr, impl, stream, req, url, it};
        using RequestHandlerForEventsT    = typename EventTransform<typename Stencil::InterfaceTraits<TInterface>::EventStructs>::Handler;
        using RequestHandlerForFunctionsT = typename ApiStructTransform<typename Stencil::InterfaceTraits<TInterface>::ApiStructs>::Handler;
        using RequestHandlerForObjectStoreT =
            typename ObjectStoreTransform<typename Stencil::InterfaceTraits<TInterface>::Objects>::Handler;

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

template <typename TImpl, ConceptInterface TInterface>
struct WebServiceInterfaceImplT : TInterface,    // TImpl implements the virtual functions in this interface,
                                  Stencil::impl::Interface::InterfaceEventHandlerT<TImpl, TInterface>
{
    WebServiceInterfaceImplT() { TInterface::template SetHandler(this); }
    virtual ~WebServiceInterfaceImplT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(WebServiceInterfaceImplT);
};

template <typename TImpl, ConceptInterface... TInterfaces> struct WebServiceT : public WebServiceInterfaceImplT<TImpl, TInterfaces>...
{
    using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
    using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
        boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;
    using Field                                    = boost::beast::http::field;
    template <typename T> using Response           = boost::beast::http::response<T>;
    template <typename T> using ResponseSerializer = boost::beast::http::response_serializer<T>;

    using Request = impl::Request;

    WebServiceT() = default;
    virtual ~WebServiceT() override { StopDaemon(); }

    CLASS_DELETE_COPY_AND_MOVE(WebServiceT);

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

    void WriteStringResponse(tcp_stream& stream, Request const& req, std::string_view const& content_type, std::string_view const& content)
    {
        auto res   = impl::create_response<boost::beast::http::string_body>(req, content_type);
        res.body() = content;
        ResponseSerializer<boost::beast::http::string_body> sr(res);
        boost::beast::http::write(stream, sr);
    }

    void WriteFileResponse(tcp_stream& stream, Request const& req, std::filesystem::path const& path)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::invalid_argument(fmt::format("Cannot send File Response. File does not exist: {}", path.string()));
        }

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) throw std::runtime_error(fmt::format("Cannot send File Response. Failed to open file: {}", path.string()));
        std::vector<uint8_t> buffer(std::filesystem::file_size(path));
        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        auto res        = impl::create_response<boost::beast::http::buffer_body>(req, mime_type(path.extension().string()));
        res.body().data = buffer.data();
        res.body().size = buffer.size();
        res.body().more = false;
        boost::beast::http::response_serializer<boost::beast::http::buffer_body> sr{res};
        boost::beast::http::write(stream, sr);
    }

    virtual bool HandleRequest(tcp_stream& /* stream */, Request const& /* req */) { return false; }

    // Return a reasonable mime type based on the extension of a file.
    static boost::beast::string_view mime_type(boost::beast::string_view path)
    {
        auto const ext = [&path] {
            auto const pos = path.rfind(".");
            if (pos == boost::beast::string_view::npos) return boost::beast::string_view{};
            return path.substr(pos);
        }();
        if (impl::iequals(ext, ".htm")) return "text/html";
        if (impl::iequals(ext, ".html")) return "text/html";
        if (impl::iequals(ext, ".php")) return "text/html";
        if (impl::iequals(ext, ".css")) return "text/css";
        if (impl::iequals(ext, ".txt")) return "text/plain";
        if (impl::iequals(ext, ".js")) return "application/javascript";
        if (impl::iequals(ext, ".json")) return "application/json";
        if (impl::iequals(ext, ".xml")) return "application/xml";
        if (impl::iequals(ext, ".swf")) return "application/x-shockwave-flash";
        if (impl::iequals(ext, ".flv")) return "video/x-flv";
        if (impl::iequals(ext, ".png")) return "image/png";
        if (impl::iequals(ext, ".jpe")) return "image/jpeg";
        if (impl::iequals(ext, ".jpeg")) return "image/jpeg";
        if (impl::iequals(ext, ".jpg")) return "image/jpeg";
        if (impl::iequals(ext, ".gif")) return "image/gif";
        if (impl::iequals(ext, ".bmp")) return "image/bmp";
        if (impl::iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
        if (impl::iequals(ext, ".tiff")) return "image/tiff";
        if (impl::iequals(ext, ".tif")) return "image/tiff";
        if (impl::iequals(ext, ".svg")) return "image/svg+xml";
        if (impl::iequals(ext, ".svgz")) return "image/svg+xml";
        return "application/text";
    }

    private:
    // Return a response for the given request.
    //
    // The concrete type of the response message (which depends on the
    // request), is type-erased in message_generator.
    template <class Body, class Allocator>
    auto _handle_request(tcp_stream& stream, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req)
    {
        // Respond to HEAD request
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wswitch-enum")
        SUPPRESS_MSVC_WARNING(4061)    // swtich enum not handled
        switch (req.method())
        {
        case boost::beast::http::verb::get: [[fallthrough]];
        case boost::beast::http::verb::put:
        {
            auto target = req.target();
            auto url    = boost::urls::parse_origin_form(target).value();
            auto segs   = url.segments();

            auto it = segs.begin();
            if (it == segs.end() || *it != "api")
            {
                if (!HandleRequest(stream, req))
                {
                    throw std::invalid_argument(fmt::format("Unable to fulfill request: {}. No Handler found", std::string_view(target)));
                }
                return;
            }
            ++it;
            auto& impl = *static_cast<TImpl*>(this);
            return impl::Selector<impl::RequestHandler<TImpl, TInterfaces>...>::Invoke(_sseManager, impl, stream, req, url, it);
        }
        default:
            throw std::invalid_argument(
                fmt::format("Request Verb:{} Not implemented", std::string_view(boost::beast::http::to_string(req.method()))));
        }
        SUPPRESS_WARNINGS_END
    }

    // Handles an HTTP server connection
    boost::asio::awaitable<void> _do_session(boost::asio::ip::tcp::socket&& socket)
    {
        tcp_stream stream(std::move(socket));
        // This buffer is required to persist across reads
        boost::beast::flat_buffer buffer;
        try
        {
            stream.expires_after(std::chrono::seconds(30000));

            // Read a request
            boost::beast::http::request<boost::beast::http::string_body> req;
            co_await boost::beast::http::async_read(stream, buffer, req, boost::asio::use_awaitable);
            _handle_request(stream, std::move(req));
        } catch (boost::system::system_error& se)
        {
            if (se.code() != boost::beast::http::error::end_of_stream) throw;
        }

        stream.socket().shutdown(tcp::socket::shutdown_send);

        // At this point the connection is closed gracefully
        // we ignore the error because the client might have
        // dropped the connection already.
    }

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
                        fmt::print(stderr, "Session Terminated with Error:  {}\n", e.what());
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
