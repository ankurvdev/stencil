#include "ObjectsTester.h"
#include "TestUtils.h"

using namespace std::literals;
using DataStore = Stencil::Database::Database<::Objects::NestedObject>;
static_assert(Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore> > 0);

struct DatabaseTester
{
    DatabaseTester()
    {
        if (std::filesystem::exists(filepath)) { std::filesystem::remove(filepath); }
        datastore->Init(filepath);
    }

    ~DatabaseTester()
    {
        datastore.reset();
        if (std::filesystem::exists(filepath)) { std::filesystem::remove(filepath); }
    }

    CLASS_DELETE_COPY_AND_MOVE(DatabaseTester);
    void test_create_object1(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_simple_object1();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        REQUIRE(obj1._fieldtracker == refobj1._fieldtracker);
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified); TODO2
        REQUIRE(obj1.val1.data == refobj1.val1);
        REQUIRE(obj1.val2.data == refobj1.val2);
        REQUIRE(obj1.val3.data == refobj1.val3);
        REQUIRE(obj1.val4.id > 0);
        // REQUIRE(obj1.val4 == refobj1.val4);
        REQUIRE(obj1.val5.data == refobj1.val5);

        // auto v46 = std::get<46>(typename DataStore::RecordTypes{});
        // auto v47 = std::get<47>(typename DataStore::RecordTypes{});
        // auto v48 = std::get<48>(typename DataStore::RecordTypes{});
        // auto v49 = std::get<49>(typename DataStore::RecordTypes{});

        generated_ids.insert({Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore>, id1.id});
    }

    void test_create_object2(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_simple_object2();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        REQUIRE(obj1._fieldtracker == refobj1._fieldtracker);
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified); TODO2
        REQUIRE(obj1.val1.data == refobj1.val1);
        REQUIRE(obj1.val2.data == refobj1.val2);
        REQUIRE(obj1.val3.data == refobj1.val3);
        REQUIRE(obj1.val4.data == refobj1.val4);
        REQUIRE(obj1.val5.data == refobj1.val5);
        REQUIRE(obj1.val6.data == refobj1.val6);
        REQUIRE(obj1.val7.data == refobj1.val7);
        REQUIRE(obj1.val8.data == refobj1.val8);
        // REQUIRE(obj1.val4 == refobj1.val4);
        // REQUIRE(obj1.val5.data == refobj1.val5);
        static_assert(Stencil::Database::TypeId<::Objects::SimpleObject2, DataStore>
                          != Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore>,
                      "Cannot have two types equal");
        generated_ids.insert({Stencil::Database::TypeId<::Objects::SimpleObject2, DataStore>, id1.id});
    }

    std::unique_ptr<DataStore>                  datastore = std::make_unique<DataStore>();
    ObjectsTester                               tester;
    std::unordered_multimap<uint16_t, uint32_t> generated_ids;
    std::filesystem::path                       filepath = "SaveAndLoad.bin"s;
};

TEST_CASE("Database", "[database]")
{
    DatabaseTester tester;
    size_t         i = 0;

    auto lock = tester.datastore->LockForEdit();
    for (i = 0; i < 1000; i++) { tester.test_create_object1(lock); }
    for (i = 0; i < 1000; i++) { tester.test_create_object2(lock); }

    tester.datastore->Flush(lock);
}
#if 0
// Count objects in a new session
{
    UserData::UserData database;
    database.Init(filepath);

    {
        auto   lock = database.LockForEdit();
        size_t j    = 0;
        for (auto const [ref, obj] : database.Objects<UserData::RemoteHost>(lock))
        {
            REQUIRE(!obj.identity.Empty());
            auto name = obj.name.Get(lock, database);
            auto uri  = obj.uri.Get(lock, database);
            REQUIRE(name == data[j].name);
            REQUIRE(uri == data[j].uri);
            ++j;
        }
        REQUIRE(j == std::size(data));
    }

    {
        // Create More objects
        auto lock = database.LockForEdit();
        for (auto const& d : data)
        {
            auto [id1, obj1] = database.Create<UserData::Identity>(lock, d.name, d.uri, L"", L"", L"");
            auto [id2, obj2] = database.Create<UserData::RemoteHost>(lock, d.name, d.uri, obj1.id);
            REQUIRE(id1.Valid());
            REQUIRE(id2.Valid());
            auto name = obj2.name.Get(lock, database);
            auto uri  = obj2.uri.Get(lock, database);
            REQUIRE(name == d.name);
            REQUIRE(uri == d.uri);
        }
    }
    {
        auto lock = database.LockForRead();
        // Count of objects should be double now
        size_t j = 0;
        for (auto const [ref, obj] : database.Objects<UserData::RemoteHost>(lock))
        {
            auto name = obj.name.Get(lock, database);
            auto uri  = obj.uri.Get(lock, database);
            REQUIRE(name == data[j % std::size(data)].name);
            REQUIRE(uri == data[j % std::size(data)].uri);
            ++j;
        }
        REQUIRE(j == 2 * std::size(data));
    }
}
std::filesystem::remove(filepath);
}
#endif