#pragma once
#include "Logging.h"
#include "Value.h"
#include "shared_string.h"
#include "uuidobject.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <climits>
#include <ctype.h>
#include <list>
#include <span>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// TODO: Get rid of this

using timestamp = decltype(std::chrono::system_clock::now());

namespace ReflectionBase
{

namespace ExceptionTraits
{
struct Generic
{
};
struct UnknownDataTypeHandler
{
};
};    // namespace ExceptionTraits

namespace Exception
{
struct Exception : ::Logging::Exception<::Logging::GenericException>
{
    Exception() : ::Logging::Exception<::Logging::GenericException>(CorrelationVector::Create()) {}
};
struct UnknownDataTypeHandler
{
};
}    // namespace Exception

struct ExceptionUnknownDataTypeHandlerTraits
{
};

enum class DataType
{
    Invalid,
    Value,
    List,
    Object,
    Enum,
    Union,
    Unknown    // TODO : Why Invalid and Unknown
};

enum class Flag : uint8_t
{
    Default,
    Optional,
    HasDefaultValue,
    Max
};

struct Flags : std::bitset<static_cast<size_t>(Flag::Max) + 1u>
{
    Flags(std::initializer_list<Flag> flags)
    {
        for (const auto f : flags)
        {
            set(static_cast<size_t>(f));
        }
    }

    bool operator[](Flag f) const { return test(static_cast<size_t>(f)); }
};

struct DataInfo
{
    shared_string                          name{};
    shared_string                          description{};
    shared_string                          typeinfo{};
    DataType                               datatype{};
    Flags                                  flags{};
    std::vector<Value>                     acceptablevalues{};
    std::vector<std::shared_ptr<DataInfo>> children{};
    bool                                   optional() const { return flags[Flag::Optional]; }
    bool                                   hasdefaultvalue() const { return flags[Flag::HasDefaultValue]; }
    bool                                   required() const { return !(flags[Flag::Optional] || flags[Flag::HasDefaultValue]); }
};

template <DataType TDataType> struct IDataTypeHandler;

template <> struct IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::Unknown>() = default;

    virtual shared_string Name() const                                      = 0;
    virtual shared_string Description() const                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const = 0;

    virtual DataType GetDataType() const = 0;

    virtual std::shared_ptr<DataInfo> GetDataInfo() const
    {
        return std::shared_ptr<DataInfo>(new DataInfo{Name(), Description(), Name(), GetDataType(), {}, {}, {}});
    }

    virtual const IDataTypeHandler<DataType::Value>*  ValueHandler() const  = 0;
    virtual const IDataTypeHandler<DataType::List>*   ListHandler() const   = 0;
    virtual const IDataTypeHandler<DataType::Object>* ObjectHandler() const = 0;
    virtual const IDataTypeHandler<DataType::Enum>*   EnumHandler() const   = 0;
    virtual const IDataTypeHandler<DataType::Union>*  UnionHandler() const  = 0;

    // template <DataType TDataType> const IDataTypeHandler<TDataType>* CastTo() const;

    inline const IDataTypeHandler<DataType::Value>*  CastToValue() const { return ValueHandler(); }
    inline const IDataTypeHandler<DataType::List>*   CastToList() const { return ListHandler(); }
    inline const IDataTypeHandler<DataType::Object>* CastToObject() const { return ObjectHandler(); }
    inline const IDataTypeHandler<DataType::Enum>*   CastToEnum() const { return EnumHandler(); }
    inline const IDataTypeHandler<DataType::Union>*  CastToUnion() const { return UnionHandler(); }
};

template <> struct IDataTypeHandler<DataType::Value> : public IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::Value>() = default;
    DataType                                 GetDataType() const override { return DataType::Value; }
    const IDataTypeHandler<DataType::Value>* ValueHandler() const override { return this; }

    // const IDataTypeHandler<DataType::Value>*  ValueHandler()  const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::List>*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Enum>*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Union>*  UnionHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    virtual void          Write(void* ptr, Value const& value) const                 = 0;
    virtual Value         Read(void* ptr) const                                      = 0;
    virtual shared_string Name() const override                                      = 0;
    virtual shared_string Description() const override                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const override = 0;
};

template <> struct IDataTypeHandler<DataType::List> : public IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::List>() = default;
    DataType                                GetDataType() const override { return DataType::List; }
    const IDataTypeHandler<DataType::List>* ListHandler() const override { return this; }

    const IDataTypeHandler<DataType::Value>* ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandler<DataType::List>*   ListHandler()   const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Enum>*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Union>*  UnionHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    virtual shared_string Name() const override                                      = 0;
    virtual shared_string Description() const override                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const override = 0;

    struct SubComponent
    {
        const IDataTypeHandler<DataType::Unknown>* handler;
        void*                                      ptr;
    };

    virtual SubComponent GetListItemHandler() const                                                       = 0;
    virtual void         Start() const                                                                    = 0;
    virtual void         End() const                                                                      = 0;
    virtual SubComponent MoveNext(void* rawptr) const                                                     = 0;
    virtual bool         TryGetSubComponent(void* rawptr, size_t index, SubComponent& subcomponent) const = 0;
};

