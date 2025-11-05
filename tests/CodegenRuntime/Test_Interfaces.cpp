#include "ObjectsTester.h"
#include "TestUtils.h"

#include <stencil/webservice_boostbeast.h>

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4191)    // type cast': unsafe conversion
SUPPRESS_MSVC_WARNING(4244)    // conversion from '_Ty' to '_Ty1', possible loss of data
SUPPRESS_MSVC_WARNING(4355)    //'this' : used in base member initializer list
SUPPRESS_MSVC_WARNING(4365)    // conversion from 'const char' to 'unsigned char', signed/unsigned mismatch
SUPPRESS_MSVC_WARNING(4548)    // expression before comma has no effect; expected expression with side - effect
SUPPRESS_MSVC_WARNING(4625)    // operator implicitly deleted
SUPPRESS_MSVC_WARNING(4626)    // operator implicitly deleted
SUPPRESS_MSVC_WARNING(4668)    // not defined as a preprocessor macro
SUPPRESS_MSVC_WARNING(4702)    // Unreachable code
SUPPRESS_MSVC_WARNING(4855)    // implicit capture of 'this' via '[=]' is deprecated
SUPPRESS_MSVC_WARNING(5026)    // operator implicitly deleted
SUPPRESS_MSVC_WARNING(5027)    // operator implicitly deleted
SUPPRESS_MSVC_WARNING(5039)    // pointer or reference to potentially throwing function passed to 'extern)
SUPPRESS_MSVC_WARNING(5262)    // implicit fall-through occurs here;
SUPPRESS_CLANG_WARNING("-Weverything")
SUPPRESS_GCC_WARNING("-Wmaybe-uninitialized")

#include <boost/asio.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/read.hpp>

#include <boost/url.hpp>
#include <boost/url/url.hpp>

#include <charconv>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <string>
SUPPRESS_WARNINGS_END

static_assert(Stencil::Database::ConceptRecord<uint32_t>);
static_assert(Stencil::Database::ConceptTrivial<uint32_t>);

static_assert(Stencil::Database::ConceptFixedSize<uint32_t>);
static_assert(Stencil::Database::ConceptBlob<shared_string>);
static_assert(Stencil::Database::ConceptComplex<std::unordered_map<uint32_t, uint32_t>>);

using namespace std::chrono_literals;
namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
using tcp       = net::ip::tcp;

template <> struct fmt::formatter<beast::error_code> : fmt::formatter<std::string_view>
{
    auto format(beast::error_code const& ec, fmt::format_context& ctx) const
    {
        return fmt::format_to(
            ctx.out(), "ec.value() = {}, ec.category() = {}, ec.message() = {}", ec.value(), ec.category().name(), ec.message());
    }
};

struct HttpClientListener
{
    using Params = std::unordered_map<std::string, std::string>;

    static constexpr std::string_view LocalHostName = "localhost";
    static constexpr std::string_view Port          = "44444";

