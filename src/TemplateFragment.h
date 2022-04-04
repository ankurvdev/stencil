#pragma once
#include "IDL3Generics.h"
#include "tree.h"

#include <memory>
#include <string>
#include <vector>

struct TemplateFragment
{
    Binding::Str::Type                            name;
    std::shared_ptr<Binding::Expression>          body;
    std::unordered_map<std::string, std::wstring> attributes;

    // Debug Info
    std::wstring sourceFileName;
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

    tree<TemplateFragment>               root;
    Binding::Str::Type                   dataSource;
    std::shared_ptr<Binding::Expression> fileName;
    FileType                             fileType;

    Template() = default;
    DELETE_COPY_DEFAULT_MOVE(Template);
};
