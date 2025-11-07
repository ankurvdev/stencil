#include "CommonMacros.h"

#if !defined HAVE_BOOSTBEAST
#error "Need boost::beast to be linked"
#endif

#include "database.h"
#include "interfaces.h"
#include "protocol_json.h"
#include "serdes.h"
#include "transactions.h"
#include "transactions.strserdes.h"
#include "typetraits.h"
#include "uuidobject.h"

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
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/url.hpp>

SUPPRESS_WARNINGS_END

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <chrono>
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
namespace Stencil::websvc::impl
{
template <typename TImpl, typename TSvc> struct RequestHandler;

using namespace std::chrono_literals;
using Request = boost::beast::http::request<boost::beast::http::string_body>;

using boost::beast::iequals;

inline std::tuple<std::string_view, std::string_view> Split(std::string_view const& path, char token = '/')
{
    size_t start = path[0] == token ? 1u : 0u;
    size_t index = path.find(token, start);
    auto   str1  = path.substr(start, index - start);
    if (index == path.npos) return {str1, {}};
    return {str1, path.substr(index)};
}

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
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wnrvo")
    return res;
    SUPPRESS_WARNINGS_END
}
}    // namespace Stencil::websvc::impl
namespace Stencil::websvc
{

using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
    boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;
using Field                                    = boost::beast::http::field;
template <typename T> using Response           = boost::beast::http::response<T>;
template <typename T> using ResponseSerializer = boost::beast::http::response_serializer<T>;

using Request = impl::Request;

inline void
WriteStringResponse(tcp_stream& stream, Request const& req, std::string_view const& content_type, std::string_view const& content)
{
    auto res   = impl::create_response<boost::beast::http::string_body>(req, content_type);
    res.body() = content;
    ResponseSerializer<boost::beast::http::string_body> sr(res);
    boost::beast::http::write(stream, sr);
}

inline void
WriteFileResponse(tcp_stream& stream, Request const& req, std::filesystem::path const& path, boost::beast::string_view const& content_type)
{
    if (!std::filesystem::exists(path))
    {
        throw std::invalid_argument(fmt::format("Cannot send File Response. File does not exist: {}", path.string()));
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error(fmt::format("Cannot send File Response. Failed to open file: {}", path.string()));
    boost::beast::http::response<boost::beast::http::file_body> res;
    boost::system::error_code                                   ec;
    auto                                                        pathstr = path.string();
    res.body().open(pathstr.c_str(), boost::beast::file_mode::scan, ec);
    res.result(boost::beast::http::status::ok);
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, content_type);
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::server, "stencil_webserver");
    res.content_length(res.body().size());

    boost::beast::http::response_serializer<boost::beast::http::file_body> sr{res};
    boost::beast::http::write_header(stream, sr);

    boost::beast::http::write(stream, sr);
}

inline void Redirect(tcp_stream& stream, Request const& req, std::string_view const& redirect_path)
{
    boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::temporary_redirect, req.version()};

    auto host = [&]() {
        if (req.count(boost::beast::http::field::location)) return req.at(boost::beast::http::field::location);
        if (req.count(boost::beast::http::field::host)) return req.at(boost::beast::http::field::host);
        throw std::runtime_error("Cannot determine host");
    }();
    auto redirect = fmt::format("http://{}{}", std::string_view(host), redirect_path);
    res.set(boost::beast::http::field::location, redirect);
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::server, "stencil_webserver");
    boost::beast::http::response_serializer<boost::beast::http::string_body> sr{res};
    boost::beast::http::write_header(stream, sr);
    boost::beast::http::write(stream, sr);
}
inline boost::beast::string_view mime_type(boost::beast::string_view path)
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
    if (impl::iequals(ext, ".kml")) return "application/vnd.google-earth.kml+xml";
    if (impl::iequals(ext, ".m3u8")) return "application/x-mpegURL";
    if (impl::iequals(ext, ".ts")) return "video/mp4";
    if (impl::iequals(ext, ".mp4")) return "video/mp4";
    if (impl::iequals(ext, ".m4s")) return "video/mp4";
    return "application/text";
}

}    // namespace Stencil::websvc
namespace Stencil::websvc::impl
{

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
    template <typename... TArgs> static auto Invoke([[maybe_unused]] TArgs&&... args)
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
        using time_point = Stencil::Timestamp;
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
            ~SSEContext() = default;
            CLASS_DELETE_COPY_AND_MOVE(SSEContext);

