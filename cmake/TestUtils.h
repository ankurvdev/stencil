#pragma once
#if defined HAVE_EMBEDRESOURCE
#include <EmbeddedResource.h>
#endif

#include "CommonMacros.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_FMT_WARNINGS
#pragma warning(disable : 4388)    // signed / unsigned mismatch (Catch2)
#include <catch2/catch_all.hpp>
#include <dtl/dtl.hpp>
#include <fmt/ostream.h>
SUPPRESS_WARNINGS_END

#include <filesystem>
#include <fstream>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#if defined HAVE_EMBEDRESOURCE
DECLARE_RESOURCE_COLLECTION(testdata);
#endif

inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
#pragma warning(push)
#pragma warning(disable : 4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
#pragma warning(pop)
    return out;
}

#if !defined _WIN32
inline bool  IsDebuggerPresent()
{
    return true;
}
#endif

inline std::vector<std::string> readlines(std::istream& istr)
{
    std::vector<std::string> lines;
    std::string              line;

    while (std::getline(istr, line))
    {
        if (line.length() > 0 && line[line.length() - 1] == '\r') { line.resize(line.length() - 1); }
        lines.push_back(std::move(line));
    }
    return lines;
}

inline std::vector<std::string> readlines(std::filesystem::path const& path)
{
    std::fstream file(path);
    return readlines(file);
}

inline void WriteStrResourse(std::vector<std::string> const& actualstring, std::string_view const& resname)
{
    auto outf = std::filesystem::absolute(std::string(resname) + ".txt");
    {
        std::ofstream f(outf);
        for (auto& l : actualstring) { f << l << "\n"; }
        f.flush();
        f.close();
    }
    FAIL(fmt::format("Comparison Failed: Output: \n{}", outf.string()));
}

inline void WriteBinResourse(std::vector<std::string> const& actualstring, std::string_view const& resname)
{
    auto outf = std::filesystem::absolute(std::string(resname) + ".bin");
    {
        std::ofstream f(outf, std::ios::binary);
        for (auto& l : actualstring)
        {
            size_t size = l.size();
            f.write(reinterpret_cast<const char*>(&size), sizeof(size));
            f.write(l.data(), static_cast<std::streamsize>(l.size()));
        }
        f.flush();
        f.close();
    }
    FAIL(fmt::format("Comparison Failed: Output: \n{}", outf.string()));
}

inline void PrintDiff(std::vector<std::string> const& actualstring, std::vector<std::string> const& expectedstring)
{

    dtl::Diff<std::string, std::vector<std::string>> d(expectedstring, actualstring);
    d.compose();                // construct an edit distance and LCS and SES
    d.composeUnifiedHunks();    // construct a difference as Unified Format with SES.

    if (actualstring != expectedstring)
    {

        if (actualstring.size() == expectedstring.size())
        {
            for (size_t i = 0; i < actualstring.size(); i++)
            {
                if (actualstring[i] != expectedstring[i])
                {
                    dtl::Diff<char, std::string> ld(expectedstring[i], actualstring[i]);
                    ld.compose();
                    auto                     ses      = ld.getSes().getSequence();
                    int                      lasttype = 0;
                    std::string              merged;
                    std::vector<std::string> deltas;
                    for (auto& sesobj : ses)
                    {
                        if (sesobj.second.type != lasttype)
                        {
                            if (lasttype == dtl::SES_COMMON) { merged = fmt::format("[{}:{}]", i, sesobj.second.afterIdx); }
                            merged += (sesobj.second.type == dtl::SES_ADD ? '+' : '-');
                        }
                        if (sesobj.second.type == dtl::SES_COMMON)
                        {
                            if (merged.size() > 0) deltas.push_back(std::move(merged));
                        }
                        else { merged += sesobj.first; }
                        lasttype = sesobj.second.type;
                    }
                    if (merged.size() > 0) deltas.push_back(std::move(merged));
                    for (auto& delta : deltas) { std::cout << delta << " "; }
                    std::cout << std::endl;
                }
            }
        }
        else
        {
            d.printUnifiedFormat();    // print a difference as Unified Format.
        }
    }
}

inline void CompareStrLines(std::vector<std::string> const& actualstring,
                            std::vector<std::string> const& expectedstring,
                            std::string_view const&         resname = "test")
{
    if (actualstring != expectedstring)
    {
        PrintDiff(actualstring, expectedstring);
        WriteStrResourse(actualstring, resname);
    }
}

inline void CompareBinLines(std::vector<std::string> const& actualstring,
                            std::vector<std::string> const& expectedstring,
                            std::string_view const&         resname = "test")
{
    if (actualstring != expectedstring)
    {
        PrintDiff(actualstring, expectedstring);
        WriteBinResourse(actualstring, resname);
    }
}

#if defined HAVE_EMBEDRESOURCE

