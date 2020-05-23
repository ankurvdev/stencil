#include "Test_Database2.h"

#include <catch2/catch.hpp>
#include <thread>

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace Catch
{
template <> struct StringMaker<std::filesystem::file_time_type>
{
    static std::string convert(std::filesystem::file_time_type const& ftime) { return std::to_string(ftime.time_since_epoch().count()); }
};
}    // namespace Catch

using DB = TestData::DataStore;

template <size_t N, typename T> constexpr Uuid TestUuid();

template <> constexpr Uuid TestUuid<0, TestData::Simple>()
{
    return Uuid("{00000000-0000-0000-0000-000000000001}");
};
template <> constexpr Uuid TestUuid<1, TestData::Simple>()
{
    return Uuid("{00000000-0000-0000-0000-000000000002}");
};

template <> constexpr Uuid TestUuid<0, TestData::Shared>()
{
    return Uuid("{00000001-0001-0001-0001-000000000000}");
};
template <> constexpr Uuid TestUuid<1, TestData::Shared>()
{
    return Uuid("{00000001-0001-0001-0001-000000000001}");
};

template <> constexpr Uuid TestUuid<0, TestData::Encrypted>()
{
    return Uuid("{00000002-0002-0002-0002-000000000000}");
};
template <> constexpr Uuid TestUuid<1, TestData::Encrypted>()
{
    return Uuid("{00000002-0002-0002-0002-000000000001}");
};

template <> constexpr Uuid TestUuid<0, TestData::EncryptedAndShared>()
{
    return Uuid("{00000003-0003-0003-0003-000000000000}");
};
template <> constexpr Uuid TestUuid<1, TestData::EncryptedAndShared>()
{
    return Uuid("{00000003-0003-0003-0003-000000000001}");
};

template <> constexpr Uuid TestUuid<0, TestData::WithSimpleRef>()
{
    return Uuid("{00000004-0004-0004-0004-000000000000}");
};
template <> constexpr Uuid TestUuid<1, TestData::WithSimpleRef>()
{
    return Uuid("{00000004-0004-0004-0004-000000000001}");
};

#if TODO_OBJREF
template <> constexpr Uuid TestUuid<TestData::WithString>()
{
    return Uuid("{00000005-0005-0005-0005-000000000000}");
};
template <> constexpr Uuid TestUuid<TestData::WithSharedString>()
{
    return Uuid("{00000006-0006-0006-0006-000000000000}");
};
template <> constexpr Uuid TestUuid<TestData::WithSharedData>()
{
    return Uuid("{00000007-0007-0007-0007-000000000000}");
};
template <> constexpr Uuid TestUuid<TestData::WithEncryptedString>()
{
    return Uuid("{00000008-0008-0008-0008-000000000000}");
};
template <> constexpr Uuid TestUuid<TestData::WithEncryptedSharedString>()
{
    return Uuid("{00000009-0009-0009-0009-000000000000}");
};
#endif

template <size_t N, typename T> constexpr std::string_view TestValue();

template <> constexpr std::string_view TestValue<0, TestData::Simple>()
{
    return "Simple_0";
};
template <> constexpr std::string_view TestValue<1, TestData::Simple>()
{
    return "Simple_1";
};

template <> constexpr std::string_view TestValue<0, TestData::Shared>()
{
    return "Shared_0";
};

template <> constexpr std::string_view TestValue<1, TestData::Shared>()
{
    return "Shared_1";
};

template <> constexpr std::string_view TestValue<0, TestData::Encrypted>()
{
    return "Encrypted_0";
};

template <> constexpr std::string_view TestValue<1, TestData::Encrypted>()
{
    return "Encrypted_1";
};
template <> constexpr std::string_view TestValue<0, TestData::EncryptedAndShared>()
{
    return "EncryptedAndShared_0";
};
template <> constexpr std::string_view TestValue<1, TestData::EncryptedAndShared>()
{
    return "EncryptedAndShared_1";
};
template <> constexpr std::string_view TestValue<0, TestData::WithSimpleRef>()
{
    return "WithSimpleRef_0";
};
template <> constexpr std::string_view TestValue<1, TestData::WithSimpleRef>()
{
    return "WithSimpleRef_1";
};
template <> constexpr std::string_view TestValue<0, Database2::ByteString>()
{
    return "Database2::ByteString_0";
};

template <> constexpr std::string_view TestValue<1, Database2::ByteString>()
{
    return "Database2::ByteString_1";
};

#if TODO_OBJREF
template <> constexpr std::string_view TestValue<TestData::WithString>()
{
    return "WithString";
};
template <> constexpr std::string_view TestValue<TestData::WithSharedString>()
{
    return "WithSharedString";
};
template <> constexpr std::string_view TestValue<TestData::WithSharedData>()
{
    return "WithSharedData";
};
template <> constexpr std::string_view TestValue<TestData::WithEncryptedString>()
{
    return "WithEncryptedString";
};
template <> constexpr std::string_view TestValue<TestData::WithEncryptedSharedString>()
{
    return "WithEncryptedSharedString";
};
#endif

