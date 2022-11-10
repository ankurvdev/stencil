#include "Objects.pidl.h"

#include "TestUtils.h"

#include "stencil/transactions.binserdes.h"
#include "stencil/transactions.strserdes.h"

#include <unordered_set>

using TransactionNestObject = Stencil::Transaction<Objects::NestedObject, void>;
TransactionNestObject CreateNestedObjectTransaction(Objects::NestedObject& obj)
{
    return Stencil::CreateRootTransaction<Objects::NestedObject>(obj);
}

struct TestReplay
{
    TestReplay() : txn2(CreateNestedObjectTransaction(obj2)) {}
    ~TestReplay()
    {
        if (std::uncaught_exceptions() == 0)
        {
            try
            {
                SelfTest();
            }
            catch (...)
            {
               // FAIL("Self Test Failed");
            }
        }
    }
    CLASS_DELETE_COPY_AND_MOVE(TestReplay);

    void Replay(std::string_view const& txndata, std::string_view const& expectedIn = {})
    {
        auto delta    = Test([&](auto& txn) { Stencil::StringTransactionSerDes::Apply(txn, txndata); });
        auto expected = expectedIn.size() == 0 ? txndata : expectedIn;
        if (expected[expected.size() - 1] == ';') { REQUIRE(delta == expected); }
        else { REQUIRE(delta.substr(0, delta.size() - 1) == expected); }
    }

    template <typename TLambda> auto Test(TLambda&& lambda)
    {
        auto txn = CreateNestedObjectTransaction(obj1);
        lambda(txn);
        snapshots.push_back(Stencil::Json::Stringify(obj1));

        auto delta = Stencil::StringTransactionSerDes::Deserialize(txn);

        changes.push_back(delta);
        lambda(txn2);

        deltatxns.push_back(Stencil::StringTransactionSerDes::Deserialize(txn2));

        Stencil::BinaryTransactionSerDes::Deserialize(txn, _binary_txns);
        Stencil::BinaryTransactionSerDes::Deserialize(txn2, _binary_acc_txns);

        // Check repeat binary deserialization doesnt change the delta
        {
            std::ostringstream strm1, strm2;
            Stencil::BinaryTransactionSerDes::Deserialize(txn, strm1);
            auto delta1 = Stencil::StringTransactionSerDes::Deserialize(txn);
            Stencil::BinaryTransactionSerDes::Deserialize(txn, strm2);

            REQUIRE(delta == delta1);
            REQUIRE(strm1.str() == strm2.str());
        }
        {
            _binary_last_txns.str("");
            Stencil::BinaryTransactionSerDes::Deserialize(txn2, _binary_last_txns);
        }
        REQUIRE(Stencil::Json::Stringify(obj1) == Stencil::Json::Stringify(obj2));
        return delta;
    }

    void SelfTest()
    {
        auto& replay = *this;
        {
            CheckOutputAgainstResource(replay.changes, "Deltas");
            CheckOutputAgainstResource(replay.deltatxns, "CumulativeDeltas");
            CheckOutputAgainstResource(replay.snapshots, "ChangeDataSnapshots");

            REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(replay.obj2));
        }
        {
            Objects::NestedObject obj3;
            auto                  txn3 = CreateNestedObjectTransaction(obj3);
            for (auto& c : replay.changes) { Stencil::StringTransactionSerDes::Apply(txn3, c); }
            REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
        }
        {
            Objects::NestedObject obj3;
            auto                  txn3 = CreateNestedObjectTransaction(obj3);
            Stencil::StringTransactionSerDes::Apply(txn3, replay.deltatxns.back());
            REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
        }

        auto binary_txns      = replay._binary_txns.str();
        auto binary_acc_txns  = replay._binary_acc_txns.str();
        auto binary_last_txns = replay._binary_last_txns.str();
        // And now binary streams
        {
            Objects::NestedObject obj3;
            auto                  txn3 = CreateNestedObjectTransaction(obj3);

            std::istringstream istrm(binary_txns);
            istrm.peek();
            while (istrm.good() && (!istrm.eof()))
            {
                Stencil::BinaryTransactionSerDes::Apply(txn3, istrm);
                istrm.peek();
            }

            REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
        }
        {
            Objects::NestedObject obj3;
            auto                  txn3 = CreateNestedObjectTransaction(obj3);
            std::istringstream    istrm(binary_last_txns);
            Stencil::BinaryTransactionSerDes::Apply(txn3, istrm);
            REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
            auto offset = static_cast<unsigned>(istrm.tellg());
            REQUIRE(offset == binary_last_txns.size());
            REQUIRE(!istrm.eof());
        }
        CompareBinaryOutputAgainstResource(binary_txns, "Transactions");
        CompareBinaryOutputAgainstResource(binary_acc_txns, "LastAccumulated");
    }
    std::vector<std::string> changes;
    std::vector<std::string> snapshots;
    std::vector<std::string> deltatxns;

    Objects::NestedObject obj1;
    Objects::NestedObject obj2;

    std::ostringstream _binary_txns;
    std::ostringstream _binary_acc_txns;
    std::ostringstream _binary_last_txns;

    TransactionNestObject txn2;
};

