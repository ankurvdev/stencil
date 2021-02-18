#include "UserData.pidl.h"
#include "stencil/Database2.h"

template <>
struct Database2::ObjTraits<UserData::UserData::Data, UserData::RemoteHost::Data>
    : Database2::FixedSizeObjTraits<UserData::UserData::Data, UserData::RemoteHost::Data>
{
    constexpr static Database2::ObjTypeId TypeId() { return 1; }
    static size_t constexpr StructMemberCount() { return 3; }
    template <size_t N> static auto& StructMember(UserData::RemoteHost::Data& obj)
    {
        if constexpr (N == 0) return obj.name();
        if constexpr (N == 1) return obj.uri();
        if constexpr (N == 2) return obj.identity();

        static_assert(N < 3);
    }
};
template <>
struct Database2::ObjTraits<UserData::UserData::Data, UserData::Identity::Data>
    : Database2::FixedSizeObjTraits<UserData::UserData::Data, UserData::Identity::Data>
{
    constexpr static Database2::ObjTypeId TypeId() { return 2; }
    static size_t constexpr StructMemberCount() { return 5; }
    template <size_t N> static auto& StructMember(UserData::Identity::Data& obj)
    {
        if constexpr (N == 0) return obj.username();
        if constexpr (N == 1) return obj.password();
        if constexpr (N == 2) return obj.privatekey();
        if constexpr (N == 3) return obj.clientcert();
        if constexpr (N == 4) return obj.secretcode();
        static_assert(N < 5);
    }
};

#if 0
template <>
void ::ReflectionBase::Construct<UserData::UserData::Data>(UserData::UserData::Data *ptr)
{
    ((Database2::DatabaseT<UserData::UserData::Data>*)ptr)->Init(ptr);
}

template <>
struct Database2::ObjTraits<UserData::UserData::Data, UserData::RemoteHost::Data>
{
    using ViewType = typename UserData::RemoteHost::Data const&;
    using SerializedType = UserData::RemoteHost::Data;

    constexpr static Database2::ObjTypeId TypeId() { return 1; }
    constexpr static bool IsBlob() { return false; }

    static const UserData::RemoteHost::Data& CreateViewFromRawData(SerializedType *ptr) { return *ptr; }
    static Database2::CompareResult Compare(UserData::UserData::Data &database, const UserData::RemoteHost::Data &obj1, const UserData::RemoteHost::Data &obj2)
    {
        return (obj1.name() == obj2.name() && obj1.uri() == obj2.uri() && obj1.identity() == obj2.identity()) ? CompareResult::Equal : CompareResult::Greater;
    }

    static void Create(UserData::UserData::Data &database, void *buffer, const std::wstring_view &name, const std::wstring_view &uri, UserData::Identity::Data::Id identityId = UserData::Identity::Data::Id::Invalid())
    {
        auto host = new (buffer) UserData::RemoteHost::Data();
        host->name(database.Create<shared_wstring>(name));
        host->uri(database.Create<shared_wstring>(uri));
        host->identity(identityId);
    }
};

template <>
struct Database2::ObjTraits<UserData::UserData::Data, UserData::Identity::Data>
{
    using ViewType = typename UserData::Identity::Data const&;
    using SerializedType = UserData::Identity::Data;

    constexpr static Database2::ObjTypeId TypeId() { return 2; }
    constexpr static bool IsBlob() { return false; }

    static const UserData::Identity::Data& CreateViewFromRawData(SerializedType *ptr) { return *ptr; }
    static Database2::CompareResult Compare(UserData::UserData::Data &database, const UserData::Identity::Data &obj1, const UserData::Identity::Data &obj2)
    {
        return (
            obj1.username() == obj2.username() &&
            obj1.password() == obj2.password() &&
            obj1.privatekey() == obj2.privatekey() &&
            obj1.secretcode() == obj2.secretcode() &&
            obj1.clientcert() == obj2.clientcert()) ? CompareResult::Equal : CompareResult::Greater;
        //return (obj1.name() == obj2.name() && obj1.uri() == obj2.uri() && obj1.identity() == obj2.identity()) ? CompareResult::Equal : CompareResult::Greater;
    }

    static void Create(UserData::UserData::Data &database, void *buffer)
    {
        auto identity = new (buffer) UserData::Identity::Data();
    };
};
#endif