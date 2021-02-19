#include "TestUtils.h"
#include "stencil/stencil.h"

struct TestInterface : public ReflectionBase::Interface<TestInterface>
{
    public:
    TestInterface()          = default;
    virtual ~TestInterface() = default;
    DELETE_COPY_AND_MOVE(TestInterface);

    virtual uint64_t                      AddNumber(uint64_t num1, uint64_t num2) = 0;
    static std::unique_ptr<TestInterface> Create(uint64_t randomInteger, shared_string randomString);
};

struct TestInterfaceFactory : public ReflectionBase::InterfaceFactory<TestInterface>
{
    public:
    virtual ~TestInterfaceFactory() = default;

    virtual std::unique_ptr<TestInterface> Activate(uint64_t randomInteger, shared_string randomString) = 0;
};

template <> struct ReflectionBase::InterfaceActivator<TestInterface>
{
    std::unique_ptr<TestInterface> Activate(uint64_t randomInteger, shared_string randomString)
    {
        return TestInterface::Create(randomInteger, randomString);
    }
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

struct TestInterface_Create_Args
{
    TestInterface* instance = nullptr;

    uint64_t      arg_randomInteger;
    shared_string arg_randomString;

    uint64_t& get_arg_randomInteger() { return arg_randomInteger; }
    void      set_arg_randomInteger(uint64_t&& value) { arg_randomInteger = value; }

    shared_string& get_arg_randomString() { return arg_randomString; }
    void           set_arg_randomString(shared_string&& value) { arg_randomString = std::move(value); }
};

template <> struct ReflectionBase::TypeTraits<TestInterface_Create_Args&>
{
    struct Traits_arg_randomInteger
    {
        using TOwner     = TestInterface_Create_Args;
        using TFieldType = uint64_t;

        static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "randomInteger"; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestInterface_Create_Args::get_arg_randomInteger; }
        static constexpr auto TPropertySetter() { return &TestInterface_Create_Args::set_arg_randomInteger; }
    };

    struct Traits_arg_randomString
    {
        using TOwner     = TestInterface_Create_Args;
        using TFieldType = shared_string;

        static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "randomString"; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestInterface_Create_Args::get_arg_randomString; }
        static constexpr auto TPropertySetter() { return &TestInterface_Create_Args::set_arg_randomString; }
    };

    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler
        = ::ReflectionServices::ReflectedStructHandler<TestInterface_Create_Args, Traits_arg_randomInteger, Traits_arg_randomString>;
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

    struct ApiTraits_Create
    {
        using TOwner = TestInterface;
        static const ::ReflectionBase::Flags Flags() { return {}; }

        using ArgsStruct = TestInterface_Create_Args;
        static constexpr std::string_view Name() { return "Create"; }
        static constexpr bool             Static = true;
    };

    using Apis = ::ReflectionBase::InterfaceApiPack<ApiTraits_AddNumber, ApiTraits_Create>;
};

template <> struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<TestInterface>::ApiTraits_AddNumber>
{
    using ArgsStruct = TestInterface_AddNumber_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "AddNumber"; }

    static uint64_t Invoke(ArgsStruct& args) { return args.instance->AddNumber(args.get_arg_num1(), args.get_arg_num1()); }
};

template <> struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<TestInterface>::ApiTraits_Create>
{
    using ArgsStruct = TestInterface_Create_Args;
    static constexpr bool             IsStatic() { return true; }
    static constexpr std::string_view Name() { return "Create"; }

    static std::unique_ptr<TestInterface> Invoke(ArgsStruct& args)
    {
        return TestInterface::Create(args.get_arg_randomInteger(), args.get_arg_randomString());
    }
};

// End Generated code

namespace impl
{
class TestInterfaceImpl : public TestInterface
{
    public:
    // Inherited via TestInterface
    virtual uint64_t AddNumber(uint64_t num1, uint64_t num2) override { return num1 + num2; }

    TestInterfaceImpl(uint64_t randomInteger, shared_string randomString) : _randomInteger(randomInteger), _randomString(randomString) {}

    DELETE_COPY_AND_MOVE(TestInterfaceImpl);

    uint64_t      _randomInteger;
    shared_string _randomString;
};

class TestInterfaceFactoryImpl : public TestInterfaceFactory
{
    // Inherited via TestInterfaceFactory
    virtual std::unique_ptr<TestInterface> Activate(uint64_t randomInteger, shared_string randomString) override
    {
        return TestInterface::Create(randomInteger, randomString);
    }
};
}    // namespace impl

template <> struct ReflectionBase::InterfaceActivator<TestInterfaceFactory>
{
    static std::unique_ptr<TestInterfaceFactory> Activate()
    {
        return std::unique_ptr<TestInterfaceFactory>(new impl::TestInterfaceFactoryImpl());
    }
};

std::unique_ptr<TestInterface> TestInterface::Create(uint64_t randomInteger, shared_string randomString)
{
    return std::unique_ptr<TestInterface>(new impl::TestInterfaceImpl(randomInteger, randomString));
}

template <> struct WebServiceHandlerTraits<TestInterface>
{
    static constexpr const std::string_view Url() { return std::string_view("TestInterface"); }
};
#pragma warning(push, 0)
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

TEST_CASE("CodeGen::WebService::HandWritten", "[WebService]")
{
    SECTION("Positive: SimpleCase")
    {
        WebService<TestInterface> svc;
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
