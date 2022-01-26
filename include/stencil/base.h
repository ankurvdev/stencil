#pragma once
#include "Logging.h"
#include "fixedwidthvalue.h"
#include "shared_tree.h"
#include "uuidobject.h"

#include <algorithm>
#include <bitset>
#include <cassert>
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
#include <uuid.h>
#include <vector>

// TODO: Get rid of this
using timestamp = decltype(std::chrono::system_clock::now());

namespace Stencil
{
struct StructMarker
{};
struct VariantMarker
{};
struct InterfaceMarker
{};

template <typename T>
concept FixedWidthValueConcept = requires(T t)
{
    typename ValueTraits<T>;
};

enum class Type
{
    Invalid,
    Primitive,       // Fixed size or blobs
    Iterable,     // Multiple values can only iterate on values
    Indexable,    // Indexing of values via Atomics
};
template <typename T> struct TypeTraits;

template <typename T>
concept ConceptIndexable = requires(T t)
{
    Stencil::TypeTraits<T>::IsIndexable();
};

template <typename T>
concept ConceptIterableOnly = requires(T t)
{
    Stencil::TypeTraits<T>::IsIterable();
};

template <typename T>
concept ConceptIterableNotIndexable = requires(T t)
{
    Stencil::TypeTraits<T>::IsIterable();
    !Stencil::TypeTraits<T>::IsIndexable();
};

template <typename T>
concept ConceptPrimitiveOnly = requires(T t)
{
    Stencil::TypeTraits<T>::IsPrimitive();
};

#if 0
namespace ExceptionTraits
{
struct Generic
{};
struct UnknownDataTypeHandler
{};
}    // namespace ExceptionTraits

namespace Exception
{
struct Exception : ::Logging::Exception<::Logging::GenericException>
{
    Exception() : ::Logging::Exception<::Logging::GenericException>(CorrelationVector::Create()) {}
};
struct UnknownDataTypeHandler
{};
}    // namespace Exception

struct ExceptionUnknownDataTypeHandlerTraits
{};
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
        for (const auto f : flags) { set(static_cast<size_t>(f)); }
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

struct IDataTypeHandlerUnknown
{
    virtual ~IDataTypeHandlerUnknown() = default;

    virtual shared_string Name() const                                      = 0;
    virtual shared_string Description() const                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const = 0;

    virtual DataType GetDataType() const = 0;

    virtual std::shared_ptr<DataInfo> GetDataInfo() const
    {
        return std::shared_ptr<DataInfo>(new DataInfo{Name(), Description(), Name(), GetDataType(), {}, {}, {}});
    }

    virtual const IDataTypeHandlerValue*  ValueHandler() const  = 0;
    virtual const IDataTypeHandlerList*   ListHandler() const   = 0;
    virtual const IDataTypeHandlerObject* ObjectHandler() const = 0;
    virtual const IDataTypeHandlerEnum*   EnumHandler() const   = 0;
    virtual const IDataTypeHandlerVariant*  VariantHandler() const  = 0;

    // template <DataType TDataType> const IDataTypeHandler<TDataType>* CastTo() const;

    inline const IDataTypeHandlerValue*  CastToValue() const { return ValueHandler(); }
    inline const IDataTypeHandlerList*   CastToList() const { return ListHandler(); }
    inline const IDataTypeHandlerObject* CastToObject() const { return ObjectHandler(); }
    inline const IDataTypeHandlerEnum*   CastToEnum() const { return EnumHandler(); }
    inline const IDataTypeHandlerVariant*  CastToVariant() const { return VariantHandler(); }
};

struct IDataTypeHandlerValue : public IDataTypeHandlerUnknown >
{
    virtual ~IDataTypeHandler() override = default;
    DataType                     GetDataType() const override { return DataType::Value; }
    const IDataTypeHandlerValue* ValueHandler() const override { return this; }

    // const IDataTypeHandlerValue*  ValueHandler()  const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerList*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerObject* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerEnum*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerVariant*  VariantHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    virtual void          Write(void* ptr, Value const& value) const                 = 0;
    virtual Value         Read(void* ptr) const                                      = 0;
    virtual shared_string Name() const override                                      = 0;
    virtual shared_string Description() const override                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const override = 0;
};

struct IDataTypeHandlerList : public IDataTypeHandlerUnknown
{
    virtual ~IDataTypeHandlerList() override = default;
    DataType                    GetDataType() const override { return DataType::List; }
    const IDataTypeHandlerList* ListHandler() const override { return this; }

