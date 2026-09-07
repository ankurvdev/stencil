#pragma once
#include "CommonMacros.h"
#include "IDL3Generics.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <utility>
#include <vector>

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4435)    // Object layout under /vd2 will change due to virtual base

namespace IDL
{
using Binding::Str;
struct Program;

struct DataSource : public std::enable_shared_from_this<DataSource>, public IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject
{
    public:
    OBJECTNAME(DataSource);
    CLASS_DELETE_COPY_AND_MOVE(DataSource);

    DataSource(std::shared_ptr<Program> owner, Str::Type&& name) :
        IDLGenerics::NamedIndexT<Program, DataSource>::NamedObject(std::move(owner), std::move(name))
    {}
};

struct Container : public std::enable_shared_from_this<Container>,
                   public IDLGenerics::AnnotatedObjectT<Container>,
                   public IDLGenerics::NamedIndexT<Program, Container>::NamedObject
{

    public:
    struct MutatorAccessorDefinition
    {
        uint8_t                                           id{};
        Binding::Str::Type                                name;
        std::shared_ptr<Binding::Expression>              returnType;
        std::vector<std::shared_ptr<Binding::Expression>> args;
    };

    OBJECTNAME(Container);
    CLASS_DELETE_COPY_AND_MOVE(Container);

    Container(std::shared_ptr<Program>                  program,
              Str::Type&&                               name,
              std::vector<Str::Type>&&                  componentsIn,
              std::optional<std::shared_ptr<Container>> base,
              const std::shared_ptr<Binding::AttributeMap>&    unorderedMap) :
        IDLGenerics::AnnotatedObjectT<Container>(unorderedMap),
        IDLGenerics::NamedIndexT<Program, Container>::NamedObject(std::move(program), std::move(name)),
        components(std::move(componentsIn))
    {
        if (base.has_value()) { AddBaseObject(base.value()->shared_from_this()); }
        else
        {
            //            AddBaseObject(program.GetFieldTypeName(L"default"));
        }
    }

    std::shared_ptr<Binding::Expression> GetNameExpression() const
    {
        auto expr = std::make_shared<Binding::Expression>();
        expr->AddString(Str::Copy(this->Name()));
        expr->AddString(Str::Create(L"<"));
        bool first = true;
        for (const auto& cname : components)
        {
            if (!first) { expr->AddString(Str::Create(L",")); }
            first = false;

            std::unique_ptr<Binding::BindingExpr> bexpr = std::make_unique<Binding::BindingExpr>();
            bexpr->binding.push_back(Str::Copy(cname));
            expr->AddBindingExpression(std::move(bexpr));
        }
        expr->AddString(Str::Create(L">"));
        return expr;
    }

    void AddMutator(MutatorAccessorDefinition const& def) { mutators.push_back(def); }
    void AddAccessor(MutatorAccessorDefinition const& def) { accessors.push_back(def); }

    size_t     ComponentSize() const { return components.size(); }
    Str::Type& Component(size_t index) LFTBND { return components[index]; }

    static std::shared_ptr<Container> FindOrCreate(const std::shared_ptr<Program>&               program,
                                                   Str::Type&&                            name,
                                                   std::vector<Str::Type>&                components,
                                                   Str::Type&&                            baseName,
                                                   const std::shared_ptr<Binding::AttributeMap>& unorderedMap);

    std::vector<Str::Type>                 components;
    std::vector<MutatorAccessorDefinition> mutators;
    std::vector<MutatorAccessorDefinition> accessors;

    friend struct ContainerFieldType;
};

struct NativeFieldType : public std::enable_shared_from_this<NativeFieldType>,
                         public IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType
{
    public:
    OBJECTNAME(NativeFieldType);
    CLASS_DELETE_COPY_AND_MOVE(NativeFieldType);

    Str::Type GetFieldName() override { return Str::Copy(Name()); }

    NativeFieldType(const std::shared_ptr<Program>&                   program,
                    Str::Type&&                                name,
                    std::optional<std::shared_ptr<IFieldType>> basetype,
                    std::shared_ptr<Binding::AttributeMap>     map) :
        
        IDLGenerics::FieldTypeIndex<Program, NativeFieldType>::FieldType(program, std::move(name), std::move(basetype), std::move(map))
    {}

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(const std::shared_ptr<Program>&               program,
                                                                 Str::Type&&                            name,
                                                                 Str::Type&&                            baseName,
                                                                 const std::shared_ptr<Binding::AttributeMap>& map);
};

struct Typedef : public std::enable_shared_from_this<Typedef>,
                 public Binding::BindableT<Typedef>,
                 public IDLGenerics::FieldTypeIndex<Program, Typedef>::FieldType
{
    public:
    OBJECTNAME(Typedef);
    CLASS_DELETE_COPY_AND_MOVE(Typedef);

    Str::Type GetFieldName() override { return Str::Copy(Name()); }

    std::shared_ptr<IDLGenerics::IFieldType> basetype;

    Typedef(const std::shared_ptr<Program>&                 program,
            Str::Type&&                              name,
            std::shared_ptr<IDLGenerics::IFieldType> basetypeIn,
            std::shared_ptr<Binding::AttributeMap>   unorderedMapIn) :
        Binding::BindableT<Typedef>(Str::Create(L"ChildFieldType"), &Typedef::GetFieldTypeBindable),
        IDLGenerics::FieldTypeIndex<Program, Typedef>::FieldType(program, std::move(name), basetypeIn, std::move(unorderedMapIn)),
        basetype(basetypeIn)

    {
        //        assert(basetype != {});
    }

    Binding::IBindable& GetFieldTypeBindable() const { return basetype->GetBindable(); }
};

struct ContainerFieldType : public std::enable_shared_from_this<ContainerFieldType>,
                            public IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType
{
    public:
    using ContainerFieldTypeMap = std::unordered_map<Str::Type, std::shared_ptr<IValue>>;

    private:
    struct BindableComponent : public Binding::IBindableComponent, public std::enable_shared_from_this<BindableComponent>
    {
        explicit BindableComponent(ContainerFieldTypeMap&& typemap) : mContainerFieldTypeMap(std::move(typemap)) {}

        size_t    GetKeyCount() override { return mContainerFieldTypeMap.size(); }
        Str::Type GetKeyAt(size_t index) override
        {
            auto it = mContainerFieldTypeMap.begin();
            for (index++; index > 0; --index, ++it);
            return Str::Copy(it->first);
        }
        Str::Type               ComponentName() override { return Str::Create(L"ContainerFieldTypeMap"); }
        std::shared_ptr<IValue> TryLookupValue(Binding::BindingContext& /*context*/, Binding::Str::View const& param) override
        {
            auto it = mContainerFieldTypeMap.find(param.data());
            if (it == mContainerFieldTypeMap.end()) return {};
            return it->second;
        }

        ContainerFieldTypeMap mContainerFieldTypeMap;
    };

    ContainerFieldTypeMap              _typemap;
    Container&                         _m_Container;
    std::shared_ptr<BindableComponent> _bindable;

    public:
    OBJECTNAME(ContainerFieldType);
    CLASS_DELETE_COPY_AND_MOVE(ContainerFieldType);

    Container const& GetContainer() const;
    ContainerFieldType(const std::shared_ptr<Program>&                                program,
                       Container&                                              container LFTBND,
                       ContainerFieldTypeMap&&                                 typemap,
                       std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                       std::shared_ptr<Binding::AttributeMap>                  unorderedMap) :
        
        IDLGenerics::FieldTypeIndex<Program, ContainerFieldType>::FieldType(program,
                                                                            GenerateFieldName(container, typemap),
                                                                            std::move(base),
                                                                            std::move(unorderedMap)),
        _typemap(typemap),
        _m_Container(container),
        _bindable(std::make_shared<BindableComponent>(std::move(typemap)))
    {
        Register(_bindable);
        AddObjectWithTranform(container.shared_from_this(), *this, &ContainerFieldType::TransformValue);
        for (auto const& m : container.mutators)
        {
            this->CreateMutator(Str::Copy(m.name), m.id, ResolveExpression(m.returnType, _typemap), ResolveExpression(m.args[0], _typemap));
        }
        for (auto const& m : container.accessors)
        {
            this->CreateAccessor(
                Str::Copy(m.name), m.id, ResolveExpression(m.returnType, _typemap), ResolveExpression(m.args[0], _typemap));
        }
    }

    std::shared_ptr<Binding::Expression> TransformValue(Binding::BindingContext& context, Binding::Expression const& expr)
    { return context.EvaluateExpression(*this, expr); }

    Str::Type GetFieldName() override { return GenerateFieldName(_m_Container, _bindable->mContainerFieldTypeMap); }

    static std::shared_ptr<Binding::Expression> ResolveExpression(const std::shared_ptr<Binding::Expression>& expr,
                                                                  ContainerFieldTypeMap const&         typemap)
    {
        return expr->Evaluate([&](Binding::BindingExpr const& expr1) {
            auto  rslt = std::make_shared<Binding::Expression>();
            const auto& val  = typemap.at(expr1.binding[0]);
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
                    else
                    {
                        return Binding::Expression::Clone(recrval->GetExpr());
                    }
                }
            }
            SUPPRESS_WARNINGS_START
            SUPPRESS_CLANG_WARNING("-Wnrvo")
            return rslt;
            SUPPRESS_WARNINGS_END
        });
    }

    static Str::Type GenerateFieldName(Container const& container, ContainerFieldTypeMap const& typemap)
    {
        auto expr = container.GetNameExpression();

        ACTION_CONTEXT([&]() { return L"Container Field Expression :" + expr->Stringify(); });
        while (!expr->FullyEvaluated()) { expr = ResolveExpression(expr, typemap); }
        return expr->String();
    }

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(std::shared_ptr<Program>                                program,
                                                                 Container&                                              container,
                                                                 std::vector<std::shared_ptr<IFieldType>> const&         containerFields,
                                                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                                                                 std::shared_ptr<Binding::AttributeMap>                  unorderedMap);

    static std::shared_ptr<IDLGenerics::IFieldType> FindOrCreate(const std::shared_ptr<Program>&                                program,
                                                                 Container&                                              container,
                                                                 ContainerFieldTypeMap&&                                 typemap,
                                                                 const std::optional<std::shared_ptr<IDLGenerics::IFieldType>>& base,
                                                                 const std::shared_ptr<Binding::AttributeMap>&                  unorderedMap);
};

