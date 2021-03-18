#include "Avid.pidl.h"

#include "TestUtils.h"

#include <cstdint>

TEST_CASE("CodeGen::OptionalProps", "[OptionalProps]")
{
    SECTION("Positive: Simplecase")
    {
        Avid::GPS::Data data;
        REQUIRE_FALSE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
        data.set_climb(0.0);
        REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
    }
}

TEST_CASE("CodeGen::ObservableProps", "[ObservableProps]")
{
    SECTION("Positive: Simplecase")
    {
        Avid::GPS::Data data;
        {
            auto ctx = data.Edit();
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
                    tracker.ObjKey(Value{i-1}, nullptr);
                    auto handler = tracker.GetHandler();
                    REQUIRE(handler->Name().str() == "climb");
                    REQUIRE(tracker.Stringify() == "0.000000");
                }
            }
        }
        {
            auto ctx = data.Edit();
            REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            ctx.set_climb(0.0);
            REQUIRE(data.IsValid(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE_FALSE(ctx.IsFieldChanged(Avid::GPS::Data::FieldIndex::climb));
            REQUIRE(ctx.CountFieldsChanged() == 0);
        }
    }
}