    const IDataTypeHandlerValue* ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandlerList*   ListHandler()   const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerObject* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerEnum*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerVariant*  VariantHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    virtual shared_string Name() const override                                      = 0;
    virtual shared_string Description() const override                               = 0;
    virtual shared_string AttributeValue(const std::string_view& key) const override = 0;

    struct SubComponent
    {
        const IDataTypeHandlerUnknown* handler;
        void*                          ptr;
    };

    virtual SubComponent GetListItemHandler() const                                             = 0;
    virtual void         Start() const                                                          = 0;
    virtual void         End() const                                                            = 0;
    virtual SubComponent MoveNext(void* rawptr) const                                           = 0;
    virtual bool         TryGetAt(void* rawptr, size_t index, SubComponent& subcomponent) const = 0;
    virtual SubComponent GetOrCreateAt(void* rawptr, size_t index) const                        = 0;
};

struct IDataTypeHandlerObject : IDataTypeHandlerUnknown
{
    virtual ~IDataTypeHandlerObject() override = default;
    DataType                      GetDataType() const override { return DataType::Object; }
    const IDataTypeHandlerObject* ObjectHandler() const override { return this; }

    const IDataTypeHandlerValue* ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerList*  ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandlerObject* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerEnum*  EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerVariant* VariantHandler() const override { throw Exception::UnknownDataTypeHandler(); }

    struct SubComponent
    {
        shared_string                  keyname;
        const IDataTypeHandlerUnknown* handler;
        void*                          ptr;
    };

    virtual void                      Start() const                                                                        = 0;
    virtual bool                      TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const = 0;
    virtual size_t                    GetSubComponentCount() const                                                         = 0;
    virtual SubComponent              GetSubComponentAt(void* rawptr, size_t index) const                                  = 0;
    virtual void                      End() const                                                                          = 0;
    virtual shared_string             Name() const override                                                                = 0;
    virtual shared_string             Description() const override                                                         = 0;
    virtual shared_string             AttributeValue(const std::string_view& key) const override                           = 0;
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

struct IDataTypeHandlerEnum : IDataTypeHandlerUnknown
{
    virtual ~IDataTypeHandlerEnum() override = default;
    DataType                    GetDataType() const override { return DataType::Enum; }
    const IDataTypeHandlerEnum* EnumHandler() const override { return this; }

