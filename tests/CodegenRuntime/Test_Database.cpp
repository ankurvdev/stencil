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
    void check(Stencil::Database::RWLock& /*lock*/,
               Objects::SimpleObject1 const&                            ref,
               Stencil::Database::Record<Objects::SimpleObject1> const& rec)
    {
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        REQUIRE(rec._fieldtracker == ref._fieldtracker);
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified); TODO2
        REQUIRE(rec.val1.data == ref.val1);
        REQUIRE(rec.val2.data == ref.val2);
        REQUIRE(rec.val3.data == ref.val3);
        REQUIRE(rec.val4.id > 0);
        // REQUIRE(obj1.val4 == refobj1.val4);
        REQUIRE(rec.val5.data == ref.val5);
    }

    void check(Stencil::Database::RWLock& /*lock*/,
               Objects::SimpleObject2 const&                            ref,
               Stencil::Database::Record<Objects::SimpleObject2> const& rec)
    {
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        REQUIRE(rec._fieldtracker == ref._fieldtracker);
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified); TODO2
        REQUIRE(rec.val1.data == ref.val1);
        REQUIRE(rec.val2.data == ref.val2);
        REQUIRE(rec.val3.data == ref.val3);
        REQUIRE(rec.val4.data == ref.val4);
        REQUIRE(rec.val5.data == ref.val5);
        REQUIRE(rec.val6.data == ref.val6);
        REQUIRE(rec.val7.data == ref.val7);
        REQUIRE(rec.val8.data == ref.val8);
    }

    void check(Stencil::Database::RWLock& lock, Objects::ListObject const& ref, Stencil::Database::Record<Objects::ListObject> const& rec)
    {
        check(lock, ref.obj1, datastore->Get(lock, rec.obj1));
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        // REQUIRE(rec.lastmodified == ref.lastmodified); TODO2
        REQUIRE(rec.value.data == ref.value);
    }

    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<uint32_t, std::unordered_map<uint32_t, ::Objects::SimpleObject1>> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<uint32_t, std::unordered_map<uint32_t, ::Objects::SimpleObject1>>> const& rec)
    {
        for (auto item : rec.Items())
        {
            auto krec = datastore->Get(lock, item.k);
            auto vrec = datastore->Get(lock, item.v);
        }
    }

    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<uint32_t, ::Objects::SimpleObject1> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<uint32_t, ::Objects::SimpleObject1>> const& rec)
    {
        for (auto item : rec.Items())
        {
            auto krec = datastore->Get(lock, item.k);
            auto vrec = datastore->Get(lock, item.v);
        }
    }
    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<shared_string, Stencil::Timestamp> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<shared_string, Stencil::Timestamp>> const& rec)
    {
        for (auto item : rec.Items())
        {
            auto krec = datastore->Get(lock, item.k);
            auto vrec = datastore->Get(lock, item.v);
        }
    }

    void check(Stencil::Database::RWLock& lock, Objects::DictObject const& ref, Stencil::Database::Record<Objects::DictObject> const& rec)
    {
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        check(lock, ref.dictdict, datastore->Get(lock, rec.dictdict));
        check(lock, ref.dictobj, datastore->Get(lock, rec.dictobj));
        check(lock, ref.dictval, datastore->Get(lock, rec.dictval));
    }

    void check(Stencil::Database::RWLock& /*lock*/, Objects::List const& /*ref*/, Stencil::Database::Record<Objects::List> const& /*rec*/)
    {
        // auto& vec = ref.listobj;
        // for (size_t i = 0; i < vec.size(); i++) { check(lock, vec[i], datastore->Get(lock, rec.listobj)[i]); }
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified); TODO2
    }

    void test_create_simple_object1(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_simple_object1();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        check(lock, refobj1, obj1);
        generated_ids.insert({Stencil::Database::TypeId<decltype(refobj1), DataStore>, id1.id});
    }

    void test_create_dict(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_dict();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        check(lock, refobj1, obj1);
        generated_ids.insert({Stencil::Database::TypeId<decltype(refobj1), DataStore>, id1.id});
    }

    void test_create_list(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_list();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        check(lock, refobj1, obj1);
        generated_ids.insert({Stencil::Database::TypeId<decltype(refobj1), DataStore>, id1.id});
    }

    void test_create_simple_object2(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_simple_object2();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        check(lock, refobj1, obj1);

        // REQUIRE(obj1.val4 == refobj1.val4);
        // REQUIRE(obj1.val5.data == refobj1.val5);
        static_assert(Stencil::Database::TypeId<::Objects::SimpleObject2, DataStore>
                          != Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore>,
                      "Cannot have two types equal");
        generated_ids.insert({Stencil::Database::TypeId<decltype(refobj1), DataStore>, id1.id});
    }

    void test_create_nested_object(Stencil::Database::RWLock& lock)
    {
        auto refobj1     = tester.create_nested_object();
        auto [id1, obj1] = datastore->Create(lock, refobj1);
        REQUIRE(id1.id > 0);
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        check(lock, refobj1.obj1, datastore->Get(lock, obj1.obj1));
        check(lock, refobj1.obj2, datastore->Get(lock, obj1.obj2));
        check(lock, refobj1.obj3, datastore->Get(lock, obj1.obj3));
        check(lock, refobj1.dict1, datastore->Get(lock, obj1.dict1));
        check(lock, refobj1.list1, datastore->Get(lock, obj1.list1));

        static_assert(Stencil::Database::TypeId<::Objects::SimpleObject2, DataStore>
                          != Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore>,
                      "Cannot have two types equal");
        generated_ids.insert({Stencil::Database::TypeId<decltype(refobj1), DataStore>, id1.id});
    }

    template <typename T>
    void dump(Stencil::Database::RWLock& lock, Stencil::Database::Ref<T> const& /*ref*/, Stencil::Database::Record<T> const& rec)
    {
        auto recview = Stencil::Database::CreateRecordView(*datastore, lock, rec);
        lines.push_back(Stencil::Json::Stringify(recview));
    }

    std::unique_ptr<DataStore>                  datastore = std::make_unique<DataStore>();
    ObjectsTester                               tester;
    std::unordered_multimap<uint16_t, uint32_t> generated_ids;
    std::filesystem::path                       filepath = "SaveAndLoad.bin"s;
    std::vector<std::string>                    lines;
};

