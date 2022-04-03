#include "Avid.pidl.h"
#include "TestUtils.h"

TEST_CASE("CodeGen::JsonDataHandler", "[DataHandlerJson]")
{
    SECTION("ParseAndStringify")
    {
        std::string_view dataStr
            = R"({"aircrafts":[)"
              R"({"seen":"2021-11-21T01:23:45.0000000","addr":1,"hexaddr":1234,"flight":"NABCDEFGH","altitude":18,"groundSpeed":19,"track":20,"lat":21,"lon":22,"verticalRate":23,"messageCount":24,"odd_cprlat":25,"odd_cprlon":26,"odd_cprtime":0,"even_cprlat":27,"even_cprlon":28,"even_cprtime":0},)"
              R"({"seen":"1969-12-31T16:00:00.0000000","addr":2,"hexaddr":5678,"flight":"123","altitude":19,"groundSpeed":20,"track":21,"lat":22,"lon":23,"verticalRate":24,"messageCount":25,"odd_cprlat":26,"odd_cprlon":27,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0},)"
              R"({"seen":"1969-12-31T16:00:00.0000000","addr":3,"hexaddr":9012,"flight":"123456789","altitude":29,"groundSpeed":30,"track":41,"lat":23,"lon":24,"verticalRate":25,"messageCount":26,"odd_cprlat":27,"odd_cprlon":28,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0})"
              R"(]})";
        std::string_view dataStr1
            = R"({"aircrafts":[)"
              R"({"seen":"2021-11-21T01:23:45.0000000","addr":1,"hexaddr":1234,"flight":"NABCDEFGH","altitude":18,"groundSpeed":19,"track":20,"lat":21,"lon":22,"verticalRate":23,"messageCount":24,"odd_cprlat":25,"odd_cprlon":26,"odd_cprtime":0,"even_cprlat":27,"even_cprlon":28,"even_cprtime":0},)"
              R"({"seen":"1969-12-31T16:00:00.0000000","addr":2,"hexaddr":5678,"flight":"123","altitude":19,"groundSpeed":20,"track":21,"lat":22,"lon":23,"verticalRate":24,"messageCount":25,"odd_cprlat":26,"odd_cprlon":27,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0},)"
              R"({"seen":"1969-12-31T16:00:00.0000000","addr":3,"hexaddr":9012,"flight":"123456789","altitude":29,"groundSpeed":30,"track":41,"lat":23,"lon":24,"verticalRate":25,"messageCount":26,"odd_cprlat":27,"odd_cprlon":28,"odd_cprtime":0,"even_cprlat":28,"even_cprlon":29,"even_cprtime":0})"
              R"(]})";
        auto parseData = Stencil::Json::Parse<Avid::Traffic>(dataStr);
        REQUIRE(parseData.aircrafts.size() == 3);
        auto& obj1 = parseData.aircrafts[0];
        auto& obj2 = parseData.aircrafts[1];
        REQUIRE(obj1.addr == 1);
        REQUIRE(obj2.addr == 2);
        auto stringified = Stencil::Json::Stringify(parseData);
        REQUIRE(stringified == dataStr);
        REQUIRE(stringified == Stencil::Json::Stringify(Stencil::Json::Parse<Avid::Traffic>(stringified)));
        REQUIRE(stringified == Stencil::Json::Stringify(Stencil::Json::Parse<Avid::Traffic>(dataStr1)));
    }
}
