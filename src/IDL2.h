#pragma once
#include "IDL3Generics.h"

#include <filesystem>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace IDL
{
using Str = Binding::Str;
struct Program;

struct DataSource : public std::enable_shared_from_this<DataSource>, public IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject
{
    public:
    OBJECTNAME(DataSource);

    DataSource(std::shared_ptr<Program> owner, Str::Type&& name) :
        IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject(owner, *this, std::move(name))
    {
    }
};

struct Container : public std::enable_shared_from_this<Container>,
                   public IDLGenerics::AnnotatedObjectT<Container>,
                   public IDLGenerics::NamedIndexT<Program, Container>::NamedObject
{
    std::vector<Str::Type> m_Components;

    public:
    OBJECTNAME(Container);

    Container(std::shared_ptr<Program>                        program,
              Str::Type&&                                     name,
              std::vector<Str::Type>&&                        components,
              std::optional<std::shared_ptr<const Container>> base,
              std::shared_ptr<Binding::AttributeMap>          unordered_map) :
        m_Components(std::move(components)),
        IDLGenerics::AnnotatedObjectT<Container>(unordered_map),
        IDLGenerics::NamedIndexT<Program, Container>::NamedObject(program, *this, std::move(name))
    {
        if (base.has_value())
        {
            AddBaseObject(base.value()->shared_from_this());
        }
        else
        {
            //            AddBaseObject(program.GetFieldTypeName(L"default"));
        }
    }

    Binding::Expression getNameExpression() const
    {
        Binding::Expression expr;
        expr.AddString(Str::Copy(this->Name()));
        expr.AddString(Str::Create(L"<"));
        bool first = true;
        for (auto& cname : m_Components)
        {
            if (!first)
            {
                expr.AddString(Str::Create(L","));
            }
            first = false;

            std::unique_ptr<Binding::BindingExpr> bexpr(new Binding::BindingExpr());
            bexpr->binding.push_back(Str::Copy(cname));
            expr.AddBindingExpression(std::move(bexpr));
        }
        expr.AddString(Str::Create(L">"));
        return expr;
    }

    const size_t                      ComponentSize() { return m_Components.size(); }
    Str::Type&                        Component(size_t index) { return m_Components[index]; }
    static std::shared_ptr<Container> FindOrCreate(std::shared_ptr<Program>               program,
                                                   Str::Type&&                            name,
                                                   std::vector<Str::Type>&                components,
                                                   Str::Type&&                            baseName,
                                                   std::shared_ptr<Binding::AttributeMap> unordered_map);
    friend struct ContainerFieldType;
};

struct NativeFieldType : public std::enable_shared_from_this<NativeFieldType>,
                         public IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType
{
    public:
    OBJECTNAME(NativeFieldType);
    virtual Str::Type GetFieldName() const override { return Str::Copy(Name()); }

    NativeFieldType(std::shared_ptr<Program>                         program,
                    Str::Type&&                                      name,
                    std::optional<std::shared_ptr<const IFieldType>> basetype,
                    std::shared_ptr<Binding::AttributeMap>           unordered_map) :
        std::enable_shared_from_this<NativeFieldType>(),
        IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType(program, *this, std::move(name), basetype, unordered_map)
    {
    }

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(std::shared_ptr<Program>               program,
                                                                 Str::Type&&                            name,
                                                                 Str::Type&&                            baseNameIn,
                                                                 std::shared_ptr<Binding::AttributeMap> unordered_map);
};

struct Typedef : public std::enable_shared_from_this<Typedef>,
                 public Binding::BindableT<Typedef>,
                 public IDLGenerics::FieldTypeIndex<Program, Typedef>::FieldType
{
    public:
    OBJECTNAME(Typedef);
    virtual Str::Type GetFieldName() const override { return Str::Copy(Name()); }

    std::shared_ptr<const IDLGenerics::IFieldType> _basetype;

    Typedef(std::shared_ptr<Program>                       program,
            Str::Type&&                                    name,
            std::shared_ptr<const IDLGenerics::IFieldType> basetype,
            std::shared_ptr<Binding::AttributeMap>         unordered_map) :
        _basetype(basetype),
        Binding::BindableT<Typedef>(*this, Str::Create(L"ChildFieldType"), &Typedef::GetFieldTypeBindable),
        IDLGenerics::FieldTypeIndex<Program, Typedef>::FieldType(program, *this, std::move(name), basetype, unordered_map)
    {
        //        assert(basetype != {});
    }

    const Binding::IBindable& GetFieldTypeBindable() const { return _basetype->GetBindable(); }
};

struct ContainerFieldType : public std::enable_shared_from_this<ContainerFieldType>,
                            public IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType
{
    public:
    typedef std::unordered_map<Str::Type, std::shared_ptr<const IValue>> ContainerFieldTypeMap;

    private:
    struct BindableComponent : public Binding::IBindableComponent, public std::enable_shared_from_this<BindableComponent>
    {
        BindableComponent(ContainerFieldTypeMap&& typemap) : m_ContainerFieldTypeMap(std::move(typemap)) {}

        virtual size_t    GetKeyCount() const override { return m_ContainerFieldTypeMap.size(); }
        virtual Str::Type GetKeyAt(size_t index) const override
        {
            auto it = m_ContainerFieldTypeMap.begin();
            for (index++; index > 0; --index, ++it)
                ;
            return Str::Copy(it->first);
        }
        virtual Str::Type                     ComponentName() const override { return Str::Create(L"ContainerFieldTypeMap"); }
        virtual std::shared_ptr<const IValue> TryLookupValue(Binding::BindingContext& /*context*/,
                                                             Binding::Str::View const& param) const override
        {
            auto it = m_ContainerFieldTypeMap.find(param.data());
            if (it == m_ContainerFieldTypeMap.end()) return {};
            return it->second;
        }

        ContainerFieldTypeMap m_ContainerFieldTypeMap;
    };

    std::shared_ptr<BindableComponent> bindable;

    const Container& m_Container;

    public:
    OBJECTNAME(ContainerFieldType);

    const Container& GetContainer() const;
    ContainerFieldType(std::shared_ptr<Program>                                      program,
                       const Container&                                              container,
                       ContainerFieldTypeMap&&                                       typemap,
                       std::optional<std::shared_ptr<const IDLGenerics::IFieldType>> base,
                       std::shared_ptr<Binding::AttributeMap>                        unordered_map) :
        IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType(program,
                                                                            *this,
                                                                            GenerateFieldName(container, typemap),
                                                                            base,
                                                                            unordered_map),
        bindable(std::make_shared<BindableComponent>(std::move(typemap))),
        m_Container(container)
    {
        Register(bindable);
        AddObjectWithTranform(container.shared_from_this(), *this, &ContainerFieldType::TransformValue);
    }

    Binding::Expression TransformValue(Binding::BindingContext& context, Binding::Expression const& expr) const
    {
        return context.EvaluateExpression(*this, expr);
    }

    virtual Str::Type GetFieldName() const override { return GenerateFieldName(m_Container, bindable->m_ContainerFieldTypeMap); }

    static Str::Type GenerateFieldName(const Container& container, const ContainerFieldTypeMap& typemap)
    {
        auto expr = container.getNameExpression();
        while (!expr.FullyEvaluated())
        {
            expr = expr.Evaluate([&](const Binding::BindingExpr& expr) {
                assert(expr.binding.size() == 1);
                Binding::Expression rslt;
                auto&               val = typemap.at(expr.binding[0]);
                if (val->GetType() == Binding::Type::String)
                {
                    rslt.AddString(Str::Copy(val->GetString()));
                }
                else
                {
                    Binding::BindingContext context{};
                    rslt.AddString(
                        Str::Copy(val->GetBindable().TryLookupOrNull(context, L"Name")->GetString()));    // TODO IFieldType::GetFieldName
                }
                return rslt;
            });
        }
        return expr.String();
    }

    static std::shared_ptr<const IDLGenerics::IFieldType>
    FindOrCreate(std::shared_ptr<Program>                                      program,
                 const Container&                                              container,
                 std::vector<std::shared_ptr<const IFieldType>> const&         containerFields,
                 std::optional<std::shared_ptr<const IDLGenerics::IFieldType>> base,
                 std::shared_ptr<Binding::AttributeMap>                        unordered_map);

    static std::shared_ptr<const IDLGenerics::IFieldType> FindOrCreate(std::shared_ptr<Program> program,
                                                                       const Container&         container,
                                                                       ContainerFieldTypeMap&&  typemap,
                                                                       std::optional<std::shared_ptr<const IDLGenerics::IFieldType>> base,
                                                                       std::shared_ptr<Binding::AttributeMap> unordered_map);
};

struct RelationshipDefinition : public std::enable_shared_from_this<RelationshipDefinition>,
                                public IDLGenerics::NamedIndexT<Program, RelationshipDefinition>::NamedObject
{
    public:
    typedef std::unordered_map<Str::Type, Str::Type> RelationshipComponentMap;

    private:
    RelationshipComponentMap m_ComponentMap;

    public:
    OBJECTNAME(RelationshipDefinition);
    RelationshipDefinition(std::shared_ptr<Program>               program,
                           Str::Type&&                            name,
                           std::shared_ptr<Binding::AttributeMap> attributes,
                           RelationshipComponentMap&&             unordered_map) :
        IDLGenerics::NamedIndexT<Program, RelationshipDefinition>::NamedObject(program, *this, std::move(name)),
        m_ComponentMap(std::move(unordered_map))
    {
        // AddAttribute(std::move(attributes));
    }

    Str::View getComponentName(Str::View const& name) const { return m_ComponentMap.at(name.data()); }
};

struct Struct;
struct Union;
struct Interface;

struct Program : public std::enable_shared_from_this<Program>,
                 public Binding::BindableT<Program>,
                 public IDLGenerics::NamedIndexT<Program, DataSource>::Owner,
                 public IDLGenerics::NamedIndexT<Program, RelationshipDefinition>::Owner,
                 public IDLGenerics::NamedIndexT<Program, Container>::Owner,

                 public IDLGenerics::FieldTypeIndex<Program, Typedef>::Owner,
                 public IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::Owner,
                 public IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::Owner,

                 public IDLGenerics::StorageIndexT<Program, Struct>::Owner,
                 public IDLGenerics::StorageIndexT<Program, Union>::Owner,
                 public IDLGenerics::StorageIndexT<Program, Interface>::Owner
{
    Str::Type             m_DefaultNamespace;
    Str::Type             m_FileName;
    Str::Type             m_Name;
    Str::Type             m_FileDirectory;
    std::filesystem::path m_File;

    public:
    OBJECTNAME(Program);

    Str::Type Name() const { return Str::Copy(m_Name); }
    Str::Type FileName() const { return Str::Copy(m_FileName); }
    Str::Type FileDirectory() const { return Str::Copy(m_FileDirectory); }
    auto      File() const { return m_File; }

    Program() :
        std::enable_shared_from_this<Program>(),
        Binding::BindableT<Program>(*this, Str::Create(L"Name"), &Program::Name, Str::Create(L"FileName"), &Program::FileName),
        IDLGenerics::NamedIndexT<Program, DataSource>::Owner(*this),
        IDLGenerics::NamedIndexT<Program, Container>::Owner(*this),
        IDLGenerics::NamedIndexT<Program, RelationshipDefinition>::Owner(*this),

        IDLGenerics::FieldTypeIndex<Program, Typedef>::Owner(*this),
        IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::Owner(*this),
        IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::Owner(*this),

        IDLGenerics::StorageIndexT<Program, Struct>::Owner(*this),
        IDLGenerics::StorageIndexT<Program, Union>::Owner(*this),
        IDLGenerics::StorageIndexT<Program, Interface>::Owner(*this)
    {
    }

    void SetFileName(std::filesystem::path const& file)
    {
        m_File          = file;
        m_Name          = Str::Create(file.stem().wstring());
        m_FileName      = Str::Create(file.filename().wstring());
        m_FileDirectory = Str::Create(file.parent_path().wstring());
    }
    void InitializeModelDataSources(const std::wstring_view& datasources);

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Program, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...);
    }

    template <typename TObject, typename... TArgs> auto CreateStorageObject(TArgs&&... args)
    {
        return IDLGenerics::StorageIndexT<Program, TObject>::Owner::CreateStorageObject(this->shared_from_this(),
                                                                                        std::forward<TArgs>(args)...);
    }

    template <typename TObject, typename... TArgs> auto CreateFieldTypeObject(TArgs&&... args)
    {
        return IDLGenerics::FieldTypeIndex<Program, TObject>::Owner::CreateFieldTypeObject(this->shared_from_this(),
                                                                                           std::forward<TArgs>(args)...);
    }

    template <typename TObject> auto _GetObjects() { return IDLGenerics::NamedIndexT<Program, TObject>::Owner::GetRange(); }
    template <typename TObject, typename... TArgs> auto TryLookup(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Program, TObject>::Owner::TryLookup(std::forward<TArgs>(args)...);
    }
    template <typename TObject, typename... TArgs> auto& Lookup(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Program, TObject>::Owner::Lookup(std::forward<TArgs>(args)...);
    }

    std::vector<std::filesystem::path> GenerateModel(bool isDryRun, std::filesystem::path const& outDir);
};

