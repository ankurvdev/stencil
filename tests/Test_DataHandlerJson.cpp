#include "Test_DataHandlerJson.h"
#include "Avid.pidl.h"
#include "TestUtils.h"

TEST_CASE("CodeGen::JsonDataHandler", "[DataHandlerJson]")
{
#if 0
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
#endif
    SECTION("ParseAndStringify")
    {
        std::string_view dataStr
            = R"({"aircrafts":[)"
              R"({"seen":"2021-11-21T01:02:03.0000000Z","addr":1,"hexaddr":[2,3,4,5,6,7,8],"flight":[99,10,11,12,13,14,15,16,17],"altitude":18,"groundSpeed":19,"track":20,"lat":21.000000,"lon":22.000000,"verticalRate":23,"messageCount":24,"odd_cprlat":25,"odd_cprlon":26,"odd_cprtime":"1970-01-01T00:00:00.0000000Z","even_cprlat":27,"even_cprlon":28,"even_cprtime":"1970-01-01T00:00:00.0000000Z"},)"
              R"({"seen":"1970-01-01T00:00:00.0000000Z","addr":2,"hexaddr":[3,4,5,6,7,8,9],"flight":[10,11,12,13,14,15,16,17,18],"altitude":19,"groundSpeed":20,"track":21,"lat":22.000000,"lon":23.000000,"verticalRate":24,"messageCount":25,"odd_cprlat":26,"odd_cprlon":27,"odd_cprtime":"1970-01-01T00:00:00.0000000Z","even_cprlat":28,"even_cprlon":29,"even_cprtime":"1970-01-01T00:00:00.0000000Z"})"
              R"(]})";
        auto parseData = Json::Parse<Avid::Traffic::Data>(dataStr);
        REQUIRE(parseData->aircrafts().size() == 2);
        auto& obj1 = parseData->aircrafts()[0];
        auto& obj2 = parseData->aircrafts()[1];
        REQUIRE(obj1.addr() == 1);
        REQUIRE(obj2.addr() == 2);
        auto stringified = Json::Stringify(parseData);
        REQUIRE(stringified == dataStr);
        REQUIRE(stringified == Json::Stringify(Json::Parse<Avid::Traffic::Data>(stringified)));
    }
}