struct AttributeDefinition : public std::enable_shared_from_this<AttributeDefinition>,
                             public IDLGenerics::NamedIndexT<Program, AttributeDefinition>::NamedObject
{
    public:
    using AttributeComponentMap = std::unordered_map<Str::Type, Str::Type>;

    private:
    AttributeComponentMap _m_ComponentMap;

    public:
    OBJECTNAME(AttributeDefinition);
    CLASS_DELETE_COPY_AND_MOVE(AttributeDefinition);

    AttributeDefinition(std::shared_ptr<Program> program,
                        Str::Type&&              name,
                        const std::shared_ptr<Binding::AttributeMap>& /* attributes */,
                        AttributeComponentMap&& unorderedMap) :
        IDLGenerics::NamedIndexT<Program, AttributeDefinition>::NamedObject(std::move(program), std::move(name)),
        _m_ComponentMap(std::move(unorderedMap))
    {
        // AddAttribute(std::move(attributes));
    }

    Str::View GetComponentName(Str::View const& name) const LFTBND { return _m_ComponentMap.at(name.data()); }
};

struct NamedConst;
struct EnumValue;
struct Program;
struct Enum;
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
    Str::Type             mDefaultNamespace;
    Str::Type             mFileName;
    Str::Type             mName;
    Str::Type             mFileDirectory;
    std::filesystem::path mFile;

    public:
    OBJECTNAME(Program);
    CLASS_DELETE_COPY_AND_MOVE(Program);

    Str::Type Name() const { return Str::Copy(mName); }
    Str::Type FileName() const { return Str::Copy(mFileName); }
    Str::Type FileDirectory() const { return Str::Copy(mFileDirectory); }
    auto      File() const { return mFile; }

    Program() :
        
        Binding::BindableT<Program>(Str::Create(L"Name"), &Program::Name, Str::Create(L"FileName"), &Program::FileName)
    { Register(imports); }

    void SetFileName(std::filesystem::path const& file)
    {
        mFile          = file;
        mName          = Str::Create(file.stem().wstring());
        mFileName      = Str::Create(file.filename().wstring());
        mFileDirectory = Str::Create(file.parent_path().wstring());
    }
    void InitializeModelDataSources(std::wstring_view const& datasources);

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Program, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...); }

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
    { return IDLGenerics::NamedIndexT<Program, TObject>::Owner::TryLookup(std::forward<TArgs>(args)...); }
    template <typename TObject, typename... TArgs> auto& Lookup(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Program, TObject>::Owner::Lookup(std::forward<TArgs>(args)...); }

    void Import(Program& importedProgram)
    {
        imports->array.push_back(importedProgram.shared_from_this());
        for (auto& [name, types] : importedProgram.fieldTypeMap) { this->AddFieldType(Str::Copy(name), types); }
    }

    struct ImportBindableComponent : public Binding::IBindableComponent,
                                     public ValueT<Binding::Type::Array>,
                                     public Binding::IValueArray,
                                     public std::enable_shared_from_this<ImportBindableComponent>

    {
        CLASS_DELETE_COPY_AND_MOVE(ImportBindableComponent);

        ImportBindableComponent() = default;
        ~ImportBindableComponent() override = default;
        
        size_t    GetKeyCount() override { return 1; }
        Str::Type GetKeyAt([[maybe_unused]] size_t index) override
        {
            assert(index == 0);
            return Str::Copy(key);
        }
        Str::Type               ComponentName() override { return Str::Copy(key); }
        std::shared_ptr<IValue> TryLookupValue(Binding::BindingContext& /*context */, Str::View const& keyIn) override
        {
            if (key == keyIn) { return this->shared_from_this(); }
            return {};
        }

        IValueArray& GetArray() LFTBND override { return *this; }
        size_t       GetCount() override { return array.size(); }
        IBindable&   GetObjectAt(size_t index) override { return *array[index]; }
        Str::Type    key = Str::Create(L"Import");

        std::vector<std::shared_ptr<BindableBase>> array;
    };

    std::shared_ptr<ImportBindableComponent> imports = std::make_shared<ImportBindableComponent>();

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
                 const std::optional<std::shared_ptr<AttributeDefinition const>>& /*def*/,
                 const std::shared_ptr<IDLGenerics::IFieldType>& fieldTypeIn) :
        Binding::BindableT<AttributeTag>(Str::Create(L"TagType"), &AttributeTag::GetAttributeDefinitionBindable),
        IDLGenerics::NamedIndexT<Struct, AttributeTag>::NamedObject(std::move(owner), std::move(name)),
        fieldType(fieldTypeIn)
    {}

    IBindable& GetAttributeDefinitionBindable() const { return fieldType->GetBindable(); }

    std::shared_ptr<Binding::BindableBase>   fieldType;
    std::unordered_map<Str::Type, Str::Type> defmap;
    std::shared_ptr<Binding::AttributeMap>   map;
};

