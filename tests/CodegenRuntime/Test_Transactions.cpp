#include "Objects.pidl.h"

#include "TestUtils.h"

#include "stencil/transactions.binserdes.h"
#include "stencil/transactions.strserdes.h"

using TransactionNestObject = Stencil::Transaction<Objects::NestedObject, void>;
TransactionNestObject CreateNestedObjectTransaction(Objects::NestedObject& obj)
{
    return Stencil::CreateRootTransaction<Objects::NestedObject>(obj);
}

struct TestReplay
{
    TestReplay() : txn2(CreateNestedObjectTransaction(obj2)) {}
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
            CheckOutputAgainstResource(replay.snapshots, "ChangeDataSnapshots");
            CheckOutputAgainstResource(replay.deltatxns, "Deltas");

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
    replay.SelfTest();
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
    TestReplay replay;

    // dict of values : create
    replay.Test([](auto& txn) {
        auto subtxn1 = txn.dict1();
        {
            auto subtxn2 = subtxn1.dictval();
            subtxn2.Assign(shared_string("now"), {});
        }
    });

    // dict of values : edit
    replay.Test([](auto& txn) {
        auto subtxn1 = txn.dict1();
        {
            auto subtxn2 = subtxn1.dictval();
            subtxn2.Assign(shared_string("now"), {});
        }
    });

    // dict of values : destroy
    replay.Test([](auto& txn) {
        auto subtxn1 = txn.dict1();
        {
            auto subtxn2 = subtxn1.dictval();
            subtxn2.Assign(shared_string("now"), {});
        }
    });

    // dict of values : create + edit + destroy
    replay.Test([](auto& txn) {
        auto subtxn1 = txn.dict1();
        {
            auto subtxn2 = subtxn1.dictval();
            subtxn2.Assign(shared_string("now"), {});
            subtxn2.Assign(shared_string("now"), {});
            subtxn2.Assign(shared_string("now"), {});
        }
    });

    // dict of values : create + edit + destroy (with flushes and in a loop)
    replay.Test([](auto& txn) {
        for (size_t i = 0; i < 10; i++)
        {
            auto subtxn1 = txn.dict1();
            {
                auto subtxn2 = subtxn1.dictval();
                subtxn2.Assign(shared_string("now"), {});
            }
            {
                auto subtxn2 = subtxn1.dictval();
                subtxn2.Assign(shared_string("now"), {});
            }
            {
                auto subtxn2 = subtxn1.dictval();
                subtxn2.Assign(shared_string("now"), {});
            }
        }
    });

    // Dict of values
    // Dict of structs
    //
    //
    // Dict of values : Create-edit-destroy with few values inserted
    // insert dict value k1:v1, k2:v2, k3:v3
    // insert dict value k::v
    // assign dict value k::v1
    // remove dict key k
    // Loop 10 times
    // remove k1, k2, k3

    // Dict of structs : Create edit-assign destroy with empty initial
    // insert dict value k::v
    // edit dict value k::v1
    // assign edit dict value k::v1
    // remove dict key k
    // Loop 10 times
    //
    // Dict of structs : Create assign edit destroy with empty initial
    // Dict of structs : Create Create Create assign edit destroy
    // Dict of struct  : remove remove remove all the way to empty

    // Dict timestamp updated with edits
    // Dict timestamp updated with remove
    // Dict timestamp update with assign

    // Dict repeated insertion overwrites
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