    const IDataTypeHandlerValue*  ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerList*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerObject* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandlerEnum*   EnumHandler()   const { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerVariant* VariantHandler() const override { throw Exception::UnknownDataTypeHandler(); }

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

struct IDataTypeHandlerVariant : IDataTypeHandlerUnknown
{
    virtual ~IDataTypeHandlerVariant() override = default;
    DataType                     GetDataType() const override { return DataType::Variant; }
    const IDataTypeHandlerVariant* VariantHandler() const override { return this; }

    const IDataTypeHandlerValue*  ValueHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerList*   ListHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerObject* ObjectHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    const IDataTypeHandlerEnum*   EnumHandler() const override { throw Exception::UnknownDataTypeHandler(); }
    // const IDataTypeHandlerVariant*  VariantHandler()  const { throw Exception::UnknownDataTypeHandler(); }

    struct VariantTypeSubComponent
    {
        shared_string               name;
        const IDataTypeHandlerEnum* uniontypehandler;
        void*                       ptr;
    };

    struct VariantSubComponent
    {
        shared_string                  name;
        const IDataTypeHandlerUnknown* handler;
        void*                          ptr;
    };

    virtual void                           Start() const                                              = 0;
    virtual VariantTypeSubComponent          GetVariantTypesHandler(void* rawptr) const                   = 0;
    virtual VariantSubComponent              GetActiveVariantHandler(void* rawptr) const                  = 0;
    virtual void                           End() const                                                = 0;
    virtual shared_string                  Name() const override                                      = 0;
    virtual shared_string                  Description() const override                               = 0;
    virtual shared_string                  AttributeValue(const std::string_view& key) const override = 0;
    virtual std::vector<Value>             AcceptableTypeNames() const                                = 0;
    virtual std::vector<VariantSubComponent> SubComponentHandlers() const                               = 0;
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
{};
struct InterfaceMarker
{};
struct InterfaceFactoryMarker
{};

struct InterfaceObjectTracker
{};

template <typename TInterface> struct InterfaceT : public InterfaceMarker
{
    using Interface = TInterface;

    constexpr std::string_view Name() { return typeid(TInterface).name(); }
    using Id = UuidBasedId<TInterface>;
    const Id& GetObjectUuid() { return _id; }

    /// TODO : Fix pointers
    static TInterface* FindObjectById(const Id& id) { return static_cast<TInterface*>(_GetRegistry()[id]); }
    static TInterface* FindObjectById(const std::string_view& id) { return FindObjectById(Id{UuidStr(id)}); }

    InterfaceT() { _GetRegistry()[_id] = this; }

    DELETE_COPY_AND_MOVE(InterfaceT);

    static std::unordered_map<Id, InterfaceT<TInterface>*>& _GetRegistry()
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
        static std::unordered_map<Id, InterfaceT<TInterface>*> registry;
#pragma clang diagnostic pop
        return registry;
    }

    Id _id = Id::Create();
};

struct IInterfaceApiHandler
{
    virtual ~IInterfaceApiHandler()                                  = default;
    virtual shared_string                  Name()                    = 0;
    virtual const IDataTypeHandlerUnknown* ReturnDataHandler() const = 0;
    virtual const IDataTypeHandlerUnknown* ArgsHandler() const       = 0;
    virtual void                           Invoke(void* ifaceobj, void* args, void* result);
};

struct IInterfaceHandler
{
    virtual ~IInterfaceHandler()                                    = default;
    virtual shared_string               Name()                      = 0;
    virtual const IInterfaceApiHandler* FindApi(Value& value) const = 0;
};
#endif
template <typename TInterfaceApi> struct InterfaceApiTraits
{
    using InterfaceType = typename TInterfaceApi::InterfaceType;
    struct ArgsStruct
    {
        InterfaceType* instance;
    };

    struct ReturnStruct
    {};

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

template <typename T> struct InterfaceObjectTraits
{
    static constexpr std::string_view Name() { return "Unknown"; }
};

template <typename TInterface> struct InterfaceTraits
{
    static TInterface* FindInstance(const std::string_view& instanceId);
};

template <typename TDataType, typename... TArgs> void Construct(TDataType* /*ptr*/, TArgs&&... /*args*/)
{}

template <typename T, typename = void> struct Functions
{
    static bool AreEqual(T const& obj1, T const& obj2) { return std::memcmp(&obj1, &obj2, sizeof(T)) == 0; }
};

template <typename T> struct Functions<T, std::enable_if_t<std::is_base_of<::Stencil::StructMarker, T>::value>>
{
    static bool AreEqual(T const& obj1, T const& obj2) { return Stencil::TypeTraits<T>::AreEqual(obj1, obj2); }
};

template <typename T> bool AreEqual(T const& obj1, T const& obj2)
{
    return Functions<T>::AreEqual(obj1, obj2);
}

}    // namespace Stencil

#if 0
// Provide handlers for some very commonly used types
namespace ReflectionServices
{
using namespace Stencil;
using DataType = DataType;

template <typename T> struct CommonValueHandler : public IDataTypeHandlerValue
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

template <typename TEnum> struct EnumHandler : public IDataTypeHandlerEnum
{
    virtual void Write(void* ptr, Value const& value) const override
    {
        if (value.GetType() == Value::Type::Unsigned)
        {
            *(static_cast<TEnum*>(ptr)) = static_cast<TEnum>(static_cast<uint32_t>(value));
            return;
        }
        if (value.GetType() == Value::Type::String)
        {
            auto const& enumnames = EnumTraits<TEnum>::EnumStrings;
            for (size_t i = 1; i < std::size(enumnames); i++)    // 0 is always invalid
            {
                if (static_cast<shared_string>(value) == enumnames[i])
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
        for (size_t i = 1; enumnames[i]; i++) { values.push_back(Value(shared_string::make(enumnames[i]))); }
        return values;
    }

    virtual shared_string Description() const override { throw std::logic_error("TODO"); }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make(TypeTraits<TEnum&>::Name()); }
};

template <typename TValue> struct StdVectorListHandler : public IDataTypeHandlerList
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

    bool TryGetAt(void* ptr, size_t index, SubComponent& subcomponent) const override
    {
        auto& vec = *static_cast<std::vector<TValue>*>(ptr);
        if (index >= vec.size()) return false;
        subcomponent = SubComponent{&_handler, &vec[index]};
        return true;
    }

    SubComponent GetOrCreateAt(void* ptr, size_t index) const override
    {
        auto& vec = *static_cast<std::vector<TValue>*>(ptr);
        if (index >= vec.size()) vec.resize(index + 1);
        return SubComponent{&_handler, &vec[index]};
    }

    virtual shared_string Description() const override { return shared_string::make("[" + _handler.Description().str() + " ...]"); }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make("[" + _handler.Name().str() + " ...]"); }
    virtual SubComponent  GetListItemHandler() const override { throw std::logic_error("TODO"); /*return {&_handler, };*/ }

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        return std::shared_ptr<DataInfo>(
            new DataInfo{Name(), Description(), Name(), GetDataType(), Flags{Flag::Optional, Flag::HasDefaultValue}, {}, {}});
    }

    typename TypeTraits<TValue&>::Handler _handler;
};

template <typename TValue, size_t N> struct StdArrayListHandler : public IDataTypeHandlerList
{
    virtual void Start() const override {}
    virtual void End() const override {}

    SubComponent MoveNext(void* ptr) const override
    {
        auto vecptr = static_cast<std::array<TValue, N>*>(ptr);
        return {&_handler, &((*vecptr)[_index++])};
    }

    bool TryGetAt(void* ptr, size_t index, SubComponent& subcomponent) const override
    {
        auto& vec = *static_cast<std::array<TValue, N>*>(ptr);
        if (index >= N) return false;
        subcomponent = SubComponent{&_handler, &vec[index]};
        return true;
    }

    SubComponent GetOrCreateAt(void* ptr, size_t index) const override
    {
        auto& vec = *static_cast<std::array<TValue, N>*>(ptr);
        if (index >= N) throw std::logic_error("Cannot Create objects on arrays");
        return SubComponent{&_handler, &vec[index]};
    }

    virtual shared_string Description() const override { return shared_string::make("[" + _handler.Description().str() + " ...]"); }

    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
    virtual shared_string Name() const override { return shared_string::make("[" + _handler.Name().str() + " ...]"); }
    virtual SubComponent  GetListItemHandler() const override { throw std::logic_error("TODO"); /* return {&_handler};  */ }

    virtual std::shared_ptr<DataInfo> GetDataInfo() const override
    {
        return std::shared_ptr<DataInfo>(
            new DataInfo{Name(), Description(), Name(), GetDataType(), Flags{Flag::Optional, Flag::HasDefaultValue}, {}, {}});
    }

    typename TypeTraits<TValue&>::Handler _handler;
    mutable size_t                        _index = 0;
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

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::Value, TFieldTraits> : public IDataTypeHandlerValue
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    void Write(void* ptr, Value const& value) const override
    {
        typename TFieldTraits::TFieldType                                         fieldValue;
        typename Stencil::TypeTraits<typename TFieldTraits::TFieldType&>::Handler handler;
        handler.Write(&fieldValue, value);
        (static_cast<TOwner*>(ptr)->*(TFieldTraits::TPropertySetter()))(std::move(fieldValue));
    }

    Value Read(void* ptr) const override
    {
        auto method = TFieldTraits::TPropertyGetter();
        auto obj    = static_cast<TOwner*>(ptr);

        typename Stencil::TypeTraits<typename TFieldTraits::TFieldType&>::Handler handler;
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
        auto info   = IDataTypeHandlerValue::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
};

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::List, TFieldTraits> : public IDataTypeHandlerList
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    virtual void Start() const override {}
    virtual void End() const override {}

    virtual SubComponent MoveNext(void* rawptr) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        return _handler.MoveNext(reinterpret_cast<void*>(&obj));
    }