struct Struct : public std::enable_shared_from_this<Struct>,
                public IDLGenerics::StorageIndexT<Program, Struct>::StorageType,
                public IDLGenerics::NamedIndexT<Struct, AttributeTag>::Owner
{
    public:
    OBJECTNAME(Struct);
    CLASS_DELETE_COPY_AND_MOVE(Struct);

    Struct(const std::shared_ptr<Program>& program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unorderedMap) :
        IDLGenerics::StorageIndexT<Program, Struct>::StorageType(program, std::move(name), {}, std::move(unorderedMap))
    {}

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Struct, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...); }
};

struct Variant : public std::enable_shared_from_this<Variant>,
                 public IDLGenerics::StorageIndexT<Program, Variant>::StorageType,
                 public IDLGenerics::NamedIndexT<Variant, AttributeTag>::Owner
{
    OBJECTNAME(Variant);
    CLASS_DELETE_COPY_AND_MOVE(Variant);

    Variant(const std::shared_ptr<Program>& program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unorderedMap) :
        IDLGenerics::StorageIndexT<Program, Variant>::StorageType(program, std::move(name), {}, std::move(unorderedMap))
    {}

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Struct, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...); }
};

struct PrimitiveConstValue : public std::enable_shared_from_this<PrimitiveConstValue>,
                             Binding::BindableT<PrimitiveConstValue>,
                             IDLGenerics::ConstValue
{
    enum class ValueType
    {
        Primitives64Bit,
        String,
        List,
        Map,
        Empty,
    };

    ValueType       valueType = ValueType::Empty;
    Primitives64Bit primitive;
    Str::Type       stringValue;

    std::vector<std::shared_ptr<IDLGenerics::ConstValue>>                                                  listValue;
    std::unordered_map<std::shared_ptr<IDLGenerics::ConstValue>, std::shared_ptr<IDLGenerics::ConstValue>> mapValue;

    PrimitiveConstValue() : Binding::BindableT<PrimitiveConstValue>(Str::Create(L"NativeType"), &PrimitiveConstValue::Stringify) {}
    explicit PrimitiveConstValue(Primitives64Bit value) : PrimitiveConstValue()
    {
        valueType = ValueType::Primitives64Bit;
        primitive = value;
    }

    explicit PrimitiveConstValue(Str::Type&& value) : PrimitiveConstValue()
    {
        valueType   = ValueType::String;
        stringValue = std::move(value);
    }

    CLASS_DELETE_COPY_AND_MOVE(PrimitiveConstValue);
    OBJECTNAME(PrimitiveConstValue);
    void              _AddBaseObject() {}
    Str::Type Stringify() const override
    {
        switch (valueType)
        {
        case ValueType::Empty: return Str::Create(L"{}");
        case ValueType::String: return stringValue.empty() ? Str::Create(L"{}") : Str::Create(L"\"" + Str::Value(stringValue) + L"\"");
        case ValueType::List: throw std::invalid_argument("List Const Value Unsupported");
        case ValueType::Map: throw std::invalid_argument("Map Const Value Unsupported");
        case ValueType::Primitives64Bit:
        {
            switch (primitive.GetType().category)
            {
            case Primitives64Bit::Type::Category::Float: return Str::Convert(fmt::format("{}", primitive.Cast<double>()));
            case Primitives64Bit::Type::Category::Signed: return Str::Convert(fmt::format("{}", primitive.Cast<int64_t>()));
            case Primitives64Bit::Type::Category::Unsigned: return Str::Convert(fmt::format("{}", primitive.Cast<uint64_t>()));
            case Primitives64Bit::Type::Category::Unknown: break;
            default: break;
            }
            throw std::invalid_argument("Unknown primitive const value type");
        }
        default: throw std::invalid_argument("Unknown const value type");
        }
    }
};

