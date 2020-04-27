#include "TestUtils.h"
DECLARE_RESOURCE_COLLECTION(testdata);
#include "Generator.h"
#include "Utilities.h"
#include "dtl/dtl.hpp"

struct ResourceFileManager
{
    ResourceFileManager() = default;
    ~ResourceFileManager()
    {
        for (auto const& [k, v] : _openedfiles)
        {
            std::filesystem::remove(v);
        }
    }

    auto load(std::string const& name, std::string const& prefix)
    {
        auto it = _openedfiles.find(name);
        if (it != _openedfiles.end())
        {
            return it->second;
        }
        auto             resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
        std::string_view expectedoutput;
        for (auto const& r : resourceCollection)
        {
            if (wstring_to_string(r.name()) == name)
            {
                auto          path = std::filesystem::current_path() / (prefix + name);
                std::ofstream f(path);
                if (!f.is_open())
                {
                    throw std::runtime_error("Cannot write resource file : " + path.string());
                }
                f << r.string();
                f.close();
                _openedfiles[name] = path;
                return path;
            }
        }
        throw std::logic_error("Cannot find resource : " + name);
    }

    std::unordered_map<std::string, std::filesystem::path> _openedfiles;
};

void RunTest(std::string const& pidlfile)
{
    ResourceFileManager resfiles;

    auto generator = Generator::Get("thrift");
    generator->LoadBuilltinTemplates();
    generator->LoadFile(resfiles.load(pidlfile, ""));

    auto outfiles = generator->Generate(false, std::filesystem::current_path());

    REQUIRE(outfiles.size() > 0);
    for (auto const& path : outfiles)
    {
        auto fname          = path.filename().string();
        auto actualstring   = readlines(path);
        auto expectedstring = readlines(resfiles.load(fname, "res_"));

        dtl::Diff<std::string, std::vector<std::string>> d(actualstring, expectedstring);
        d.compose();                // construct an edit distance and LCS and SES
        d.composeUnifiedHunks();    // construct a difference as Unified Format with SES.
        d.printUnifiedFormat();     // print a difference as Unified Format.
        REQUIRE(actualstring == expectedstring);
    }
}

TEST_CASE("CodeGen::ThriftGenerator::CLOpts1", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest("CLOpts1.pidl"));
}

TEST_CASE("CodeGen::ThriftGenerator::CLOpts2", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest("CLOpts2.pidl"));
}

TEST_CASE("CodeGen::ThriftGenerator::UnionTest", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest("UnionTest.pidl"));
}

TEST_CASE("CodeGen::ThriftGenerator::UserData", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest("UserData.pidl"));
}

TEST_CASE("CodeGen::ThriftGenerator::WebService", "[ThriftGenerator]")
{
    REQUIRE_NOTHROW(RunTest("SimpleWebService.pidl"));
}
