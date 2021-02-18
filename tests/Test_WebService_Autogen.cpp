#include "TestUtils.h"

#include <ComplexWebService.pidl.h>
#include <SimpleWebService.pidl.h>

#include <sstream>
#include <string>
#include <string_view>
//
// template <> struct ReflectionBase::InterfaceActivator<SimpleWebService::>
//{
//    std::unique_ptr<zzInterface_Namezz> Activate(uint64_t randomInteger, shared_string randomString)
//    {
//        return zzInterface_Namezz::Create(randomInteger, randomString);
//    }
//};

class CppHttpLib
{
    public:
    CppHttpLib(std::string_view /*proto*/, std::string_view host, uint16_t port, std::string_view get)
    {
        httplib::Client cli(host.data(), port);
        auto            res = cli.Get(get.data());
        if (res && res->status == 200)
        {
            _response = res->body;
        }
        else
        {
            throw std::runtime_error("Unable to get");
        }
    }

    std::string response() { return _response; }

    private:
    std::string _response;
};

#if 0
TEST_CASE("CodeGen::WebService::Autogen", "[WebService]")
{
    SECTION("Positive: SimpleCase")
    {
        WebService<SimpleWebService::TestInterface> svc;
        svc.StartOnPort(44444);
        try
        {
            auto str = CppHttpLib("http", "localhost", 44444, "/TestInterface/create?randomInteger=22&randomString=aadya").response();
            REQUIRE(str.size() == 38);
            REQUIRE(str[0] == '{');
            REQUIRE(str[37] == '}');
        }
        catch (std::exception const& ex)
        {
            FAIL(ex.what());
        }
        svc.StopDaemon();
        svc.WaitForStop();
    }
}
#endif