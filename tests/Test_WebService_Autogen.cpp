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
    CatalogImpl() = default;
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

    SECTION("CatalogImpl")
    {
        Stencil::WebService<CatalogImpl> svc;
        svc.StartOnPort(44444);
        svc.StopDaemon();
        svc.WaitForStop();
    }
}
