#pragma once
#include "IDL2.h"
#include "TemplateFragment.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>

struct GeneratedCode
{
    tree<Binding::Str::Type> content{};
    std::wstring             datasource{};
    std::filesystem::path    filename{};

    GeneratedCode() = default;
    DELETE_COPY_DEFAULT_MOVE(GeneratedCode);
};
