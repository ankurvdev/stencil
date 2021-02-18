#pragma once
#include <stencil/stencil.h>
namespace UserData
{
namespace UserData
{
struct Data;
}
namespace Identity
{
struct Data;
}
namespace RemoteHost
{
struct Data;
}
namespace UserData
{

struct Data :
    public ::Database2::OwnerT<::UserData::UserData::Data,::UserData::RemoteHost::Data>,
    public ::Database2::OwnerT<::UserData::UserData::Data,::UserData::Identity::Data>,
    public ::Stencil::TimestampedT<::UserData::UserData::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        modified
,        creation
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::modified:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::creation:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    timestamp _modified = {};

    public:
    timestamp&       modified() { return _modified; }
    const timestamp& modified() const { return _modified; }
    void                            modified(timestamp&& val) { _modified = std::move(val); }
    timestamp&       get_modified() { return _modified; }

    bool isset_modified() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::modified); }

    void set_modified(timestamp&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::modified, _modified, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::modified, _modified, val);
        _modified = std::move(val);
    }

    private:
    timestamp _creation = {};

    public:
    timestamp&       creation() { return _creation; }
    const timestamp& creation() const { return _creation; }
    void                            creation(timestamp&& val) { _creation = std::move(val); }
    timestamp&       get_creation() { return _creation; }

    bool isset_creation() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::creation); }

    void set_creation(timestamp&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::creation, _creation, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::creation, _creation, val);
        _creation = std::move(val);
    }

};

}    // namespace UserData
namespace Identity
{

struct Data :
    public ::Database2::ObjectT<::UserData::UserData::Data,::UserData::Identity::Data>,
    public ::UuidObjectT<::UserData::Identity::Data>,
    public ::Database2::EncryptedT<::UserData::Identity::Data>,
    public ::Stencil::TimestampedT<::UserData::Identity::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        username
,        password
,        privatekey
,        clientcert
,        secretcode
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
        case FieldIndex::username:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::password:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::privatekey:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::clientcert:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::secretcode:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _username = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       username() { return _username; }
    const ::Database2::ChildRef<Database2::WideString>& username() const { return _username; }
    void                            username(::Database2::ChildRef<Database2::WideString>&& val) { _username = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_username() { return _username; }

    bool isset_username() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::username); }

    void set_username(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::username, _username, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::username, _username, val);
        _username = std::move(val);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _password = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       password() { return _password; }
    const ::Database2::ChildRef<Database2::WideString>& password() const { return _password; }
    void                            password(::Database2::ChildRef<Database2::WideString>&& val) { _password = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_password() { return _password; }

    bool isset_password() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::password); }

    void set_password(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::password, _password, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::password, _password, val);
        _password = std::move(val);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _privatekey = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       privatekey() { return _privatekey; }
    const ::Database2::ChildRef<Database2::WideString>& privatekey() const { return _privatekey; }
    void                            privatekey(::Database2::ChildRef<Database2::WideString>&& val) { _privatekey = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_privatekey() { return _privatekey; }

    bool isset_privatekey() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::privatekey); }

    void set_privatekey(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::privatekey, _privatekey, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::privatekey, _privatekey, val);
        _privatekey = std::move(val);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _clientcert = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       clientcert() { return _clientcert; }
    const ::Database2::ChildRef<Database2::WideString>& clientcert() const { return _clientcert; }
    void                            clientcert(::Database2::ChildRef<Database2::WideString>&& val) { _clientcert = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_clientcert() { return _clientcert; }

    bool isset_clientcert() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::clientcert); }

    void set_clientcert(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::clientcert, _clientcert, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::clientcert, _clientcert, val);
        _clientcert = std::move(val);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _secretcode = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       secretcode() { return _secretcode; }
    const ::Database2::ChildRef<Database2::WideString>& secretcode() const { return _secretcode; }
    void                            secretcode(::Database2::ChildRef<Database2::WideString>&& val) { _secretcode = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_secretcode() { return _secretcode; }

    bool isset_secretcode() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::secretcode); }

    void set_secretcode(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::secretcode, _secretcode, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::secretcode, _secretcode, val);
        _secretcode = std::move(val);
    }

};

}    // namespace Identity
namespace RemoteHost
{

struct Data :
    public ::Database2::ObjectT<::UserData::UserData::Data,::UserData::RemoteHost::Data>,
    public ::UuidObjectT<::UserData::RemoteHost::Data>,
    public ::Stencil::TimestampedT<::UserData::RemoteHost::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        name
,        uri
,        identity
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::name:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::uri:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::identity:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _name = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       name() { return _name; }
    const ::Database2::ChildRef<Database2::WideString>& name() const { return _name; }
    void                            name(::Database2::ChildRef<Database2::WideString>&& val) { _name = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_name() { return _name; }

    bool isset_name() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::name); }

    void set_name(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::name, _name, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::name, _name, val);
        _name = std::move(val);
    }

    private:
    ::Database2::ChildRef<Database2::WideString> _uri = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       uri() { return _uri; }
    const ::Database2::ChildRef<Database2::WideString>& uri() const { return _uri; }
    void                            uri(::Database2::ChildRef<Database2::WideString>&& val) { _uri = std::move(val); }
    ::Database2::ChildRef<Database2::WideString>&       get_uri() { return _uri; }

    bool isset_uri() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::uri); }

    void set_uri(::Database2::ChildRef<Database2::WideString>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::uri, _uri, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::uri, _uri, val);
        _uri = std::move(val);
    }

    private:
    ::UuidBasedId<::UserData::Identity::Data> _identity = ::UuidBasedId<::UserData::Identity::Data>::Create();

    public:
    ::UuidBasedId<::UserData::Identity::Data>&       identity() { return _identity; }
    const ::UuidBasedId<::UserData::Identity::Data>& identity() const { return _identity; }
    void                            identity(::UuidBasedId<::UserData::Identity::Data>&& val) { _identity = std::move(val); }
    ::UuidBasedId<::UserData::Identity::Data>&       get_identity() { return _identity; }

    bool isset_identity() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::identity); }

    void set_identity(::UuidBasedId<::UserData::Identity::Data>&& val)
    {
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::identity, _identity, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::identity, _identity, val);
        _identity = std::move(val);
    }

};

}    // namespace RemoteHost
}    // namespace UserData

