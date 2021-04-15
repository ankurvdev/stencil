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
#pragma once
#include <stencil/stencil.h>
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
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        //<Field Join=','>
        zzField_Namezz
        //</Field>
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               //<Field >
               + 1u
            //</Field>
            ;
    }

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

        case FieldIndex::Invalid: break;

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
    void                            zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }
    zzFieldType_NativeTypezz&       get_zzNamezz() { return _zzNamezz; }

    bool isset_zzNamezz() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::zzField_Namezz); }

    void set_zzNamezz(zzFieldType_NativeTypezz&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::zzField_Namezz, _zzNamezz, val);
        _zzNamezz = std::move(val);
    }

    //<FieldType_Mutator>
    zzReturnType_NativeTypezz zzNamezz_zzField_Namezz(zzArg_NativeTypezz&& args)
    {
        return Stencil::Mutators<zzField_FieldType_NativeTypezz>::zzNamezz(_zzField_Namezz, std::move(args));
    }
    //</FieldType_Mutator>

    //<FieldType_Accessor>
    zzReturnType_NativeTypezz zzNamezz_zzField_Namezz(zzArg_NativeTypezz const& args) const
    {
        return Stencil::Accessors<zzField_FieldType_NativeTypezz>::zzNamezz(_zzField_Namezz, args);
    }
    //</FieldType_Accessor>
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
    void       set_Type(UnionType&& val) { _type = std::move(val); }

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

        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    //<FieldAttribute>
    template <FieldIndex TFieldType> static constexpr std::string_view FieldAttributeValue_zzFieldAttribute_Namezz();
    //</FieldAttribute>

    //<Field>

    private:
    zzFieldType_NativeTypezz _zzNamezz;

    public:
    zzFieldType_NativeTypezz&       zzNamezz() { return _zzNamezz; }
    const zzFieldType_NativeTypezz& zzNamezz() const { return _zzNamezz; }
    void                            zzNamezz(const zzFieldType_NativeTypezz& val) { _zzNamezz = val; }
    void                            zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    zzFieldType_NativeTypezz& get_zzNamezz() { return _zzNamezz; }
    void                      set_zzNamezz(zzFieldType_NativeTypezz&& val) { _zzNamezz = std::move(val); }

    //</Field>
};
}    // namespace zzUnion_Namezz
//</Union>

//<Interface>

struct zzInterface_Namezz : public ReflectionBase::Interface<zzInterface_Namezz>
{
    public:
    zzInterface_Namezz()          = default;
    virtual ~zzInterface_Namezz() = default;
    DELETE_COPY_AND_MOVE(zzInterface_Namezz);
    //<Function>
    virtual zzReturnType_NativeTypezz zzFunction_Namezz(
        //<Args_Field Join=','>
        zzFieldType_NativeTypezz const& zzNamezz
        //</Args_Field>
        )
        = 0;
    //</Function>
    // static std::unique_ptr<zzInterface_Namezz> Create();
};

struct zzInterface_NamezzFactory : public ReflectionBase::InterfaceFactory<zzInterface_Namezz>
{
    public:
    virtual std::unique_ptr<zzInterface_Namezz> Activate() = 0;
    virtual ~zzInterface_NamezzFactory()                   = default;
};

//<Function>
struct zzInterface_Namezz_zzFunction_Namezz_Args
{
    zzInterface_Namezz* instance = nullptr;

    //<Args_Field>
    zzFieldType_NativeTypezz        arg_zzNamezz{};
    zzFieldType_NativeTypezz&       get_arg_zzNamezz() { return arg_zzNamezz; }
    zzFieldType_NativeTypezz const& get_carg_zzNamezz() const { return arg_zzNamezz; }
    void                            set_arg_zzNamezz(zzFieldType_NativeTypezz&& value) { arg_zzNamezz = std::move(value); }
    //</Args_Field>
};

//</Function>

//</Interface>
}    // namespace zzProgram_Namezz

//<Interface>
//<Function>
template <> struct ReflectionBase::TypeTraits<zzProgram_Namezz::zzInterface_Namezz_zzFunction_Namezz_Args&>
{
    //<Args_Field>

    struct Traits_arg_zzNamezz
    {
        using TOwner     = zzProgram_Namezz::zzInterface_Namezz_zzFunction_Namezz_Args;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view    Name() { return "zzNamezz"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_zzNamezz; }
    };
    //</Args_Field>

    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "zzFunction_Namezz"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = zzProgram_Namezz::zzInterface_Namezz_zzFunction_Namezz_Args;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Args_Field>
               && ReflectionBase::AreEqual(obj1.get_carg_zzNamezz(), obj2.get_carg_zzNamezz())
            //</Args_Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzInterface_Namezz_zzFunction_Namezz_Args
                                                                 //<Args_Field>
                                                                 ,
                                                                 Traits_arg_zzNamezz
                                                                 //</Args_Field>
                                                                 >;
};
//</Function>

