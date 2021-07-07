#if 0
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
        /*
        * struct simplobj
        * {
        *     key1: string
        *    key2: string
        * }
        * 
        * struct objwithlist {
        *   data: list<int>
        * }
        * 
        * struct test
        * {
        *   key1: string
        *   listofint: list<int>
        *   listoflist: list<list<int>>
        *   listofobj :  list<simplobj>
        *   objoflist: objwithlist
        *   string key2
        * }
        */

        std::string_view args[] = {"test",
                                   "--key1=str0",
                                   "--listofint=1:2:3:4",
                                   "--listoflist={1:2}:{3:4}:{5:6}",
                                   "--listofobj={str1:str2}:{str3:str4}",
                                   "--objoflist",
                                   "--listofint=1:2:4",
                                   "--key2=str5"};

        CommandLineArgs<decltype(meta)>().template Load<std::string_view>(meta, args);

        CheckOutputAgainstResource(meta->tracelines, "testdata_CommandLineReaderHandler_SimpleTest_output.txt");
    }

    SECTION("Negative: Extrabracket")
    {
        auto meta = make_obj();
        meta->AddComponent(make_list());
        meta->AddComponent(make_list_of_list());
        meta->AddComponent(make_list_of_obj());
        meta->AddComponent(make_obj_of_list());

        std::string_view args[] = {"test",
                                   "--key1=str0",
                                   "--listofint=1:2:3:4",
                                   "--listoflist={1:2}:{3:4}:{5:6}}",
                                   "--listofobj={str1:str2}:{str3:str4}",
                                   "--objoflist",
                                   "--listofint=1:2:4",
                                   "--key2=str5"};
#if _WIN32
        if (IsDebuggerPresent())
        {
            return;
        }
#endif

        REQUIRE_THROWS_MATCHES(CommandLineArgs<decltype(meta)>().template Load<std::string_view>(meta, args),
                               CommandLineArgsReader::Exception,
                               Catch::Message("Error processing args : Illegal Bracket usage\ntest --key1=str0 --listofint=1:2:3:4 \n ==> "
                                              "--listoflist={1:2}:{3:4}:{5:6}} <== \n"));

        CheckOutputAgainstResource(meta->tracelines, "testdata_CommandLineReaderHandler_ExtraBracket_output.txt");
    }
}
#endif
