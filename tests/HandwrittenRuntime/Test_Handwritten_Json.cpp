#include "Test_Handwritten.h"
#define CATCH_CONFIG_MAIN
#include "TestUtils.h"

#include "stencil/protocol_binary.h"
#include <stencil/protocol_cli.h>

struct TestCase
{
    std::string json;
    std::string desc;
    bool        valid;
};

struct Aggregator
{
    struct Entry
    {
        std::string          primaryName;
        std::string          caseName;
        std::string          jsnInp;
        std::string          jsnOut;
        std::string          cliInp;
        std::string          cliOut;
        std::vector<uint8_t> binOut;
    };

    std::vector<Entry> _entries;

    void AddJsn(std::string primaryName, std::string caseName, std::string jsnInp, std::string jsnOut, std::vector<uint8_t> binOut)
    {
        auto key = fmt::format("[{}]:{}", primaryName, caseName);
        for (auto& ent : _entries)
        {
            if (ent.primaryName == primaryName && ent.caseName == caseName) { throw std::logic_error("Duplicate"); }
        }

        _entries.push_back(Entry{primaryName, caseName, jsnInp, jsnOut, "", "", binOut});
    }

    void AddCLI(std::string primaryName, std::string caseName, std::string cliInp, std::string cliOut)
    {
        auto key = fmt::format("[{}]:{}", primaryName, caseName);
        for (auto& ent : _entries)
        {
            if (ent.primaryName == primaryName && ent.caseName == caseName)
            {
                ent.cliInp = cliInp;
                ent.cliOut = cliOut;
                return;
            }
        }

        throw std::logic_error("Unknown Test case");
    }
    ~Aggregator() { TODO(""); }
};

Aggregator agg;

template <typename T> static void RunTestCase(TestCase const& tc, std::vector<std::string>& lines, std::string const& name)
{
    if (!tc.valid)
    {
        return;
        // if (IsDebuggerPresent()) return;
    }
    lines.push_back(fmt::format("Testcase[{}]:{}, Input: {}", name, tc.desc, tc.json));
    try
    {

        auto obj1  = Stencil::Json::Parse<T>(tc.json);
        auto jstr1 = Stencil::Json::Stringify<T>(obj1);
        auto obj2  = Stencil::Json::Parse<T>(jstr1);
        auto jstr2 = Stencil::Json::Stringify<T>(obj2);

        auto bin1 = Stencil::Serialize<T, Stencil::ProtocolBinary>(obj1).Reset();
        auto bin2 = Stencil::Serialize<T, Stencil::ProtocolBinary>(obj2).Reset();

        REQUIRE(bin1 == bin2);

        // d::copy(bin1.begin(), bin1.end(), std::back_inserter(data));
        REQUIRE(jstr1 == jstr2);
        lines.push_back(fmt::format("Testcase[{}]:{}, Output: {}", name, tc.desc, jstr2));
        agg.AddJsn(name, tc.desc, tc.json, jstr2, bin1);
    } catch (std::exception const& ex)
    {
        lines.push_back(fmt::format("Testcase[{}]:{}, Exception: {}", name, tc.desc, ex.what()));
    }
}

