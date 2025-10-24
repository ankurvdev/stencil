#include "ObjectsTester.h"
#include "TestUtils.h"
#include "stencil/protocol_json.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("JsonNestedList", "[Json]")
{
    SECTION("test1")
    {
        auto jsonstr = R"([{"list1": {"liststr": ["0"]}},{"list1": {"liststr": ["1"]}}])";
        auto obj     = Stencil::Json::Parse<std::vector<std::shared_ptr<Objects::NestedObject>>>(jsonstr);
        REQUIRE(obj.size() > 0);
        CHECK(obj[0]->list1.liststr[0] == "0");
        REQUIRE(obj.size() > 1);
        CHECK(obj[1]->list1.liststr[0] == "1");
    }
}