struct Struct;

struct RelationshipTag : public std::enable_shared_from_this<RelationshipTag>,
                         public Binding::BindableT<RelationshipTag>,
                         public IDLGenerics::NamedIndexT<Struct, RelationshipTag>::NamedObject
{
    OBJECTNAME(RelationshipTag);

    RelationshipTag(std::shared_ptr<Struct> owner,
                    Str::Type&&             name,
                    std::optional<std::shared_ptr<const RelationshipDefinition>> /*def*/,
                    std::shared_ptr<const IDLGenerics::IFieldType> fieldType) :
        Binding::BindableT<RelationshipTag>(*this, Str::Create(L"TagType"), &RelationshipTag::GetRelationshipDefinitionBindable),
        IDLGenerics::NamedIndexT<Struct, RelationshipTag>::NamedObject(owner, *this, std::move(name)),
        //  _definition(def),
        _fieldType(fieldType)
    {
    }

    const IBindable& GetRelationshipDefinitionBindable() const { return _fieldType->GetBindable(); }

    // bool       HasAttributes() const { return _map != {}; }
    // const auto GetAttributes() const { return _map->GetAttributes(); }

    std::shared_ptr<const Binding::BindableBase> _fieldType;
    //  const IDLGenerics::IFieldType& _fieldType;
    //&                                _definition;
    std::unordered_map<Str::Type, Str::Type> _defmap;
    std::shared_ptr<Binding::AttributeMap>   _map;
};