TEST_CASE("Database", "[database]")
{
    DatabaseTester tester;
    for (size_t count = 1; count < 1000; count = count * 10)
    {
        auto lock = tester.datastore->LockForEdit();
        for (size_t i = 0; i < count; i++) { tester.test_create_simple_object1(lock); }
        for (auto [ref, rec] : tester.datastore->Items<Objects::SimpleObject1>(lock)) { tester.dump(lock, ref, rec); }
        for (size_t i = 0; i < count; i++) { tester.test_create_dict(lock); }
        for (auto [ref, rec] : tester.datastore->Items<Objects::DictObject>(lock)) { tester.dump(lock, ref, rec); }
        for (size_t i = 0; i < count; i++) { tester.test_create_list(lock); }
        for (auto [ref, rec] : tester.datastore->Items<Objects::List>(lock)) { tester.dump(lock, ref, rec); }
        for (size_t i = 0; i < count; i++) { tester.test_create_simple_object2(lock); }
        for (auto [ref, rec] : tester.datastore->Items<Objects::SimpleObject2>(lock)) { tester.dump(lock, ref, rec); }
        for (size_t i = 0; i < count; i++) { tester.test_create_nested_object(lock); }
        for (auto [ref, rec] : tester.datastore->Items<Objects::NestedObject>(lock)) { tester.dump(lock, ref, rec); }
        tester.datastore->Flush(lock);
    }
    // Json Export
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