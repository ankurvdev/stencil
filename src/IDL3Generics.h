#pragma once
#include "Binding.h"
#include "DebugInfo.h"

#include <algorithm>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4435)    // Object layout under /vd2 will change due to virtual base

#define WIDENSTR(x) WIDENSTR_(x)
#define WIDENSTR_(x) L##x
#define OBJECTNAME(str)                                                                       \
    static constexpr auto BindingKeyName() { return std::wstring_view(WIDENSTR(#str)); }      \
    Str::Type             ObjectTypeName() override { return Str::Create(BindingKeyName()); } \
    struct                                                                                    \
    {                                                                                         \
    } dummy

namespace IDLGenerics
{
using Str = Binding::Str;

template <typename TOwner, typename TParent = TOwner>
struct AttributeT :    // public std::enable_shared_from_this<AttributeT<TOwner, TParent>>,
                       // public Binding::BindableParent<TOwner, AttributeT<TOwner, TParent>>,
                       public Binding::BindableT<AttributeT<TOwner, TParent>>
{
    OBJECTNAME(Attribute);

    AttributeT(Str::Type&& keyIn, std::shared_ptr<Binding::Expression>&& valueIn) :
        Binding::BindableT<AttributeT<TOwner, TParent>>(Str::Create(L"Key"), &AttributeT::Key, Str::Create(L"Value"), &AttributeT::Value),
        //        _owner(owner),
        _key(std::move(keyIn)),
        _value(std::move(valueIn))
    {
    }

    //    TOwner&                    Parent() const { return _owner; }
    Str::Type                  Key() const { return Str::Copy(_key); }
    Binding::Expression const& Value() const { return *_value; }

    Str::Type                            _key;
    std::shared_ptr<Binding::Expression> _value;
};

// Both TOwner and TParent should be Derived classes of AnnotatedObject
template <typename TOwner, typename TParent = TOwner>
struct AnnotatedObjectT : public Binding::BindableObjectArray<AnnotatedObjectT<TOwner, TParent>, AttributeT<TOwner, TParent>>,
                          public Binding::BindableDictionaryT<AnnotatedObjectT<TOwner, TParent>>
{
    using Self = AnnotatedObjectT<TOwner, TParent>;
    AnnotatedObjectT(std::shared_ptr<Binding::AttributeMap> map) : Binding::BindableDictionaryT<Self>(map) { AddAttributes(map); }
    ~AnnotatedObjectT() override = default;
    DELETE_COPY_AND_MOVE(AnnotatedObjectT);

    void AddAttributes(std::shared_ptr<Binding::AttributeMap> map)
    {
        if (map == nullptr) return;
        for (auto& kvp : map->GetAttributes())
        {
            attributes.push_back(
                std::make_shared<AttributeT<TOwner, TParent>>(Str::Copy(kvp.first), Binding::Expression::Clone(kvp.second)));
            this->AddToArray(attributes.back());
        }
        Binding::BindableDictionaryT<Self>::AddAttributes(map);
    }

    std::vector<std::shared_ptr<AttributeT<TOwner, TParent>>> attributes;
};

template <typename TOwner, typename TObject> struct NamedIndexT
{
    struct Owner : public Binding::BindableObjectArray<TOwner, TObject>
    {
        Owner() = default;
        DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> std::shared_ptr<TObject>& CreateNamedObject(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<std::enable_shared_from_this<TObject>, TObject>, "Object should be a shared_ptr");
            static_assert(std::is_base_of_v<std::enable_shared_from_this<TOwner>, TOwner>, "Owner should be a shared_ptr");
            static_assert(std::is_base_of<NamedIndexT<TOwner, TObject>::NamedObject, TObject>::value);
            auto ptr = std::make_shared<TObject>(std::forward<TArgs>(args)...);
            Binding::BindableObjectArray<TOwner, TObject>::AddToArray(ptr->shared_from_this());
            return (_namemap[ptr->Name()] = ptr);
            // return ptr;
        }

        std::optional<std::shared_ptr</*const*/ TObject>> TryLookup(Str::View const& key)
        {
            auto it = _namemap.find(key.data());
            return it != _namemap.end() ? (it->second) : std::optional<std::shared_ptr<TObject>>{};
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
            Binding::BindableT<TObject, NamedObject>(Str::Create(L"Name"), &NamedObject::Name), _name(std::move(name)), _owner(owner)
        {
            static_assert(std::is_base_of_v<NamedObject, TObject>, "TObject should have NamedObject as a base class");
            this->SetName(Str::Copy(_name));
        }

        DELETE_COPY_AND_MOVE(NamedObject);

        TOwner&   Parent() const { return *_owner; }
        Str::Type Name() const { return Str::Copy(_name); }

        private:
        Str::Type               _name;
        std::shared_ptr<TOwner> _owner;
    };
};

struct IFieldType : public virtual Binding::BindableBase
{
    public:
    IFieldType()  = default;
    ~IFieldType() override = default;
    DELETE_COPY_AND_MOVE(IFieldType);

    // virtual Binding::IBindable& GetBindable() const = 0;
    virtual Str::Type GetFieldName() = 0;
    Str::Type         GetFieldCategory() { return ObjectTypeName(); }

    template <typename TFieldType> static Str::Type GetFieldCategoryStatic()
    {
        return Str::ToLower(Str::Create(TFieldType::BindingKeyName()));
    }
    virtual void AddAttributes(std::shared_ptr<Binding::AttributeMap> map) = 0;

    private:
    Str::Type _lower;
};

struct FieldTypeStore
{
    std::shared_ptr<IFieldType> GetFieldTypeName(Str::View const& name) const
    {
        ACTION_CONTEXT([&]() { return L"Searching For FieldType::" + Str::Create(name); });
        return _fieldTypeMap.at(Str::Type(name));
    }
    std::optional<std::shared_ptr</*const*/ IFieldType>> TryGetFieldTypeName(Str::View const& name) const
    {
        auto it = _fieldTypeMap.find(Str::Type(name));
        if (it == _fieldTypeMap.end()) return {};
        return {it->second};
    }

    void AddFieldType(Str::Type&& name, std::shared_ptr<IFieldType> ptr) { _fieldTypeMap[std::move(name)] = ptr; }

    size_t                                                               GetFieldCount() { return _fieldTypeMap.size(); }
    std::unordered_map<Str::Type, std::shared_ptr</*const*/ IFieldType>> _fieldTypeMap;
};

template <typename TOwner, typename TObject> struct FieldTypeIndex
{
    struct Owner : public NamedIndexT<TOwner, TObject>::Owner, public virtual FieldTypeStore
    {
        Owner() = default;
        DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> auto CreateFieldTypeObject(TArgs&&... args)
        {
            static_assert(std::is_base_of<FieldTypeIndex<TOwner, TObject>::FieldType, TObject>::value);
            auto ptr = NamedIndexT<TOwner, TObject>::Owner::CreateNamedObject(std::forward<TArgs>(args)...);
            ptr->SetFieldId(GetFieldCount());
            AddFieldType(ptr->GetFieldName(), ptr);
            return ptr;
        }
    };

    struct FieldType;

    struct Mutator : public std::enable_shared_from_this<Mutator>,
                     public NamedIndexT<TOwner, Mutator>::NamedObject,
                     Binding::BindableT<Mutator>

    {
        OBJECTNAME(Mutator);
        DELETE_COPY_AND_MOVE(Mutator);

        Mutator(std::shared_ptr<TOwner>                    owner,
                TObject*                                   fieldType,
                Str::Type&&                                name,
                uint8_t                                    id,
                std::shared_ptr<const Binding::Expression> returnType,
                std::shared_ptr<const Binding::Expression> argType) :
            NamedIndexT<TOwner, Mutator>::NamedObject(owner, std::move(name)),
            Binding::BindableT<Mutator>(Str::Create(L"FieldType"),
                                        &Mutator::GetFieldTypeBindable,
                                        Str::Create(L"Id"),
                                        &Mutator::GetId,
                                        Str::Create(L"ReturnType"),
                                        &Mutator::GetReturnTypeBindable,
                                        Str::Create(L"Arg"),
                                        &Mutator::GetArgTypeBindable),
            _id(id),
            _argType(argType),
            _returnType(returnType),
            _owner(owner),
            _fieldType(fieldType)
        {
        }
        auto                GetId() const { return Str::Create(std::to_wstring(_id)); }
        Binding::IBindable& GetFieldTypeBindable() const { return *_fieldType; }

        Binding::IBindable& GetReturnTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ReturnType: " + _returnType->Stringify(); });
            return *_owner->GetFieldTypeName(Binding::BindingContext::EvaluateExpression(*_returnType)->String());
        }

        Binding::IBindable& GetArgTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ArgType: " + _argType->Stringify(); });
            return *_owner->GetFieldTypeName(Binding::BindingContext::EvaluateExpression(*_argType)->String());
        }
        uint8_t                                    _id;
        std::shared_ptr<const Binding::Expression> _argType;
        std::shared_ptr<const Binding::Expression> _returnType;
        std::shared_ptr<TOwner>                    _owner;
        TObject*                                   _fieldType;
    };

    struct Accessor : public std::enable_shared_from_this<Accessor>,
                      public NamedIndexT<TOwner, Accessor>::NamedObject,
                      Binding::BindableT<Accessor>
    {
        OBJECTNAME(Accessor);

        DELETE_COPY_AND_MOVE(Accessor);

        Accessor(std::shared_ptr<TOwner>                    owner,
                 TObject*                                   fieldType,
                 Str::Type&&                                name,
                 uint8_t                                    id,
                 std::shared_ptr<const Binding::Expression> returnType,
                 std::shared_ptr<const Binding::Expression> argType) :
            NamedIndexT<TOwner, Accessor>::NamedObject(owner, std::move(name)),
            Binding::BindableT<Accessor>(Str::Create(L"FieldType"),
                                         &Accessor::GetFieldTypeBindable,
                                         Str::Create(L"Id"),
                                         &Accessor::GetId,
                                         Str::Create(L"ReturnType"),
                                         &Accessor::GetReturnTypeBindable,
                                         Str::Create(L"Arg"),
                                         &Accessor::GetArgTypeBindable),
            _id(id),
            _argType(argType),
            _returnType(returnType),
            _owner(owner),
            _fieldType(fieldType)

        {
        }
        auto GetId() const { return Str::Create(std::to_wstring(_id)); }

        Binding::IBindable& GetFieldTypeBindable() const { return *_fieldType; }

        Binding::IBindable& GetReturnTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ReturnType: " + _returnType->Stringify(); });
            return *_owner->GetFieldTypeName(Binding::BindingContext::EvaluateExpression(*_returnType)->String());
        }

        Binding::IBindable& GetArgTypeBindable() const
        {
            ACTION_CONTEXT([&]() { return L"Mutator ArgType: " + _argType->Stringify(); });
            return *_owner->GetFieldTypeName(Binding::BindingContext::EvaluateExpression(*_argType)->String());
        }

        uint8_t                                    _id;
        std::shared_ptr<const Binding::Expression> _argType;
        std::shared_ptr<const Binding::Expression> _returnType;
        std::shared_ptr<TOwner>                    _owner;
        TObject*                                   _fieldType;
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
        FieldType(std::shared_ptr<TOwner>                    owner,
                  Str::Type&&                                name,
                  std::optional<std::shared_ptr<IFieldType>> basetype,
                  std::shared_ptr<Binding::AttributeMap>     map) :
            Binding::BindableT<TObject, FieldType>(Str::Create(L"Id"), &FieldType::GetFieldId),
            IDLGenerics::AnnotatedObjectT<TOwner, FieldType>(map),
            NamedIndexT<TOwner, TObject>::NamedObject(owner, std::move(name)),
            _owner(owner)
        {
            static_assert(std::is_base_of_v<FieldType, TObject>, "TObject should have FieldType as a base class");

            if (!basetype.has_value())
            {
                auto category           = GetFieldCategoryStatic<TObject>();
                auto defaultforcategory = L"default_" + Str::Value(category);
                auto defbasetype        = owner->TryGetFieldTypeName(Str::Create(defaultforcategory));
                if (!defbasetype.has_value())
                {
                    defbasetype = owner->TryGetFieldTypeName(Str::Create(L"default"));
                }
                basetype = defbasetype;
            }
            if (basetype.has_value())
            {
                AddBaseObject(basetype.value());
            }
        }

        DELETE_COPY_AND_MOVE(FieldType);

        template <typename... TArgs> auto CreateMutator(TArgs&&... args)
        {
            auto fieldType = static_cast<TObject*>(this);
            return NamedIndexT<TOwner, Mutator>::Owner::CreateNamedObject(_owner, fieldType, std::forward<TArgs>(args)...);
        }

        template <typename... TArgs> auto CreateAccessor(TArgs&&... args)
        {
            auto fieldType = static_cast<TObject*>(this);
            return NamedIndexT<TOwner, Accessor>::Owner::CreateNamedObject(_owner, fieldType, std::forward<TArgs>(args)...);
        }

        //  virtual Str::Type GetFieldName() override;
        virtual void AddAttributes(std::shared_ptr<Binding::AttributeMap> /* map */) {}
        void         SetFieldId(size_t id) { _fieldId = id; }
        Str::Type    GetFieldId() const { return Str::Create(std::to_wstring(_fieldId)); }
        size_t       _fieldId{};

        std::shared_ptr<TOwner> _owner;
    };
};

class ConstValue
{
    public:
    enum class ValueType
    {
        Null,
        Integer,
        Double,
        String,
        List,
        Map
    } valueType;
    //    typedef std::unordered_map<std::shared_ptr<ConstValue>, std::shared_ptr<ConstValue>> Map;
    int                                                                          intValue{};
    double                                                                       doubleValue{};
    Str::Type                                                                    stringValue{};
    std::vector<std::unique_ptr<ConstValue>>                                     listValue{};
    std::unordered_map<std::unique_ptr<ConstValue>, std::unique_ptr<ConstValue>> mapValue{};

    ConstValue(int value) : valueType(ValueType::Integer), intValue(value) {}
    ConstValue(double value) : valueType(ValueType::Double), doubleValue(value) {}
    ConstValue(Str::Type&& value) : valueType(ValueType::String), stringValue(std::move(value)) {}
    ConstValue() : valueType(ValueType::Null) {}

    Str::Type Stringify() const
    {
        switch (valueType)
        {
        case ValueType::Null: return Str::Create(L"nullptr");
        case ValueType::Integer: return Str::Create(std::to_wstring(intValue));
        case ValueType::Double: return Str::Create(std::to_wstring(doubleValue));
        case ValueType::String: return Str::Create(L"\"" + Str::Value(stringValue) + L"\"");
        case ValueType::List: throw std::invalid_argument("List Const Value Unsupported");
        case ValueType::Map: throw std::invalid_argument("Map Const Value Unsupported");
        default: throw std::invalid_argument("Unknown const value type");
        }
    }

    static Str::Type DefaultStringifiedValue() { return Str::Create(L"{}"); }
};

template <typename TOwner, typename TObject> struct StorageIndexT
{

    struct Owner : public FieldTypeIndex<TOwner, TObject>::Owner
    {
        Owner() = default;

        DELETE_COPY_AND_MOVE(Owner);

        template <typename... TArgs> auto CreateStorageObject(TArgs&&... args)
        {
            static_assert(std::is_base_of<StorageIndexT<TOwner, TObject>::StorageType, TObject>::value);
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

        StructAttributeFieldValue(std::shared_ptr<FieldAttribute> owner, Str::Type&& name, std::shared_ptr<Binding::Expression>&& value) :
            Binding::BindableT<StructAttributeFieldValue>(Str::Create(L"Value"), &StructAttributeFieldValue::Value),
            NamedIndexT<FieldAttribute, StructAttributeFieldValue>::NamedObject(owner, std::move(name)),
            _value(std::move(value))
        {
        }

        DELETE_COPY_AND_MOVE(StructAttributeFieldValue);

        auto const& Value() const { return *_value; }

        std::shared_ptr<Binding::Expression> _value;
    };

    struct FieldAttribute : public std::enable_shared_from_this<FieldAttribute>,
                            public NamedIndexT<TObject, FieldAttribute>::NamedObject,
                            public NamedIndexT<FieldAttribute, StructAttributeFieldValue>::Owner

    {
        OBJECTNAME(FieldAttribute);

        DELETE_COPY_AND_MOVE(FieldAttribute);

        FieldAttribute(std::shared_ptr<TObject> owner, Str::Type&& name) :
            NamedIndexT<TObject, FieldAttribute>::NamedObject(owner, std::move(name))
        {
        }

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
        DELETE_COPY_AND_MOVE(Field);

        Field(std::shared_ptr<TObject>               owner,
              Str::Type&&                            name,
              std::shared_ptr<IFieldType>            fieldType,
              std::shared_ptr<ConstValue>            defaultValue,
              std::shared_ptr<Binding::AttributeMap> map) :
            Binding::BindableT<Field>(Str::Create(L"FieldType"),
                                      &Field::GetFieldTypeBindable,
                                      Str::Create(L"InitialValue"),
                                      &Field::GetInitialValue,
                                      Str::Create(L"HasDefaultValue"),
                                      &Field::HasDefaultValue,
                                      Str::Create(L"IsOptional"),
                                      &Field::IsOptional),
            IDLGenerics::AnnotatedObjectT<Field>(map),
            NamedIndexT<TObject, Field>::NamedObject(owner, std::move(name)),
            _defaultValue(defaultValue),
            _fieldType(fieldType),
            _map(map)
        {
        }

        Str::Type GetInitialValue() const
        {
            if (_defaultValue != nullptr)
            {
                return _defaultValue->Stringify();
            }
            Binding::BindingContext context;
            auto                    defval = _fieldType->TryLookupOrNull(context, Str::Create(L"DefaultValue"));
            if (defval != nullptr)
            {
                assert(defval->GetType() == Binding::Type::String || defval->GetType() == Binding::Type::Expr);
                if (defval->GetType() == Binding::Type::String)
                {
                    return Str::Copy(defval->GetString());
                }
                else
                {
                    return defval->GetExpr().String();
                }
            }
            return ConstValue::DefaultStringifiedValue();
        }

        Str::Type HasDefaultValue() const { return Str::Create(_defaultValue ? L"true" : L"false"); }

        Str::Type IsOptional() const { return Str::Create(L"false"); }

        Binding::IBindable& GetFieldTypeBindable() const { return *_fieldType; }
        bool                HasAttributes() const { return _map != nullptr; }
        const auto&         GetAttributes() const { return _map->GetAttributes(); }

        std::shared_ptr<ConstValue> _defaultValue;

        std::shared_ptr<IFieldType>            _fieldType;
        std::shared_ptr<Binding::AttributeMap> _map;
    };

    struct StorageType : public FieldTypeIndex<TOwner, TObject>::FieldType,
                         public NamedIndexT<TObject, FieldAttribute>::Owner,
                         public NamedIndexT<TObject, Field>::Owner
    {
        StorageType(std::shared_ptr<TOwner>                    owner,
                    Str::Type&&                                name,
                    std::optional<std::shared_ptr<IFieldType>> basetype,
                    std::shared_ptr<Binding::AttributeMap>     map) :
            FieldTypeIndex<TOwner, TObject>::FieldType(owner, std::move(name), basetype, map), _owner(owner)
        {
            static_assert(std::is_base_of_v<StorageType, TObject>, "StorageType should be a base of TObject");
        }

        DELETE_COPY_AND_MOVE(StorageType);

        void CreateField(std::shared_ptr<IDLGenerics::IFieldType> fieldType,
                         Str::Type&&                              name,
                         std::shared_ptr<ConstValue>              defaultValue,
                         std::shared_ptr<Binding::AttributeMap>   map)
        {
            auto field = NamedIndexT<TObject, Field>::Owner::CreateNamedObject(
                SUPER(TObject).shared_from_this(), Str::Copy(name), fieldType, defaultValue, map);
            if (!field->HasAttributes()) return;
            for (const auto& it : field->GetAttributes())
            {
                auto attribute = NamedIndexT<TObject, FieldAttribute>::Owner::TryLookup(it.first);
                if (!attribute.has_value())
                {
                    attribute = NamedIndexT<TObject, FieldAttribute>::Owner::CreateNamedObject(SUPER(TObject).shared_from_this(),
                                                                                               Str::Copy(it.first));
                }
                attribute.value()->AddAttributeForFieldName(Str::Copy(name), Binding::Expression::Clone(it.second));
            }
            // Make sure all attributes on fields
        }

        virtual Str::Type GetFieldName() override { return Str::Copy(this->Name()); }

        std::shared_ptr<TOwner> _owner;
    };
};

}    // namespace IDLGenerics

#pragma warning(pop)
