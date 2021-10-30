#include "Avid.pidl.h"

#include "TestUtils.h"

#include <cstdint>

TEST_CASE("OptionalProps", "[OptionalProps]")
{
    SECTION("value")
    {
        Avid::GPS::Data data;
        REQUIRE_FALSE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
        data.set_climb(0.0);
        REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
    }
}

TEST_CASE("ObservableProps", "[ObservableProps]")
{
    SECTION("value")
    {
        Avid::GPS::Data data;
        {
            Stencil::Transaction<Avid::GPS::Data> ctx(data);
            REQUIRE_FALSE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            ctx.set_climb(0.0);
            REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE(ctx.IsFieldChanged(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE(ctx.CountFieldsChanged() == 1);
            for (unsigned i = 1; i <= Avid::GPS::Data::FieldCount(); i++)
            {
                if (ctx.IsFieldChanged(static_cast<Avid::GPS::Data::FieldIndex>(i)))
                {
                    ReflectionServices::StateTraker<Avid::GPS::Data, void*> tracker(&data, nullptr);
                    tracker.ObjKey(Value{i - 1}, nullptr);
                    auto handler = tracker.GetHandler();
                    REQUIRE(handler->Name().str() == "climb");
                    REQUIRE(tracker.Stringify() == "0.000000");
                }
            }
        }
        {
            Stencil::Transaction<Avid::GPS::Data> ctx(data);
            REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            ctx.set_climb(0.0);
            REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE_FALSE(ctx.IsFieldChanged(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE(ctx.CountFieldsChanged() == 0);
        }
    }

    SECTION("list-add")
    {
        Avid::Traffic::Data  data;
        Avid::Aircraft::Data aircraft;
        aircraft.set_hexaddr({1, 1, 1, 1, 1, 1, 1});
        Stencil::Transaction<Avid::Traffic::Data> ctx(data);
        ctx.add_aircrafts(std::move(aircraft));
        REQUIRE(ctx.IsFieldChanged(Avid::Traffic::Data::FieldIndex::aircrafts));
        REQUIRE(ctx.CountFieldsChanged() == 1);
    }
}
