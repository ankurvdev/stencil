// Anything here is not part of the generated code
struct ModelDefinition
{
};
const int         EnumKeyCount     = 0;
const int         EnumCount        = 0;
const int         zzFieldType_Idzz = 0;
const int         zzStruct_Idzz    = 0;
const int         zzUnion_Idzz     = 0;
const HandlerData zzFieldType_NativeHandlerDatazz{nullptr, nullptr, nullptr};
int               resolve_enum(const char** names, int count, const char* name)
{
    return 0;
}
struct zzFieldType_NativeTypezz
{
    int a;
};
struct zzChildFieldType_NativeTypezz
{
    int a;
};
struct zzReturnType_NativeTypezz
{
};
typedef void* Target;
class NativeHandler
{
    public:
    static const ITypeHandler* Make(const ModelDefinition* def, const TypeHandlerFactoryData handlerData) { return (ITypeHandler*)nullptr; }
};
typedef NativeHandler zzFieldType_NativeHandlerzz;
typedef NativeHandler zzReturnType_NativeHandlerzz;

namespace zzProgram_Namezz
{
namespace zzDataSource_Namezz
{
void* GetExtensionData()
{
    return nullptr;
}
}    // namespace zzDataSource_Namezz
}    // namespace zzProgram_Namezz

//<Template file="zzFileNamezz.h">
//<![CDATA[
#pragma once
#include <DataModel.h>
//]]>
namespace zzProgram_Namezz
{
//<Struct>
namespace zzStruct_Namezz
{
struct Data;
}
//</Struct>
//<Typedef>
typedef zzChildFieldType_NativeTypezz zzNamezz;
//</Typedef>
//<Struct>
namespace zzStruct_Namezz
{

struct Data :
    //<RelationshipTag>
    public zzTagType_NativeTypezz,
    //</RelationshipTag>
    public ReflectionBase::ObjMarker
{
    //<![CDATA[
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/
    //]]>

    enum class FieldIndex
    {
        Invalid,
        //<Field Join=','>
        zzField_Namezz
        //</Field>
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
            //<Field>
        case FieldIndex::zzField_Namezz:
            //<Attribute>
            if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
            //</Attribute>
            return ::ReflectionServices::EmptyAttributeValue(key);
            //</Field>
        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    //<Field>
    private:
    zzFieldType_NativeTypezz _zzNamezz = zzInitialValuezz;

    public:
    zzFieldType_NativeTypezz&       zzNamezz() { return _zzNamezz; }
    const zzFieldType_NativeTypezz& zzNamezz() const { return _zzNamezz; }
    //void                            zzNamezz(const zzFieldType_NativeTypezz& val) { _zzNamezz = val; }
    void                            zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    zzFieldType_NativeTypezz& get_zzNamezz()
    {
        return _zzNamezz;
        ;
    }
    void set_zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    //</Field>
};

}    // namespace zzStruct_Namezz
//</Struct>

//<Union>
namespace zzUnion_Namezz
{
struct Union;
}
//</Union>
//<Typedef>
typedef zzChildFieldType_NativeTypezz zzNamezz;
//</Typedef>
//<Union>
namespace zzUnion_Namezz
{
enum class UnionType
{
    Invalid,
    //<Field Join=','>
    zzField_Namezz
    //</Field>
};

struct Data : public ReflectionBase::ObjMarker
{
    UnionType _type;

    public:
    UnionType Type() const { return _type; }

    UnionType& get_Type() { return _type; }
    void       set_Type(UnionType&& val) { _type = (UnionType)std::move(val); }

    Data() : _type(UnionType::Invalid) {}

