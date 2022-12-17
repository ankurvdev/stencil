#pragma once
#include "IDL3Generics.h"

#include <filesystem>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4435)    // Object layout under /vd2 will change due to virtual base

namespace IDL
{
using Str = Binding::Str;
struct Program;

struct DataSource : public std::enable_shared_from_this<DataSource>, public IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject
{
    public:
    OBJECTNAME(DataSource);
    CLASS_DELETE_COPY_AND_MOVE(DataSource);

    DataSource(std::shared_ptr<Program> owner, Str::Type&& name) :
        IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject(owner, std::move(name))
    {}
};

struct Container : public std::enable_shared_from_this<Container>,
                   public IDLGenerics::AnnotatedObjectT<Container>,
                   public IDLGenerics::NamedIndexT<Program, Container>::NamedObject
{

    public:
    struct MutatorAccessorDefinition
    {
        uint8_t                                           id;
        Binding::Str::Type                                name;
        std::shared_ptr<Binding::Expression>              returnType;
        std::vector<std::shared_ptr<Binding::Expression>> args;
    };

    OBJECTNAME(Container);
    CLASS_DELETE_COPY_AND_MOVE(Container);

    Container(std::shared_ptr<Program>                  program,
              Str::Type&&                               name,
              std::vector<Str::Type>&&                  components,
              std::optional<std::shared_ptr<Container>> base,
              std::shared_ptr<Binding::AttributeMap>    unordered_map) :
        IDLGenerics::AnnotatedObjectT<Container>(unordered_map),
        IDLGenerics::NamedIndexT<Program, Container>::NamedObject(program, std::move(name)),
        m_Components(std::move(components))
    {
        if (base.has_value()) { AddBaseObject(base.value()->shared_from_this()); }
        else
        {
            //            AddBaseObject(program.GetFieldTypeName(L"default"));
        }
    }

    std::shared_ptr<Binding::Expression> getNameExpression() const
    {
        auto expr = std::make_shared<Binding::Expression>();
        expr->AddString(Str::Copy(this->Name()));
        expr->AddString(Str::Create(L"<"));
        bool first = true;
        for (auto& cname : m_Components)
        {
            if (!first) { expr->AddString(Str::Create(L",")); }
            first = false;

            std::unique_ptr<Binding::BindingExpr> bexpr(new Binding::BindingExpr());
            bexpr->binding.push_back(Str::Copy(cname));
            expr->AddBindingExpression(std::move(bexpr));
        }
        expr->AddString(Str::Create(L">"));
        return expr;
    }

    void AddMutator(MutatorAccessorDefinition const& def) { _mutators.push_back(def); }
    void AddAccessor(MutatorAccessorDefinition const& def) { _accessors.push_back(def); }

    size_t     ComponentSize() const { return m_Components.size(); }
    Str::Type& Component(size_t index) { return m_Components[index]; }

    static std::shared_ptr<Container> FindOrCreate(std::shared_ptr<Program>               program,
                                                   Str::Type&&                            name,
                                                   std::vector<Str::Type>&                components,
                                                   Str::Type&&                            baseName,
                                                   std::shared_ptr<Binding::AttributeMap> unordered_map);

    std::vector<Str::Type>                 m_Components;
    std::vector<MutatorAccessorDefinition> _mutators;
    std::vector<MutatorAccessorDefinition> _accessors;

    friend struct ContainerFieldType;
};

struct NativeFieldType : public std::enable_shared_from_this<NativeFieldType>,
                         public IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType
{
    public:
    OBJECTNAME(NativeFieldType);
    CLASS_DELETE_COPY_AND_MOVE(NativeFieldType);

    virtual Str::Type GetFieldName() override { return Str::Copy(Name()); }

    NativeFieldType(std::shared_ptr<Program>                   program,
                    Str::Type&&                                name,
                    std::optional<std::shared_ptr<IFieldType>> basetype,
                    std::shared_ptr<Binding::AttributeMap>     unordered_map) :
        std::enable_shared_from_this<NativeFieldType>(),
        IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType(program, std::move(name), basetype, unordered_map)
    {}

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
    CLASS_DELETE_COPY_AND_MOVE(Typedef);

    virtual Str::Type GetFieldName() override { return Str::Copy(Name()); }

    std::shared_ptr<IDLGenerics::IFieldType> _basetype;

    Typedef(std::shared_ptr<Program>                 program,
            Str::Type&&                              name,
            std::shared_ptr<IDLGenerics::IFieldType> basetype,
            std::shared_ptr<Binding::AttributeMap>   unordered_map) :
        Binding::BindableT<Typedef>(Str::Create(L"ChildFieldType"), &Typedef::GetFieldTypeBindable),
        IDLGenerics::FieldTypeIndex<Program, Typedef>::FieldType(program, std::move(name), basetype, unordered_map),
        _basetype(basetype)

    {
        //        assert(basetype != {});
    }

    Binding::IBindable& GetFieldTypeBindable() const { return _basetype->GetBindable(); }
};

struct ContainerFieldType : public std::enable_shared_from_this<ContainerFieldType>,
                            public IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType
{
    public:
    typedef std::unordered_map<Str::Type, std::shared_ptr<IValue>> ContainerFieldTypeMap;

    private:
    struct BindableComponent : public Binding::IBindableComponent, public std::enable_shared_from_this<BindableComponent>
    {
        BindableComponent(ContainerFieldTypeMap&& typemap) : m_ContainerFieldTypeMap(std::move(typemap)) {}

        virtual size_t    GetKeyCount() override { return m_ContainerFieldTypeMap.size(); }
        virtual Str::Type GetKeyAt(size_t index) override
        {
            auto it = m_ContainerFieldTypeMap.begin();
            for (index++; index > 0; --index, ++it)
                ;
            return Str::Copy(it->first);
        }
        virtual Str::Type               ComponentName() override { return Str::Create(L"ContainerFieldTypeMap"); }
        virtual std::shared_ptr<IValue> TryLookupValue(Binding::BindingContext& /*context*/, Binding::Str::View const& param) override
        {
            auto it = m_ContainerFieldTypeMap.find(param.data());
            if (it == m_ContainerFieldTypeMap.end()) return {};
            return it->second;
        }

        ContainerFieldTypeMap m_ContainerFieldTypeMap;
    };

    ContainerFieldTypeMap              _typemap;
    Container&                         m_Container;
    std::shared_ptr<BindableComponent> bindable;

    public:
    OBJECTNAME(ContainerFieldType);
    CLASS_DELETE_COPY_AND_MOVE(ContainerFieldType);

    const Container& GetContainer() const;
    ContainerFieldType(std::shared_ptr<Program>                                program,
                       Container&                                              container,
                       ContainerFieldTypeMap&&                                 typemap,
                       std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                       std::shared_ptr<Binding::AttributeMap>                  unordered_map) :
        std::enable_shared_from_this<ContainerFieldType>(),
        IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType(program,
                                                                            GenerateFieldName(container, typemap),
                                                                            base,
                                                                            unordered_map),
        _typemap(typemap),
        m_Container(container),
        bindable(std::make_shared<BindableComponent>(std::move(typemap)))
    {
        Register(bindable);
        AddObjectWithTranform(container.shared_from_this(), *this, &ContainerFieldType::TransformValue);
        for (auto const& m : container._mutators)
        {
            this->CreateMutator(Str::Copy(m.name), m.id, ResolveExpression(m.returnType, _typemap), ResolveExpression(m.args[0], _typemap));
        }
        for (auto const& m : container._accessors)
        {
            this->CreateAccessor(
                Str::Copy(m.name), m.id, ResolveExpression(m.returnType, _typemap), ResolveExpression(m.args[0], _typemap));
        }
    }

    std::shared_ptr<Binding::Expression> TransformValue(Binding::BindingContext& context, Binding::Expression const& expr)
    {
        return context.EvaluateExpression(*this, expr);
    }

    virtual Str::Type GetFieldName() override { return GenerateFieldName(m_Container, bindable->m_ContainerFieldTypeMap); }

    static std::shared_ptr<Binding::Expression> ResolveExpression(std::shared_ptr<Binding::Expression> expr,
                                                                  const ContainerFieldTypeMap&         typemap)
    {
        return expr->Evaluate([&](const Binding::BindingExpr& expr1) {
            auto  rslt = std::make_shared<Binding::Expression>();
            auto& val  = typemap.at(expr1.binding[0]);
            ACTION_CONTEXT([&]() { return L"Evaluating Expression :" + rslt->Stringify() + L" On Value: " + val->Stringify(); });
            if (val->GetType() == Binding::Type::String)
            {
                assert(expr1.binding.size() == 1);
                rslt->AddString(Str::Copy(val->GetString()));
            }
            else
            {
                if (expr1.binding.size() == 1)
                {
                    Binding::BindingContext context{};
                    rslt->AddString(
                        Str::Copy(val->GetBindable().TryLookupOrNull(context, L"Name")->GetString()));    // TODO IFieldType::GetFieldName
                }
                else
                {
                    Binding::BindingContext          context{};
                    std::shared_ptr<Binding::IValue> recrval = val;
                    for (size_t i = 1; i < expr1.binding.size(); i++)
                    {
                        recrval = recrval->GetBindable().TryLookupOrNull(context, expr1.binding[i]);
                    }
                    if (val->GetType() == Binding::Type::String)
                    {
                        rslt->AddString(Str::Copy(recrval->GetString()));    // TODO IFieldType::GetFieldName
                    }
                    else { return Binding::Expression::Clone(recrval->GetExpr()); }
                }
            }
            return rslt;
        });
    }

    static Str::Type GenerateFieldName(const Container& container, const ContainerFieldTypeMap& typemap)
    {
        auto expr = container.getNameExpression();

        ACTION_CONTEXT([&]() { return L"Container Field Expression :" + expr->Stringify(); });
        while (!expr->FullyEvaluated()) { expr = ResolveExpression(expr, typemap); }
        return expr->String();
    }

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(std::shared_ptr<Program>                                program,
                                                                 Container&                                              container,
                                                                 std::vector<std::shared_ptr<IFieldType>> const&         containerFields,
                                                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                                                                 std::shared_ptr<Binding::AttributeMap>                  unordered_map);

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(std::shared_ptr<Program>                                program,
                                                                 Container&                                              container,
                                                                 ContainerFieldTypeMap&&                                 typemap,
                                                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                                                                 std::shared_ptr<Binding::AttributeMap>                  unordered_map);
};

struct AttributeDefinition : public std::enable_shared_from_this<AttributeDefinition>,
                             public IDLGenerics::NamedIndexT<Program, AttributeDefinition>::NamedObject
{
    public:
    typedef std::unordered_map<Str::Type, Str::Type> AttributeComponentMap;

    private:
    AttributeComponentMap m_ComponentMap;

    public:
    OBJECTNAME(AttributeDefinition);
    CLASS_DELETE_COPY_AND_MOVE(AttributeDefinition);

    AttributeDefinition(std::shared_ptr<Program> program,
                        Str::Type&&              name,
                        std::shared_ptr<Binding::AttributeMap> /* attributes */,
                        AttributeComponentMap&& unordered_map) :
        IDLGenerics::NamedIndexT<Program, AttributeDefinition>::NamedObject(program, std::move(name)),
        m_ComponentMap(std::move(unordered_map))
    {
        // AddAttribute(std::move(attributes));
    }

    Str::View getComponentName(Str::View const& name) const { return m_ComponentMap.at(name.data()); }
};

struct NamedConst : public std::enable_shared_from_this<NamedConst>, public IDLGenerics::NamedIndexT<Program, NamedConst>::NamedObject
{
    public:
    OBJECTNAME(NamedConst);
    DELETE_COPY_AND_MOVE(NamedConst);

