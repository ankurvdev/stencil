#pragma once
#if defined USE_EMBEDRESOURCE
#include <EmbeddedResource.h>
#endif

#pragma warning(push, 3)
#pragma warning(disable : 4868)
#pragma warning(disable : 4738)
#pragma warning(disable : 4365)    // signed unsigned mismatch
#pragma warning(disable : 5219)    // implicit conversion from 'uint64_t' to 'double', possible loss of data
#pragma warning(disable : 5204)    // class has virtual functions, but its trivial destructor is not virtual
#pragma warning(disable : 4668)    // not defined as a preprocessor macro
#pragma warning(disable : 5039)    // pointer or reference to potentially throwing function passed to 'extern)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <catch2/catch.hpp>
#include <dtl/dtl.hpp>
#include <fmt/ostream.h>

#pragma clang diagnostic pop
#pragma warning(pop)

#include <filesystem>
#include <fstream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#if defined USE_EMBEDRESOURCE
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

inline void CompareLines(std::vector<std::string> const& actualstring,
                         std::vector<std::string> const& expectedstring,
                         std::string_view const&         resname = "test")
{

    dtl::Diff<std::string, std::vector<std::string>> d(expectedstring, actualstring);
    d.compose();                // construct an edit distance and LCS and SES
    d.composeUnifiedHunks();    // construct a difference as Unified Format with SES.

    if (actualstring != expectedstring)
    {
        d.printUnifiedFormat();    // print a difference as Unified Format.
        {
            std::ofstream f(std::string(resname) + ".txt");
            for (auto& l : actualstring) { f << l << "\n"; }
            f.flush();
            f.close();
        }
        FAIL("Comparison Failed: Output: " + std::string(resname) + ".txt");
    }
}

#if defined USE_EMBEDRESOURCE

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
        for (auto const r : resourceCollection)
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

inline std::vector<std::string> LoadResource(std::string_view const& name)
{
    auto testresname = GeneratePrefixFromTestName() + std::string(name) + ".txt";

    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const r : resourceCollection)
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

inline void CheckOutputAgainstResource(std::vector<std::string> const& lines, std::string_view const& resourcename)
{
    CompareLines(lines, LoadResource(resourcename), GeneratePrefixFromTestName() + std::string(resourcename));
}
template <typename TData> inline void CompareBinaryOutputAgainstResource(TData const& actual, std::string_view const& resourcename)
{
    auto testresname        = GeneratePrefixFromTestName() + std::string(resourcename) + ".bin";
    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const r : resourceCollection)
    {
        auto resname = wstring_to_string(r.name());
        if (resname == testresname || resname == resourcename)
        {
            auto data = r.string();
            auto spn  = std::span<const char>(reinterpret_cast<char const*>(actual.data()), actual.size());
            INFO("Checking Resource : " + resname)
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
                FAIL(fmt::format("Binary comparison failed at Index = {} => {} != {}", index, uint32_t{*it1}, uint32_t{*it2}));
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
