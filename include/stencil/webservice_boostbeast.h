#pragma once
#include "CommonMacros.h"

#if !defined HAVE_BOOSTBEAST
#error "Need boost::beast to be linked"
#endif

#include "database.h"
#include "fmtboostbeast.h"
#include "interfaces.h"
#include "protocol_json.h"
#include "serdes.h"
#include "transactions.h"
#include "transactions.strserdes.h"
#include "typetraits.h"

#ifndef HAVE_SET_THREAD_NAME
#define SetThreadName(...)
#endif

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_FMT_WARNINGS
SUPPRESS_MSVC_WARNING(4242)
SUPPRESS_MSVC_WARNING(4702)
SUPPRESS_MSVC_WARNING(5219)
SUPPRESS_MSVC_WARNING(5262)    // implicit fall-through occurs here;

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/url.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/std.h>
SUPPRESS_WARNINGS_END

#include <chrono>
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
    if (index == std::string_view::npos) return {str1, {}};
    return {str1, path.substr(index)};
}

template <typename T> auto CreateResponse(Request const& req, std::string_view const& contentType)
{
    boost::beast::http::response<T, boost::beast::http::fields> res;
    res.result(boost::beast::http::status::ok);
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, contentType);
    res.set(boost::beast::http::field::access_control_allow_origin, "*");
    res.set(boost::beast::http::field::server, "stencil_webserver");
    return res;
}
}    // namespace Stencil::websvc::impl
namespace Stencil::websvc
{
template <typename TImpl, ConceptInterface TInterface> struct WebServiceImplTraits;

template <typename TImpl, ConceptInterface TInterface>
    requires std::is_base_of_v<TInterface, TImpl>
struct WebServiceImplTraits<TImpl, TInterface>
{
    static TInterface& QueryInterface(TImpl& impl) { return *static_cast<TInterface*>(&impl); }
};

using tcp        = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>
using tcp_stream = typename boost::beast::tcp_stream::rebind_executor<
    boost::asio::use_awaitable_t<>::executor_with_default<boost::asio::any_io_executor>>::other;
using Field                                    = boost::beast::http::field;
template <typename T> using Response           = boost::beast::http::response<T>;
template <typename T> using ResponseSerializer = boost::beast::http::response_serializer<T>;

using Request = impl::Request;
inline void TryCleanShutdown(tcp_stream& stream)
{
    boost::system::error_code ec;
    ec = stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (ec && ec != boost::system::errc::not_connected) { fmt::print(stderr, "Shutdown Error: {}\n", ec); }
}

inline void
WriteStringResponse(tcp_stream& stream, Request const& req, std::string_view const& contentType, std::string_view const& content)
{
    auto res   = impl::CreateResponse<boost::beast::http::string_body>(req, contentType);
    res.body() = content;
    ResponseSerializer<boost::beast::http::string_body> sr(res);
    boost::beast::http::write(stream, sr);
    TryCleanShutdown(stream);
}

template <typename TCallback>
inline void WriteFileResponse(tcp_stream&                      stream,    // NOLINT(readability-function-cognitive-complexity)
                              Request const&                   req,
                              std::filesystem::path const&     path,
                              boost::beast::string_view const& contentType,
                              TCallback const&                 customizeResponse)
{
    auto filePath = path;

    if (!std::filesystem::exists(filePath))
    {
        throw std::invalid_argument(fmt::format("Cannot send File Response. File does not exist: {}", filePath));
    }

    while (std::filesystem::is_symlink(filePath)) { filePath = std::filesystem::read_symlink(filePath); }
    filePath = std::filesystem::absolute(filePath);
    // Open file to get size
    auto const fileSize = static_cast<size_t>(std::filesystem::file_size(filePath));
    if (fileSize == 0) { throw std::runtime_error(fmt::format("Media file is empty: {}", filePath)); }
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) { throw std::runtime_error(fmt::format("Cannot send File Response. Failed to open file: {}", filePath)); }
    file.seekg(0);

    // Check for Range header
    auto rangeHeader = req.find(boost::beast::http::field::range);
    bool hasRange    = rangeHeader != req.end();

