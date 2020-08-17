#pragma once
#include <EmbeddedResource.h>

#pragma warning(push, 0)
#pragma warning(disable : 4868)
#pragma warning(disable : 4738)
#include <catch2/catch.hpp>
#include <dtl/dtl.hpp>
#pragma warning(pop

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
DECLARE_RESOURCE_COLLECTION(testdata);

inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
#pragma warning(push)
#pragma warning(disable : 4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
#pragma warning(pop)
    return out;
}

inline std::vector<std::string> readlines(std::filesystem::path const& filepath)
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

inline void CompareLines(std::vector<std::string> const& actualstring,
                         std::vector<std::string> const& expectedstring,
                         std::string const&              resname = "test")
{

    dtl::Diff<std::string, std::vector<std::string>> d(actualstring, expectedstring);
    d.compose();                // construct an edit distance and LCS and SES
    d.composeUnifiedHunks();    // construct a difference as Unified Format with SES.

    if (actualstring != expectedstring)
    {
        d.printUnifiedFormat();    // print a difference as Unified Format.
        {
            std::ofstream f(resname + "_failure.txt");
            for (auto& l : actualstring)
            {
                f << l << std::endl;
            }
            f.flush();
            f.close();
        }
        REQUIRE_FALSE("Comparison Failed: Output: failure.txt");
    }
}

inline void CheckOutputAgainstResource(std::vector<std::string> const& lines, std::string const& resourcename)
{
    ResourceFileManager resfiles;
    CompareLines(lines, readlines(resfiles.load(resourcename, "res_")), resourcename);
}
