#include "TestUtils.h"
#include "Test_Handwritten.h"

#include <stencil/protocol_cli.h>

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

template <typename T> static void RunTestCase(TestCaseCLI const& tc, std::vector<std::string>& lines, std::string const& name)
{
    if (!tc.valid)
    {
        return;
        // if (IsDebuggerPresent()) return;
    }

    lines.push_back(fmt::format("Testcase[{}]:{}", name, tc));
    try
    {

        auto obj1  = Stencil::CLI::Parse<T>(tc.args).obj;
        auto jstr1 = Stencil::CLI::Stringify<T>(obj1);
        auto obj2  = Stencil::CLI::Parse<T>(jstr1).obj;
        auto jstr2 = Stencil::CLI::Stringify<T>(obj2);
        REQUIRE(jstr1 == jstr2);
        lines.push_back(fmt::format("Testcase[{}]:{}, Output,{}", name, tc.desc, fmt::join(jstr2, "\t")));
    } catch (std::exception const& ex)
    {
        lines.push_back(fmt::format("Testcase[{}]:{}, Exception,{}", name, tc.desc, ex.what()));
    }
}

template <typename T> static void RunTestCases(std::initializer_list<TestCaseCLI> cases, std::string const& name)
{

    std::vector<std::string> lines;
    RunTestCase<T>({{"1"}, "default-1", false}, lines, name);
    RunTestCase<T>({{}, "default-2", true}, lines, name);
    RunTestCase<T>({{"[]"}, "default-3", false}, lines, name);
    RunTestCase<T>({{"mismatched"}, "default-4", false}, lines, name);
    for (auto& tc : cases) { RunTestCase<T>(tc, lines, name); }
    TestCommon::CheckResource<TestCommon::StrFormat>(lines, name);

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("CLI", "[CLI]")
{
    SECTION("TestObj")
    {
        RunTestCases<TestObj>({}, "TestObj");
    }
    SECTION("WithPrimitives64Bit")
    {
        RunTestCases<WithPrimitives64Bit>({{{"--f1=-1"}, "int64-1", true},
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
        RunTestCases<ComplexPrimitives>(
            {
                {{"--f1=01234567"}, "char[8]", true},
                {{"--f2=1"}, "uint16[4]-1", true},
                {{"--f2=65536"}, "uint16[4]-2", true},
                {{"--f2=1234567890123456"}, "uint16[4]-3", true},
                {{"--f2=0,1,2,3"}, "uint16[4]-4", true},
                {{"--f3=0.1,0.2"}, "float[2]", true},
            },
            "ComplexPrimitives");
    }

    SECTION("LargePrimitives")
    {
        RunTestCases<LargePrimitives>(
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
        RunTestCases<WithBlobs>(
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
        RunTestCases<Nested>(
            {
                {{"f1"}, "WithPrimitives64Bit", true},
                {{"f1", "--f1=1234"}, "WithPrimitives64Bit", true},
                {{"f2"}, "WithBlobs", true},
                {{"f2", "--f1=01234567}"}, "WithBlobs", true},
                {{"f3"}, "WithPrimitives64Bit", true},
                {{"f3", "--f1=1234"}, "WithPrimitives64Bit", true},
                {{"f4"}, "vec[WithBlobs]", true},
                {{"f4", "f1", "-", "f1", "-", "f1"}, "vec[WithBlobs]", true},
                {{"f4", "--f1=01234567", "-", "--f1=890124"}, "vec[WithBlobs]", true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>(
            {
                {{"timestamp", "2020-01-02:03:04:05.600", "f1"}, "multi-attributed-1", true},
                {{"timestamp", "bad-timestamp", "f1"}, "multi-attributed-2", false},
                {{"uuid", "{01234567-8901-2345-6789-012345678901}", "f1"}, "multi-attributed-3", true},
                {{"uuid", "01234567-8901-2345-6789-012345678901", "f1"}, "multi-attributed-3", true},
                {{"uuid", "0"}, "multi-attributed-3", false},
                {{"f1"}, "multi-attributed-3", true},
                {{"timestamp", "2020-01-02:03:04:05.600", "uuid", "01234567-8901-2345-6789-012345678901", "f1"}, "", true},
            },
            "MultiAttributed");
    }

    SECTION("WithVariant")
    {
        RunTestCases<WithVariant>(
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
        RunTestCases<NamedVariant>(
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
