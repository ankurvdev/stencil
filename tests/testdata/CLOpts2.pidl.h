#pragma once
#include <stencil/stencil.h>
// SECTION START: DECLARATIONS
#if true

namespace CLOpts2
{
struct InstallOptions;
struct QueueOptions;
struct PauseOptions;
struct CancelOptions;
struct ResumeOptions;
struct UpdateOptions;
struct HydrateOptions;
struct CommandLineOptions;
}    // namespace CLOpts2

template <> struct Stencil::TypeTraits<CLOpts2::InstallOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::QueueOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::PauseOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::CancelOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::ResumeOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::UpdateOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::HydrateOptions>;
template <> struct Stencil::TypeTraits<CLOpts2::CommandLineOptions>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace CLOpts2
{
struct InstallOptions :
    public Stencil::StructT<InstallOptions>
{
    shared_string ProductId = {};
    bool Repair = 0;
    bool ForceNonSD = 0;
    shared_string TargetVolume = {};
    shared_string User = {};
};
struct QueueOptions :
    public Stencil::StructT<QueueOptions>
{
    shared_string ProductId = {};
};
struct PauseOptions :
    public Stencil::StructT<PauseOptions>
{
    shared_string ProductId = {};
};
struct CancelOptions :
    public Stencil::StructT<CancelOptions>
{
    shared_string ProductId = {};
};
struct ResumeOptions :
    public Stencil::StructT<ResumeOptions>
{
    shared_string ProductId = {};
};
struct UpdateOptions :
    public Stencil::StructT<UpdateOptions>
{
    shared_string ProductId = {};
};
struct HydrateOptions :
    public Stencil::StructT<HydrateOptions>
{
    shared_string ProductId = {};
};
struct CommandLineOptions : public Stencil::VariantT<CommandLineOptions>
{
    std::variant<std::monostate,
                 ::CLOpts2::InstallOptions
,                 ::CLOpts2::QueueOptions
,                 ::CLOpts2::PauseOptions
,                 ::CLOpts2::CancelOptions
,                 ::CLOpts2::ResumeOptions
,                 ::CLOpts2::UpdateOptions
,                 ::CLOpts2::HydrateOptions
                 >
        _variant;

    enum class VariantType
    {
        Invalid,
        install
,        queue
,        pause
,        cancel
,        resume
,        update
,        hydrate
    };

    struct Variant_Invalid
    {};
    struct Variant_installT
    {};
    struct Variant_queueT
    {};
    struct Variant_pauseT
    {};
    struct Variant_cancelT
    {};
    struct Variant_resumeT
    {};
    struct Variant_updateT
    {};
    struct Variant_hydrateT
    {};
    VariantType Type() const { return static_cast<VariantType>(_variant.index()); }

    auto operator=(::CLOpts2::InstallOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::InstallOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::InstallOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::InstallOptions&       install() { return std::get<::CLOpts2::InstallOptions>(_variant); }
    ::CLOpts2::InstallOptions const& install() const { return std::get<::CLOpts2::InstallOptions>(_variant); }
    auto operator=(::CLOpts2::QueueOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::QueueOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::QueueOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::QueueOptions&       queue() { return std::get<::CLOpts2::QueueOptions>(_variant); }
    ::CLOpts2::QueueOptions const& queue() const { return std::get<::CLOpts2::QueueOptions>(_variant); }
    auto operator=(::CLOpts2::PauseOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::PauseOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::PauseOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::PauseOptions&       pause() { return std::get<::CLOpts2::PauseOptions>(_variant); }
    ::CLOpts2::PauseOptions const& pause() const { return std::get<::CLOpts2::PauseOptions>(_variant); }
    auto operator=(::CLOpts2::CancelOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::CancelOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::CancelOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::CancelOptions&       cancel() { return std::get<::CLOpts2::CancelOptions>(_variant); }
    ::CLOpts2::CancelOptions const& cancel() const { return std::get<::CLOpts2::CancelOptions>(_variant); }
    auto operator=(::CLOpts2::ResumeOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::ResumeOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::ResumeOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::ResumeOptions&       resume() { return std::get<::CLOpts2::ResumeOptions>(_variant); }
    ::CLOpts2::ResumeOptions const& resume() const { return std::get<::CLOpts2::ResumeOptions>(_variant); }
    auto operator=(::CLOpts2::UpdateOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::UpdateOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::UpdateOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::UpdateOptions&       update() { return std::get<::CLOpts2::UpdateOptions>(_variant); }
    ::CLOpts2::UpdateOptions const& update() const { return std::get<::CLOpts2::UpdateOptions>(_variant); }
    auto operator=(::CLOpts2::HydrateOptions&& obj)
    {
        _variant = std::forward<::CLOpts2::HydrateOptions>(obj);
        return *this;
    }
    auto operator=(::CLOpts2::HydrateOptions const& obj)
    {
        _variant = obj;
        return *this;
    }

    ::CLOpts2::HydrateOptions&       hydrate() { return std::get<::CLOpts2::HydrateOptions>(_variant); }
    ::CLOpts2::HydrateOptions const& hydrate() const { return std::get<::CLOpts2::HydrateOptions>(_variant); }
};
}    // namespace CLOpts2
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<CLOpts2::InstallOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>
{
    using TObj = CLOpts2::InstallOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
,        Field_Repair
,        Field_ForceNonSD
,        Field_TargetVolume
,        Field_User
    };

    struct Field_ProductIdT
    {};
    struct Field_RepairT
    {};
    struct Field_ForceNonSDT
    {};
    struct Field_TargetVolumeT
    {};
    struct Field_UserT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return false;
        case Fields::Field_Repair: return true;
        case Fields::Field_ForceNonSD: return true;
        case Fields::Field_TargetVolume: return true;
        case Fields::Field_User: return true;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return false; }
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_RepairT /* key */) { return true; }
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ForceNonSDT /* key */) { return true; }
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_TargetVolumeT /* key */) { return true; }
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_UserT /* key */) { return true; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_RepairT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_RepairT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "Repair"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "Repair";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_RepairT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_RepairT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_Repair);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_Repair) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ForceNonSDT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ForceNonSDT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ForceNonSD"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ForceNonSD";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ForceNonSDT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ForceNonSDT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ForceNonSD);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ForceNonSD) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_TargetVolumeT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_TargetVolumeT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "TargetVolume"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "TargetVolume";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_TargetVolumeT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_TargetVolumeT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_TargetVolume);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_TargetVolume) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_UserT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_UserT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "User"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "User";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_UserT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_UserT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_User);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_User) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_RepairT>
{
    static auto Value() { return "Repair existing installated product"; }
};

