#include "TestUtils.h"
DECLARE_RESOURCE_COLLECTION(testdata);
#include "Generator.h"
#pragma warning(push, 0)
#include "dtl/dtl.hpp"
#pragma warning(pop)

static inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
#pragma warning(push)
#pragma warning(disable : 4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
#pragma warning(pop)
    return out;
}

static inline std::vector<std::string> readlines(std::filesystem::path const& filepath)
{
    std::vector<std::string> lines;
    std::ifstream            file(filepath);
    std::string              line;

    while (std::getline(file, line))
    {
        if (line.length() > 0 && line[line.length() - 1] == '\r')
        {
            line.resize(line.length() - 1);
        }
        lines.push_back(std::move(line));
    }
    return lines;
}

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
        auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
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

            dtl::Diff<std::string, std::vector<std::string>> d(actualstring, expectedstring);
            d.compose();                // construct an edit distance and LCS and SES
            d.composeUnifiedHunks();    // construct a difference as Unified Format with SES.

            if (actualstring != expectedstring)
            {
                d.printUnifiedFormat();    // print a difference as Unified Format.
                REQUIRE_FALSE("Comparison Failed");
            }
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
