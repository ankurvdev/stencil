#include "Avid.pidl.h"

#include "TestUtils.h"

#include <cstdint>

TEST_CASE("OptionalProps", "[OptionalProps]")
{
    SECTION("value")
    {
        Avid::GPS data;
        using FieldIndex = Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields;
        REQUIRE_FALSE(data.IsValid(FieldIndex::Field_climb));
        data.climb = 0.0;
        REQUIRE(data.IsValid(FieldIndex::Field_climb));
    }
}

TEST_CASE("ObservableProps", "[ObservableProps]")
{
    SECTION("value")
    {
        Avid::GPS data;
        using FieldIndex = Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields;

        {
            Stencil::Transaction<Avid::GPS> ctx(data);
            REQUIRE_FALSE(data.IsValid(FieldIndex::Field_climb));
            ctx.set_climb(0.0);
            REQUIRE(data.IsValid(FieldIndex::Field_climb));
            REQUIRE(ctx.IsFieldChanged(FieldIndex::Field_climb));
            REQUIRE(ctx.CountFieldsChanged() == 1);
        }
        {
            Stencil::Transaction<Avid::GPS> ctx(data);
            REQUIRE(data.IsValid(FieldIndex::Field_climb));
            ctx.set_climb(0.0);
            REQUIRE(data.IsValid(FieldIndex::Field_climb));
            REQUIRE_FALSE(ctx.IsFieldChanged(FieldIndex::Field_climb));
            REQUIRE(ctx.CountFieldsChanged() == 0);
        }
    }

    SECTION("list-add")
    {
        Avid::Traffic  data;
        Avid::Aircraft aircraft;
        using FieldIndex = Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields;

        aircraft.hexaddr = {1, 1, 1, 1, 1, 1, 1};
        Stencil::Transaction<Avid::Traffic> ctx(data);
        ctx.add_aircrafts(std::move(aircraft));
        REQUIRE(ctx.IsFieldChanged(FieldIndex::Field_aircrafts));
        REQUIRE(ctx.CountFieldsChanged() == 1);
    }
}
