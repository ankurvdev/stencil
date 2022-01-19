#include "TestUtils.h"
#include "stencil/stencil.h"
struct TestInterface;

struct TestObj_obj : Database2::ObjectT<TestInterface, TestObj_obj>
{};

struct TestInterface : public ReflectionBase::InterfaceT<TestInterface>, public Database2::OwnerT<TestInterface, TestObj_obj>
{
    public:
    TestInterface()          = default;
    virtual ~TestInterface() = default;
    DELETE_COPY_AND_MOVE(TestInterface);

    virtual uint64_t AddNumber(uint64_t num1, uint64_t num2) = 0;
};

struct TestInterface_AddNumber_Args
{
    TestInterface* instance = nullptr;

    uint64_t arg_num1{};
    uint64_t arg_num2{};

    uint64_t& get_arg_num1() { return arg_num1; }
    void      set_arg_num1(uint64_t&& value) { arg_num1 = value; }

    uint64_t& get_arg_num2() { return arg_num2; }
    void      set_arg_num2(uint64_t&& value) { arg_num2 = value; }
};

template <> struct ReflectionBase::TypeTraits<TestInterface_AddNumber_Args&>
{
    struct Traits_arg_num1
    {
        using TOwner     = TestInterface_AddNumber_Args;
        using TFieldType = uint64_t;
        static constexpr std::string_view    Name() { return "num1"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TestInterface_AddNumber_Args::get_arg_num1; }
        static constexpr auto TPropertySetter() { return &TestInterface_AddNumber_Args::set_arg_num1; }
    };

    struct Traits_arg_num2
    {
        using TOwner     = TestInterface_AddNumber_Args;
        using TFieldType = uint64_t;

        static constexpr std::string_view    Name() { return "num2"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestInterface_AddNumber_Args::get_arg_num2; }
        static constexpr auto TPropertySetter() { return &TestInterface_AddNumber_Args::set_arg_num2; }
    };

    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "AddNumber"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<TestInterface_AddNumber_Args, Traits_arg_num1, Traits_arg_num2>;
};
template <> struct ReflectionBase::InterfaceObjectTraits<TestObj_obj>
{
    static constexpr std::string_view Name() { return "obj"; }
};

template <> struct ReflectionBase::InterfaceTraits<TestInterface>
{
    struct ApiTraits_AddNumber
    {
        using TOwner = TestInterface;
        static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "AddNumber"; }
        static constexpr bool             Static = false;
    };

    using Apis    = std::tuple<ApiTraits_AddNumber>;
    using Objects = std::tuple<TestObj_obj>;
};

template <> struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<TestInterface>::ApiTraits_AddNumber>
{
    using ArgsStruct = TestInterface_AddNumber_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "AddNumber"; }

    static uint64_t Invoke(ArgsStruct& args) { return args.instance->AddNumber(args.get_arg_num1(), args.get_arg_num2()); }
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
        if (res && res->status == 200) { 
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
