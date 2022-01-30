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
#if 0
    SECTION("FixedSize") { RunTestCases<FixedSize>({}); }

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
