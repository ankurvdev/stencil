#pragma once
#include "IDL2.h"
#include "TemplateFragment.h"

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Generator
{
    virtual ~Generator() = default;

    void LoadBuilltinTemplates();
    void LoadTemplate(std::filesystem::path const& templatePath);

    virtual void LoadFile(std::filesystem::path const& inputFile) = 0;

    std::vector<std::filesystem::path> Generate(bool isDryRun, std::filesystem::path const& outDir);

    static std::unique_ptr<Generator> CreateThriftGenerator();

    static std::unique_ptr<Generator> Get(std::string const& type)
    {
        if (type == "thrift") return CreateThriftGenerator();

        throw std::invalid_argument("Valid Generators [thrift]. Unknown Generator Type : " + type);
    }

    protected:
    IDL::Program& Program_() { return *_program; }

    private:
    void _AddTemplate(std::string_view const& name, std::string_view const& text);
    void _AddTypeDefinitions(std::string_view const& name, std::string_view const& text);
    void _AddContent(std::string_view const& name, std::string_view const& text);

    std::shared_ptr<IDL::Program> _program = std::make_shared<IDL::Program>();
    std::vector<Template>         _templates;
};