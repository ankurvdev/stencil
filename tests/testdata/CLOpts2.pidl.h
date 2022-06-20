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
    shared_string TargetVolume = "";
    shared_string User = "";
};
struct QueueOptions :
    public Stencil::StructT<QueueOptions>
{
    shared_string ProductId = "";
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
    shared_string ProductId = "";
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

    VariantType Type() const { return static_cast<VariantType>(_variant.index()); }

    ::CLOpts2::InstallOptions&       install() { return std::get<::CLOpts2::InstallOptions>(_variant); }
    ::CLOpts2::InstallOptions const& install() const { return std::get<::CLOpts2::InstallOptions>(_variant); }
    ::CLOpts2::QueueOptions&       queue() { return std::get<::CLOpts2::QueueOptions>(_variant); }
    ::CLOpts2::QueueOptions const& queue() const { return std::get<::CLOpts2::QueueOptions>(_variant); }
    ::CLOpts2::PauseOptions&       pause() { return std::get<::CLOpts2::PauseOptions>(_variant); }
    ::CLOpts2::PauseOptions const& pause() const { return std::get<::CLOpts2::PauseOptions>(_variant); }
    ::CLOpts2::CancelOptions&       cancel() { return std::get<::CLOpts2::CancelOptions>(_variant); }
    ::CLOpts2::CancelOptions const& cancel() const { return std::get<::CLOpts2::CancelOptions>(_variant); }
    ::CLOpts2::ResumeOptions&       resume() { return std::get<::CLOpts2::ResumeOptions>(_variant); }
    ::CLOpts2::ResumeOptions const& resume() const { return std::get<::CLOpts2::ResumeOptions>(_variant); }
    ::CLOpts2::UpdateOptions&       update() { return std::get<::CLOpts2::UpdateOptions>(_variant); }
    ::CLOpts2::UpdateOptions const& update() const { return std::get<::CLOpts2::UpdateOptions>(_variant); }
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
    enum class Fields
    {
        Invalid,
        Field_ProductId
,        Field_Repair
,        Field_ForceNonSD
,        Field_TargetVolume
,        Field_User
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
,        "Repair"
,        "ForceNonSD"
,        "TargetVolume"
,        "User"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::InstallOptions> : Stencil::TransactionT<CLOpts2::InstallOptions>
{
    using TData  = CLOpts2::InstallOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::InstallOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    Transaction<bool> _subtracker_Repair;
    Transaction<bool> _subtracker_ForceNonSD;
    Transaction<shared_string> _subtracker_TargetVolume;
    Transaction<shared_string> _subtracker_User;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::InstallOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
        ,
        _subtracker_Repair(Obj().Repair)
        ,
        _subtracker_ForceNonSD(Obj().ForceNonSD)
        ,
        _subtracker_TargetVolume(Obj().TargetVolume)
        ,
        _subtracker_User(Obj().User)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    auto& Repair()
    {
        MarkFieldEdited_(Fields::Field_Repair);
        return _subtracker_Repair;
    }
    auto& ForceNonSD()
    {
        MarkFieldEdited_(Fields::Field_ForceNonSD);
        return _subtracker_ForceNonSD;
    }
    auto& TargetVolume()
    {
        MarkFieldEdited_(Fields::Field_TargetVolume);
        return _subtracker_TargetVolume;
    }
    auto& User()
    {
        MarkFieldEdited_(Fields::Field_User);
        return _subtracker_User;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    void set_Repair(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_Repair, Obj().Repair, val);
        Obj().Repair = std::move(val);
    }

    void set_ForceNonSD(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_ForceNonSD, Obj().ForceNonSD, val);
        Obj().ForceNonSD = std::move(val);
    }

    void set_TargetVolume(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_TargetVolume, Obj().TargetVolume, val);
        Obj().TargetVolume = std::move(val);
    }

    void set_User(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_User, Obj().User, val);
        Obj().User = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Field_Repair: return lambda("Repair", Repair());
        case Fields::Field_ForceNonSD: return lambda("ForceNonSD", ForceNonSD());
        case Fields::Field_TargetVolume: return lambda("TargetVolume", TargetVolume());
        case Fields::Field_User: return lambda("User", User());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        if (fieldName == "Repair") { return lambda(Fields::Field_Repair, Repair()); }
        if (fieldName == "ForceNonSD") { return lambda(Fields::Field_ForceNonSD, ForceNonSD()); }
        if (fieldName == "TargetVolume") { return lambda(Fields::Field_TargetVolume, TargetVolume()); }
        if (fieldName == "User") { return lambda(Fields::Field_User, User()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
        lambda("Repair", Fields::Field_Repair, Repair(), Obj().Repair);
        lambda("ForceNonSD", Fields::Field_ForceNonSD, ForceNonSD(), Obj().ForceNonSD);
        lambda("TargetVolume", Fields::Field_TargetVolume, TargetVolume(), Obj().TargetVolume);
        lambda("User", Fields::Field_User, User(), Obj().User);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Repair().Flush();

        if (IsFieldEdited(Fields::Field_Repair))
        {
            if (!Repair().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_Repair));
        }

        ForceNonSD().Flush();

        if (IsFieldEdited(Fields::Field_ForceNonSD))
        {
            if (!ForceNonSD().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ForceNonSD));
        }

        TargetVolume().Flush();

        if (IsFieldEdited(Fields::Field_TargetVolume))
        {
            if (!TargetVolume().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_TargetVolume));
        }

        User().Flush();

        if (IsFieldEdited(Fields::Field_User))
        {
            if (!User().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_User));
        }

        Stencil::TransactionT<CLOpts2::InstallOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::InstallOptions> : Stencil::VisitorT<CLOpts2::InstallOptions>
{
    using TData  = CLOpts2::InstallOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

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

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
        lambda(Fields::Field_Repair, obj.Repair);
        lambda(Fields::Field_ForceNonSD, obj.ForceNonSD);
        lambda(Fields::Field_TargetVolume, obj.TargetVolume);
        lambda(Fields::Field_User, obj.User);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::QueueOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::QueueOptions> : Stencil::TransactionT<CLOpts2::QueueOptions>
{
    using TData  = CLOpts2::QueueOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::QueueOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::QueueOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::QueueOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::QueueOptions> : Stencil::VisitorT<CLOpts2::QueueOptions>
{
    using TData  = CLOpts2::QueueOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::PauseOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::PauseOptions> : Stencil::TransactionT<CLOpts2::PauseOptions>
{
    using TData  = CLOpts2::PauseOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::PauseOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::PauseOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::PauseOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::PauseOptions> : Stencil::VisitorT<CLOpts2::PauseOptions>
{
    using TData  = CLOpts2::PauseOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::CancelOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::CancelOptions> : Stencil::TransactionT<CLOpts2::CancelOptions>
{
    using TData  = CLOpts2::CancelOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::CancelOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::CancelOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::CancelOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::CancelOptions> : Stencil::VisitorT<CLOpts2::CancelOptions>
{
    using TData  = CLOpts2::CancelOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::ResumeOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::ResumeOptions> : Stencil::TransactionT<CLOpts2::ResumeOptions>
{
    using TData  = CLOpts2::ResumeOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::ResumeOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::ResumeOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::ResumeOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::ResumeOptions> : Stencil::VisitorT<CLOpts2::ResumeOptions>
{
    using TData  = CLOpts2::ResumeOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::UpdateOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::UpdateOptions> : Stencil::TransactionT<CLOpts2::UpdateOptions>
{
    using TData  = CLOpts2::UpdateOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::UpdateOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::UpdateOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::UpdateOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::UpdateOptions> : Stencil::VisitorT<CLOpts2::UpdateOptions>
{
    using TData  = CLOpts2::UpdateOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::HydrateOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>
{
    enum class Fields
    {
        Invalid,
        Field_ProductId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "ProductId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts2::HydrateOptions> : Stencil::TransactionT<CLOpts2::HydrateOptions>
{
    using TData  = CLOpts2::HydrateOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts2::HydrateOptions>::Fields;

    Transaction<shared_string> _subtracker_ProductId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts2::HydrateOptions>(ptr)
        ,
        _subtracker_ProductId(Obj().ProductId)
    {}

    auto& ProductId()
    {
        MarkFieldEdited_(Fields::Field_ProductId);
        return _subtracker_ProductId;
    }
    void set_ProductId(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_ProductId, Obj().ProductId, val);
        Obj().ProductId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_ProductId: return lambda("ProductId", ProductId());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "ProductId") { return lambda(Fields::Field_ProductId, ProductId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("ProductId", Fields::Field_ProductId, ProductId(), Obj().ProductId);
    }

    void Flush()
    {
        ProductId().Flush();

        if (IsFieldEdited(Fields::Field_ProductId))
        {
            if (!ProductId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ProductId));
        }

        Stencil::TransactionT<CLOpts2::HydrateOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts2::HydrateOptions> : Stencil::VisitorT<CLOpts2::HydrateOptions>
{
    using TData  = CLOpts2::HydrateOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_ProductId: return lambda(obj.ProductId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_ProductId, obj.ProductId);
    }
};

template <> struct Stencil::TypeTraits<CLOpts2::CommandLineOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts2::CommandLineOptions>
{
    using Key    = CLOpts2::CommandLineOptions::VariantType;
    using Fields = Key;
};

template <> struct Stencil::EnumTraits<CLOpts2::CommandLineOptions::VariantType>
{
    using Enum = CLOpts2::CommandLineOptions::VariantType;

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

    static CLOpts2::CommandLineOptions::VariantType ForIndex(size_t index)
    {
        return static_cast<CLOpts2::CommandLineOptions::VariantType>(index);
    }
};

template <> struct Stencil::Visitor<CLOpts2::CommandLineOptions>
{
    using Fields = CLOpts2::CommandLineOptions::VariantType;

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

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        auto fieldType = static_cast<Fields>(obj.index());
        std::visit([&](auto&& arg) { lambda(fieldType, arg); }, obj._variant);
    }
};

// template <> struct Stencil::Visitor<CLOpts2::CommandLineOptions> : Stencil::StructVisitor<CLOpts2::CommandLineOptions>
// {};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