            tcp_stream& stream;
        };

        SSEListenerManager*     _manager{nullptr};
        SSEContext*             _ctx{nullptr};
        std::condition_variable _dataAvailable{};
        time_point              _lastSendAt;
        bool                    _stopRequested = false;
        size_t                  _category      = {0};

        Instance()  = default;
        ~Instance() = default;
        CLASS_DELETE_COPY_AND_MOVE(Instance);

        bool _streamEnded() const { return _ctx == nullptr; }
        void Start(SSEContext& ctx, std::span<char const> const& msg)
        {

            {
                auto lock = std::unique_lock<std::mutex>(_manager->_mutex);
                _manager->Register(lock, shared_from_this());
                if (_manager->_stopRequested) return;
                _ctx = &ctx;
                Send(lock, msg);
            }
            do
            {
                auto constexpr KeepAliveInterval = 10s;
                auto lock                        = std::unique_lock<std::mutex>(_manager->_mutex);
                if (_manager->_stopRequested) return;
                auto status = _dataAvailable.wait_for(lock, KeepAliveInterval);
                if (status == std::cv_status::timeout && ((_lastSendAt + KeepAliveInterval) < Stencil::Timestamp::clock::now()))
                {    // timed out
                    if (!Send(lock, "\n\n")) { return; }
                }
                if (_manager->_stopRequested) return;
                if (_streamEnded()) { return; }
            } while (true);
        }

        void Release(std::unique_lock<std::mutex> const& /* lock */)
        {
            _stopRequested = true;
            if (_ctx) _ctx->stream.close();
            _ctx = nullptr;
            _dataAvailable.notify_all();
        }

        bool Send(std::unique_lock<std::mutex> const& lock, std::span<char const> const& msg)
        {
            if (_streamEnded()) return false;
            if (msg.size() == 0) return true;
            _lastSendAt  = Stencil::Timestamp::clock::now();
            auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();
            if (msgSize == 0) return true;
            boost::asio::const_buffer b{msg.data(), msgSize};
            boost::system::error_code ec;
            boost::beast::net::write(_ctx->stream, boost::beast::http::make_chunk(b), ec);
            if (ec.failed())
            {
                Release(lock);
                return false;
            }
            return true;
        }
    };

    void Send(size_t category, std::span<char const> const& msg)
    {
        auto lock = std::unique_lock<std::mutex>(_mutex);
        for (auto it = _sseListeners.begin(); it != _sseListeners.end();)
        {
            if (_stopRequested) return;
            auto inst = *it;
            if (inst->_stopRequested)
            {
                ++it;
                continue;
            }
            if (inst->_category != 0 && category != 0 && inst->_category != category)
            {
                ++it;
                continue;
            }
            if (!inst->Send(lock, msg)) { it = _sseListeners.erase(it); }
            else
            {
                inst->_dataAvailable.notify_all();
                ++it;
            }
        }
    }

    std::shared_ptr<Instance> CreateInstance(size_t category)
    {
        auto inst       = std::make_shared<Instance>();
        inst->_manager  = this;
        inst->_category = category;
        return inst;
    }

    void Register(std::unique_lock<std::mutex> const& /* lock */, std::shared_ptr<Instance> instance) { _sseListeners.insert(instance); }

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