template <size_t N, typename TObj> struct ObjTester;

template <size_t N, typename... TObjs> auto CreateObjects(DB& db)
{
    auto lock = db.LockForEdit();

    using Tuple = std::tuple<ObjTester<N, TObjs>...>;

    std::vector<std::unique_ptr<Tuple>> arrayoftuples(1000);
    for (size_t i = 0; i < 1000; i++)
    {
        arrayoftuples[i].reset(new Tuple(ObjTester<N, TObjs>{i}...));
        std::apply([&](auto&... x) { auto t = std::make_tuple(x.CreateObj(lock, db)...); }, *arrayoftuples[i].get());
    }
    return arrayoftuples;
}

template <size_t N, typename T> void VerifyObjects(T const& arrayoftuples, DB& db)
{
    auto lock = db.LockForRead();
    for (auto& t : arrayoftuples)
    {
        std::apply([&](auto&... x) { auto t = std::make_tuple(x.VerifyObj(lock, db)...); }, *t.get());
    }
}

template <size_t N> struct ObjTester<N, Database2::ByteString>
{
    template <typename TLock> bool CreateObj(TLock const& lock, DB& db)
    {
        auto [ref1, obj] = db.Create<Database2::ByteString>(lock, TestValue<N, Database2::ByteString>());
        ref              = ref1;
        auto obj1        = db.Get(lock, ref);
        REQUIRE(obj == TestValue<N, Database2::ByteString>());
        // REQUIRE(obj1 == TestValue<Database2::ByteString>());
        return true;
    }
    template <typename TLock> bool VerifyObj(TLock const& lock, DB& db)
    {
        auto obj = db.Get(lock, ref);
        REQUIRE(obj == TestValue<N, Database2::ByteString>());
        return true;
    }

    size_t                                    iteration;
    Database2::Ref<DB, Database2::ByteString> ref{};
};

template <size_t N> struct ObjTester<N, TestData::WithSimpleRef>
{
    using Type = TestData::WithSimpleRef;

    template <typename TLock> bool CreateObj(TLock const& lock, DB& db)
    {
        auto [ref1, obj] = db.Create<TestData::WithSimpleRef>(lock,
                                                              TestUuid<N, TestData::Simple>(),
                                                              TestUuid<N, TestData::WithSimpleRef>(),
                                                              TestUuid<N, TestData::Simple>(),
                                                              TestValue<N, Database2::ByteString>());
        ref              = ref1;
        auto obj1        = db.Get(lock, ref);
        REQUIRE(obj.uuid == TestUuid<N, Type>());
        return true;
    }

    template <typename TLock> bool VerifyObj(TLock const& lock, DB& db)
    {
        auto obj = db.Get(lock, ref);
        REQUIRE(obj.uuid == TestUuid<N, Type>());
        // auto subobj = obj.ref1.Get(lock.shared(), db, obj);
        // REQUIRE(subobj.uuid == TestUuid<TestData::Simple>());
        return true;
    }

    size_t                   iteration;
    Database2::Ref<DB, Type> ref;
};

template <typename TObj> void TestCaseForObj()
{
    auto dbFileName = std::to_string(typeid(TObj).hash_code()) + ".bin"s;

    std::filesystem::remove(dbFileName);
    {
        DB                 db{dbFileName};
        ObjTester<0, TObj> tester{0, {}};
        tester.CreateObj(db.LockForEdit(), db);
        tester.VerifyObj(db.LockForRead(), db);
    }
    std::filesystem::remove(dbFileName);
}

