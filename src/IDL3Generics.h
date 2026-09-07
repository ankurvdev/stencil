#pragma once
#include "Binding.h"
#include "DebugInfo.h"
#include "stencil/primitives64bit.h"

#include <algorithm>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4435)    // Object layout under /vd2 will change due to virtual base

#define WIDENSTR(x) WIDENSTR_(x)
#define WIDENSTR_(x) L##x
#define OBJECTNAME(str)                           \
    static constexpr auto BindingKeyName()        \
    { return std::wstring_view(WIDENSTR(#str)); } \
    Str::Type ObjectTypeName() override           \
    { return Str::Create(BindingKeyName()); }     \
    struct                                        \
    {                                             \
    } dummy

namespace IDLGenerics
{
using Binding::Str;

template <typename TOwner, typename TParent = TOwner>
struct AttributeT :    // public std::enable_shared_from_this<AttributeT<TOwner, TParent>>,
                       // public Binding::BindableParent<TOwner, AttributeT<TOwner, TParent>>,
                       public Binding::BindableT<AttributeT<TOwner, TParent>>
{
    OBJECTNAME(Attribute);

    AttributeT(Str::Type&& keyIn, std::shared_ptr<Binding::Expression>&& valueIn) :
        Binding::BindableT<AttributeT<TOwner, TParent>>(Str::Create(L"Key"), &AttributeT::Key, Str::Create(L"Value"), &AttributeT::Value),
        //        _owner(owner),
        key(std::move(keyIn)),
        value(std::move(valueIn))
    {}

    //    TOwner&                    Parent() const { return _owner; }
    [[nodiscard]] Str::Type                  Key() const { return Str::Copy(key); }
    [[nodiscard]] Binding::Expression const& Value() const { return *value; }

    Str::Type                            key;
    std::shared_ptr<Binding::Expression> value;
};

// Both TOwner and TParent should be Derived classes of AnnotatedObject
template <typename TOwner, typename TParent = TOwner>
struct AnnotatedObjectT : public Binding::BindableObjectArray<AnnotatedObjectT<TOwner, TParent>, AttributeT<TOwner, TParent>>,
                          public Binding::BindableDictionaryT<AnnotatedObjectT<TOwner, TParent>>
{
    using Self = AnnotatedObjectT<TOwner, TParent>;

   private:
private:
 explicit AnnotatedObjectT(std::shared_ptr<Binding::AttributeMap> const& map) : Binding::BindableDictionaryT<Self>(map)
    { AddAttributes(map); }
public:

public:


    public:
    ~AnnotatedObjectT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(AnnotatedObjectT);

    void AddAttributes(std::shared_ptr<Binding::AttributeMap> const& map)
    {
        if (map == nullptr) return;
        for (auto const& kvp : map->GetAttributes())
        {
            attributes.push_back(
                std::make_shared<AttributeT<TOwner, TParent>>(Str::Copy(kvp.first), Binding::Expression::Clone(kvp.second)));
            this->AddToArray(attributes.back());
        }
        Binding::BindableDictionaryT<Self>::AddAttributes(map);
    }

    std::vector<std::shared_ptr<AttributeT<TOwner, TParent>>> attributes;
    friend TOwner;
friend TParent;
};

template <typename TOwner, typename TObject> struct NamedIndexT
{
    struct NamedObject;

    struct Owner : public Binding::BindableObjectArray<TOwner, TObject>
    {
        Owner() = default;
        CLASS_DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> std::shared_ptr<TObject>& CreateNamedObject(TArgs&&... args) LFTBND
        {
            static_assert(std::is_base_of_v<std::enable_shared_from_this<TObject>, TObject>, "Object should be a shared_ptr");
            static_assert(std::is_base_of_v<std::enable_shared_from_this<TOwner>, TOwner>, "Owner should be a shared_ptr");
            static_assert(std::is_base_of_v<NamedObject, TObject>);
            auto ptr = std::make_shared<TObject>(std::forward<TArgs>(args)...);
            Binding::BindableObjectArray<TOwner, TObject>::AddToArray(ptr->shared_from_this());
            return (_namemap[ptr->Name()] = ptr);
            // return ptr;
        }

        std::optional<std::shared_ptr</*const*/ TObject>> TryLookup(Str::View const& key)
        {
            auto it = _namemap.find(key.data());
            return it != _namemap.end() ? it->second : std::optional<std::shared_ptr<TObject>>{};
        }

        TObject& Lookup(Str::View const& key) { return *_namemap.at(key.data()).get(); }
        //}
        /*   catch (std::exception& ex)
           {
               Str::Type names;
               for (auto& kvp : _namemap)
               {
                   names += kvp.first;
               }
               throw IDLDebug::Exception(this.GetDebugContextInfo(),
                                         "Cannot find named %s '%s'. Available Names: %s. %s",
                                         TObject::BindingKeyName(),
                                         key.c_str(),
                                         names.c_str(),
                                         ex.what());
           }*/

        auto GetRange() { return Range(_namemap.begin(), _namemap.end()); }

        private:
        std::unordered_map<Str::Type, std::shared_ptr<TObject>> _namemap;
    };

    struct NamedObject : public Binding::BindableT<TObject, NamedObject>, public Binding::BindableParent<TOwner, TObject>
    {
        static constexpr Str::View BindingKeyName() { return L"Name"; }
        using ParentType = TOwner;

        NamedObject(std::shared_ptr<TOwner> owner, Str::Type&& name) :
            Binding::BindableT<TObject, NamedObject>(Str::Create(L"Name"), &NamedObject::Name),
            _name(std::move(name)),
            _owner(std::move(std::move(std::move(std::move(std::move(std::move(std::move(owner))))))))
        {
            static_assert(std::is_base_of_v<NamedObject, TObject>, "TObject should have NamedObject as a base class");
            this->SetName(Str::Copy(_name));
        }

        CLASS_DELETE_COPY_AND_MOVE(NamedObject);

        [[nodiscard]] TOwner&   Parent() const { return *_owner; }
        [[nodiscard]] Str::Type Name() const { return Str::Copy(_name); }

        private:
        Str::Type               _name;
        std::shared_ptr<TOwner> _owner;
    };
};

struct IFieldType : public virtual Binding::BindableBase
{
    public:
    IFieldType()           = default;
    ~IFieldType() override = default;
    CLASS_DELETE_COPY_AND_MOVE(IFieldType);

    // virtual Binding::IBindable& GetBindable() const = 0;
    virtual Str::Type GetFieldName() = 0;
    Str::Type         GetFieldCategory() { return ObjectTypeName(); }

    template <typename TFieldType> static Str::Type GetFieldCategoryStatic()
    { return Str::ToLower(Str::Create(TFieldType::BindingKeyName())); }
    virtual void AddAttributes(std::shared_ptr<Binding::AttributeMap> map) = 0;

    private:
    Str::Type _lower;
};

struct FieldTypeStore
{
    [[nodiscard]] std::shared_ptr<IFieldType> GetFieldTypeName(Str::View const& name) const
    {
        ACTION_CONTEXT([&]() { return L"Searching For FieldType::" + Str::Create(name); });
        return fieldTypeMap.at(Str::Type(name));
    }

    [[nodiscard]] std::optional<std::shared_ptr</*const*/ IFieldType>> TryGetFieldTypeName(Str::View const& name) const
    {
        auto it = fieldTypeMap.find(Str::Type(name));
        if (it == fieldTypeMap.end()) return {};
        return {it->second};
    }

    void AddFieldType(Str::Type&& name, std::shared_ptr<IFieldType> ptr) { fieldTypeMap[std::move(name)] = std::move(ptr); }

    [[nodiscard]] size_t                                                               GetFieldCount() const { return fieldTypeMap.size(); }
    std::unordered_map<Str::Type, std::shared_ptr</*const*/ IFieldType>> fieldTypeMap;
};

template <typename TOwner, typename TObject> struct FieldTypeIndex
{
    struct FieldType;

    struct Owner : public NamedIndexT<TOwner, TObject>::Owner, public virtual FieldTypeStore
    {
        Owner() = default;
        CLASS_DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> auto CreateFieldTypeObject(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<FieldType, TObject>);
            auto ptr = NamedIndexT<TOwner, TObject>::Owner::CreateNamedObject(std::forward<TArgs>(args)...);
            ptr->SetFieldId(GetFieldCount());
            AddFieldType(ptr->GetFieldName(), ptr);
            SUPPRESS_WARNINGS_START
            SUPPRESS_CLANG_WARNING("-Wnrvo")
            return ptr;
            SUPPRESS_WARNINGS_END
        }
    };

    struct Mutator : public std::enable_shared_from_this<Mutator>,
                     public NamedIndexT<TOwner, Mutator>::NamedObject,
                     Binding::BindableT<Mutator>

    {
        OBJECTNAME(Mutator);
        CLASS_DELETE_COPY_AND_MOVE(Mutator);

        Mutator(std::shared_ptr<TOwner> const&             ownerIn,
                TObject*                                   fieldTypeIn LFTBND,
                Str::Type&&                                name,
                uint8_t                                    idIn,
                std::shared_ptr<Binding::Expression const> returnTypeIn,
                std::shared_ptr<Binding::Expression const> argTypeIn) :
            NamedIndexT<TOwner, Mutator>::NamedObject(ownerIn, std::move(name)),
            Binding::BindableT<Mutator>(Str::Create(L"FieldType"),
                                        &Mutator::GetFieldTypeBindable,
                                        Str::Create(L"Id"),
                                        &Mutator::GetId,
                                        Str::Create(L"ReturnType"),
                                        &Mutator::GetReturnTypeBindable,
                                        Str::Create(L"Arg"),
                                        &Mutator::GetArgTypeBindable),
            id(idIn),
            argType(std::move(std::move(argTypeIn))),
            returnType(std::move(std::move(returnTypeIn))),
            owner(std::move(ownerIn)),
            fieldType(fieldTypeIn)
        {}
        auto                              GetId() const { return Str::Create(std::to_wstring(id)); }
        [[nodiscard]] Binding::IBindable& GetFieldTypeBindable() const { return *fieldType; }

        [[nodiscard]] Binding::Expression const& GetReturnTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ReturnType: " + returnType->Stringify(); });
            return *returnType;
        }

        [[nodiscard]] Binding::Expression const& GetArgTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ArgType: " + argType->Stringify(); });
            return *argType;
        }
        uint8_t                                    id;
        std::shared_ptr<Binding::Expression const> argType;
        std::shared_ptr<Binding::Expression const> returnType;
        std::shared_ptr<TOwner>                    owner;
        TObject*                                   fieldType;
    };

    struct Accessor : public std::enable_shared_from_this<Accessor>,
                      public NamedIndexT<TOwner, Accessor>::NamedObject,
                      Binding::BindableT<Accessor>
    {
        OBJECTNAME(Accessor);

        CLASS_DELETE_COPY_AND_MOVE(Accessor);

        Accessor(std::shared_ptr<TOwner> const&             ownerIn,
                 TObject*                                   fieldTypeIn LFTBND,
                 Str::Type&&                                name,
                 uint8_t                                    idIn,
                 std::shared_ptr<Binding::Expression const> returnTypeIn,
                 std::shared_ptr<Binding::Expression const> argTypeIn) :
            NamedIndexT<TOwner, Accessor>::NamedObject(ownerIn, std::move(name)),
            Binding::BindableT<Accessor>(Str::Create(L"FieldType"),
                                         &Accessor::GetFieldTypeBindable,
                                         Str::Create(L"Id"),
                                         &Accessor::GetId,
                                         Str::Create(L"ReturnType"),
                                         &Accessor::GetReturnTypeBindable,
                                         Str::Create(L"Arg"),
                                         &Accessor::GetArgTypeBindable),
            id(idIn),
            argType(std::move(std::move(argTypeIn))),
            returnType(std::move(std::move(returnTypeIn))),
            owner(std::move(ownerIn)),
            fieldType(fieldTypeIn)

        {}
        auto GetId() const { return Str::Create(std::to_wstring(id)); }

        [[nodiscard]] Binding::IBindable& GetFieldTypeBindable() const { return *fieldType; }

        [[nodiscard]] Binding::Expression const& GetReturnTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ReturnType: " + returnType->Stringify(); });
            return *returnType;
        }

        [[nodiscard]] Binding::Expression const& GetArgTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ArgType: " + argType->Stringify(); });
            return *argType;
        }

        uint8_t                                    id;
        std::shared_ptr<Binding::Expression const> argType;
        std::shared_ptr<Binding::Expression const> returnType;
        std::shared_ptr<TOwner>                    owner;
        TObject*                                   fieldType;
    };

    struct FieldType : public IFieldType,
                       public Binding::BindableT<TObject, FieldType>,
                       public IDLGenerics::AnnotatedObjectT<TOwner, FieldType>,
                       public NamedIndexT<TOwner, Mutator>::Owner,
                       public NamedIndexT<TOwner, Accessor>::Owner,
                       public NamedIndexT<TOwner, TObject>::NamedObject

    {
        using MutatorType  = Mutator;
        using AccessorType = Accessor;
        FieldType(std::shared_ptr<TOwner> const&           ownerIn,
                  Str::Type&&                                name,
                  std::optional<std::shared_ptr<IFieldType>> basetype,
                  const std::shared_ptr<Binding::AttributeMap>&     map) :
            Binding::BindableT<TObject, FieldType>(Str::Create(L"Id"), &FieldType::GetFieldId),
            IDLGenerics::AnnotatedObjectT<TOwner, FieldType>(std::move(map)),
            NamedIndexT<TOwner, TObject>::NamedObject(ownerIn, std::move(name)),
            owner(std::move(ownerIn))
        {
            static_assert(std::is_base_of_v<FieldType, TObject>, "TObject should have FieldType as a base class");

            if (!basetype.has_value())
            {
                auto category           = GetFieldCategoryStatic<TObject>();
                auto defaultforcategory = L"default_" + Str::Value(category);
                auto defbasetype        = owner->TryGetFieldTypeName(Str::Create(defaultforcategory));
                if (!defbasetype.has_value()) { defbasetype = owner->TryGetFieldTypeName(Str::Create(L"default")); }
                basetype = std::move(defbasetype);
            }
            if (basetype.has_value()) { AddBaseObject(basetype.value()); }
            else
            {
                if (this->Name() != L"default") { throw std::logic_error("Please specify a default type"); }
            }
        }

        CLASS_DELETE_COPY_AND_MOVE(FieldType);

        template <typename... TArgs> auto CreateMutator(TArgs&&... args)
        {
            auto fieldType = static_cast<TObject*>(this);
            return NamedIndexT<TOwner, Mutator>::Owner::CreateNamedObject(owner, fieldType, std::forward<TArgs>(args)...);
        }

        template <typename... TArgs> auto CreateAccessor(TArgs&&... args)
        {
            auto fieldType = static_cast<TObject*>(this);
            return NamedIndexT<TOwner, Accessor>::Owner::CreateNamedObject(owner, fieldType, std::forward<TArgs>(args)...);
        }

        //  virtual Str::Type GetFieldName() override;
        void AddAttributes(std::shared_ptr<Binding::AttributeMap> map) override
        { IDLGenerics::AnnotatedObjectT<TOwner, FieldType>::AddAttributes(map); }

        void                    SetFieldId(size_t id) { fieldId = id; }
        [[nodiscard]] Str::Type GetFieldId() const { return Str::Create(std::to_wstring(fieldId)); }
        size_t                  fieldId{};

        std::shared_ptr<TOwner> owner;
    };
};

