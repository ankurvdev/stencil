#include "CommonMacros.h"
#include "Interfaces.pidl.h"
#include "ObjectsTester.h"
#include "TestUtils.h"
#include "stencil/typetraits_path.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <stencil/webservice_boostbeast.h>
#include <unordered_map>

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

// NOLINTBEGIN(readability-magic-numbers, cppcoreguidelines-pro-type-reinterpret-cast, readability-function-cognitive-complexity)
static_assert(Stencil::Database::ConceptRecord<uint32_t>);
static_assert(Stencil::Database::ConceptTrivial<uint32_t>);

static_assert(Stencil::Database::ConceptFixedSize<uint32_t>);
static_assert(Stencil::Database::ConceptBlob<shared_string>);
static_assert(Stencil::Database::ConceptComplex<std::unordered_map<uint32_t, uint32_t>>);

using namespace std::chrono_literals;
namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
using net::ip::tcp;

template <typename TSvc, typename TImpl>
static std::unordered_map<uint32_t, Objects::SimpleObject1>
Function1Impl(TSvc& svc, TImpl& obj, uint32_t const& arg1, Objects::SimpleObject1 const& arg2)
{
    std::unordered_map<uint32_t, Objects::SimpleObject1> retval;

    auto key    = arg1 + 1;
    auto copied = arg2;
    copied.val1 += 1;
    copied.val2 += 1;
    copied.val3 += 1;
    copied.val5 += 1.0;
    retval[key] = copied;

    svc.RaiseEvent(obj, Interfaces::Server1::Args_SomethingHappened{.arg1 = key, .arg2 = copied});
    return retval;
}

namespace
{
struct HttpClientListener
{
    using Params = std::unordered_map<std::string, std::string>;

    static constexpr std::string_view LocalHostName = "127.0.0.1";
    static constexpr std::string_view Port          = "44444";

    explicit HttpClientListener(std::string_view const& urlIn) : url(urlIn), ioc(), resolver(ioc), stream(ioc)
    {
        req = {http::verb::get, url, 11 /*HTTP Version 1.1*/};
        req.set(http::field::host, LocalHostName);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::accept, "text/event-stream");
        req.set(http::field::connection, "Keep-Alive");
    }

    CLASS_DELETE_COPY_AND_MOVE(HttpClientListener);
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    bool ChunkCallback(char const* data, size_t len)
    {
        if (data[len - 1] == '\0') len--;
        sseData.emplace_back(data, len);
        return true;
    }
    SUPPRESS_WARNINGS_END
    void SseListener()
    {
        bool reconnect = true;
        while (!stopRequested && reconnect)
        {
            stream.connect(resolver.resolve(LocalHostName, Port));
            // Send request
            http::write(stream, req);
            boost::asio::streambuf buf(4096);

            // Read header first
            http::response_parser<http::empty_body> parser;
            parser.body_limit(0);    // no body in header
            http::read_header(stream, buf, parser);

            auto const& res = parser.get();
            if (res.result() != http::status::ok) { throw std::runtime_error("Bad response"); }
            beast::error_code ec;
            {
                std::unique_lock<std::mutex> guard(mutex);
                responseRecieved = true;
                cv.notify_all();
            }

            while (!stopRequested)
            {
                auto bytes = boost::asio::read_until(stream.socket(), buf, "\r\n", ec);
                if (bytes == 0 && ec)
                {
                    if (ec == net::error::eof || ec == beast::http::error::end_of_stream) { continue; }
                    fmt::print(stderr, "sse_listener[{}]:{} buf.avail={} ec={} TODO: Throw\n", url, __LINE__, buf.size(), ec);
                    throw beast::system_error(ec);
                }

                if (ec)
                {
                    fmt::print(stderr,
                               "sse_listener[{}]:{} buf.avail={} ec={} error reading chunk-encoded-part-size bytes: {}\n",
                               url,
                               __LINE__,
                               buf.size(),
                               ec,
                               bytes);
                }

                auto const* bufchars = reinterpret_cast<char const*>(buf.data().data());
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
                                    url,
                                    __LINE__,
                                    buf.size(),
                                    std::string_view(bufchars, bytes - 2)));
                }

