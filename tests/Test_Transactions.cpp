#include "TestUtils.h"
#include "Transactions.pidl.h"

#include "stencil/transactions.h"

struct TestReplay
{

    void Replay(std::string_view const& txndata)
    {
        Stencil::MemTransactionRecorder                 record;
        Stencil::Transaction<Transactions::State::Data> txn(obj, record);
        Stencil::StringTransactionDataReader::Replay<Transactions::State::Data>(txn, txndata);
    }
    Transactions::State::Data                    obj;
    std::vector<Stencil::MemTransactionRecorder> records;
};

TEST_CASE("Transactions", "[Transactions]")
{
    TestReplay replay;
    replay.Replay("imu.pitchDeg1E6 = 1000000;imu.pitchDeg1E6 = 1000000;gps.loc.lat1E7 = 10000000;gps.loc.lon1E7 = 10000000");
    replay.Replay("imu.pitchDeg1E6 = 2000000;imu.pitchDeg1E6 = 2000000;gps.loc.lat1E7 = 20000000;gps.loc.lon1E7 = 20000000");
    replay.Replay("traffic.aircrafts:add[0] = {addr: 100}");
    replay.Replay("traffic.aircrafts:add[0] = {addr: 200}");
    replay.Replay("traffic.aircrafts:add[0] = {addr: 300}");
    replay.Replay("traffic.aircrafts:add[0] = {addr: 400}");
    replay.Replay("traffic.aircrafts:add[0] = {addr: 500}");
    replay.Replay("traffic.aircrafts.0.loc.lat1E7 = 30000001;");
    replay.Replay("traffic.aircrafts.1.loc.lat1E7 = 30000002;");
    replay.Replay("traffic.aircrafts.2.loc.lat1E7 = 30000003;");
    replay.Replay("traffic.aircrafts.3.loc.lat1E7 = 30000004;");
    replay.Replay("traffic.aircrafts.4.loc.lat1E7 = 30000005;");
    replay.Replay("traffic.aircrafts:remove[2]");
    replay.Replay("traffic.aircrafts:remove[2]");
    replay.Replay("environment.pressureHgIn1E7 = 110000000");
    replay.Replay("settings.altimeterHgIn1E7 = 222000000");

    // CheckOutputAgainstResource(testsvc->GetChangeDataSnapshots(), "ChangeDataSnapshots");
    // CheckOutputAgainstResource(testsvc->GetChangeDelta(), "ChangeDelta.txt");

    // auto& fdr = *static_cast<FlightDataRecorder const*>(fdrsvc.get());
    // REQUIRE(fdr.GetRecordFiles().size() == 1);
    // CompareFileAgainstResource(fdr.GetRecordFiles()[0], "FDRTrace.bin");

    // TODO
    // 1. Replay FDRTrace on empty object Avid::State::Data{} and compare with final state
    //          auto json = Json::Stringify(testsrc->ReadData().Obj());

    // 2. KML Exporter Tests
}
