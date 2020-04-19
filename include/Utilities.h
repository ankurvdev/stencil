#pragma once
#include "Common.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <stdarg.h>
#include <string>
#include <vector>
inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
#pragma warning(push)
#pragma warning(disable : 4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
#pragma warning(pop)
    return out;
}

inline std::string readfile(std::filesystem::path const& filepath)
{
    std::ifstream file(filepath);
    if (file.fail()) throw std::runtime_error("Cannot Load File : " + filepath.string());
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.reserve((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

inline std::vector<std::string> readlines(std::filesystem::path const& filepath)
{
    std::vector<std::string> lines;
    std::ifstream            file(filepath);
    std::string              line;

    while (std::getline(file, line))
    {
        if (line.length() > 0 && line[line.length() -1] == '\r')
        {
            line.resize(line.length() - 1);
        }
        lines.push_back(std::move(line));
    }
    return lines;
}
#if 0

inline bool beginswith(const std::string& str, const std::string& match, std::string& remainder)
{
    if (str.find(match) == 0)
    {
        remainder = str.substr(match.length());
        return true;
    }
    return false;
}

// trim from start
inline std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](auto c) { return !std::isspace(c); }));
    return s;
}

// trim from end
inline std::string& rtrim(std::string& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](auto c) { return !std::isspace(c); }).base(), s.end());
    return s;
}

// trim from both ends
inline std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

inline std::string replaceall(std::string str, std::string match, std::string replace)
{
    if (match == replace) return str;
    for (auto it = str.rfind(match); it != std::string::npos; it = str.rfind(match, it))
    {
        str = str.replace(it, match.length(), replace);
    }

    return str;
}
#endif