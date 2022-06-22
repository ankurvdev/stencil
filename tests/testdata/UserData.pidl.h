#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace UserData
{
struct UserData;
struct Identity;
struct RemoteHost;
}    // namespace UserData

template <> struct Stencil::TypeTraits<UserData::UserData>;
template <> struct Stencil::TypeTraits<UserData::Identity>;
template <> struct Stencil::TypeTraits<UserData::RemoteHost>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace UserData
{
struct UserData :
    public ::Database2::OwnerT<::UserData::UserData,::UserData::RemoteHost>,
    public ::Database2::OwnerT<::UserData::UserData,::UserData::Identity>,
    public ::Stencil::TimestampedT<::UserData::UserData>,
    public Stencil::StructT<UserData>
{
    Stencil::Timestamp modified = {};
    Stencil::Timestamp creation = {};
};
struct Identity :
    public ::Database2::ObjectT<::UserData::UserData,::UserData::Identity>,
    public ::UuidObjectT<::UserData::Identity>,
    public ::Database2::EncryptedT<::UserData::Identity>,
    public ::Stencil::TimestampedT<::UserData::Identity>,
    public Stencil::StructT<Identity>
{
    ::Database2::ChildRef<Database2::WideString> username = {};
    ::Database2::ChildRef<Database2::WideString> password = {};
    ::Database2::ChildRef<Database2::WideString> privatekey = {};
    ::Database2::ChildRef<Database2::WideString> clientcert = {};
    ::Database2::ChildRef<Database2::WideString> secretcode = {};
};
struct RemoteHost :
    public ::Database2::ObjectT<::UserData::UserData,::UserData::RemoteHost>,
    public ::UuidObjectT<::UserData::RemoteHost>,
    public ::Stencil::TimestampedT<::UserData::RemoteHost>,
    public Stencil::StructT<RemoteHost>
{
    ::Database2::ChildRef<Database2::WideString> name = {};
    ::Database2::ChildRef<Database2::WideString> uri = {};
    ::UuidBasedId<::UserData::Identity> identity = ::UuidBasedId<::UserData::Identity>::Create();
};
}    // namespace UserData
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<UserData::UserData>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<UserData::UserData>
{
    enum class Fields
    {
        Invalid,
        Field_modified
,        Field_creation
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<UserData::UserData>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<UserData::UserData>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "modified"
,        "creation"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<UserData::UserData>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<UserData::UserData>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<UserData::UserData> : Stencil::TransactionT<UserData::UserData>
{
    using TData  = UserData::UserData;
    using Fields = Stencil::TypeTraitsForIndexable<UserData::UserData>::Fields;

    Transaction<Stencil::Timestamp> _subtracker_modified;
    Transaction<Stencil::Timestamp> _subtracker_creation;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<UserData::UserData>(ptr)
        ,
        _subtracker_modified(Obj().modified)
        ,
        _subtracker_creation(Obj().creation)
    {}

    auto& modified()
    {
        MarkFieldEdited_(Fields::Field_modified);
        return _subtracker_modified;
    }
    auto& creation()
    {
        MarkFieldEdited_(Fields::Field_creation);
        return _subtracker_creation;
    }
    void set_modified(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_modified, Obj().modified, val);
        Obj().modified = std::move(val);
    }

    void set_creation(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_creation, Obj().creation, val);
        Obj().creation = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_modified: return lambda("modified", modified());
        case Fields::Field_creation: return lambda("creation", creation());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "modified") { return lambda(Fields::Field_modified, modified()); }
        if (fieldName == "creation") { return lambda(Fields::Field_creation, creation()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("modified", Fields::Field_modified, modified(), Obj().modified);
        lambda("creation", Fields::Field_creation, creation(), Obj().creation);
    }

    void Flush()
    {
        modified().Flush();

        if (IsFieldEdited(Fields::Field_modified))
        {
            if (!modified().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_modified));
        }

        creation().Flush();

        if (IsFieldEdited(Fields::Field_creation))
        {
            if (!creation().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_creation));
        }

        Stencil::TransactionT<UserData::UserData>::Flush_();
    }
};

template <> struct Stencil::Visitor<UserData::UserData> : Stencil::VisitorT<UserData::UserData>
{
    using TData  = UserData::UserData;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_modified: return lambda(obj.modified);
        case Fields::Field_creation: return lambda(obj.creation);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_modified, obj.modified);
        lambda(Fields::Field_creation, obj.creation);
    }
};

