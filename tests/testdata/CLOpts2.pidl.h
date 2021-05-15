#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace CLOpts2::InstallOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::InstallOptions::Data&>;
namespace CLOpts2::QueueOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::QueueOptions::Data&>;
namespace CLOpts2::PauseOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::PauseOptions::Data&>;
namespace CLOpts2::CancelOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::CancelOptions::Data&>;
namespace CLOpts2::ResumeOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::ResumeOptions::Data&>;
namespace CLOpts2::UpdateOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::UpdateOptions::Data&>;
namespace CLOpts2::HydrateOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts2::HydrateOptions::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace CLOpts2
{
namespace InstallOptions
{
struct Data;
}
namespace QueueOptions
{
struct Data;
}
namespace PauseOptions
{
struct Data;
}
namespace CancelOptions
{
struct Data;
}
namespace ResumeOptions
{
struct Data;
}
namespace UpdateOptions
{
struct Data;
}
namespace HydrateOptions
{
struct Data;
}
namespace InstallOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
,        Repair
,        ForceNonSD
,        TargetVolume
,        User
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Repair:
            if (key == "Description") return "Repair existing installated product";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::ForceNonSD:
            if (key == "Description") return "Force installation to non SD";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::TargetVolume:
            if (key == "Description") return "Target Volume";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::User:
            if (key == "Description") return "User to install for";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = {};

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
    private:
    bool _Repair = 0;

    public:
    bool&       Repair() { return _Repair; }
    const bool& Repair() const { return _Repair; }
    void                            Repair(bool&& val) { _Repair = std::move(val); }
    bool&       get_Repair() { return _Repair; }

    bool isset_Repair() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::Repair); }

    void set_Repair(bool&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::Repair, _Repair, val);
        _Repair = std::move(val);
    }

#if 0
#endif
    private:
    bool _ForceNonSD = 0;

    public:
    bool&       ForceNonSD() { return _ForceNonSD; }
    const bool& ForceNonSD() const { return _ForceNonSD; }
    void                            ForceNonSD(bool&& val) { _ForceNonSD = std::move(val); }
    bool&       get_ForceNonSD() { return _ForceNonSD; }

    bool isset_ForceNonSD() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ForceNonSD); }

    void set_ForceNonSD(bool&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ForceNonSD, _ForceNonSD, val);
        _ForceNonSD = std::move(val);
    }

#if 0
#endif
    private:
    shared_string _TargetVolume = "";

    public:
    shared_string&       TargetVolume() { return _TargetVolume; }
    const shared_string& TargetVolume() const { return _TargetVolume; }
    void                            TargetVolume(shared_string&& val) { _TargetVolume = std::move(val); }
    shared_string&       get_TargetVolume() { return _TargetVolume; }

    bool isset_TargetVolume() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::TargetVolume); }

    void set_TargetVolume(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::TargetVolume, _TargetVolume, val);
        _TargetVolume = std::move(val);
    }

#if 0
#endif
    private:
    shared_string _User = "";

    public:
    shared_string&       User() { return _User; }
    const shared_string& User() const { return _User; }
    void                            User(shared_string&& val) { _User = std::move(val); }
    shared_string&       get_User() { return _User; }

    bool isset_User() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::User); }

    void set_User(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::User, _User, val);
        _User = std::move(val);
    }

#if 0
#endif
};

}    // namespace InstallOptions
namespace QueueOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = "";

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace QueueOptions
namespace PauseOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = {};

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace PauseOptions
namespace CancelOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = {};

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace CancelOptions
namespace ResumeOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = {};

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace ResumeOptions
namespace UpdateOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = "";

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace UpdateOptions
namespace HydrateOptions
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        ProductId
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _ProductId = {};

    public:
    shared_string&       ProductId() { return _ProductId; }
    const shared_string& ProductId() const { return _ProductId; }
    void                            ProductId(shared_string&& val) { _ProductId = std::move(val); }
    shared_string&       get_ProductId() { return _ProductId; }

    bool isset_ProductId() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ProductId); }

    void set_ProductId(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ProductId, _ProductId, val);
        _ProductId = std::move(val);
    }

#if 0
#endif
};

}    // namespace HydrateOptions
namespace CommandLineOptions
{
struct Union;
}
namespace CommandLineOptions
{
enum class UnionType
{
    Invalid,
    install
,    queue
,    pause
,    cancel
,    resume
,    update
,    hydrate
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
        install
,        queue
,        pause
,        cancel
,        resume
,        update
,        hydrate
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::install:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::queue:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::pause:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::cancel:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::resume:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::update:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::hydrate:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    ::CLOpts2::InstallOptions::Data _install;

