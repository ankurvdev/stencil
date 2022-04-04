#include "TestUtils.h"
#include "Transactions.pidl.h"

#include "stencil/transactions.binserdes.h"
#include "stencil/transactions.strserdes.h"

struct TestReplay
{
    TestReplay() : txn2(obj2) {}
    CLASS_DELETE_COPY_AND_MOVE(TestReplay);

    void Replay(std::string_view const& txndata, std::string_view const& expectedIn = {})
    {
        Stencil::Transaction<Transactions::Object> txn(obj1);
        Stencil::StringTransactionSerDes::Apply(txn, txndata);
        snapshots.push_back(Stencil::Json::Stringify(obj1));
        auto expected = expectedIn.size() == 0 ? txndata : expectedIn;

        auto delta = Stencil::StringTransactionSerDes::Deserialize(txn);

        changes.push_back(delta);
        if (expected[expected.size() - 1] == ';') { REQUIRE(delta == expected); }
        else
        {
            REQUIRE(delta.substr(0, delta.size() - 1) == expected);
        }
        Stencil::StringTransactionSerDes::Apply(txn2, txndata);
        deltatxns.push_back(Stencil::StringTransactionSerDes::Deserialize(txn2));

        Stencil::BinaryTransactionSerDes::Deserialize(txn, binary_txns);
        Stencil::BinaryTransactionSerDes::Deserialize(txn2, binary_acc_txns);

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
            binary_last_txns.str("");
            Stencil::BinaryTransactionSerDes::Deserialize(txn2, binary_last_txns);
        }
    }

    std::vector<std::string> changes;
    std::vector<std::string> snapshots;
    std::vector<std::string> deltatxns;

    Transactions::Object obj1;
    Transactions::Object obj2;

    std::ostringstream binary_txns;
    std::ostringstream binary_acc_txns;
    std::ostringstream binary_last_txns;

    Stencil::Transaction<Transactions::Object> txn2;
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
    {
        CheckOutputAgainstResource(replay.snapshots, "ChangeDataSnapshots");
        CheckOutputAgainstResource(replay.deltatxns, "Deltas");

        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(replay.obj2));
    }
    {
        Transactions::Object                       obj3;
        Stencil::Transaction<Transactions::Object> txn3(obj3);
        for (auto& c : replay.changes) { Stencil::StringTransactionSerDes::Apply(txn3, c); }
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
    }
    {
        Transactions::Object                       obj3;
        Stencil::Transaction<Transactions::Object> txn3(obj3);
        Stencil::StringTransactionSerDes::Apply(txn3, replay.deltatxns.back());
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
    }

    auto binary_txns      = replay.binary_txns.str();
    auto binary_acc_txns  = replay.binary_acc_txns.str();
    auto binary_last_txns = replay.binary_last_txns.str();
    // And now binary streams
    {
        Transactions::Object                       obj3;
        Stencil::Transaction<Transactions::Object> txn3(obj3);

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
        Transactions::Object                       obj3;
        Stencil::Transaction<Transactions::Object> txn3(obj3);
        std::istringstream                         istrm(binary_last_txns);
        Stencil::BinaryTransactionSerDes::Apply(txn3, istrm);
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
        auto offset = static_cast<unsigned>(istrm.tellg());
        REQUIRE(offset == binary_last_txns.size());
        REQUIRE(!istrm.eof());
    }
    CompareBinaryOutputAgainstResource(binary_txns, "Transactions");
    CompareBinaryOutputAgainstResource(binary_acc_txns, "LastAccumulated");
}

TEST_CASE("Timestamped_Transactions", "[transaction][timestamp")
{

    SECTION("Auto Update On Assign", "Timestamp should automatically update when a field value is assigned")
    {
        Transactions::Object obj1;

        auto t1 = obj1.obj1.lastmodified;
        auto t2 = t1;
        auto t3 = t1;
        auto t4 = t1;
        {
            // Update only on flush
            // Update on sub-struct edit
            Stencil::Transaction<Transactions::Object> txn(obj1);
            txn.obj1().set_val1(1000);
            t2 = obj1.obj1.lastmodified;
            txn.Flush();

            t3 = obj1.obj1.lastmodified;
            t4 = obj1.lastmodified;
            REQUIRE(t1 == t2);
            REQUIRE(t1 < t3);
            REQUIRE(t1 < t4);
        }
        {
            // No update on false edits
            Stencil::Transaction<Transactions::Object> txn(obj1);
            txn.obj1().set_val1(1000);
            t4 = obj1.obj1.lastmodified;
            txn.Flush();
            REQUIRE(t3 == t4);
        }
        {
            // List edits
            t2 = obj1.lastmodified;
            Stencil::Transaction<Transactions::Object> txn(obj1);
            Transactions::ListObject                   lobj1, lobj2;
            lobj1.value = 100;
            txn.list1().add_listobj(std::move(lobj1));
            txn.Flush();
            t3 = obj1.lastmodified;
            REQUIRE(t2 < t3);
            txn.list1().edit_listobj(0).set_value(200);
            txn.Flush();
            t4 = obj1.lastmodified;
            REQUIRE(t3 < t4);
            txn.Flush();
            // TODO : Bugfix
            // REQUIRE(t4 == obj1.lastmodified);
        }
    }
}

TEST_CASE("Transactions_Bugs", "[transaction]")
{
    SECTION("List-Edit", "Object sublist edit must propagate up as object edits too")
    {
        Transactions::Object obj1;
        obj1.list1.listobj.push_back({});
        Stencil::Transaction<Transactions::Object> txn(obj1);
        txn.list1().edit_listobj(0).obj1().set_val1(1);
        txn.Flush();
        REQUIRE(txn.list1().IsChanged());
        REQUIRE(txn.IsChanged());
    }
}
