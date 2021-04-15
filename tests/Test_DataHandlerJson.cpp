#include "Test_DataHandlerJson.h"
#include "TestUtils.h"

TEST_CASE("CodeGen::JsonDataHandler", "[DataHandlerJson]")
{
    SECTION("Positive: Simplecase")
    {
        auto data = Json::Parse<TestStruct1>(
            "{\"field1\":\"value1\", \"field2\": true, \"field3\": 41, \"field4\":[4,1], \"field5\":{\"field1\":\"value2\"}}");
        REQUIRE(data->get_field1() == "value1");
        REQUIRE(data->get_field2());
        REQUIRE(data->get_field3() == 41);
        REQUIRE(data->get_field4().size() == 2);
        REQUIRE(data->get_field4()[0] == 4);
        REQUIRE(data->get_field4()[1] == 1);
        REQUIRE(data->get_field5() != nullptr);
        REQUIRE(data->get_field5()->get_field1() == "value2");
    }
}