template <> struct IDataTypeHandler<DataType::Object> : IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::Object>() = default;
    DataType                                  GetDataType() const override { return DataType::Object; }
    const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { return this; }

    const IDataTypeHandler<DataType::Value>* ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::List>*  ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Enum>*  EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Union>* UnionHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    struct SubComponent
    {
        shared_string                              keyname;
        const IDataTypeHandler<DataType::Unknown>* handler;
        void*                                      ptr;
    };

    virtual void                      Start() const                                                                  = 0;
    virtual bool                      TryGetSubComponent(void* rawptr, Value& key, SubComponent& subcomponent) const = 0;
    virtual size_t                    GetSubComponentCount() const                                                   = 0;
    virtual SubComponent              GetSubComponentAt(void* rawptr, size_t index) const                            = 0;
    virtual void                      End() const                                                                    = 0;
    virtual shared_string             Name() const override                                                          = 0;
    virtual shared_string             Description() const override                                                   = 0;
    virtual shared_string             AttributeValue(const std::string_view& key) const override                     = 0;
    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        std::vector<std::shared_ptr<DataInfo>> children;
        for (size_t i = 0; i < GetSubComponentCount(); i++)
        {
            auto h     = GetSubComponentAt(nullptr, i);
            auto info  = h.handler->GetDataInfo();
            info->name = h.keyname;
            children.push_back(std::move(info));
        }

        return std::shared_ptr<DataInfo>(new DataInfo{Name(), Description(), Name(), GetDataType(), {}, {}, std::move(children)});
    }
};

template <> struct IDataTypeHandler<DataType::Enum> : IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::Enum>() = default;
    DataType                                GetDataType() const override { return DataType::Enum; }
    const IDataTypeHandler<DataType::Enum>* EnumHandler() const override { return this; }

    const IDataTypeHandler<DataType::Value>*  ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::List>*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandler<DataType::Enum>*   EnumHandler()   const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Union>* UnionHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    virtual void               Write(void* ptr, Value const& value) const                 = 0;
    virtual Value              Read(void* ptr) const                                      = 0;
    virtual shared_string      Name() const override                                      = 0;
    virtual shared_string      AttributeValue(const std::string_view& key) const override = 0;
    virtual std::vector<Value> AcceptableValues() const                                   = 0;

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        return std::shared_ptr<DataInfo>(new DataInfo{Name(), Description(), Name(), GetDataType(), {}, AcceptableValues(), {}});
    }
};

template <> struct IDataTypeHandler<DataType::Union> : IDataTypeHandler<DataType::Unknown>
{
    virtual ~IDataTypeHandler<DataType::Union>() = default;
    DataType                                 GetDataType() const override { return DataType::Union; }
    const IDataTypeHandler<DataType::Union>* UnionHandler() const override { return this; }

    const IDataTypeHandler<DataType::Value>*  ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::List>*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Object>* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandler<DataType::Enum>*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandler<DataType::Union>*  UnionHandler()  const { throw Exception::UnknownDataTypeHandler(); }

    struct UnionTypeSubComponent
    {
        shared_string                           name;
        const IDataTypeHandler<DataType::Enum>* uniontypehandler;
        void*                                   ptr;
    };

    struct UnionSubComponent
    {
        shared_string                              name;
        const IDataTypeHandler<DataType::Unknown>* handler;
        void*                                      ptr;
    };

    virtual void                           Start() const                                              = 0;
    virtual UnionTypeSubComponent          GetUnionTypesHandler(void* rawptr) const                   = 0;
    virtual UnionSubComponent              GetActiveUnionHandler(void* rawptr) const                  = 0;
    virtual void                           End() const                                                = 0;
    virtual shared_string                  Name() const override                                      = 0;
    virtual shared_string                  Description() const override                               = 0;
    virtual shared_string                  AttributeValue(const std::string_view& key) const override = 0;
    virtual std::vector<Value>             AcceptableTypeNames() const                                = 0;
    virtual std::vector<UnionSubComponent> SubComponentHandlers() const                               = 0;
    virtual std::shared_ptr<DataInfo>      GetDataInfo() const override
    {
        std::vector<std::shared_ptr<DataInfo>> children;
        for (auto& h : SubComponentHandlers())
        {
            auto info  = h.handler->GetDataInfo();
            info->name = h.name;
            children.push_back(std::move(info));
        }

        return std::shared_ptr<DataInfo>(
            new DataInfo{Name(), Description(), Name(), GetDataType(), {}, AcceptableTypeNames(), std::move(children)});
    }
};

template <typename T, typename = void> struct TypeTraits;