    HttpClientListener(std::string_view const& url) : _url(url), ioc(), resolver(ioc), stream(ioc)
    {
        req = {http::verb::get, _url, 11 /*HTTP Version 1.1*/};
        req.set(http::field::host, LocalHostName);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::accept, "text/event-stream");
        req.set(http::field::connection, "Keep-Alive");
    }

    CLASS_DELETE_COPY_AND_MOVE(HttpClientListener);
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    bool _ChunkCallback(char const* data, size_t len)
    {
        if (data[len - 1] == '\0') len--;
        _sseData.push_back(std::string(data, len));
        return true;
    }
    SUPPRESS_WARNINGS_END
    void _SSEListener()
    {
        bool reconnect = true;
        while (!_stopRequested && reconnect)
        {
            stream.connect(resolver.resolve(LocalHostName, Port));
            // Send request
            http::write(stream, req);
            beast::flat_buffer buffer;

            // Read header first
            http::response_parser<http::empty_body> parser;
            parser.body_limit(0);    // no body in header
            http::read_header(stream, buffer, parser);

            auto const& res = parser.get();
            if (res.result() != http::status::ok) { throw std::runtime_error("Bad response"); }
            beast::error_code      ec;
            boost::asio::streambuf buf(4096);
            {
                std::unique_lock<std::mutex> guard(_mutex);
                _responseRecieved = true;
                _cv.notify_all();
            }

            while (!_stopRequested)
            {
                auto bytes = boost::asio::read_until(stream.socket(), buf, "\r\n", ec);
                if (bytes == 0 && ec)
                {
                    if (ec == net::error::eof || ec == beast::http::error::end_of_stream) { break; }
                    fmt::print(stderr, "sse_listener[{}]:{} buf.avail={} ec={} TODO: Throw\n", _url, __LINE__, buf.size(), ec);
                    throw beast::system_error(ec);
                }

                if (ec)
                {
                    fmt::print(stderr,
                               "sse_listener[{}]:{} buf.avail={} ec={} error reading chunk-encoded-part-size bytes: {}\n",
                               _url,
                               __LINE__,
                               buf.size(),
                               ec,
                               bytes);
                }

                auto bufchars = reinterpret_cast<char const*>(buf.data().data());
                assert(bytes >= 2 && bufchars[bytes - 1] == '\n' && bufchars[bytes - 2] == '\r');

                if (bytes == 2)
                {
                    buf.consume(bytes);
                    // These are common. Not really keep-alive message/
                    // fmt::print(stderr, "sse_listener[{}]: empty-message buf.size()={}\n", _url, bytes, buf.size());
                    continue;
                }
                std::size_t messageSize = 0;

                auto result = std::from_chars(bufchars, bufchars + bytes - 2, messageSize, 16);
                if (result.ptr != bufchars + bytes - 2 || result.ec != std::errc())
                {    //
                    throw std::runtime_error(
                        fmt::format("sse_listener[{}]:{} buf.avail = {}, Invalid hex string: {} (chunk-encoding part size) recieved\n",
                                    _url,
                                    __LINE__,
                                    buf.size(),
                                    std::string_view(bufchars, bytes - 2)));
                }

                buf.consume(bytes);

                for (size_t i = 0, remaining = messageSize; i < messageSize;)
                {
                    size_t buf_prev_size = buf.size();
                    size_t read_bytes    = std::min(remaining, buf.max_size());
                    if (buf_prev_size < read_bytes)
                    {
                        bytes = boost::asio::read(stream.socket(), buf, boost::asio::transfer_exactly(read_bytes - buf_prev_size), ec);
                        bytes += buf_prev_size;
                    }
                    else
                    {
                        bytes = read_bytes;
                        ec    = {};
                    }

                    if (bytes != read_bytes)
                    {    //
                        fmt::print(stderr,
                                   "sse_listener[{}]:{} buf.avail={}=>{} ec={} recieved: {} != requested: {}\n",
                                   _url,
                                   __LINE__,
                                   buf_prev_size,
                                   buf.size(),
                                   ec,
                                   bytes,
                                   read_bytes);
                        assert(ec);    // something catastrophic must have happened
                        // truncate how much we read to how much is available
                        read_bytes = std::min(static_cast<size_t>(buf.size()), read_bytes);
                    }

                    // buf.size() !=  read_bytes is pretty commmon.
                    // Just means that read_until actually read and buffered a lot more than it said it did
                    // fmt::print(stderr, "Mismatch buf.size() {} != read_bytes {} ec = {}\n", buf.size(), read_bytes, ec);

                    bufchars = reinterpret_cast<char const*>(buf.data().data());
                    if (!(messageSize == 2 && read_bytes == 2 && bufchars[0] == '\n' && bufchars[1] == '\n'))
                    {
                        _ChunkCallback(bufchars, read_bytes);
                    }
                    i += read_bytes;
                    remaining -= read_bytes;
                    buf.consume(read_bytes);

                    if (ec && bytes == read_bytes)
                    {    //
                        fmt::print(stderr, "sse_listener[{}]:{} buf.avail={}=>{} ec={}\n", _url, __LINE__, buf_prev_size, buf.size(), ec);
                    }

                    // fmt::print(" {}/{}", bytes, remaining);
                    if (ec && (ec == net::error::eof || ec == beast::http::error::end_of_stream))
                    {
                        continue;
                        // Continue flushing the buf
                    }

                    if (ec)
                    {
                        fmt::print(stderr, "sse_listener[{}]:{} buf.avail={} ec={}\n", _url, __LINE__, buf.size(), ec);
                        if ((_stopRequested && (ec == net::error::operation_aborted || ec == boost::asio::error::interrupted))) { return; }
                        fmt::print(stderr, "SSEListener encountered error :{}\n", ec);
                        return;
                        // throw beast::system_error(ec);
                    }
                }
            }

            // Gracefully close
            ec = stream.socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec)
            {    //
                fmt::print(stderr, "Error in shutdown: {}\n", ec);
            }
        }
    }

    void Start()
    {
        _activated        = true;
        _responseRecieved = false;
        _stopRequested    = false;

        _sseListener = std::thread([&]() {
            try
            {
                this->_SSEListener();
            } catch (std::exception const&)
            {}
        });
        std::unique_lock<std::mutex> guard(_mutex);
        _cv.wait(guard, [&]() { return this->_responseRecieved; });
    }

    void RequestStop()
    {
        _stopRequested = true;
        // beast::error_code ec;
        //  ec = stream.socket().close(ec);
    }

    void Stop()
    {
        RequestStop();
        if (_sseListener.joinable()) _sseListener.join();
    }

    static std::string Get(std::string_view const& target, Params const& params)
    {
        boost::urls::url url;
        url.set_path(target);
        for (auto const& [k, v] : params)
        {    //
            url.params().append({k, v});
        }

        net::io_context   ioc;
        tcp::resolver     resolver(ioc);
        beast::tcp_stream stream(ioc);

        stream.connect(resolver.resolve(LocalHostName, Port));
        {
            // Send request
            http::request<http::string_body> req{http::verb::get, url.encoded_target(), 11 /*HTTP Version 1.1*/};
            req.set(http::field::host, LocalHostName);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(http::field::accept, "application/json");
            http::write(stream, req);
        }
        beast::flat_buffer buffer;
        /*
         {
             http::response_parser<http::empty_body> parser;
             http::read_header(stream, buffer, parser);
             auto const& res = parser.get();
             if (res.result() != http::status::ok) { throw std::runtime_error("Bad response"); };
         } */
        {
            http::response<http::dynamic_body> res;
            beast::error_code                  ec;
            http::read(stream, buffer, res, ec);
            if (ec && ec != net::error::eof && ec != beast::http::error::end_of_stream)
            {
                fmt::print(stderr, "SSEListener encountered error :{}", ec);
                throw beast::system_error(ec);
            }
            ec = stream.socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != boost::asio::error::not_connected)
            {
                fmt::print(stderr, "SSEListener encountered error :{}", ec);
                throw beast::system_error(ec);
            }
            return beast::buffers_to_string(res.body().data());
        }
    }

    ~HttpClientListener() { Stop(); }

    bool                     _activated{false};
    bool                     _responseRecieved;
    std::atomic<bool>        _stopRequested;
    std::mutex               _mutex;
    std::condition_variable  _cv;
    std::vector<std::string> _sseData;
    std::thread              _sseListener;
    std::string              _url;

    net::io_context                  ioc;
    tcp::resolver                    resolver;
    beast::tcp_stream                stream;
    http::request<http::string_body> req;
};