    public:
    enum class FieldIndex
    {
        Invalid,
        //<Field Join=','>
        zzField_Namezz
        //</Field>
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        //<Field>
        case FieldIndex::zzField_Namezz:
        {
            //<Attribute>
            if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
            //</Attribute>
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
            //</Field>

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    //<FieldAttribute><![CDATA[
    template <FieldIndex TFieldType> static constexpr std::string_view FieldAttributeValue_zzFieldAttribute_Namezz();
    //]]></FieldAttribute>

    //<Field>

    private:
    zzFieldType_NativeTypezz _zzNamezz;

    public:
    zzFieldType_NativeTypezz&       zzNamezz() { return _zzNamezz; }
    const zzFieldType_NativeTypezz& zzNamezz() const { return _zzNamezz; }
    void                            zzNamezz(const zzFieldType_NativeTypezz& val) { _zzNamezz = val; }
    void                            zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    zzFieldType_NativeTypezz& get_zzNamezz()
    {
        return _zzNamezz;
        ;
    }
    void set_zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    //</Field>
};
}    // namespace zzUnion_Namezz
//</Union>

//<Interface>
#if 0
namespace zzInterface_Namezz
{
InterfaceDefinition * GetInterfaceDefinition();

class Interface
{
public:
    //<Function>
    virtual zzReturnType_NativeTypezz zzFunction_Namezz(
        //<Args_Field Join=','>
        zzFieldType_NativeTypezz zzNamezz
        //</Args_Field>
    ) = 0;
    //</Function>
};

namespace InterfaceBroker
{
//<Function>
namespace zzFunction_Namezz
{
struct Data {
public:
    static const ModelDefinition * GetModelDefinition();
    //<Args_Field>
    zzFieldType_NativeTypezz m_zzNamezz;
    //</Args_Field>
public:
    //<Args_Field>
    zzFieldType_NativeTypezz & get_zzNamezz() { return this->m_zzNamezz; }
    void set_zzNamezz(zzFieldType_NativeTypezz &val) { this->m_zzNamezz = val; }
    //</Args_Field>

protected:
    //<Args_Field>
    static Target s_get_target_zzNamezz(Target ptr) { return &(((Data*)ptr)->m_zzNamezz); }
    //</Args_Field>

};

const InterfaceApiDefinition GetInterfaceApiDefinition();
}
//</Function>
};

}
#endif
//</Interface>
}    // namespace zzProgram_Namezz

//<Struct>

//<![CDATA[
template <> struct ReflectionBase::TypeTraits<zzProgram_Namezz::zzStruct_Namezz::Data&>
{
    //]]>

    //<Field><![CDATA[

    struct Traits_zzNamezz
    {
        using TOwner = zzStruct_Program_Namezz::zzStruct_Namezz::Data;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_zzNamezz; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::zzField_Namezz, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{//]]><If HasDefaultValue='true'>
                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           //</If>
                                           //<If IsOptional='true'>
                                           ::ReflectionBase::Flag::Optional,
                                           //</If>
                                           ::ReflectionBase::Flag::Max};
        }
    };
    //</Field>

    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "zzStruct_Namezz"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        //<Attribute>
        if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
        //</Attribute>
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    //<![CDATA[
    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzStruct_Namezz::Data,
                                                                 //]]>
                                                                 //<Field Join=','>
                                                                 Traits_zzNamezz
                                                                 //</Field><![CDATA[
                                                                 >;
    //]]>
};
//</Struct>

//<Union>

//<![CDATA[

template <> struct ReflectionServices::EnumTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  //]]>
                                                  //<Field>
                                                  "zzField_Namezz",
                                                  //</Field>
                                                  //<![CDATA[

                                                  0};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>
{
    static constexpr auto ValueType() { return Value::Type::Integer; }
    static void           Get(Value& /*obj*/) { throw 1; }
    static void           Get(const Value& /*obj*/) { throw 1; }
    static void           Check() { throw 1; }
};

template <> struct ReflectionBase::TypeTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType&>
{
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Value; }
    static constexpr std::string_view           Name() { return "zzUnion_Namezz"; }

    using Handler = ::ReflectionServices::EnumHandler<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>;
};

template <> struct ReflectionBase::TypeTraits<zzProgram_Namezz::zzUnion_Namezz::Data&>
{
    //]]>
    //<Field><![CDATA[
    struct Traits_zzNamezz
    {
        using TOwner = zzUnion_Program_Namezz::zzUnion_Namezz::Data;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_zzNamezz; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::zzField_Namezz, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{//]]><If HasDefaultValue='true'>
                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           //</If>
                                           //<If IsOptional='true'>
                                           ::ReflectionBase::Flag::Optional,
                                           //</If>
                                           ::ReflectionBase::Flag::Max};
        }
    };
    //]]></Field>

    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "zzUnion_Namezz"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        //<Attribute>
        if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
        //</Attribute>

        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    //<![CDATA[
    using Handler = ::ReflectionServices::ReflectedUnionHandler<zzProgram_Namezz::zzUnion_Namezz::Data,
                                                                zzUnion_Program_Namezz::zzUnion_Namezz::UnionType,
                                                                //]]>
                                                                //<Field Join=','>
                                                                Traits_zzNamezz
                                                                //</Field><![CDATA[
                                                                >;
    //]]>
};
//</Union>
//</Template>