struct ObjMarker
{
};
struct InterfaceMarker
{
};
struct InterfaceFactoryMarker
{
};

struct InterfaceObjectTracker
{
};

template <typename TInterface> struct Interface : public InterfaceMarker
{
    constexpr std::string_view Name() { return typeid(TInterface).name(); }
    using Id = UuidBasedId<TInterface>;
    const Id& GetObjectUuid() { return _id; }

    /// TODO : Fix pointers
    static TInterface* FindObjectById(const Id& id) { return static_cast<TInterface*>(_GetRegistry()[id]); }
    static TInterface* FindObjectById(const std::string_view& id) { return FindObjectById(Id{UuidStr(id)}); }

    Interface() { _GetRegistry()[_id] = this; }

    DELETE_COPY_AND_MOVE(Interface);

    static std::unordered_map<Id, Interface<TInterface>*>& _GetRegistry()
    {
        static std::unordered_map<Id, Interface<TInterface>*> registry;
        return registry;
    }

    Id _id = Id::Create();
};

template <typename TInterface> struct InterfaceFactory : public InterfaceFactoryMarker, public InterfaceMarker
{
};

template <typename TInterface> struct InterfaceActivator
{
    static std::unique_ptr<TInterface> Activate();
};

struct IInterfaceApiHandler
{
    virtual ~IInterfaceApiHandler()                                              = default;
    virtual shared_string                              Name()                    = 0;
    virtual const IDataTypeHandler<DataType::Unknown>* ReturnDataHandler() const = 0;
    virtual const IDataTypeHandler<DataType::Unknown>* ArgsHandler() const       = 0;
    virtual void                                       Invoke(void* ifaceobj, void* args, void* result);
};

struct IInterfaceHandler
{
    virtual ~IInterfaceHandler()                                    = default;
    virtual shared_string               Name()                      = 0;
    virtual const IInterfaceApiHandler* FindApi(Value& value) const = 0;
};

template <typename TInterfaceApi> struct InterfaceApiTraits
{
    using InterfaceType = typename TInterfaceApi::InterfaceType;
    struct ArgsStruct
    {
        InterfaceType* instance;
    };

    struct ReturnStruct
    {
    };

    static ReturnStruct Invoke(ArgsStruct& args);

    //    using Handler = void;
    //    using ReturnDataType = void;
    //    using ArgsDataType = void;
    static constexpr bool IsStatic()
    {
        static_assert(!std::is_same<TInterfaceApi, TInterfaceApi>::value, "Specialization Required for InterfaceApiTraits::IsStatic");
        return false;
    }
    static constexpr std::string_view Name()
    {
        static_assert(!std::is_same<TInterfaceApi, TInterfaceApi>::value, "Specialization Required for InterfaceApiTraits::Name");
        return std::string_view("", 0);
    }
};

template <typename... TInterfaceApis> struct InterfaceApiPack
{
};

template <typename TInterface> struct InterfaceTraits
{
    static TInterface* FindInstance(const std::string_view& instanceId);
};

template <typename TDataType, typename... TArgs> void Construct(TDataType* /*ptr*/, TArgs&&... /*args*/)
{
}

template <typename T, typename = void> struct Functions
{
    static bool AreEqual(T const& obj1, T const& obj2) { return std::memcmp(&obj1, &obj2, sizeof(T)) == 0; }
};

template <typename T> struct Functions<T, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
{
    static bool AreEqual(T const& obj1, T const& obj2) { return ReflectionBase::TypeTraits<T&>::AreEqual(obj1, obj2); }
};

template <typename T> bool AreEqual(T const& obj1, T const& obj2)
{
    return Functions<T>::AreEqual(obj1, obj2);
}

}    // namespace ReflectionBase

// Provide handlers for some very commonly used types
namespace ReflectionServices
{
using namespace ReflectionBase;
using DataType = DataType;

template <typename T> struct CommonValueHandler : public IDataTypeHandler<DataType::Value>
{
    virtual void  Write(void* ptr, Value const& value) const override { *(static_cast<T*>(ptr)) = value.convert<T>(); }
    virtual Value Read(void* ptr) const override { return Value(*(static_cast<T*>(ptr))); }

    virtual shared_string Name() const override { return shared_string::make(typeid(this).name()); }
    virtual shared_string Description() const override { return Name(); }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
};

template <typename T> struct EnumTraits
{
    static constexpr std::string_view EnumStrings[]{"Invalid"};    // 0 is always invalid
};

template <typename TEnum> struct EnumHandler : public IDataTypeHandler<DataType::Enum>
{
    virtual void Write(void* ptr, Value const& value) const override
    {
        if (value.GetType() == Value::Type::Unsigned)
        {
            *(static_cast<TEnum*>(ptr)) = (TEnum)((uint32_t)value);
            return;
        }
        if (value.GetType() == Value::Type::String)
        {
            auto const& enumnames = EnumTraits<TEnum>::EnumStrings;
            for (size_t i = 1; i < std::size(enumnames); i++)    // 0 is always invalid
            {
                if ((shared_string)value == enumnames[i])
                {
                    *(static_cast<TEnum*>(ptr)) = static_cast<TEnum>(i);
                    return;
                }
            }
        }
        throw Exception::Exception();
    }
    virtual Value Read(void* /*ptr*/) const override { throw std::logic_error("TODO"); }

    virtual std::vector<Value> AcceptableValues() const override
    {
        std::vector<Value> values;
        auto               enumnames = EnumTraits<TEnum>::EnumStrings;
        for (size_t i = 1; enumnames[i]; i++)
        {
            values.push_back(Value(shared_string::make(enumnames[i])));
        }
        return values;
    }

    virtual shared_string Description() const override { throw std::logic_error("TODO"); }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make(TypeTraits<TEnum&>::Name()); }
};

