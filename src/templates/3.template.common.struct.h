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
//<Import>
#include "zzNamezz.pidl.h"
//</Import>

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

//<InterfaceObjectStore>
struct zzInterface_Namezz_ObjectStore_zzNamezz;
//</InterfaceObjectStore>
//</Interface>
}    // namespace zzProgram_Namezz

//<Struct>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzStruct_Namezz>;
//</Struct>

//<Variant>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzVariant_Namezz>;
//</Variant>

//<Interface>
template <> struct Stencil::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>;

//<InterfaceObjectStore>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>;
//</InterfaceObjectStore>

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

//<Enum>
enum class zzNamezz
{
    //<EnumValue Join=','>
    zzEnumValue_Namezz
    //</EnumValue>
};
//</Enum>

//<NamedConst>
SUPPRESS_WARNINGS_START
SUPPRESS_CLANG_WARNING("implicit-float-conversion")

static constexpr zzFieldType_NativeTypezz zzNamezz = zzFieldType_NativeTypezz{zzValue_NativeTypezz};

SUPPRESS_WARNINGS_END
//</NamedConst>

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
//<InterfaceObjectStore>
struct zzInterface_Namezz_ObjectStore_zzNamezz : zzObjectType_NativeTypezz    //, Database::ObjectT<ObjectStore, ObjectStore_zzNamezz>
{};
//</InterfaceObjectStore>
//</Interface>

}    // namespace zzProgram_Namezz
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

//<Enum>
template <> struct Stencil::EnumTraits<zzProgram_Namezz::zzNamezz>
{
    using Enum = zzProgram_Namezz::zzNamezz;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        //<EnumValue Join=','>
        "zzEnumValue_Namezz"
        //</EnumValue>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Enum ForIndex(size_t index) { return static_cast<Enum>(index); }
};
//</Enum>

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
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Field Join=','>
        "zzNamezz"
        //</Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
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
            Stencil::TimestampedT<ElemType>::UpdateTimestamp(_elem);
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
        Stencil::OptionalPropsT<ElemType>::MarkValid(_elem, elemTxnState.field);
    }

    void NotifyElementEdited_(ElemTxnState const& elemTxnState) { _MarkFieldEdited(elemTxnState.field); }

    bool IsElementChanged(ElemTxnState const& elemTxnState) { return _IsFieldChanged(elemTxnState.field); }
    bool IsChanged() const { return (_elemState.assigntracker | _elemState.edittracker).any(); }

    template <typename TLambda> void VisitChanges([[maybe_unused]] TLambda&& lambda)
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

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    void Assign(ElemType const& /* elem */) { TODO("DoNotCommit"); }
    void Add(ElemType&& /* elem */) { std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { std::logic_error("Invalid operation"); }

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

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
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

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        //<Field>
        lambda(Fields::Field_zzNamezz, obj.zzNamezz);
        //</Field>
    }
};

template <> struct Stencil::Comparator<zzProgram_Namezz::zzStruct_Namezz, zzProgram_Namezz::zzStruct_Namezz>
{
    using ThisType = zzProgram_Namezz::zzStruct_Namezz;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Field>
               && Stencil::AreEqual(obj1.zzNamezz, obj2.zzNamezz)
            //</Field>
            ;
    }
};

template <>
struct Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz> :
    //<AttributeTag>
    public Stencil::Database::Record<zzTagType_NativeTypezz>,
    //</AttributeTag>
    public Stencil::Database::RecordT<zzProgram_Namezz::zzStruct_Namezz>
{
    public:
    //<Field>
    Stencil::Database::RecordNest<zzFieldType_NativeTypezz>::Type zzNamezz{};
    //</Field>
};

