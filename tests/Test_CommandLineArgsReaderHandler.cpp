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

        std::string_view args[] = {"test",
                                   "--key1=str0",
                                   "--listofint=1:2:3:4",
                                   "--listoflist={1:2}:{3:4}:{5:6}",
                                   "--listofobj={str1:str2}:{str3:str4}",
                                   "--objoflist",
                                   "--listofint=1:2:4",
                                   "--key2=str5"};

        REQUIRE_NOTHROW(CommandLineArgsReader(&handler).template Parse<std::string_view>(args));
        CheckOutputAgainstResource(handler._tracker.lines, "testdata_CommandLineReaderHandler_SimpleTest_output.txt");
    }

    SECTION("Negative: Extrabracket")
    {
        auto meta = make_obj();
        meta->AddComponent(make_list());
        meta->AddComponent(make_list_of_list());
        meta->AddComponent(make_list_of_obj());
        meta->AddComponent(make_obj_of_list());

        Handler handler(meta);

        std::string_view args[] = {"test",
                                   "--key1=str0",
                                   "--listofint=1:2:3:4",
                                   "--listoflist={1:2}:{3:4}:{5:6}}",
                                   "--listofobj={str1:str2}:{str3:str4}",
                                   "--objoflist",
                                   "--listofint=1:2:4",
                                   "--key2=str5"};

        REQUIRE_THROWS_MATCHES(CommandLineArgsReader(&handler).template Parse<std::string_view>(args),
                               CommandLineArgsReader::Exception,
                               Catch::Message("Error processing args : Illegal Bracket usage\ntest --key1=str0 --listofint=1:2:3:4 \n ==> "
                                              "--listoflist={1:2}:{3:4}:{5:6}} <== \n"));
        CheckOutputAgainstResource(handler._tracker.lines, "testdata_CommandLineReaderHandler_ExtraBracket_output.txt");

    }
}