template <typename TValue> struct StdVectorListHandler : public IDataTypeHandler<DataType::List>
{
    virtual void Start() const override {}
    virtual void End() const override {}

    SubComponent MoveNext(void* ptr) const override
    {
        auto vecptr = static_cast<std::vector<TValue>*>(ptr);
        auto index  = vecptr->size();
        vecptr->resize(index + 1);
        return {&_handler, &((*vecptr)[index])};
    }

    bool TryGetSubComponent(void* ptr, size_t index, SubComponent& subcomponent) const override
    {
        auto vecptr = static_cast<std::vector<TValue>*>(ptr);
        vecptr->resize(index + 1);
        subcomponent = {&_handler, &((*vecptr)[index])};
        return true;
    }
    virtual shared_string Description() const override
    {
        return shared_string::make("[" + _handler.Description().str() + " ...]");
        ;
    }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make("[" + _handler.Name().str() + " ...]"); }
    virtual SubComponent  GetListItemHandler() const override { return {&_handler}; }

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        return std::shared_ptr<DataInfo>(
            new DataInfo{Name(), Description(), Name(), GetDataType(), Flags{Flag::Optional, Flag::HasDefaultValue}, {}, {}});
    }

    typename TypeTraits<TValue&>::Handler _handler;
};

template <typename TValue, size_t N> struct StdArrayListHandler : public IDataTypeHandler<DataType::List>
{
    virtual void Start() const override {}
    virtual void End() const override {}

    SubComponent MoveNext(void* ptr) const override
    {
        auto vecptr = static_cast<std::array<TValue, N>*>(ptr);
        auto index  = vecptr->size();
        return {&_handler, &((*vecptr)[index])};
    }

    bool TryGetSubComponent(void* ptr, size_t index, SubComponent& subcomponent) const override
    {
        auto vecptr  = static_cast<std::array<TValue, N>*>(ptr);
        subcomponent = {&_handler, &((*vecptr)[index])};
        return true;
    }

    virtual shared_string Description() const override
    {
        return shared_string::make("[" + _handler.Description().str() + " ...]");
        ;
    }

    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make("[" + _handler.Name().str() + " ...]"); }
    virtual SubComponent  GetListItemHandler() const override { return {&_handler}; }

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        return std::shared_ptr<DataInfo>(
            new DataInfo{Name(), Description(), Name(), GetDataType(), Flags{Flag::Optional, Flag::HasDefaultValue}, {}, {}});
    }

    typename TypeTraits<TValue&>::Handler _handler;
};

static inline constexpr std::string_view EmptyAttributeValue(const std::string_view& /*name*/)
{
    return "";
}

template <typename TFieldTraits> struct ObjectFieldTraits;
/*
{
    typename TOwner;
    typename TFieldType;

    static constexpr std::string_view Name();
    static constexpr auto TPropertyGetter();
    static constexpr auto TPropertySetter();
    static constexpr auto TAttributeValue();
    static constexpr auto Flags();
};
*/

