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
                {R"({"f1": {}})", "Primitives64Bit", true},
                {R"({"f1": {"f1": 1234}})", "Primitives64Bit", true},
                {R"({"f2": {}})", "WithBlobs", true},
                {R"({"f2": {"f1": "01234567"}})", "WithBlobs", true},
                {R"({"f3": {}})", "Primitives64Bit", true},
                {R"({"f3": {"f1": 1234}})", "Primitives64Bit", true},
                {R"({"f4": []})", "vec[WithBlobs]", true},
                {R"({"f4": [{}, {}, {}]})", "vec[WithBlobs]", true},
                {R"({"f4": [{"f1": "01234567"}, {"f1": "890124"}, {}]})", "vec[WithBlobs]", true},
            },
            "Nested");
    }

#if 0
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
