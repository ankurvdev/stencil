#define CATCH_CONFIG_MAIN

#include "TestUtils.h"
DECLARE_RESOURCE_COLLECTION(testdata);
#include "Generator.h"

void RunTest(std::initializer_list<std::string_view const> const& pidlfiles)
{
    ResourceFileManager resfiles;

    for (auto const& pidlfile : pidlfiles)
    {

        auto generator = Generator::Get("thrift");
        generator->LoadBuilltinTemplates();
        generator->LoadFile(resfiles.load(std::string(pidlfile), ""));

        auto outfiles = generator->Generate(false, std::filesystem::current_path());

        REQUIRE(outfiles.size() > 0);
        for (auto const& path : outfiles)
        {
            auto fname          = path.filename().string();
            auto actualstring   = readlines(path);
            auto expectedstring = readlines(resfiles.load(fname, "res_"));
            CompareLines(actualstring, expectedstring);
        }
    }
}

TEST_CASE("CodeGen::ThriftGenerator::CLOpts1", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"CLOpts1.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::CLOpts2", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"CLOpts2.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::UnionTest", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"UnionTest.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::UserData", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"UserData.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::WebService1", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"SimpleWebService.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::WebService2", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"Metadata.pidl", "ComplexWebService.pidl"}));
}

TEST_CASE("CodeGen::ThriftGenerator::Avid", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"Avid.pidl"}));
}