template <DataType TDataType, typename TFieldTraits> struct ObjectDataTypeHandler;

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::Value, TFieldTraits> : public IDataTypeHandler<DataType::Value>
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    void Write(void* ptr, Value const& value) const override
    {
        typename TFieldTraits::TFieldType                                                fieldValue;
        typename ReflectionBase::TypeTraits<typename TFieldTraits::TFieldType&>::Handler handler;
        handler.Write(&fieldValue, value);
        (static_cast<TOwner*>(ptr)->*(TFieldTraits::TPropertySetter()))(std::move(fieldValue));
    }

    Value Read(void* ptr) const override
    {
        auto method = TFieldTraits::TPropertyGetter();
        auto obj    = static_cast<TOwner*>(ptr);

        typename ReflectionBase::TypeTraits<typename TFieldTraits::TFieldType&>::Handler handler;
        return handler.Read(&(obj->*method)());
    }

    virtual shared_string Description() const override
    {
        auto fielddesc = TFieldTraits::TAttributeValue("Description");
        return shared_string::make(fielddesc.empty() ? FieldTypeTraits::Description() : fielddesc);
    }

    virtual shared_string AttributeValue(const std::string_view& key) const override
    {
        return shared_string::make(TFieldTraits::TAttributeValue(key));
    }
    virtual shared_string Name() const override { return shared_string::make(TFieldTraits::Name()); }

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        auto info   = IDataTypeHandler<DataType::Value>::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
};

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::List, TFieldTraits> : public IDataTypeHandler<DataType::List>
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    virtual void Start() const override{};
    virtual void End() const override {}

    virtual SubComponent MoveNext(void* rawptr) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        return _handler.MoveNext((void*)&obj);
    }

    virtual bool TryGetSubComponent(void* rawptr, size_t /*index*/, SubComponent& subcomponent) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        subcomponent    = {&_handler, (void*)&obj};
        return true;
    }

    virtual shared_string Description() const override
    {
        auto fielddesc = TFieldTraits::TAttributeValue("Description");
        return shared_string::make(fielddesc.empty() ? FieldTypeTraits::Description() : fielddesc);
    }
    virtual shared_string AttributeValue(const std::string_view& key) const override
    {
        return shared_string::make(TFieldTraits::TAttributeValue(key));
    }
    virtual shared_string             Name() const override { return shared_string::make(FieldTypeTraits::Name()); }
    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        auto info   = IDataTypeHandler<DataType::List>::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
    virtual SubComponent GetListItemHandler() const override { return {&_handler, nullptr}; }

    typename FieldTypeTraits::Handler _handler;
};

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::Object, TFieldTraits> : public IDataTypeHandler<DataType::Object>
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    virtual void Start() const override{};
    virtual void End() const override {}
    virtual bool TryGetSubComponent(void* rawptr, Value& key, SubComponent& subcomponent) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        return _handler.CastToObject()->TryGetSubComponent(&obj, key, subcomponent);
    }

    virtual shared_string Description() const override
    {
        auto fielddesc = TFieldTraits::TAttributeValue("Description");
        return shared_string::make(fielddesc.empty() ? FieldTypeTraits::AttributeValue("Description") : fielddesc);
    }

    virtual shared_string AttributeValue(const std::string_view& key) const override
    {
        return shared_string::make(TFieldTraits::TAttributeValue(key));
    }
    virtual shared_string Name() const override { return shared_string::make(FieldTypeTraits::Name()); }

    virtual size_t       GetSubComponentCount() const override { return _handler.CastToObject()->GetSubComponentCount(); }
    virtual SubComponent GetSubComponentAt(void* rawptr, size_t index) const override
    {
        return _handler.CastToObject()->GetSubComponentAt(rawptr, index);
    }
    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        auto info   = IDataTypeHandler<DataType::Object>::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
    using FieldHandler = typename FieldTypeTraits::Handler;

    FieldHandler _handler;
};

struct ReflectedStruct
{
};

template <typename TStruct, typename... TFieldTraits> struct ReflectedStructHandler : public IDataTypeHandler<DataType::Object>
{
    void Start() const override {}
    void End() const override {}

    template <typename T> struct FieldTraitsHelper
    {
        using TFieldType      = std::invoke_result_t<decltype(T::TPropertyGetter()), std::unique_ptr<TStruct>>;
        using FieldTypeTraits = TypeTraits<TFieldType&>;
    };

    template <typename Trait> struct Handler : public ObjectDataTypeHandler<FieldTraitsHelper<Trait>::FieldTypeTraits::Type(), Trait>
    {
    };

    template <typename Trait, typename... Traits> bool _FindMatch(size_t index, TStruct* ptr, SubComponent& s) const
    {
        if (index == 0)
        {
            s.handler = &std::get<Handler<Trait>>(_handlers);
            s.ptr     = ptr;
            s.keyname = shared_string::make(Trait::Name());
            return true;
        }
        else if constexpr (sizeof...(Traits) != 0)
        {
            return _FindMatch<Traits...>(index - 1, ptr, s);
        }
        else
        {
            return false;
        }
    }

    template <typename Trait, typename... Traits> bool _FindMatch(const std::string_view& key, TStruct* ptr, SubComponent& s) const
    {
        if (key == Trait::Name())
        {
            s.handler = &std::get<Handler<Trait>>(_handlers);
            s.ptr     = ptr;
            s.keyname = shared_string::make(Trait::Name());
            return true;
        }

        if constexpr (sizeof...(Traits) != 0)
        {
            return _FindMatch<Traits...>(key, ptr, s);
        }
        else
        {
            return false;
        }
    }

    bool TryGetSubComponent(void* rawptr, Value& key, SubComponent& subcomponent) const override
    {
        auto ptr = static_cast<TStruct*>(rawptr);
        if (key.GetType() == Value::Type::Unsigned)
        {
            return _FindMatch<TFieldTraits...>(key.convert<size_t>(), ptr, subcomponent);
        }

        return _FindMatch<TFieldTraits...>(key.convert<shared_string>().c_str(), ptr, subcomponent);
    }

    virtual shared_string Description() const override { return nullptr; }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { return nullptr; }
    virtual shared_string Name() const override { return shared_string::make(TypeTraits<TStruct&>::Name()); }
    virtual size_t        GetSubComponentCount() const override { return sizeof...(TFieldTraits); }
    virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
    {
        SubComponent sub;
        if (!_FindMatch<TFieldTraits...>(index, nullptr, sub))
        {
            throw Exception::Exception();
        }
        sub.ptr = rawptr;
        return sub;
    }

    private:
    std::tuple<Handler<TFieldTraits>...> _handlers;
};

