#include "TestUtils.h"

#include <Catalog.pidl.h>
#include <ComplexWebService.pidl.h>
#include <SimpleWebService.pidl.h>

#include <sstream>
#include <string>
#include <string_view>
struct TestInterfaceImpl : SimpleWebService::TestInterface
{
    TestInterfaceImpl() = default;
    CLASS_DELETE_COPY_AND_MOVE(TestInterfaceImpl);
    virtual SimpleWebService::Data::Data Create(int32_t const& randomInteger, shared_string const& randomString) override
    {
        SimpleWebService::Data::Data data;
        data.randomInteger() = randomInteger;
        data.randomString()  = randomString;
        return data;
    }
};

struct CatalogImpl : Catalog::Catalog
{
    CatalogImpl() { objects.Init(); }
    CLASS_DELETE_COPY_AND_MOVE(CatalogImpl);

    virtual std::vector<::Catalog::File::Data> Search(shared_string const& /*query*/) override
    {
        throw std::logic_error("Not Implemented");
    }
    virtual void RequestDownload(shared_string const& /*link*/) override {}
};

class CppHttpLib
{
    public:
    CppHttpLib(std::string_view /*proto*/, std::string_view host, uint16_t port, std::string_view get)
    {
        httplib::Client cli(host.data(), port);
        auto            res = cli.Get(get.data());
        if (res && res->status == 200) { _response = res->body; }
        else
        {
            throw std::runtime_error("Unable to get");
        }
    }

    std::string response() { return _response; }

    private:
    std::string _response;
};

TEST_CASE("CodeGen::WebService::Autogen", "[WebService]")
{
    SECTION("Positive: SimpleCase")
    {
        Stencil::WebService<TestInterfaceImpl> svc;
        svc.StartOnPort(44444);
        try
        {
            auto str = CppHttpLib("http", "localhost", 44444, "/TestInterface/create?randomInteger=22&randomString=aadya").response();
            REQUIRE(str == R"({"randomInteger":22,"randomString":aadya})");
        } catch (std::exception const& ex)
        {
            FAIL(ex.what());
        }
        svc.StopDaemon();
        svc.WaitForStop();
    }
}

TEST_CASE("CatalogImpl", "[WebService]")
{
    SECTION("CatalogImpl")
    {
        Stencil::WebService<CatalogImpl> svc;
        svc.StartOnPort(44444);
        auto Test = [&](auto const& input, auto const& out) {
            auto str = CppHttpLib("http", "localhost", 44444, input).response();
            REQUIRE(str == out);
        };

        Test("/catalog/onlinedir/create?url=http%3A%2F%2F127.0.0.1%2F", "131072");
        Test("/catalog/onlinedir/create?url=http%3A%2F%2F127.0.0.2%2F&maxFrequencyInMins=60", "131073");
        Test("/catalog/onlinedir/get?ids=131072", "");
        Test("/catalog/onlinedir/get?ids=131072,131073", "");
        Test("/catalog/onlinedir/131072", "");
        Test("/catalog/onlinedir/131072?maxFrequencyInMins=120", "");
        Test("/catalog/onlinedir/131073?url=http%3A%2F%2F127.0.0.3%2F", "");
        Test("/catalog/onlinedir/delete?ids=131072", "");
        Test("/catalog/onlinedir/all", "");
        Test("/catalog/onlinedir/create?url=http%3A%2F%2F127.0.0.4%2F", "");
        Test("/catalog/onlinedir/all", "");

        Test("/catalog/localdir/create?url=http%3A%2F%2F127.0.0.1%2F", "");
        Test("/catalog/localdir/create?url=http%3A%2F%2F127.0.0.2%2F&maxFrequencyInMins=60", "");
        Test("/catalog/localdir/get?ids=0", "");
        Test("/catalog/localdir/get?ids=0,1", "");
        Test("/catalog/localdir/0", "");
        Test("/catalog/localdir/0?maxFrequencyInMins=120", "");
        Test("/catalog/localdir/1?url=http%3A%2F%2F127.0.0.3%2F", "");
        Test("/catalog/localdir/delete?ids=0", "");
        Test("/catalog/localdir/all", "");
        Test("/catalog/localdir/create?url=http%3A%2F%2F127.0.0.4%2F", "");
        Test("/catalog/localdir/all", "");

        svc.StopDaemon();
        svc.WaitForStop();
    }
}
