#include "ObjectsTester.h"
#include "TestUtils.h"

#include "stencil/transactions.binserdes.h"
#include "stencil/transactions.strserdes.h"

#include <unordered_set>

using TransactionNestObject = Stencil::Transaction<Objects::NestedObject, void>;
static TransactionNestObject CreateNestedObjectTransaction(Objects::NestedObject& obj)
{
    return Stencil::CreateRootTransaction<Objects::NestedObject>(obj);
}

template <typename T> bool TestObjEqual(T const& obj1, T const& obj2)
{
    return TestCommon::JsonStringEqual(Stencil::Json::Stringify(obj1), Stencil::Json::Stringify(obj2));
}

template <typename T> bool TestObjEqual(T const& obj1, std::string const& snapshot)
{
    return TestCommon::JsonStringEqual(Stencil::Json::Stringify(obj1), snapshot);
}

template <typename T> std::string SerDesSer(T const& obj)
{
    auto str  = Stencil::Json::Stringify(obj);
    auto obj1 = Stencil::Json::Parse<T>(str);
    return Stencil::Json::Stringify(obj1);
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
            } catch (...)
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
        if (expected[expected.size() - 1] == ';') { CHECK(delta == expected); }
        else { CHECK(delta.substr(0, delta.size() - 1) == expected); }
    }

    static std::string BinTxnSerialize(TransactionNestObject& txn)
    {
        std::ostringstream ostr;
        Stencil::BinaryTransactionSerDes::Deserialize(txn, ostr);
        return ostr.str();
    }

    static void BinTxnApply(TransactionNestObject& txn, std::string const& txnbin)
    {
        std::istringstream istrm(txnbin);
        Stencil::BinaryTransactionSerDes::Apply(txn, istrm);
        auto offset = static_cast<unsigned>(istrm.tellg());
        CHECK(offset == txnbin.size());
        CHECK(!istrm.eof());
    }

    template <typename TLambda> std::string Test(TLambda&& lambda)
    {
        auto txn1 = CreateNestedObjectTransaction(obj1);
        lambda(txn1);
        lambda(txn2);
        auto snapshot1 = Stencil::Json::Stringify(obj1);
        auto snapshot2 = Stencil::Json::Stringify(obj2);

        auto txn1str = Stencil::StringTransactionSerDes::Deserialize(txn1);
        // if (_debug) CHECK(txn1str == _expected_txn1str[index]);
        CHECK(TestObjEqual(obj1, snapshot1));                                     // Check deserialization doesnt change anything
        CHECK(Stencil::StringTransactionSerDes::Deserialize(txn1) == txn1str);    // Check repeat deserialization produces same output

        auto txn2str = Stencil::StringTransactionSerDes::Deserialize(txn2);
        // if (_debug) CHECK(txn2str == _expected_txn2str[index]);
        CHECK(TestObjEqual(obj2, snapshot2));
        CHECK(Stencil::StringTransactionSerDes::Deserialize(txn2) == txn2str);

        auto txn1bin = BinTxnSerialize(txn1);
        // if (_debug) CHECK(txn1bin == _expected_txn1bin[index]);
        CHECK(TestObjEqual(obj1, snapshot1));
        CHECK(BinTxnSerialize(txn1) == txn1bin);

        auto txn2bin = BinTxnSerialize(txn2);
        // if (_debug) CHECK(txn2bin == _expected_txn2bin[index]);
        CHECK(TestObjEqual(obj2, snapshot2));
        CHECK(BinTxnSerialize(txn2) == txn2bin);

        CHECK(TestObjEqual(obj1, obj2));
        _json_snapshots.push_back(snapshot1);
        _txn1str.push_back(txn1str);
        _txn2str.push_back(txn2str);
        _txn1bin.push_back(txn1bin);
        _txn2bin.push_back(txn2bin);

        if (!_debug)
        {
            {
                Objects::NestedObject obj3{};
                auto                  txn3 = CreateNestedObjectTransaction(obj3);
                for (auto& c : _txn1str) { Stencil::StringTransactionSerDes::Apply(txn3, c); }
                CHECK(TestObjEqual(obj1, obj3));
            }
            {
                Objects::NestedObject obj3{};
                auto                  txn3 = CreateNestedObjectTransaction(obj3);
                Stencil::StringTransactionSerDes::Apply(txn3, _txn2str.back());
                CHECK(TestObjEqual(obj2, obj3));
                // unordered map can sometime change ordering
                // CHECK(Stencil::Json::Stringify(obj) == Stencil::Json::Stringify(obj3));
            }

            {
                Objects::NestedObject obj3{};
                auto                  txn3 = CreateNestedObjectTransaction(obj3);
                for (auto& c : _txn1bin) { BinTxnApply(txn3, c); }
                CHECK(TestObjEqual(obj1, obj3));
            }
            {
                Objects::NestedObject obj3{};
                auto                  txn3 = CreateNestedObjectTransaction(obj3);
                BinTxnApply(txn3, _txn2bin.back());
                CHECK(TestObjEqual(obj2, obj3));
                // unordered map can sometime change ordering
                // CHECK(Stencil::Json::Stringify(obj1) == Stencil::Json::Stringify(obj3));
            }
        }
        return txn1str;
    }

    void SelfTest()
    {
        TestCommon::CheckResource<TestCommon::StrFormat>(_txn1str, "Deltas");
        TestCommon::CheckResource<TestCommon::StrFormat>(_txn2str, "CumulativeDeltas");
        TestCommon::CheckResource<TestCommon::JsonFormat>(_json_snapshots, "ChangeDataSnapshots");
        // Too many variations in CumulativeDeltaBin due to unordered_map unstable ordering
        // TestCommon::CheckOutputAgainstBinResource(_txn1bin, "DeltaBin");
        // TestCommon::CheckOutputAgainstBinResource(_txn2bin, "CumulativeDeltaBin");
    }

    std::vector<std::string> _txn1str;
    std::vector<std::string> _txn1bin;
    std::vector<std::string> _txn2str;
    std::vector<std::string> _txn2bin;

    std::vector<std::string> _json_snapshots;

    // std::vector<std::string> _expected_txn1str        = LoadStrResource("Deltas");
    // std::vector<std::string> _expected_txn2str        = LoadStrResource("CumulativeDeltas");
    // std::vector<std::string> _expected_txn1bin        = LoadBinResource("DeltaBin");
    // std::vector<std::string> _expected_txn2bin        = LoadBinResource("CumulativeDeltaBin");
    // std::vector<std::string> _expected_json_snapshots = LoadStrResource("ChangeDataSnapshots");

    Objects::NestedObject obj1{};
    Objects::NestedObject obj2{};

    TransactionNestObject txn2;
    bool                  _debug = false;
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
            CHECK(t1 == t2);
            CHECK(t1 < t3);
            CHECK(t1 < t4);
            CHECK(t3 < t4);

            {
                // No update on false edits
                auto txn = CreateNestedObjectTransaction(obj1);
                txn.obj1().set_val1(1000);
            }
            t4 = obj1.obj1.lastmodified;
            CHECK(t3 == t4);
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
            CHECK(t2 < t3);
            {
                auto txn = CreateNestedObjectTransaction(obj1);
                txn.list1().edit_listobj(0).set_value(200);
            }
            t4 = obj1.lastmodified;
            CHECK(t3 < t4);
        }
    }
}

