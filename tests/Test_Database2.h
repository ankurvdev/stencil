#include "Database2.h"
std::filesystem::path DbFileName();

namespace Database = Database2;

namespace TestData
{
// Do we need to tag each object with the DatabasT

struct DataStore;

struct Simple
{

    static constexpr Database2::ObjTypeId TypeId() { return 1; }
    Uuid                                  uuid;
};

struct Shared
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Shared; }

    static constexpr Database2::ObjTypeId TypeId() { return 2; }

    Uuid uuid;
};

struct Encrypted
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Unique; }

    static constexpr Database2::ObjTypeId TypeId() { return 3; }

    Uuid uuid;
};

struct EncryptedAndShared
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Shared; }

    static constexpr Database2::ObjTypeId TypeId() { return 4; }

    Uuid uuid;
};

struct WithSimpleRef
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Self; }

    static constexpr Database2::ObjTypeId     TypeId() { return 5; }
    Database2::ChildRef<Simple>               ref1;
    Uuid                                      uuid;
    Database2::ChildRef<Simple>               ref2;
    Database2::ChildRef<Database::ByteString> ref3;
};

#if TODO_OBJREF
struct WithString : Database2::DatabaseObjRef<DataStore, WithString, Database::ByteString>
{
    WithString(Uuid const& uuid1, Uuid const& uuid2) :
        Database2::DatabaseObjRef<DataStore, WithString, Database::ByteString>(uuid2), uuid(uuid1)
    {
    }
    Uuid uuid;
};

struct WithSharedString : Database::DatabaseObjRef<DataStore, WithSharedString, Database::ByteSharedString>
{
    WithSharedString(Uuid const& uuid1, Uuid const& uuid2) :
        Database2::DatabaseObjRef<DataStore, WithSharedString, Database::ByteSharedString>(uuid2), uuid(uuid1)
    {
    }
    Uuid uuid;
};

struct WithSharedData : Database::DatabaseObjRef<DataStore, WithSharedData, Shared>
{
    WithSharedData(Uuid const& uuid1, Uuid const& uuid2) : Database2::DatabaseObjRef<DataStore, WithSharedData, Shared>(uuid2), uuid(uuid1)
    {
    }
    Uuid uuid;
};

struct WithEncryptedString : Database::DatabaseObjRef<DataStore, WithEncryptedString, Database::ByteEncryptedString>
{
    WithEncryptedString(Uuid const& uuid1, Uuid const& uuid2) :
        Database2::DatabaseObjRef<DataStore, WithEncryptedString, Database::ByteEncryptedString>(uuid2), uuid(uuid1)
    {
    }
    Uuid uuid;
};

struct WithEncryptedSharedString : Database::DatabaseObjRef<DataStore, WithEncryptedSharedString, Database::ByteEncryptedSharedString>
{
    WithEncryptedSharedString(Uuid const& uuid1, Uuid const& uuid2) :
        Database2::DatabaseObjRef<DataStore, WithEncryptedSharedString, Database::ByteEncryptedSharedString>(uuid2), uuid(uuid1)
    {
    }
    Uuid uuid;
};
#endif
struct DataStore : public Database2::DatabaseT<DataStore>
{
    template <typename... TArgs> DataStore(TArgs&&... args) : Database2::DatabaseT<DataStore>( std::forward<TArgs>(args)...) {}
    DELETE_COPY_AND_MOVE(DataStore);
};
}    // namespace TestData

/*Traits*/
template <>
struct Database2::ObjTraits<TestData::DataStore, TestData::WithSimpleRef>
    : Database2::FixedSizeObjTraits<TestData::DataStore, TestData::WithSimpleRef>
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Self; }
    static size_t constexpr StructMemberCount() { return 4; }
    template <size_t N> static auto& StructMember(TestData::WithSimpleRef& obj)
    {
        if constexpr (N == 0u) return obj.ref1;
        if constexpr (N == 1u) return obj.uuid;
        if constexpr (N == 2u) return obj.ref2;
        if constexpr (N == 3u) return obj.ref3;
        static_assert(N < StructMemberCount());
    }
};

template <>
struct Database2::ObjTraits<TestData::DataStore, TestData::Simple> : Database2::FixedSizeObjTraits<TestData::DataStore, TestData::Simple>
{
    static constexpr auto GetOwnership() { return Database2::Ownership::Unique; }
    static size_t constexpr StructMemberCount() { return 1u; }
    template <size_t N> static auto& StructMember(TestData::Simple& obj)
    {
        if constexpr (N == 0u) return obj.uuid;
        static_assert(N < StructMemberCount());
    }
};

#if TODO_OBJREF
template <> struct Database2::ObjTraits<TestData::DataStore, TestData::WithString>
{
    using WireType = TestData::WithString;
    using SnapType = WireType;
    using ViewType = WireType const&;
    using EditType = WireType&;
    static constexpr ObjTypeId TypeId() { return 5; }
    static constexpr size_t    RecordSize() { return sizeof(WireType); }
};
template <> struct Database2::ObjTraits<TestData::DataStore, TestData::WithSharedString>
{
    using WireType = TestData::WithSharedString;
    using SnapType = WireType;
    using ViewType = WireType const&;
    using EditType = WireType&;
    static constexpr ObjTypeId TypeId() { return 6; }
    static constexpr size_t    RecordSize() { return sizeof(WireType); }
};
template <> struct Database2::ObjTraits<TestData::DataStore, TestData::WithSharedData>
{
    using WireType = TestData::WithSharedData;
    using SnapType = WireType;
    using ViewType = WireType const&;
    using EditType = WireType&;
    static constexpr ObjTypeId TypeId() { return 7; }
    static constexpr size_t    RecordSize() { return sizeof(WireType); }
};
template <> struct Database2::ObjTraits<TestData::DataStore, TestData::WithEncryptedString>
{
    using WireType = TestData::WithEncryptedString;
    using SnapType = WireType;
    using ViewType = WireType const&;
    using EditType = WireType&;
    static constexpr ObjTypeId TypeId() { return 8; }
    static constexpr size_t    RecordSize() { return sizeof(WireType); }
};
template <> struct Database2::ObjTraits<TestData::DataStore, TestData::WithEncryptedSharedString>
{
    using WireType = TestData::WithEncryptedSharedString;
    using SnapType = WireType;
    using ViewType = WireType const&;
    using EditType = WireType&;
    static constexpr ObjTypeId TypeId() { return 9; }
    static constexpr size_t    RecordSize() { return sizeof(WireType); }
};
#endif