class ConstValue : public virtual Binding::BindableBase
{
    public:
    ConstValue()           = default;
    ~ConstValue() override = default;
    CLASS_DELETE_COPY_AND_MOVE(ConstValue);

    [[noreturn]] static Str::Type DefaultStringifiedValue()
    { throw std::logic_error("Specify a default type"); /*return Str::Create(L" "); */ /* Dont leave this empty */ }

    [[nodiscard]] virtual Str::Type Stringify() const = 0;
};

template <typename TOwner, typename TObject> struct StorageIndexT
{
    struct StorageType;

    struct Owner : public FieldTypeIndex<TOwner, TObject>::Owner
    {
        Owner() = default;

        CLASS_DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> auto CreateStorageObject(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<StorageType, TObject>);
            return this->CreateFieldTypeObject(std::forward<TArgs>(args)...);
        }
    };

    struct FieldAttribute;
    struct StructAttributeFieldValue;

    struct StructAttributeFieldValue : public std::enable_shared_from_this<StructAttributeFieldValue>,
                                       Binding::BindableT<StructAttributeFieldValue>,
                                       NamedIndexT<FieldAttribute, StructAttributeFieldValue>::NamedObject
    {
        OBJECTNAME(StructAttributeFieldValue);

        StructAttributeFieldValue(std::shared_ptr<FieldAttribute> owner, Str::Type&& name, std::shared_ptr<Binding::Expression>&& valueIn) :
            Binding::BindableT<StructAttributeFieldValue>(Str::Create(L"Value"), &StructAttributeFieldValue::Value),
            NamedIndexT<FieldAttribute, StructAttributeFieldValue>::NamedObject(std::move(owner), std::move(name)),
            value(std::move(valueIn))
        {}

        CLASS_DELETE_COPY_AND_MOVE(StructAttributeFieldValue);

        auto const& Value() const { return *value; }

        std::shared_ptr<Binding::Expression> value;
    };

    struct FieldAttribute : public std::enable_shared_from_this<FieldAttribute>,
                            public NamedIndexT<TObject, FieldAttribute>::NamedObject,
                            public NamedIndexT<FieldAttribute, StructAttributeFieldValue>::Owner

    {
        OBJECTNAME(FieldAttribute);

        CLASS_DELETE_COPY_AND_MOVE(FieldAttribute);

        FieldAttribute(std::shared_ptr<TObject> owner, Str::Type&& name) :
            NamedIndexT<TObject, FieldAttribute>::NamedObject(std::move(owner), std::move(name))
        {}

        void AddAttributeForFieldName(Str::Type&& name, std::shared_ptr<Binding::Expression>&& value)
        {
            NamedIndexT<FieldAttribute, StructAttributeFieldValue>::Owner::CreateNamedObject(
                this->shared_from_this(), std::move(name), std::move(value));
        }
    };

    struct Field : public std::enable_shared_from_this<Field>,
                   public Binding::BindableT<Field>,
                   public IDLGenerics::AnnotatedObjectT<Field>,
                   public NamedIndexT<TObject, Field>::NamedObject
    {
        OBJECTNAME(Field);
        CLASS_DELETE_COPY_AND_MOVE(Field);

        Field(std::shared_ptr<TObject>                      owner,
              Str::Type&&                                   name,
              std::shared_ptr<IFieldType>                   fieldTypeIn,
              std::shared_ptr<ConstValue>                   defaultValueIn,
              std::shared_ptr<Binding::AttributeMap> const& mapIn) :
            Binding::BindableT<Field>(Str::Create(L"FieldType"),
                                      &Field::GetFieldTypeBindable,
                                      Str::Create(L"InitialValue"),
                                      &Field::GetInitialValue,
                                      Str::Create(L"HasDefaultValue"),
                                      &Field::HasDefaultValue,
                                      Str::Create(L"IsOptional"),
                                      &Field::IsOptional),
            IDLGenerics::AnnotatedObjectT<Field>(mapIn),
            NamedIndexT<TObject, Field>::NamedObject(std::move(owner), std::move(name)),
            defaultValue(std::move(std::move(defaultValueIn))),
            fieldType(std::move(std::move(fieldTypeIn))),
            map(mapIn)
        {}

        [[nodiscard]] Str::Type GetInitialValue() const
        {
            Binding::BindingContext          context;
            std::shared_ptr<Binding::IValue> defval;

            if (defaultValue != nullptr) { defval = defaultValue->TryLookupOrNull(context, Str::Create(L"NativeType")); }
            else
            {
                defval = fieldType->TryLookupOrNull(context, Str::Create(L"DefaultValue"));
            }
            if (defval == nullptr)
            {
                fieldType->TryLookupOrNull(context, Str::Create(L"DefaultValue"));
                return ConstValue::DefaultStringifiedValue();
            }

            assert(defval->GetType() == Binding::Type::String || defval->GetType() == Binding::Type::Expr);
            if (defval->GetType() == Binding::Type::String) { return Str::Copy(defval->GetString()); }

            return defval->GetExpr().String();
        }

        [[nodiscard]] Str::Type HasDefaultValue() const { return Str::Create(defaultValue ? L"true" : L"false"); }

        [[nodiscard]] Str::Type IsOptional() const { return Str::Create(L"false"); }

        [[nodiscard]] Binding::IBindable& GetFieldTypeBindable() const { return *fieldType; }
        [[nodiscard]] bool                HasAttributes() const { return map != nullptr; }
        auto const&                       GetAttributes() const { return map->GetAttributes(); }

        std::shared_ptr<ConstValue> defaultValue;

        std::shared_ptr<IFieldType>            fieldType;
        std::shared_ptr<Binding::AttributeMap> map;
    };

    struct StorageType : public FieldTypeIndex<TOwner, TObject>::FieldType,
                         public NamedIndexT<TObject, FieldAttribute>::Owner,
                         public NamedIndexT<TObject, Field>::Owner
    {
        StorageType(std::shared_ptr<TOwner> const&            ownerIn,
                    Str::Type&&                                name,
                    std::optional<std::shared_ptr<IFieldType>> basetype,
                    const std::shared_ptr<Binding::AttributeMap>&     map) :
            FieldTypeIndex<TOwner, TObject>::FieldType(ownerIn, std::move(name), std::move(basetype), std::move(map)), owner(std::move(ownerIn))
        { static_assert(std::is_base_of_v<StorageType, TObject>, "StorageType should be a base of TObject"); }

        CLASS_DELETE_COPY_AND_MOVE(StorageType);

        void CreateField(std::shared_ptr<IDLGenerics::IFieldType> const& fieldType,
                         Str::Type&&                                     nameIn,
                         std::shared_ptr<ConstValue> const&              defaultValue,
                         std::shared_ptr<Binding::AttributeMap> const&   map)
        {
            auto field = NamedIndexT<TObject, Field>::Owner::CreateNamedObject(
                SUPER(TObject).shared_from_this(), Str::Copy(nameIn), fieldType, defaultValue, map);
            if (!field->HasAttributes()) return;
            for (auto const& it : field->GetAttributes())
            {
                auto attribute = NamedIndexT<TObject, FieldAttribute>::Owner::TryLookup(it.first);
                if (!attribute.has_value())
                {
                    attribute = NamedIndexT<TObject, FieldAttribute>::Owner::CreateNamedObject(SUPER(TObject).shared_from_this(),
                                                                                               Str::Copy(it.first));
                }
                attribute.value()->AddAttributeForFieldName(Str::Copy(nameIn), Binding::Expression::Clone(it.second));
            }
            // Make sure all attributes on fields
        }

        Str::Type GetFieldName() override { return Str::Copy(this->Name()); }

        std::shared_ptr<TOwner> owner;
    };
};

}    // namespace IDLGenerics

SUPPRESS_WARNINGS_END
