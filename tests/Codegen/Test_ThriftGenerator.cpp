#define CATCH_CONFIG_MAIN

#include "TestUtils.h"
DECLARE_RESOURCE_COLLECTION(testdata);
#include "Generator.h"

static void RunTest(std::initializer_list<std::string_view const> const& pidlfiles)
{
    ResourceFileManager resfiles;

    for (auto const& pidlfile : pidlfiles)
    {
        auto generator = Generator::Get("thrift");
        generator->LoadBuilltinTemplates();
        generator->LoadFile(resfiles.load(std::string(pidlfile), ""));
        auto outfiles = generator->Generate(false, std::filesystem::current_path());
    }

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

TEST_CASE("CodeGen", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest({"CLOpts1.pidl",
                             "CLOpts2.pidl",
                             "VariantTest.pidl",
                             "UserData.pidl",
                             "SimpleWebService.pidl",
                             "Metadata.pidl",
                             "ComplexWebService.pidl",
                             "Avid.pidl"}));
}
