#include "Test_Handwritten.h"
#include "stencil/protocol_binary.h"

#include "TestUtils.h"

struct TestCase
{
    std::string json;
    std::string desc;
    bool        valid;
};

template <typename T> static void RunTestCase(TestCase const& tc, std::vector<uint8_t>& data, std::string const& /*name*/)
{
    if (!tc.valid)
    {
        return;
        // if (Catch2::isDebuggerActive()) return;
    }

    // lines.push_back(fmt::format("Testcase[{}]:{}, Input: {}", name, tc.desc, tc.json));
    try
    {
        T obj1 = Stencil::Json::Parse<T>(tc.json);

        auto bin1 = Stencil::Serialize<Stencil::ProtocolBinary>(obj1).Reset();
        auto jst1 = Stencil::Serialize<Stencil::ProtocolJsonVal>(obj1).str();

        T obj2 = Stencil::Deserialize<T, Stencil::ProtocolBinary>(bin1);

        auto bin2 = Stencil::Serialize<Stencil::ProtocolBinary>(obj2).Reset();
        auto jst2 = Stencil::Serialize<Stencil::ProtocolJsonVal>(obj2).str();

        REQUIRE(jst1 == jst2);
        REQUIRE(bin1 == bin2);
        std::copy(bin1.begin(), bin1.end(), std::back_inserter(data));
    } catch (std::exception const& /*ex*/)
    {
        // lines.push_back(fmt::format("Testcase[{}]:{}, Exception: {}", name, tc.desc, ex.what()));
    }
}

