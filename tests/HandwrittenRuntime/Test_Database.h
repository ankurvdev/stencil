#include "UserData.pidl.h"
#include "stencil/Database2.h"

template <>
struct Database2::ObjTraits<UserData::UserData, UserData::RemoteHost>
    : Database2::FixedSizeObjTraits<UserData::UserData, UserData::RemoteHost>
{
    constexpr static Database2::ObjTypeId TypeId() { return 1; }
    static size_t constexpr StructMemberCount() { return 3; }
    template <size_t N> static auto& StructMember(UserData::RemoteHost& obj)
    {
        if constexpr (N == 0) return obj.name;
        if constexpr (N == 1) return obj.uri;
        if constexpr (N == 2) return obj.identity;

        static_assert(N < 3);
    }
};
template <>
struct Database2::ObjTraits<UserData::UserData, UserData::Identity> : Database2::FixedSizeObjTraits<UserData::UserData, UserData::Identity>
{
    constexpr static Database2::ObjTypeId TypeId() { return 2; }
    static size_t constexpr StructMemberCount() { return 5; }
    template <size_t N> static auto& StructMember(UserData::Identity& obj)
    {
        if constexpr (N == 0) return obj.username;
        if constexpr (N == 1) return obj.password;
        if constexpr (N == 2) return obj.privatekey;
        if constexpr (N == 3) return obj.clientcert;
        if constexpr (N == 4) return obj.secretcode;
        static_assert(N < 5);
    }
};

#ifdef TODO1
template <>
void ::ReflectionBase::Construct<UserData::UserData>(UserData::UserData *ptr)
{
    ((Database2baseT<UserData::UserData>*)ptr)->Init(ptr);
}

template <>
struct Database2::ObjTraits<UserData::UserData, UserData::RemoteHost>
{
    using ViewType = typename UserData::RemoteHost const&;
    using SerializedType = UserData::RemoteHost;

    constexpr static Database2::ObjTypeId TypeId() { return 1; }
    constexpr static bool IsBlob() { return false; }

    static const UserData::RemoteHost& CreateViewFromRawData(SerializedType *ptr) { return *ptr; }
    static Database2::CompareResult Compare(UserData::UserData &database, const UserData::RemoteHost &obj1, const UserData::RemoteHost &obj2)
    {
        return (obj1.name() == obj2.name() && obj1.uri() == obj2.uri() && obj1.identity() == obj2.identity()) ? CompareResult::Equal : CompareResult::Greater;
    }

    static void Create(UserData::UserData &database, void *buffer, const std::wstring_view &name, const std::wstring_view &uri, UserData::Identity::Id identityId = UserData::Identity::Id::Invalid())
    {
        auto host = new (buffer) UserData::RemoteHost();
        host->name(database.Create<shared_wstring>(name));
        host->uri(database.Create<shared_wstring>(uri));
        host->identity(identityId);
    }
};

template <>
struct Database2::ObjTraits<UserData::UserData, UserData::Identity>
{
    using ViewType = typename UserData::Identity const&;
    using SerializedType = UserData::Identity;

    constexpr static Database2::ObjTypeId TypeId() { return 2; }
    constexpr static bool IsBlob() { return false; }

    static const UserData::Identity& CreateViewFromRawData(SerializedType *ptr) { return *ptr; }
    static Database2::CompareResult Compare(UserData::UserData &database, const UserData::Identity &obj1, const UserData::Identity &obj2)
    {
        return (
            obj1.username() == obj2.username() &&
            obj1.password() == obj2.password() &&
            obj1.privatekey() == obj2.privatekey() &&
            obj1.secretcode() == obj2.secretcode() &&
            obj1.clientcert() == obj2.clientcert()) ? CompareResult::Equal : CompareResult::Greater;
        //return (obj1.name() == obj2.name() && obj1.uri() == obj2.uri() && obj1.identity() == obj2.identity()) ? CompareResult::Equal : CompareResult::Greater;
    }

    static void Create(UserData::UserData &database, void *buffer)
    {
        auto identity = new (buffer) UserData::Identity();
    };
};
#endif
