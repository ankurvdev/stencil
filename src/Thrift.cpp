#include "Thrift.ly.h"

#include <cstdio>
#include <fstream>
#include <iostream>

namespace IDL::Lang::Thrift
{

std::shared_ptr<IDL::Struct> CreateStruct(Context& context, Str::Type& id)
{
    return context.program.CreateStorageObject<IDL::Struct>(std::move(id), TypeAttributeList{});
}

void AddFieldsToStruct(Context& /* context */, std::shared_ptr<IDL::Struct> strct, FieldList& fields, TypeAttributeList& map)
{
    strct->AddAttributes(map);
    for (auto& f : fields)
    {
        strct->CreateField(f.m_FieldType.value(), std::move(f.m_Id), std::move(f.m_FieldValue), std::move(f.m_AttributeMap));
    }

    fields.clear();
    map.reset();
}

std::shared_ptr<IDL::Variant> CreateVariant(Context& context, Str::Type& id, FieldList& fields, TypeAttributeList& map)
{
    auto strct = context.program.CreateStorageObject<IDL::Variant>(std::move(id), map);
    for (auto& f : fields) { strct->CreateField(f.m_FieldType.value(), std::move(f.m_Id), f.m_FieldValue, std::move(f.m_AttributeMap)); }

    fields.clear();
    map.reset();
    return strct;
}

static void CreateRelectionshipDefinitionRecursively(Context&                         context,
                                                     Str::Type&&                      name,
                                                     AttributeComponentList&          map,
                                                     ComponentList&                   componentmap,
                                                     AttributeComponentList::iterator atit)
{
    TypeAttributeList attributemap;
    for (auto it = atit; it != map.end(); ++it)
    {
        if (it->second.size() == 1) { componentmap[Str::Copy(it->first)] = Str::Copy(it->second[0]); }
        else
        {
            for (auto vit = it->second.begin(); vit != it->second.end(); ++vit)
            {
                componentmap[Str::Copy(it->first)] = Str::Copy(*vit);
                auto itnew                         = it;
                CreateRelectionshipDefinitionRecursively(context, Str::Copy(name), map, componentmap, ++itnew);
            }
            return;
        }
    }

    auto def = context.program.TryLookup<IDL::AttributeDefinition>(name);
    if (!def.has_value() && componentmap.size() == 1)
    {
        auto  it        = componentmap.begin();
        auto& container = context.program.Lookup<IDL::Container>(name);
        auto& strct     = context.program.Lookup<IDL::Struct>(it->second);

        IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
        containermap.insert(std::make_pair(Str::Create(L"object"), strct.shared_from_this()));
        auto existing = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
        if (!existing.has_value())
        {
            existing = context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(
                container, std::move(containermap), existing, attributemap);
        }

        strct.CreateNamedObject<IDL::AttributeTag>(existing.value()->GetFieldName(), def, existing.value());
    }
    else
    {

        for (auto& it : componentmap)
        {
            IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
            for (auto& it1 : componentmap)
            {
                auto& strct                        = context.program.Lookup<IDL::Struct>(it1.second);
                containermap[Str::Copy(it1.first)] = strct.shared_from_this();
            }

            auto& strct     = context.program.Lookup<IDL::Struct>(it.second);
            auto& container = context.program.Lookup<IDL::Container>(def.value()->getComponentName(it.first));
            auto  existing  = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
            if (!existing.has_value())
            {
                existing = context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(
                    container, std::move(containermap), existing, attributemap);
            }

            strct.CreateNamedObject<IDL::AttributeTag>(existing.value()->GetFieldName(), def, existing.value());
        }
    }
}

void CreateAttribute(Context& context, Str::Type& name, AttributeComponentList& map)
{
    ComponentList componentmap;
    return CreateRelectionshipDefinitionRecursively(context, std::move(name), map, componentmap, map.begin());
}
}    // namespace IDL::Lang::Thrift

struct ThriftGenerator : Generator
{
    virtual void LoadFile(std::filesystem::path const& inputFile) override
    {
        IDLDebug::ErrorAggregator errorAggregator;

        IDL::Lang::Thrift::Context context{Program_(), TypeDefinitions_()};
        context.program.SetFileName(inputFile);
        try
        {
            IDL::Lang::Thrift::LoadFile(context, inputFile);
        } catch (std::exception const& ex)
        {
            for (auto& err : context.errors)
            {
                std::cerr << "error: " << inputFile.string() << "[" << err.line << ":" << err.col << "] " << err.msg;
            }
            std::wcerr << " " << context.errors.size() << std::endl << errorAggregator.GetErrors() << std::endl << ex.what() << std::endl;
            throw;
        }
    }
};

std::unique_ptr<Generator> Generator::CreateThriftGenerator()
{
    return std::make_unique<ThriftGenerator>();
}

void IDL::Lang::Thrift::Context::Import(Str::View const& name)
{
    auto path = (program.File().parent_path() / std::filesystem::path(Str::Value(name).c_str()));

    std::shared_ptr<IDL::Program> importedProgram = std::make_shared<IDL::Program>();
    importedProgram->SetFileName(path);
    IDL::Lang::Thrift::Context importedCtx{*importedProgram.get(), typeDefinitions};
    IDL::Lang::Thrift::LoadFile(importedCtx, path);
    program.Import(*importedProgram);
}
