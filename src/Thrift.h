#pragma once
#include "Generator.h"
#include "IDL2.h"

#include <algorithm>
namespace std
{
template <typename TStr1, typename TStr2> bool iequals(TStr1 const& str1, TStr2 const& str2)
{
    return std::equal(
        std::begin(str1), std::end(str1), std::begin(str2), std::end(str2), [](int a, int b) { return tolower(a) == tolower(b); });
}
}    // namespace std

namespace IDL::Lang::Thrift
{
template <typename T> using StrOps = Binding::StrOps<T>;
using Str                          = Binding::Str;
class Context;

struct Function;
struct Field;

using TypeAttribute     = std::pair<Str::Type, Str::Type>;
using TypeAttributeList = std::shared_ptr<Binding::AttributeMap>;

using Program   = std::optional<std::shared_ptr<IDL::Program>>;
using Typedef   = std::optional<std::shared_ptr<IDL::Typedef>>;
using Struct    = std::optional<std::shared_ptr<IDL::Struct>>;
using Union     = std::optional<std::shared_ptr<IDL::Union>>;
using Interface = std::optional<std::shared_ptr<IDL::Interface>>;
using FieldType = std::optional<std::shared_ptr<IDLGenerics::IFieldType>>;

using Id = Str::Type;

using ConstValue = std::shared_ptr<IDLGenerics::ConstValue>;

using ConstValueList = std::vector<ConstValue>;
using ConstValueDict = std::vector<std::pair<ConstValue, ConstValue>>;
using FunctionList   = std::vector<Function>;
using FieldList      = std::vector<Field>;
using FieldTypeList  = std::vector<FieldType>;
using IdList         = std::vector<Str::Type>;

using RelationshipComponent     = std::pair<Str::Type, std::vector<Str::Type>>;
using RelationshipComponentList = std::unordered_map<Str::Type, std::vector<Str::Type>>;
using ComponentList             = std::unordered_map<Str::Type, Str::Type>;

}    // namespace IDL::Lang::Thrift

namespace IDL::Lang::Thrift
{
class Context
{
    public:
    Context(IDL::Program& p) : program(p) {}

    ONLY_MOVE_CONSTRUCT(Context);

    struct ExceptionInfo
    {
        int         line;
        int         col;
        std::string msg;
    };
    bool Debug() { return false; }
    void InitializeModelDataSources(const std::wstring_view& datasource) { program.InitializeModelDataSources(datasource); }
    void Import(Str::View const& file);
    void NotifyError(int line, int col, std::string const& msg) { errors.push_back(ExceptionInfo{line, col, msg}); }

    IDL::Program&              program;
    Str::Type                  filename;
    std::vector<ExceptionInfo> errors;
};

struct Field
{
    int               m_FieldId{};
    bool              m_IsOptional{};
    FieldType         m_FieldType{};
    Str::Type         m_Id{};
    ConstValue        m_FieldValue{};
    TypeAttributeList m_AttributeMap{};
    Field() = default;
    Field(int fieldId, Str::View const& attributes, FieldType fieldType, Str::Type& id, ConstValue& fieldValue, TypeAttributeList map) :

        m_FieldId(fieldId),
        m_IsOptional(!Str::IsEmpty(attributes) && Str::IEqual(attributes, Str::Create(L"optional"))),
        m_FieldType(fieldType),
        m_Id(std::move(id)),
        m_FieldValue(std::move(fieldValue)),
        m_AttributeMap(map)
    {
    }
};

struct Function
{
    Str::Type         m_Name;
    bool              m_isStatic;
    FieldType         m_ReturnType;
    FieldList         m_Fields;
    TypeAttributeList m_Attributes;
    Function() = default;
    Function(Str::View const& isStatic, FieldType retType, Str::Type& name, FieldList& fields, TypeAttributeList& map) :
        m_Name(std::move(name)), m_isStatic(!Str::IsEmpty(isStatic)), m_ReturnType(retType), m_Fields(std::move(fields)), m_Attributes(map)
    {
    }
};

inline std::shared_ptr<IDL::Typedef> CreateTypedef(Context& context, FieldType fieldType, Str::Type& name, TypeAttributeList map)
{
    return context.program.CreateFieldTypeObject<IDL::Typedef>(std::move(name), fieldType.value(), map);
}

std::shared_ptr<IDL::Struct> CreateStruct(Context& context, Str::Type& name, FieldList& fields, TypeAttributeList& map);

std::shared_ptr<IDL::Union> CreateUnion(Context& context, Str::Type& name, FieldList& fields, TypeAttributeList& map);

void CreateRelationship(Context& context, Str::Type& name, RelationshipComponentList& map);

#if 0

    {
        std::vector<std::unordered_map<std::string, std::string>> expandedmap;
        expandedmap.push_back({});
        for (auto it = map->begin(); it != map->end(); ++it) {
            auto oldmap = expandedmap;
            std::vector<std::unordered_map<std::string, std::string>> newmap;
            for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                for (auto it2 = oldmap.begin(); it2 != oldmap.end(); ++it2)
                {
                    (*it2)[it->first] = (*it1);
                    newmap.push_back(*it2);
                }
            }
            expandedmap = newmap;
        }