template <typename TContext> struct RequestHandlerForAllEvents
{
    static bool Matches(TContext& ctx) { return impl::iequals(*ctx.url_seg_it, "events"); }
    static auto Invoke(TContext& ctx)
    {
        SSEListenerManager::Instance::SSEContext ctx1(ctx.stream, ctx.req);
        ctx.sse.CreateInstance(typeid(TContext).hash_code())->Start(ctx1, "event: init\ndata: \n\n");
        ctx.impl.OnSSEInstanceEnded();
    }
};

template <typename TContext, typename TEventStructs> struct RequestHandlerForEvents
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TEventStructs>::Name(), *ctx.url_seg_it); }
    static auto Invoke(TContext& ctx)
    {
        SSEListenerManager::Instance::SSEContext ctx1(ctx.stream, ctx.req);
        ctx.sse.CreateInstance(typeid(TContext).hash_code())->Start(ctx1, "event: init\ndata: \n\n");
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
            do
            {
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
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return args;
        SUPPRESS_WARNINGS_END
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
            ctx.sse.Send(typeid(TContext).hash_code(),
                         fmt::format("event: objectstore_create\ndata: {{\"{}\": {{\"{}\": {}}}}}\n\n",
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
                typeid(TContext).hash_code(),
                fmt::format("event: objectstore_edit\ndata: {{\'{}\': {{", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            _ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                rslt << jsobj;
                ctx.sse.Send(typeid(TContext).hash_code(), fmt::format("{}\'{}\': {}", (first ? ' ' : ','), id, jsobj));
                first = false;
            });
            ctx.sse.Send(typeid(TContext).hash_code(), "}}\n\n");
        }
        else if (action == "delete")
        {
            ctx.sse.Send(
                typeid(TContext).hash_code(),
                fmt::format("event: objectstore_delete\ndata: {{\'{}\': [", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            _ForeachObjId(ctx, [&](uint32_t id) {
                try
                {
                    objects.template Delete<TObjectStoreObj>(lock, {id});
                    rslt << "true";
                    ctx.sse.Send(typeid(TContext).hash_code(), fmt::format("{}{}", (first ? ' ' : ','), id));
                    first = false;
                } catch (std::exception const& /*ex*/) { rslt << "false"; }
            });
            ctx.sse.Send(typeid(TContext).hash_code(), "]}\n\n");
        }
        else
        {
            throw std::invalid_argument(fmt::format("Unknown object store action: {}", action));
        }
        return rslt.str();
    }
};

template <typename TContext, typename TArgsStruct> struct RequestHandlerForFunctions
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TArgsStruct>::Name(), *ctx.url_seg_it); }
    static auto _CreateArgStruct(TContext& ctx)
    {
        TArgsStruct args{};
        if (ctx.req.method() == boost::beast::http::verb::get)
        {
            for (auto const& param : ctx.url.params())
            {
                using TKey = typename Stencil::TypeTraitsForIndexable<TArgsStruct>::Key;
                TKey key{};
                Stencil::SerDesRead<Stencil::ProtocolString>(key, param.key);
                auto& jsonval = param.value;    // Clang complains about capturing localbinding variables
                Visitor<TArgsStruct>::VisitKey(args, key, [&](auto& val) { Stencil::SerDesRead<Stencil::ProtocolJsonVal>(val, jsonval); });
            }
            SUPPRESS_WARNINGS_START
            SUPPRESS_CLANG_WARNING("-Wnrvo")
            return args;
            SUPPRESS_WARNINGS_END
        }
        else if (ctx.req.method() == boost::beast::http::verb::put)
        {
            auto data = ctx.req.body();
            Stencil::SerDesRead<Stencil::ProtocolJsonVal>(args, data);
            SUPPRESS_WARNINGS_START
            SUPPRESS_CLANG_WARNING("-Wnrvo")
            return args;
            SUPPRESS_WARNINGS_END
        }
        else
        {
            throw std::runtime_error("Only get and put supported for functions");
        }
    }

    static auto Invoke(TContext& ctx)
    {
        using Traits = ::Stencil::InterfaceApiTraits<TArgsStruct>;
        std::ostringstream rslt;

        auto args = _CreateArgStruct(ctx);
        if constexpr (std::is_same_v<void, decltype(Traits::Invoke(ctx.impl, args))>)
        {
            Traits::Invoke(ctx.impl, args);
            auto res   = impl::create_response<boost::beast::http::string_body>(ctx.req, "application/json");
            res.body() = "{}";
            boost::beast::http::response_serializer<boost::beast::http::string_body, boost::beast::http::fields> sr{res};
            boost::beast::http::write(ctx.stream, sr);
        }
        else
        {
            auto retval = Traits::Invoke(ctx.impl, args);
            rslt << Stencil::Json::Stringify<decltype(retval)>(retval);
            auto msg   = rslt.str();
            auto res   = impl::create_response<boost::beast::http::string_body>(ctx.req, "application/json");
            res.body() = msg;
            boost::beast::http::response_serializer<boost::beast::http::string_body, boost::beast::http::fields> sr{res};
            boost::beast::http::write(ctx.stream, sr);
        }
    }
};