    size_t startByte = 0;
    size_t endByte   = fileSize - 1;
    if (hasRange)
    {
        // Parse Range header (format: "bytes=start-end")
        std::string rangeValue = std::string(rangeHeader->value());

        if (rangeValue.starts_with("bytes="))
        {
            constexpr size_t bytesPrefix = 6;    // length of "bytes="
            std::string      rangeSpec   = rangeValue.substr(bytesPrefix);
            size_t           dashPos     = rangeSpec.find('-');

            if (dashPos != std::string::npos)
            {
                std::string startStr = rangeSpec.substr(0, dashPos);
                std::string endStr   = rangeSpec.substr(dashPos + 1);

                try
                {
                    if (!startStr.empty()) { startByte = static_cast<size_t>(std::stoull(startStr)); }

                    if (!endStr.empty()) { endByte = static_cast<size_t>(std::stoull(endStr)); }
                    else
                    {
                        endByte = fileSize - 1;
                    }

                    // Validate range
                    if (startByte > endByte || startByte >= fileSize) {}

                    // Clamp end byte to file size
                    if (endByte >= fileSize) { endByte = fileSize - 1; }
                } catch (...)
                {
                    // Invalid range format, ignore and send full file
                    hasRange  = false;
                    startByte = 0;
                    endByte   = fileSize - 1;
                }
            }
        }
    }

    boost::system::error_code ec;

    auto fillResponseHeaders = [&](auto& res) {
        res.result(boost::beast::http::status::ok);
        res.version(req.version());
        res.keep_alive(req.keep_alive());
        res.set(boost::beast::http::field::content_type, contentType);
        res.set(boost::beast::http::field::access_control_allow_origin, "*");
        res.set(boost::beast::http::field::server, "stencil_webserver");
        res.set(boost::beast::http::field::accept_ranges, "bytes");
        res.set(boost::beast::http::field::content_length, std::to_string(fileSize));
    };

    if (hasRange && (startByte != 0 || endByte != fileSize - 1))
    {
        // Seek to start position
        file.seekg(static_cast<std::streamoff>(startByte));

        if (!file.good())
        {
            fmt::print(stderr, "Error seeking file {}\n", filePath.string());
            file.close();
            throw std::runtime_error(fmt::format("Failed to open media file: {}", filePath.string()));
        }

        // Read the requested range
        size_t      contentLength = endByte - startByte + 1;
        std::string fileContent;

        fileContent.resize(contentLength);

        file.read(fileContent.data(), static_cast<std::streamsize>(contentLength));
        auto bytesRead = static_cast<size_t>(file.gcount());
        file.close();

        if (bytesRead < contentLength)
        {
            // Adjust content length if we read less than expected
            fileContent.resize(bytesRead);
            contentLength = bytesRead;
            endByte       = startByte + bytesRead - 1;
        }

        // Prepare partial content response
        boost::beast::http::response<boost::beast::http::string_body> res;
        res.version(req.version());
        fillResponseHeaders(res);
        res.result(boost::beast::http::status::partial_content);
        res.set(boost::beast::http::field::content_range, fmt::format("bytes {}-{}/{}", startByte, endByte, fileSize));
        customizeResponse(res);
        res.body() = std::move(fileContent);
        res.prepare_payload();

        // Send the response
        boost::beast::http::response_serializer<boost::beast::http::string_body> sr{res};
        boost::beast::http::write_header(stream, sr);
        boost::beast::http::write(stream, sr);    // TODO : async_write
        TryCleanShutdown(stream);
        return;
    }

