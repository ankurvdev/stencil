#pragma once
#include "IDL2.h"
#include "TemplateFragment.h"

#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Generator
{
    virtual ~Generator() = default;

    void LoadBuilltinTemplates();
    void LoadTemplate(std::filesystem::path const& templatePath);

    void FinalizeTypeDefinitions();

    virtual void LoadFile(std::filesystem::path const& inputFile) = 0;

    std::vector<std::filesystem::path> Generate(bool isDryRun, std::filesystem::path const& outDir);

    static std::unique_ptr<Generator> CreateThriftGenerator();

    static std::unique_ptr<Generator> Get(std::string const& type)
    {
        if (type == "thrift") return CreateThriftGenerator();

        throw std::invalid_argument("Valid Generators [thrift]. Unknown Generator Type : " + type);
    }

    using MutatorAccessorDefinition = IDL::Container::MutatorAccessorDefinition;

    struct FieldTypeDecl
    {
        Binding::Str::Type                     name;
        Binding::Str::Type                     baseField;
        std::shared_ptr<Binding::AttributeMap> annotationMap;
        std::vector<MutatorAccessorDefinition> mutators;
        std::vector<MutatorAccessorDefinition> accessors;

        void Merge(FieldTypeDecl&& decl);
    };

    struct ContainerTypeDecl : FieldTypeDecl
    {
        std::vector<Binding::Str::Type> args;

        void Merge(ContainerTypeDecl&& decl)
        {
            if (decl.args.size() != 0)
            {
                if (args.size() == 0)
                {
                    args = std::move(decl.args);
                }
                else
                {
                    throw std::logic_error("Cannot reset args");
                }
            }
            FieldTypeDecl::Merge(std::move(decl));
        }
    };

    protected:
    IDL::Program& Program_() { return *_program; }

    private:
    using StrType = Binding::Str::Type;

    bool _finalized{false};

    std::vector<FieldTypeDecl>     _fieldTypeDecls;
    std::vector<ContainerTypeDecl> _containerDecls;

    std::map<StrType, size_t>                               _fieldTypeDeclMap;
    std::map<StrType, size_t>                               _containerDeclMap;
    std::map<StrType, std::unordered_map<StrType, StrType>> _relationshipDefs;

    FieldTypeDecl _structDefault, _interfaceDefault, _unionDefault, _typedefDefault, _fnargsDefault;

    FieldTypeDecl& _FindOrInsertFieldTypeDecl(StrType const& name)
    {
        auto it = _fieldTypeDeclMap.find(name);
        if (it == _fieldTypeDeclMap.end())
        {
            _fieldTypeDeclMap[name] = _fieldTypeDecls.size();
            _fieldTypeDecls.push_back(FieldTypeDecl{});
            return _fieldTypeDecls.back();
        }
        return _fieldTypeDecls.at(it->second);
    }

    ContainerTypeDecl& _FindOrInsertContainerDecls(StrType const& name)
    {
        auto it = _containerDeclMap.find(name);
        if (it == _containerDeclMap.end())
        {
            _containerDeclMap[name] = _containerDecls.size();
            _containerDecls.push_back(ContainerTypeDecl{});
            return _containerDecls.back();
        }
        return _containerDecls.at(it->second);
    }

    void _AddTypeDefinitions(std::string_view const& name, std::string_view const& text);

    void _AddTemplate(std::string_view const& name, std::string_view const& text);
    void _AddContent(std::string_view const& name, std::string_view const& text);

    std::shared_ptr<IDL::Program> _program = std::make_shared<IDL::Program>();
    std::vector<Template>         _templates;
};