                buf.consume(bytes);

                for (size_t i = 0, remaining = messageSize; i < messageSize;)
                {
                    size_t bufPrevSize = buf.size();
                    size_t readBytes   = std::min(remaining, buf.max_size());
                    if (bufPrevSize < readBytes)
                    {
                        bytes = boost::asio::read(stream.socket(), buf, boost::asio::transfer_exactly(readBytes - bufPrevSize), ec);
                        bytes += bufPrevSize;
                    }
                    else
                    {
                        bytes = readBytes;
                        ec    = {};
                    }

                    if (bytes != readBytes)
                    {    //
                        fmt::print(stderr,
                                   "sse_listener[{}]:{} buf.avail={}=>{} ec={} recieved: {} != requested: {}\n",
                                   url,
                                   __LINE__,
                                   bufPrevSize,
                                   buf.size(),
                                   ec,
                                   bytes,
                                   readBytes);
                        assert(ec);    // something catastrophic must have happened
                        // truncate how much we read to how much is available
                        readBytes = std::min(static_cast<size_t>(buf.size()), readBytes);
                    }

                    // buf.size() !=  read_bytes is pretty commmon.
                    // Just means that read_until actually read and buffered a lot more than it said it did
                    // fmt::print(stderr, "Mismatch buf.size() {} != read_bytes {} ec = {}\n", buf.size(), read_bytes, ec);

                    bufchars = reinterpret_cast<char const*>(buf.data().data());
                    if (messageSize != 2 || readBytes != 2 || bufchars[0] != '\n' || bufchars[1] != '\n')
                    {
                        ChunkCallback(bufchars, readBytes);
                    }
                    i += readBytes;
                    remaining -= readBytes;
                    buf.consume(readBytes);

                    if (ec && bytes == readBytes)
                    {    //
                        fmt::print(stderr, "sse_listener[{}]:{} buf.avail={}=>{} ec={}\n", url, __LINE__, bufPrevSize, buf.size(), ec);
                    }

                    // fmt::print(" {}/{}", bytes, remaining);
                    if (ec && (ec == net::error::eof || ec == beast::http::error::end_of_stream))
                    {
                        continue;
                        // Continue flushing the buf
                    }

                    if (ec)
                    {
                        fmt::print(stderr, "sse_listener[{}]:{} buf.avail={} ec={}\n", url, __LINE__, buf.size(), ec);
                        if ((stopRequested && (ec == net::error::operation_aborted || ec == boost::asio::error::interrupted))) { return; }
                        fmt::print(stderr, "SSEListener encountered error :{}\n", ec);
                        return;
                        // throw beast::system_error(ec);
                    }
                }
            }

            // Gracefully close
            ec = stream.socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != net::error::not_connected)
            {    //
                fmt::print(stderr, "Error in shutdown: {}\n", ec);
            }
        }
    }

    void Start()
    {
        activated        = true;
        responseRecieved = false;
        stopRequested    = false;

        sseListener = std::thread([&] {
            try
            {
                this->SseListener();
            } catch (std::exception const&) { fmt::print(stderr, "Exception caught in SSEListener\n"); }
        });
        std::unique_lock<std::mutex> guard(mutex);
        cv.wait(guard, [&] { return this->responseRecieved; });
    }

    void RequestStop()
    {
        stopRequested = true;
        // beast::error_code ec;
        //  ec = stream.socket().close(ec);
    }

    void Stop()
    {
        RequestStop();
        if (sseListener.joinable()) sseListener.join();
    }

    static auto Head(std::string_view const& target, Params const& params)
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
            http::request<http::string_body> req{http::verb::head, url.encoded_target(), 11 /*HTTP Version 1.1*/};
            req.set(http::field::host, LocalHostName);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(http::field::accept, "application/json");
            http::write(stream, req);
        }
        beast::flat_buffer                      buffer;
        http::response_parser<http::empty_body> parser;
        parser.skip(true);
        beast::error_code                            ec;
        auto                                         bytesRead = http::read_header(stream, buffer, parser);
        std::unordered_map<std::string, std::string> responseFields;
        auto                                         res = parser.get();
        for (auto const& field : res) { responseFields[field.name_string()] = field.value(); }
        responseFields["bytesRead"] = std::to_string(bytesRead);
        if (buffer.size() != 0) { throw std::runtime_error("ERROR: server sent body bytes: " + std::to_string(buffer.size())); }
        if (stream.socket().read_some(buffer.prepare(8192), ec) != 0) { throw std::runtime_error("ERROR: server sent body bytes"); }
        if (ec != net::error::eof) { throw beast::system_error(ec); }
        return responseFields;
    }

    static void Download(std::string_view const& target, Params const& params, std::filesystem::path const& output)
    {
        auto          content = Get(target, params);
        std::ofstream ofs(output);
        ofs << content;
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

    bool                     activated{false};
    bool                     responseRecieved{false};
    std::atomic<bool>        stopRequested;
    std::mutex               mutex;
    std::condition_variable  cv;
    std::vector<std::string> sseData;
    std::thread              sseListener;
    std::string              url;

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
            auto const&      act    = actual[i];
            auto&            exp    = expected[i];
            std::string_view prefix = "data: ";
            if (act == exp) continue;
            if (act.size() < prefix.size() || exp.size() < prefix.size()) return false;
            if (act.substr(0, prefix.size()) != exp.substr(0, prefix.size())) return false;
            if (!act.starts_with(prefix)) return false;
            if (!TestCommon::JsonStringEqual(act.substr(prefix.size()), exp.substr(prefix.size()))) return false;
        }

        return true;
    }
};