template <> struct ReflectionBase::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>
{
    //<Function>
    struct ApiTraits_zzNamezz
    {
        using TOwner = zzProgram_Namezz::zzInterface_Namezz;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "zzNamezz"; }
        static constexpr bool                Static = false;
    };
    //</Function>

    using Apis = ::ReflectionBase::InterfaceApiPack<
        //<Function  Join=','>
        ApiTraits_zzNamezz
        //</Function>
        >;
};

//<Function>

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>::ApiTraits_zzNamezz>
{
    using ArgsStruct = zzProgram_Namezz::zzInterface_Namezz_zzFunction_Namezz_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "zzNamezz"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->zzNamezz(
            //<Args_Field Join=','>
            args.get_arg_zzNamezz()
            //</Args_Field>
        );
    }
};

//</Function>
#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct WebServiceHandlerTraits<zzProgram_Namezz::zzInterface_Namezz>
{
    static constexpr const std::string_view Url() { return std::string_view("zzInterface_Namezz"); }
};
#endif

//</Interface>

//<Struct>

template <> struct ReflectionBase::TypeTraits<zzProgram_Namezz::zzStruct_Namezz::Data&>
{
    //<Field>

    struct Traits_zzNamezz
    {
        using TOwner     = zzStruct_Program_Namezz::zzStruct_Namezz::Data;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_zzNamezz; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::zzField_Namezz, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{//<If HasDefaultValue='true'>
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

    using ThisType = zzProgram_Namezz::zzStruct_Namezz::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Field>
               && ReflectionBase::AreEqual(obj1.zzNamezz(), obj2.zzNamezz())
            //</Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzStruct_Namezz::Data,
                                                                 //<Field Join=','>
                                                                 Traits_zzNamezz
                                                                 //</Field>
                                                                 >;
};

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, zzProgram_Namezz::zzStruct_Namezz::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    //<Field>
    DeltaTracker<zzFieldType_NativeTypezz> _subtracker_zzNamezz;
    //</Field>

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        //<Field>
        ,
        _subtracker_zzNamezz(&_ptr->zzNamezz())
    //</Field>
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda(_subtracker_zzNamezz); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda(_subtracker_zzNamezz); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    //<Field>

    void set_zzNamezz(zzFieldType_NativeTypezz&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::zzField_Namezz, _ptr->zzNamezz(), val);
        _ptr->set_zzNamezz(std::move(val));
    }

    //<FieldType_Mutator>
    zzReturnType_NativeTypezz zzNamezz_zzField_Namezz(zzArg_NativeTypezz&& args)
    {
        Stencil::ObservablePropsT<TData>::OnMutationRequested(
            *this, TData::FieldIndex::zzField_Namezz, uint8_t{zzIdzz}, _ptr->zzField_Namezz(), args);

        return _ptr->zzNamezz_zzField_Namezz(std::move(args));
    }
    //</FieldType_Mutator>

    //<FieldType_Accessor>
    zzReturnType_NativeTypezz zzNamezz_zzField_Namezz(zzArg_NativeTypezz const& args) const { return _ptr->zzNamezz_zzField_Namezz(args); }
    //</FieldType_Accessor>
    //</Field>
};

//</Struct>
//<Union>

template <> struct ReflectionServices::EnumTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  //<Field>
                                                  "zzField_Namezz",
                                                  //</Field>

                                                  nullptr};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>
{
    static constexpr auto    ValueType() { return Value::Type::Unsigned; }
    [[noreturn]] static void Get(Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Get(const Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Check() { throw std::logic_error("Not Implemented"); }
};

template <> struct ReflectionBase::TypeTraits<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType&>
{
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Value; }
    static constexpr std::string_view           Name() { return "zzUnion_Namezz"; }

    using Handler = ::ReflectionServices::EnumHandler<zzUnion_Program_Namezz::zzUnion_Namezz::UnionType>;
};

template <> struct ReflectionBase::TypeTraits<zzProgram_Namezz::zzUnion_Namezz::Data&>
{
    //<Field>
    struct Traits_zzNamezz
    {
        using TOwner     = zzUnion_Program_Namezz::zzUnion_Namezz::Data;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_zzNamezz; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::zzField_Namezz, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{//<If HasDefaultValue='true'>
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
    static constexpr std::string_view           Name() { return "zzUnion_Namezz"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        //<Attribute>
        if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
        //</Attribute>

        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = zzProgram_Namezz::zzUnion_Namezz::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Field>
               && ReflectionBase::AreEqual(obj1.zzNamezz(), obj2.zzNamezz())
            //</Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedUnionHandler<zzProgram_Namezz::zzUnion_Namezz::Data,
                                                                zzUnion_Program_Namezz::zzUnion_Namezz::UnionType,
                                                                //<Field Join=','>
                                                                Traits_zzNamezz
                                                                //</Field>
                                                                >;
};
//</Union>
//</Template>