    if (req.method() == boost::beast::http::verb::head)
    {
        boost::beast::http::response<boost::beast::http::empty_body> res;
        fillResponseHeaders(res);
        customizeResponse(res);
        boost::beast::http::response_serializer<boost::beast::http::empty_body> sr{res};
        boost::beast::http::write_header(stream, sr);
        boost::beast::http::write(stream, sr, ec);    // TODO : async_write
        TryCleanShutdown(stream);
    }
    else
    {
        boost::beast::http::response<boost::beast::http::file_body> res;
        res.body().open(filePath.string().c_str(), boost::beast::file_mode::scan, ec);
        res.content_length(res.body().size());
        fillResponseHeaders(res);
        customizeResponse(res);
        res.prepare_payload();
        boost::beast::http::response_serializer<boost::beast::http::file_body> sr{res};
        boost::beast::http::write_header(stream, sr);
        boost::beast::http::write(stream, sr, ec);    // TODO : async_write
        if (ec)
        {
            if (ec == boost::beast::http::error::end_of_stream || ec == boost::asio::error::broken_pipe)
            {
                TryCleanShutdown(stream);
                // Ignore end of stream errors
                return;
            }
            fmt::print(stderr, "Error writing file response: {}\n", ec);
            if (ec == boost::asio::error::connection_reset)
            {
                TryCleanShutdown(stream);
                return;
            }
            throw boost::system::system_error{ec};
        }
        TryCleanShutdown(stream);
    }
}

inline void
WriteFileResponse(tcp_stream& stream, Request const& req, std::filesystem::path const& path, boost::beast::string_view const& contentType)
{
    WriteFileResponse(stream, req, path, contentType, [](auto&) {});
}

inline void Redirect(tcp_stream& stream, Request const& req, std::string_view const& redirectPath)
{
    boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::temporary_redirect, req.version()};

    auto host = [&]() {
        if (req.contains(boost::beast::http::field::location)) return req.at(boost::beast::http::field::location);
        if (req.contains(boost::beast::http::field::host)) return req.at(boost::beast::http::field::host);
        throw std::runtime_error("Cannot determine host");
    }();
    auto redirect = fmt::format("http://{}{}", std::string_view(host), redirectPath);
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
    boost::beast::http::write(stream, sr);    // TODO : async_write
}