/*
interface Server1 {
event SomethingHappened(uint32 arg1, SimpleObject1 arg2);
objectstore SimpleObject1   obj1;
objectstore NestedObject    obj2;
dict<uint32, SimpleObject1> Function1(uint32 arg1, SimpleObject1 arg2);
}
*/
/// Generated code begins

struct SSEFormat : TestCommon::JsonFormat
{
    static bool Compare(std::vector<std::string> const& actual, std::istream& ss)
    {
        auto expected = TestCommon::ReadStrStream(ss);
        if (actual.size() != expected.size()) return false;

        for (size_t i = 0; i != actual.size(); i++)
        {
            auto&            act    = actual[i];
            auto&            exp    = expected[i];
            std::string_view prefix = "data: ";
            if (act == exp) continue;
            if (act.size() < prefix.size() || exp.size() < prefix.size()) return false;
            if (act.substr(0, prefix.size()) != exp.substr(0, prefix.size())) return false;
            if (act.substr(0, prefix.size()) != prefix) return false;
            if (!TestCommon::JsonStringEqual(act.substr(prefix.size()), exp.substr(prefix.size()))) return false;
        }

        return true;
    }
};

struct Server1Impl
    : Stencil::websvc::WebServiceT<Server1Impl, Interfaces::Server1, Stencil::websvc::WebSynchronizedState<Objects::NestedObject>>
{
    Server1Impl() { objects.Init(std::filesystem::path("SaveAndLoad.bin")); }
    ~Server1Impl() override = default;
    CLASS_DELETE_COPY_AND_MOVE(Server1Impl);
    std::string_view Name() override { return "state"; }
    std::string      StateStringify() override { return Stencil::Json::Stringify(state); }

    std::unordered_map<uint32_t, Objects::SimpleObject1> Function1(uint32_t const& arg1, Objects::SimpleObject1 const& arg2) override
    {
        std::unordered_map<uint32_t, Objects::SimpleObject1> retval;

        auto key    = arg1 + 1;
        auto copied = arg2;
        copied.val1 += 1;
        copied.val2 += 1;
        copied.val3 += 1;
        copied.val5 += 1.0;
        retval[key] = std::move(copied);

        Raise_SomethingHappened(key, copied);
        return retval;
    }

    void Function2() override {}
    void Function3(uint32_t const& /* arg1 */) override {}

    void OnSSEInstanceEnded() {}

    void                  OnStateChange(Stencil::Transaction<Objects::NestedObject>::View const& txnv) { NotifyStateChanged(txnv); }
    Objects::NestedObject state;
    // Event listeners ?
};