template <> struct ReflectionBase::TypeTraits<UserData::UserData::Data&>
{
    struct Traits_modified
    {
        using TOwner     = UserData::UserData::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "modified"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_modified; }
        static constexpr auto TPropertySetter() { return &TOwner::set_modified; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::modified, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_creation
    {
        using TOwner     = UserData::UserData::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "creation"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_creation; }
        static constexpr auto TPropertySetter() { return &TOwner::set_creation; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::creation, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "UserData"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = UserData::UserData::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.modified(), obj2.modified())
               && ReflectionBase::AreEqual(obj1.creation(), obj2.creation())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::UserData::Data,
                                                                 Traits_modified
,                                                                 Traits_creation
                                                                 >;
};

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, UserData::UserData::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::modified:
            lambda(DeltaTracker<timestamp>(&_ptr->modified(), IsFieldChanged(TData::FieldIndex::modified)));
            return;
        case TData::FieldIndex::creation:
            lambda(DeltaTracker<timestamp>(&_ptr->creation(), IsFieldChanged(TData::FieldIndex::creation)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

template <> struct ReflectionBase::TypeTraits<UserData::Identity::Data&>
{
    struct Traits_username
    {
        using TOwner     = UserData::Identity::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "username"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_username; }
        static constexpr auto TPropertySetter() { return &TOwner::set_username; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::username, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_password
    {
        using TOwner     = UserData::Identity::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "password"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_password; }
        static constexpr auto TPropertySetter() { return &TOwner::set_password; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::password, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_privatekey
    {
        using TOwner     = UserData::Identity::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "privatekey"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_privatekey; }
        static constexpr auto TPropertySetter() { return &TOwner::set_privatekey; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::privatekey, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_clientcert
    {
        using TOwner     = UserData::Identity::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "clientcert"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_clientcert; }
        static constexpr auto TPropertySetter() { return &TOwner::set_clientcert; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::clientcert, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_secretcode
    {
        using TOwner     = UserData::Identity::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "secretcode"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_secretcode; }
        static constexpr auto TPropertySetter() { return &TOwner::set_secretcode; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::secretcode, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Identity"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = UserData::Identity::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.username(), obj2.username())
               && ReflectionBase::AreEqual(obj1.password(), obj2.password())
               && ReflectionBase::AreEqual(obj1.privatekey(), obj2.privatekey())
               && ReflectionBase::AreEqual(obj1.clientcert(), obj2.clientcert())
               && ReflectionBase::AreEqual(obj1.secretcode(), obj2.secretcode())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::Identity::Data,
                                                                 Traits_username
,                                                                 Traits_password
,                                                                 Traits_privatekey
,                                                                 Traits_clientcert
,                                                                 Traits_secretcode
                                                                 >;
};

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, UserData::Identity::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::username:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->username(), IsFieldChanged(TData::FieldIndex::username)));
            return;
        case TData::FieldIndex::password:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->password(), IsFieldChanged(TData::FieldIndex::password)));
            return;
        case TData::FieldIndex::privatekey:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->privatekey(), IsFieldChanged(TData::FieldIndex::privatekey)));
            return;
        case TData::FieldIndex::clientcert:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->clientcert(), IsFieldChanged(TData::FieldIndex::clientcert)));
            return;
        case TData::FieldIndex::secretcode:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->secretcode(), IsFieldChanged(TData::FieldIndex::secretcode)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

template <> struct ReflectionBase::TypeTraits<UserData::RemoteHost::Data&>
{
    struct Traits_name
    {
        using TOwner     = UserData::RemoteHost::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "name"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_name; }
        static constexpr auto TPropertySetter() { return &TOwner::set_name; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::name, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_uri
    {
        using TOwner     = UserData::RemoteHost::Data;
        using TFieldType = ::Database2::ChildRef<Database2::WideString>;

        static constexpr std::string_view Name() { return "uri"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_uri; }
        static constexpr auto TPropertySetter() { return &TOwner::set_uri; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::uri, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_identity
    {
        using TOwner     = UserData::RemoteHost::Data;
        using TFieldType = ::UuidBasedId<::UserData::Identity::Data>;

        static constexpr std::string_view Name() { return "identity"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_identity; }
        static constexpr auto TPropertySetter() { return &TOwner::set_identity; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::identity, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "RemoteHost"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = UserData::RemoteHost::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.name(), obj2.name())
               && ReflectionBase::AreEqual(obj1.uri(), obj2.uri())
               && ReflectionBase::AreEqual(obj1.identity(), obj2.identity())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::RemoteHost::Data,
                                                                 Traits_name
,                                                                 Traits_uri
,                                                                 Traits_identity
                                                                 >;
};

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, UserData::RemoteHost::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::name:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->name(), IsFieldChanged(TData::FieldIndex::name)));
            return;
        case TData::FieldIndex::uri:
            lambda(DeltaTracker<::Database2::ChildRef<Database2::WideString>>(&_ptr->uri(), IsFieldChanged(TData::FieldIndex::uri)));
            return;
        case TData::FieldIndex::identity:
            lambda(DeltaTracker<::UuidBasedId<::UserData::Identity::Data>>(&_ptr->identity(), IsFieldChanged(TData::FieldIndex::identity)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