template <typename TContext> struct RequestHandlerFallback
{
    static bool Matches(TContext& /* ctx */) { return true; }
    static auto Invoke(TContext& ctx)
    {
        if (!ctx.impl.HandleRequest(ctx.stream, ctx.req, ctx.url))
        {
            throw std::invalid_argument(fmt::format("Cannot determine handler for api : {}", std::string_view(ctx.req.target())));
        }
    }
};

template <typename TImpl, ConceptInterface TInterface> struct RequestHandler<TImpl, TInterface>
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
                auto             obj   = *it;
                std::string_view query = obj.value;
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
            ctx.sse.CreateInstance(typeid(TContext).hash_code())->Start(ctx1, fmt::format("event: init\ndata: {}\n\n", rsltstr));
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

        using TypesT    = tuple_cat_t<std::tuple<RequestHandlerForAllEvents<WebRequestContext<TImpl, TInterface>>>,
                                      RequestHandlerForEventsT,
                                      RequestHandlerForObjectStoreT,
                                      std::tuple<RequestHandlerForObjectStoreListener<WebRequestContext<TImpl, TInterface>>>,
                                      RequestHandlerForFunctionsT,
                                      std::tuple<RequestHandlerFallback<WebRequestContext<TImpl, TInterface>>>>;
        using SelectorT = typename SelectorTransform<TypesT>::SelectorT;
        return SelectorT::Invoke(ctx);
    }
};

template <typename TInterfaceImpl> struct SessionInterface
{
    using Uuid = uuids::uuid;
    template <typename TImpl> auto CreateSession(TImpl& impl)
    {
        auto sptr                = std::make_shared<TInterfaceImpl>(impl);
        _sessions[sptr->id.uuid] = sptr;
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return sptr;
        SUPPRESS_WARNINGS_END
    }

    auto FindSession(Uuid const& uuid) { return _sessions[uuid]; }

    void EndSession(TInterfaceImpl* ptr) { _sessions.erase(ptr->id.uuid); }

    std::unordered_map<Uuid, std::shared_ptr<TInterfaceImpl>> _sessions;
};

template <typename TInterfaceImpl, ConceptInterface TInterface>
struct SessionInterfaceT : TInterface,
                           UuidObjectT<TInterfaceImpl>,
                           Stencil::impl::Interface::InterfaceEventHandlerT<TInterfaceImpl, TInterface>
{
    using Interface = TInterface;
    SessionInterfaceT() { TInterface::SetHandler(this); }
    ~SessionInterfaceT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(SessionInterfaceT);

    void* handler{nullptr};

    template <typename TEventArgs> void OnEvent(TEventArgs const& args)
    {
        auto msg = fmt::format("event: {}\ndata: {}\n\n", Stencil::InterfaceApiTraits<TEventArgs>::Name(), Stencil::Json::Stringify(args));
        _sseManager.Send(0, msg);
    }
    virtual bool HandleRequest(tcp_stream& /* stream */, Request const& /* req */, boost::urls::url_view const& /* url */) { return false; }
    virtual void OnSSEInstanceEnded() = 0;

    impl::SSEListenerManager _sseManager;
};

