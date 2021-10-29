#include "TestUtils.h"
#include "Transactions.pidl.h"

#include "stencil/transactions.h"

struct TestReplay
{
    TestReplay() : txn2(obj2) {}

    void Replay(std::string_view const& txndata, std::string_view const& expectedIn = {})
    {
        Stencil::Transaction<Transactions::Object::Data> txn(obj1);
        Stencil::StringTransactionSerDes::Apply(txn, txndata);
        snapshots.push_back(Stencil::Json::Stringify(obj1));
        auto expected = expectedIn.size() == 0 ? txndata : expectedIn;
        auto delta    = Stencil::StringTransactionSerDes::Deserialize(txn);
        changes.push_back(delta);
        if (txndata[expected.size() - 1] == ';') { REQUIRE(delta == expected); }
        else
        {
            REQUIRE(delta.substr(0, delta.size() - 1) == expected);
        }
        Stencil::StringTransactionSerDes::Apply(txn2, txndata);
        deltatxns.push_back(Stencil::StringTransactionSerDes::Deserialize(txn2));
    }

    std::vector<std::string> changes;
    std::vector<std::string> snapshots;
    std::vector<std::string> deltatxns;

    Transactions::Object::Data obj1;
    Transactions::Object::Data obj2;

    Stencil::Transaction<Transactions::Object::Data> txn2;

    std::vector<Stencil::MemTransactionRecorder> records;
};

TEST_CASE("Transactions", "[Transactions]")
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
}
