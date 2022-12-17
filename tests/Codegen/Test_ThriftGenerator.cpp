#include "TestUtils.h"
DECLARE_RESOURCE_COLLECTION(testdata);
#include "Generator.h"

static void RunTest(std::vector<std::string> const& pidlfiles)
{
    TestCommon::ResourceFileManager resfiles;
    for (auto const& pidlfile : pidlfiles) { resfiles.load(std::string(pidlfile), ""); }

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
            auto          fname = path.filename().string();
            std::ifstream ifs(path);
            TestCommon::CheckResource<TestCommon::StrFormat>(TestCommon::ReadStrStream(ifs), fname);
        }
    }
}

TEST_CASE("CodeGen", "[ThriftGenerator]")
{
    std::vector<std::string> pidlfiles;
    for (const auto res : LOAD_RESOURCE_COLLECTION(testdata))
    {
        auto fname = std::filesystem::path(res.name());
        if (fname.extension().string() == ".pidl") { pidlfiles.push_back(fname.string()); }
    }
    RunTest({"EnumConstsInitializers.pidl"});
    REQUIRE(!pidlfiles.empty());
    REQUIRE_NOTHROW(RunTest(pidlfiles));
}
