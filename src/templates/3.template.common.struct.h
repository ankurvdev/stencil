// Anything here is not part of the generated code
const int zzFieldType_Idzz = 0;
const int zzStruct_Idzz    = 0;
const int zzVariant_Idzz   = 0;

struct zzFieldType_NativeTypezz
{
    int a;
};
struct zzChildFieldType_NativeTypezz
{
    int a;
};
struct zzReturnType_NativeTypezz
{};

//<Template file="zzFileNamezz.h">
#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace zzProgram_Namezz
{
//<Struct>
struct zzStruct_Namezz;
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzStruct_Namezz>;
//</Struct>

//<Variant>
struct zzVariant_Namezz;
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzVariant_Namezz>;
//</Variant>
}    // namespace zzProgram_Namezz

#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace zzProgram_Namezz
{
//<Typedef>
typedef zzChildFieldType_NativeTypezz zzNamezz;
//</Typedef>

//<Struct>
struct zzStruct_Namezz :
    //<AttributeTag>
    public zzTagType_NativeTypezz,
    //</AttributeTag>
    public Stencil::StructT<zzStruct_Namezz>
{
    //<Field>
    zzFieldType_NativeTypezz zzNamezz = zzInitialValuezz;
    //</Field>
};
//</Struct>

//<Variant>
struct zzVariant_Namezz :
    //<AttributeTag>
    public zzTagType_NativeTypezz,
    //</AttributeTag>
    public Stencil::VariantT<zzStruct_Namezz>
{
    enum class VariantType
    {
        Invalid,
        //<Field Join=','>
        zzField_Namezz
        //</Field>
    };

    VariantType type = VariantType::Invalid;

    //<Field>
    zzFieldType_NativeTypezz zzNamezz = zzInitialValuezz;
    //</Field>
};
//</Variant>

//<Interface>
struct zzInterface_Namezz : public Stencil::InterfaceT<zzInterface_Namezz>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    //<InterfaceObjectStore>
    struct ObjectStore_zzNamezz : zzObjectType_NativeTypezz, Database2::ObjectT<ObjectStore, ObjectStore_zzNamezz>
    {};

    //</InterfaceObjectStore>
    struct ObjectStore : Dummy
        //<InterfaceObjectStore>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_zzNamezz>
    //</InterfaceObjectStore>
    {};

    zzInterface_Namezz()          = default;
    virtual ~zzInterface_Namezz() = default;
    DELETE_COPY_AND_MOVE(zzInterface_Namezz);
    //<InterfaceFunction>
    virtual zzReturnType_NativeTypezz zzInterfaceFunction_Namezz(
        //<Args_Field Join=','>
        zzFieldType_NativeTypezz const& zzNamezz
        //</Args_Field>
        )
        = 0;
    //</InterfaceFunction>

    //<InterfaceFunction>
    struct Args_zzInterfaceFunction_Namezz
    {
        //<Args_Field>
        zzFieldType_NativeTypezz arg_zzNamezz{};
        //</Args_Field>
    };

    zzReturnType_NativeTypezz zzInterfaceFunction_Namezz(Args_zzInterfaceFunction_Namezz const& args)
    {
        return this->zzInterfaceFunction_Namezz(
            //<Args_Field Join=','>
            args.zzNamezz
            //</Args_Field>
        );
    }

    //</InterfaceFunction>
    ObjectStore objects;
};
//</InterfaceFunction>

}    // namespace zzProgram_Namezz
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

//<Struct>

template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzStruct_Namezz>
{
    enum class FieldType
    {
        Invalid = 0,
        //<Field Join=','>
        zzNamezz
        //</Field>
    };

    constexpr size_t    FieldIndexForType(FieldType type) { return static_cast<size_t>(type) - 1; }
    constexpr FieldType FieldTypeForIndex(size_t index) { return static_cast<FieldType>(index) + 1; }

    //<Field>
    struct zzNamezz
    {
        using TStruct    = zzProgram_Namezz::zzStruct_Namezz;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static auto& Get(TStruct const& obj) { return obj.zzNamezz; }

        static TStruct& Set(zzFieldType_NativeTypezz&& val)
        {
            obj.zzNamezz = std::move(val);
            return obj;
        }

        static TStruct& Set(zzFieldType_NativeTypezz const& val)
        {
            obj.zzNamezz = val;
            return obj;
        }

        static constexpr auto Getter() { return &Get; }
        static constexpr auto Setter() { return &Set; }

        static constexpr auto AttributeValue(const std::string_view& key)
        {
            //<Attribute>
            if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
            //</Attribute>
            return "<Unknown>";
        }

        static const ::Stencil::Flags Flags()
        {
            return ::Stencil::Flags{//<If HasDefaultValue='true'>
                                    ::Stencil::Flag::HasDefaultValue,
                                    //</If>
                                    //<If IsOptional='true'>
                                    ::Stencil::Flag::Optional,
                                    //</If>
                                    ::Stencil::Flag::Max};
        }
    };
    //</Field>

    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "zzStruct_Namezz"; }
    static constexpr std::string_view    AttributeValue(const std::string_view& key)
    {
        //<Attribute>
        if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
        //</Attribute>
        return "<Unknown>";
    }

    using ThisType = zzProgram_Namezz::zzStruct_Namezz::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Field>
               && Stencil::AreEqual(obj1.zzNamezz(), obj2.zzNamezz())
            //</Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzStruct_Namezz::Data,
                                                                 //<Field Join=','>
                                                                 zzNamezz
                                                                 //</Field>
                                                                 >;
};

