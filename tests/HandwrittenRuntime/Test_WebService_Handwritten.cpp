#include "TestUtils.h"
#include "stencil/stencil.h"
struct TestInterface;

struct TestInterface : public Stencil::InterfaceT<TestInterface>
{
    public:
    struct DataStore;

    struct TestObj_obj : Database2::ObjectT<DataStore, TestObj_obj>
    {
        uint64_t arg_num1{};
        uint64_t arg_num2{};
    };

    struct DataStore : Database2::DatabaseT<DataStore>
    {
        using ObjectTypePack = std::tuple<TestObj_obj>;
        DataStore()          = default;
        CLASS_DELETE_COPY_AND_MOVE(DataStore);
    };

    TestInterface()          = default;
    virtual ~TestInterface() = default;
    CLASS_DELETE_COPY_AND_MOVE(TestInterface);

    virtual uint64_t AddNumber(uint64_t num1, uint64_t num2) = 0;

    DataStore objects;
};

template <> struct Stencil::TypeTraits<TestInterface::TestObj_obj>
{
    struct Traits_arg_num1
    {
        static constexpr std::string_view Name() { return "num1"; }
    };

    struct Traits_arg_num2
    {
        static constexpr std::string_view Name() { return "num2"; }
    };

    static constexpr std::string_view Name() { return "AddNumber"; }
};

struct TestInterface_AddNumber_Args
{
    uint64_t arg_num1{};
    uint64_t arg_num2{};
};

template <> struct Stencil::TypeTraits<TestInterface_AddNumber_Args>
{
    struct Traits_arg_num1
    {
        static constexpr std::string_view Name() { return "num1"; }
    };

    struct Traits_arg_num2
    {
        static constexpr std::string_view Name() { return "num2"; }
    };

    static constexpr std::string_view Name() { return "AddNumber"; }
};
template <> struct Stencil::InterfaceObjectTraits<TestInterface::TestObj_obj>
{
    static constexpr std::string_view Name() { return "obj"; }
};

template <> struct Stencil::InterfaceTraits<TestInterface>
{
    struct ApiTraits_AddNumber
    {
        static constexpr std::string_view Name() { return "AddNumber"; }
        static constexpr bool             Static = false;
    };

    using ApiStructs = std::tuple<ApiTraits_AddNumber>;
    using Objects    = std::tuple<TestInterface::TestObj_obj>;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<TestInterface>::ApiTraits_AddNumber>
{
    using ArgsStruct = TestInterface_AddNumber_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "AddNumber"; }

    static uint64_t Invoke(TestInterface& obj, ArgsStruct& args) { return obj.AddNumber(args.arg_num1, args.arg_num2); }
};

// End Generated code

namespace impl
{
class TestInterfaceImpl : public TestInterface
{
    public:
    // Inherited via TestInterface
    virtual uint64_t AddNumber(uint64_t num1, uint64_t num2) override { return num1 + num2; }

    TestInterfaceImpl() = default;
    TestInterfaceImpl(uint64_t randomInteger, shared_string randomString) : _randomInteger(randomInteger), _randomString(randomString) {}

    DELETE_COPY_AND_MOVE(TestInterfaceImpl);

    uint64_t      _randomInteger;
    shared_string _randomString;
};

}    // namespace impl

template <> struct Stencil::WebServiceHandlerTraits<TestInterface>
{
    static constexpr const std::string_view Url() { return std::string_view("TestInterface"); }
    static void HandleRequest(TestInterface& obj, httplib::Request const& req, httplib::Response& res, std::string_view const& path);
};

#pragma warning(push, 3)
#include <httplib.h>
#pragma warning(pop)

#include <sstream>
#include <string>
#include <string_view>

class CppHttpLib
{
    public:
    CppHttpLib(std::string_view /*proto*/, std::string_view host, uint16_t port, std::string_view get)
    {
        httplib::Client cli(host.data(), port);
        auto            res = cli.Get(get.data());
        if (res && res->status == 200) { _response = res->body; }
        else { throw std::runtime_error("Unable to get"); }
    }

    std::string response() { return _response; }

    private:
    std::string _response;
};

TEST_CASE("CodeGen::WebService::HandWritten", "[WebService]")
{
    SECTION("Positive: SimpleCase")
    {
        Stencil::WebService<impl::TestInterfaceImpl> svc;
        svc.StartOnPort(44444);
        try
        {
            auto str = CppHttpLib("http", "localhost", 44444, "/TestInterface/AddNumber?num1=22&num2=33").response();
            REQUIRE(str == "55");
        } catch (std::exception const& ex)
        {
            FAIL(ex.what());
        }
        svc.StopDaemon();
        svc.WaitForStop();
    }
}