        auto def = context.program.Lookup<IDL::RelationshipDefinition>(id);
        for (auto it = expandedmap.begin(); it != expandedmap.end(); ++it)
        {
            IDL::ContainerFieldType::ContainerFieldTypeMap fieldmap;
            for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
                auto f = fieldmap[it2->first] = context.program.GetFieldTypeName(it2->second);
                assert(f != nullptr);
            }
            for (auto it2 = it->begin(); it2 != it->end(); ++it2)
            {
                auto compname = def->getComponentName(it2->first);
                auto container = context.program.Lookup<IDL::Container>(compname);
                auto fieldtype = IDL::ContainerFieldType::FindOrCreate(context.program, container, std::move(fieldmap), nullptr, nullptr);
                auto strct = context.program.Lookup<IDL::Struct>(it2->second);
            }
        }
        return nullptr;
    }
#endif
struct StrValueType : public Binding::ValueT<Binding::Type::String>, public std::enable_shared_from_this<StrValueType>
{
    StrValueType(Str::Type&& value) : _value(std::move(value)) {}
    virtual Str::Type const& GetString() override { return _value; }
    Str::Type                _value;
};

inline std::shared_ptr<IDLGenerics::IFieldType> CreateArrayType(Context& context, FieldType& field, int count, TypeAttributeList& map)
{
    auto&                                          container = context.program.Lookup<IDL::Container>(Str::Create(L"array"));
    IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
    containermap.insert(std::make_pair(Str::Create(L"type"), field.value()));
    containermap.insert(std::make_pair(Str::Create(L"size"), std::make_shared<StrValueType>(std::to_wstring(count))));

    auto existing = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
    if (existing.has_value())
    {
        return existing.value();
    }

    return context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(container, std::move(containermap), existing, std::move(map));
}

inline std::shared_ptr<IDLGenerics::IFieldType>
CreateContainerType(Context& context, Str::Type& id, FieldTypeList& fields, TypeAttributeList& map)
{
    auto& container = context.program.Lookup<IDL::Container>(std::move(id));
    assert(fields.size() == container.ComponentSize());
    IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
    size_t                                         index = 0;
    for (auto& f : fields)
    {
        containermap[Str::Copy(container.Component(index++))] = f.value();
    }

    auto existing = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
    if (existing.has_value())
    {
        return existing.value();
    }

    return context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(container, std::move(containermap), existing, std::move(map));
}

inline std::shared_ptr<IDLGenerics::IFieldType> FindFieldType(Context& context, Str::Type& name)
{
    return context.program.GetFieldTypeName(std::move(name));
}

inline TypeAttributeList CreateAttributeMapEntry(TypeAttributeList ptr, TypeAttribute& entry)
{
    if (ptr == nullptr)
    {
        ptr = std::make_shared<Binding::AttributeMap>();
    }

    ptr->AddEntry(std::move(entry.first), Binding::Expression::Create(std::move(entry.second), Str::Create(L"%"), Str::Create(L"%"), L':'));
    return ptr;
}

inline Interface CreateInterface(Context& context, Str::Type& name, Interface& /*base*/, FunctionList& functions, TypeAttributeList& map)
{
    auto iface = context.program.CreateStorageObject<IDL::Interface>(std::move(name), map);
    for (auto& f : functions)
    {
        auto argsstruct = iface->CreateStorageObject<IDL::FunctionArgs>(Str::Copy(f.m_Name), nullptr);
        for (auto& a : f.m_Fields)
        {
            argsstruct->CreateField(a.m_FieldType.value(), std::move(a.m_Id), std::move(a.m_FieldValue), std::move(a.m_AttributeMap));
        }

        iface->CreateNamedObject<IDL::Function>(std::move(f.m_Name), f.m_ReturnType.value(), std::move(argsstruct));
    }
    return iface;
}

inline Interface FindInterface(Context& /*program*/, Str::Type& /*name*/)
{
    TODO();
}

inline ConstValue CreateConstValue(int value)
{
    return std::make_shared<IDLGenerics::ConstValue>(value);
}
inline ConstValue CreateConstValue(double value)
{
    return std::make_shared<IDLGenerics::ConstValue>(value);
}
inline ConstValue CreateConstValue(Str::Type&& value)
{
    return std::make_shared<IDLGenerics::ConstValue>(std::move(value));
}
inline ConstValue FindConstValue(Str::Type&& name)
{
    if (Str::IEqual(name, Str::Create(L"null"))) return nullptr;
    throw std::logic_error("Not Implement. Named Keyword (%s). Const Values not yet supported" /*, name.c_str()*/);
}
inline ConstValue CreateConstValue(ConstValueList&& /*value*/)
{
    throw std::logic_error("Not Implement. List Const Values not yet supported");
}
inline ConstValue CreateConstValue(ConstValueDict&& /*value*/)
{
    throw std::logic_error("Not Implement. Map Const Values not yet supported");
}
inline ConstValueList ConstValueAddValue(ConstValueList&& /*vec*/, ConstValue&& /*value*/)
{
    throw std::logic_error("Not Implement. List Const Values not yet supported");
}
inline ConstValueDict ConstValueAddValue(ConstValueDict&& /*vec*/, ConstValue&& /*key*/, ConstValue&& /*value*/)
{
    throw std::logic_error("Not Implement. Map Const Values not yet supported");
}

}    // namespace IDL::Lang::Thrift
