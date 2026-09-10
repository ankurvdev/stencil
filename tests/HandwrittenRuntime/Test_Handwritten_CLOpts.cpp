#include "CommonMacros.h"
#include "TestUtils.h"
#include "Test_Handwritten.h"

#include <stencil/protocol_cli.h>

namespace
{
struct TestCaseCLI
{
    std::vector<std::string> args;
    std::string              desc;
    bool                     valid;
};
}    // namespace

template <> struct fmt::formatter<TestCaseCLI> : fmt::formatter<std::string_view>
{

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext> auto format(TestCaseCLI const& tc, FormatContext& ctx) const
    { return fmt::format_to(ctx.out(), "{} :: {}", fmt::join(tc.args, " "), tc.desc); }
};
SUPPRESS_WARNINGS_START
SUPPRESS_CLANG_WARNING("-Wlifetime-safety-invalidation")
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
    } catch (std::exception const& ex) { lines.push_back(fmt::format("Testcase[{}]:{}, Exception,{}", name, tc.desc, ex.what())); }
}

SUPPRESS_WARNINGS_END

template <typename T> static void RunTestCases(std::initializer_list<TestCaseCLI> cases, std::string const& name)
{

    std::vector<std::string> lines;
    RunTestCase<T>({{"1"}, "default-1", false}, lines, name);
    RunTestCase<T>({{}, "default-2", true}, lines, name);
    RunTestCase<T>({{"[]"}, "default-3", false}, lines, name);
    RunTestCase<T>({{"mismatched"}, "default-4", false}, lines, name);
    for (auto const& tc : cases) { RunTestCase<T>(tc, lines, name); }
    TestCommon::CheckResource<TestCommon::StrFormat>(lines, name);

    // CompareFileAgainstResource(logfname, reffname.string());
}

