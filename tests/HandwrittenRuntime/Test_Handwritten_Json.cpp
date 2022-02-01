#include "Test_Handwritten.h"
#define CATCH_CONFIG_MAIN
#include "TestUtils.h"

struct TestCase
{
    std::string_view json;
    std::string_view desc;
    bool             valid;
};

template <typename T> void RunTestCase(TestCase const& tc, std::ostream& ostr)
{
    if (!tc.valid)
    {
        if (IsDebuggerPresent()) return;
    }
    fmt::print(ostr, "Testcase[{}]:{}, Input: {}", typeid(T).name(), tc.desc, tc.json);
    try
    {

        auto obj1 = Stencil::Json::Parse<T>(tc.json);
        // auto jstr1 = Stencil::Json::Stringify<T>(obj1);
        // auto obj2  = Stencil::Json::Parse<T>(jstr1);
        // auto jstr2 = Stencil::Json::Stringify<T>(obj2);
        // REQUIRE(jstr1 == jstr2);
        // fmt::print(ostr, "Testcase:{}, Output: {}", tc.desc, jstr2);
    } catch (std::exception const& ex)
    {
        fmt::print(ostr, "Testcase:{}, Exception: {}", ex.what());
    }
}

template <typename T> void RunTestCases(std::initializer_list<TestCase> cases)
{
    std::filesystem::path logfname = Catch::getResultCapture().getCurrentTestName() + ".txt";
    std::filesystem::path reffname = Catch::getResultCapture().getCurrentTestName() + ".txt";
    {
        std::ofstream logfile(logfname);
        RunTestCase<T>({"1", "default-1", false}, logfile);
        RunTestCase<T>({"{}", "default-2", true}, logfile);
        RunTestCase<T>({"[]", "default-3", false}, logfile);
        RunTestCase<T>({R"({"mismatched": {}})", "default-4", false}, logfile);
        for (auto& tc : cases) { RunTestCase<T>(tc, logfile); }
    }

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("Json", "[Json]")
{
    SECTION("TestObj") { RunTestCases<TestObj>({}); }
    /*
    *   int64_t  f1;
    int16_t  f2;
    uint64_t f3;
    char     f4;
    double   f5;
    float    f6;
    bool     f7;

    std::chrono::time_point<std::chrono::system_clock>          f8;
    std::chrono::time_point<std::chrono::high_resolution_clock> f9;

    std::array<double, 4>   f10;    // Iterable only
    std::array<char, 8>     f11;    // Iterable, blob (string) and Value (64-bit)
    std::array<uint16_t, 4> f12;    // Iterable and by value (64 bit)

    uuids::uuid f13;    // iterable , blob (string) or FixedSize buffer
    */
    SECTION("FixedSize")
    {
        RunTestCases<FixedSize>({
            {R"({"f1": -1})", "int64-1", true},
            {R"({"f2": -1})", "int16-1", true},
            {R"({"f3": 1})", "uint64-1", true},
            {R"({"f4": "a"})", "char-1", true},
            {R"({"f5": 0.1})", "double-1", true},
            {R"({"f6": 0.1})", "float-1", true},
            {R"({"f7": true})", "bool-1", true},
            {R"({"f8": "2012-04-23T18:25:43.511Z"})", "time-1", true},
            {R"({"f9": 100})", "time-2", true},
            {R"({"f10": [0.1, 0.2, 0.3, 0.4]})", "int64-1", true},
            {R"({"f11": "01234567"})", "chararry-1", true},
            {R"({"f12": 0x0123456789abcdef})", "intarray-1", true},
            {R"({"f13": "{01234567-8901-2345-6789-012345678901}"})", "uuid-1", true},
        });
    }

#if 0
    SECTION("WithBlobs") { RunTestCases<WithBlobs>({}); }

    SECTION("Nested") { RunTestCases<Nested>({}); }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>({
            {R"({"f1": {"timestamp": "2020-01-02:03:04:05.600"}})", "multi-attributed-1", true},
            {R"({"f1": {"timestamp": "bad-timestamp"}})", "multi-attributed-2", false},
            {R"({"f1": {"uuid": "{01234567-8901-2345-6789-012345678901}"}})", "multi-attributed-3", true},
            {R"({"f1": {"uuid": "01234567-8901-2345-6789-012345678901"}})", "multi-attributed-3", true},
            {R"({"f1": {"uuid": 0}})", "multi-attributed-3", false},
            {R"({"f1": {"f1", {}}})", "multi-attributed-3", true},
            {R"({"f1": {"timestamp": "2020-01-02:03:04:05.600", "uuid": "01234567-8901-2345-6789-012345678901", "f1", {}}})", "", true},
        });
    }
#endif
}
