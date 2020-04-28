#include "DebugInfo.h"
#include "Thrift.ly.h"

#include <cstdio>
#include <fstream>
#include <iostream>

namespace IDL::Lang::Thrift
{
void Context::Import(Str::View const& name)
{
    std::ifstream f(program.File().parent_path() / std::filesystem::path(Str::Value(name).c_str()));
    Load(*this, f);
}

std::shared_ptr<IDL::Struct> CreateStruct(Context& context, Str::Type& id, FieldList& fields, TypeAttributeList& map)
{
    auto strct = context.program.CreateStorageObject<IDL::Struct>(std::move(id), map);
    for (auto& f : fields)
    {
        strct->CreateField(f.m_FieldType.value(), std::move(f.m_Id), std::move(f.m_FieldValue), std::move(f.m_AttributeMap));
    }

    fields.clear();
    map.reset();
    return strct;
}

std::shared_ptr<IDL::Union> CreateUnion(Context& context, Str::Type& id, FieldList& fields, TypeAttributeList& map)
{
    auto strct = context.program.CreateStorageObject<IDL::Union>(std::move(id), map);
    for (auto& f : fields)
    {
        strct->CreateField(f.m_FieldType.value(), std::move(f.m_Id), f.m_FieldValue, std::move(f.m_AttributeMap));
    }

    fields.clear();
    map.reset();
    return strct;
}

void CreateRelectionshipDefinitionRecursively(Context&                            context,
                                              Str::Type&&                         name,
                                              RelationshipComponentList&          map,
                                              ComponentList&                      componentmap,
                                              RelationshipComponentList::iterator atit)
{
    TypeAttributeList attributemap;
    for (auto it = atit; it != map.end(); ++it)
    {
        if (it->second.size() == 1)
        {
            componentmap[Str::Copy(it->first)] = Str::Copy(it->second[0]);
        }
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

    auto def = context.program.TryLookup<IDL::RelationshipDefinition>(name);
    if (!def.has_value() && componentmap.size() == 1)
    {
        auto  it        = componentmap.begin();
        auto& container = context.program.Lookup<IDL::Container>(name);
        auto& strct     = context.program.Lookup<IDL::Struct>(it->second);

        IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
        containermap[Str::Create(L"object")] = strct.shared_from_this();
        auto existing = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
        if (!existing.has_value())
        {
            existing = context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(
                container, std::move(containermap), existing, attributemap);
        }

        strct.CreateNamedObject<IDL::RelationshipTag>(existing.value()->GetFieldName(), def, existing.value());
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

            strct.CreateNamedObject<IDL::RelationshipTag>(existing.value()->GetFieldName(), def, existing.value());
        }
    }
}

void CreateRelationship(Context& context, Str::Type& name, RelationshipComponentList& map)
{
    ComponentList componentmap;
    return CreateRelectionshipDefinitionRecursively(context, std::move(name), map, componentmap, map.begin());
}
}    // namespace IDL::Lang::Thrift

struct ThriftGenerator : Generator
{
    virtual void LoadFile(std::filesystem::path const& inputFile) override
    {
        IDL::Lang::Thrift::Context context{Program_()};
        context.program.SetFileName(inputFile);
        try
        {
            IDL::Lang::Thrift::LoadFile(context, inputFile);
        }
        catch (std::exception const& ex)
        {
            for (auto& err : context.errors)
            {
                std::cerr << "error: " << inputFile.string() << "[" << err.line << ":" << err.col << "] " << err.msg;
            }
            std::wcerr << context.errors.size() << IDLDebug::ErrorAggregator::Get().GetErrors() << ex.what() << std::endl;
            throw;
        }
    }
};

std::unique_ptr<Generator> Generator::CreateThriftGenerator()
{
    return std::make_unique<ThriftGenerator>();
}