    NamedConst(std::shared_ptr<Program>                              owner,
               std::shared_ptr<IDLGenerics::IFieldType>              fieldType,
               Str::Type&&                                           name,
               std::shared_ptr<IDLGenerics::ConstValue const> const& val) :
        IDLGenerics::NamedIndexT<Program, NamedConst>::NamedObject(owner, std::move(name)), _fieldType(fieldType), _value(val)
    {}
    auto value() const { return _value; }

    std::shared_ptr<IDLGenerics::IFieldType>       _fieldType;
    std::shared_ptr<IDLGenerics::ConstValue const> _value;
};

struct Enum;

struct EnumValue : public std::enable_shared_from_this<EnumValue>,
                   IDLGenerics::ConstValue,
                   public IDLGenerics::NamedIndexT<Enum, EnumValue>::NamedObject
{
    OBJECTNAME(EnumValue);
    EnumValue(std::shared_ptr<Enum> owner, Str::Type&& name, uint64_t value) :
        IDLGenerics::NamedIndexT<Enum, EnumValue>::NamedObject(owner, std::move(name)),
        _value(std::make_shared<IDLGenerics::ConstValue>(Primitives64Bit{value}))
    {}

    CLASS_DELETE_COPY_AND_MOVE(EnumValue);

    auto value() const { return _value; }

    std::shared_ptr<IDLGenerics::ConstValue const> _value;
};

struct Enum : public std::enable_shared_from_this<Enum>,
              // public Binding::BindableT<Enum>,
              public IDLGenerics::NamedIndexT<Enum, EnumValue>::Owner,
              public IDLGenerics::FieldTypeIndex<Program, Enum>::FieldType
{
    public:
    OBJECTNAME(Enum);
    DELETE_COPY_AND_MOVE(Enum);

    virtual Str::Type GetFieldName() override { return Str::Copy(Name()); }

    Enum(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        // Binding::BindableT<Typedef>(Str::Create(L"ChildFieldType"), &Typedef::GetFieldTypeBindable),
        IDLGenerics::FieldTypeIndex<Program, Enum>::FieldType(program, std::move(name), {}, unordered_map)
    {
        //        assert(basetype != {});
    }

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Enum, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...);
    }