TEST_CASE("Transactions", "[transaction]")
{
    TestReplay replay;
    replay.Replay("obj1.val2 = 1000000;obj1.val2 = 1000000;obj2.val1 = 10000000;obj2.val2 = 10.0000001",
                  "obj1.val2 = 1000000;obj2.val1 = true;obj2.val2 = 10.0000001");
    replay.Replay("obj1.val2 = 2000000;obj1.val5 = 20;obj2.val2 = 20000000;");
    replay.Replay("list1.listobj:add[0] = {\"value\": 100}",
                  "list1.listobj:add[0] = {\"value\":100,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0}}");
    replay.Replay("list1.listobj:add[1] = {\"value\":200,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0}}");

    replay.Replay("list1.listobj:add[2] = {\"value\":300,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0}}");

    replay.Replay("list1.listobj:add[3] = {\"value\":400,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0}}");

    replay.Replay("list1.listobj:add[4] = {\"value\":500,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0}}");

    replay.Replay("list1.listobj.0.obj1.val2 = 30000001;");
    replay.Replay("list1.listobj.1.obj1.val2 = 30000002;");
    replay.Replay("list1.listobj.2.obj1.val2 = 30000003;");
    replay.Replay("list1.listobj.3.obj1.val2 = 30000004;");
    replay.Replay("list1.listobj.4.obj1.val2 = 30000005;");
    replay.Replay("list1.listobj:remove[2] = {}");
    replay.Replay("list1.listobj:remove[2] = {}");
    replay.Replay("obj3.obj1.val1 = 110000000");
    replay.Replay("obj3.obj1.val2 = 222000000");
}

TEST_CASE("Timestamped_Transactions", "[transaction][timestamp")
{

    SECTION("Auto Update On Assign", "Timestamp should automatically update when a field value is assigned")
    {
        Objects::NestedObject obj1{};

        auto t1 = obj1.obj1.lastmodified;
        auto t2 = t1;
        auto t3 = t1;
        auto t4 = t1;
        {
            // Update only on flush
            // Update on sub-struct edit
            {
                auto txn = CreateNestedObjectTransaction(obj1);
                {
                    auto subtxn  = txn.obj1();
                    auto subtxn1 = subtxn.val1();
                    subtxn1.Assign(1000);
                    t2 = obj1.obj1.lastmodified;
                }
                t3 = obj1.obj1.lastmodified;
            }
            t4 = obj1.lastmodified;
            REQUIRE(t1 == t2);
            REQUIRE(t1 < t3);
            REQUIRE(t1 < t4);
            REQUIRE(t3 < t4);

            {
                // No update on false edits
                auto txn = CreateNestedObjectTransaction(obj1);
                txn.obj1().set_val1(1000);
            }
            t4 = obj1.obj1.lastmodified;
            REQUIRE(t3 == t4);
        }
        {
            // List edits
            t2 = obj1.lastmodified;
            {
                auto                txn = CreateNestedObjectTransaction(obj1);
                Objects::ListObject lobj1, lobj2;
                lobj1.value = 100;
                txn.list1().add_listobj(std::move(lobj1));
            }
            t3 = obj1.lastmodified;
            REQUIRE(t2 < t3);
            {
                auto txn = CreateNestedObjectTransaction(obj1);
                txn.list1().edit_listobj(0).set_value(200);
            }
            t4 = obj1.lastmodified;
            REQUIRE(t3 < t4);
        }
    }
}

