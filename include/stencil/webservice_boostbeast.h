#include "CommonMacros.h"
#include "interfaces.h"
#include "protocol_json.h"

SUPPRESS_WARNINGS_START
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
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
#include <vector>

#define STENCIL_USING_WEBSERVICE

#if !defined(BOOST_ASIO_HAS_CO_AWAIT)
#error Need co await
#endif

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

    inline std::tuple<std::string_view, std::string_view> Split(std::string_view const& path, char token = '/')
    {
        if (path.empty()) throw std::logic_error("Invalid path");
        size_t start = path[0] == token ? 1u : 0u;
        size_t index = path.find(token, start);
        auto   str1  = path.substr(start, index - start);
        if (index == path.npos) return {str1, {}};
        return {str1, path.substr(index)};
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
        //_sseManager.Send(msg);
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

        // Make sure we can handle the method
        if (req.method() != boost::beast::http::verb::get && req.method() != boost::beast::http::verb::head)
            return bad_request("Unknown HTTP-method");

        Response res{};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "application/json");
        // res.content_length(size);
        res.keep_alive(req.keep_alive());

        // Build the path to the requested file
        std::string path;    //= path_cat(doc_root, req.target());
        if (req.target().back() == '/') path.append("index.html");

        // Attempt to open the file
        boost::beast::error_code                  ec;
        boost::beast::http::file_body::value_type body;
        body.open(path.c_str(), boost::beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if (ec == boost::beast::errc::no_such_file_or_directory) return not_found(req.target());

        // Handle an unknown error
        if (ec) return server_error(ec.message());

        // Cache the size since we need it after the move
        auto const size = body.size();

        // Respond to HEAD request
        if (req.method() == boost::beast::http::verb::head)
        {
            boost::beast::http::response<boost::beast::http::empty_body> res{boost::beast::http::status::ok, req.version()};
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return res;
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
    SUPPRESS_WARNINGS_START
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
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
    SUPPRESS_WARNINGS_END

    bool _HandleRequest(const Request& req, Response& res)
    {
        std::string_view const& path = req.target();
        auto [ifname, subpath]       = Split(path);
        if (ifname != "api" || subpath.empty()) return false;
        try
        {
            auto [ifname1, subpath1] = Split(subpath);
            bool found               = (_TryHandleRequest<Ts>(req, res, ifname1, subpath1) || ...);
            if (!found) throw std::runtime_error(fmt::format("No matching interface found for {}", ifname1));
        } catch (std::exception const& ex)
        {
            res.result(500);
            res.body() = ex.what();
        }
        return true;
    }

    void _HandleError(const Request& /* req */, Response& res)
    {
        res.set_content(fmt::format("<p>Error Status: <span style='color:red;'>{}</span></p>", res.status), "text/html");
    }

    template <ConceptInterface T>
    bool _TryHandleRequest(Request const& req, Response& res, std::string_view const& ifname, std::string_view const& path)
    {
        if (!impl::iequal(Stencil::InterfaceTraits<T>::Name(), ifname)) { return false; }
        impl::WebRequestHandler<T>{*std::get<std::unique_ptr<T>>(_impls).get(), req, res, _sseManager}.HandleRequest(path);
        return true;
    }

    boost::asio::io_context ioc{1};

    std::thread             _listenthread;
    std::condition_variable _cond;
    std::mutex              _mutex;

    int _port;
    //   impl::SSEListenerManager           _sseManager;
    std::tuple<std::unique_ptr<Ts>...> _impls = {Ts::Create()...};
};
}    // namespace Stencil
