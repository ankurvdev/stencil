#include "ObjectsTester.h"
#include "TestUtils.h"
#include "stencil/database.h"

#include <unordered_set>

using namespace Catch::literals;
using namespace std::literals;
using DataStore = Stencil::Database::Database<::Objects::NestedObject>;
static_assert(Stencil::Database::TypeId<::Objects::SimpleObject1, DataStore> > 0);
static_assert(Stencil::ConceptIndexable<Stencil::Database::RecordView<::Objects::SimpleObject1, DataStore>>);

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
        REQUIRE(rec.lastmodified == ref.lastmodified);
        REQUIRE(rec.val1.data == ref.val1);
        REQUIRE(rec.val2.data == ref.val2);
        REQUIRE(rec.val3.data == ref.val3);
        REQUIRE(rec.val4.id > 0);
        // REQUIRE(obj1.val4 == refobj1.val4);
        REQUIRE(rec.val5.data == Catch::Approx(ref.val5));
    }

    void check(Stencil::Database::RWLock& /*lock*/,
               Objects::SimpleObject2 const&                            ref,
               Stencil::Database::Record<Objects::SimpleObject2> const& rec)
    {
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        REQUIRE(rec._fieldtracker == ref._fieldtracker);
        REQUIRE(rec.lastmodified == ref.lastmodified);
        REQUIRE(rec.val1.data == ref.val1);
        REQUIRE(rec.val2.data == Catch::Approx(ref.val2));
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
        REQUIRE(rec.lastmodified == ref.lastmodified);
        REQUIRE(rec.value.data == ref.value);
    }

    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<uint32_t, std::unordered_map<uint32_t, ::Objects::SimpleObject1>> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<uint32_t, std::unordered_map<uint32_t, ::Objects::SimpleObject1>>> const& rec)
    {
        for (auto item : rec.Items())
        {
            /*auto krec =*/datastore->Get(lock, item.k);
            /*auto vrec =*/datastore->Get(lock, item.v);
        }
    }

    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<uint32_t, ::Objects::SimpleObject1> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<uint32_t, ::Objects::SimpleObject1>> const& rec)
    {
        for (auto item : rec.Items())
        {
            /* auto krec = */ datastore->Get(lock, item.k);
            /* auto vrec =  */ datastore->Get(lock, item.v);
        }
    }
    void check(Stencil::Database::RWLock& lock,
               std::unordered_map<shared_string, Stencil::Timestamp> const& /*ref*/,
               Stencil::Database::Record<std::unordered_map<shared_string, Stencil::Timestamp>> const& rec)
    {
        for (auto item : rec.Items())
        {
            /* auto krec = */ datastore->Get(lock, item.k);
            /* auto vrec = */ datastore->Get(lock, item.v);
        }
    }

    void check(Stencil::Database::RWLock& lock, Objects::DictObject const& ref, Stencil::Database::Record<Objects::DictObject> const& rec)
    {
        // static_assert(sizeof(obj1) == sizeof(refobj1) - sizeof(shared_string) + sizeof(Stencil::Database::Ref<shared_string>));
        check(lock, ref.dictval, datastore->Get(lock, rec.dictval));
        check(lock, ref.dictobj, datastore->Get(lock, rec.dictobj));
        check(lock, ref.dictdict, datastore->Get(lock, rec.dictdict));
    }

    void check(Stencil::Database::RWLock& /*lock*/, Objects::List const& /*ref*/, Stencil::Database::Record<Objects::List> const& /*rec*/)
    {
        // auto& vec = ref.listobj;
        // for (size_t i = 0; i < vec.size(); i++) { check(lock, vec[i], datastore->Get(lock, rec.listobj)[i]); }
        // REQUIRE(obj1.lastmodified == refobj1.lastmodified);
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
    void dump(Stencil::Database::RWLock& lock, Stencil::Database::Ref<T> const& ref, Stencil::Database::Record<T> const& rec)
    {
        auto recview = Stencil::Database::CreateRecordView(*datastore, lock, ref, rec);
        lines.push_back(Stencil::Json::Stringify(recview));
    }

    template <typename T> void delete_half_with_iterator(Stencil::Database::RWLock& lock)
    {
        size_t                       i = 0;
        std::unordered_set<uint32_t> todelete;
        for (auto it = generated_ids.cbegin(); it != generated_ids.cend();)
        {
            if (it->first == Stencil::Database::TypeId<T, DataStore> && ((++i) % 2 == 0))
            {
                todelete.insert(it->second);
                it = generated_ids.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto [ref, rec] : datastore->Items<T>(lock))
        {
            if (todelete.count(ref.id) > 0)
            {
                datastore->Delete(lock, ref);
                lines.push_back(fmt::format("{{\"delete\": {}}}", ref.id));
            }
        }
    }

    template <typename T> void delete_half(Stencil::Database::RWLock& lock)
    {
        size_t i = 0;
        for (auto it = generated_ids.cbegin(); it != generated_ids.cend();)
        {
            if (it->first == Stencil::Database::TypeId<T, DataStore> && ((++i) % 2 == 0))
            {
                datastore->Delete(lock, Stencil::Database::Ref<T>{it->second});
                lines.push_back(fmt::format("{{\"delete\": {}}}", it->second));
                it = generated_ids.erase(it);
            }
            else
            {
                ++it;
            }
        }
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
        if (count % 2 == 0)
        {
            tester.delete_half_with_iterator<Objects::NestedObject>(lock);
            tester.delete_half_with_iterator<Objects::DictObject>(lock);
            tester.delete_half_with_iterator<Objects::List>(lock);
            tester.delete_half_with_iterator<Objects::SimpleObject2>(lock);
            tester.delete_half_with_iterator<Objects::SimpleObject1>(lock);
        }
        else
        {
            tester.delete_half<Objects::NestedObject>(lock);
            tester.delete_half<Objects::DictObject>(lock);
            tester.delete_half<Objects::List>(lock);
            tester.delete_half<Objects::SimpleObject2>(lock);
            tester.delete_half<Objects::SimpleObject1>(lock);
        }

        tester.datastore->Flush(lock);
    }
    // Json Export
}

TEST_CASE("Database File Storage", "[database]")
{
    auto dbFileName = std::filesystem::path("SaveAndLoadFile.bin"s);
    if (std::filesystem::exists(dbFileName)) std::filesystem::remove(dbFileName);

    // Constructor With Path Creates New File and Writes Header
    {
        {
            DataStore datastore;
            datastore.Init(dbFileName);
        }
        REQUIRE(std::filesystem::exists(dbFileName));
        REQUIRE(std::filesystem::file_size(dbFileName) == (8192 * 2 + 80));
    }

    // Constructor With Path Reads Existing File but doesnt touch it
    {
        auto time = std::filesystem::file_time_type::clock::now();
        {
            DataStore datastore;
            datastore.Init(dbFileName);
        }
        if (std::filesystem::last_write_time(dbFileName).time_since_epoch().count() > time.time_since_epoch().count())
        {
            FAIL("Failed. Database was modified");
        }
    }

    // Constructor With IStream reads file
    {
        auto time = std::filesystem::file_time_type::clock::now();
        {
            DataStore datastore;
            datastore.Init(std::ifstream(dbFileName));
            ObjectsTester tester;
            auto          lock    = datastore.LockForEdit();
            auto          refobj1 = tester.create_simple_object1();
            datastore.Create(lock, refobj1);
        }

        if (std::filesystem::last_write_time(dbFileName).time_since_epoch().count() > time.time_since_epoch().count())
        {
            FAIL("Database changed");
        }
    }

    // Empty Constructor for in-memory datastore
    {
        {
            DataStore datastore;

            datastore.Init();
            ObjectsTester tester;
            auto          lock    = datastore.LockForEdit();
            auto          refobj1 = tester.create_simple_object1();
            datastore.Create(lock, refobj1);
        }
        {
            // TODO : Verify nothing written
        }
    }
}
