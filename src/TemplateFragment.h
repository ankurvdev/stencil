#pragma once
#include "IDL3Generics.h"
#include "tree.h"
#include <string>
#include <vector>

struct TemplateFragment
{
    Binding::Str::Type                            name;
    Binding::Expression                           body;
    std::unordered_map<std::string, std::wstring> attributes;

    // Debug Info
    std::string sourceFileName;
    size_t      rowstart;
    size_t      rowend;
};

struct Template
{
    enum FileType
    {
        Code,
        Header
    };

    tree<TemplateFragment> root;
    Binding::Str::Type     dataSource;
    Binding::Expression    fileName;
    FileType               fileType;
};