struct Server1Impl
    : Stencil::websvc::WebServiceT<Server1Impl, Interfaces::Server1, Stencil::websvc::WebSynchronizedState<Objects::NestedObject>>,
      Interfaces::Server1::Interface
{
    Server1Impl() { objects.Init(std::filesystem::path("SaveAndLoad.bin")); }
    ~Server1Impl() override = default;
    CLASS_DELETE_COPY_AND_MOVE(Server1Impl);
    static std::string_view   Name() { return "state"; }
    [[nodiscard]] std::string StateStringify() const { return Stencil::Json::Stringify(state); }

    struct EditCtx
    {
        EditCtx(Server1Impl* thatIn LFTBND, Objects::NestedObject& stateIn LFTBND) :
            txn(Stencil::CreateRootTransaction<Objects::NestedObject>(stateIn)), that(thatIn)
        {}
        ~EditCtx() { that->OnStateChange(txn); }
        CLASS_DELETE_COPY_AND_MOVE(EditCtx);
        auto& TXN() LFTBND { return txn; }

        Stencil::Transaction<Objects::NestedObject> txn;
        Server1Impl*                                that;
    };

    auto EditContext() LFTBND { return EditCtx(this, state); }

    std::unordered_map<uint32_t, Objects::SimpleObject1> Function1(uint32_t const& arg1, Objects::SimpleObject1 const& arg2) override
    { return Function1Impl(*this, *this, arg1, arg2); }

    void                    Function2() override {}
    void                    Function3(uint32_t const& /* arg1 */) override {}
    Stencil::websvc::File   GetFile([[maybe_unused]] Stencil::WFPath const& p) override { throw std::logic_error("Not implemented"); }
    Stencil::websvc::Stream GetStream([[maybe_unused]] Stencil::RFPath const& p) override { throw std::logic_error("Not implemented"); }

    void OnStateChange(Stencil::Transaction<Objects::NestedObject>::View const& txnv) { NotifyStateChanged(txnv); }

    Objects::NestedObject state;
    // Event listeners ?
};

// struct NoEventImpl : Stencil::websvc::WebServiceT<NoEventImpl, Interfaces::NoEvent>, Interfaces::NoEvent::Interface
//{
//     NoEventImpl()           = default;
//     ~NoEventImpl() override = default;
//     CLASS_DELETE_COPY_AND_MOVE(NoEventImpl);
//
//     void Function2() override {}
//     void Function3(uint32_t const& /* arg1 */) override {}
// };

