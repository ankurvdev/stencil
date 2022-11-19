#include "Interfaces.pidl.h"
#include "TestUtils.h"

#include <condition_variable>
using namespace std::chrono_literals;

/*
interface Server1 {
    event SomethingHappened(uint32 arg1, SimpleObject1 arg2);
    objectstore SimpleObject1   obj1;
    objectstore NestedObject    obj2;
    dict<uint32, SimpleObject1> Function1(uint32 arg1, SimpleObject1 arg2);
}
*/
/// Generated code begins
struct Server1Impl : Interfaces::Server1
{
    Server1Impl()           = default;
    ~Server1Impl() override = default;
    CLASS_DELETE_COPY_AND_MOVE(Server1Impl);

    std::unordered_map<uint32_t, Interfaces::SimpleObject1> Function1(uint32_t const& arg1, Interfaces::SimpleObject1 const& arg2) override
    {
        std::unordered_map<uint32_t, Interfaces::SimpleObject1> retval;

        auto key    = arg1 + 1;
        auto copied = arg2;
        copied.val1 += 1;
        copied.val2 += 1;
        copied.val3 += 1;
        copied.val5 += 1.0;
        retval[key] = std::move(copied);

        // TODO Raise_SomethingHappened(key, copied);
        return retval;
    }
    ObjectStore objectstore;
    // Event listeners ?
};

std::unique_ptr<Interfaces::Server1> Interfaces::Server1::Create()
{
    return std::make_unique<Server1Impl>();
}
// Generated code ends

struct Tester
{
    struct SSEListener
    {
        SSEListener(std::string_view const& url) : _ssecli("localhost", 44444), _url(url) {}
        CLASS_DELETE_COPY_AND_MOVE(SSEListener);

        bool _ChunkCallback(const char* data, uint64_t /*len*/)
        {
            std::unique_lock<std::mutex> guard(_mutex);
            _sseData << data;
            _responseRecieved = true;
            _cv.notify_all();
            return true;
        }
        void _SSEListener()
        {
            _ssecli.set_follow_location(true);
            const httplib::Headers headers   = {{"Accept", "text/event-stream"}, {"Connection", "Keep-Alive"}};
            bool                   reconnect = true;
            while (!_stopRequested && reconnect)
            {
                auto res = _ssecli.Get(_url, headers, [&](const char* data, uint64_t len) { return this->_ChunkCallback(data, len); });
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
        }

        bool                    _responseRecieved;
        std::atomic<bool>       _stopRequested;
        std::mutex              _mutex;
        httplib::Client         _ssecli;
        std::condition_variable _cv;
        std::ostringstream      _sseData;
        std::thread             _sseListener;
        std::string             _url;
    };

    Tester()
    {
        svc.StartOnPort(44444);
        _sseListener1.Start();
        _sseListener2.Start();
        _sseListener3.Start();
    }
    ~Tester() = default;
    CLASS_DELETE_COPY_AND_MOVE(Tester);

    void cli_create_obj1() {}
    void cli_read_obj1() {}
    void cli_edit_obj1() {}
    void cli_destroy_obj1() {}

    void svc_create_obj1() {}
    void svc_read_obj1() {}
    void svc_edit_obj1() {}
    void svc_destroy_obj1() {}

    void cli_create_obj2() {}
    void cli_read_obj2() {}
    void cli_edit_obj2() {}
    void cli_destroy_obj2() {}

    void svc_create_obj2() {}
    void svc_read_obj2() {}
    void svc_edit_obj2() {}
    void svc_destroy_obj2() {}

    void svc_raise_event() {}
    void svc_call_function() {}

    SSEListener _sseListener1{"/api/server1/events"};
    SSEListener _sseListener2{"/api/server1/obj1/events"};
    SSEListener _sseListener3{"/api/server1/obj2/events"};

    Stencil::WebService<Interfaces::Server1> svc;
};

TEST_CASE("WebService-objectstore", "[interfaces]")
{
    Tester tester;
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
}