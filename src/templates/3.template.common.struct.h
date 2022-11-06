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
//</Struct>

//<Variant>
struct zzVariant_Namezz;
//</Variant>

//<Interface>
struct zzInterface_Namezz;
//</Interface>
}    // namespace zzProgram_Namezz

//<Struct>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzStruct_Namezz>;
//</Struct>

//<Variant>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzVariant_Namezz>;
//</Variant>

//<Interface>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz>;
//</Interface>

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
struct zzVariant_Namezz : public Stencil::VariantT<zzVariant_Namezz>
{
    std::variant<std::monostate,
                 //<Field Join=','>
                 zzFieldType_NativeTypezz
                 //</Field>
                 >
        _variant;

    enum class VariantType
    {
        Invalid,
        //<Field Join=','>
        zzNamezz
        //</Field>
    };

    VariantType Type() const { return static_cast<VariantType>(_variant.index()); }

    //<Field>
    zzFieldType_NativeTypezz&       zzNamezz() { return std::get<zzFieldType_NativeTypezz>(_variant); }
    zzFieldType_NativeTypezz const& zzNamezz() const { return std::get<zzFieldType_NativeTypezz>(_variant); }
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

    zzReturnType_NativeTypezz zzInterfaceFunction_Namezz(Args_zzInterfaceFunction_Namezz&& args)
    {
        return this->zzInterfaceFunction_Namezz(
            //<Args_Field Join=','>
            std::move(args.zzNamezz)
            //</Args_Field>
        );
    }

    //</InterfaceFunction>
    ObjectStore objects;
};
//</Interface>

}    // namespace zzProgram_Namezz
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

//<Struct>

template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzStruct_Namezz>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>
{
    enum class Fields
    {
        Invalid,
        //<Field Join=','>
        Field_zzNamezz
        //</Field>
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Field Join=','>
        "zzNamezz"
        //</Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<zzProgram_Namezz::zzStruct_Namezz, TContainer>
{
    using Fields = Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        //<Field>
        ElemTxnState zzNamezz = {Fields::Field_zzField_Namezz};
        //</Field>
    };

    using Txn               = Stencil::Transaction<zzProgram_Namezz::zzStruct_Namezz, TContainer>;
    using ElemType          = zzProgram_Namezz::zzStruct_Namezz;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    //<Field>
    using Transaction_zzNamezz = Stencil::Transaction<zzFieldType_NativeTypezz, Txn>;
    //</Field>

    struct TxnState
    {
        std::bitset<64> assigntracker;    // TODO1
        std::bitset<64> edittracker;      // TODO1
        //<Field>
        typename Transaction_zzNamezz::TxnState zzNamezz{};
        //</Field>
    };

    Transaction(TxnState& elemState, ContainerTxnState& containerState, TContainer& container, ElemType& elem) :
        _elemState(elemState), _containerState(containerState), _container(container), _elem(elem)
    {}

    ~Transaction()
    {
        if (IsChanged())
        {
            if constexpr (std::is_base_of_v<Stencil::TimestampedT<ElemType>, ElemType>) { _elem.UpdateTimestamp_(); }
            _container.NotifyElementEdited_(_containerState);
        }
    }

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    ElemType const& Elem() const { return _elem; }

    bool _IsFieldAssigned(Fields key) const { return _elemState.assigntracker.test(static_cast<uint8_t>(key)); }
    bool _IsFieldEdited(Fields key) const { return _elemState.edittracker.test(static_cast<uint8_t>(key)); }
    bool _IsFieldChanged(Fields key) const { return _IsFieldAssigned(key) || _IsFieldEdited(key); }
    void _MarkFieldAssigned(Fields key) { _elemState.assigntracker.set(static_cast<uint8_t>(key)); }
    void _MarkFieldEdited(Fields key) { _elemState.edittracker.set(static_cast<uint8_t>(key)); }

    size_t _CountFieldsChanged() const { return (_elemState.assigntracker | _elemState.edittracker).count(); }

    void NotifyElementAssigned_(ElemTxnState const& elemTxnState)
    {
        _MarkFieldAssigned(elemTxnState.field);
        if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<ElemType>, ElemType>)
        {
            if (!_elem.IsValid(elemTxnState.field))
            {
                _elem.MarkValid(elemTxnState.field);
                return;
            }
        }
    }

    void NotifyElementEdited_(ElemTxnState const& elemTxnState) { _MarkFieldEdited(elemTxnState.field); }

    bool IsChanged() const { return (_elemState.assigntracker | _elemState.edittracker).any(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        //<Field>
        if (_IsFieldAssigned(Fields::Field_zzField_Namezz))
        {
            auto txn = zzNamezz();
            lambda(Fields::Field_zzField_Namezz, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_zzField_Namezz))
        {
            auto txn = zzNamezz();
            lambda(Fields::Field_zzField_Namezz, uint8_t{3u}, uint32_t{0u}, txn);
        }
        //</Field>
    }
    void Assign(Fields key, ElemType&& elem)
    {
        _MarkFieldAssigned(key);
        switch (key)
        {
        //<Field>
        case Fields::Field_zzNamezz:
        {
            _elem.zzNamezz = std::move(elem);
            break;
        }
        //</Field>
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void Assign(ElemType&& /* elem */) { TODO("DoNotCommit"); }
    void Assign(ElemType const& /* elem */) { TODO("DoNotCommit"); }
    void Add(ElemType&& /* elem */) { std::logic_error("Invalid operation"); }
    void Remove(size_t /* key */) { std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    //<Field>
    auto zzNamezz()
    {
        return Stencil::CreateTransaction<Transaction_zzNamezz>(_elemState.zzNamezz, _txnStateForElem.zzNamezz, *this, _elem.zzNamezz);
    }

    //</Field>
    //<Field>
    void set_zzNamezz(zzFieldType_NativeTypezz&& val)
    {
        auto subtxn = zzNamezz();
        subtxn.Assign(std::forward<zzFieldType_NativeTypezz>(val));
    }

    //<FieldType_Mutator>
    zzReturnTypezz zzNamezz_zzField_Namezz(zzArgzz&& args)
    {
        _MarkFieldEdited(Fields::Field_zzField_Namezz);
        auto txn = zzField_Namezz();
        return Stencil::Mutators<std::remove_reference_t<decltype(txn)>>::zzNamezz(txn, std::move(args));
    }
    //</FieldType_Mutator>
    //</Field>

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        //<Field>
        case Fields::Field_zzNamezz:
        {
            auto txn = zzNamezz();
            return lambda(txn);
        }
        //</Field>
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        //<Field>
        {
            auto txn = zzNamezz();
            lambda(Fields::Field_zzNamezz, txn, Elem().zzNamezz);
        }
        //</Field>
    }
};

template <> struct Stencil::Visitor<zzProgram_Namezz::zzStruct_Namezz> : Stencil::VisitorT<zzProgram_Namezz::zzStruct_Namezz>
{
    using TData  = zzProgram_Namezz::zzStruct_Namezz;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        //<Field>
        case Fields::Field_zzNamezz: return lambda(obj.zzNamezz);
        //</Field>
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        //<Field>
        lambda(Fields::Field_zzNamezz, obj.zzNamezz);
        //</Field>
    }
};

//</Struct>

//<Interface>
//<InterfaceFunction>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args>
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

    using Handler = ::ReflectionServices::ReflectedStructHandler<zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args
                                                                     //<Args_Field Join=','>
                                                                     Traits_arg_zzNamezz
                                                                 //</Args_Field>
                                                                 >;
};

template <>
Stencil::Comparator<zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args,
                    zzProgram_Namezz::zzInterface_Namezz_zzInterfaceFunction_Namezz_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               //<Args_Field>
               && Stencil::AreEqual(obj1.get_carg_zzNamezz(), obj2.get_carg_zzNamezz())
        //</Args_Field>
        ;
}
}
;
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
        //<InterfaceFunction Join=','>
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