template <ConceptIndexable TState> struct SynchronizedState
{
    SynchronizedState()          = default;
    virtual ~SynchronizedState() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(SynchronizedState);

    void* handler{nullptr};

    virtual std::string_view Name()           = 0;
    virtual std::string      StateStringify() = 0;
};

template <typename TImpl, ConceptIndexable TState> struct RequestHandler<TImpl, SynchronizedState<TState>>
{
    static bool Matches(SSEListenerManager& /* sseMgr */,
                        TImpl& impl,
                        tcp_stream& /* stream */,
                        Request const& /* req */,
                        boost::urls::url_view& /*url*/,
                        boost::urls::segments_base::iterator& it)
    {
        return iequals(static_cast<SynchronizedState<TState>*>(&impl)->Name(), *it);
    }

    static void Invoke(SSEListenerManager& sseMgr,
                       TImpl&              impl,
                       tcp_stream&         stream,
                       Request const&      req,
                       boost::urls::url_view& /* url */,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        SSEListenerManager::Instance::SSEContext ctx1(stream, req);
        sseMgr.CreateInstance(typeid(TState).hash_code())
            ->Start(ctx1, fmt::format("event: init\ndata: {}\n\n", static_cast<SynchronizedState<TState>*>(&impl)->StateStringify()));
    }
};

template <typename TImpl, typename TInterfaceImpl> struct RequestHandler<TImpl, SessionInterface<TInterfaceImpl>>
{
    static bool Matches(SSEListenerManager& /* sseMgr */,
                        TImpl& /* impl */,
                        tcp_stream& /* stream */,
                        Request const& /* req */,
                        boost::urls::url_view& /*url*/,
                        boost::urls::segments_base::iterator& it)
    {
        return iequals("session", *it);
    }

    static void Invoke(SSEListenerManager& /* sseMgr */,
                       TImpl&                                impl,
                       tcp_stream&                           stream,
                       Request const&                        req,
                       boost::urls::url_view&                url,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        auto reqpath = url.path().substr(std::string_view("/api/session/920ca96a-1705-4635-a162-4f4686efd2ab").size());
        if (reqpath.size() > 0 && reqpath[0] == '/') reqpath = reqpath.substr(1);
        auto session = impl.FindSession(uuids::uuid::from_string(*it).value());
        if (session == nullptr)
        {
            session = impl.CreateSession(impl);
            Redirect(stream, req, fmt::format("{}/{}/{}", "/api/session", uuids::to_string(session->id.uuid), reqpath));
        }
        else
        {
            RequestHandler<TInterfaceImpl, typename TInterfaceImpl::Interface>::Invoke(
                session->_sseManager, *session, stream, req, url, it);
        }
    }
};

}    // namespace Stencil::websvc::impl

namespace Stencil::websvc
{
SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4626)
SUPPRESS_MSVC_WARNING(5027)
SUPPRESS_MSVC_WARNING(4623)
SUPPRESS_MSVC_WARNING(4625)
SUPPRESS_MSVC_WARNING(4583)
SUPPRESS_MSVC_WARNING(4582)
SUPPRESS_MSVC_WARNING(4702)

using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
    boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;
using Field                                    = boost::beast::http::field;
template <typename T> using Response           = boost::beast::http::response<T>;
template <typename T> using ResponseSerializer = boost::beast::http::response_serializer<T>;
using Request                                  = impl::Request;

template <typename TImpl, typename T> struct WebServiceInterfaceImplT;