    template <typename TObject, typename... TArgs> auto& Lookup(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Enum, TObject>::Owner::Lookup(std::forward<TArgs>(args)...);
    }

    // Binding::IBindable& GetFieldTypeBindable() const { return _basetype->GetBindable(); }
};

struct Struct;
struct Variant;
struct Interface;

struct Program : public std::enable_shared_from_this<Program>,
                 public Binding::BindableT<Program>,
                 public IDLGenerics::NamedIndexT<Program, DataSource>::Owner,
                 public IDLGenerics::NamedIndexT<Program, AttributeDefinition>::Owner,
                 public IDLGenerics::NamedIndexT<Program, Container>::Owner,
                 public IDLGenerics::NamedIndexT<Program, NamedConst>::Owner,

                 public IDLGenerics::FieldTypeIndex<Program, Typedef>::Owner,
                 public IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::Owner,
                 public IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::Owner,
                 public IDLGenerics::FieldTypeIndex<Program, Enum>::Owner,

                 public IDLGenerics::StorageIndexT<Program, Struct>::Owner,
                 public IDLGenerics::StorageIndexT<Program, Variant>::Owner,
                 public IDLGenerics::StorageIndexT<Program, Interface>::Owner
{
    Str::Type             m_DefaultNamespace;
    Str::Type             m_FileName;
    Str::Type             m_Name;
    Str::Type             m_FileDirectory;
    std::filesystem::path m_File;

    public:
    OBJECTNAME(Program);
    CLASS_DELETE_COPY_AND_MOVE(Program);

    Str::Type Name() const { return Str::Copy(m_Name); }
    Str::Type FileName() const { return Str::Copy(m_FileName); }
    Str::Type FileDirectory() const { return Str::Copy(m_FileDirectory); }
    auto      File() const { return m_File; }

    Program() :
        std::enable_shared_from_this<Program>(),
        Binding::BindableT<Program>(Str::Create(L"Name"), &Program::Name, Str::Create(L"FileName"), &Program::FileName)
    {
        Register(_imports);
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

    void Import(Program& importedProgram)
    {
        _imports->_array.push_back(importedProgram.shared_from_this());
        for (auto& [name, types] : importedProgram._fieldTypeMap) { this->AddFieldType(Str::Copy(name), types); }
    }

    struct ImportBindableComponent : public Binding::IBindableComponent,
                                     public ValueT<Binding::Type::Array>,
                                     public Binding::IValueArray,
                                     public std::enable_shared_from_this<ImportBindableComponent>

    {
        CLASS_DELETE_MOVE_AND_COPY_ASSIGNMENT(ImportBindableComponent);

        ImportBindableComponent() = default;
        virtual size_t    GetKeyCount() override { return 1; }
        virtual Str::Type GetKeyAt([[maybe_unused]] size_t index) override
        {
            assert(index == 0);
            return Str::Copy(_key);
        }
        virtual Str::Type               ComponentName() override { return Str::Copy(_key); }
        virtual std::shared_ptr<IValue> TryLookupValue(Binding::BindingContext& /*context */, Str::View const& key) override
        {
            if (key == _key) { return this->shared_from_this(); }
            return {};
        }

        IValueArray& GetArray() override { return *this; }
        size_t       GetCount() override { return _array.size(); }
        IBindable&   GetObjectAt(size_t index) override { return *_array[index]; }
        Str::Type    _key = Str::Create(L"Import");

        std::vector<std::shared_ptr<BindableBase>> _array;
    };

    std::shared_ptr<ImportBindableComponent> _imports = std::make_shared<ImportBindableComponent>();

    std::vector<std::filesystem::path> GenerateModel(bool isDryRun, std::filesystem::path const& outDir);
};

struct Struct;

struct AttributeTag : public std::enable_shared_from_this<AttributeTag>,
                      public Binding::BindableT<AttributeTag>,
                      public IDLGenerics::NamedIndexT<Struct, AttributeTag>::NamedObject
{
    OBJECTNAME(AttributeTag);
    CLASS_DELETE_COPY_AND_MOVE(AttributeTag);

    AttributeTag(std::shared_ptr<Struct> owner,
                 Str::Type&&             name,
                 std::optional<std::shared_ptr<const AttributeDefinition>> /*def*/,
                 std::shared_ptr<IDLGenerics::IFieldType> fieldType) :
        Binding::BindableT<AttributeTag>(Str::Create(L"TagType"), &AttributeTag::GetAttributeDefinitionBindable),
        IDLGenerics::NamedIndexT<Struct, AttributeTag>::NamedObject(owner, std::move(name)),
        _fieldType(fieldType)
    {}

    IBindable& GetAttributeDefinitionBindable() const { return _fieldType->GetBindable(); }

    std::shared_ptr<Binding::BindableBase>   _fieldType;
    std::unordered_map<Str::Type, Str::Type> _defmap;
    std::shared_ptr<Binding::AttributeMap>   _map;
};

struct Struct : public std::enable_shared_from_this<Struct>,
                public IDLGenerics::StorageIndexT<Program, Struct>::StorageType,
                public IDLGenerics::NamedIndexT<Struct, AttributeTag>::Owner
{
    public:
    OBJECTNAME(Struct);
    CLASS_DELETE_COPY_AND_MOVE(Struct);

    Struct(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Struct>::StorageType(program, std::move(name), {}, unordered_map)
    {}

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    {
        return IDLGenerics::NamedIndexT<Struct, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...);
    }
};

struct Variant : public std::enable_shared_from_this<Variant>, public IDLGenerics::StorageIndexT<Program, Variant>::StorageType
{
    OBJECTNAME(Variant);
    CLASS_DELETE_COPY_AND_MOVE(Variant);

    Variant(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Variant>::StorageType(program, std::move(name), {}, unordered_map)
    {}
};

struct FunctionArgs;
struct InterfaceFunction;
struct InterfaceEvent;
struct InterfaceObjectStore;

struct Interface : public std::enable_shared_from_this<Interface>,
                   public IDLGenerics::StorageIndexT<Program, Interface>::StorageType,
                   public IDLGenerics::NamedIndexT<Interface, InterfaceFunction>::Owner,
                   public IDLGenerics::NamedIndexT<Interface, InterfaceEvent>::Owner,
                   public IDLGenerics::NamedIndexT<Interface, InterfaceObjectStore>::Owner,
                   public IDLGenerics::StorageIndexT<Interface, FunctionArgs>::Owner
{
    private:
    const std::shared_ptr<Program> m_Program{};

    public:
    OBJECTNAME(Interface);
    CLASS_DELETE_COPY_AND_MOVE(Interface);

    Interface(std::shared_ptr<Program> program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Program, Interface>::StorageType(program, std::move(name), {}, unordered_map)
    {
        // HACK : We want default types from Program to propagate to the interface
        // Should we just have the search be recursive ?
        for (auto const& [fieldname, fieldtype] : program->_fieldTypeMap) { this->AddFieldType(Str::Type{fieldname}, fieldtype); }
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
    CLASS_DELETE_COPY_AND_MOVE(FunctionArgs);

    FunctionArgs(std::shared_ptr<Interface> iface, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unordered_map) :
        IDLGenerics::StorageIndexT<Interface, FunctionArgs>::StorageType(iface, std::move(name), {}, unordered_map)
    {}
};

struct InterfaceFunction : public std::enable_shared_from_this<InterfaceFunction>,
                           public Binding::BindableT<InterfaceFunction>,
                           public IDLGenerics::NamedIndexT<Interface, InterfaceFunction>::NamedObject
{

    public:
    OBJECTNAME(InterfaceFunction);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceFunction);

    auto& Args() const { return m_Args; }
    InterfaceFunction(std::shared_ptr<Interface>               iface,
                      Str::Type&&                              name,
                      std::shared_ptr<IDLGenerics::IFieldType> returnType,
                      std::shared_ptr<FunctionArgs>            args) :

        Binding::BindableT<InterfaceFunction>(Str::Create(L"ReturnType"),
                                              &InterfaceFunction::GetBindableReturnType,
                                              Str::Create(L"Args"),
                                              &InterfaceFunction::GetBindableArgs),
        IDLGenerics::NamedIndexT<Interface, InterfaceFunction>::NamedObject(iface, std::move(name)),
        m_ReturnType(returnType),
        m_Args(args)
    {}

    Binding::IBindable& GetBindableReturnType() const { return m_ReturnType->GetBindable(); }
    Binding::IBindable& GetBindableArgs() const { return *m_Args; }

    std::shared_ptr<BindableBase> m_ReturnType;
    std::shared_ptr<FunctionArgs> m_Args;
};

struct InterfaceEvent : public std::enable_shared_from_this<InterfaceEvent>,
                        public Binding::BindableT<InterfaceEvent>,
                        public IDLGenerics::NamedIndexT<Interface, InterfaceEvent>::NamedObject
{

    public:
    OBJECTNAME(InterfaceEvent);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceEvent);

    auto& Args() const { return m_Args; }
    InterfaceEvent(std::shared_ptr<Interface> iface, Str::Type&& name, std::shared_ptr<FunctionArgs> args) :

        Binding::BindableT<InterfaceEvent>(Str::Create(L"Args"), &InterfaceEvent::GetBindableArgs),
        IDLGenerics::NamedIndexT<Interface, InterfaceEvent>::NamedObject(iface, std::move(name)),
        m_Args(args)
    {}

    Binding::IBindable&           GetBindableArgs() const { return *m_Args; }
    std::shared_ptr<FunctionArgs> m_Args;
};

struct InterfaceObjectStore : public std::enable_shared_from_this<InterfaceObjectStore>,
                              public Binding::BindableT<InterfaceObjectStore>,
                              public IDLGenerics::NamedIndexT<Interface, InterfaceObjectStore>::NamedObject
{

    public:
    OBJECTNAME(InterfaceObjectStore);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceObjectStore);
    InterfaceObjectStore(std::shared_ptr<Interface> iface, std::shared_ptr<IDLGenerics::IFieldType> objectType, Str::Type&& name) :
        Binding::BindableT<InterfaceObjectStore>(Str::Create(L"ObjectType"), &InterfaceObjectStore::GetBindableObjectType),
        IDLGenerics::NamedIndexT<Interface, InterfaceObjectStore>::NamedObject(iface, std::move(name)),
        m_ObjectType(objectType)
    {}

    Binding::IBindable&           GetBindableObjectType() const { return m_ObjectType->GetBindable(); }
    std::shared_ptr<BindableBase> m_ObjectType;
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

inline std::shared_ptr<IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(std::shared_ptr<Program> program,
                                 Container&               container,
                                 ContainerFieldTypeMap&&  typemap,
                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> /*base*/,
                                 std::shared_ptr<Binding::AttributeMap> unordered_map)
{
    auto ctName    = ContainerFieldType::GenerateFieldName(container, typemap);
    auto fieldType = program->TryGetFieldTypeName(ctName);
    if (fieldType.has_value())
    {
        assert(Str::Equal(fieldType.value()->GetFieldCategory(), Str::Create(L"containerfieldtype")));
        assert(unordered_map == nullptr);
        fieldType.value()->AddAttributes(unordered_map);
        return fieldType.value();
    }

    return program->CreateNamedObject<ContainerFieldType>(
        container, std::move(typemap), program->TryGetFieldTypeName(Str::Create(L"default")), unordered_map);
}

inline std::shared_ptr<IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(std::shared_ptr<Program>                                program,
                                 Container&                                              container,
                                 std::vector<std::shared_ptr<IFieldType>> const&         containerFields,
                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                                 std::shared_ptr<Binding::AttributeMap>                  unordered_map)
{
    auto&                                     typeNames = container.m_Components;
    ContainerFieldType::ContainerFieldTypeMap fieldMap;
    size_t                                    i = 0;
    for (auto it = typeNames.begin(); it != typeNames.end(); ++it, ++i) { fieldMap[Str::Copy(*it)] = containerFields.at(i); }
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
        else { baseName = Str::Create(L"default"); }
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
#pragma warning(pop)