template <typename TUnion, typename TTypeTrait, typename... TFieldTraits>
struct ReflectedUnionHandler : public IDataTypeHandler<DataType::Union>
{
    void Start() const override {}
    void End() const override {}

    template <typename Trait> struct Handler : public ObjectDataTypeHandler<Trait::Type(), Trait>
    {
    };

    virtual UnionTypeSubComponent GetUnionTypesHandler(void* rawptr) const override
    {
        auto ptr = static_cast<TUnion*>(rawptr);

        UnionTypeSubComponent s;
        s.name             = shared_string::make("AllowedType");
        s.ptr              = ptr;
        s.uniontypehandler = &_typehandler;
        return s;
    }

    virtual UnionSubComponent GetActiveUnionHandler(void* rawptr) const override
    {
        auto   ptr        = static_cast<TUnion*>(rawptr);
        size_t activetype = static_cast<size_t>(ptr->Type());
        if (activetype == 0)    // Enum type 0 is reserved to be unset
        {
            throw Exception::Exception();
        }
        auto s = _structhandler.GetSubComponentAt(rawptr, (activetype - 1));
        return UnionSubComponent{s.keyname, s.handler, s.ptr};
    }

    virtual shared_string Description() const override { return AttributeValue("Description"); }
    virtual shared_string AttributeValue(const std::string_view& key) const override
    {
        return shared_string::make(TypeTraits<TUnion&>::AttributeValue(key));
    }
    virtual shared_string      Name() const override { return shared_string::make(TypeTraits<TUnion&>::Name()); }
    virtual std::vector<Value> AcceptableTypeNames() const override { return _typehandler.AcceptableValues(); }

    virtual std::vector<UnionSubComponent> SubComponentHandlers() const override
    {
        std::vector<UnionSubComponent> comps;
        for (size_t i = 0; i < _structhandler.GetSubComponentCount(); i++)
        {
            UnionSubComponent u;
            auto              s = _structhandler.GetSubComponentAt(nullptr, i);
            u.name              = s.keyname;
            u.handler           = s.handler;
            u.ptr               = s.ptr;

            comps.push_back(u);
        }
        return comps;
    }

    private:
    ::ReflectionServices::EnumHandler<TTypeTrait>   _typehandler;
    ReflectedStructHandler<TUnion, TFieldTraits...> _structhandler;
};

