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
        Stencil::Transaction<Transactions::Object::Data> txn(obj1);
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
            std::ofstream binary_lastacc_txns("Transactions.LastAccumulated.bin", std::ios::binary);
            Stencil::BinaryTransactionSerDes::Deserialize(txn2, binary_lastacc_txns);
            binary_lastacc_txns.close();
        }
    }

    std::vector<std::string> changes;
    std::vector<std::string> snapshots;
    std::vector<std::string> deltatxns;

    Transactions::Object::Data obj1;
    Transactions::Object::Data obj2;

    std::ofstream binary_txns     = std::ofstream("Transactions.bin", std::ios::binary);
    std::ofstream binary_acc_txns = std::ofstream("Transactions.Accumulated.bin", std::ios::binary);

    Stencil::Transaction<Transactions::Object::Data> txn2;
};

TEST_CASE("Transactions", "[transaction]")
{
    TestReplay replay;
    replay.Replay("obj1.val2 = 1000000;obj1.val2 = 1000000;obj2.val1 = 10000000;obj2.val2 = 10.0000001",
                  "obj1.val2 = 1000000;obj2.val1 = 1;obj2.val2 = 10.000000");
    replay.Replay("obj1.val2 = 2000000;obj1.val5 = 20.000000;obj2.val2 = 20000000.000000;");
    replay.Replay("list1.listobj:add[0] = {\"value\": 100}",
                  "list1.listobj:add[0] = {\"value\":100,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");
    replay.Replay("list1.listobj:add[1] = {\"value\":200,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[2] = {\"value\":300,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[3] = {\"value\":400,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[4] = {\"value\":500,\"obj1\":{\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

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
        CheckOutputAgainstResource(replay.snapshots, "Transactions_ChangeDataSnapshots.txt");
        CheckOutputAgainstResource(replay.deltatxns, "Transactions_Deltas.txt");

        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(replay.obj2));
    }
    {
        Transactions::Object::Data                       obj3;
        Stencil::Transaction<Transactions::Object::Data> txn3(obj3);
        for (auto& c : replay.changes) { Stencil::StringTransactionSerDes::Apply(txn3, c); }
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
    }
    {
        Transactions::Object::Data                       obj3;
        Stencil::Transaction<Transactions::Object::Data> txn3(obj3);
        Stencil::StringTransactionSerDes::Apply(txn3, replay.deltatxns.back());
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
    }

    replay.binary_txns.close();
    replay.binary_acc_txns.close();

    // And now binary streams
    {
        Transactions::Object::Data                       obj3;
        Stencil::Transaction<Transactions::Object::Data> txn3(obj3);

        std::ifstream istrm("Transactions.bin", std::ios::binary);
        istrm.peek();
        while (istrm.good() && (!istrm.eof()))
        {
            Stencil::BinaryTransactionSerDes::Apply(txn3, istrm);
            istrm.peek();
        }

        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
    }
    {
        Transactions::Object::Data                       obj3;
        Stencil::Transaction<Transactions::Object::Data> txn3(obj3);

        std::ifstream istrm("Transactions.LastAccumulated.bin", std::ios::binary);
        Stencil::BinaryTransactionSerDes::Apply(txn3, istrm);
        REQUIRE(Stencil::Json::Stringify(replay.obj1) == Stencil::Json::Stringify(obj3));
        auto offset = static_cast<unsigned>(istrm.tellg());
        REQUIRE(offset == std::filesystem::file_size("Transactions.LastAccumulated.bin"));
        REQUIRE(!istrm.eof());
    }
    CompareFileAgainstResource("Transactions.bin", "Transactions.bin");
    CompareFileAgainstResource("Transactions.LastAccumulated.bin", "Transactions.LastAccumulated.bin");
}

TEST_CASE("Timestamped_Transactions", "[transaction][timestamp")
{

    SECTION("Auto Update On Assign", "Timestamp should automatically update when a field value is assigned")
    {
        Transactions::Object::Data obj1;

        auto t1 = obj1.obj1().LastModified();
        auto t2 = t1;
        auto t3 = t1;
        auto t4 = t1;
        {
            // Update only on flush
            // Update on sub-struct edit
            Stencil::Transaction<Transactions::Object::Data> txn(obj1);
            txn.obj1().set_val1(1000);
            t2 = obj1.obj1().LastModified();
            txn.Flush();

            t3 = obj1.obj1().LastModified();
            t4 = obj1.LastModified();
            REQUIRE(t1 == t2);
            REQUIRE(t1 < t3);
            REQUIRE(t1 < t4);
        }
        {
            // No update on false edits
            Stencil::Transaction<Transactions::Object::Data> txn(obj1);
            txn.obj1().set_val1(1000);
            t4 = obj1.obj1().LastModified();
            txn.Flush();
            REQUIRE(t3 == t4);
        }
        {
            // List edits
            t2 = obj1.LastModified();
            Stencil::Transaction<Transactions::Object::Data> txn(obj1);
            Transactions::ListObject::Data                   lobj1, lobj2;
            lobj1.set_value(100);
            txn.list1().add_listobj(std::move(lobj1));
            txn.Flush();
            t3 = obj1.LastModified();
            REQUIRE(t2 < t3);
            txn.list1().edit_listobj(0).set_value(200);
            txn.Flush();
            t4 = obj1.LastModified();
            REQUIRE(t3 < t4);
            txn.Flush();
            // TODO : Bugfix
            // REQUIRE(t4 == obj1.LastModified());
        }
    }
}

TEST_CASE("Transactions_Bugs", "[transaction]")
{
    SECTION("List-Edit", "Object sublist edit must propagate up as object edits too")
    {
        Transactions::Object::Data obj1;
        obj1.list1().listobj().push_back({});
        Stencil::Transaction<Transactions::Object::Data> txn(obj1);
        txn.list1().edit_listobj(0).obj1().set_val1(1);
        txn.Flush();
        REQUIRE(txn.list1().IsChanged());
        REQUIRE(txn.IsChanged());
    }
}