struct NamedConst : public std::enable_shared_from_this<NamedConst>,
                    Binding::BindableT<NamedConst>,
                    public IDLGenerics::NamedIndexT<Program, NamedConst>::NamedObject,
                    IDLGenerics::ConstValue
{
    public:
    OBJECTNAME(NamedConst);
    CLASS_DELETE_COPY_AND_MOVE(NamedConst);

    NamedConst(const std::shared_ptr<Program>&                        owner,
               std::shared_ptr<IDLGenerics::IFieldType>        fieldTypeIn,
               Str::Type&&                                     name,
               std::shared_ptr<IDLGenerics::ConstValue> const& valueIn) :
        Binding::BindableT<NamedConst>(Str::Create(L"FieldType"),
                                       &NamedConst::GetBindableFieldType,
                                       Str::Create(L"Value"),
                                       &NamedConst::GetBindableValue),
        IDLGenerics::NamedIndexT<Program, NamedConst>::NamedObject(owner, std::move(name)),
        fieldType(std::move(std::move(fieldTypeIn))),
        value(valueIn)
    {
        auto base = owner->TryGetFieldTypeName(Str::Create(L"default_namedconst"));
        AddBaseObject(base.value());
    }

    Binding::IBindable& GetBindableFieldType() const { return fieldType->GetBindable(); }
    Binding::IBindable& GetBindableValue() const { return value->GetBindable(); }

    [[noreturn]] Str::Type Stringify() const override { TODO("ConstStringify"); }

    std::shared_ptr<IDLGenerics::IFieldType> fieldType;
    std::shared_ptr<IDLGenerics::ConstValue> value;
};