    bool TryGetAt(void* rawptr, size_t index, SubComponent& subcomponent) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        return _handler.TryGetAt(&obj, index, subcomponent);
    }

    SubComponent GetOrCreateAt(void* rawptr, size_t index) const override
    {
        auto  structptr = static_cast<TOwner*>(rawptr);
        auto& obj       = (structptr->*(TFieldTraits::TPropertyGetter()))();
        return _handler.GetOrCreateAt(&obj, index);
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
        auto info   = IDataTypeHandlerList::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
    virtual SubComponent GetListItemHandler() const override { return {&_handler, nullptr}; }

    typename FieldTypeTraits::Handler _handler;
};

template <typename TFieldTraits> struct ObjectDataTypeHandler<DataType::Object, TFieldTraits> : public IDataTypeHandlerObject
{
    using TOwner          = typename TFieldTraits::TOwner;
    using TFieldType      = std::invoke_result_t<decltype(TFieldTraits::TPropertyGetter()), std::unique_ptr<TOwner>>;
    using FieldTypeTraits = TypeTraits<TFieldType&>;

    virtual void Start() const override {}
    virtual void End() const override {}
    virtual bool TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const override
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
        auto info   = IDataTypeHandlerObject::GetDataInfo();
        info->flags = TFieldTraits::Flags();
        return info;
    }
    using FieldHandler = typename FieldTypeTraits::Handler;