template <> struct Stencil::Transaction<zzProgram_Namezz::zzStruct_Namezz> : Stencil::TransactionT<zzProgram_Namezz::zzStruct_Namezz>
{
    using TData = zzProgram_Namezz::zzStruct_Namezz;

    //<Field>
    Transaction<zzFieldType_NativeTypezz> _subtracker_zzNamezz;
    //</Field>

    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<zzProgram_Namezz::zzStruct_Namezz>(ptr)
        //<Field>
        ,
        _subtracker_zzNamezz(Obj().zzNamezz())
    //</Field>
    {}

    //<Field>
    auto& zzNamezz()
    {
        MarkFieldEdited_(TData::FieldIndex::zzField_Namezz);
        return _subtracker_zzNamezz;
    }
    //</Field>
    //<Field>
    void set_zzNamezz(zzFieldType_NativeTypezz&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::zzNamezz, Obj().zzNamezz(), val);
        Obj().set_zzNamezz(std::move(val));
    }

    //<FieldType_Mutator>
    zzReturnTypezz zzNamezz_zzField_Namezz(zzArgzz&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::zzField_Namezz);
        return Stencil::Mutators<std::remove_reference_t<decltype(zzField_Namezz())>>::zzNamezz(zzField_Namezz(), std::move(args));
    }
    //</FieldType_Mutator>
    //</Field>

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: return lambda("zzNamezz", zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        //<Field>
        if (fieldName == "zzNamezz") { return lambda(TData::FieldIndex::zzNamezz, zzNamezz()); }
        //</Field>
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        //<Field>
        lambda("zzNamezz", TData::FieldIndex::zzNamezz, zzNamezz(), Obj().zzNamezz());
        //</Field>
    }

    void Flush()
    {
        //<Field>
        zzNamezz().Flush();

        if (IsFieldEdited(TData::FieldIndex::zzNamezz))
        {
            if (!zzNamezz().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::zzNamezz));
        }

        //</Field>

        Stencil::TransactionT<zzProgram_Namezz::zzStruct_Namezz::Data>::Flush_();
    }
};

template <> struct Stencil::Visitor<zzProgram_Namezz::zzStruct_Namezz, void> : Stencil::VisitorT<zzProgram_Namezz::zzStruct_Namezz>
{
    using TData = zzProgram_Namezz::zzStruct_Namezz;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda("zzNamezz", _ref.get().zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda("zzNamezz", _ref.get().zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        //<Field>
        lambda("zzNamezz", _ref.get().zzNamezz());
        //</Field>
    }

    std::reference_wrapper<TData> _ref;
};

//</Struct>

//<Interface>
//<InterfaceFunction>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args&>
{
    //<Args_Field>

    struct Traits_arg_zzNamezz
    {
        using TOwner     = zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view Name() { return "zzNamezz"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_zzNamezz; }
    };
    //</Args_Field>

    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "zzInterfaceFunction_Namezz"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Args_Field>
               && Stencil::AreEqual(obj1.get_carg_zzNamezz(), obj2.get_carg_zzNamezz())
            //</Args_Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args
                                                                 //<Args_Field>
                                                                 ,
                                                                 Traits_arg_zzNamezz
                                                                 //</Args_Field>
                                                                 >;
};
//</InterfaceFunction>

template <> struct Stencil::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>
{
    //<InterfaceFunction>
    struct ApiTraits_zzNamezz
    {
        using TOwner = zzProgram_Namezz::zzInterface_Namezz;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "zzNamezz"; }
        static constexpr bool             Static = false;
    };
    //</InterfaceFunction>

    using Apis = std::tuple<
        //<InterfaceFunction  Join=','>
        ApiTraits_zzNamezz
        //</InterfaceFunction>
        >;

    using Objects = std::tuple<
        //<InterfaceObjectStore Join=','>
        zzProgram_Namezz::zzInterface_Namezz::ObjectStore_zzNamezz
        //</InterfaceObjectStore>
        >;
};

