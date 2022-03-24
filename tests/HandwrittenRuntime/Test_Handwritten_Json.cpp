#include "Test_Handwritten.h"
#define CATCH_CONFIG_MAIN
#include "TestUtils.h"

struct TestCase
{
    std::string_view json;
    std::string_view desc;
    bool             valid;
};

template <typename T> static void RunTestCase(TestCase const& tc, std::vector<std::string>& lines, std::string const& name)
{
    if (!tc.valid)
    {
        if (IsDebuggerPresent()) return;
    }
    lines.push_back(fmt::format("Testcase[{}]:{}, Input: {}", name, tc.desc, tc.json));
    try
    {

        auto obj1  = Stencil::Json::Parse<T>(tc.json);
        auto jstr1 = Stencil::Json::Stringify<T>(obj1);
        auto obj2  = Stencil::Json::Parse<T>(jstr1);
        auto jstr2 = Stencil::Json::Stringify<T>(obj2);
        REQUIRE(jstr1 == jstr2);
        lines.push_back(fmt::format("Testcase[{}]:{}, Output: {}", name, tc.desc, jstr2));
    } catch (std::exception const& ex)
    {
        lines.push_back(fmt::format("Testcase[{}]:{}, Exception: {}", name, tc.desc, ex.what()));
    }
}

template <typename T> static void RunTestCases(std::initializer_list<TestCase> cases, std::string const& name)
{
    {
        std::vector<std::string> lines;
        RunTestCase<T>({"1", "default-1", false}, lines, name);
        RunTestCase<T>({"{}", "default-2", true}, lines, name);
        RunTestCase<T>({"[]", "default-3", false}, lines, name);
        RunTestCase<T>({R"({"mismatched": {}})", "default-4", false}, lines, name);
        for (auto& tc : cases) { RunTestCase<T>(tc, lines, name); }
        CheckOutputAgainstResource(lines, name);
    }

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("Json", "[Json]")
{
    SECTION("TestObj") { RunTestCases<TestObj>({}, "TestObj"); }

    SECTION("WithPrimitives64Bit")
    {
        RunTestCases<WithPrimitives64Bit>({{R"({"f1": -1})", "int64-1", true},
                                           {R"({"f2": -1})", "int16-1", true},
                                           {R"({"f3": 1})", "uint64-1", true},
                                           {R"({"f4": "a"})", "char-1", true},
                                           {R"({"f5": 0.1})", "double-1", true},
                                           {R"({"f6": 0.1})", "float-1", true},
                                           {R"({"f7": true})", "bool-1", true},
                                           {R"({"f8": "2012-04-23T18:25:43.511Z"})", "time-1", true},
                                           {R"({"f9": 100})", "time-2", true}},
                                          "WithPrimitives64Bit");
    }

    SECTION("ComplexPrimitives")
    {
        RunTestCases<ComplexPrimitives>(
            {
                {R"({"f1": "01234567"})", "char[8]", true},
                {R"({"f2": 1})", "uint16[4]", true},
                {R"({"f2": 65536})", "uint16[4]", true},
                {R"({"f2": 1234567890123456})", "uint16[4]", true},
                {R"({"f2": [0,1,2,3]})", "uint16[4]", true},
                {R"({"f3": [0.1, 0.2]})", "float[2]", true},
            },
            "ComplexPrimitives");
    }

    SECTION("LargePrimitives")
    {
        RunTestCases<LargePrimitives>(
            {
                {R"({"f1": "01234567"})", "char[16]", true},
                {R"({"f2": [0,1,2,3,4,5,6,7]})", "uint16[8]", true},
                {R"({"f3": [0.1, 0.2, 0.3, 0.4]})", "float[4]", true},
                {R"({"f4": "01234567-8901-2345-6789-012345678901"})", "uuid", true},

            },
            "LargePrimitives");
    }

    SECTION("WithBlobs")
    {
        RunTestCases<WithBlobs>(
            {
                {R"({"f1": "01234567"})", "string", true},
                {R"({"f2":  "abcdef"})", "wstring", true},
                {R"({"f3": [0.1, 0.2, 0.3, 0.4]})", "vec[double]", true},
                {R"({"f4": ["abc", "def", "ghi"]})", "vec[string]", true},
            },
            "WithBlobs");
    }

    SECTION("Nested")
    {
        RunTestCases<Nested>(
            {
                {R"({"f1": {}})", "WithPrimitives64Bit", true},
                {R"({"f1": {"f1": 1234}})", "WithPrimitives64Bit", true},
                {R"({"f2": {}})", "WithBlobs", true},
                {R"({"f2": {"f1": "01234567"}})", "WithBlobs", true},
                {R"({"f3": {}})", "WithPrimitives64Bit", true},
                {R"({"f3": {"f1": 1234}})", "WithPrimitives64Bit", true},
                {R"({"f4": []})", "vec[WithBlobs]", true},
                {R"({"f4": [{}, {}, {}]})", "vec[WithBlobs]", true},
                {R"({"f4": [{"f1": "01234567"}, {"f1": "890124"}, {}]})", "vec[WithBlobs]", true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>(
            {
                {R"({"timestamp": "2020-01-02:03:04:05.600", "f1": {}})", "multi-attributed-1", true},
                {R"({"timestamp": "bad-timestamp", "f1": {}})", "multi-attributed-2", false},
                {R"({"uuid": "{01234567-8901-2345-6789-012345678901}", "f1": {}})", "multi-attributed-3", true},
                {R"({"uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})", "multi-attributed-3", true},
                {R"({"uuid": 0})", "multi-attributed-3", false},
                {R"({"f1": {}})", "multi-attributed-3", true},
                {R"({"timestamp": "2020-01-02:03:04:05.600", "uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})", "", true},
            },
            "MultiAttributed");
    }

    SECTION("WithVariant")
    {
        RunTestCases<WithVariant>(
            {
                {R"({"f1": {"0": {"f1": {}}}, "f2": {"0": 1}, "f3": {"0": {"1": 0.1}}, "f4": {"0": 2}})", "0", true},
                {R"({"f1": {"1": {"f1": {}}}, "f2": {"1": "a"}, "f3": {"1": "d"}, "f4": {"1": {"f1": {}}}})", "1", true},
                {R"({"f1": {"2": {"f1": {}}}, "f2": {"2": "bcd"}, "f3": {"2": "efg"}, "f4": {"0": 3}})", "2", true},
                {R"({"f1": {"3": {"f1": {}}}, "f2": {"3": "01234567-8901-2345-6789-012345678901"}, "f3": {"3": "{a1234567-8901-2345-6789-012345678901}"}, "f4": {"1": {"f2": {}}}})",
                 "3",
                 true},
            },
            "WithVariant");
    }

    SECTION("NamedVariant")
    {
        RunTestCases<NamedVariant>(
            {
                {R"({"f1": {"f1": {}}})", "0", true},
                {R"({"f2": {"f1": {}}})", "1", true},
                {R"({"f3": {"f1": {}}})", "2", true},
                {R"({"f4": {"f1": {}}})", "3", true},
                {R"({"f5": 0.1234})", "4", true},
                {R"({"f6": "abcd"})", "5", true},

            },
            "NamedVariant");
    }
}
