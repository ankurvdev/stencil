
#include "EnumConstsInitializers.pidl.h"

#include "TestUtils.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <span>

TEST_CASE("EnumConstsInitializers", "[EnumConstsInitializers]")
{
    SECTION("Simple")
    {
        EnumConstsInitializers::DefaultValues obj{};
        CHECK(obj.enum1 == EnumConstsInitializers::EnumType1::unknown);
        CHECK(obj.enum3 == EnumConstsInitializers::EnumType2::three);
        // CHECK(obj.val1 == "conststring");
        CHECK(obj.val2 == 100);
        CHECK(obj.val3 == Catch::Approx(100.0));
        CHECK(obj.val4 == Catch::Approx(101.1234));
    }
}
