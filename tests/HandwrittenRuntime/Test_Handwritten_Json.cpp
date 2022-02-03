#include "Test_Handwritten.h"
#define CATCH_CONFIG_MAIN
#include "TestUtils.h"

struct TestCase
{
    std::string_view json;
    std::string_view desc;
    bool             valid;
};

template <typename T> void RunTestCase(TestCase const& tc, std::vector<std::string>& lines)
{
    if (!tc.valid)
    {
        if (IsDebuggerPresent()) return;
    }
    lines.push_back(fmt::format("Testcase[{}]:{}, Input: {}", typeid(T).name(), tc.desc, tc.json));
    try
    {

        auto obj1  = Stencil::Json::Parse<T>(tc.json);
        auto jstr1 = Stencil::Json::Stringify<T>(obj1);
        auto obj2  = Stencil::Json::Parse<T>(jstr1);
        auto jstr2 = Stencil::Json::Stringify<T>(obj2);
        REQUIRE(jstr1 == jstr2);
        lines.push_back(fmt::format("Testcase[{}]:{}, Output: {}", typeid(T).name(), tc.desc, jstr2));
    } catch (std::exception const& ex)
    {
        lines.push_back(fmt::format("Testcase[{}]:{}, Exception: {}", typeid(T).name(), tc.desc, ex.what()));
    }
}

template <typename T> void RunTestCases(std::initializer_list<TestCase> cases, std::string const& name)
{
    {
        std::vector<std::string> lines;
        RunTestCase<T>({"1", "default-1", false}, lines);
        RunTestCase<T>({"{}", "default-2", true}, lines);
        RunTestCase<T>({"[]", "default-3", false}, lines);
        RunTestCase<T>({R"({"mismatched": {}})", "default-4", false}, lines);
        for (auto& tc : cases) { RunTestCase<T>(tc, lines); }
        CheckOutputAgainstResource(lines, name);
    }

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("Json", "[Json]")
{
    SECTION("TestObj") { RunTestCases<TestObj>({}, "TestObj"); }

    SECTION("Primitives64Bit")
    {
        RunTestCases<Primitives64Bit>({{R"({"f1": -1})", "int64-1", true},
                                       {R"({"f2": -1})", "int16-1", true},
                                       {R"({"f3": 1})", "uint64-1", true},
                                       {R"({"f4": "a"})", "char-1", true},
                                       {R"({"f5": 0.1})", "double-1", true},
                                       {R"({"f6": 0.1})", "float-1", true},
                                       {R"({"f7": true})", "bool-1", true},
                                       {R"({"f8": "2012-04-23T18:25:43.511Z"})", "time-1", true},
                                       {R"({"f9": 100})", "time-2", true}},
                                      "Primitives64Bit");
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