template <> struct Stencil::TypeTraits<UserData::Identity>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<UserData::Identity>
{
    enum class Fields
    {
        Invalid,
        Field_username
,        Field_password
,        Field_privatekey
,        Field_clientcert
,        Field_secretcode
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<UserData::Identity>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<UserData::Identity>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "username"
,        "password"
,        "privatekey"
,        "clientcert"
,        "secretcode"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<UserData::Identity>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<UserData::Identity>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<UserData::Identity> : Stencil::TransactionT<UserData::Identity>
{
    using TData  = UserData::Identity;
    using Fields = Stencil::TypeTraitsForIndexable<UserData::Identity>::Fields;

    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_username;
    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_password;
    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_privatekey;
    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_clientcert;
    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_secretcode;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<UserData::Identity>(ptr)
        ,
        _subtracker_username(Obj().username)
        ,
        _subtracker_password(Obj().password)
        ,
        _subtracker_privatekey(Obj().privatekey)
        ,
        _subtracker_clientcert(Obj().clientcert)
        ,
        _subtracker_secretcode(Obj().secretcode)
    {}

    auto& username()
    {
        MarkFieldEdited_(Fields::Field_username);
        return _subtracker_username;
    }
    auto& password()
    {
        MarkFieldEdited_(Fields::Field_password);
        return _subtracker_password;
    }
    auto& privatekey()
    {
        MarkFieldEdited_(Fields::Field_privatekey);
        return _subtracker_privatekey;
    }
    auto& clientcert()
    {
        MarkFieldEdited_(Fields::Field_clientcert);
        return _subtracker_clientcert;
    }
    auto& secretcode()
    {
        MarkFieldEdited_(Fields::Field_secretcode);
        return _subtracker_secretcode;
    }
    void set_username(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_username, Obj().username, val);
        Obj().username = std::move(val);
    }

    void set_password(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_password, Obj().password, val);
        Obj().password = std::move(val);
    }

    void set_privatekey(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_privatekey, Obj().privatekey, val);
        Obj().privatekey = std::move(val);
    }

    void set_clientcert(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_clientcert, Obj().clientcert, val);
        Obj().clientcert = std::move(val);
    }

    void set_secretcode(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_secretcode, Obj().secretcode, val);
        Obj().secretcode = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_username: return lambda("username", username());
        case Fields::Field_password: return lambda("password", password());
        case Fields::Field_privatekey: return lambda("privatekey", privatekey());
        case Fields::Field_clientcert: return lambda("clientcert", clientcert());
        case Fields::Field_secretcode: return lambda("secretcode", secretcode());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "username") { return lambda(Fields::Field_username, username()); }
        if (fieldName == "password") { return lambda(Fields::Field_password, password()); }
        if (fieldName == "privatekey") { return lambda(Fields::Field_privatekey, privatekey()); }
        if (fieldName == "clientcert") { return lambda(Fields::Field_clientcert, clientcert()); }
        if (fieldName == "secretcode") { return lambda(Fields::Field_secretcode, secretcode()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("username", Fields::Field_username, username(), Obj().username);
        lambda("password", Fields::Field_password, password(), Obj().password);
        lambda("privatekey", Fields::Field_privatekey, privatekey(), Obj().privatekey);
        lambda("clientcert", Fields::Field_clientcert, clientcert(), Obj().clientcert);
        lambda("secretcode", Fields::Field_secretcode, secretcode(), Obj().secretcode);
    }

    void Flush()
    {
        username().Flush();

        if (IsFieldEdited(Fields::Field_username))
        {
            if (!username().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_username));
        }

        password().Flush();

        if (IsFieldEdited(Fields::Field_password))
        {
            if (!password().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_password));
        }

        privatekey().Flush();

        if (IsFieldEdited(Fields::Field_privatekey))
        {
            if (!privatekey().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_privatekey));
        }

        clientcert().Flush();

        if (IsFieldEdited(Fields::Field_clientcert))
        {
            if (!clientcert().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_clientcert));
        }

        secretcode().Flush();

        if (IsFieldEdited(Fields::Field_secretcode))
        {
            if (!secretcode().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_secretcode));
        }

        Stencil::TransactionT<UserData::Identity>::Flush_();
    }
};

