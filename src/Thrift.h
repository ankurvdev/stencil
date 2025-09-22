#pragma once
#include "Generator.h"
#include "IDL2.h"

#include <algorithm>
#include <variant>

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
struct Field;
struct InterfaceFunction;
struct InterfaceEvent;
struct InterfaceObjectStore;

using TypeAttribute     = std::pair<Str::Type, Str::Type>;
using TypeAttributeList = std::shared_ptr<Binding::AttributeMap>;

using Program    = std::optional<std::shared_ptr<IDL::Program>>;
using Typedef    = std::optional<std::shared_ptr<IDL::Typedef>>;
using Struct     = std::optional<std::shared_ptr<IDL::Struct>>;
using Variant    = std::optional<std::shared_ptr<IDL::Variant>>;
using Interface  = std::optional<std::shared_ptr<IDL::Interface>>;
using Enum       = std::optional<std::shared_ptr<IDL::Enum>>;
using NamedConst = std::optional<std::shared_ptr<IDL::NamedConst>>;

// using InterfaceFunction    = std::optional<std::shared_ptr<IDL::InterfaceFunction>>;
// using InterfaceEvent       = std::optional<std::shared_ptr<IDL::InterfaceEvent>>;
// using InterfaceObjectStore = std::optional<std::shared_ptr<IDL::InterfaceObjectStore>>;
using InterfaceMember = std::variant<InterfaceFunction, InterfaceEvent, InterfaceObjectStore>;

using FieldType = std::optional<std::shared_ptr<IDLGenerics::IFieldType>>;

using Id = Str::Type;

using ConstValue = std::shared_ptr<IDLGenerics::ConstValue>;
using EnumValue  = std::pair<Str::Type, uint64_t>;

using ConstValueList         = std::vector<ConstValue>;
using ConstValueDict         = std::vector<std::pair<ConstValue, ConstValue>>;
using InterfaceMemberList    = std::vector<InterfaceMember>;
using FieldList              = std::vector<Field>;
using FieldTypeList          = std::vector<FieldType>;
using IdList                 = std::vector<Str::Type>;
using EnumValueList          = std::vector<EnumValue>;
using AttributeComponent     = std::pair<Str::Type, std::vector<Str::Type>>;
using AttributeComponentList = std::unordered_map<Str::Type, std::vector<Str::Type>>;
using ComponentList          = std::unordered_map<Str::Type, Str::Type>;

}    // namespace IDL::Lang::Thrift

namespace IDL::Lang::Thrift
{
class Context
{
    public:
    Context(IDL::Program& p, TypeDefinitions& t) : program(p), typeDefinitions(t) { typeDefinitions.LoadIntoProgram(p); }

    CLASS_ONLY_MOVE_CONSTRUCT(Context);

    struct ExceptionInfo
    {
        int         line;
        int         col;
        std::string msg;
    };
    bool Debug() { return false; }
    void InitializeModelDataSources(std::wstring_view const& datasource) { program.InitializeModelDataSources(datasource); }
    void LoadFile(std::filesystem::path const& fpath);
    void Import(Str::View const& file);
    void NotifyError(int line, int col, std::string const& msg) { errors.push_back(ExceptionInfo{line, col, msg}); }