struct Enum : public std::enable_shared_from_this<Enum>,
              // public Binding::BindableT<Enum>,
              public IDLGenerics::NamedIndexT<Enum, EnumValue>::Owner,
              public IDLGenerics::FieldTypeIndex<Program, Enum>::FieldType
{
    public:
    OBJECTNAME(Enum);
    CLASS_DELETE_COPY_AND_MOVE(Enum);

    Str::Type GetFieldName() override { return Str::Copy(Name()); }

    Enum(const std::shared_ptr<Program>& program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unorderedMap) :
        // Binding::BindableT<Typedef>(Str::Create(L"ChildFieldType"), &Typedef::GetFieldTypeBindable),
        IDLGenerics::FieldTypeIndex<Program, Enum>::FieldType(program, std::move(name), {}, std::move(unorderedMap))
    {
        //        assert(basetype != {});
    }

    template <typename TObject, typename... TArgs> auto CreateNamedObject(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Enum, TObject>::Owner::CreateNamedObject(this->shared_from_this(), std::forward<TArgs>(args)...); }

    template <typename TObject, typename... TArgs> auto& Lookup(TArgs&&... args)
    { return IDLGenerics::NamedIndexT<Enum, TObject>::Owner::Lookup(std::forward<TArgs>(args)...); }

    // Binding::IBindable& GetFieldTypeBindable() const { return _basetype->GetBindable(); }
};

