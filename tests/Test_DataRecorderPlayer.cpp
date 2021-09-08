
#include "Avid.pidl.h"

#include "TestUtils.h"

#include <stencil/datarecorder.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <span>

static void RecordTrafficAdd(Avid::Traffic::Data& data, std::filesystem::path const& recordlog, uint8_t hexaddr)
{
    Stencil::FileTransactionRecorder<Avid::Traffic::Data> recorder(recordlog);

    {
        Avid::Aircraft::Data aircraft;
        aircraft.set_hexaddr({hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr});

        recorder.Start(data).add_aircrafts(std::move(aircraft));
    }
}

static void RecordTrafficRemoveIndex(Avid::Traffic::Data& data, std::filesystem::path const& recordlog, size_t index)
{
    Stencil::FileTransactionRecorder<Avid::Traffic::Data> recorder(recordlog);

    recorder.Start(data).remove_aircrafts(size_t{index});
}

static void RecordTrafficEdit(Avid::Traffic::Data& data, std::filesystem::path const& recordlog, uint8_t hexaddr)
{
    Stencil::FileTransactionRecorder<Avid::Traffic::Data> recorder(recordlog);

    auto ctx = recorder.Start(data);
    {
        auto subctx = ctx.edit_aircrafts(0);
        subctx.set_hexaddr({hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr, hexaddr});
    }
}

static void RecordChangeGPSClimb(std::filesystem::path const& recordlog, double value)
{
    Stencil::FileTransactionRecorder<Avid::GPS::Data> recorder(recordlog);
    Avid::GPS::Data                                   data;
    recorder.Start(data).set_climb(double{value});
}

#if 0
static void RecordChangeGPSSpeed(std::filesystem::path const& recordlog, double value)
{
    Avid::GPS::Data                        data;
    Stencil::DataRecorder<Avid::GPS::Data> recorder(recordlog);
    auto                                   ctx = data.Edit();
    ctx.set_speed(double{value});
    recorder.Record(data, ctx);
}
#endif

static void RecordChangeGPSClimbAndSpeed(std::filesystem::path const& recordlog, double climb, double speed)
{
    Avid::GPS::Data                                   data;
    Stencil::FileTransactionRecorder<Avid::GPS::Data> recorder(recordlog);

    auto ctx = recorder.Start(data);
    ctx.set_speed(double{speed});
    ctx.set_climb(double{climb});
}

TEST_CASE("DataRecorder", "[DataRecorder]")
{
    auto recordlog = std::filesystem::path("record.bin");
    if (std::filesystem::exists(recordlog)) { std::filesystem::remove(recordlog); }

    SECTION("value1")
    {
        RecordChangeGPSClimb(recordlog, 1.0);
        {
            Stencil::DataPlayerT<Avid::GPS::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 0.0);
            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 1.0);
        }
    }

    SECTION("value2")
    {
        RecordChangeGPSClimbAndSpeed(recordlog, 1.0, 1.0);
        {
            Stencil::DataPlayerT<Avid::GPS::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 0.0);
            REQUIRE(replay.Get<Avid::GPS::Data>().speed() == 0.0);

            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 1.0);
            REQUIRE(replay.Get<Avid::GPS::Data>().speed() == 1.0);
        }
    }

    SECTION("logappend")
    {
        RecordChangeGPSClimbAndSpeed(recordlog, 1.0, 2.0);
        RecordChangeGPSClimbAndSpeed(recordlog, 3.0, 4.0);
        {
            Stencil::DataPlayerT<Avid::GPS::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 0.0);
            REQUIRE(replay.Get<Avid::GPS::Data>().speed() == 0.0);
            replay.Start();
            replay.Wait(2);
            REQUIRE(replay.Get<Avid::GPS::Data>().climb() == 3.0);
            REQUIRE(replay.Get<Avid::GPS::Data>().speed() == 4.0);
        }
    }

    SECTION("list-add")
    {
        Avid::Traffic::Data data;

        RecordTrafficAdd(data, recordlog, 1);
        {
            Stencil::DataPlayerT<Avid::Traffic::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 0);
            replay.Start();
            replay.Wait(1);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 1);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().at(0).hexaddr().at(0) == 1u);
        }
    }

    SECTION("list-remove")
    {
        Avid::Traffic::Data data;
        RecordTrafficAdd(data, recordlog, 1);
        RecordTrafficAdd(data, recordlog, 2);
        {
            Stencil::DataPlayerT<Avid::Traffic::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 0);
            replay.Start();
            replay.Wait(2);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 2);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().at(0).hexaddr().at(0) == 1u);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().at(1).hexaddr().at(0) == 2u);
        }

        RecordTrafficRemoveIndex(data, recordlog, 0);
        {
            {
                Stencil::DataPlayerT<Avid::Traffic::Data> replay(recordlog);
                REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 0);
                replay.Start();
                replay.Wait(3);
                REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 1);
                REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().at(0).hexaddr().at(0) == 2u);
            }
        }
    }

    SECTION("list-edit")
    {
        Avid::Traffic::Data data;
        RecordTrafficAdd(data, recordlog, 1);
        RecordTrafficAdd(data, recordlog, 2);
        RecordTrafficEdit(data, recordlog, 3);
        {
            Stencil::DataPlayerT<Avid::Traffic::Data> replay(recordlog);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 0);
            replay.Start();
            replay.Wait(3);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().size() == 2);
            REQUIRE(replay.Get<Avid::Traffic::Data>().aircrafts().at(0).hexaddr().at(0) == 3u);
        }
    }

    std::filesystem::remove(recordlog);
}