struct SvcSeparateImplSvc
    : Stencil::websvc::WebServiceT<SvcSeparateImplSvc, Interfaces::Server1, Stencil::websvc::WebSynchronizedState<Objects::NestedObject>>
{
    SvcSeparateImplSvc() { objects.Init(std::filesystem::path("SaveAndLoad.bin")); }
    ~SvcSeparateImplSvc() = default;
    CLASS_DELETE_COPY_AND_MOVE(SvcSeparateImplSvc);

    static std::string_view   Name() { return "state"; }
    [[nodiscard]] std::string StateStringify() const { return Stencil::Json::Stringify(state); }

    struct EditCtx
    {
        EditCtx(SvcSeparateImplSvc* thatIn LFTBND, Objects::NestedObject& stateIn LFTBND) :
            txn(Stencil::CreateRootTransaction<Objects::NestedObject>(stateIn)), that(thatIn)
        {}
        ~EditCtx() { that->OnStateChange(txn); }
        CLASS_DELETE_COPY_AND_MOVE(EditCtx);
        auto& TXN() LFTBND { return txn; }

        Stencil::Transaction<Objects::NestedObject> txn;
        SvcSeparateImplSvc*                         that;
    };

    auto EditContext() LFTBND { return EditCtx(this, state); }

    void OnStateChange(Stencil::Transaction<Objects::NestedObject>::View const& txnv) { NotifyStateChanged(txnv); }

    Objects::NestedObject                           state;
    std::unique_ptr<Interfaces::Server1::Interface> impl;
};
}    // namespace

template <> struct Stencil::InterfaceSvcTraits<SvcSeparateImplSvc, Interfaces::Server1>
{
    static auto& QueryInterface(SvcSeparateImplSvc& impl LFTBND) { return *impl.impl; }
};
namespace
{
struct ImplSeparateImplSvc : Interfaces::Server1::Interface
{
    std::unordered_map<uint32_t, Objects::SimpleObject1> Function1(uint32_t const& arg1, Objects::SimpleObject1 const& arg2) override
    { return Function1Impl(*svc, *svc, arg1, arg2); }

    void                Function2() override {}
    void                Function3(uint32_t const& /* arg1 */) override {}
    SvcSeparateImplSvc* svc{nullptr};

    Stencil::websvc::File   GetFile([[maybe_unused]] Stencil::WFPath const& p) override { throw std::logic_error("Not implemented"); }
    Stencil::websvc::Stream GetStream([[maybe_unused]] Stencil::RFPath const& p) override { throw std::logic_error("Not implemented"); }
};

struct ImplNoInterfaceSvc;

struct SvcNoInterfaceSvc
    : Stencil::websvc::WebServiceT<SvcNoInterfaceSvc, Interfaces::Server1, Stencil::websvc::WebSynchronizedState<Objects::NestedObject>>
{
    SvcNoInterfaceSvc() { objects.Init(std::filesystem::path("SaveAndLoad.bin")); }
    ~SvcNoInterfaceSvc() = default;
    CLASS_DELETE_COPY_AND_MOVE(SvcNoInterfaceSvc);

    static std::string_view   Name() { return "state"; }
    [[nodiscard]] std::string StateStringify() const { return Stencil::Json::Stringify(state); }

    struct EditCtx
    {
        EditCtx(SvcNoInterfaceSvc* thatIn LFTBND, Objects::NestedObject& stateIn LFTBND) :
            txn(Stencil::CreateRootTransaction<Objects::NestedObject>(stateIn)), that(thatIn)
        {}
        ~EditCtx() { that->OnStateChange(txn); }
        CLASS_DELETE_COPY_AND_MOVE(EditCtx);
        auto& TXN() LFTBND { return txn; }

        Stencil::Transaction<Objects::NestedObject> txn;
        SvcNoInterfaceSvc*                          that;
    };

    auto EditContext() LFTBND { return EditCtx(this, state); }

    void OnStateChange(Stencil::Transaction<Objects::NestedObject>::View const& txnv) { NotifyStateChanged(txnv); }

    Objects::NestedObject state;
    ImplNoInterfaceSvc*   impl{nullptr};
};

struct ImplNoInterfaceSvc
{
    static constexpr size_t LargeFileIntCount = 32z * 1024z;