template <typename T> static void RunTestCases(const std::vector<TestCase>& cases, std::string const& name)
{
    {
        std::vector<uint8_t> data;
        RunTestCase<T>({"1", "default-1", false}, data, name);
        RunTestCase<T>({"{}", "default-2", true}, data, name);
        RunTestCase<T>({"[]", "default-3", false}, data, name);
        RunTestCase<T>({R"({"mismatched": {}})", "default-4", false}, data, name);
        for (const auto& tc : cases) { RunTestCase<T>(tc, data, name); }
        // Due wchar_t size differences linux and windows arent ABI compatible.
        // CompareBinaryOutputAgainstResource(data, name);

        // CheckOutputAgainstResource(lines, name);
    }

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("Binary", "[Protocol]")
{
    SECTION("TestObj")
    {
        RunTestCases<TestObj>({}, "TestObj");
    }

    SECTION("WithPrimitives64Bit")
    {
        RunTestCases<WithPrimitives64Bit>({{.json=R"({"f1": -1})", .desc="int64-1", .valid=true},
                                           {.json=R"({"f2": -1})", .desc="int16-1", .valid=true},
                                           {.json=R"({"f3": 1})", .desc="uint64-1", .valid=true},
                                           {.json=R"({"f4": "a"})", .desc="char-1", .valid=true},
                                           {.json=R"({"f5": 0.1})", .desc="double-1", .valid=true},
                                           {.json=R"({"f6": 0.1})", .desc="float-1", .valid=true},
                                           {.json=R"({"f7": true})", .desc="bool-1", .valid=true},
                                           {.json=R"({"f8": "2012-04-23T18:25:43.511Z"})", .desc="time-1", .valid=true},
                                           {.json=R"({"f9": 100})", .desc="time-2", .valid=true},},
                                          "WithPrimitives64Bit");
    }

    SECTION("ComplexPrimitives")
    {
        RunTestCases<ComplexPrimitives>(
            {
                {.json=R"({"f1": "01234567"})", .desc="char[8]", .valid=true},
                {.json=R"({"f2": 1})", .desc="uint16[4]", .valid=true},
                {.json=R"({"f2": 65536})", .desc="uint16[4]", .valid=true},
                {.json=R"({"f2": 1234567890123456})", .desc="uint16[4]", .valid=true},
                {.json=R"({"f2": [0,1,2,3]})", .desc="uint16[4]", .valid=true},
                {.json=R"({"f3": [0.1, 0.2]})", .desc="float[2]", .valid=true},
            },
            "ComplexPrimitives");
    }

    SECTION("LargePrimitives")
    {
        RunTestCases<LargePrimitives>(
            {
                {.json=R"({"f1": "01234567"})", .desc="char[16]", .valid=true},
                {.json=R"({"f2": [0,1,2,3,4,5,6,7]})", .desc="uint16[8]", .valid=true},
                {.json=R"({"f3": [0.1, 0.2, 0.3, 0.4]})", .desc="float[4]", .valid=true},
                {.json=R"({"f4": "01234567-8901-2345-6789-012345678901"})", .desc="uuid", .valid=true},

            },
            "LargePrimitives");
    }

    SECTION("WithBlobs")
    {
        RunTestCases<WithBlobs>(
            {
                {.json=R"({"f1": "01234567"})", .desc="string", .valid=true},
                {.json=R"({"f2":  "abcdef"})", .desc="wstring", .valid=true},
                {.json=R"({"f3": [0.0, 0.0, 0.0, 0.0]})", .desc="vec[double]", .valid=true},
                {.json=R"({"f4": ["abc", "def", "ghi"]})", .desc="vec[string]", .valid=true},
            },
            "WithBlobs");
    }

    SECTION("Nested")
    {
        RunTestCases<Nested>(
            {
                {.json=R"({"f1": {}})", .desc="WithPrimitives64Bit", .valid=true},
                {.json=R"({"f1": {"f1": 1234}})", .desc="WithPrimitives64Bit", .valid=true},
                {.json=R"({"f2": {}})", .desc="WithBlobs", .valid=true},
                {.json=R"({"f2": {"f1": "01234567"}})", .desc="WithBlobs", .valid=true},
                {.json=R"({"f3": {}})", .desc="WithPrimitives64Bit", .valid=true},
                {.json=R"({"f3": {"f1": 1234}})", .desc="WithPrimitives64Bit", .valid=true},
                {.json=R"({"f4": []})", .desc="vec[WithBlobs]", .valid=true},
                {.json=R"({"f4": [{}, {}, {}]})", .desc="vec[WithBlobs]", .valid=true},
                {.json=R"({"f4": [{"f1": "01234567"}, {"f1": "890124"}, {}]})", .desc="vec[WithBlobs]", .valid=true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>(
            {
                {.json=R"({"timestamp": "2020-01-02:03:04:05.600", "f1": {}})", .desc="multi-attributed-1", .valid=true},
                {.json=R"({"timestamp": "bad-timestamp", "f1": {}})", .desc="multi-attributed-2", .valid=false},
                {.json=R"({"uuid": "{01234567-8901-2345-6789-012345678901}", "f1": {}})", .desc="multi-attributed-3", .valid=true},
                {.json=R"({"uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})", .desc="multi-attributed-3", .valid=true},
                {.json=R"({"uuid": 0})", .desc="multi-attributed-3", .valid=false},
                {.json=R"({"f1": {}})", .desc="multi-attributed-3", .valid=true},
                {.json=R"({"timestamp": "2020-01-02:03:04:05.600", "uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})", .desc="", .valid=true},
            },
            "MultiAttributed");
    }

    SECTION("WithVariant")
    {
        RunTestCases<WithVariant>(
            {
                {.json=R"({"f1": {"0": {"f1": {}}}, "f2": {"0": 1}, "f3": {"0": {"1": 0.1}}, "f4": {"0": 2}})", .desc="0", .valid=true},
                {.json=R"({"f1": {"1": {"f1": {}}}, "f2": {"1": "a"}, "f3": {"1": "d"}, "f4": {"1": {"f1": {}}}})", .desc="1", .valid=true},
                {.json=R"({"f1": {"2": {"f1": {}}}, "f2": {"2": "bcd"}, "f3": {"2": "efg"}, "f4": {"0": 3}})", .desc="2", .valid=true},
                {.json=R"({"f1": {"3": {"f1": {}}}, "f2": {"3": "01234567-8901-2345-6789-012345678901"}, "f3": {"3": "{a1234567-8901-2345-6789-012345678901}"}, "f4": {"1": {"f2": {}}}})",
                 .desc="3",
                 .valid=true,},
            },
            "WithVariant");
    }

    SECTION("NamedVariant")
    {
        RunTestCases<NamedVariant>(
            {
                {.json=R"({"f1": {"f1": {}}})", .desc="0", .valid=true},
                {.json=R"({"f2": {"f1": {}}})", .desc="1", .valid=true},
                {.json=R"({"f3": {"f1": {}}})", .desc="2", .valid=true},
                {.json=R"({"f4": {"f1": {}}})", .desc="3", .valid=true},
                {.json=R"({"f5": 0.1234})", .desc="4", .valid=true},
                {.json=R"({"f6": "abcd"})", .desc="5", .valid=true},

            },
            "NamedVariant");
    }
}