struct EnumValue : public std::enable_shared_from_this<EnumValue>,
                   IDLGenerics::ConstValue,
                   public IDLGenerics::NamedIndexT<Enum, EnumValue>::NamedObject
{
    OBJECTNAME(EnumValue);
    EnumValue(const std::shared_ptr<Enum>& owner, Str::Type&& name, uint64_t /* value */) :
        IDLGenerics::NamedIndexT<Enum, EnumValue>::NamedObject(owner, Str::Copy(name))
    {
        auto base = owner->Parent().TryGetFieldTypeName(Str::Create(L"default_enumvalue"));
        AddBaseObject(base.value());
    }

    [[noreturn]] Str::Type Stringify() const override { TODO("EnumStringify"); }

    CLASS_DELETE_COPY_AND_MOVE(EnumValue);
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
    std::shared_ptr<Program> const _m_Program;

    public:
    OBJECTNAME(Interface);
    CLASS_DELETE_COPY_AND_MOVE(Interface);

    Interface(const std::shared_ptr<Program>& program, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unorderedMap) :
        IDLGenerics::StorageIndexT<Program, Interface>::StorageType(program, std::move(name), {}, std::move(unorderedMap))
    {
        // HACK : We want default types from Program to propagate to the interface
        // Should we just have the search be recursive ?
        for (auto const& [fieldname, fieldtype] : program->fieldTypeMap) { this->AddFieldType(Str::Type{fieldname}, fieldtype); }
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

    FunctionArgs(const std::shared_ptr<Interface>& iface, Str::Type&& name, std::shared_ptr<Binding::AttributeMap> unorderedMap) :
        IDLGenerics::StorageIndexT<Interface, FunctionArgs>::StorageType(iface, std::move(name), {}, std::move(unorderedMap))
    {}
};

struct InterfaceFunction : public std::enable_shared_from_this<InterfaceFunction>,
                           public Binding::BindableT<InterfaceFunction>,
                           public IDLGenerics::NamedIndexT<Interface, InterfaceFunction>::NamedObject
{

    public:
    OBJECTNAME(InterfaceFunction);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceFunction);

    auto& Args() const LFTBND { return mArgs; }
    InterfaceFunction(std::shared_ptr<Interface>               iface,
                      Str::Type&&                              name,
                      const std::shared_ptr<IDLGenerics::IFieldType>& returnType,
                      std::shared_ptr<FunctionArgs>            args) :

        Binding::BindableT<InterfaceFunction>(Str::Create(L"ReturnType"),
                                              &InterfaceFunction::GetBindableReturnType,
                                              Str::Create(L"Args"),
                                              &InterfaceFunction::GetBindableArgs),
        IDLGenerics::NamedIndexT<Interface, InterfaceFunction>::NamedObject(std::move(iface), std::move(name)),
        mReturnType(returnType),
        mArgs(std::move(std::move(args)))
    {}

    Binding::IBindable& GetBindableReturnType() const { return mReturnType->GetBindable(); }
    Binding::IBindable& GetBindableArgs() const { return *mArgs; }

    std::shared_ptr<BindableBase> mReturnType;
    std::shared_ptr<FunctionArgs> mArgs;
};