template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzVariant_Namezz>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzVariant_Namezz>
{
    using Key    = zzProgram_Namezz::zzVariant_Namezz::VariantType;
    using Fields = Key;
};

template <> struct Stencil::EnumTraits<zzProgram_Namezz::zzVariant_Namezz::VariantType>
{
    using Enum = zzProgram_Namezz::zzVariant_Namezz::VariantType;

    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Field Join=','>
        "zzNamezz"
        //</Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static zzProgram_Namezz::zzVariant_Namezz::VariantType ForIndex(size_t index)
    {
        return static_cast<zzProgram_Namezz::zzVariant_Namezz::VariantType>(index);
    }
};

template <> struct Stencil::Visitor<zzProgram_Namezz::zzVariant_Namezz>
{
    using Fields = zzProgram_Namezz::zzVariant_Namezz::VariantType;

    template <typename TType, typename TObj, typename TLambda> static void _SetAndVisit(TObj& obj, TLambda&& lambda)
    {
        using Type = std::remove_cvref_t<TType>;
        obj        = Type{};
        lambda(std::get<Type>(obj));
    }

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
            //<Field>
        case Fields::zzNamezz:
            _SetAndVisit<zzFieldType_NativeTypezz>(obj._variant, std::forward<TLambda>(lambda));
            break;
            //</Field>
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        auto fieldType = static_cast<Fields>(obj.index());
        std::visit([&](auto&& arg) { lambda(fieldType, arg); }, obj._variant);
    }
};

// template <> struct Stencil::Visitor<zzProgram_Namezz::zzVariant_Namezz> : Stencil::StructVisitor<zzProgram_Namezz::zzVariant_Namezz>
// {};

//</Variant>
#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
//</Template>