    IDL::Program&              program;
    TypeDefinitions&           typeDefinitions;
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
    {}
};

struct InterfaceFunction
{
    Str::Type         m_Name;
    bool              m_isStatic;
    FieldType         m_ReturnType;
    FieldList         m_Fields;
    TypeAttributeList m_Attributes;
    InterfaceFunction() = default;
    InterfaceFunction(Str::View const& isStatic, FieldType retType, Str::Type& name, FieldList& fields, TypeAttributeList& map) :
        m_Name(std::move(name)), m_isStatic(!Str::IsEmpty(isStatic)), m_ReturnType(retType), m_Fields(std::move(fields)), m_Attributes(map)
    {}
};

struct InterfaceEvent
{
    Str::Type         m_Name;
    FieldList         m_Fields;
    TypeAttributeList m_Attributes;
    InterfaceEvent() = default;
    InterfaceEvent(Str::Type& name, FieldList& fields, TypeAttributeList& map) :
        m_Name(std::move(name)), m_Fields(std::move(fields)), m_Attributes(map)
    {}
};

struct InterfaceObjectStore
{
    FieldType         m_ObjectType;
    Str::Type         m_Name;
    TypeAttributeList m_Attributes;
    InterfaceObjectStore() = default;
    InterfaceObjectStore(FieldType objectType, Str::Type& name, TypeAttributeList& map) :
        m_ObjectType(objectType), m_Name(std::move(name)), m_Attributes(map)
    {}
};

inline std::shared_ptr<IDL::Typedef> CreateTypedef(Context& context, FieldType fieldType, Str::Type& name, TypeAttributeList map)
{
    return context.program.CreateFieldTypeObject<IDL::Typedef>(std::move(name), fieldType.value(), map);
}

inline std::shared_ptr<IDL::Enum> CreateEnum(Context& context, Str::Type& name, EnumValueList& enumvalues, TypeAttributeList& map)
{
    auto enumobj = context.program.CreateFieldTypeObject<IDL::Enum>(std::move(name), map);
    for (auto& ev : enumvalues) { enumobj->template CreateNamedObject<IDL::EnumValue>(std::move(ev.first), ev.second); }
    return enumobj;
}

inline EnumValue CreateEnumValue(Context& /*context*/, Str::Type& name, uint64_t value = std::numeric_limits<uint64_t>::max())
{
    return {name, value};
}

std::shared_ptr<IDL::Struct> CreateStruct(Context& context, Str::Type& name);

void AddFieldsToStruct(Context& context, std::shared_ptr<IDL::Struct>, FieldList& fields, TypeAttributeList& map);

std::shared_ptr<IDL::Variant> CreateVariant(Context& context, Str::Type& name, FieldList& fields, TypeAttributeList& map);

void CreateAttribute(Context& context, Str::Type& name, AttributeComponentList& map);

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
    if (existing.has_value()) { return existing.value(); }

    return context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(container, std::move(containermap), existing, std::move(map));
}

inline std::shared_ptr<IDLGenerics::IFieldType>
CreateContainerType(Context& context, Str::Type& id, FieldTypeList& fields, TypeAttributeList& map)
{
    auto& container = context.program.Lookup<IDL::Container>(std::move(id));
    assert(fields.size() == container.ComponentSize());
    IDL::ContainerFieldType::ContainerFieldTypeMap containermap;
    size_t                                         index = 0;
    for (auto& f : fields) { containermap[Str::Copy(container.Component(index++))] = f.value(); }

    auto existing = context.program.TryGetFieldTypeName(IDL::ContainerFieldType::GenerateFieldName(container, containermap));
    if (existing.has_value()) { return existing.value(); }

    return context.program.CreateFieldTypeObject<IDL::ContainerFieldType>(container, std::move(containermap), existing, std::move(map));
}

inline std::shared_ptr<IDLGenerics::IFieldType> FindFieldType(Context& context, Str::Type& name)
{
    return context.program.GetFieldTypeName(std::move(name));
}

inline TypeAttributeList CreateAttributeMapEntry(TypeAttributeList ptr, TypeAttribute& entry)
{
    if (ptr == nullptr) { ptr = std::make_shared<Binding::AttributeMap>(); }

    ptr->AddEntry(std::move(entry.first), Binding::Expression::Create(std::move(entry.second), Str::Create(L"%"), Str::Create(L"%"), L':'));
    return ptr;
}

