#include "ObjectsTester.h"
#include "TestUtils.h"

#include <stencil/webservice_boostbeast.h>

SUPPRESS_WARNINGS_START

SUPPRESS_MSVC_WARNING(4365)    // conversion from 'const char' to 'unsigned char', signed/unsigned mismatch
SUPPRESS_MSVC_WARNING(4355)    //'this' : used in base member initializer list
SUPPRESS_MSVC_WARNING(4855)    // implicit capture of 'this' via '[=]' is deprecated
SUPPRESS_MSVC_WARNING(4548)    // expression before comma has no effect; expected expression with side - effect
SUPPRESS_MSVC_WARNING(4702)    // Unreachable code
SUPPRESS_MSVC_WARNING(4668)    // not defined as a preprocessor macro
SUPPRESS_MSVC_WARNING(5039)    // pointer or reference to potentially throwing function passed to 'extern)
SUPPRESS_MSVC_WARNING(4191)    // type cast': unsafe conversion
SUPPRESS_MSVC_WARNING(5262)    // implicit fall-through occurs here;
SUPPRESS_CLANG_WARNING("-Weverything")
SUPPRESS_GCC_WARNING("-Wmaybe-uninitialized")
#include <limits.h>

#include <httplib.h>

SUPPRESS_WARNINGS_END

static_assert(Stencil::Database::ConceptRecord<uint32_t>);
static_assert(Stencil::Database::ConceptTrivial<uint32_t>);

static_assert(Stencil::Database::ConceptFixedSize<uint32_t>);
static_assert(Stencil::Database::ConceptBlob<shared_string>);
static_assert(Stencil::Database::ConceptComplex<std::unordered_map<uint32_t, uint32_t>>);

#include <condition_variable>
#include <memory>

using namespace std::chrono_literals;
static auto CreateCLI()
{
    return httplib::Client("localhost", 44444);
}

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

struct Server1Impl : Stencil::WebServiceT<Server1Impl, Interfaces::Server1>
{
    Server1Impl() { objects.Init(std::filesystem::path("SaveAndLoad.bin")); }
    ~Server1Impl() override = default;
    CLASS_DELETE_COPY_AND_MOVE(Server1Impl);

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

    // Event listeners ?
};

// Generated code ends

struct Tester : ObjectsTester
{

    struct SSEListener
    {
        SSEListener(std::string_view const& url) : _url(url) {}
        CLASS_DELETE_COPY_AND_MOVE(SSEListener);
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
        bool _ChunkCallback(const char* data, size_t len)
        {
            if (data[len - 1] == '\0') len--;
            std::unique_lock<std::mutex> guard(_mutex);
            _sseData.push_back(std::string(data, len));
            _responseRecieved = true;
            _cv.notify_all();
            return true;
        }
        SUPPRESS_WARNINGS_END
        void _SSEListener()
        {
            _ssecli.set_follow_location(true);
            const httplib::Headers headers   = {{"Accept", "text/event-stream"}, {"Connection", "Keep-Alive"}};
            bool                   reconnect = true;
            while (!_stopRequested && reconnect)
            {
                auto res = _ssecli.Get(_url, headers, [&](const char* data, size_t len) { return this->_ChunkCallback(data, len); });
                if (res)
                {
                    // printf("HTTP %i\n", res->status);
                    // check that response code is 2xx
                    reconnect = (res->status / 100) == 2;
                }
                else { std::this_thread::sleep_for(std::chrono::milliseconds(100ms)); }
            }
        }

        void Start()
        {
            _responseRecieved = false;
            _stopRequested    = false;
            _sseListener      = std::thread([&]() { this->_SSEListener(); });
            std::unique_lock<std::mutex> guard(_mutex);
            _cv.wait(guard, [&]() { return this->_responseRecieved; });
        }

        ~SSEListener()
        {
            _stopRequested = true;
            _ssecli.stop();
            if (_sseListener.joinable()) _sseListener.join();
        }

        bool                     _responseRecieved;
        std::atomic<bool>        _stopRequested;
        std::mutex               _mutex;
        httplib::Client          _ssecli = CreateCLI();
        std::condition_variable  _cv;
        std::vector<std::string> _sseData;
        std::thread              _sseListener;
        std::string              _url;
    };

    Tester()
    {
        if (std::filesystem::exists(dbfile)) std::filesystem::remove(dbfile);
        svc = std::make_unique<Server1Impl>();
        svc->StartOnPort(44444);
        _sseListener1.Start();
        _sseListener2.Start();
        //_sseListener2.Start();
        // _sseListener3.Start();
    }

    ~Tester()
    {
        svc->StopDaemon();
        svc.reset();
        if (std::filesystem::exists(dbfile)) std::filesystem::remove(dbfile);

        TestCommon::CheckResource<TestCommon::JsonFormat>(_json_lines, "json");
        TestCommon::CheckResource<SSEFormat>(TestCommon::ResplitLines(_sseListener1._sseData), "server1_somethinghappened");
        TestCommon::CheckResource<SSEFormat>(TestCommon::ResplitLines(_sseListener2._sseData), "server1_objectstore");
    }

    CLASS_DELETE_COPY_AND_MOVE(Tester);

    template <typename T> auto _create_http_params(T const& obj)
    {
        httplib::Params params;
        Stencil::Visitor<T>::VisitAll(obj, [&](auto const& key, auto const& val) {
            std::string keystr = Stencil::Serialize<Stencil::ProtocolString>(key).str();
            std::string valstr = Stencil::Serialize<Stencil::ProtocolJsonVal>(val).str();
            params.insert({std::move(keystr), std::move(valstr)});
        });
        return params;
    }

    auto _valid_cli_call_get(std::string const& path, httplib::Params const& params)
    {
        auto response = CreateCLI().Get(path, params, httplib::Headers(), httplib::Progress());
        REQUIRE(response == true);
        REQUIRE(response->status == 200);
        return response->body;
    }

    auto _valid_cli_json_get(std::string const& path, httplib::Params const& params)
    {
        auto json = _valid_cli_call_get(path, params);
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
        _valid_cli_json_get("/api/server1/function1", httplib::Params{{"arg1", arg1}, {"arg2", arg2}});
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

    std::vector<std::string> _json_lines;
    std::string              _cliObj1Id;
    std::string              _cliObj2Id;

    uint32_t              _count{0};
    std::filesystem::path dbfile{"SaveAndLoad.bin"};

    SSEListener _sseListener1{"/api/server1/somethinghappened"};
    SSEListener _sseListener2{"/api/server1/objectstore"};
    // SSEListener _sseListener3{"/api/server1/obj2/events"};
    std::unique_ptr<Server1Impl> svc;
};

TEST_CASE("WebService-objectstore", "[interfaces]")
{
    std::filesystem::path dbfile{"SaveAndLoad.bin"};

    Tester tester;
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
    std::this_thread::sleep_for(std::chrono::milliseconds(100ms));
}