TEST_CASE("CLI", "[CLI]")
{
    SECTION("TestObj")
    { RunTestCases<TestObj>({}, "TestObj"); }
    SECTION("WithPrimitives64Bit")
    {
        RunTestCases<WithPrimitives64Bit>(
            {
                {.args = {"--f1=-1"}, .desc = "int64-1", .valid = true},
                {.args = {"--f2=-1"}, .desc = "int16-1", .valid = true},
                {.args = {"--f3=1"}, .desc = "uint64-1", .valid = true},
                {.args = {"--f4=a"}, .desc = "char-1", .valid = true},
                {.args = {"--f5=0.1"}, .desc = "double-1", .valid = true},
                {.args = {"--f6=0.1"}, .desc = "float-1", .valid = true},
                {.args = {"--f7=true"}, .desc = "bool-1", .valid = true},
                {.args = {"--f8=2012-04-23T18:25:43.511Z"}, .desc = "time-1", .valid = true},
                {.args = {"--f9=100"}, .desc = "time-2", .valid = true},
            },
            "WithPrimitives64Bit");
    }

    SECTION("ComplexPrimitives")
    {
        RunTestCases<ComplexPrimitives>(
            {
                {.args = {"--f1=01234567"}, .desc = "char[8]", .valid = true},
                {.args = {"--f2=1"}, .desc = "uint16[4]-1", .valid = true},
                {.args = {"--f2=65536"}, .desc = "uint16[4]-2", .valid = true},
                {.args = {"--f2=1234567890123456"}, .desc = "uint16[4]-3", .valid = true},
                {.args = {"--f2=0,1,2,3"}, .desc = "uint16[4]-4", .valid = true},
                {.args = {"--f3=0.1,0.2"}, .desc = "float[2]", .valid = true},
            },
            "ComplexPrimitives");
    }

    SECTION("LargePrimitives")
    {
        RunTestCases<LargePrimitives>(
            {
                {.args = {"--f1=01234567"}, .desc = "char[16]", .valid = true},
                {.args = {"--f2=0,1,2,3,4,5,6,7"}, .desc = "uint16[8]", .valid = true},
                {.args = {"--f3=0.1,0.2,0.3,0.4"}, .desc = "float[4]", .valid = true},
                {.args = {"--f4=01234567-8901-2345-6789-012345678901"}, .desc = "uuid", .valid = true},
            },
            "LargePrimitives");
    }

    SECTION("WithBlobs")
    {
        RunTestCases<WithBlobs>(
            {
                {.args = {"--f1=01234567"}, .desc = "string", .valid = true},
                {.args = {"--f2=abcdef"}, .desc = "wstring", .valid = true},
                {.args = {"--f3=0.1,0.2", "--f3=0.3,0.4"}, .desc = "vec[double]", .valid = true},
                {.args = {"--f4=abc,def,ghi"}, .desc = "vec[string]", .valid = true},
            },
            "WithBlobs");
    }

    SECTION("Nested")
    {
        RunTestCases<Nested>(
            {
                {.args = {"f1"}, .desc = "WithPrimitives64Bit", .valid = true},
                {.args = {"f1", "--f1=1234"}, .desc = "WithPrimitives64Bit", .valid = true},
                {.args = {"f2"}, .desc = "WithBlobs", .valid = true},
                {.args = {"f2", "--f1=01234567}"}, .desc = "WithBlobs", .valid = true},
                {.args = {"f3"}, .desc = "WithPrimitives64Bit", .valid = true},
                {.args = {"f3", "--f1=1234"}, .desc = "WithPrimitives64Bit", .valid = true},
                {.args = {"f4"}, .desc = "vec[WithBlobs]", .valid = true},
                {.args = {"f4", "f1", "-", "f1", "-", "f1"}, .desc = "vec[WithBlobs]", .valid = true},
                {.args = {"f4", "--f1=01234567", "-", "--f1=890124"}, .desc = "vec[WithBlobs]", .valid = true},
            },
            "Nested");
    }

    SECTION("MultiAttributed")
    {
        RunTestCases<MultiAttributed>(
            {
                {.args = {"timestamp", "2020-01-02:03:04:05.600", "f1"}, .desc = "multi-attributed-1", .valid = true},
                {.args = {"timestamp", "bad-timestamp", "f1"}, .desc = "multi-attributed-2", .valid = false},
                {.args = {"uuid", "{01234567-8901-2345-6789-012345678901}", "f1"}, .desc = "multi-attributed-3", .valid = true},
                {.args = {"uuid", "01234567-8901-2345-6789-012345678901", "f1"}, .desc = "multi-attributed-3", .valid = true},
                {.args = {"uuid", "0"}, .desc = "multi-attributed-3", .valid = false},
                {.args = {"f1"}, .desc = "multi-attributed-3", .valid = true},
                {
                    .args  = {"timestamp", "2020-01-02:03:04:05.600", "uuid", "01234567-8901-2345-6789-012345678901", "f1"},
                    .desc  = "",
                    .valid = true,
                },
            },
            "MultiAttributed");
    }

    SECTION("WithVariant")
    {
        RunTestCases<WithVariant>(
            {
                {.args={"f1", "0", "f1", "--f1=1", "--", "f2", "0", "1", "--", "f3", "0", "1", "0.1", "--", "f4", "0", "2"}, .desc="0", .valid=true},
                {.args={"f1", "1", "f1", "--f1=2", "--", "f2", "1", "a", "--", "f3", "1", "d", "--", "f4", "1", "f1", ""}, .desc="1", .valid=true},
                {.args={"f1", "2", "--f1=3", "--", "f2", "2", "bcd", "--", "f3", "2", "efg", "--", "f4", "0", "3"}, .desc="2", .valid=true},
                {.args={
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
                 .desc="3",
                 .valid=true,},
            },
            "WithVariant");
    }

    SECTION("NamedVariant")
    {
        RunTestCases<NamedVariant>(
            {
                {.args = {"f1", "f1", "--f1=1"}, .desc = "0", .valid = true},
                {.args = {"f2", "f1", "--f1=2"}, .desc = "1", .valid = true},
                {.args = {"f3", "--f1=3"}, .desc = "2", .valid = true},
                {.args = {"f4", "--f1=4"}, .desc = "3", .valid = true},
                {.args = {"f5", "0.1234"}, .desc = "4", .valid = true},
                {.args = {"f6", "abcd"}, .desc = "5", .valid = true},

            },
            "NamedVariant");
    }
}