    FieldHandler _handler;
};

struct ReflectedStruct
{};

template <typename TStruct, typename... TFieldTraits> struct ReflectedStructHandler : public IDataTypeHandlerObject
{
    void Start() const override {}
    void End() const override {}

    template <typename T> struct FieldTraitsHelper
    {
        using TFieldType      = std::invoke_result_t<decltype(T::TPropertyGetter()), std::unique_ptr<TStruct>>;
        using FieldTypeTraits = TypeTraits<TFieldType&>;
    };

    template <typename Trait> struct Handler : public ObjectDataTypeHandler<FieldTraitsHelper<Trait>::FieldTypeTraits::Type(), Trait>
    {};

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

        if constexpr (sizeof...(Traits) != 0) { return _FindMatch<Traits...>(key, ptr, s); }
        else
        {
            return false;
        }
    }

    bool TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const override
    {
        auto ptr = static_cast<TStruct*>(rawptr);
        if (key.GetType() == Value::Type::Unsigned) { return _FindMatch<TFieldTraits...>(key.convert<size_t>(), ptr, subcomponent); }

        return _FindMatch<TFieldTraits...>(key.convert<shared_string>().c_str(), ptr, subcomponent);
    }

    virtual shared_string Description() const override { return nullptr; }
    virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { return nullptr; }
    virtual shared_string Name() const override { return shared_string::make(TypeTraits<TStruct&>::Name()); }
    virtual size_t        GetSubComponentCount() const override { return sizeof...(TFieldTraits); }
    virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
    {
        SubComponent sub;
        if (!_FindMatch<TFieldTraits...>(index, nullptr, sub)) { throw Exception::Exception(); }
        sub.ptr = rawptr;
        return sub;
    }

    private:
    std::tuple<Handler<TFieldTraits>...> _handlers;
};

