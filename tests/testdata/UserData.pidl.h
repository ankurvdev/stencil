#pragma once
#include <DataModel.h>
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
    public ::Database2::OwnerT<UserData::Data,RemoteHost::Data>,
    public ::Database2::OwnerT<UserData::Data,Identity::Data>,
    public ::TimestampedT<UserData::Data>,
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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::modified:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::creation:
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    timestamp& get_modified()
    {
        return _modified;
        ;
    }
    void set_modified(timestamp&& val) { _modified = std::move(val); }

    private:
    timestamp _creation = {};

    public:
    timestamp&       creation() { return _creation; }
    const timestamp& creation() const { return _creation; }
    void                            creation(timestamp&& val) { _creation = std::move(val); }

    timestamp& get_creation()
    {
        return _creation;
        ;
    }
    void set_creation(timestamp&& val) { _creation = std::move(val); }

};

}    // namespace UserData
namespace Identity
{

struct Data :
    public ::Database2::ObjectT<UserData::Data,Identity::Data>,
    public ::UuidObjectT<Identity::Data>,
    public ::Database2::EncryptedT<Identity::Data>,
    public ::TimestampedT<Identity::Data>,
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

    ::Database2::ChildRef<Database2::WideString>& get_username()
    {
        return _username;
        ;
    }
    void set_username(::Database2::ChildRef<Database2::WideString>&& val) { _username = std::move(val); }

    private:
    ::Database2::ChildRef<Database2::WideString> _password = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       password() { return _password; }
    const ::Database2::ChildRef<Database2::WideString>& password() const { return _password; }
    void                            password(::Database2::ChildRef<Database2::WideString>&& val) { _password = std::move(val); }

    ::Database2::ChildRef<Database2::WideString>& get_password()
    {
        return _password;
        ;
    }
    void set_password(::Database2::ChildRef<Database2::WideString>&& val) { _password = std::move(val); }

    private:
    ::Database2::ChildRef<Database2::WideString> _privatekey = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       privatekey() { return _privatekey; }
    const ::Database2::ChildRef<Database2::WideString>& privatekey() const { return _privatekey; }
    void                            privatekey(::Database2::ChildRef<Database2::WideString>&& val) { _privatekey = std::move(val); }

    ::Database2::ChildRef<Database2::WideString>& get_privatekey()
    {
        return _privatekey;
        ;
    }
    void set_privatekey(::Database2::ChildRef<Database2::WideString>&& val) { _privatekey = std::move(val); }

    private:
    ::Database2::ChildRef<Database2::WideString> _clientcert = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       clientcert() { return _clientcert; }
    const ::Database2::ChildRef<Database2::WideString>& clientcert() const { return _clientcert; }
    void                            clientcert(::Database2::ChildRef<Database2::WideString>&& val) { _clientcert = std::move(val); }

    ::Database2::ChildRef<Database2::WideString>& get_clientcert()
    {
        return _clientcert;
        ;
    }
    void set_clientcert(::Database2::ChildRef<Database2::WideString>&& val) { _clientcert = std::move(val); }

    private:
    ::Database2::ChildRef<Database2::WideString> _secretcode = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       secretcode() { return _secretcode; }
    const ::Database2::ChildRef<Database2::WideString>& secretcode() const { return _secretcode; }
    void                            secretcode(::Database2::ChildRef<Database2::WideString>&& val) { _secretcode = std::move(val); }

    ::Database2::ChildRef<Database2::WideString>& get_secretcode()
    {
        return _secretcode;
        ;
    }
    void set_secretcode(::Database2::ChildRef<Database2::WideString>&& val) { _secretcode = std::move(val); }

};

}    // namespace Identity
namespace RemoteHost
{

struct Data :
    public ::Database2::ObjectT<UserData::Data,RemoteHost::Data>,
    public ::UuidObjectT<RemoteHost::Data>,
    public ::TimestampedT<RemoteHost::Data>,
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

    ::Database2::ChildRef<Database2::WideString>& get_name()
    {
        return _name;
        ;
    }
    void set_name(::Database2::ChildRef<Database2::WideString>&& val) { _name = std::move(val); }

    private:
    ::Database2::ChildRef<Database2::WideString> _uri = {};

    public:
    ::Database2::ChildRef<Database2::WideString>&       uri() { return _uri; }
    const ::Database2::ChildRef<Database2::WideString>& uri() const { return _uri; }
    void                            uri(::Database2::ChildRef<Database2::WideString>&& val) { _uri = std::move(val); }

    ::Database2::ChildRef<Database2::WideString>& get_uri()
    {
        return _uri;
        ;
    }
    void set_uri(::Database2::ChildRef<Database2::WideString>&& val) { _uri = std::move(val); }

    private:
    ::UuidBasedId<Identity::Data> _identity = ::UuidBasedId<Identity::Data>::Create();

    public:
    ::UuidBasedId<Identity::Data>&       identity() { return _identity; }
    const ::UuidBasedId<Identity::Data>& identity() const { return _identity; }
    void                            identity(::UuidBasedId<Identity::Data>&& val) { _identity = std::move(val); }

    ::UuidBasedId<Identity::Data>& get_identity()
    {
        return _identity;
        ;
    }
    void set_identity(::UuidBasedId<Identity::Data>&& val) { _identity = std::move(val); }

};

}    // namespace RemoteHost
}    // namespace UserData

template <> struct ReflectionBase::TypeTraits<UserData::UserData::Data&>
{
    struct Traits_modified
    {
        using TOwner = UserData::UserData::Data;

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
        using TOwner = UserData::UserData::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::UserData::Data,
                                                                 Traits_modified
,                                                                 Traits_creation
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<UserData::Identity::Data&>
{
    struct Traits_username
    {
        using TOwner = UserData::Identity::Data;

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
        using TOwner = UserData::Identity::Data;

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
        using TOwner = UserData::Identity::Data;

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
        using TOwner = UserData::Identity::Data;

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
        using TOwner = UserData::Identity::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::Identity::Data,
                                                                 Traits_username
,                                                                 Traits_password
,                                                                 Traits_privatekey
,                                                                 Traits_clientcert
,                                                                 Traits_secretcode
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<UserData::RemoteHost::Data&>
{
    struct Traits_name
    {
        using TOwner = UserData::RemoteHost::Data;

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
        using TOwner = UserData::RemoteHost::Data;

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
        using TOwner = UserData::RemoteHost::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<UserData::RemoteHost::Data,
                                                                 Traits_name
,                                                                 Traits_uri
,                                                                 Traits_identity
                                                                 >;
};