    public:
    ::CLOpts2::InstallOptions::Data&       install() { return _install; }
    const ::CLOpts2::InstallOptions::Data& install() const { return _install; }
    void                            install(const ::CLOpts2::InstallOptions::Data& val) { _install = val; }
    void                            install(::CLOpts2::InstallOptions::Data&& val) { _install = std::move(val); }

    ::CLOpts2::InstallOptions::Data& get_install() { return _install; }
    void                      set_install(::CLOpts2::InstallOptions::Data&& val) { _install = std::move(val); }

    private:
    ::CLOpts2::QueueOptions::Data _queue;

    public:
    ::CLOpts2::QueueOptions::Data&       queue() { return _queue; }
    const ::CLOpts2::QueueOptions::Data& queue() const { return _queue; }
    void                            queue(const ::CLOpts2::QueueOptions::Data& val) { _queue = val; }
    void                            queue(::CLOpts2::QueueOptions::Data&& val) { _queue = std::move(val); }

    ::CLOpts2::QueueOptions::Data& get_queue() { return _queue; }
    void                      set_queue(::CLOpts2::QueueOptions::Data&& val) { _queue = std::move(val); }

    private:
    ::CLOpts2::PauseOptions::Data _pause;

    public:
    ::CLOpts2::PauseOptions::Data&       pause() { return _pause; }
    const ::CLOpts2::PauseOptions::Data& pause() const { return _pause; }
    void                            pause(const ::CLOpts2::PauseOptions::Data& val) { _pause = val; }
    void                            pause(::CLOpts2::PauseOptions::Data&& val) { _pause = std::move(val); }

    ::CLOpts2::PauseOptions::Data& get_pause() { return _pause; }
    void                      set_pause(::CLOpts2::PauseOptions::Data&& val) { _pause = std::move(val); }

    private:
    ::CLOpts2::CancelOptions::Data _cancel;

    public:
    ::CLOpts2::CancelOptions::Data&       cancel() { return _cancel; }
    const ::CLOpts2::CancelOptions::Data& cancel() const { return _cancel; }
    void                            cancel(const ::CLOpts2::CancelOptions::Data& val) { _cancel = val; }
    void                            cancel(::CLOpts2::CancelOptions::Data&& val) { _cancel = std::move(val); }

    ::CLOpts2::CancelOptions::Data& get_cancel() { return _cancel; }
    void                      set_cancel(::CLOpts2::CancelOptions::Data&& val) { _cancel = std::move(val); }

    private:
    ::CLOpts2::ResumeOptions::Data _resume;

    public:
    ::CLOpts2::ResumeOptions::Data&       resume() { return _resume; }
    const ::CLOpts2::ResumeOptions::Data& resume() const { return _resume; }
    void                            resume(const ::CLOpts2::ResumeOptions::Data& val) { _resume = val; }
    void                            resume(::CLOpts2::ResumeOptions::Data&& val) { _resume = std::move(val); }

    ::CLOpts2::ResumeOptions::Data& get_resume() { return _resume; }
    void                      set_resume(::CLOpts2::ResumeOptions::Data&& val) { _resume = std::move(val); }

    private:
    ::CLOpts2::UpdateOptions::Data _update;

    public:
    ::CLOpts2::UpdateOptions::Data&       update() { return _update; }
    const ::CLOpts2::UpdateOptions::Data& update() const { return _update; }
    void                            update(const ::CLOpts2::UpdateOptions::Data& val) { _update = val; }
    void                            update(::CLOpts2::UpdateOptions::Data&& val) { _update = std::move(val); }

    ::CLOpts2::UpdateOptions::Data& get_update() { return _update; }
    void                      set_update(::CLOpts2::UpdateOptions::Data&& val) { _update = std::move(val); }

    private:
    ::CLOpts2::HydrateOptions::Data _hydrate;

    public:
    ::CLOpts2::HydrateOptions::Data&       hydrate() { return _hydrate; }
    const ::CLOpts2::HydrateOptions::Data& hydrate() const { return _hydrate; }
    void                            hydrate(const ::CLOpts2::HydrateOptions::Data& val) { _hydrate = val; }
    void                            hydrate(::CLOpts2::HydrateOptions::Data&& val) { _hydrate = std::move(val); }

