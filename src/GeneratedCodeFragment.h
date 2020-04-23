#pragma once
#include "IDL2.h"
#include "TemplateFragment.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>

struct Content
{
    tree<TemplateFragment>::iterator templ;
    Binding::IBindable const&        dataContext;
    std::wstring                     code;
};

struct GeneratedCode
{
    tree<Binding::Str::Type> content{};
    std::wstring             datasource;
    std::filesystem::path    filename;
};