// Generated code ends

struct Tester : ObjectsTester
{
    using Params = HttpClientListener::Params;
    Tester()
    {
        if (std::filesystem::exists(dbfile)) std::filesystem::remove(dbfile);
        svc = std::make_unique<Server1Impl>();
        svc->StartOnPort(44444, 6);
    }

    ~Tester()
    {
        _sseListener1.RequestStop();
        _sseListener2.RequestStop();
        _sseListener3.RequestStop();

        svc->StopDaemon();
        svc.reset();

        if (std::filesystem::exists(dbfile)) std::filesystem::remove(dbfile);
        TestCommon::CheckResource<TestCommon::JsonFormat>(_json_lines, "json");

        auto checkListener = [&](HttpClientListener& listener, std::string_view const& name) {
            if (listener._activated) { TestCommon::CheckResource<SSEFormat>(TestCommon::ResplitLines(listener._sseData), name); }
        };
        checkListener(_sseListener1, "server1_somethinghappened");
        checkListener(_sseListener2, "server1_objectstore");
        checkListener(_sseListener3, "server1_statenotifications");
    }

    void StartListeners()
    {
        _sseListener1.Start();
        _sseListener2.Start();
        _sseListener3.Start();
    }

    CLASS_DELETE_COPY_AND_MOVE(Tester);

    template <typename T> auto _create_http_params(T const& obj)
    {
        Params params;
        Stencil::Visitor<T>::VisitAll(obj, [&](auto const& key, auto const& val) {
            std::string keystr = Stencil::Serialize<Stencil::ProtocolString>(key).str();
            std::string valstr = Stencil::Serialize<Stencil::ProtocolJsonVal>(val).str();
            params.insert({std::move(keystr), std::move(valstr)});
        });
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return params;
        SUPPRESS_WARNINGS_END
    }

    auto _valid_cli_json_get(std::string const& path, Params const& params)
    {
        auto json = HttpClientListener::Get(path, params);
        CHECK(json != "");
        _json_lines.push_back(json);
        return json;
    }

    void cli_create_obj1()
    {
        auto obj1  = create_simple_object1();
        _cliObj1Id = _valid_cli_json_get("/api/server1/obj1/create", _create_http_params(obj1));
    }

