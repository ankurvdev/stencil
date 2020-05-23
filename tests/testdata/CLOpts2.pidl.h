#pragma once
#include <DataModel.h>
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

    private:
    bool _Repair = 0;

    public:
    bool&       Repair() { return _Repair; }
    const bool& Repair() const { return _Repair; }
    void                            Repair(bool&& val) { _Repair = std::move(val); }

    bool& get_Repair()
    {
        return _Repair;
        ;
    }
    void set_Repair(bool&& val) { _Repair = std::move(val); }

    private:
    bool _ForceNonSD = 0;

    public:
    bool&       ForceNonSD() { return _ForceNonSD; }
    const bool& ForceNonSD() const { return _ForceNonSD; }
    void                            ForceNonSD(bool&& val) { _ForceNonSD = std::move(val); }

    bool& get_ForceNonSD()
    {
        return _ForceNonSD;
        ;
    }
    void set_ForceNonSD(bool&& val) { _ForceNonSD = std::move(val); }

    private:
    shared_string _TargetVolume = "";

    public:
    shared_string&       TargetVolume() { return _TargetVolume; }
    const shared_string& TargetVolume() const { return _TargetVolume; }
    void                            TargetVolume(shared_string&& val) { _TargetVolume = std::move(val); }

    shared_string& get_TargetVolume()
    {
        return _TargetVolume;
        ;
    }
    void set_TargetVolume(shared_string&& val) { _TargetVolume = std::move(val); }

    private:
    shared_string _User = "";

    public:
    shared_string&       User() { return _User; }
    const shared_string& User() const { return _User; }
    void                            User(shared_string&& val) { _User = std::move(val); }

    shared_string& get_User()
    {
        return _User;
        ;
    }
    void set_User(shared_string&& val) { _User = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::ProductId:
            if (key == "Description") return "Catalog/Product:Sku:Bundle";
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    shared_string& get_ProductId()
    {
        return _ProductId;
        ;
    }
    void set_ProductId(shared_string&& val) { _ProductId = std::move(val); }

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
    void       set_Type(UnionType&& val) { _type = (UnionType)std::move(val); }

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
        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    InstallOptions::Data _install;

    public:
    InstallOptions::Data&       install() { return _install; }
    const InstallOptions::Data& install() const { return _install; }
    void                            install(const InstallOptions::Data& val) { _install = val; }
    void                            install(InstallOptions::Data&& val) { _install = std::move(val); }

    InstallOptions::Data& get_install()
    {
        return _install;
        ;
    }
    void set_install(InstallOptions::Data&& val) { _install = std::move(val); }

    private:
    QueueOptions::Data _queue;

    public:
    QueueOptions::Data&       queue() { return _queue; }
    const QueueOptions::Data& queue() const { return _queue; }
    void                            queue(const QueueOptions::Data& val) { _queue = val; }
    void                            queue(QueueOptions::Data&& val) { _queue = std::move(val); }

    QueueOptions::Data& get_queue()
    {
        return _queue;
        ;
    }
    void set_queue(QueueOptions::Data&& val) { _queue = std::move(val); }

    private:
    PauseOptions::Data _pause;

    public:
    PauseOptions::Data&       pause() { return _pause; }
    const PauseOptions::Data& pause() const { return _pause; }
    void                            pause(const PauseOptions::Data& val) { _pause = val; }
    void                            pause(PauseOptions::Data&& val) { _pause = std::move(val); }

    PauseOptions::Data& get_pause()
    {
        return _pause;
        ;
    }
    void set_pause(PauseOptions::Data&& val) { _pause = std::move(val); }

    private:
    CancelOptions::Data _cancel;

    public:
    CancelOptions::Data&       cancel() { return _cancel; }
    const CancelOptions::Data& cancel() const { return _cancel; }
    void                            cancel(const CancelOptions::Data& val) { _cancel = val; }
    void                            cancel(CancelOptions::Data&& val) { _cancel = std::move(val); }

    CancelOptions::Data& get_cancel()
    {
        return _cancel;
        ;
    }
    void set_cancel(CancelOptions::Data&& val) { _cancel = std::move(val); }

    private:
    ResumeOptions::Data _resume;

    public:
    ResumeOptions::Data&       resume() { return _resume; }
    const ResumeOptions::Data& resume() const { return _resume; }
    void                            resume(const ResumeOptions::Data& val) { _resume = val; }
    void                            resume(ResumeOptions::Data&& val) { _resume = std::move(val); }

    ResumeOptions::Data& get_resume()
    {
        return _resume;
        ;
    }
    void set_resume(ResumeOptions::Data&& val) { _resume = std::move(val); }

    private:
    UpdateOptions::Data _update;

    public:
    UpdateOptions::Data&       update() { return _update; }
    const UpdateOptions::Data& update() const { return _update; }
    void                            update(const UpdateOptions::Data& val) { _update = val; }
    void                            update(UpdateOptions::Data&& val) { _update = std::move(val); }

    UpdateOptions::Data& get_update()
    {
        return _update;
        ;
    }
    void set_update(UpdateOptions::Data&& val) { _update = std::move(val); }

    private:
    HydrateOptions::Data _hydrate;

    public:
    HydrateOptions::Data&       hydrate() { return _hydrate; }
    const HydrateOptions::Data& hydrate() const { return _hydrate; }
    void                            hydrate(const HydrateOptions::Data& val) { _hydrate = val; }
    void                            hydrate(HydrateOptions::Data&& val) { _hydrate = std::move(val); }

    HydrateOptions::Data& get_hydrate()
    {
        return _hydrate;
        ;
    }
    void set_hydrate(HydrateOptions::Data&& val) { _hydrate = std::move(val); }

};
}    // namespace CommandLineOptions
}    // namespace CLOpts2

template <> struct ReflectionBase::TypeTraits<CLOpts2::InstallOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::InstallOptions::Data;

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
        using TOwner = CLOpts2::InstallOptions::Data;

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
        using TOwner = CLOpts2::InstallOptions::Data;

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
        using TOwner = CLOpts2::InstallOptions::Data;

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
        using TOwner = CLOpts2::InstallOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::InstallOptions::Data,
                                                                 Traits_ProductId
,                                                                 Traits_Repair
,                                                                 Traits_ForceNonSD
,                                                                 Traits_TargetVolume
,                                                                 Traits_User
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::QueueOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::QueueOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::QueueOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::PauseOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::PauseOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::PauseOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::CancelOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::CancelOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::CancelOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::ResumeOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::ResumeOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::ResumeOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::UpdateOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::UpdateOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::UpdateOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<CLOpts2::HydrateOptions::Data&>
{
    struct Traits_ProductId
    {
        using TOwner = CLOpts2::HydrateOptions::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts2::HydrateOptions::Data,
                                                                 Traits_ProductId
                                                                 >;
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
                                                  0};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<CLOpts2::CommandLineOptions::UnionType>
{
    static constexpr auto ValueType() { return Value::Type::Integer; }
    static void           Get(Value& /*obj*/) { throw 1; }
    static void           Get(const Value& /*obj*/) { throw 1; }
    static void           Check() { throw 1; }
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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
        using TOwner = CLOpts2::CommandLineOptions::Data;

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