//<InterfaceObjectStore>
template <> struct Stencil::TypeTraits<zzObjectType_NativeTypezz>;
template <>
struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz::ObjectStore_zzNamezz&>
    : public Stencil::TypeTraits<zzObjectType_NativeTypezz&>
{};
//</InterfaceObjectStore>

//<InterfaceFunction>

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>::ApiTraits_zzNamezz>
{
    using ArgsStruct = zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args;
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

//</InterfaceFunction>

//<InterfaceObjectStore>
template <> struct Stencil::InterfaceObjectTraits<zzProgram_Namezz::zzInterface_Namezz::ObjectStore_zzNamezz>
{
    static constexpr std::string_view Name() { return "zzNamezz"; }
};
//</InterfaceObjectStore>

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<zzProgram_Namezz::zzInterface_Namezz>
{
    static constexpr const std::string_view Url() { return std::string_view("zzInterface_Namezz"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(zzProgram_Namezz::zzInterface_Namezz& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

//</Interface>

//<Variant>

template <> struct ReflectionServices::EnumTraits<zzVariant_Program_Namezz::zzVariant_Namezz::VariantType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  //<Field>
                                                  "zzField_Namezz",
                                                  //</Field>

                                                  nullptr};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<zzVariant_Program_Namezz::zzVariant_Namezz::VariantType>
{
    static constexpr auto    ValueType() { return Value::Type::Unsigned; }
    [[noreturn]] static void Get(Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Get(const Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Check() { throw std::logic_error("Not Implemented"); }
};

template <> struct Stencil::TypeTraits<zzVariant_Program_Namezz::zzVariant_Namezz::VariantType&>
{
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Value; }
    static constexpr std::string_view    Name() { return "zzVariant_Namezz"; }

    using Handler = ::ReflectionServices::EnumHandler<zzVariant_Program_Namezz::zzVariant_Namezz::VariantType>;
};

template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzVariant_Namezz&>
{
    //<Field>
    struct Traits_zzNamezz
    {
        using TOwner     = zzVariant_Program_Namezz::zzVariant_Namezz::Data;
        using TFieldType = zzFieldType_NativeTypezz;

        static constexpr std::string_view Name() { return "zzNamezz"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_zzNamezz; }
        static constexpr auto TPropertySetter() { return &TOwner::set_zzNamezz; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::zzField_Namezz, key);
        }

        static const ::Stencil::Flags Flags()
        {
            return ::Stencil::Flags{//<If HasDefaultValue='true'>
                                    ::Stencil::Flag::HasDefaultValue,
                                    //</If>
                                    //<If IsOptional='true'>
                                    ::Stencil::Flag::Optional,
                                    //</If>
                                    ::Stencil::Flag::Max};
        }
    };
    //</Field>

    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "zzVariant_Namezz"; }
    static constexpr std::string_view    AttributeValue(const std::string_view& key)
    {
        //<Attribute>
        if (key == "zzAttribute_Keyzz") return "zzAttribute_Valuezz";
        //</Attribute>

        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = zzProgram_Namezz::zzVariant_Namezz::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Field>
               && Stencil::AreEqual(obj1.zzNamezz(), obj2.zzNamezz())
            //</Field>
            ;
    }

    using Handler = ::ReflectionServices::ReflectedVariantHandler<zzProgram_Namezz::zzVariant_Namezz::Data,
                                                                  zzVariant_Program_Namezz::zzVariant_Namezz::VariantType,
                                                                  //<Field Join=','>
                                                                  Traits_zzNamezz
                                                                  //</Field>
                                                                  >;
};

template <> struct Stencil::Visitor<zzProgram_Namezz::zzVariant_Namezz, void> : Stencil::VisitorT<zzProgram_Namezz::zzVariant_Namezz::Data>
{
    using TData = zzProgram_Namezz::zzVariant_Namezz::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda("zzNamezz", _ref.get().zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda("zzNamezz", _ref.get().zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        //<Field>
        lambda("zzNamezz", _ref.get().zzNamezz());
        //</Field>
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const zzProgram_Namezz::zzVariant_Namezz, void> : Stencil::VisitorT<const zzProgram_Namezz::zzVariant_Namezz>
{
    using TData = zzProgram_Namezz::zzVariant_Namezz const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        //<Field>
        case TData::FieldIndex::zzNamezz: lambda("zzNamezz", _ref.get().zzNamezz()); return;
        //</Field>
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        //<Field>
        lambda("zzNamezz", _ref.get().zzNamezz());
        //</Field>
    }

    std::reference_wrapper<TData> _ref;
};

//</Variant>
#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
//</Template>