struct InterfaceEvent : public std::enable_shared_from_this<InterfaceEvent>,
                        public Binding::BindableT<InterfaceEvent>,
                        public IDLGenerics::NamedIndexT<Interface, InterfaceEvent>::NamedObject
{

    public:
    OBJECTNAME(InterfaceEvent);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceEvent);

    auto& Args() const LFTBND { return mArgs; }
    InterfaceEvent(std::shared_ptr<Interface> iface, Str::Type&& name, std::shared_ptr<FunctionArgs> args) :

        Binding::BindableT<InterfaceEvent>(Str::Create(L"Args"), &InterfaceEvent::GetBindableArgs),
        IDLGenerics::NamedIndexT<Interface, InterfaceEvent>::NamedObject(std::move(iface), std::move(name)),
        mArgs(std::move(std::move(args)))
    {}

    Binding::IBindable&           GetBindableArgs() const { return *mArgs; }
    std::shared_ptr<FunctionArgs> mArgs;
};

struct InterfaceObjectStore : public std::enable_shared_from_this<InterfaceObjectStore>,
                              public Binding::BindableT<InterfaceObjectStore>,
                              public IDLGenerics::NamedIndexT<Interface, InterfaceObjectStore>::NamedObject
{

    public:
    OBJECTNAME(InterfaceObjectStore);
    CLASS_DELETE_COPY_AND_MOVE(InterfaceObjectStore);
    InterfaceObjectStore(std::shared_ptr<Interface> iface, const std::shared_ptr<IDLGenerics::IFieldType>& objectType, Str::Type&& name) :
        Binding::BindableT<InterfaceObjectStore>(Str::Create(L"ObjectType"), &InterfaceObjectStore::GetBindableObjectType),
        IDLGenerics::NamedIndexT<Interface, InterfaceObjectStore>::NamedObject(std::move(iface), std::move(name)),
        mObjectType(objectType)
    {}

    Binding::IBindable&           GetBindableObjectType() const { return mObjectType->GetBindable(); }
    std::shared_ptr<BindableBase> mObjectType;
};

inline void Program::InitializeModelDataSources(std::wstring_view const& datasourcesIn)
{
    size_t start = 0;
    while (start != std::wstring_view::npos)
    {
        auto index = datasourcesIn.find(';', start);
        auto obj   = CreateNamedObject<DataSource>(Str::Create(datasourcesIn.substr(start, index)));
        start      = index;
    }
}