TEST_CASE("Transactions for unordered_map", "[transaction]")
{
    struct Tester
    {
        TestReplay    replay;
        size_t        _counter{0};
        int32_t       create_int32() { return static_cast<int32_t>(++_counter); }
        uint8_t       create_uint8() { return static_cast<int32_t>(++_counter); }
        uint32_t      create_uint32() { return static_cast<uint32_t>(++_counter); }
        shared_string create_string() { return shared_string(fmt::format("str{}", static_cast<uint32_t>(++_counter))); }
        double        create_double() { return static_cast<double>(++_counter * 100) + (static_cast<double>(++_counter) / 100.0); }
        auto          create_timestamp() { return Stencil::Timestamp{} + std::chrono::seconds{++_counter}; }

        Objects::SimpleObject1 create_obj()
        {
            return Objects::SimpleObject1{
                .val1 = create_int32(), .val2 = create_uint32(), .val3 = create_uint8(), .val4 = create_string(), .val5 = create_double()};
        }

        auto dict_value_create(shared_string const& key)
        {
            auto ts = create_timestamp();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Assign(key, std::move(ts));
                }
            });
        }

        auto dict_value_edit(shared_string const& key)
        {
            auto ts = create_timestamp();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.Assign(std::move(ts)); });
                }
            });
        }

        auto dict_value_destroy(shared_string const& key)
        {
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Remove(key);
                }
            });
        }

        auto dict_value_create_edit_destroy(shared_string const& key)
        {
            auto ts1 = create_timestamp();
            auto ts2 = create_timestamp();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Assign(key, std::move(ts1));
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.Assign(std::move(ts2)); });
                    subtxn2.Remove(key);
                }
            });
        }

        auto dict_value_create_edit_destroy2(shared_string const& key)
        {
            auto ts1 = create_timestamp();
            auto ts2 = create_timestamp();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Assign(key, std::move(ts1));
                }
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.Assign(std::move(ts2)); });
                }
                {
                    auto subtxn2 = subtxn1.dictval();
                    subtxn2.Remove(key);
                }
            });
        }

        auto dict_obj_create(uint32_t const& key)
        {
            auto obj = create_obj();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Assign(key, std::move(obj));
                }
            });
        }

        auto dict_obj_edit(uint32_t const& key)
        {
            auto val1 = create_int32();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.set_val1(std::move(val1)); });
                }
            });
        }

        auto dict_obj_destroy(uint32_t const& key)
        {
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Remove(key);
                }
            });
        }
        auto dict_obj_create_edit_destroy(uint32_t const& key)
        {
            auto val1 = create_int32();
            auto obj  = create_obj();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Assign(key, std::move(obj));
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.set_val1(std::move(val1)); });
                    subtxn2.Remove(key);
                }
            });
        }
        auto dict_obj_create_edit_destroy2(uint32_t const& key)
        {
            auto val1 = create_int32();
            auto obj  = create_obj();
            return replay.Test([&](auto& txn) {
                auto subtxn1 = txn.dict1();
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Assign(key, std::move(obj));
                }
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Edit(key, [&](auto& subtxn3) { subtxn3.set_val1(std::move(val1)); });
                }
                {
                    auto subtxn2 = subtxn1.dictobj();
                    subtxn2.Remove(key);
                }
            });
        }
    };

    SECTION("dict value : create edit destroy")
    {
        Tester                     tester;
        std::vector<shared_string> keylist = {shared_string("now1"), shared_string("now2"), shared_string("now3"), shared_string("now4")};
        std::unordered_map<shared_string, std::unordered_map<std::string, std::string>> expected;
        std::unordered_set<shared_string>                                               done;
        for (auto& key1 : keylist)
        {
            for (auto& key : keylist)
            {
                if (done.count(key) > 0) continue;
                REQUIRE(tester.dict_value_create(key) != "");
                REQUIRE(tester.dict_value_edit(key) != "");
                REQUIRE(tester.dict_value_edit(key) != "");
                REQUIRE(tester.dict_value_destroy(key) != "");
                REQUIRE(tester.dict_value_create(key) != "");
                REQUIRE(tester.dict_value_create_edit_destroy(key) != "");
                REQUIRE(tester.dict_value_create_edit_destroy2(key) != "");
            }
            REQUIRE(tester.dict_value_create(key1) != "");
            REQUIRE(tester.dict_value_edit(key1) != "");
            done.insert(key1);
        }

        for (auto key : keylist) { REQUIRE(tester.dict_value_edit(key) != ""); }

        for (auto key : keylist) { REQUIRE(tester.dict_value_destroy(key) != ""); }
    }

    SECTION("dict obj : create edit destroy")
    {
        Tester                tester;
        std::vector<uint32_t> keylist = {tester.create_uint32(), tester.create_uint32(), tester.create_uint32(), tester.create_uint32()};
        for (auto& key1 : keylist)
        {
            for (auto& key : keylist)
            {
                REQUIRE(tester.dict_obj_create(key) != "");
                REQUIRE(tester.dict_obj_edit(key) != "");
                REQUIRE(tester.dict_obj_edit(key) != "");
                REQUIRE(tester.dict_obj_destroy(key) != "");
                REQUIRE(tester.dict_obj_create(key) != "");    // Dict repeated insertion overwrites
                REQUIRE(tester.dict_obj_create_edit_destroy(key) != "");
                REQUIRE(tester.dict_obj_create_edit_destroy2(key) != "");
            }
            REQUIRE(tester.dict_obj_create(key1) != "");
            REQUIRE(tester.dict_obj_edit(key1) != "");
        }

        for (auto key : keylist) { REQUIRE(tester.dict_obj_edit(key) != ""); }

        for (auto key : keylist) { REQUIRE(tester.dict_obj_destroy(key) != ""); }
    }

    SECTION("dict value timestamp update : create edit destroy")
    {}

    SECTION("dict obj timestamp update : create edit destroy")
    {}

    //
    // Dict of struct : Assign clears off past edit and assign transactions (wipe slate)
    //
}

TEST_CASE("Transactions_Bugs", "[transaction]")
{
    SECTION("List-Edit", "Object sublist edit must propagate up as object edits too")
    {
        Objects::NestedObject obj1;
        obj1.list1.listobj.push_back({});
        auto txn = CreateNestedObjectTransaction(obj1);
        txn.list1().edit_listobj(0).obj1().set_val1(1);
        // txn.Flush();
        REQUIRE(txn.list1().IsChanged());
        REQUIRE(txn.IsChanged());
    }
}