template <typename T, typename TStackData> struct StateTraker
{
    enum Mode
    {
        Default,
        List,
        Obj,
        Enum,
        Union,
        UnionChild,
        ObjKeyValue,
        ListValue
    };
    StateTraker(T* ptr, TStackData&& data) { _stack.push_back(StateStack{&_rootHandler, (void*)ptr, Mode::Obj, std::move(data)}); }

    DataType GetDataTypeHint() const { return _stack.back().Handler->GetDataType(); }

    const IDataTypeHandler<DataType::Unknown>* GetHandler() const { return _stack.back().Handler; }

    std::string Stringify()
    {
        switch (GetDataTypeHint())
        {
        case DataType::Value:
        {
            return _stack.back().Handler->ValueHandler()->Read(_stack.back().Ptr).template convert<shared_string>().str();
        }
        case DataType::Object: [[fallthrough]];
        case DataType::Unknown: [[fallthrough]];
        case DataType::Invalid: [[fallthrough]];
        case DataType::List: [[fallthrough]];
        case DataType::Enum: [[fallthrough]];
        case DataType::Union: break;
        }
        throw std::logic_error("unsupported data type");
    }

    void HandleValue(Value const& val, TStackData&& data)
    {
        _Start(Mode::Default, std::move(data));
        auto handler = _stack.back().Handler->ValueHandler();
        assert(handler != nullptr);
        handler->Write(_stack.back().Ptr, val);
        _End(Mode::Default);
    }

    void HandleEnum(Value const& val, TStackData&& data)
    {
        _Start(Mode::Enum, std::move(data));
        auto handler = _stack.back().Handler->EnumHandler();
        assert(handler != nullptr);
        handler->Write(_stack.back().Ptr, val);
        _End(Mode::Enum);
    }

    void UnionType(Value const& val, TStackData&& data)
    {
        _Start(Mode::Union, std::move(data));
        auto handler = _stack.back().Handler->UnionHandler();
        assert(handler != nullptr);
        auto typecomp = handler->GetUnionTypesHandler(_stack.back().Ptr);
        typecomp.uniontypehandler->Write(typecomp.ptr, val);
        auto activecomp = handler->GetActiveUnionHandler(_stack.back().Ptr);
        _stack.push_back(StateStack{activecomp.handler, activecomp.ptr, Mode::UnionChild, std::move(data)});
    }

    void _Start(Mode mode, TStackData&& data)
    {
        if (_stack.back().mode == Mode::List)
        {
            auto handler = _stack.back().Handler->ListHandler();
            assert(handler != nullptr);
            auto subcomponent = handler->MoveNext(_stack.back().Ptr);
            _stack.push_back(StateStack{subcomponent.handler, subcomponent.ptr, Mode::ListValue, std::move(data)});
        }
        else
        {
            auto back = _stack.back();
            back.mode = mode;
            back.data = std::move(data);
            _stack.push_back(back);
        }
    }

    void _End(Mode /*mode*/)
    {
        _stack.pop_back();
        if (_stack.back().mode == Mode::ObjKeyValue)
        {
            _stack.pop_back();
        }
    }

    void ListStart(TStackData&& data)
    {
        _Start(Mode::List, std::move(data));
        auto handler = _stack.back().Handler->ListHandler();
        assert(handler != nullptr);
        handler->Start();
    }

    void ListEnd()
    {
        auto handler = _stack.back().Handler->ListHandler();
        assert(handler != nullptr);
        handler->End();
        _End(Mode::List);
    }

    void ObjStart(TStackData&& data)
    {
        _Start(Mode::Obj, std::move(data));
        auto handler = _stack.back().Handler->CastToObject();
        assert(handler != nullptr);
        handler->Start();
    }

    void ObjEnd()
    {
        auto handler = _stack.back().Handler->CastToObject();
        assert(handler != nullptr);
        handler->End();
        _End(Mode::Obj);
    }

    void ObjKey(Value key, TStackData&& data)
    {
        if (!TryObjKey(key, std::move(data)))
        {
            throw Exception::Exception();
        }
    }

    bool TryObjKey(Value key, TStackData&& data)
    {
        IDataTypeHandler<DataType::Object>::SubComponent subcomponent;
        if (!_stack.back().Handler->CastToObject()->TryGetSubComponent(_stack.back().Ptr, key, subcomponent))
        {
            return false;
        }
        _stack.push_back(StateStack{subcomponent.handler, subcomponent.ptr, Mode::ObjKeyValue, std::move(data)});
        return true;
    }

    struct StateStack
    {
        const IDataTypeHandler<DataType::Unknown>* Handler = nullptr;
        void*                                      Ptr     = nullptr;
        Mode                                       mode;
        TStackData                                 data;
    };

    private:
    typename TypeTraits<T&>::Handler _rootHandler;
    std::vector<StateStack>          _stack;
};
}    // namespace ReflectionServices

namespace ReflectionServices
{
using namespace ReflectionBase;

template <typename... TApiTraits> struct InterfaceHandler
{
};

}    // namespace ReflectionServices

template <typename T> struct ReflectionBase::TypeTraits<T&, std::enable_if_t<Value::Supported<T>::value>>
{
    static constexpr DataType         Type() { return DataType::Value; }
    static constexpr std::string_view Name() { return std::string_view(typeid(T).name()); }
    static constexpr std::string_view Description() { return Name(); }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    using ValueType = T;
    using Handler   = typename ::ReflectionServices::CommonValueHandler<T>;
};

template <typename TClock, typename TDur> struct ReflectionBase::TypeTraits<std::chrono::time_point<TClock, TDur>&>
{
    using time_point = std::chrono::time_point<TClock, TDur>;

    static constexpr DataType Type() { return DataType::Value; }
    static std::string        Name() { return "timestamp"; }

    static std::string                              Description() { return "Timestamp"; }
    static std::string_view                         AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }
    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    using ValueType = uint64_t;

    struct Handler : public ::ReflectionBase::IDataTypeHandler<DataType::Value>
    {
        virtual void Write(void* ptr, Value const& value) const override
        {
            using tpinttype = decltype(std::chrono::nanoseconds{}.count());
            *(static_cast<time_point*>(ptr))
                = time_point(std::chrono::duration_cast<TDur>(std::chrono::nanoseconds(value.convert<tpinttype>())));
        }

        virtual Value Read(void* ptr) const override
        {
            auto val = std::chrono::duration_cast<std::chrono::nanoseconds>((static_cast<time_point*>(ptr))->time_since_epoch()).count();
            return Value(val);
        }

        virtual shared_string Name() const override { return "timestamp"; }
        virtual shared_string Description() const override { return Name(); }
        virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    };
};