    [[maybe_unused]] std::unordered_map<uint32_t, Objects::SimpleObject1> Function1(uint32_t const&               arg1,    // NOLINT
                                                                                    Objects::SimpleObject1 const& arg2)
    { return Function1Impl(*svc, *svc, arg1, arg2); }

    [[maybe_unused]] void Function2() {}
    [[maybe_unused]] void Function3(uint32_t const& /* arg1 */) {}

    [[maybe_unused]] auto GetFile(std::filesystem::path const& wpath)
    {
        outpath = std::filesystem::temp_directory_path() / wpath;
        std::ofstream ofs(outpath);
        for (size_t i = 0; i < LargeFileIntCount; ++i) { ofs << i << "\n"; }
        return Stencil::websvc::File{outpath};
    }

    [[maybe_unused]] [[noreturn]] Stencil::websvc::Stream GetStream(std::filesystem::path const& /* rpath */)    // NOLINT
    { TODO("NotImpl"); }                                                                                         // NOLINT

    std::filesystem::path outpath;
    SvcNoInterfaceSvc*    svc{nullptr};
};

// Generated code ends
}    // namespace

template <> struct Stencil::InterfaceSvcTraits<SvcNoInterfaceSvc, Interfaces::Server1>
{
    static auto& QueryInterface(SvcNoInterfaceSvc& svc) { return *svc.impl; }
};