inline boost::beast::string_view MimeType(boost::beast::string_view path)
{
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == boost::beast::string_view::npos) { return boost::beast::string_view{}; }
        return path.substr(pos);
    }();

    if (impl::iequals(ext, ".css")) { return "text/css"; }
    if (impl::iequals(ext, ".htm")) { return "text/html"; }
    if (impl::iequals(ext, ".html")) { return "text/html"; }
    if (impl::iequals(ext, ".php")) { return "text/html"; }
    if (impl::iequals(ext, ".txt")) { return "text/plain"; }

    if (impl::iequals(ext, ".js")) { return "application/javascript"; }
    if (impl::iequals(ext, ".json")) { return "application/json"; }
    if (impl::iequals(ext, ".kml")) { return "application/vnd.google-earth.kml+xml"; }
    if (impl::iequals(ext, ".m3u8")) { return "application/x-mpegURL"; }
    if (impl::iequals(ext, ".m3u")) { return "application/x-mpegURL"; }
    if (impl::iequals(ext, ".swf")) { return "application/x-shockwave-flash"; }
    if (impl::iequals(ext, ".xml")) { return "application/xml"; }

    if (impl::iequals(ext, ".bmp")) { return "image/bmp"; }
    if (impl::iequals(ext, ".gif")) { return "image/gif"; }
    if (impl::iequals(ext, ".ico")) { return "image/vnd.microsoft.icon"; }
    if (impl::iequals(ext, ".jpe")) { return "image/jpeg"; }
    if (impl::iequals(ext, ".jpeg")) { return "image/jpeg"; }
    if (impl::iequals(ext, ".jpg")) { return "image/jpeg"; }
    if (impl::iequals(ext, ".png")) { return "image/png"; }
    if (impl::iequals(ext, ".svg")) { return "image/svg+xml"; }
    if (impl::iequals(ext, ".svgz")) { return "image/svg+xml"; }
    if (impl::iequals(ext, ".tif")) { return "image/tiff"; }
    if (impl::iequals(ext, ".tiff")) { return "image/tiff"; }

    if (impl::iequals(ext, ".avi")) { return "video/x-msvideo"; }
    if (impl::iequals(ext, ".flv")) { return "video/x-flv"; }
    if (impl::iequals(ext, ".m4s")) { return "video/mp4"; }
    if (impl::iequals(ext, ".mkv")) { return "video/x-matroska"; }
    if (impl::iequals(ext, ".mov")) { return "video/quicktime"; }
    if (impl::iequals(ext, ".mp4")) { return "video/mp4"; }
    if (impl::iequals(ext, ".ts")) { return "video/vnd.dlna.mpeg-tts"; }
    if (impl::iequals(ext, ".webm")) { return "video/webm"; }
    if (impl::iequals(ext, ".wmv")) { return "video/x-ms-wmv"; }

    if (impl::iequals(ext, ".flac")) { return "audio/flac"; }
    if (impl::iequals(ext, ".m4a")) { return "audio/mp4"; }
    if (impl::iequals(ext, ".mp3")) { return "audio/mpeg"; }
    if (impl::iequals(ext, ".ogg")) { return "audio/ogg"; }
    if (impl::iequals(ext, ".wav")) { return "audio/wav"; }

    return "application/octet-stream";
}

}    // namespace Stencil::websvc
namespace Stencil::websvc::impl
{

template <typename... Types> struct Selector
{
    SUPPRESS_WARNINGS_START
    SUPPRESS_MSVC_WARNING(4702)    // unreachable code
    template <typename T, typename... TArgs> static bool InvokeIfMatch(TArgs&&... args)
    {
        if (T::Matches(std::forward<TArgs>(args)...))
        {

            T::Invoke(std::forward<TArgs>(args)...);
            return true;
        }
        return false;
    }
    SUPPRESS_WARNINGS_END

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

struct SvcMgr
{
    SvcMgr()  = default;
    ~SvcMgr() = default;
    CLASS_DELETE_COPY_AND_MOVE(SvcMgr);

    struct SSEInstance : std::enable_shared_from_this<SSEInstance>
    {
        using time_point = Stencil::Timestamp;

        SvcMgr* manager{nullptr};

        tcp_stream stream;
        time_point lastSendAt;
        bool       stopRequested = false;
        size_t     category      = {0};

        public:
        SSEInstance(size_t categoryIn, tcp_stream&& streamIn, Request const& req) : stream(std::move(streamIn)), category(categoryIn)
        {
            auto res = impl::CreateResponse<boost::beast::http::buffer_body>(req, "text/event-stream");
            boost::beast::http::response_serializer<boost::beast::http::buffer_body> sr(res);
            res.set(boost::beast::http::field::transfer_encoding, "chunked");
            res.body().data = nullptr;
            res.body().size = 0;
            res.body().more = true;
            boost::beast::http::write_header(stream, sr);    // TODO : async_write
        }
        ~SSEInstance() = default;
        CLASS_DELETE_COPY_AND_MOVE(SSEInstance);

        void Release(std::unique_lock<std::mutex> const& /* lock */)
        {
            stopRequested = true;
            TryCleanShutdown(stream);
            stream.close();
        }

        bool Send(std::unique_lock<std::mutex> const& lock, std::span<char const> const& msg)
        {
            if (msg.size() == 0) return true;
            lastSendAt   = Stencil::Timestamp::clock::now();
            auto msgSize = msg[msg.size() - 1] == '\0' ? msg.size() - 1 : msg.size();
            if (msgSize == 0) return true;
            boost::asio::const_buffer b{msg.data(), msgSize};
            boost::system::error_code ec;
            boost::beast::net::write(stream, boost::beast::http::make_chunk(b), ec);
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
            if (inst->stopRequested)
            {
                ++it;
                continue;
            }
            if (inst->category != 0 && category != 0 && inst->category != category)
            {
                ++it;
                continue;
            }
            if (!inst->Send(lock, msg)) { it = _sseListeners.erase(it); }
            else
            {
                // inst->dataAvailable.notify_all();
                ++it;
            }
        }
    }

    boost::asio::awaitable<void> WaitForTimeout()
    {
        static auto constexpr KeepAliveInterval = 10s;
        boost::asio::steady_timer timer{_ioc};
        boost::system::error_code ec;
        while (true)
        {
            co_await timer.async_wait(boost::asio::redirect_error(boost::asio::use_awaitable, ec));
            auto lock = std::unique_lock<std::mutex>(_mutex);
            for (auto const& listener : _sseListeners)
            {
                if ((listener->lastSendAt + KeepAliveInterval) < Stencil::Timestamp::clock::now()) { listener->Release(lock); }
            }
            if (_sseListeners.empty()) { co_return; }
        }
    }

    std::shared_ptr<SSEInstance>
    CreateInstance(size_t category, tcp_stream&& streamIn, Request const& req, std::span<char const> const& msg)
    {
        auto inst = std::make_shared<SSEInstance>(category, std::move(streamIn), req);
        auto lock = std::unique_lock<std::mutex>(_mutex);
        if (_sseListeners.empty())
        {
            boost::asio::co_spawn(_ioc, WaitForTimeout(), [](std::exception_ptr const& e) {
                if (e) try
                    {
                        std::rethrow_exception(e);
                    } catch (std::exception& e) { fmt::print(stderr, "Error in acceptor: {}\n", e.what()); }
            });
        }
        _sseListeners.insert(inst);
        inst->Send(lock, msg);
        return inst;
    }

    void Stop()
    {
        auto lock      = std::unique_lock<std::mutex>(_mutex);
        _stopRequested = true;
        for (auto const& inst : _sseListeners) { inst->Release(lock); }
        _sseListeners.clear();
        _ioc.stop();
    }

    auto& IOC() { return _ioc; }

    private:
    std::unordered_set<std::shared_ptr<SSEInstance>> _sseListeners;

    boost::asio::io_context _ioc{8};
    bool                    _stopRequested = false;
    std::mutex              _mutex;
};

template <typename TImpl, ConceptInterface TInterface> struct WebRequestContext
{
    using Interface = TInterface;
    using Impl      = TImpl;

    impl::SvcMgr&                         mgr;         // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    TImpl&                                impl;        // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    tcp_stream&                           stream;      // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    Request const&                        req;         // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    boost::urls::url_view&                url;         // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    boost::urls::segments_base::iterator& urlSegIt;    // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

template <typename TContext> struct RequestHandlerForAllEvents
{
    static bool Matches(TContext& ctx) { return impl::iequals(*ctx.urlSegIt, "events"); }
    static auto Invoke(TContext& ctx)
    {
        ctx.mgr.CreateInstance(0, std::move(ctx.stream), ctx.req, "event: init\ndata: \n\n");
        // ctx.impl.OnSSEInstanceEnded();
    }
};

template <typename TContext, typename TEventStructs> struct RequestHandlerForEvents
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TEventStructs>::Name(), *ctx.urlSegIt); }
    static auto Invoke(TContext& ctx)
    {
        ctx.mgr.CreateInstance(typeid(TContext).hash_code(), std::move(ctx.stream), std::move(ctx.req), "event: init\ndata: \n\n");
    }
};