template <typename TVariant, typename TTypeTrait, typename... TFieldTraits> struct ReflectedVariantHandler : public IDataTypeHandlerVariant
{
    void Start() const override {}
    void End() const override {}

    template <typename Trait> struct Handler : public ObjectDataTypeHandler<Trait::Type(), Trait>
    {};

    virtual VariantTypeSubComponent GetVariantTypesHandler(void* rawptr) const override
    {
        auto ptr = static_cast<TVariant*>(rawptr);

        VariantTypeSubComponent s;
        s.name             = shared_string::make("AllowedType");
        s.ptr              = ptr;
        s.uniontypehandler = &_typehandler;
        return s;
    }

    virtual VariantSubComponent GetActiveVariantHandler(void* rawptr) const override
    {
        auto   ptr        = static_cast<TVariant*>(rawptr);
        size_t activetype = static_cast<size_t>(ptr->Type());
        if (activetype == 0)    // Enum type 0 is reserved to be unset
        {
            throw Exception::Exception();
        }
        auto s = _structhandler.GetSubComponentAt(rawptr, (activetype - 1));
        return VariantSubComponent{s.keyname, s.handler, s.ptr};
    }

    virtual shared_string Description() const override { return AttributeValue("Description"); }
    virtual shared_string AttributeValue(const std::string_view& key) const override
    {
        return shared_string::make(TypeTraits<TVariant&>::AttributeValue(key));
    }
    virtual shared_string      Name() const override { return shared_string::make(TypeTraits<TVariant&>::Name()); }
    virtual std::vector<Value> AcceptableTypeNames() const override { return _typehandler.AcceptableValues(); }

    virtual std::vector<VariantSubComponent> SubComponentHandlers() const override
    {
        std::vector<VariantSubComponent> comps;
        for (size_t i = 0; i < _structhandler.GetSubComponentCount(); i++)
        {
            VariantSubComponent u;
            auto                s = _structhandler.GetSubComponentAt(nullptr, i);
            u.name                = s.keyname;
            u.handler             = s.handler;
            u.ptr                 = s.ptr;

            comps.push_back(u);
        }
        return comps;
    }

    private:
    ::ReflectionServices::EnumHandler<TTypeTrait>     _typehandler;
    ReflectedStructHandler<TVariant, TFieldTraits...> _structhandler;
};

template <typename T, typename TStackData> struct StateTraker
{
    enum Mode
    {
        Default,
        List,
        Obj,
        Enum,
        Variant,
        VariantChild,
        ObjKeyValue,
        ListValue
    };
    StateTraker(T* ptr, TStackData&& data)
    {
        _stack.push_back(StateStack{&_rootHandler, reinterpret_cast<void*>(ptr), Mode::Obj, std::move(data)});
    }

    DataType GetDataTypeHint() const { return _stack.back().Handler->GetDataType(); }

    const IDataTypeHandlerUnknown* GetHandler() const { return _stack.back().Handler; }

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
        case DataType::Variant: break;
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

    void VariantType(Value const& val, TStackData&& data)
    {
        _Start(Mode::Variant, std::move(data));
        auto handler = _stack.back().Handler->VariantHandler();
        assert(handler != nullptr);
        auto typecomp = handler->GetVariantTypesHandler(_stack.back().Ptr);
        typecomp.uniontypehandler->Write(typecomp.ptr, val);
        auto activecomp = handler->GetActiveVariantHandler(_stack.back().Ptr);
        _stack.push_back(StateStack{activecomp.handler, activecomp.ptr, Mode::VariantChild, std::move(data)});
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
        if (_stack.back().mode == Mode::ObjKeyValue) { _stack.pop_back(); }
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
        if (!TryObjKey(key, std::move(data))) { throw Exception::Exception(); }
    }

    bool TryObjKey(Value key, TStackData&& data)
    {
        IDataTypeHandlerObject::SubComponent subcomponent;
        if (!_stack.back().Handler->CastToObject()->TryGetSubComponent(_stack.back().Ptr, key, subcomponent)) { return false; }
        _stack.push_back(StateStack{subcomponent.handler, subcomponent.ptr, Mode::ObjKeyValue, std::move(data)});
        return true;
    }

    struct StateStack
    {
        const IDataTypeHandlerUnknown* Handler = nullptr;
        void*                          Ptr     = nullptr;
        Mode                           mode;
        TStackData                     data;
    };

    private:
    typename TypeTraits<T>::Handler _rootHandler;
    std::vector<StateStack>         _stack;
};
}    // namespace ReflectionServices

namespace ReflectionServices
{
using namespace Stencil;

template <typename... TApiTraits> struct InterfaceHandler
{};

}    // namespace ReflectionServices
#endif

template <Stencil::FixedWidthValueConcept T> struct Stencil::TypeTraits<T>
{
    // static constexpr DataType         Type() { return DataType::Value; }
    static constexpr std::string_view Name() { return std::string_view(typeid(T).name()); }
    static constexpr std::string_view Description() { return Name(); }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    using ValueType = T;
    // using Handler   = typename ::ReflectionServices::CommonValueHandler<T>;
};