template <> struct Stencil::Database::RecordTraits<zzProgram_Namezz::zzStruct_Namezz>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        //<Field>
        typename Stencil::Database::RecordTraits<zzFieldType_NativeTypezz>::RecordTypes,
        //</Field>
        //<AttributeTag>
        typename Stencil::Database::RecordTraits<zzTagType_NativeTypezz>::RecordTypes,
        //</AttributeTag>
        std::tuple<zzProgram_Namezz::zzStruct_Namezz>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              zzProgram_Namezz::zzStruct_Namezz const&                      obj,
                              Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz>& rec)
    {
        //<AttributeTag>
        Stencil::Database::impl::WriteToBuffer(db,
                                               lock,
                                               static_cast<zzTagType_NativeTypezz const&>(obj),
                                               static_cast<Stencil::Database::Record<zzTagType_NativeTypezz>&>(rec));
        //</AttributeTag>
        //<Field>
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.zzNamezz, rec.zzNamezz);
        //</Field>
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz>>
    : Stencil::VisitorT<Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz>>
{
    using TData  = Stencil::Database::Record<zzProgram_Namezz::zzStruct_Namezz>;
    using Fields = TypeTraitsForIndexable<zzProgram_Namezz::zzStruct_Namezz>::Fields;

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

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        //<Field>
        lambda(Fields::Field_zzNamezz, obj.zzNamezz);
        //</Field>
    }
};

//</Struct>

//<Interface>

//<InterfaceObjectStore>

template <>
struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::TypeTraits<zzObjectType_NativeTypezz>
{};

template <>
struct Stencil::Database::RecordTraits<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::Database::RecordTraits<zzObjectType_NativeTypezz>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<typename Stencil::Database::RecordTraits<zzObjectType_NativeTypezz>::RecordTypes,
                                                       std::tuple<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>>;
};

template <>
struct Stencil::Database::Record<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::Database::Record<zzObjectType_NativeTypezz>
{};

template <>
struct Stencil::Visitor<Stencil::Database::Record<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>>
    : Stencil::Visitor<Stencil::Database::Record<zzObjectType_NativeTypezz>>
{};

//</InterfaceObjectStore>

namespace zzProgram_Namezz
{
struct zzInterface_Namezz : public Stencil::InterfaceT<zzInterface_Namezz>
{
    using BaseClass = Stencil::InterfaceT<zzInterface_Namezz>;

    public:
    struct Dummy
    {};

    using ObjectStore = Stencil::Database::Database<
        //<InterfaceObjectStore Join=','>
        zzInterface_Namezz_ObjectStore_zzNamezz
        //</InterfaceObjectStore>
        >;

    zzInterface_Namezz()          = default;
    virtual ~zzInterface_Namezz() = default;
    CLASS_DELETE_COPY_AND_MOVE(zzInterface_Namezz);

    //<InterfaceFunction>
    virtual zzReturnType_NativeTypezz zzInterfaceFunction_Namezz(
        //<Args_Field Join=','>
        zzFieldType_NativeTypezz const& zzNamezz
        //</Args_Field>
        )
        = 0;
    struct Args_zzInterfaceFunction_Namezz
    {
        //<Args_Field>
        zzFieldType_NativeTypezz zzNamezz{};
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

    //<InterfaceEvent>
    struct Args_zzInterfaceEvent_Namezz
    {
        //<Args_Field>
        zzFieldType_NativeTypezz zzNamezz{};
        //</Args_Field>
    };

    void Raise_zzInterfaceEvent_Namezz(
        //<Args_Field Join=','>
        zzFieldType_NativeTypezz const& zzNamezz
        //</Args_Field>
    )
    {
        BaseClass::RaiseEvent(Args_zzInterfaceEvent_Namezz{
            //<Args_Field Join=','>
            zzNamezz
            //</Args_Field>
        });
    }
    //</InterfaceEvent>

    ObjectStore objects;

    static std::unique_ptr<zzInterface_Namezz> Create();
    static std::shared_ptr<zzInterface_Namezz> CreateShared();
    static std::shared_ptr<zzInterface_Namezz> Activate();
};
}    // namespace zzProgram_Namezz
//<InterfaceFunction>

template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
{
    enum class Fields
    {
        Invalid,
        //<Args_Field Join=','>
        arg_zzNamezz
        //</Args_Field>
    };

    using Key = Fields;
};

template <>
struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Args_Field Join=','>
        "zzNamezz"
        //</Args_Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>::Fields
    ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>::Fields>(
            index);
    }
};

