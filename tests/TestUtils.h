#pragma once
#include <EmbeddedResource.h>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
DECLARE_RESOURCE_COLLECTION(testdata);

inline void CheckOutputAgainstResource(std::vector<std::string> const& lines, std::filesystem::path const& resourcename)
{
    auto             resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    std::string_view expectedoutput;
    for (auto const& r : resourceCollection)
    {
        if (r.name() == resourcename.filename())
        {
            expectedoutput = r.string();
        }
    }

    std::fstream outputfile(resourcename, std::ios::binary | std::ios::out);
    REQUIRE(outputfile.is_open());
    for (auto& l : lines)
    {
        outputfile << l << "\n";
    }
    outputfile.flush();
    outputfile.close();

    auto   its = expectedoutput.begin();
    size_t row = 0;
    for (auto& l : lines)
    {
        REQUIRE_FALSE((its + l.length()) == expectedoutput.end());
        std::string expectedline(its, its + l.length());
        its += l.length();
        while (its != expectedoutput.end() && (*its == '\n' || *its == '\r')) ++its;
        ++row;
    }
    REQUIRE(its == expectedoutput.end());
}