    ::CLOpts2::HydrateOptions::Data& get_hydrate() { return _hydrate; }
    void                      set_hydrate(::CLOpts2::HydrateOptions::Data&& val) { _hydrate = std::move(val); }

};
}    // namespace CommandLineOptions
}    // namespace CLOpts2
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct ReflectionBase::TypeTraits<CLOpts2::InstallOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::InstallOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_Repair
    {
        using TOwner     = CLOpts2::InstallOptions::Data;
        using TFieldType = bool;

        static constexpr std::string_view Name() { return "Repair"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_Repair; }
        static constexpr auto TPropertySetter() { return &TOwner::set_Repair; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::Repair, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_ForceNonSD
    {
        using TOwner     = CLOpts2::InstallOptions::Data;
        using TFieldType = bool;

        static constexpr std::string_view Name() { return "ForceNonSD"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ForceNonSD; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ForceNonSD; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ForceNonSD, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_TargetVolume
    {
        using TOwner     = CLOpts2::InstallOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "TargetVolume"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_TargetVolume; }
        static constexpr auto TPropertySetter() { return &TOwner::set_TargetVolume; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::TargetVolume, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_User
    {
        using TOwner     = CLOpts2::InstallOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "User"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_User; }
        static constexpr auto TPropertySetter() { return &TOwner::set_User; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::User, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "InstallOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Install a specific product";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::InstallOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
               && ReflectionBase::AreEqual(obj1.Repair(), obj2.Repair())
               && ReflectionBase::AreEqual(obj1.ForceNonSD(), obj2.ForceNonSD())
               && ReflectionBase::AreEqual(obj1.TargetVolume(), obj2.TargetVolume())
               && ReflectionBase::AreEqual(obj1.User(), obj2.User())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::InstallOptions::Data,
                                                                 Traits_ProductId
,                                                                 Traits_Repair
,                                                                 Traits_ForceNonSD
,                                                                 Traits_TargetVolume
,                                                                 Traits_User
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::InstallOptions::Data> : Stencil::TransactionT<CLOpts2::InstallOptions::Data>
{
    using TData = CLOpts2::InstallOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    Transaction<bool> _subtracker_Repair;
    Transaction<bool> _subtracker_ForceNonSD;
    Transaction<shared_string> _subtracker_TargetVolume;
    Transaction<shared_string> _subtracker_User;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::InstallOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
        ,
        _subtracker_Repair(Obj().Repair(), rec)
        ,
        _subtracker_ForceNonSD(Obj().ForceNonSD(), rec)
        ,
        _subtracker_TargetVolume(Obj().TargetVolume(), rec)
        ,
        _subtracker_User(Obj().User(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Repair: lambda(_subtracker_Repair); return;
        case TData::FieldIndex::ForceNonSD: lambda(_subtracker_ForceNonSD); return;
        case TData::FieldIndex::TargetVolume: lambda(_subtracker_TargetVolume); return;
        case TData::FieldIndex::User: lambda(_subtracker_User); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Repair: lambda(_subtracker_Repair); return;
        case TData::FieldIndex::ForceNonSD: lambda(_subtracker_ForceNonSD); return;
        case TData::FieldIndex::TargetVolume: lambda(_subtracker_TargetVolume); return;
        case TData::FieldIndex::User: lambda(_subtracker_User); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

    void set_Repair(bool&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::Repair, Obj().Repair(), val);
        Obj().set_Repair(std::move(val));
    }

    void set_ForceNonSD(bool&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ForceNonSD, Obj().ForceNonSD(), val);
        Obj().set_ForceNonSD(std::move(val));
    }

    void set_TargetVolume(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::TargetVolume, Obj().TargetVolume(), val);
        Obj().set_TargetVolume(std::move(val));
    }

    void set_User(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::User, Obj().User(), val);
        Obj().set_User(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::QueueOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::QueueOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "QueueOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Listen for progress notifications for a specific product or all queued products";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::QueueOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::QueueOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::QueueOptions::Data> : Stencil::TransactionT<CLOpts2::QueueOptions::Data>
{
    using TData = CLOpts2::QueueOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::QueueOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::PauseOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::PauseOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "PauseOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Pause the given product";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::PauseOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::PauseOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::PauseOptions::Data> : Stencil::TransactionT<CLOpts2::PauseOptions::Data>
{
    using TData = CLOpts2::PauseOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::PauseOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::CancelOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::CancelOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "CancelOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Cancel the given product";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::CancelOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::CancelOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::CancelOptions::Data> : Stencil::TransactionT<CLOpts2::CancelOptions::Data>
{
    using TData = CLOpts2::CancelOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::CancelOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::ResumeOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::ResumeOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "ResumeOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Resume the given product";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::ResumeOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::ResumeOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::ResumeOptions::Data> : Stencil::TransactionT<CLOpts2::ResumeOptions::Data>
{
    using TData = CLOpts2::ResumeOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::ResumeOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::UpdateOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::UpdateOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "UpdateOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Check for updates for a given product or all installed products";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::UpdateOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::UpdateOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::UpdateOptions::Data> : Stencil::TransactionT<CLOpts2::UpdateOptions::Data>
{
    using TData = CLOpts2::UpdateOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::UpdateOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionBase::TypeTraits<CLOpts2::HydrateOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner     = CLOpts2::HydrateOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "ProductId"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ProductId; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ProductId; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ProductId, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "HydrateOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Download document for a given product";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::HydrateOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.ProductId(), obj2.ProductId())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::HydrateOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts2::HydrateOptions::Data> : Stencil::TransactionT<CLOpts2::HydrateOptions::Data>
{
    using TData = CLOpts2::HydrateOptions::Data;

    Transaction<shared_string> _subtracker_ProductId;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr, TransactionRecorder& rec) :
        Stencil::TransactionT<CLOpts2::HydrateOptions::Data>(ptr, rec)
        ,
        _subtracker_ProductId(Obj().ProductId(), rec)
    {
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::ProductId: lambda(_subtracker_ProductId); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_ProductId(shared_string&& val)
    {
        OnStructFieldChangeRequested(TData::FieldIndex::ProductId, Obj().ProductId(), val);
        Obj().set_ProductId(std::move(val));
    }

};

template <> struct ReflectionServices::EnumTraits<CLOpts2::CommandLineOptions::UnionType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  "install",
                                                  "queue",
                                                  "pause",
                                                  "cancel",
                                                  "resume",
                                                  "update",
                                                  "hydrate",
                                                  nullptr};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<CLOpts2::CommandLineOptions::UnionType>
{
    static constexpr auto    ValueType() { return Value::Type::Unsigned; }
    [[noreturn]] static void Get(Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Get(const Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Check() { throw std::logic_error("Not Implemented"); }
};

template <> struct ReflectionBase::TypeTraits<CLOpts2::CommandLineOptions::UnionType&>
{
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Value; }
    static constexpr std::string_view           Name() { return "CommandLineOptions"; }

    using Handler = ::ReflectionServices::EnumHandler<CLOpts2::CommandLineOptions::UnionType>;
};

template <> struct ReflectionBase::TypeTraits<CLOpts2::CommandLineOptions::Data&>
{
    struct Traits_install
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::InstallOptions::Data;

        static constexpr std::string_view Name() { return "install"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_install; }
        static constexpr auto TPropertySetter() { return &TOwner::set_install; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::install, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_queue
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::QueueOptions::Data;

        static constexpr std::string_view Name() { return "queue"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_queue; }
        static constexpr auto TPropertySetter() { return &TOwner::set_queue; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::queue, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_pause
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::PauseOptions::Data;

        static constexpr std::string_view Name() { return "pause"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_pause; }
        static constexpr auto TPropertySetter() { return &TOwner::set_pause; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::pause, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_cancel
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::CancelOptions::Data;

        static constexpr std::string_view Name() { return "cancel"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_cancel; }
        static constexpr auto TPropertySetter() { return &TOwner::set_cancel; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::cancel, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_resume
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::ResumeOptions::Data;

        static constexpr std::string_view Name() { return "resume"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_resume; }
        static constexpr auto TPropertySetter() { return &TOwner::set_resume; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::resume, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_update
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::UpdateOptions::Data;

        static constexpr std::string_view Name() { return "update"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_update; }
        static constexpr auto TPropertySetter() { return &TOwner::set_update; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::update, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_hydrate
    {
        using TOwner     = CLOpts2::CommandLineOptions::Data;
        using TFieldType = ::CLOpts2::HydrateOptions::Data;

        static constexpr std::string_view Name() { return "hydrate"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_hydrate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_hydrate; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::hydrate, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "CommandLineOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        if (key == "Description") return "Command Line tool for windows-store operations";
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts2::CommandLineOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.install(), obj2.install())
               && ReflectionBase::AreEqual(obj1.queue(), obj2.queue())
               && ReflectionBase::AreEqual(obj1.pause(), obj2.pause())
               && ReflectionBase::AreEqual(obj1.cancel(), obj2.cancel())
               && ReflectionBase::AreEqual(obj1.resume(), obj2.resume())
               && ReflectionBase::AreEqual(obj1.update(), obj2.update())
               && ReflectionBase::AreEqual(obj1.hydrate(), obj2.hydrate())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedUnionHandler<CLOpts2::CommandLineOptions::Data,
                                                                CLOpts2::CommandLineOptions::UnionType,
                                                                Traits_install
,                                                                Traits_queue
,                                                                Traits_pause
,                                                                Traits_cancel
,                                                                Traits_resume
,                                                                Traits_update
,                                                                Traits_hydrate
                                                                >;
};
#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