template <typename T> struct Stencil::TypeTraits<std::vector<T>&>
{
    using ListObjType = T;
    // static constexpr DataType Type() { return DataType::List; }
    static std::string      Name() { return "list<" + std::string(::Stencil::TypeTraits<T&>::Name()) + ">"; }
    static std::string      Description() { return "list<" + std::string(::Stencil::TypeTraits<T&>::AttributeValue("Description")) + ">"; }
    static std::string_view AttributeValue(const std::string_view& /*key*/) { return ""; }

    bool AreEqual(std::vector<T>& obj1, std::vector<T>& obj2)
    {
        return std::equal(obj1.begin(), obj2.end(), obj2.begin(), obj2.end(), [](auto&& o1, auto&& o2) {
            return Stencil::TypeTraits<decltype(o1)>::AreEqual(o1, o2);
        });
    }

    // using Handler = typename ::ReflectionServices::StdVectorListHandler<T>;
};

template <typename T, size_t N> struct Stencil::TypeTraits<std::array<T, N>&>
{
    using ListObjType = T;

    // static constexpr DataType Type() { return DataType::List; }
    static std::string Name() { return "array<" + std::string(::Stencil::TypeTraits<T&>::Name()) + ">"; }

    static std::string      Description() { return "array<" + std::string(::Stencil::TypeTraits<T&>::AttributeValue("Description")) + ">"; }
    static std::string_view AttributeValue(const std::string_view& /* key */) { return ""; }

    bool AreEqual(std::array<T, N>& obj1, std::array<T, N>& obj2)
    {
        return std::equal(obj1.begin(), obj2.end(), obj2.begin(), obj2.end(), [](auto&& o1, auto&& o2) {
            return Stencil::TypeTraits<decltype(o1)>::AreEqual(o1, o2);
        });
    }

    // using Handler = typename ::ReflectionServices::StdArrayListHandler<T, N>;
};

template <typename T> struct Stencil::TypeTraits<std::unique_ptr<T>>
{
    static constexpr auto Type() { return Stencil::TypeTraits<T>::Type(); }
    static constexpr bool IsPrimitive() { return Stencil::TypeTraits<T>::IsPrimitive(); }
    static constexpr bool IsIterable() { return Stencil::TypeTraits<T>::IsIterable(); }
    static constexpr bool IsIndexable() { return Stencil::TypeTraits<T>::IsIndexable(); }

    static constexpr std::string_view Name() { return ::Stencil::TypeTraits<T>::Name(); }
    static std::string_view           AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }
    static bool                       AreEqual(std::unique_ptr<T> const& obj1, std::unique_ptr<T> const& obj2)
    {
        return Stencil::TypeTraits<T>::AreEqual(*obj1, *obj2);
    }

#if 0

    struct Handler : public ::Stencil::IDataTypeHandlerObject
    {
        virtual void Start() const override {}
        virtual bool TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const override
        {
            auto& obj = *static_cast<std::unique_ptr<T>*>(rawptr);
            if (obj == nullptr) { obj.reset(new T()); }
            return _handler.TryGetSubComponent(obj.get(), key, subcomponent);
        }
        virtual void End() const override {}

        virtual shared_string Description() const override { return AttributeValue("Description"); }
        virtual shared_string AttributeValue(const std::string_view& key) const override
        {
            return shared_string::make(::Stencil::TypeTraits<T&>::AttributeValue(key));
        }
        virtual shared_string Name() const override { return shared_string::make(::Stencil::TypeTraits<T&>::Name()); }
        virtual size_t        GetSubComponentCount() const override { return _handler.GetSubComponentCount(); }
        virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
        {
            return _handler.GetSubComponentAt(rawptr, index);
        }

        typename ::Stencil::TypeTraits<T&>::Handler _handler;
    };
#endif
};