namespace
{
template <typename TSvc> struct Tester : ObjectsTester
{
    using Params = HttpClientListener::Params;
    Tester() : svc(std::make_unique<TSvc>())
    {
        svc->StartOnPort(44444, 4);
        tempFiles.emplace_back("SaveAndLoad.bin");
    }

    ~Tester()
    {
        sseListener1.RequestStop();
        sseListener2.RequestStop();
        sseListener3.RequestStop();

        svc->StopDaemon();
        svc.reset();

        for (auto const& fpath : tempFiles)
        {
            if (std::filesystem::exists(fpath)) std::filesystem::remove(fpath);
        }

        if (std::filesystem::exists(dbfile)) std::filesystem::remove(dbfile);
        TestCommon::CheckResource<TestCommon::JsonFormat>(jsonLines, "json");

        auto checkListener = [&](HttpClientListener& listener, std::string_view const& name) {
            if (listener.activated) { TestCommon::CheckResource<SSEFormat>(TestCommon::ResplitLines(listener.sseData), name); }
        };
        checkListener(sseListener1, "server1_somethinghappened");
        checkListener(sseListener2, "server1_objectstore");
        checkListener(sseListener3, "server1_statenotifications");
    }

    void StartListeners()
    {
        sseListener1.Start();
        sseListener2.Start();
        sseListener3.Start();
    }

    CLASS_DELETE_COPY_AND_MOVE(Tester);

    template <typename T> auto CreateHttpParams(T const& obj)
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

    auto ValidCliJsonGet(std::string const& path, Params const& params)
    {
        auto json = HttpClientListener::Get(path, params);
        CHECK(!json.empty());
        jsonLines.push_back(json);
        return json;
    }

    void CliCreateObj1()
    {
        auto obj1 = CreateSimpleObject1();
        cliObj1Id = ValidCliJsonGet("/api/server1/obj1/create", CreateHttpParams(obj1));
    }

    void CliReadObj1()
    {
        REQUIRE(cliObj1Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj1/read/{}", cliObj1Id), {});
        // auto all = _valid_cli_json_get(fmt::format("/api/server1/obj1/all"), {});
    }

    void CliEditObj1()
    {
        REQUIRE(cliObj1Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj1/edit/{}", cliObj1Id), CreateHttpParams(CreateSimpleObject1()));
    }

    void CliDestroyObj1()
    {
        REQUIRE(cliObj1Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj1/delete/{}", cliObj1Id), {});
    }

    void CliCreateObj2() { cliObj2Id = ValidCliJsonGet("/api/server1/obj2/create", CreateHttpParams(CreateNestedObject())); }

    void CliReadObj2()
    {
        REQUIRE(cliObj2Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj2/read/{}", cliObj2Id), {});
        // auto all = _valid_cli_json_get(fmt::format("/api/server1/obj2/all"), {});
    }

    void CliEditObj2()
    {
        REQUIRE(cliObj2Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj2/edit/{}", cliObj2Id), CreateHttpParams(CreateNestedObject()));
    }

    void CliDestroyObj2()
    {
        REQUIRE(cliObj2Id.length() > 1);
        ValidCliJsonGet(fmt::format("/api/server1/obj2/delete/{}", cliObj2Id), {});
    }
    void CliCallFunction()
    {
        auto arg1 = fmt::format("{}", CreateUint32());
        auto arg2 = Stencil::Json::Stringify(CreateSimpleObject1());
        ValidCliJsonGet("/api/server1/function1", Params{{"arg1", arg1}, {"arg2", arg2}});
    }
    void CliRequestStateChange1() { ValidCliJsonGet("/api/state/apply", Params{{"obj1.val1", "20"}, {"obj2.val1", "true"}}); }
    void CliRequestStateChange2() { ValidCliJsonGet("/api/state/apply", Params{{"obj1.val1", "-20"}, {"obj2.val1", "false"}}); }

    void CliGetFile()
    {
        auto reqfname = (std::filesystem::temp_directory_path() / CreateFilePath());
        auto resfname = (std::filesystem::temp_directory_path() / CreateFilePath());
        tempFiles.emplace_back(reqfname);
        tempFiles.emplace_back(resfname);
        HttpClientListener::Download("/api/server1/getfile", Params{{"p", reqfname.filename().string()}}, resfname);
        TestCommon::CheckFileEqual<TestCommon::StrFormat>(resfname, reqfname);
    }
    void CliGetFileHead()
    {
        auto reqfname = (std::filesystem::temp_directory_path() / CreateFilePath());
        auto resfname = (std::filesystem::temp_directory_path() / CreateFilePath());
        tempFiles.emplace_back(reqfname);
        tempFiles.emplace_back(resfname);
        auto response = HttpClientListener::Head("/api/server1/getfile", Params{{"p", reqfname.filename().string()}});
        CHECK(response["Content-Length"] == std::to_string(std::filesystem::file_size(reqfname)));
        CHECK(response["Content-Type"] == "application/octet-stream");
        CHECK(response["Accept-Ranges"] == "bytes");
        CHECK(response["Server"] == "stencil_webserver");
        CHECK(response["bytesRead"] == "164");
    }
    void SvcCreateObj1() {}
    void SvcReadObj1() {}
    void SvcEditObj1() {}
    void SvcDestroyObj1() {}

    void SvcCreateObj2() {}
    void SvcReadObj2() {}
    void SvcEditObj2() {}
    void SvcDestroyObj2() {}

    void SvcCallFunction()
    {
        auto arg1 = CreateUint32();
        auto arg2 = CreateSimpleObject1();
        Stencil::InterfaceSvcTraits<TSvc, Interfaces::Server1>::QueryInterface(*svc).Function1(arg1, arg2);
    }

    void SvcStateChange()
    {
        Stencil::Transaction<Objects::NestedObject> txn(svc->state);
        {
            auto subtxn = txn.obj1();
            subtxn.set_val1(20);
        }
        svc->OnStateChange(txn);
    }
    std::vector<std::filesystem::path> tempFiles;

    std::vector<std::string> jsonLines;
    std::string              cliObj1Id;
    std::string              cliObj2Id;

    uint32_t              count{0};
    std::filesystem::path dbfile{"SaveAndLoad.bin"};
    HttpClientListener    sseListener1{"/api/server1/somethinghappened"};
    HttpClientListener    sseListener2{"/api/server1/objectstore"};
    HttpClientListener    sseListener3{"/api/state"};

    // SSEListener _sseListener3{"/api/server1/obj2/events"};
    std::unique_ptr<TSvc> svc;
};
}    // namespace

TEST_CASE("WebService-objectstore", "[interfaces]")
{
    Tester<Server1Impl> tester;
    tester.StartListeners();

    tester.CliCallFunction();
    tester.SvcCallFunction();

    tester.CliCreateObj1();
    tester.CliReadObj1();
    tester.CliEditObj1();
    tester.CliDestroyObj1();
    tester.SvcCreateObj1();
    tester.SvcReadObj1();
    tester.SvcEditObj1();
    tester.SvcDestroyObj1();

    tester.CliCreateObj2();
    tester.CliReadObj2();
    tester.CliEditObj2();
    tester.CliDestroyObj2();
    tester.SvcCreateObj2();
    tester.SvcReadObj2();
    tester.SvcEditObj2();
    tester.SvcDestroyObj2();

    tester.svc->Raise_SomethingHappened(tester.CreateUint32(), tester.CreateSimpleObject1());
    tester.SvcCallFunction();
    tester.SvcStateChange();
    tester.CliRequestStateChange1();
    tester.CliRequestStateChange2();
}

TEST_CASE("WebService-nolistener", "[interfaces]")
{

    Tester<Server1Impl> tester;
    tester.sseListener1.Start();
    tester.CliCreateObj1();
    tester.SvcStateChange();
    tester.CliRequestStateChange1();
    tester.CliRequestStateChange2();
}

TEST_CASE("WebService-SvcSeparateImplSvc", "[interfaces]")
{
    auto                       impl = std::make_unique<ImplSeparateImplSvc>();
    Tester<SvcSeparateImplSvc> tester;
    impl->svc        = tester.svc.get();
    tester.svc->impl = std::move(impl);

    tester.StartListeners();

    tester.CliCallFunction();
    tester.SvcCallFunction();

    tester.CliCreateObj1();
    tester.CliReadObj1();
    tester.CliEditObj1();
    tester.CliDestroyObj1();
    tester.SvcCreateObj1();
    tester.SvcReadObj1();
    tester.SvcEditObj1();
    tester.SvcDestroyObj1();

    tester.CliCreateObj2();
    tester.CliReadObj2();
    tester.CliEditObj2();
    tester.CliDestroyObj2();
    tester.SvcCreateObj2();
    tester.SvcReadObj2();
    tester.SvcEditObj2();
    tester.SvcDestroyObj2();

    // tester.SvcRaiseEvent();
    tester.svc->Raise_SomethingHappened(tester.CreateUint32(), tester.CreateSimpleObject1());

    tester.SvcCallFunction();
    tester.SvcStateChange();
    tester.CliRequestStateChange1();
    tester.CliRequestStateChange2();
}

TEST_CASE("WebService-NoInterface", "[websvc]")
{
    ImplNoInterfaceSvc        impl;
    Tester<SvcNoInterfaceSvc> tester;
    tester.svc->impl = &impl;
    impl.svc         = tester.svc.get();
    tester.StartListeners();

    tester.CliCallFunction();
    tester.SvcCallFunction();

    tester.CliCreateObj1();
    tester.CliReadObj1();
    tester.CliEditObj1();
    tester.CliDestroyObj1();
    tester.SvcCreateObj1();
    tester.SvcReadObj1();
    tester.SvcEditObj1();
    tester.SvcDestroyObj1();

    tester.CliCreateObj2();
    tester.CliReadObj2();
    tester.CliEditObj2();
    tester.CliDestroyObj2();
    tester.SvcCreateObj2();
    tester.SvcReadObj2();
    tester.SvcEditObj2();
    tester.SvcDestroyObj2();

    tester.svc->RaiseEvent(
        *tester.svc, Interfaces::Server1::Args_SomethingHappened{.arg1 = tester.CreateUint32(), .arg2 = tester.CreateSimpleObject1()});

    tester.SvcCallFunction();
    tester.SvcStateChange();
    tester.CliRequestStateChange1();
    tester.CliRequestStateChange2();
    tester.CliGetFileHead();
    tester.CliGetFile();
    // tester.CliGetFileRange();
    // tester.CliGetFileRangeOutofBound();
}

// NOLINTEND(readability-magic-numbers, cppcoreguidelines-pro-type-reinterpret-cast, readability-function-cognitive-complexity)