template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_ForceNonSDT>
{
    static auto Value() { return "Force installation to non SD"; }
};

template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_TargetVolumeT>
{
    static auto Value() { return "Target Volume"; }
};

template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Field_UserT>
{
    static auto Value() { return "User to install for"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::InstallOptions>
{
    static auto Value() { return "Install a specific product"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
,        "Repair"
,        "ForceNonSD"
,        "TargetVolume"
,        "User"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::InstallOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
        ElemTxnState Repair = {Fields::Field_Repair};
        ElemTxnState ForceNonSD = {Fields::Field_ForceNonSD};
        ElemTxnState TargetVolume = {Fields::Field_TargetVolume};
        ElemTxnState User = {Fields::Field_User};
    };

    using Txn               = Stencil::Transaction<CLOpts2::InstallOptions, TContainer>;
    using ElemType          = CLOpts2::InstallOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    using Transaction_Repair = Stencil::Transaction<bool, Txn>;
    using Transaction_ForceNonSD = Stencil::Transaction<bool, Txn>;
    using Transaction_TargetVolume = Stencil::Transaction<shared_string, Txn>;
    using Transaction_User = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
        typename Transaction_Repair::TxnState Repair{};
        typename Transaction_ForceNonSD::TxnState ForceNonSD{};
        typename Transaction_TargetVolume::TxnState TargetVolume{};
        typename Transaction_User::TxnState User{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
        if (_IsFieldAssigned(Fields::Field_Repair))
        {
            auto txn = Repair();
            lambda(Fields::Field_Repair, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_Repair))
        {
            auto txn = Repair();
            lambda(Fields::Field_Repair, uint8_t{3u}, uint32_t{0u}, txn);
        }
        if (_IsFieldAssigned(Fields::Field_ForceNonSD))
        {
            auto txn = ForceNonSD();
            lambda(Fields::Field_ForceNonSD, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ForceNonSD))
        {
            auto txn = ForceNonSD();
            lambda(Fields::Field_ForceNonSD, uint8_t{3u}, uint32_t{0u}, txn);
        }
        if (_IsFieldAssigned(Fields::Field_TargetVolume))
        {
            auto txn = TargetVolume();
            lambda(Fields::Field_TargetVolume, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_TargetVolume))
        {
            auto txn = TargetVolume();
            lambda(Fields::Field_TargetVolume, uint8_t{3u}, uint32_t{0u}, txn);
        }
        if (_IsFieldAssigned(Fields::Field_User))
        {
            auto txn = User();
            lambda(Fields::Field_User, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_User))
        {
            auto txn = User();
            lambda(Fields::Field_User, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    auto Repair()
    {
        return Stencil::CreateTransaction<Transaction_Repair>(_elemState.Repair, _txnStateForElem.Repair, *this, _elem.Repair);
    }

    auto ForceNonSD()
    {
        return Stencil::CreateTransaction<Transaction_ForceNonSD>(_elemState.ForceNonSD, _txnStateForElem.ForceNonSD, *this, _elem.ForceNonSD);
    }

    auto TargetVolume()
    {
        return Stencil::CreateTransaction<Transaction_TargetVolume>(_elemState.TargetVolume, _txnStateForElem.TargetVolume, *this, _elem.TargetVolume);
    }

    auto User()
    {
        return Stencil::CreateTransaction<Transaction_User>(_elemState.User, _txnStateForElem.User, *this, _elem.User);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    void set_Repair(bool&& val)
    {
        auto subtxn = Repair();
        subtxn.Assign(std::forward<bool>(val));
    }

    void set_ForceNonSD(bool&& val)
    {
        auto subtxn = ForceNonSD();
        subtxn.Assign(std::forward<bool>(val));
    }

    void set_TargetVolume(shared_string&& val)
    {
        auto subtxn = TargetVolume();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    void set_User(shared_string&& val)
    {
        auto subtxn = User();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Field_Repair:
        {
            auto txn = Repair();
            return lambda(txn);
        }
        case Fields::Field_ForceNonSD:
        {
            auto txn = ForceNonSD();
            return lambda(txn);
        }
        case Fields::Field_TargetVolume:
        {
            auto txn = TargetVolume();
            return lambda(txn);
        }
        case Fields::Field_User:
        {
            auto txn = User();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
        {
            auto txn = Repair();
            lambda(Traits::Field_RepairT{}, txn, Elem().Repair);
        }
        {
            auto txn = ForceNonSD();
            lambda(Traits::Field_ForceNonSDT{}, txn, Elem().ForceNonSD);
        }
        {
            auto txn = TargetVolume();
            lambda(Traits::Field_TargetVolumeT{}, txn, Elem().TargetVolume);
        }
        {
            auto txn = User();
            lambda(Traits::Field_UserT{}, txn, Elem().User);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::InstallOptions> : Stencil::VisitorT<CLOpts2::InstallOptions>
{
    using TData  = CLOpts2::InstallOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Field_Repair: return lambda(obj.Repair);
        case Fields::Field_ForceNonSD: return lambda(obj.ForceNonSD);
        case Fields::Field_TargetVolume: return lambda(obj.TargetVolume);
        case Fields::Field_User: return lambda(obj.User);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
        lambda(Traits::Field_RepairT{}, obj.Repair);
        lambda(Traits::Field_ForceNonSDT{}, obj.ForceNonSD);
        lambda(Traits::Field_TargetVolumeT{}, obj.TargetVolume);
        lambda(Traits::Field_UserT{}, obj.User);
    }
};

template <> struct Stencil::Comparator<CLOpts2::InstallOptions, CLOpts2::InstallOptions>
{
    using ThisType = CLOpts2::InstallOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
               && Stencil::AreEqual(obj1.Repair, obj2.Repair)
               && Stencil::AreEqual(obj1.ForceNonSD, obj2.ForceNonSD)
               && Stencil::AreEqual(obj1.TargetVolume, obj2.TargetVolume)
               && Stencil::AreEqual(obj1.User, obj2.User)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::InstallOptions> :
    public Stencil::Database::RecordT<CLOpts2::InstallOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
    Stencil::Database::RecordNest<bool>::Type Repair{};
    Stencil::Database::RecordNest<bool>::Type ForceNonSD{};
    Stencil::Database::RecordNest<shared_string>::Type TargetVolume{};
    Stencil::Database::RecordNest<shared_string>::Type User{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::InstallOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        typename Stencil::Database::RecordTraits<bool>::RecordTypes,
        typename Stencil::Database::RecordTraits<bool>::RecordTypes,
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::InstallOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::InstallOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::InstallOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::InstallOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.Repair, rec.Repair);
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ForceNonSD, rec.ForceNonSD);
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.TargetVolume, rec.TargetVolume);
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.User, rec.User);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::InstallOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::InstallOptions>>
{
    using TData  = CLOpts2::InstallOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Field_Repair: return lambda(obj.Repair);
        case Fields::Field_ForceNonSD: return lambda(obj.ForceNonSD);
        case Fields::Field_TargetVolume: return lambda(obj.TargetVolume);
        case Fields::Field_User: return lambda(obj.User);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
        lambda(Traits::Field_RepairT{}, obj.Repair);
        lambda(Traits::Field_ForceNonSDT{}, obj.ForceNonSD);
        lambda(Traits::Field_TargetVolumeT{}, obj.TargetVolume);
        lambda(Traits::Field_UserT{}, obj.User);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::QueueOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>
{
    using TObj = CLOpts2::QueueOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return true;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return true; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::QueueOptions>
{
    static auto Value() { return "Listen for progress notifications for a specific product or all queued products"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::QueueOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::QueueOptions, TContainer>;
    using ElemType          = CLOpts2::QueueOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::QueueOptions> : Stencil::VisitorT<CLOpts2::QueueOptions>
{
    using TData  = CLOpts2::QueueOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::QueueOptions, CLOpts2::QueueOptions>
{
    using ThisType = CLOpts2::QueueOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::QueueOptions> :
    public Stencil::Database::RecordT<CLOpts2::QueueOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::QueueOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::QueueOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::QueueOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::QueueOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::QueueOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::QueueOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::QueueOptions>>
{
    using TData  = CLOpts2::QueueOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::PauseOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>
{
    using TObj = CLOpts2::PauseOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return false;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return false; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::PauseOptions>
{
    static auto Value() { return "Pause the given product"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::PauseOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::PauseOptions, TContainer>;
    using ElemType          = CLOpts2::PauseOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::PauseOptions> : Stencil::VisitorT<CLOpts2::PauseOptions>
{
    using TData  = CLOpts2::PauseOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::PauseOptions, CLOpts2::PauseOptions>
{
    using ThisType = CLOpts2::PauseOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::PauseOptions> :
    public Stencil::Database::RecordT<CLOpts2::PauseOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::PauseOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::PauseOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::PauseOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::PauseOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::PauseOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::PauseOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::PauseOptions>>
{
    using TData  = CLOpts2::PauseOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::CancelOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>
{
    using TObj = CLOpts2::CancelOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return false;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return false; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::CancelOptions>
{
    static auto Value() { return "Cancel the given product"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::CancelOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::CancelOptions, TContainer>;
    using ElemType          = CLOpts2::CancelOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::CancelOptions> : Stencil::VisitorT<CLOpts2::CancelOptions>
{
    using TData  = CLOpts2::CancelOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::CancelOptions, CLOpts2::CancelOptions>
{
    using ThisType = CLOpts2::CancelOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::CancelOptions> :
    public Stencil::Database::RecordT<CLOpts2::CancelOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::CancelOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::CancelOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::CancelOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::CancelOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::CancelOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::CancelOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::CancelOptions>>
{
    using TData  = CLOpts2::CancelOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::ResumeOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>
{
    using TObj = CLOpts2::ResumeOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return false;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return false; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::ResumeOptions>
{
    static auto Value() { return "Resume the given product"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::ResumeOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::ResumeOptions, TContainer>;
    using ElemType          = CLOpts2::ResumeOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::ResumeOptions> : Stencil::VisitorT<CLOpts2::ResumeOptions>
{
    using TData  = CLOpts2::ResumeOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::ResumeOptions, CLOpts2::ResumeOptions>
{
    using ThisType = CLOpts2::ResumeOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::ResumeOptions> :
    public Stencil::Database::RecordT<CLOpts2::ResumeOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::ResumeOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::ResumeOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::ResumeOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::ResumeOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::ResumeOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::ResumeOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::ResumeOptions>>
{
    using TData  = CLOpts2::ResumeOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::UpdateOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>
{
    using TObj = CLOpts2::UpdateOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return true;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return true; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::UpdateOptions>
{
    static auto Value() { return "Check for updates for a given product or all installed products"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::UpdateOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::UpdateOptions, TContainer>;
    using ElemType          = CLOpts2::UpdateOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::UpdateOptions> : Stencil::VisitorT<CLOpts2::UpdateOptions>
{
    using TData  = CLOpts2::UpdateOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::UpdateOptions, CLOpts2::UpdateOptions>
{
    using ThisType = CLOpts2::UpdateOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::UpdateOptions> :
    public Stencil::Database::RecordT<CLOpts2::UpdateOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::UpdateOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::UpdateOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::UpdateOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::UpdateOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::UpdateOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::UpdateOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::UpdateOptions>>
{
    using TData  = CLOpts2::UpdateOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::HydrateOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>
{
    using TObj = CLOpts2::HydrateOptions;
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    struct Field_ProductIdT
    {};
    using Key = Fields;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key key)
    {
        switch (key)
        {
        case Fields::Field_ProductId: return false;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
        return false;
    }

    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Field_ProductIdT /* key */) { return false; }
};
template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Field_ProductIdT, Stencil::ProtocolString>
{
    using T = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Field_ProductIdT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "ProductId"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "ProductId";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <>
struct Stencil::SerDes<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Field_ProductIdT, Stencil::ProtocolBinary>
{
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields;
    using T      = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Field_ProductIdT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */)
    {
        ctx << static_cast<uint32_t>(Fields::Field_ProductId);
    }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::Field_ProductId) { throw std::invalid_argument("Invalid"); }
    }
};
template <>
struct Stencil::Attribute<Stencil::AttributeType::Description,
                          Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Field_ProductIdT>
{
    static auto Value() { return "Catalog/Product:Sku:Bundle"; }
};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::HydrateOptions>
{
    static auto Value() { return "Download document for a given product"; }
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields>(index);
    }
};

template <Stencil::ConceptTransaction TContainer> struct Stencil::Transaction<CLOpts2::HydrateOptions, TContainer>
{
    using Traits = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>;

    using Fields = Traits::Fields;

    struct ElemTxnState
    {
        Fields field;
    };

    struct TxnStateForElem
    {
        ElemTxnState ProductId = {Fields::Field_ProductId};
    };

    using Txn               = Stencil::Transaction<CLOpts2::HydrateOptions, TContainer>;
    using ElemType          = CLOpts2::HydrateOptions;
    using ContainerTxnState = typename TContainer::ElemTxnState;

    using Transaction_ProductId = Stencil::Transaction<shared_string, Txn>;
    struct TxnState
    {
        std::bitset<64> assigntracker;    
        std::bitset<64> edittracker;      
        typename Transaction_ProductId::TxnState ProductId{};
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
        if (_IsFieldAssigned(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{0u}, uint32_t{0u}, txn);
        }
        else if (_IsFieldEdited(Fields::Field_ProductId))
        {
            auto txn = ProductId();
            lambda(Fields::Field_ProductId, uint8_t{3u}, uint32_t{0u}, txn);
        }
    }

    void Assign(ElemType&& elem)
    {
        // if (Stencil::AreEqual(_elem, elem)) return;
        std::swap(_elem, elem);
        _container.NotifyElementAssigned_(_containerState);
    }

    // void Assign(ElemType const& /* elem */);
    void Add(ElemType&& /* elem */) { throw std::logic_error("Invalid operation"); }

    template <typename T> void Remove(T /* key */) { throw std::logic_error("Invalid operation"); }

    private:
    TxnStateForElem    _txnStateForElem{};
    TxnState&          _elemState;
    ContainerTxnState& _containerState;
    TContainer&        _container;
    ElemType&          _elem;

    public:
    auto ProductId()
    {
        return Stencil::CreateTransaction<Transaction_ProductId>(_elemState.ProductId, _txnStateForElem.ProductId, *this, _elem.ProductId);
    }

    void set_ProductId(shared_string&& val)
    {
        auto subtxn = ProductId();
        subtxn.Assign(std::forward<shared_string>(val));
    }

    template <typename TLambda> auto Edit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId:
        {
            auto txn = ProductId();
            return lambda(txn);
        }
        case Fields::Invalid: [[fallthrough]];
        default: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Assign(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        return Edit(index, std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        {
            auto txn = ProductId();
            lambda(Traits::Field_ProductIdT{}, txn, Elem().ProductId);
        }
    }
};

template <> struct Stencil::Visitor<CLOpts2::HydrateOptions> : Stencil::VisitorT<CLOpts2::HydrateOptions>
{
    using TData  = CLOpts2::HydrateOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::Comparator<CLOpts2::HydrateOptions, CLOpts2::HydrateOptions>
{
    using ThisType = CLOpts2::HydrateOptions;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && Stencil::AreEqual(obj1.ProductId, obj2.ProductId)
            ;
    }
};

template <>
struct Stencil::Database::Record<CLOpts2::HydrateOptions> :
    public Stencil::Database::RecordT<CLOpts2::HydrateOptions>
{
    public:
    Stencil::Database::RecordNest<shared_string>::Type ProductId{};
};

template <> struct Stencil::Database::RecordTraits<CLOpts2::HydrateOptions>
{
    using RecordTypes = Stencil::Database::tuple_cat_t<
        typename Stencil::Database::RecordTraits<shared_string>::RecordTypes,
        std::tuple<CLOpts2::HydrateOptions>>;

    static constexpr size_t Size() { return sizeof(Stencil::Database::Record<CLOpts2::HydrateOptions>); }

    template <typename TDb>
    static void WriteToBuffer(TDb&                                                          db,
                              Stencil::Database::RWLock const&                              lock,
                              CLOpts2::HydrateOptions const&                      obj,
                              Stencil::Database::Record<CLOpts2::HydrateOptions>& rec)
    {
        Stencil::Database::impl::WriteToBuffer(db, lock, obj.ProductId, rec.ProductId);
    }
};

template <>
struct Stencil::Visitor<Stencil::Database::Record<CLOpts2::HydrateOptions>>
    : Stencil::VisitorT<Stencil::Database::Record<CLOpts2::HydrateOptions>>
{
    using TData  = CLOpts2::HydrateOptions;
    using Traits = Stencil::TypeTraitsForIndexable<TData>;
    using Fields = Traits::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Traits::Field_ProductIdT{}, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::CommandLineOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::CommandLineOptions>
{
    using TObj   = CLOpts2::CommandLineOptions;
    using Key    = TObj::VariantType;
    using Fields = Key;
    static constexpr bool HasDefaultValueForKey(TObj const& /* obj */, Key /* key */) { return false; }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_installT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_installT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "install"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "install";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_installT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_installT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::install); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::install) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_queueT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_queueT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "queue"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "queue";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_queueT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_queueT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::queue); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::queue) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_pauseT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_pauseT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "pause"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "pause";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_pauseT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_pauseT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::pause); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::pause) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_cancelT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_cancelT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "cancel"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "cancel";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_cancelT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_cancelT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::cancel); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::cancel) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_resumeT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_resumeT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "resume"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "resume";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_resumeT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_resumeT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::resume); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::resume) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_updateT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_updateT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "update"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "update";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_updateT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_updateT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::update); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::update) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_hydrateT, Stencil::ProtocolString>
{
    using T = CLOpts2::CommandLineOptions::Variant_hydrateT;
    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { fmt::print(ctx, "{}", "hydrate"); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        std::string_view name = "hydrate";
        if (!std::equal(std::begin(ctx), std::end(ctx), std::begin(name), std::end(name), [](auto l, auto r) {
                return std::tolower(l) == std::tolower(r);
            }))
        {
            throw std::invalid_argument("Invalid");
        }
    }
};

template <> struct Stencil::SerDes<CLOpts2::CommandLineOptions::Variant_hydrateT, Stencil::ProtocolBinary>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;
    using T      = CLOpts2::CommandLineOptions::Variant_hydrateT;

    template <typename Context> static auto Write(Context& ctx, T const& /* obj */) { ctx << static_cast<uint32_t>(Fields::hydrate); }

    template <typename Context> static auto Read(T& /* obj */, Context& ctx)
    {
        auto val = static_cast<Fields>(ctx.template read<uint32_t>());
        if (val != Fields::hydrate) { throw std::invalid_argument("Invalid"); }
    }
};
template <> struct Stencil::EnumTraits<CLOpts2::CommandLineOptions::VariantType>
{
    using Enum = CLOpts2::CommandLineOptions::VariantType;
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    static constexpr std::string_view Names[] = {
        "Invalid",
        "install"
,        "queue"
,        "pause"
,        "cancel"
,        "resume"
,        "update"
,        "hydrate"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }
    SUPPRESS_WARNINGS_END
    static CLOpts2::CommandLineOptions::VariantType ForIndex(size_t index)
    {
        return static_cast<CLOpts2::CommandLineOptions::VariantType>(index);
    }
};

template <> struct Stencil::Visitor<CLOpts2::CommandLineOptions>
{
    using Fields  = CLOpts2::CommandLineOptions::VariantType;
    using Variant = CLOpts2::CommandLineOptions;
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
        case Fields::install:
            _SetAndVisit<::CLOpts2::InstallOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::queue:
            _SetAndVisit<::CLOpts2::QueueOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::pause:
            _SetAndVisit<::CLOpts2::PauseOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::cancel:
            _SetAndVisit<::CLOpts2::CancelOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::resume:
            _SetAndVisit<::CLOpts2::ResumeOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::update:
            _SetAndVisit<::CLOpts2::UpdateOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::hydrate:
            _SetAndVisit<::CLOpts2::HydrateOptions>(obj._variant, std::forward<TLambda>(lambda));
            break;
        case Fields::Invalid: [[fallthrough]];
        default: break;
        }
    }

    template <typename T, typename TField, typename TLambda> static void VisitKey(T& obj, TField /* field */, TLambda&& lambda)
    {
        if constexpr (std::is_same_v<TField, Variant::Variant_installT>)
        {
            _SetAndVisit<::CLOpts2::InstallOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_queueT>)
        {
            _SetAndVisit<::CLOpts2::QueueOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_pauseT>)
        {
            _SetAndVisit<::CLOpts2::PauseOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_cancelT>)
        {
            _SetAndVisit<::CLOpts2::CancelOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_resumeT>)
        {
            _SetAndVisit<::CLOpts2::ResumeOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_updateT>)
        {
            _SetAndVisit<::CLOpts2::UpdateOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
        if constexpr (std::is_same_v<TField, Variant::Variant_hydrateT>)
        {
            _SetAndVisit<::CLOpts2::HydrateOptions>(obj._variant, std::forward<TLambda>(lambda));
        }
    }

    template <typename T, typename TLambda> static void VisitAll(T& obj, TLambda&& lambda)
    {
        // auto fieldType = static_cast<Fields>(obj._variant.index());
        if (std::holds_alternative<::CLOpts2::InstallOptions>(obj._variant))
        {
            lambda(Variant::Variant_installT{}, std::get<::CLOpts2::InstallOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::QueueOptions>(obj._variant))
        {
            lambda(Variant::Variant_queueT{}, std::get<::CLOpts2::QueueOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::PauseOptions>(obj._variant))
        {
            lambda(Variant::Variant_pauseT{}, std::get<::CLOpts2::PauseOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::CancelOptions>(obj._variant))
        {
            lambda(Variant::Variant_cancelT{}, std::get<::CLOpts2::CancelOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::ResumeOptions>(obj._variant))
        {
            lambda(Variant::Variant_resumeT{}, std::get<::CLOpts2::ResumeOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::UpdateOptions>(obj._variant))
        {
            lambda(Variant::Variant_updateT{}, std::get<::CLOpts2::UpdateOptions>(obj._variant));
        }
        if (std::holds_alternative<::CLOpts2::HydrateOptions>(obj._variant))
        {
            lambda(Variant::Variant_hydrateT{}, std::get<::CLOpts2::HydrateOptions>(obj._variant));
        }
    }
};

// template <> struct Stencil::Visitor<CLOpts2::CommandLineOptions> : Stencil::StructVisitor<CLOpts2::CommandLineOptions>
// {};

template <> struct Stencil::Attribute<Stencil::AttributeType::Description, CLOpts2::CommandLineOptions>
{
    static auto Value() { return "Command Line tool for windows-store operations"; }
};
template <> struct Stencil::VisitorForVariant<CLOpts2::CommandLineOptions>
{
    using T      = CLOpts2::CommandLineOptions;
    using Fields = CLOpts2::CommandLineOptions::VariantType;

    static bool IsMonostate(T const& obj) { return obj._variant.index() == 0; }

    template <typename TLambda> static void VisitAlternatives(T const& /* obj */, TLambda&& lambda)
    {
        {
            ::CLOpts2::InstallOptions varobj{};
            lambda(Fields::install, varobj);
        }
        {
            ::CLOpts2::QueueOptions varobj{};
            lambda(Fields::queue, varobj);
        }
        {
            ::CLOpts2::PauseOptions varobj{};
            lambda(Fields::pause, varobj);
        }
        {
            ::CLOpts2::CancelOptions varobj{};
            lambda(Fields::cancel, varobj);
        }
        {
            ::CLOpts2::ResumeOptions varobj{};
            lambda(Fields::resume, varobj);
        }
        {
            ::CLOpts2::UpdateOptions varobj{};
            lambda(Fields::update, varobj);
        }
        {
            ::CLOpts2::HydrateOptions varobj{};
            lambda(Fields::hydrate, varobj);
        }
    }

    template <typename TLambda> static void VisitActiveAlternative(T const& obj, TLambda&& lambda)
    {
        std::visit(
            [&](auto const& o) {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(o)>, std::monostate>) {}
                else { lambda(static_cast<Fields>(obj._variant.index()), o); }
            },
            obj._variant);
    }

    template <typename TLambda> static void VisitActiveAlternative(T& obj, TLambda&& lambda)
    {
        std::visit(
            [&](auto& o) {
                if constexpr (std::is_same_v<std::remove_cvref_t<decltype(o)>, std::monostate>) {}
                else { lambda(static_cast<Fields>(obj._variant.index()), o); }
            },
            obj._variant);
    }
};
#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