inline Interface
CreateInterface(Context& context, Str::Type& name, Interface& /*base*/, InterfaceMemberList& members, TypeAttributeList& map)
{
    auto iface = context.program.CreateStorageObject<IDL::Interface>(std::move(name), map);
    for (auto& m : members)
    {
        switch (m.index())
        {
        case 0:    // InterfaceFunction
        {
            auto f          = std::get<InterfaceFunction>(m);
            auto argsstruct = iface->CreateStorageObject<IDL::FunctionArgs>(Str::Copy(f.m_Name), nullptr);
            for (auto& a : f.m_Fields)
            {
                argsstruct->CreateField(a.m_FieldType.value(), std::move(a.m_Id), std::move(a.m_FieldValue), std::move(a.m_AttributeMap));
            }

            iface->CreateNamedObject<IDL::InterfaceFunction>(std::move(f.m_Name), f.m_ReturnType.value(), std::move(argsstruct));
        }
        break;
        case 1:    // InterfaceEvent
        {
            auto e          = std::get<InterfaceEvent>(m);
            auto argsstruct = iface->CreateStorageObject<IDL::FunctionArgs>(Str::Copy(e.m_Name), nullptr);
            for (auto& a : e.m_Fields)
            {
                argsstruct->CreateField(a.m_FieldType.value(), std::move(a.m_Id), std::move(a.m_FieldValue), std::move(a.m_AttributeMap));
            }

            iface->CreateNamedObject<IDL::InterfaceEvent>(std::move(e.m_Name), std::move(argsstruct));
        }
        break;
        case 2:    // InterfaceObjectStore
        {
            auto o = std::get<InterfaceObjectStore>(m);
            iface->CreateNamedObject<IDL::InterfaceObjectStore>(o.m_ObjectType.value(), std::move(o.m_Name));
        }
        break;
        default: throw std::logic_error("invalid interface member type");
        }
    }
    return iface;
}

[[noreturn]] inline Interface FindInterface(Context& /*program*/, Str::Type& /*name*/)
{
    TODO();
}

inline ConstValue CreateConstValue(int value)
{
    return std::make_shared<IDL::PrimitiveConstValue>(Primitives64Bit{value});
}
inline ConstValue CreateConstValue(double value)
{
    return std::make_shared<IDL::PrimitiveConstValue>(Primitives64Bit{value});
}
inline ConstValue CreateConstValue(Str::Type&& value)
{
    return std::make_shared<IDL::PrimitiveConstValue>(std::move(value));
}
inline ConstValue FindConstValue(Context& context, Str::Type&& name)
{
    if (Str::IEqual(name, Str::Create(L"null"))) return nullptr;
    auto dotindex = name.find('.');
    if (dotindex != name.npos)
    {
        auto& enumtype = context.program.Lookup<IDL::Enum>(name.substr(0, dotindex));
        auto& enumval  = enumtype.Lookup<IDL::EnumValue>(name.substr(dotindex + 1));
        return enumval.shared_from_this();
    }
    else
    {
        return context.program.Lookup<IDL::NamedConst>(name).shared_from_this();
    }

    // throw std::logic_error("Not Implement. Named Keyword (%s). Const Values not yet supported" /*, name.c_str()*/);
}
[[noreturn]] inline ConstValue CreateConstValue(ConstValueList&& /*value*/)
{
    throw std::logic_error("Not Implement. List Const Values not yet supported");
}
[[noreturn]] inline ConstValue CreateConstValue(ConstValueDict&& /*value*/)
{
    throw std::logic_error("Not Implement. Map Const Values not yet supported");
}
[[noreturn]] inline ConstValueList ConstValueAddValue(ConstValueList&& /*vec*/, ConstValue&& /*value*/)
{
    throw std::logic_error("Not Implement. List Const Values not yet supported");
}

[[noreturn]] inline ConstValueDict ConstValueAddValue(ConstValueDict&& /*vec*/, ConstValue&& /*key*/, ConstValue&& /*value*/)
{
    throw std::logic_error("Not Implement. Map Const Values not yet supported");
}

inline ConstValue CreateDefaultConstValue()
{
    return std::make_shared<IDL::PrimitiveConstValue>();
}

inline auto CreateNamedConst(Context& context, FieldType& fieldType, Str::Type& name, ConstValue& val)
{
    return context.program.CreateNamedObject<IDL::NamedConst>(fieldType.value(), std::move(name), val);
}
}    // namespace IDL::Lang::Thrift
