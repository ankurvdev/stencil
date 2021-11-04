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
                  "list1.listobj:add[0] = {\"value\":100,\"obj1\":{\"time\":0,\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");
    replay.Replay("list1.listobj:add[1] = {\"value\":200,\"obj1\":{\"time\":0,\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[2] = {\"value\":300,\"obj1\":{\"time\":0,\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[3] = {\"value\":400,\"obj1\":{\"time\":0,\"val1\":0,\"val2\":0,"
                  "\"val3\":0,\"val4\":null,\"val5\":0.000000}}");

    replay.Replay("list1.listobj:add[4] = {\"value\":500,\"obj1\":{\"time\":0,\"val1\":0,\"val2\":0,"
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
        auto                       t1 = obj1.obj1().time();

        Stencil::Transaction<Transactions::Object::Data> txn(obj1);
        txn.obj1().set_val1(1000);
        auto t2 = obj1.obj1().time();

        txn.Flush();
        auto t3 = obj1.obj1().time();
        REQUIRE(t1 == t2);
        REQUIRE(t1 < t3);
    }

    SECTION("Auto Update On List Add", "Timestamp should automatically update when a list item is added") {}

    SECTION("Auto Update On List Remove", "Timestamp should automatically update when a list item is removed") {}

    SECTION("Auto Update On List Edit", "Timestamp should automatically update when a list is edited") {}

    SECTION("Auto Update On Struct Edit", "Timestamp should automatically update when a field struct is edited") {}

    SECTION("Update only on finalize/flush", "Timestamp should be updated only when the transaction is finalized") {}

    SECTION("No update on false edits", "Timestamp should not be updated on false edits/ no value changes") {}
}
