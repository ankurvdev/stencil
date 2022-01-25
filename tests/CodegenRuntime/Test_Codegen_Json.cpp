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
              R"({"seen":2021112100,"addr":1,"hexaddr":[2,3,4,5,6,7,8],"flight":"NABCDEFGH","altitude":18,"groundSpeed":19,"track":20,"lat":21.000000,"lon":22.000000,"verticalRate":23,"messageCount":24,"odd_cprlat":25,"odd_cprlon":26,"odd_cprtime":0,"even_cprlat":27,"even_cprlon":28,"even_cprtime":0},)"
              R"({"seen":0,"addr":2,"hexaddr":[3,4,5,6,7,8,9],"flight":"123","altitude":19,"groundSpeed":20,"track":21,"lat":22.000000,"lon":23.000000,"verticalRate":24,"messageCount":25,"odd_cprlat":26,"odd_cprlon":27,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0},)"
              R"({"seen":1,"addr":3,"hexaddr":[4,5,6,7,8,9,0],"flight":"123456789","altitude":29,"groundSpeed":30,"track":41,"lat":23.000000,"lon":24.000000,"verticalRate":25,"messageCount":26,"odd_cprlat":27,"odd_cprlon":28,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0})"
              R"(]})";
        std::string_view dataStr1
            = R"({"aircrafts":[)"
              R"({"seen":2021112100,"addr":1,"hexaddr":[2,3,4,5,6,7,8],"flight":"NABCDEFGH","altitude":18,"groundSpeed":19,"track":20,"lat":21.000000,"lon":22.000000,"verticalRate":23,"messageCount":24,"odd_cprlat":25,"odd_cprlon":26,"odd_cprtime":0,"even_cprlat":27,"even_cprlon":28,"even_cprtime":0},)"
              R"({"seen":0,"addr":2,"hexaddr":[3,4,5,6,7,8,9],"flight":"123","altitude":19,"groundSpeed":20,"track":21,"lat":22.000000,"lon":23.000000,"verticalRate":24,"messageCount":25,"odd_cprlat":26,"odd_cprlon":27,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0},)"
              R"({"seen":1,"addr":3,"hexaddr":[4,5,6,7,8,9,0],"flight":"123456789","altitude":29,"groundSpeed":30,"track":41,"lat":23.000000,"lon":24.000000,"verticalRate":25,"messageCount":26,"odd_cprlat":27,"odd_cprlon":28,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0})"
              R"(]})";
        auto parseData = Stencil::Json::Parse<Avid::Traffic::Data>(dataStr);
        REQUIRE(parseData->aircrafts().size() == 3);
        auto& obj1 = parseData->aircrafts()[0];
        auto& obj2 = parseData->aircrafts()[1];
        REQUIRE(obj1.addr() == 1);
        REQUIRE(obj2.addr() == 2);
        auto stringified = Stencil::Json::Stringify(parseData);
        REQUIRE(stringified == dataStr);
        REQUIRE(stringified == Stencil::Json::Stringify(Stencil::Json::Parse<Avid::Traffic::Data>(stringified)));
        REQUIRE(stringified == Stencil::Json::Stringify(Stencil::Json::Parse<Avid::Traffic::Data>(dataStr1)));
    }
}