#define TEST_CASE_FOR_OBJTYPE(type) \
    TEST_CASE("CodeGen::Database2::" #type, "[Unit]") { TestCaseForObj<type>(); }

#define TEST_CASE_FOR_(type, ...) TEST_CASE_FOR_OBJTYPE(type) TEST_CASE_FOR_(__VA_ARGS__)
#define TEST_CASE_FOR__(arg) TEST_CASE_FOR_ arg
#define TEST_CASE_FOR(args) TEST_CASE_FOR__((args))

#define ALL_TESTED_TYPES Database2::ByteString, TestData::WithSimpleRef
TEST_CASE_FOR_OBJTYPE(Database2::ByteString)
TEST_CASE_FOR_OBJTYPE(TestData::WithSimpleRef)

// TEST_CASE_FOR(ALL_TESTED_TYPES);

TEST_CASE("CodeGen::Database2::SaveAndLoadFile", "[Database2]")
{
    auto dbFileName = "SaveAndLoadFile.bin"s;
    if (std::filesystem::exists(dbFileName)) std::filesystem::remove(dbFileName);

    // Constructor With Path Creates New File and Writes Header
    {
        {
            DB datastore{dbFileName};
        }
        REQUIRE(std::filesystem::exists(dbFileName));
        REQUIRE(std::filesystem::file_size(dbFileName) == (8192 * 2 + 80));
    }

    // Constructor With Path Reads Existing File but doesnt touch it
    {
        auto time = std::filesystem::file_time_type::clock::now();
        {
            DB datastore{dbFileName};
        }
        REQUIRE(std::filesystem::last_write_time(dbFileName) < time);
    }

    // Constructor With IStream reads file
    {
        auto time = std::filesystem::file_time_type::clock::now();
        {
            DB datastore{std::ifstream(dbFileName)};
            CreateObjects<0, ALL_TESTED_TYPES>(datastore);
        }

        REQUIRE(std::filesystem::last_write_time(dbFileName) < time);
    }

    // Empty Constructor for in-memory datastore
    {
        {
            DB datastore{};
            CreateObjects<0, ALL_TESTED_TYPES>(datastore);
        }
        {
            // TODO : Verify nothing written
        }
    }
}

TEST_CASE("CodeGen::Database2::SaveAndLoadObjects", "[Database2]")
{
    auto dbFileName = "SaveAndLoadObjects.bin"s;

    SECTION("Check Objects Write and Read")
    {
        if (std::filesystem::exists(dbFileName)) std::filesystem::remove(dbFileName);

        auto state = [&]() {
            DB datastore{dbFileName};
            return CreateObjects<0, ALL_TESTED_TYPES>(datastore);
        }();

        {
            DB datastore{dbFileName};
            VerifyObjects<0>(state, datastore);
        }
        // AvailableSlot Initialized to right value on reload
        {
            DB datastore{dbFileName};
            VerifyObjects<0>(state, datastore);
            VerifyObjects<1>(CreateObjects<1, ALL_TESTED_TYPES>(datastore), datastore);
            VerifyObjects<0>(state, datastore);
        }
    }

    SECTION("ChildRef Release marks dirty and saves and reloads")
    {
        if (std::filesystem::exists(dbFileName)) std::filesystem::remove(dbFileName);

        auto objs = [&]() {
            DB datastore{dbFileName};
            return CreateObjects<0, TestData::WithSimpleRef>(datastore);
        }();
        auto time = std::filesystem::last_write_time(dbFileName);
        std::this_thread::sleep_for(10ms);    // Looks like the below code is too quick for appreciable difference in modified time
        {
            DB    datastore{dbFileName};
            auto  lock    = datastore.LockForEdit();
            auto& editObj = datastore.Get(lock, std::get<0>(*objs[0]).ref);
            REQUIRE(editObj.ref1.ref.Valid());
            editObj.ref1.Release(lock, datastore);
            REQUIRE(!editObj.ref1.ref.Valid());
        }
        REQUIRE(std::filesystem::last_write_time(dbFileName) > time);
        {
            DB    datastore{dbFileName};
            auto  lock    = datastore.LockForRead();
            auto& editObj = datastore.Get(lock, std::get<0>(*objs[0]).ref);
            REQUIRE(!editObj.ref1.ref.Valid());
        }
    }
    SECTION("Get Edit ref marks the object as dirty")
    {    // FAIL("TODO");
    }
}

TEST_CASE("CodeGen::Database2::UniqueSharedAndSelf", "[Database2]")
{
    auto dbFileName = "UniqueSharedAndSelf.bin"s;

    if (std::filesystem::exists(dbFileName)) std::filesystem::remove(dbFileName);

    SECTION("Self")
    {
        DB store{dbFileName};
        {
            auto lock           = store.LockForEdit();
            auto [ref, objstr1] = store.Create<Database2::ByteString>(lock, TestValue<0, Database2::ByteString>());
            REQUIRE(ref.page > 0);
            REQUIRE(ref.slot >= 0);
            REQUIRE(objstr1 == TestValue<0, Database2::ByteString>());
        }
        {
            auto   lock  = store.LockForRead();
            size_t count = 0;
            for (auto const [ref, obj] : store.Objects<Database2::ByteString>(lock))
            {
                count++;
            }
            REQUIRE(count == 1);
        }
        {
            auto lock = store.LockForEdit();
            for (auto const [ref, obj] : store.Objects<Database2::ByteString>(lock))
            {
                store.Delete(lock, ref);
            }
            size_t count = 0;
            for (auto const [ref, obj] : store.Objects<Database2::ByteString>(lock))
            {
                count++;
            }
            // TODO : REQUIRE(count == 0);

            // TODO : TEst case for verifying that editlock causes a page dirty
        }
    }
}