template <typename TContext, typename TObjectStoreObj> struct RequestHandlerForObjectStore
{
    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name(), *ctx.urlSegIt); }

    template <typename TLambda> static auto ForeachObjId(TContext& ctx, TLambda const& lambda)
    {
        std::ostringstream rslt;

        auto subpath = *(++ctx.urlSegIt);
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
            while (sindex < ids.size())
            {
                auto eindex = ids.find(',', sindex);
                if (eindex == std::string_view::npos) eindex = ids.size();
                auto idstr = ids.substr(sindex, eindex - sindex);
                if (!first) { rslt << ','; }
                rslt << idstr;
                first   = false;
                auto id = static_cast<uint32_t>(std::stoul(idstr));
                lambda(id);
                sindex = eindex + 1;
            }
            rslt << '}';
        }
        else
        {
            auto id = static_cast<uint32_t>(std::stoul(subpath));
            lambda(id);
        }
        return rslt.str();
    }

    template <typename TArgsStruct> static auto CreateArgStruct(TContext& ctx)
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
        auto action = *(++ctx.urlSegIt);
        auto msg    = Handle(ctx, action);
        auto res    = impl::CreateResponse<boost::beast::http::string_body>(ctx.req, "application/json");
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
            auto [id, obj1] = objects.template Create<TObjectStoreObj>(lock, CreateArgStruct<TObjectStoreObj>(ctx));
            uint32_t idint  = id.id;
            fmt::print(rslt, "{}", Stencil::Json::Stringify(idint));
            ctx.mgr.Send(typeid(TContext).hash_code(),
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
            ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                rslt << jsobj;
            });
        }
        else if (action == "edit")
        {
            ctx.mgr.Send(
                typeid(TContext).hash_code(),
                fmt::format("event: objectstore_edit\ndata: {{\'{}\': {{", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            ForeachObjId(ctx, [&](uint32_t id) {
                auto obj1  = objects.template Get<TObjectStoreObj>(lock, {id});
                auto jsobj = Stencil::Json::Stringify(Stencil::Database::CreateRecordView(objects, lock, {id}, obj1));
                rslt << jsobj;
                ctx.mgr.Send(typeid(TContext).hash_code(), fmt::format("{}\'{}\': {}", (first ? ' ' : ','), id, jsobj));
                first = false;
            });
            ctx.mgr.Send(typeid(TContext).hash_code(), "}}\n\n");
        }
        else if (action == "delete")
        {
            ctx.mgr.Send(
                typeid(TContext).hash_code(),
                fmt::format("event: objectstore_delete\ndata: {{\'{}\': [", Stencil::InterfaceObjectTraits<TObjectStoreObj>::Name()));
            auto lock  = objects.LockForEdit();
            bool first = true;
            ForeachObjId(ctx, [&](uint32_t id) {
                try
                {
                    objects.template Delete<TObjectStoreObj>(lock, {id});
                    rslt << "true";
                    ctx.mgr.Send(typeid(TContext).hash_code(), fmt::format("{}{}", (first ? ' ' : ','), id));
                    first = false;
                } catch (std::exception const& /*ex*/) { rslt << "false"; }
            });
            ctx.mgr.Send(typeid(TContext).hash_code(), "]}\n\n");
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
    using TImpl      = typename TContext::Impl;
    using TInterface = typename TContext::Interface;

    static bool Matches(TContext& ctx) { return impl::iequals(Stencil::InterfaceApiTraits<TArgsStruct>::Name(), *ctx.urlSegIt); }
    static auto CreateArgStruct(TContext& ctx)
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
            return args;
        }
        if (ctx.req.method() == boost::beast::http::verb::put)
        {
            auto data = ctx.req.body();
            Stencil::SerDesRead<Stencil::ProtocolJsonVal>(args, data);
            return args;
        }

        throw std::runtime_error("Only get and put supported for functions");
    }

    static auto Invoke(TContext& ctx)
    {
        using Traits = ::Stencil::InterfaceApiTraits<TArgsStruct>;
        std::ostringstream rslt;

        auto  args  = CreateArgStruct(ctx);
        auto& ifobj = WebServiceImplTraits<TImpl, TInterface>::QueryInterface(ctx.impl);
        if constexpr (std::is_same_v<void, decltype(Traits::Invoke(ifobj, args))>)
        {
            Traits::Invoke(ifobj, args);
            auto res   = impl::CreateResponse<boost::beast::http::string_body>(ctx.req, "application/json");
            res.body() = "{}";
            boost::beast::http::response_serializer<boost::beast::http::string_body, boost::beast::http::fields> sr{res};
            boost::beast::http::write(ctx.stream, sr);
        }
        else
        {
            auto retval = Traits::Invoke(ifobj, args);
            rslt << Stencil::Json::Stringify<decltype(retval)>(retval);
            auto msg   = rslt.str();
            auto res   = impl::CreateResponse<boost::beast::http::string_body>(ctx.req, "application/json");
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
    static bool Matches(SvcMgr& /* mgr */,
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
        static bool Matches(TContext& ctx) { return impl::iequals(*ctx.urlSegIt, std::string_view("objectstore")); }

        static auto Invoke(TContext& ctx)
        {
            std::ostringstream rslt;

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
                    ctx.urlSegIt             = ctx.url.segments().begin();
                    using TypesT    = typename ObjectStoreTransform<typename Stencil::InterfaceTraits<TInterface>::Objects>::Handler;
                    using SelectorT = typename SelectorTransform<TypesT>::SelectorT;
                    SelectorT::Invoke(ctx);
                    query = remaining;
                }
            }
            rslt << ']';
            auto rsltstr = rslt.str();
            ctx.mgr.CreateInstance(
                typeid(TContext).hash_code(), std::move(ctx.stream), ctx.req, fmt::format("event: init\ndata: {}\n\n", rsltstr));
        }
    };

    static void Invoke(SvcMgr&                               mgr,
                       TImpl&                                impl,
                       tcp_stream&                           stream,
                       Request const&                        req,
                       boost::urls::url_view&                url,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        WebRequestContext<TImpl, TInterface> ctx{mgr, impl, stream, req, url, it};
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

template <ConceptIndexable TState> struct SynchronizedState
{};

template <typename TImpl, ConceptIndexable TState> struct RequestHandler<TImpl, SynchronizedState<TState>>
{
    static bool Matches(SvcMgr& /* mgr */,
                        TImpl& impl,
                        tcp_stream& /* stream */,
                        Request const& /* req */,
                        boost::urls::url_view& /*url*/,
                        boost::urls::segments_base::iterator& it)
    {
        return iequals(impl.Name(), *it);
    }

    static void Invoke(SvcMgr&                               mgr,
                       TImpl&                                impl,
                       tcp_stream&                           stream,
                       Request const&                        req,
                       boost::urls::url_view&                url,
                       boost::urls::segments_base::iterator& it)
    {
        ++it;
        if (*it == "apply")
        {

            auto  editCtx = impl.EditContext();
            auto& txn     = editCtx.TXN();
            if (req.method() == boost::beast::http::verb::get)
            {
                for (auto const& param : url.params())
                {
                    Stencil::StringTransactionSerDes::Apply(txn, fmt::format("{}={};", param.key, param.value));
                }
            }
            else if (req.method() == boost::beast::http::verb::put)
            {
                auto data = req.body();
                Stencil::StringTransactionSerDes::Apply(txn, data);
            }
            else
            {
                throw std::logic_error(
                    fmt::format("Only GET and PUT methods are supported for {}", std::string_view{url.data(), url.size()}));
            }
            WriteStringResponse(stream, req, "application/json", "{}");
        }
        else
        {
            mgr.CreateInstance(
                typeid(TState).hash_code(), std::move(stream), req, fmt::format("event: init\ndata: {}\n\n", impl.StateStringify()));
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

template <typename TImpl, ConceptInterface TInterface> struct WebServiceInterfaceImplT<TImpl, TInterface>
{};

template <typename TImpl, ConceptIndexable T>
struct WebServiceInterfaceImplT<TImpl, impl::SynchronizedState<T>> : impl::SynchronizedState<T>
{

    void NotifyStateChanged(Stencil::Transaction<T>::View const& txn)
    {
        auto msg = fmt::format("event: changed\ndata: {}\n\n", Stencil::StringTransactionSerDes::Deserialize(txn));
        static_cast<TImpl*>(this)->SSESend(typeid(T).hash_code(), msg);
    }
};

template <ConceptIndexable TState> using WebSynchronizedState = impl::SynchronizedState<TState>;

template <typename TImpl, typename... TServices> struct WebServiceT : public WebServiceInterfaceImplT<TImpl, TServices>...
{
    using WebService = WebServiceT<TImpl, TServices...>;

    static constexpr size_t NumServices = sizeof...(TServices);

    static constexpr auto Timeout = std::chrono::seconds{30};

    WebServiceT() = default;
    ~WebServiceT() { StopDaemon(); }

    CLASS_DELETE_COPY_AND_MOVE(WebServiceT);

    void StartOnPort(uint16_t port, uint16_t numThreads = 4)
    {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        boost::asio::co_spawn(_mgr.IOC(), DoListen_(tcp::endpoint{address, port}), [](std::exception_ptr const& e) {
            if (e) try
                {
                    std::rethrow_exception(e);
                } catch (std::exception& e) { fmt::print(stderr, "Error in acceptor: {}\n", e.what()); }
        });

        for (size_t i = 0; i < numThreads + NumServices; i++)
        {
            _listenthreads.emplace_back([this]() {
                SetThreadName("ios-runner");
                _mgr.IOC().run();
            });
        }
    }

    void StopDaemon()
    {
        _mgr.Stop();
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
        _mgr.Send(0, msg);
    }

    template <ConceptInterface TInterface> auto& GetInterface()
    {
        return WebServiceImplTraits<TImpl, TInterface>::QueryInterface(*static_cast<TImpl*>(this));
    }

    void SSESend(size_t typeHash, std::span<char const> const& msg) { _mgr.Send(typeHash, msg); }

    bool HandleRequest(tcp_stream& stream, Stencil::websvc::Request const& /* req */, boost::urls::url_view const& /* url */)
    {
        TryCleanShutdown(stream);
        return false;
    }

    private:
    // Return a reasonable mime type based on the extension of a file.

    // private: TODO: remove this when boost beast isnt experimental anymore
    // Return a response for the given request.
    //
    // The concrete type of the response message (which depends on the
    // request), is type-erased in message_generator.
    template <class Body, class Allocator>
    auto HandleRequest_(tcp_stream& stream, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>& req)
    {
        // Respond to HEAD request
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wswitch-enum")
        SUPPRESS_MSVC_WARNING(4061)    // swtich enum not handled
        switch (req.method())
        {
        case boost::beast::http::verb::get: [[fallthrough]];
        case boost::beast::http::verb::head: [[fallthrough]];
        case boost::beast::http::verb::put:
        {
            auto& impl   = *static_cast<TImpl*>(this);
            auto  target = req.target();
            if (target == "/.well-known/appspecific/com.chrome.devtools.json") { return; }
            auto url  = boost::urls::parse_origin_form(target).value();
            auto segs = url.segments();

            auto it = segs.begin();
            if (it == segs.end() || *it != "api")
            {
                if (!impl.HandleRequest(stream, req, url))
                {
                    throw std::invalid_argument(fmt::format("Unable to fulfill request: {}. No Handler found", std::string_view(target)));
                }
                return;
            }
            ++it;
            return impl::Selector<impl::RequestHandler<TImpl, TServices>...>::Invoke(_mgr, impl, stream, req, url, it);
        }
        default:
            TryCleanShutdown(stream);
            throw std::invalid_argument(
                fmt::format("Request Verb:{} Not implemented", std::string_view(boost::beast::http::to_string(req.method()))));
        }
        SUPPRESS_WARNINGS_END
    }

    boost::asio::awaitable<void> DoListen_(tcp::endpoint endpoint)
    {
        auto acceptor = boost::asio::use_awaitable_t<boost::asio::any_io_executor>::as_default_on(
            tcp::acceptor(co_await boost::asio::this_coro::executor));
        acceptor.open(endpoint.protocol());
        acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen(boost::asio::socket_base::max_listen_connections);

        while (true)
        {
            boost::beast::flat_buffer buffer;
            Request                   req;
            tcp_stream                stream(co_await acceptor.async_accept());
            stream.expires_after(Timeout);
            try
            {
                [[maybe_unused]] auto bytesTransferred
                    = co_await boost::beast::http::async_read(stream, buffer, req, boost::asio::use_awaitable);
                SetThreadName(fmt::format("w:{}", req.target()).c_str());
                HandleRequest_(stream, req);
                SetThreadName("w:...");
            } catch (boost::system::system_error const& e)
            {
                if (e.code() != boost::beast::http::error::end_of_stream)
                {
                    fmt::print(stderr, "Error Starting Session: {}\n", e.what());
                    throw;
                }
            }
        }
    }

    std::vector<std::thread> _listenthreads;
    int                      _port{};
    impl::SvcMgr             _mgr;
};
SUPPRESS_WARNINGS_END
}    // namespace Stencil::websvc