inline std::shared_ptr<IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(const std::shared_ptr<Program>& program,
                                 Container&               container,
                                 ContainerFieldTypeMap&&  typemap,
                                 const std::optional<std::shared_ptr<IDLGenerics::IFieldType>>& /*base*/,
                                 const std::shared_ptr<Binding::AttributeMap>& unorderedMap)
{
    auto ctName    = ContainerFieldType::GenerateFieldName(container, typemap);
    auto fieldType = program->TryGetFieldTypeName(ctName);
    if (fieldType.has_value())
    {
        assert(Str::Equal(fieldType.value()->GetFieldCategory(), Str::Create(L"containerfieldtype")));
        assert(unorderedMap == nullptr);
        fieldType.value()->AddAttributes(unorderedMap);
        return fieldType.value();
    }

    return program->CreateNamedObject<ContainerFieldType>(
        container, std::move(typemap), program->TryGetFieldTypeName(Str::Create(L"default")), unorderedMap);
}

inline std::shared_ptr<IDLGenerics::IFieldType>
ContainerFieldType::FindOrCreate(std::shared_ptr<Program>                                program,
                                 Container&                                              container,
                                 std::vector<std::shared_ptr<IFieldType>> const&         containerFields,
                                 std::optional<std::shared_ptr<IDLGenerics::IFieldType>> base,
                                 std::shared_ptr<Binding::AttributeMap>                  unorderedMap)
{
    auto&                                     typeNames = container.components;
    ContainerFieldType::ContainerFieldTypeMap fieldMap;
    size_t                                    i = 0;
    for (auto it = typeNames.begin(); it != typeNames.end(); ++it, ++i) { fieldMap[Str::Copy(*it)] = containerFields.at(i); }
    return FindOrCreate(std::move(program), container, std::move(fieldMap), std::move(base), std::move(unorderedMap));
}

inline std::shared_ptr<IDLGenerics::IFieldType> NativeFieldType::FindOrCreate(const std::shared_ptr<Program>&               program,
                                                                              Str::Type&&                            name,
                                                                              Str::Type&&                            baseNameIn,
                                                                              const std::shared_ptr<Binding::AttributeMap>& unorderedMap)
{
    auto fieldType = program->TryGetFieldTypeName(name);
    if (fieldType.has_value())
    {
        assert(Str::Equal(fieldType.value()->ObjectTypeName(), Str::Create(NativeFieldType::BindingKeyName())));
        assert(Str::IsEmpty(baseNameIn));
        fieldType.value()->AddAttributes(unorderedMap);
        return fieldType.value();
    }

    Str::Type baseName = std::move(baseNameIn);
    if (Str::IsEmpty(baseName))
    {
        if (Str::Equal(name, Str::Create(L"default")))
        {
            return program->CreateNamedObject<NativeFieldType>(std::move(name), fieldType, unorderedMap);
        }
        
        
            baseName = Str::Create(L"default");
        
    }

    return program->CreateNamedObject<NativeFieldType>(std::move(name), program->GetFieldTypeName(std::move(baseName)), unorderedMap);
}

inline std::shared_ptr<Container> Container::FindOrCreate(const std::shared_ptr<Program>&               program,
                                                          Str::Type&&                            name,
                                                          std::vector<Str::Type>&                components,
                                                          Str::Type&&                            baseNameIn,
                                                          const std::shared_ptr<Binding::AttributeMap>& unorderedMap)
{
    auto container = program->TryLookup<Container>(name);
    if (container.has_value())
    {
        assert(Str::IsEmpty(baseNameIn));
        container.value()->AddAttributes(unorderedMap);
    }
    auto base = program->TryLookup<Container>(std::move(baseNameIn));
    return program->CreateNamedObject<Container>(std::move(name), std::move(components), base, unorderedMap);
}
}    // namespace IDL
SUPPRESS_WARNINGS_END