template <typename T> static void RunTestCases(std::initializer_list<TestCase> const& inp, std::string const& name)
{
    {
        std::vector<TestCase>    cases(inp);
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
                {R"({"f2": 1})", "uint16[4]_1", true},
                {R"({"f2": 65536})", "uint16[4]_2", true},
                {R"({"f2": 1234567890123456})", "uint16[4]_3", true},
                {R"({"f2": [0,1,2,3]})", "uint16[4]_4", true},
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
                {R"({"f1": {}})", "WithPrimitives64Bit_1", true},
                {R"({"f1": {"f1": 1234}})", "WithPrimitives64Bit_2", true},
                {R"({"f2": {}})", "WithBlobs_1", true},
                {R"({"f2": {"f1": "01234567"}})", "WithBlobs_2", true},
                {R"({"f3": {}})", "WithPrimitives64Bit_3", true},
                {R"({"f3": {"f1": 1234}})", "WithPrimitives64Bit_4", true},
                {R"({"f4": []})", "vec[WithBlobs]_1", true},
                {R"({"f4": [{}, {}, {}]})", "vec[WithBlobs]_2", true},
                {R"({"f4": [{"f1": "01234567"}, {"f1": "890124"}, {}]})", "vec[WithBlobs]_3", true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>(
            {
                {R"({"timestamp": "2020-01-02:03:04:05.600", "f1": {}})", "timestamp-1", true},
                //               {R"({"timestamp": "bad-timestamp", "f1": {}})", "multi-attributed-2", false},
                {R"({"uuid": "{01234567-8901-2345-6789-012345678901}", "f1": {}})", "uuid_1", true},
                {R"({"uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})", "uuid_2", true},
                //{R"({"uuid": 0})", "multi-attributed-3", false},
                {R"({"f1": {}})", "no_attributes", true},
                {R"({"timestamp": "2020-01-02:03:04:05.600", "uuid": "01234567-8901-2345-6789-012345678901", "f1": {}})",
                 "all_attributes",
                 true},
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

struct TestCaseCLI
{
    std::vector<std::string> args;
    std::string              desc;
    bool                     valid;
};

template <> struct fmt::formatter<TestCaseCLI> : fmt::formatter<std::string_view>
{

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext> auto format(const TestCaseCLI& tc, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{} :: {}", fmt::join(tc.args, " "), tc.desc);
    }
};

template <typename T> static void RunTestCaseCLI(TestCaseCLI const& tc, std::vector<std::string>& lines, std::string const& name)
{
    if (!tc.valid)
    {
        return;
        // if (IsDebuggerPresent()) return;
    }

    lines.push_back(fmt::format("Testcase[{}]:{}", name, tc));
    try
    {

        auto obj1  = Stencil::CLI::Parse<T>(tc.args);
        auto jstr1 = Stencil::CLI::Stringify<T>(obj1);
        auto obj2  = Stencil::CLI::Parse<T>(jstr1);
        auto jstr2 = Stencil::CLI::Stringify<T>(obj2);
        REQUIRE(jstr1 == jstr2);
        lines.push_back(fmt::format("Testcase[{}]:{}, Output,{}", name, tc.desc, fmt::join(jstr2, "\t")));
        agg.AddCLI(name, tc.desc, fmt::format("{}", fmt::join(tc.args, "\t")), fmt::format("{}", fmt::join(jstr2, "\t")));

    } catch (std::exception const& ex)
    {
        lines.push_back(fmt::format("Testcase[{}]:{}, Exception,{}", name, tc.desc, ex.what()));
    }
}

template <typename T> static void RunTestCaseCLIs(std::initializer_list<TestCaseCLI> cases, std::string const& name)
{
    {
        std::vector<std::string> lines;
        RunTestCaseCLI<T>({{"1"}, "default-1", false}, lines, name);
        RunTestCaseCLI<T>({{}, "default-2", true}, lines, name);
        RunTestCaseCLI<T>({{"[]"}, "default-3", false}, lines, name);
        RunTestCaseCLI<T>({{"mismatched"}, "default-4", false}, lines, name);
        for (auto& tc : cases) { RunTestCaseCLI<T>(tc, lines, name); }
        CheckOutputAgainstResource(lines, name);
    }

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("CLI", "[CLI]")
{
    SECTION("TestObj") { RunTestCaseCLIs<TestObj>({}, "TestObj"); }
    SECTION("WithPrimitives64Bit")
    {
        RunTestCaseCLIs<WithPrimitives64Bit>({{{"--f1=-1"}, "int64-1", true},
                                              {{"--f2=-1"}, "int16-1", true},
                                              {{"--f3=1"}, "uint64-1", true},
                                              {{"--f4=a"}, "char-1", true},
                                              {{"--f5=0.1"}, "double-1", true},
                                              {{"--f6=0.1"}, "float-1", true},
                                              {{"--f7=true"}, "bool-1", true},
                                              {{"--f8=2012-04-23T18:25:43.511Z"}, "time-1", true},
                                              {{"--f9=100"}, "time-2", true}},
                                             "WithPrimitives64Bit");
    }

    SECTION("ComplexPrimitives")
    {
        RunTestCaseCLIs<ComplexPrimitives>(
            {
                {{"--f1=01234567"}, "char[8]", true},
                {{"--f2=1"}, "uint16[4]_1", true},
                {{"--f2=65536"}, "uint16[4]_2", true},
                {{"--f2=1234567890123456"}, "uint16[4]_3", true},
                {{"--f2=0,1,2,3"}, "uint16[4]_4", true},
                {{"--f3=0.1,0.2"}, "float[2]", true},
            },
            "ComplexPrimitives");
    }

    SECTION("LargePrimitives")
    {
        RunTestCaseCLIs<LargePrimitives>(
            {
                {{"--f1=01234567"}, "char[16]", true},
                {{"--f2=0,1,2,3,4,5,6,7"}, "uint16[8]", true},
                {{"--f3=0.1,0.2,0.3,0.4"}, "float[4]", true},
                {{"--f4=01234567-8901-2345-6789-012345678901"}, "uuid", true},
            },
            "LargePrimitives");
    }

    SECTION("WithBlobs")
    {
        RunTestCaseCLIs<WithBlobs>(
            {
                {{"--f1=01234567"}, "string", true},
                {{"--f2=abcdef"}, "wstring", true},
                {{"--f3=0.1,0.2", "--f3=0.3,0.4"}, "vec[double]", true},
                {{"--f4=abc,def,ghi"}, "vec[string]", true},
            },
            "WithBlobs");
    }

    SECTION("Nested")
    {
        RunTestCaseCLIs<Nested>(
            {
                {{"f1"}, "WithPrimitives64Bit_1", true},
                {{"f1", "--f1=1234"}, "WithPrimitives64Bit_2", true},
                {{"f2"}, "WithBlobs_1", true},
                {{"f2", "--f1=01234567}"}, "WithBlobs_2", true},
                {{"f3"}, "WithPrimitives64Bit_3", true},
                {{"f3", "--f1=1234"}, "WithPrimitives64Bit_4", true},
                {{"f4"}, "vec[WithBlobs]_1", true},
                {{"f4", "f1", "-", "f1", "-", "f1"}, "vec[WithBlobs]_2", true},
                {{"f4", "--f1=01234567", "-", "--f1=890124"}, "vec[WithBlobs]_3", true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCaseCLIs<MultiAttributed>(
            {
                {{"timestamp", "2020-01-02:03:04:05.600", "f1"}, "timestamp-1", true},
                // {{"timestamp", "bad-timestamp", "f1"}, "multi-attributed-2", false},
                {{"uuid", "{01234567-8901-2345-6789-012345678901}", "f1"}, "uuid_1", true},
                {{"uuid", "01234567-8901-2345-6789-012345678901", "f1"}, "uuid_2", true},
                //  {{"uuid", "0"}, "multi-attributed-3", false},
                {{"f1"}, "no_attributes", true},
                {{"timestamp", "2020-01-02:03:04:05.600", "uuid", "01234567-8901-2345-6789-012345678901", "f1"}, "all_attributes", true},
            },
            "MultiAttributed");
    }

    SECTION("WithVariant")
    {
        RunTestCaseCLIs<WithVariant>(
            {
                {{"f1", "0", "f1", "--f1=1", "--", "f2", "0", "1", "--", "f3", "0", "1", "0.1", "--", "f4", "0", "2"}, "0", true},
                {{"f1", "1", "f1", "--f1=2", "--", "f2", "1", "a", "--", "f3", "1", "d", "--", "f4", "1", "f1", ""}, "1", true},
                {{"f1", "2", "f1", "--f1=3", "--", "f2", "2", "bcd", "--", "f3", "2", "efg", "--", "f4", "0", "3"}, "2", true},
                {{
                     "f1",
                     "3",
                     "--f1=4",
                     "--",
                     "f2",
                     "3",
                     "{aaaaaaaa-aaaa-aaaa-aaaa-012345678901}",
                     "--",
                     "f3",
                     "3",
                     "{a1234567-8901-2345-6789-012345678901}",
                     "--",
                     "f4",
                     "1",
                     "f2",
                 },
                 "3",
                 true},
            },
            "WithVariant");
    }

    SECTION("NamedVariant")
    {
        RunTestCaseCLIs<NamedVariant>(
            {
                {{"f1", "f1", "--f1=1"}, "0", true},
                {{"f2", "f1", "--f1=2"}, "1", true},
                {{"f3", "f1", "--f1=3"}, "2", true},
                {{"f4", "--f1=4"}, "3", true},
                {{"f5", "0.1234"}, "4", true},
                {{"f6", "abcd"}, "5", true},

            },
            "NamedVariant");
    }
}