template <typename T> struct Stencil::TypeTraits<std::shared_ptr<T>>
{
    static constexpr auto Type() { return Stencil::TypeTraits<T>::Type(); }
    static constexpr bool IsAtomic() { return Stencil::TypeTraits<T>::IsAtomic(); }
    static constexpr bool IsIterable() { return Stencil::TypeTraits<T>::IsIterable(); }
    static constexpr bool IsIndexable() { return Stencil::TypeTraits<T>::IsIndexable(); }

    static constexpr std::string_view Name() { return ::Stencil::TypeTraits<T>::Name(); }
    static std::string_view           AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }
    static bool                       AreEqual(std::shared_ptr<T> const& obj1, std::shared_ptr<T> const& obj2)
    {
        return Stencil::TypeTraits<T>::AreEqual(*obj1, *obj2);
    }

#if 0
    struct Handler : public ::Stencil::IDataTypeHandlerObject
    {
        virtual void Start() const override {}
        virtual bool TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const override
        {
            auto& obj = *static_cast<std::shared_ptr<T>*>(rawptr);
            if (obj == nullptr) { obj.reset(new T()); }
            return _handler.TryGetSubComponent(obj.get(), key, subcomponent);
        }
        virtual void End() const override {}

        virtual shared_string Description() const override { return AttributeValue("Description"); }
        virtual shared_string AttributeValue(const std::string_view& key) const override
        {
            return shared_string::make(::Stencil::TypeTraits<T&>::AttributeValue(key));
        }
        virtual shared_string Name() const override { return shared_string::make(::Stencil::TypeTraits<T&>::Name()); }
        virtual size_t        GetSubComponentCount() const override { return _handler.GetSubComponentCount(); }
        virtual SubComponent  GetSubComponentAt(void* rawptr, size_t index) const override
        {
            return _handler.GetSubComponentAt(rawptr, index);
        }

        typename ::Stencil::TypeTraits<T&>::Handler _handler;
    };
#endif
};
#if 0
template <typename T> struct Stencil::TypeTraits<shared_tree<T>&>
{
    using ListObjType = T;

    static constexpr DataType         Type() { return DataType::List; }
    static constexpr std::string_view Name() { return ::Stencil::TypeTraits<T&>::Name(); }
    static std::string                Description()
    {
        return "shared_tree<" + std::string(::Stencil::TypeTraits<T&>::AttributeValue("Description")) + ">";
    }

    static std::string_view AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }

    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    struct Handler : public ::Stencil::IDataTypeHandlerList
    {
        virtual void Start() const override {}

        SubComponent MoveNext(void* /*ptr*/) const override { throw std::logic_error("TODO"); }

        bool TryGetAt(void* /*ptr*/, size_t /*index*/, SubComponent& /*subcomponent*/) const override { throw std::logic_error("TODO"); }
        SubComponent GetOrCreateAt(void* /*ptr*/, size_t /*index*/) const override { throw std::logic_error("TODO"); }

        virtual void End() const override {}

        virtual shared_string Description() const override { throw std::logic_error("TODO"); }
        virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
        virtual shared_string Name() const override { throw std::logic_error("TODO"); }

        virtual SubComponent GetListItemHandler() const override { return {&_handler, nullptr}; }

        typename ::Stencil::TypeTraits<T&>::Handler _handler;
    };
};
#endif

#if 0
template <typename T> struct Stencil::TypeTraits<UuidBasedId<T>&>
{
    static constexpr DataType         Type() { return DataType::Value; }
    static constexpr std::string_view Name() { return ::Stencil::TypeTraits<T&>::Name(); }
    static std::string_view           AttributeValue(const std::string_view& /*key*/) { throw std::logic_error("TODO"); }
    static std::string                Description() { throw std::logic_error("TODO"); }

    template <typename T1, typename T2> static bool AreEqual(T1 const& obj1, T2 const& obj2) { return obj1 == obj2; }

    struct Handler : public ::Stencil::IDataTypeHandlerValue
    {
        virtual shared_string Description() const override { throw std::logic_error("TODO"); }
        virtual shared_string AttributeValue(const std::string_view& /*key*/) const override { throw std::logic_error("TODO"); }
        virtual shared_string Name() const override { return shared_string::make(::Stencil::TypeTraits<T&>::Name()); }

        virtual void  Write(void* /*ptr*/, Value const& /*value*/) const override { throw std::logic_error("TODO"); }
        virtual Value Read(void* /*ptr*/) const override { throw std::logic_error("TODO"); }

        typename ::Stencil::TypeTraits<T&>::Handler _handler;
    };
};
#endif