struct UnorderedMapTester : public ObjectsTester
{
    TestReplay replay;
    auto       dict_value_create(shared_string const& key)
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

    Objects::SimpleObject1 create_obj()
    {
        Objects::SimpleObject1 obj{};
        obj.val1 = create_int32();
        obj.val2 = create_uint32();
        obj.val3 = create_uint8();
        obj.val4 = create_string();
        obj.val5 = create_double();
        return obj;
    }

    auto dict_obj_create(shared_string const& key)
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

    auto dict_obj_edit(shared_string const& key)
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

    auto dict_obj_destroy(shared_string const& key)
    {
        return replay.Test([&](auto& txn) {
            auto subtxn1 = txn.dict1();
            {
                auto subtxn2 = subtxn1.dictval();
                subtxn2.Remove(key);
            }
        });
    }

    auto dict_obj_create_edit_destroy(shared_string const& key)
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

    auto dict_obj_create_edit_destroy2(shared_string const& key)
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

TEST_CASE("Transactions unordered_map dict_value", "[transaction]")
{

    UnorderedMapTester         tester;
    std::vector<shared_string> keylist = {shared_string("now1"), shared_string("now2"), shared_string("now3"), shared_string("now4")};
    std::unordered_map<shared_string, std::unordered_map<std::string, std::string>> expected;
    std::unordered_set<shared_string>                                               done;
    for (auto& key1 : keylist)
    {
        for (auto& key : keylist)
        {
            if (done.count(key) > 0) continue;
            CHECK(tester.dict_value_create(key) != "");
            CHECK(tester.dict_value_edit(key) != "");
            CHECK(tester.dict_value_edit(key) != "");
            CHECK(tester.dict_value_destroy(key) != "");
            CHECK(tester.dict_value_create(key) != "");
            CHECK(tester.dict_value_create_edit_destroy(key) != "");
            CHECK(tester.dict_value_create_edit_destroy2(key) != "");
        }
        CHECK(tester.dict_value_create(key1) != "");
        CHECK(tester.dict_value_edit(key1) != "");
        done.insert(key1);
    }

    for (auto key : keylist) { CHECK(tester.dict_value_edit(key) != ""); }

    for (auto key : keylist) { CHECK(tester.dict_value_destroy(key) != ""); }
}

TEST_CASE("Transactions unordered_map dict_obj")
{
    UnorderedMapTester           tester;
    std::vector<uint32_t>        keylist = {tester.create_uint32(), tester.create_uint32(), tester.create_uint32(), tester.create_uint32()};
    std::unordered_set<uint32_t> done;
    for (auto& key1 : keylist)
    {
        for (auto& key : keylist)
        {
            if (done.count(key) > 0) continue;
            CHECK(tester.dict_obj_create(key) != "");
            CHECK(tester.dict_obj_edit(key) != "");
            CHECK(tester.dict_obj_edit(key) != "");
            CHECK(tester.dict_obj_destroy(key) != "");
            CHECK(tester.dict_obj_create(key) != "");
            CHECK(tester.dict_obj_create_edit_destroy(key) != "");
            CHECK(tester.dict_obj_create_edit_destroy2(key) != "");
        }
        CHECK(tester.dict_obj_create(key1) != "");
        CHECK(tester.dict_obj_edit(key1) != "");
        done.insert(key1);
    }
    for (auto key : keylist) { CHECK(tester.dict_obj_edit(key) != ""); }

    for (auto key : keylist) { CHECK(tester.dict_obj_destroy(key) != ""); }
}

TEST_CASE("Transactions unordered_map timestamp update : create edit destroy")
{}

//
// Dict of struct : Assign clears off past edit and assign transactions (wipe slate)
//

TEST_CASE("Transactions_Bugs", "[transaction]")
{
    SECTION("List-Edit", "Object sublist edit must propagate up as object edits too")
    {
        Objects::NestedObject obj1;
        obj1.list1.listobj.push_back({});
        auto txn = CreateNestedObjectTransaction(obj1);
        txn.list1().edit_listobj(0).obj1().set_val1(1);
        // txn.Flush();
        CHECK(txn.list1().IsChanged());
        CHECK(txn.IsChanged());
    }
}
