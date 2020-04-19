#include "Test_CommandLineArgsReaderHandler.h"
#include "TestUtils.h"

TEST_CASE("CodeGen::CommandLineReaderHandler", "[CommandLineArgsReader]")
{
    SECTION("Positive: Simplecase")
    {
        auto meta = make_obj();
        meta->AddComponent(make_list());
        meta->AddComponent(make_list_of_list());
        meta->AddComponent(make_list_of_obj());
        meta->AddComponent(make_obj_of_list());

        Handler handler(meta);

        const char* args[] = {"test",
                              "--key1=str0",
                              "--listofint=1:2:3:4",
                              "--listoflist={1:2}:{3:4}:{5:6}",
                              "--listofobj={str1:str2}:{str3:str4}",
                              "--objoflist",
                              "--listofint=1:2:4",
                              "--key2=str5"};
        REQUIRE_NOTHROW(CommandLineArgsReader(&handler).Parse(std::size(args), args));
        CheckOutputAgainstResource(handler._tracker.lines, L"testdata_CommandLineReaderHandler_SimpleTest_output.txt");
    }
}