template <typename TImpl, ConceptInterface TInterface>
struct WebServiceInterfaceImplT<TImpl, TInterface> : TInterface,    // TImpl implements the virtual functions in this interface,
                                                     Stencil::impl::Interface::InterfaceEventHandlerT<TImpl, TInterface>
{
    WebServiceInterfaceImplT() { TInterface::SetHandler(this); }
    virtual ~WebServiceInterfaceImplT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(WebServiceInterfaceImplT);
};

template <typename TImpl, ConceptIndexable T>
struct WebServiceInterfaceImplT<TImpl, impl::SynchronizedState<T>> : impl::SynchronizedState<T>
{
    WebServiceInterfaceImplT()           = default;
    ~WebServiceInterfaceImplT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(WebServiceInterfaceImplT);

    void NotifyStateChanged(Stencil::Transaction<T>::View const& txn)
    {
        auto impl = static_cast<TImpl*>(this);
        auto msg  = fmt::format("event: changed\ndata: {}\n\n", Stencil::StringTransactionSerDes::Deserialize(txn));
        impl->_sseManager.Send(typeid(T).hash_code(), msg);
    }
};

template <ConceptIndexable TState> using WebSynchronizedState = impl::SynchronizedState<TState>;

template <typename TInterfaceImpl, ConceptInterface TInterface>
using WebSessionInterfaceT = impl::SessionInterfaceT<TInterfaceImpl, TInterface>;

template <typename TInterfaceImpl> using WebSessionInterface = impl::SessionInterface<TInterfaceImpl>;

template <typename TImpl, typename TInterfaceImpl>
struct WebServiceInterfaceImplT<TImpl, WebSessionInterface<TInterfaceImpl>> : WebSessionInterface<TInterfaceImpl>
{
    WebServiceInterfaceImplT()          = default;
    virtual ~WebServiceInterfaceImplT() = default;
    CLASS_DELETE_COPY_AND_MOVE(WebServiceInterfaceImplT);
};

template <typename TImpl, typename... TServices> struct WebServiceT : public WebServiceInterfaceImplT<TImpl, TServices>...
{
    using WebService = WebServiceT<TImpl, TServices...>;

    WebServiceT() = default;
    virtual ~WebServiceT() override { StopDaemon(); }

    CLASS_DELETE_COPY_AND_MOVE(WebServiceT);

    void StartOnPort(uint16_t port, uint16_t numThreads = 4)
    {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        boost::asio::co_spawn(ioc, _do_listen(tcp::endpoint{address, port}), [](std::exception_ptr e) {
            if (e) try
                {
                    std::rethrow_exception(e);
                } catch (std::exception& e) { fmt::print(stderr, "Error in acceptor: {}\n", e.what()); }
        });

        for (size_t i = 0; i < numThreads; i++)
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
        _sseManager.Send(0, msg);
    }

    template <ConceptInterface T> auto& GetInterface() { return *static_cast<T*>(this); }

    virtual bool HandleRequest(tcp_stream& /* stream */, Stencil::websvc::Request const& /* req */, boost::urls::url_view const& /* url */)
    {
        return false;
    }

    // Return a reasonable mime type based on the extension of a file.

    // private: TODO: remove this when boost beast isnt experimental anymore
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
                if (!HandleRequest(stream, req, url))
                {
                    throw std::invalid_argument(fmt::format("Unable to fulfill request: {}. No Handler found", std::string_view(target)));
                }
                return;
            }
            ++it;
            auto& impl = *static_cast<TImpl*>(this);
            return impl::Selector<impl::RequestHandler<TImpl, TServices>...>::Invoke(_sseManager, impl, stream, req, url, it);
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
            [[maybe_unused]] auto data = co_await boost::beast::http::async_read(stream, buffer, req, boost::asio::use_awaitable);
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
                    } catch (std::exception& e) { fmt::print(stderr, "Session Terminated with Error:  {}\n", e.what()); }
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
}    // namespace Stencil::websvc
