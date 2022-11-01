
#include "Objects.pidl.h"

#include "TestUtils.h"

#include <stencil/datarecorder.h>
#include <stencil/transactions.binserdes.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <span>

#if 0
static void RecordTrafficAdd(Avid::Traffic& data, std::filesystem::path const& recordlog, uint8_t hexaddr)
{
    Stencil::DataRecorder<Avid::Traffic> recorder(recordlog);

    Avid::Aircraft aircraft;
    aircraft.hexaddr = {hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr};

    Stencil::Transaction<Avid::Traffic> txn(data);
    txn.add_aircrafts(std::move(aircraft));
    recorder.Record(txn);
}

static void RecordTrafficRemoveIndex(Avid::Traffic& data, std::filesystem::path const& recordlog, size_t index)
{
    Stencil::DataRecorder<Avid::Traffic> recorder(recordlog);
    Stencil::Transaction<Avid::Traffic>  txn(data);

    txn.remove_aircrafts(size_t{index});
    recorder.Record(txn);
}

static void RecordTrafficEdit(Avid::Traffic& data, std::filesystem::path const& recordlog, uint8_t hexaddr)
{
    Stencil::DataRecorder<Avid::Traffic> recorder(recordlog);
    Stencil::Transaction<Avid::Traffic>  txn(data);

    auto& subctx = txn.edit_aircrafts(0);
    subctx.set_hexaddr({hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr});
    recorder.Record(txn);
}

static void RecordChangeGPSClimb(std::filesystem::path const& recordlog, double value)
{
    Stencil::DataRecorder<Avid::GPS> recorder(recordlog);
    Avid::GPS                        data;

    Stencil::Transaction<Avid::GPS> txn(data);
    txn.set_climb(double{value});
    recorder.Record(txn);
}
/*
static void RecordChangeGPSSpeed(std::filesystem::path const& recordlog, double value)
{
    Stencil::DataRecorder<Avid::GPS> recorder(recordlog);

    Avid::GPS                       data;
    Stencil::Transaction<Avid::GPS> txn(data);
    txn.set_speed(double{value});
    recorder.Record(txn);
}*/

static void RecordChangeGPSClimbAndSpeed(std::filesystem::path const& recordlog, double climb, double speed)
{
    Stencil::DataRecorder<Avid::GPS> recorder(recordlog);

    Avid::GPS                       data;
    Stencil::Transaction<Avid::GPS> txn(data);
    txn.set_speed(double{speed});
    txn.set_climb(double{climb});
    recorder.Record(txn);
}

TEST_CASE("DataRecorder", "[DataRecorder]")
{
    auto recordlog = std::filesystem::path("record.bin");
    if (std::filesystem::exists(recordlog)) { std::filesystem::remove(recordlog); }

    SECTION("value1")
    {
        RecordChangeGPSClimb(recordlog, 1.0);
        {
            Stencil::DataPlayerT<Avid::GPS> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS>().climb == 0.0);
            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::GPS>().climb == 1.0);
        }
    }

    SECTION("value2")
    {
        RecordChangeGPSClimbAndSpeed(recordlog, 1.0, 1.0);
        {
            Stencil::DataPlayerT<Avid::GPS> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS>().climb == 0.0);
            REQUIRE(replay.Get<Avid::GPS>().speed == 0.0);

            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::GPS>().climb == 1.0);
            REQUIRE(replay.Get<Avid::GPS>().speed == 1.0);
        }
    }

    SECTION("logappend")
    {
        RecordChangeGPSClimbAndSpeed(recordlog, 1.0, 2.0);
        RecordChangeGPSClimbAndSpeed(recordlog, 3.0, 4.0);
        {
            Stencil::DataPlayerT<Avid::GPS> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS>().climb == 0.0);
            REQUIRE(replay.Get<Avid::GPS>().speed == 0.0);
            replay.Start();
            replay.Wait(2);
            REQUIRE(replay.Get<Avid::GPS>().climb == 3.0);
            REQUIRE(replay.Get<Avid::GPS>().speed == 4.0);
        }
    }

    SECTION("list-add")
    {
        Avid::Traffic data;

        RecordTrafficAdd(data, recordlog, 1);
        {
            Stencil::DataPlayerT<Avid::Traffic> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 0);
            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 1);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.at(0).hexaddr.at(0) == 1u);
        }
    }

    SECTION("list-remove")
    {
        Avid::Traffic data;
        RecordTrafficAdd(data, recordlog, 1);
        RecordTrafficAdd(data, recordlog, 2);
        {
            Stencil::DataPlayerT<Avid::Traffic> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 0);
            replay.Start();
            replay.Wait(2);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 2);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.at(0).hexaddr.at(0) == 1u);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.at(1).hexaddr.at(0) == 2u);
        }

        RecordTrafficRemoveIndex(data, recordlog, 0);
        {
            {
                Stencil::DataPlayerT<Avid::Traffic> replay(recordlog);
                REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 0);
                replay.Start();
                replay.Wait(3);
                REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 1);
                REQUIRE(replay.Get<Avid::Traffic>().aircrafts.at(0).hexaddr.at(0) == 2u);
            }
        }
    }

    SECTION("list-edit")
    {
        Avid::Traffic data;
        RecordTrafficAdd(data, recordlog, 1);
        RecordTrafficAdd(data, recordlog, 2);
        RecordTrafficEdit(data, recordlog, 3);
        {
            Stencil::DataPlayerT<Avid::Traffic> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 0);
            replay.Start();
            replay.Wait(3);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.size() == 2);
            REQUIRE(replay.Get<Avid::Traffic>().aircrafts.at(0).hexaddr.at(0) == 3u);
        }
    }

    std::filesystem::remove(recordlog);
}
#endif