    void cli_read_obj1()
    {
        REQUIRE(_cliObj1Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj1/read/{}", _cliObj1Id), {});
        // auto all = _valid_cli_json_get(fmt::format("/api/server1/obj1/all"), {});
    }

    void cli_edit_obj1()
    {
        REQUIRE(_cliObj1Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj1/edit/{}", _cliObj1Id), _create_http_params(create_simple_object1()));
    }

    void cli_destroy_obj1()
    {
        REQUIRE(_cliObj1Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj1/delete/{}", _cliObj1Id), {});
    }

    void cli_create_obj2() { _cliObj2Id = _valid_cli_json_get("/api/server1/obj2/create", _create_http_params(create_nested_object())); }

    void cli_read_obj2()
    {
        REQUIRE(_cliObj2Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj2/read/{}", _cliObj2Id), {});
        // auto all = _valid_cli_json_get(fmt::format("/api/server1/obj2/all"), {});
    }

    void cli_edit_obj2()
    {
        REQUIRE(_cliObj2Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj2/edit/{}", _cliObj2Id), _create_http_params(create_nested_object()));
    }

    void cli_destroy_obj2()
    {
        REQUIRE(_cliObj2Id.length() > 1);
        _valid_cli_json_get(fmt::format("/api/server1/obj2/delete/{}", _cliObj2Id), {});
    }
    void cli_call_function()
    {
        auto arg1 = fmt::format("{}", create_uint32());
        auto arg2 = Stencil::Json::Stringify(create_simple_object1());
        _valid_cli_json_get("/api/server1/function1", Params{{"arg1", arg1}, {"arg2", arg2}});
    }

    void svc_create_obj1() {}
    void svc_read_obj1() {}
    void svc_edit_obj1() {}
    void svc_destroy_obj1() {}

    void svc_create_obj2() {}
    void svc_read_obj2() {}
    void svc_edit_obj2() {}
    void svc_destroy_obj2() {}

    void svc_raise_event()
    {
        auto arg1 = create_uint32();
        auto arg2 = create_simple_object1();
        svc->GetInterface<Interfaces::Server1>().Raise_SomethingHappened(arg1, arg2);
    }

    void svc_call_function()
    {
        auto arg1 = create_uint32();
        auto arg2 = create_simple_object1();
        svc->GetInterface<Interfaces::Server1>().Function1(arg1, arg2);
    }

    void svc_state_change()
    {
        Stencil::Transaction<Objects::NestedObject> txn(svc->state);
        {
            auto subtxn = txn.obj1();
            subtxn.set_val1(20);
        }
        svc->OnStateChange(txn);
    }

    std::vector<std::string> _json_lines;
    std::string              _cliObj1Id;
    std::string              _cliObj2Id;

    uint32_t              _count{0};
    std::filesystem::path dbfile{"SaveAndLoad.bin"};

    HttpClientListener _sseListener1{"/api/server1/somethinghappened"};
    HttpClientListener _sseListener2{"/api/server1/objectstore"};
    HttpClientListener _sseListener3{"/api/state"};

    // SSEListener _sseListener3{"/api/server1/obj2/events"};
    std::unique_ptr<Server1Impl> svc;
};

TEST_CASE("WebService-objectstore", "[interfaces]")
{
    std::filesystem::path dbfile{"SaveAndLoad.bin"};

    Tester tester;
    tester.StartListeners();

    tester.cli_call_function();
    tester.svc_call_function();

    tester.cli_create_obj1();
    tester.cli_read_obj1();
    tester.cli_edit_obj1();
    tester.cli_destroy_obj1();
    tester.svc_create_obj1();
    tester.svc_read_obj1();
    tester.svc_edit_obj1();
    tester.svc_destroy_obj1();

    tester.cli_create_obj2();
    tester.cli_read_obj2();
    tester.cli_edit_obj2();
    tester.cli_destroy_obj2();
    tester.svc_create_obj2();
    tester.svc_read_obj2();
    tester.svc_edit_obj2();
    tester.svc_destroy_obj2();

    tester.svc_raise_event();
    tester.svc_call_function();
    tester.svc_state_change();
    std::this_thread::sleep_for(std::chrono::milliseconds(100ms));
}

TEST_CASE("WebService-nolistener", "[interfaces]")
{
    std::filesystem::path dbfile{"SaveAndLoad.bin"};

    Tester tester;
    tester._sseListener1.Start();
    tester.cli_create_obj1();
    tester.svc_state_change();
    std::this_thread::sleep_for(std::chrono::milliseconds(100ms));
}
