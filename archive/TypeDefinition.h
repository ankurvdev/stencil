#pragma once
#include "Thrift.h"
#include <iostream>

namespace IDL::Lang::TypeDefinition
{
template <typename T> using StrOps = Binding::StrOps<T>;

using Attributes        = std::shared_ptr<Binding::AttributeMap>;
using TextValue         = Str::Type;
using KVP               = std::pair<Str::Type, Str::Type>;
using ContainerTypeList = std::vector<Str::Type>;
using NameWithTypeList  = std::pair<Str::Type, std::vector<Str::Type>>;
using AttributeMap      = std::unordered_map<Str::Type, Str::Type>;

inline Attributes CreateAttributeMapEntry(Attributes& ptr, KVP& entry)
{
    ptr->AddEntry(std::move(entry.first), Binding::Expression::Create(std::move(entry.second), Str::Create(L"%"), Str::Create(L"%"), L':'));
    return ptr;
}

inline Attributes CreateAttributeMapEntry(KVP& entry)
{
    auto ptr = std::make_shared<Binding::AttributeMap>();
    return CreateAttributeMapEntry(ptr, entry);
}

class Context
{
    public:
    Context(IDL::Program& programIn) : program(programIn) {}
    ONLY_MOVE_CONSTRUCT(Context);

    void CreateFieldDefinition(NameWithTypeList&& name, NameWithTypeList&& basename, Attributes&& map)
    {
        if (basename.second.size() > 0) throw std::logic_error("Basename for field definitions cannot have typelist");
        if (name.second.size() > 0)
        {
            auto base = program.TryLookup<IDL::Container>(basename.first);
            program.CreateNamedObject<IDL::Container>(std::move(name.first), std::move(name.second), base, map);
        }
        else
        {
            program.CreateFieldTypeObject<IDL::NativeFieldType>(std::move(name.first), program.TryGetFieldTypeName(basename.first), map);
        }
    }
    void CreateAttributeDefinition(Str::Type&& name, AttributeMap&& components)
    {
        program.CreateNamedObject<IDL::AttributeDefinition>(std::move(name), nullptr, std::move(components));
    }

    bool Debug() { return false; }
    void NotifyError(int line, int col, std::string const& msg)
    {
        std::cerr << "Error(TypeDefinition)[" << line << ":" << col << "] : " << msg << std::endl;
    }

    IDL::Program& program;
};

}    // namespace IDL::Lang::TypeDefinition