template <> struct Stencil::Visitor<UserData::Identity> : Stencil::VisitorT<UserData::Identity>
{
    using TData  = UserData::Identity;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_username: return lambda(obj.username);
        case Fields::Field_password: return lambda(obj.password);
        case Fields::Field_privatekey: return lambda(obj.privatekey);
        case Fields::Field_clientcert: return lambda(obj.clientcert);
        case Fields::Field_secretcode: return lambda(obj.secretcode);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_username, obj.username);
        lambda(Fields::Field_password, obj.password);
        lambda(Fields::Field_privatekey, obj.privatekey);
        lambda(Fields::Field_clientcert, obj.clientcert);
        lambda(Fields::Field_secretcode, obj.secretcode);
    }
};

template <> struct Stencil::TypeTraits<UserData::RemoteHost>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<UserData::RemoteHost>
{
    enum class Fields
    {
        Invalid,
        Field_name
,        Field_uri
,        Field_identity
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<UserData::RemoteHost>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<UserData::RemoteHost>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "name"
,        "uri"
,        "identity"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<UserData::RemoteHost>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<UserData::RemoteHost>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<UserData::RemoteHost> : Stencil::TransactionT<UserData::RemoteHost>
{
    using TData  = UserData::RemoteHost;
    using Fields = Stencil::TypeTraitsForIndexable<UserData::RemoteHost>::Fields;

    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_name;
    Transaction<::Database2::ChildRef<Database2::WideString>> _subtracker_uri;
    Transaction<::UuidBasedId<::UserData::Identity>> _subtracker_identity;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<UserData::RemoteHost>(ptr)
        ,
        _subtracker_name(Obj().name)
        ,
        _subtracker_uri(Obj().uri)
        ,
        _subtracker_identity(Obj().identity)
    {}

    auto& name()
    {
        MarkFieldEdited_(Fields::Field_name);
        return _subtracker_name;
    }
    auto& uri()
    {
        MarkFieldEdited_(Fields::Field_uri);
        return _subtracker_uri;
    }
    auto& identity()
    {
        MarkFieldEdited_(Fields::Field_identity);
        return _subtracker_identity;
    }
    void set_name(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_name, Obj().name, val);
        Obj().name = std::move(val);
    }

    void set_uri(::Database2::ChildRef<Database2::WideString>&& val)
    {
        MarkFieldAssigned_(Fields::Field_uri, Obj().uri, val);
        Obj().uri = std::move(val);
    }

    void set_identity(::UuidBasedId<::UserData::Identity>&& val)
    {
        MarkFieldAssigned_(Fields::Field_identity, Obj().identity, val);
        Obj().identity = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, [[maybe_unused]] TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_name: return lambda("name", name());
        case Fields::Field_uri: return lambda("uri", uri());
        case Fields::Field_identity: return lambda("identity", identity());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit([[maybe_unused]] std::string_view const& fieldName, [[maybe_unused]] TLambda&& lambda)
    {
        if (fieldName == "name") { return lambda(Fields::Field_name, name()); }
        if (fieldName == "uri") { return lambda(Fields::Field_uri, uri()); }
        if (fieldName == "identity") { return lambda(Fields::Field_identity, identity()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll([[maybe_unused]] TLambda&& lambda)
    {
        lambda("name", Fields::Field_name, name(), Obj().name);
        lambda("uri", Fields::Field_uri, uri(), Obj().uri);
        lambda("identity", Fields::Field_identity, identity(), Obj().identity);
    }

    void Flush()
    {
        name().Flush();

        if (IsFieldEdited(Fields::Field_name))
        {
            if (!name().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_name));
        }

        uri().Flush();

        if (IsFieldEdited(Fields::Field_uri))
        {
            if (!uri().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_uri));
        }

        identity().Flush();

        if (IsFieldEdited(Fields::Field_identity))
        {
            if (!identity().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_identity));
        }

        Stencil::TransactionT<UserData::RemoteHost>::Flush_();
    }
};

template <> struct Stencil::Visitor<UserData::RemoteHost> : Stencil::VisitorT<UserData::RemoteHost>
{
    using TData  = UserData::RemoteHost;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey([[maybe_unused]] T& obj, Fields field, [[maybe_unused]] TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_name: return lambda(obj.name);
        case Fields::Field_uri: return lambda(obj.uri);
        case Fields::Field_identity: return lambda(obj.identity);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        lambda(Fields::Field_name, obj.name);
        lambda(Fields::Field_uri, obj.uri);
        lambda(Fields::Field_identity, obj.identity);
    }
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