struct Struct : public std::enable_shared_from_this<Struct>,
                public IDLGenerics::StorageIndexT<Program, Struct>::StorageType,
                public IDLGenerics::NamedIndexT<Struct, RelationshipTag>::Owner
{
    public:
    OBJECTNAME(Struct);
    Struct(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Struct>::StorageType(program, *this, std::move(name), {}, unordered_map),
        IDLGenerics::NamedIndexT<Struct, RelationshipTag>::Owner(*this)
    {
    }

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Struct, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...);
    }
};

struct Union : public std::enable_shared_from_this<Union>, public IDLGenerics::StorageIndexT<Program, Union>::StorageType
{
    OBJECTNAME(Union);
    Union(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Union>::StorageType(program, *this, std::move(name), {}, unordered_map)
    {
    }
};

struct FunctionArgs;
struct Function;

struct Interface : public std::enable_shared_from_this<Interface>,
                   public IDLGenerics::StorageIndexT<Program, Interface>::StorageType,
                   public IDLGenerics::NamedIndexT<Interface, Function>::Owner,
                   public IDLGenerics::StorageIndexT<Interface, FunctionArgs>::Owner
{
    private:
    const std::shared_ptr<Program> m_Program{};

    public:
    OBJECTNAME(Interface);
    Interface(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Interface>::StorageType(program, *this, std::move(name), {}, unordered_map),
        IDLGenerics::NamedIndexT<Interface, Function>::Owner(*this),
        IDLGenerics::StorageIndexT<Interface, FunctionArgs>::Owner(*this)
    {
    }

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Interface, TObject>::Owner::CreateNamedObject(this->shared_from_this(),
                                                                                      std::forward<TArgs>(args)...);
    }

    template <typename TObject, typename... TArgs> auto CreateStorageObject(TArgs&&... args)
    {
        return IDLGenerics::StorageIndexT<Interface, TObject>::Owner::CreateStorageObject(this->shared_from_this(),
                                                                                          std::forward<TArgs>(args)...);
    }

    template <typename TObject, typename... TArgs> auto CreateFieldTypeObject(TArgs&&... args)
    {
        return IDLGenerics::FieldTypeIndex<Interface, TObject>::Owner::CreateFieldTypeObject(this->shared_from_this(),
                                                                                             std::forward<TArgs>(args)...);
    }
};

struct FunctionArgs : public std::enable_shared_from_this<FunctionArgs>,
                      public IDLGenerics::StorageIndexT<Interface, FunctionArgs>::StorageType
{
    OBJECTNAME(FunctionArgs);

    FunctionArgs(std::shared_ptr<Interface> iface, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Interface, FunctionArgs>::StorageType(iface, *this, std::move(name), {}, unordered_map)
    {
    }
};

struct Function : public std::enable_shared_from_this<Function>,
                  public Binding::BindableT<Function>,
                  public IDLGenerics::NamedIndexT<Interface, Function>::NamedObject
{
    std::shared_ptr<const BindableBase> m_ReturnType;
    std::shared_ptr<const FunctionArgs> m_Args;

    public:
    OBJECTNAME(Function);

    auto& Args() const { return m_Args; }
    Function(std::shared_ptr<Interface>                     iface,
             Str::Type&&                                    name,
             std::shared_ptr<const IDLGenerics::IFieldType> returnType,
             std::shared_ptr<const FunctionArgs>            args) :
        m_ReturnType(returnType),
        m_Args(args),
        Binding::BindableT<Function>(*this,
                                     Str::Create(L"ReturnType"),
                                     &Function::GetBindableReturnType,
                                     Str::Create(L"Args"),
                                     &Function::GetBindableArgs),
        IDLGenerics::NamedIndexT<Interface, Function>::NamedObject(iface, *this, std::move(name))
    {
    }

    const Binding::IBindable& GetBindableReturnType() const { return m_ReturnType->GetBindable(); }
    const Binding::IBindable& GetBindableArgs() const { return *m_Args; }
};

inline void Program::InitializeModelDataSources(const std::wstring_view& datasourcesIn)
{
    size_t start = 0;
    while (start != datasourcesIn.npos)
    {
        auto index = datasourcesIn.find(';', start);
        auto obj   = CreateNamedObject<DataSource>(Str::Create(datasourcesIn.substr(start, index)));
        start      = index;
    }
}

inline std::shared_ptr<const IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(std::shared_ptr<Program>                                      program,
                                 const Container&                                              container,
                                 ContainerFieldTypeMap&&                                       typemap,
                                 std::optional<std::shared_ptr<const IDLGenerics::IFieldType>> base,
                                 std::shared_ptr<Binding::AttributeMap>                        unordered_map)
{
    auto ctName    = ContainerFieldType::GenerateFieldName(container, typemap);
    auto fieldType = program->TryGetFieldTypeName(ctName);
    if (fieldType.has_value())
    {
        assert(Str::Equal(fieldType.value()->GetFieldCategory(), Str::Create(L"containerfieldtype")));
        assert(!base.has_value());
        assert(unordered_map == nullptr);
        fieldType.value()->AddAttributes(unordered_map);
        return fieldType.value();
    }

    return program->CreateNamedObject<ContainerFieldType>(
        container, std::move(typemap), program->TryGetFieldTypeName(Str::Create(L"default")), unordered_map);
}

inline std::shared_ptr<const IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(std::shared_ptr<Program>                                      program,
                                 const Container&                                              container,
                                 std::vector<std::shared_ptr<const IFieldType>> const&         containerFields,
                                 std::optional<std::shared_ptr<const IDLGenerics::IFieldType>> base,
                                 std::shared_ptr<Binding::AttributeMap>                        unordered_map)
{
    auto&                                     typeNames = container.m_Components;
    ContainerFieldType::ContainerFieldTypeMap fieldMap;
    int                                       i = 0;
    for (auto it = typeNames.begin(); it != typeNames.end(); ++it, ++i)
    {
        fieldMap[Str::Copy(*it)] = containerFields.at(i);
    }
    return FindOrCreate(program, container, std::move(fieldMap), base, unordered_map);
}

inline std::shared_ptr<IDLGenerics::IFieldType> NativeFieldType::FindOrCreate(std::shared_ptr<Program>               program,
                                                                              Str::Type&&                            name,
                                                                              Str::Type&&                            baseNameIn,
                                                                              std::shared_ptr<Binding::AttributeMap> unordered_map)
{
    auto fieldType = program->TryGetFieldTypeName(name);
    if (fieldType.has_value())
    {
        assert(Str::Equal(fieldType.value()->ObjectTypeName(), Str::Create(NativeFieldType::BindingKeyName())));
        assert(Str::IsEmpty(baseNameIn));
        fieldType.value()->AddAttributes(unordered_map);
        return fieldType.value();
    }

    Str::Type baseName = std::move(baseNameIn);
    if (Str::IsEmpty(baseName))
    {
        if (Str::Equal(name, Str::Create(L"default")))
        {
            return program->CreateNamedObject<NativeFieldType>(std::move(name), fieldType, unordered_map);
        }
        else
        {
            baseName = Str::Create(L"default");
        }
    }

    return program->CreateNamedObject<NativeFieldType>(std::move(name), program->GetFieldTypeName(std::move(baseName)), unordered_map);
}

inline std::shared_ptr<Container> Container::FindOrCreate(std::shared_ptr<Program>               program,
                                                          Str::Type&&                            name,
                                                          std::vector<Str::Type>&                components,
                                                          Str::Type&&                            baseNameIn,
                                                          std::shared_ptr<Binding::AttributeMap> unordered_map)
{
    auto container = program->TryLookup<Container>(name);
    if (container.has_value())
    {
        assert(Str::IsEmpty(baseNameIn));
        container.value()->AddAttributes(unordered_map);
    }
    auto base = program->TryLookup<Container>(std::move(baseNameIn));
    return program->CreateNamedObject<Container>(std::move(name), std::move(components), base, unordered_map);
}
}    // namespace IDL