template <typename T> struct ReflectionBase::TypeTraits<std::vector<T>&>
{
    static constexpr DataType Type() { return DataType::List; }
    static std::string        Name() { return "list<" + std::string(::ReflectionBase::TypeTraits<T&>::Name()) + ">"; }

    static std::string Description()
    {
        return "list<" + std::string(::ReflectionBase::TypeTraits<T&>::AttributeValue("Description")) + ">";
    }
    static std::string_view AttributeValue(const std::string_view& key) { return ""; }

    bool AreEqual(std::vector<T>& obj1, std::vector<T>& obj2)
    {
        return std::equal(obj1.begin(), obj2.end(), obj2.begin(), obj2.end(), [](auto&& o1, auto&& o2) {
            return ReflectionBase::TypeTraits<decltype(o1)>::AreEqual(o1, o2);
        });
    }
    using Handler = typename ::ReflectionServices::StdVectorListHandler<T>;
};

template <typename T, size_t N> struct ReflectionBase::TypeTraits<std::array<T, N>&>
{
    static constexpr DataType Type() { return DataType::List; }
    static std::string        Name() { return "array<" + std::string(::ReflectionBase::TypeTraits<T&>::Name()) + ">"; }

    static std::string Description()
    {
        return "array<" + std::string(::ReflectionBase::TypeTraits<T&>::AttributeValue("Description")) + ">";
    }
    static std::string_view AttributeValue(const std::string_view& key) { return ""; }

    bool AreEqual(std::array<T, N>& obj1, std::array<T, N>& obj2)
    {
        return std::equal(obj1.begin(), obj2.end(), obj2.begin(), obj2.end(), [](auto&& o1, auto&& o2) {
            return ReflectionBase::TypeTraits<decltype(o1)>::AreEqual(o1, o2);
        });
    }
    using Handler = typename ::ReflectionServices::StdArrayListHandler<T, N>;
};

template <typename T>
struct ReflectionBase::TypeTraits<std::unique_ptr<T>&, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
{
    static constexpr DataType                       Type() { return DataType::Object; }
    static constexpr std::string_view               Name() { return ::ReflectionBase::TypeTraits<T&>::Name(); }
    static std::string_view                         AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }
    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    struct Handler : public ::ReflectionBase::IDataTypeHandler<DataType::Object>
    {
        virtual void Start() const override {}
        virtual bool TryGetSubComponent(void* rawptr, Value& key, SubComponent& subcomponent) const override
        {
            auto& obj = *static_cast<std::unique_ptr<T>*>(rawptr);
            if (obj == nullptr)
            {
                obj.reset(new T());
            }
            return _handler.TryGetSubComponent(obj.get(), key, subcomponent);
        }
        virtual void End() const override {}

        virtual shared_string Description() const override { return AttributeValue("Description"); }
        virtual shared_string AttributeValue(const std::string_view& key) const override
        {
            return shared_string::make(::ReflectionBase::TypeTraits<T&>::AttributeValue(key));
        }
        virtual shared_string Name() const override { return shared_string::make(::ReflectionBase::TypeTraits<T&>::Name()); }
        virtual size_t        GetSubComponentCount() const override { return _handler.GetSubComponentCount(); }
        virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
        {
            return _handler.GetSubComponentAt(rawptr, index);
        }

        typename ::ReflectionBase::TypeTraits<T&>::Handler _handler;
    };
};

template <typename T>
struct ReflectionBase::TypeTraits<std::shared_ptr<T>&, std::enable_if_t<std::is_base_of<::ReflectionBase::ObjMarker, T>::value>>
{
    static constexpr DataType         Type() { return DataType::Object; }
    static constexpr std::string_view Name() { return ::ReflectionBase::TypeTraits<T&>::Name(); }
    static std::string_view           AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }

    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    struct Handler : public ::ReflectionBase::IDataTypeHandler<DataType::Object>
    {
        virtual void Start() const override {}
        virtual bool TryGetSubComponent(void* rawptr, Value& key, SubComponent& subcomponent) const override
        {
            auto& obj = *static_cast<std::shared_ptr<T>*>(rawptr);
            if (obj == nullptr)
            {
                obj.reset(new T());
            }
            return _handler.TryGetSubComponent(obj.get(), key, subcomponent);
        }
        virtual void End() const override {}

        virtual shared_string Description() const override { return AttributeValue("Description"); }
        virtual shared_string AttributeValue(const std::string_view& key) const override
        {
            return shared_string::make(::ReflectionBase::TypeTraits<T&>::AttributeValue(key));
        }
        virtual shared_string Name() const override { return shared_string::make(::ReflectionBase::TypeTraits<T&>::Name()); }
        virtual size_t        GetSubComponentCount() const override { return _handler.GetSubComponentCount(); }
        virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
        {
            return _handler.GetSubComponentAt(rawptr, index);
        }

        typename ::ReflectionBase::TypeTraits<T&>::Handler _handler;
    };
};