template <>
struct Stencil::Visitor<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
    : Stencil::VisitorT<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
{
    using TData  = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        //<Args_Field>
        case Fields::arg_zzNamezz: return lambda(obj.zzNamezz);
        //</Args_Field>
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        //<Args_Field>
        lambda(Fields::arg_zzNamezz, obj.zzNamezz);
        //</Args_Field>
    }
};

template <>
struct Stencil::Comparator<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz,
                           zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
{
    using ThisType = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Args_Field>
               && Stencil::AreEqual(obj1.zzNamezz, obj2.zzNamezz)
            //</Args_Field>
            ;
    }
};

template <> struct Stencil::InterfaceApiTraits<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz>
{
    using ArgsStruct = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "zzNamezz"; }

    static auto Invoke(zzProgram_Namezz::zzInterface_Namezz& instance, ArgsStruct& args)
    {
        return instance.zzNamezz(
            //<Args_Field Join=','>
            args.zzNamezz
            //</Args_Field>
        );
    }
};
//</InterfaceFunction>

//<InterfaceEvent>
template <> struct Stencil::TypeTraits<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
{
    enum class Fields
    {
        Invalid,
        //<Args_Field Join=','>
        arg_zzNamezz
        //</Args_Field>
    };

    using Key = Fields;
};

template <>
struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Args_Field Join=','>
        "zzNamezz"
        //</Args_Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>::Fields
    ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>::Fields>(
            index);
    }
};

template <>
struct Stencil::Visitor<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
    : Stencil::VisitorT<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
{
    using TData  = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        //<Args_Field>
        case Fields::arg_zzNamezz: return lambda(obj.zzNamezz);
        //</Args_Field>
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        //<Args_Field>
        lambda(Fields::arg_zzNamezz, obj.zzNamezz);
        //</Args_Field>
    }
};

template <>
struct Stencil::Comparator<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz,
                           zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
{
    using ThisType = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               //<Args_Field>
               && Stencil::AreEqual(obj1.zzNamezz, obj2.zzNamezz)
            //</Args_Field>
            ;
    }
};

template <> struct Stencil::InterfaceApiTraits<zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz>
{
    using ArgsStruct = zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz;
    static constexpr std::string_view Name() { return "zzNamezz"; }
};
//</InterfaceEvent>

template <> struct Stencil::InterfaceTraits<zzProgram_Namezz::zzInterface_Namezz>
{
    static constexpr std::string_view Name() { return "zzInterface_Namezz"; }

    using ApiStructs = std::tuple<
        //<InterfaceFunction Join=','>
        zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceFunction_Namezz
        //</InterfaceFunction>
        >;

    using Objects = std::tuple<
        //<InterfaceObjectStore Join=','>
        zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz
        //</InterfaceObjectStore>
        >;

    using EventStructs = std::tuple<
        //<InterfaceEvent>
        zzProgram_Namezz::zzInterface_Namezz::Args_zzInterfaceEvent_Namezz
        //</InterfaceEvent>
        >;
};

//<InterfaceObjectStore>

template <>
struct Stencil::TypeTraitsForPrimitive<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::TypeTraitsForPrimitive<zzObjectType_NativeTypezz>
{};
template <>
struct Stencil::TypeTraitsForIterable<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::TypeTraitsForIterable<zzObjectType_NativeTypezz>
{};
template <>
struct Stencil::TypeTraitsForIndexable<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
    : public Stencil::TypeTraitsForIndexable<zzObjectType_NativeTypezz>
{};
template <>
struct Stencil::Visitor<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz> : public Stencil::Visitor<zzObjectType_NativeTypezz>
{};
//</InterfaceObjectStore>

//<InterfaceObjectStore>
template <> struct Stencil::InterfaceObjectTraits<zzProgram_Namezz::zzInterface_Namezz_ObjectStore_zzNamezz>
{
    static constexpr std::string_view Name() { return "zzNamezz"; }
};
//</InterfaceObjectStore>

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
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        //<Field Join=','>
        "zzNamezz"
        //</Field>
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
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

    template <typename T, typename TLambda> static void VisitAll(T& obj, TLambda&& lambda)
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