inline std::string GeneratePrefixFromTestName()
{
    auto prefix = Catch::getResultCapture().getCurrentTestName() + "_";
    for (auto& c : prefix)
    {
        if (std::isalpha(c) || std::isdigit(c)) continue;
        c = '_';
    }
    return prefix;
}

struct ResourceFileManager
{

    ResourceFileManager() = default;
    ~ResourceFileManager()
    {
        for (auto const& [k, v] : _openedfiles) { std::filesystem::remove(v); }
    }

    auto load(std::string const& name, std::string const& prefix)
    {
        auto testresname = GeneratePrefixFromTestName() + name;

        auto it = _openedfiles.find(testresname);
        if (it != _openedfiles.end()) { return it->second; }
        auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
        for (auto const& r : resourceCollection)
        {
            auto resname = wstring_to_string(r.name());
            if (resname == testresname || resname == name)
            {
                auto          path = std::filesystem::current_path() / (prefix + resname);
                std::ofstream f(path);
                auto const&   str = r.string();
                if (!f.is_open()) { throw std::runtime_error("Cannot write resource file : " + path.string()); }
                f << str;
                f.close();
                _openedfiles[resname] = path;
                return path;
            }
        }
        throw std::logic_error("Cannot find resource : " + testresname);
    }
    std::unordered_map<std::string, std::filesystem::path> _openedfiles;
};

inline std::vector<std::string> LoadStrResource(std::string_view const& name)
{
    auto testresname = GeneratePrefixFromTestName() + std::string(name) + ".txt";

    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const& r : resourceCollection)
    {
        auto resname = wstring_to_string(r.name());
        if (resname == testresname || resname == name)
        {
            std::string       str(r.string());
            std::stringstream ss(str);
            return readlines(ss);
        }
    }
    return std::vector<std::string>();
}

inline std::vector<std::string> LoadBinResource(std::string_view const& name)
{
    auto testresname = GeneratePrefixFromTestName() + std::string(name) + ".bin";

    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const& r : resourceCollection)
    {
        auto resname = wstring_to_string(r.name());
        if (resname == testresname || resname == name)
        {
            std::string              str(r.string());
            std::istringstream       ss(str);
            std::size_t              size;
            std::vector<std::string> data;
            ss.peek();
            while (!ss.eof())
            {
                ss.read(reinterpret_cast<char*>(&size), sizeof(size));
                if (size > 1024 * 1024) { return data; }
                std::string line;
                line.resize(size);
                ss.read(line.data(), static_cast<std::streamsize>(size));
                if (ss.eof()) throw std::logic_error("Reached end unexpectedly");
                data.push_back(line);
                ss.peek();
            }
            return data;
        }
    }
    return std::vector<std::string>();
}

inline void CheckOutputAgainstStrResource(std::vector<std::string> const& lines, std::string_view const& resourcename)
{
    CompareStrLines(lines, LoadStrResource(resourcename), GeneratePrefixFromTestName() + std::string(resourcename));
}

inline void CheckOutputAgainstBinResource(std::vector<std::string> const& lines, std::string_view const& resourcename)
{
    CompareBinLines(lines, LoadBinResource(resourcename), GeneratePrefixFromTestName() + std::string(resourcename));
}

template <typename TData> inline void CompareBinaryOutputAgainstResource(TData const& actual, std::string_view const& resourcename)
{
    auto testresname        = GeneratePrefixFromTestName() + std::string(resourcename) + ".bin";
    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const& r : resourceCollection)
    {
        auto resname = wstring_to_string(r.name());
        if (resname == testresname || resname == resourcename)
        {
            auto data = r.string();
            auto spn  = std::span<const char>(reinterpret_cast<char const*>(actual.data()), actual.size());
            INFO("Checking Resource : " + resname);
            if (data.size() == spn.size() && std::equal(spn.begin(), spn.end(), data.begin())) { return; }
            std::ofstream f(testresname, std::ios::binary);
            f.write(reinterpret_cast<char const*>(actual.data()), static_cast<std::streamsize>(actual.size()));
            f.flush();
            f.close();
            size_t index = 0;
            auto   it2   = data.begin();
            for (auto it = spn.begin(); it != spn.end(); ++it, ++it2, ++index)
            {
                if ((*it) == (*it2)) continue;
                FAIL(fmt::format("Binary comparison failed at Index = {} => {} != {}", index, int32_t{*it}, int32_t{*it2}));
            }
            return;
        }
    }
    std::ofstream f(testresname, std::ios::binary);
    f.write(reinterpret_cast<char const*>(actual.data()), static_cast<std::streamsize>(actual.size()));
    f.flush();
    f.close();
    FAIL("Cannot find reference resource : " + testresname);
}

inline void CompareFileAgainstResource(std::filesystem::path const& actualf, std::string_view const& resourcename)
{
    std::ifstream         instream(actualf, std::ios::in | std::ios::binary);
    std::vector<char>     actualdata((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    std::span<const char> spn = actualdata;
    CompareBinaryOutputAgainstResource(spn, resourcename);
}
#endif
