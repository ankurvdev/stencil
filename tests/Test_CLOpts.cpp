#include "CLOpts1.pidl.h"
#include "CLOpts2.pidl.h"

#include "stencil/stencil.h"

#define CATCH_CONFIG_MAIN
#include "TestUtils.h"
#include <iterator>

template <typename TStruct, typename... TArgs> TStruct ParseArgs(TArgs&&... args)
{
    TStruct          data;
    std::string_view testargv[] = {"test", std::forward<TArgs>(args)...};
    CommandLineArgs<TStruct>().template Load<std::string_view>(data, testargv);
    return data;
}

template <typename TException> struct ExceptionMatcher : public Catch::MatcherBase<TException>
{
    bool match(TException const& se) const override { return _stricmp(se.what(), _expected->what()) == 0; }

    std::string describe() const override { return typeid(TException).name(); }

    ExceptionMatcher(const TException& obj) : _expected(&obj) {}

    const TException* _expected;
};

template <typename TStruct, typename TException, typename... TArgs> auto RequireGenerateException(const TException& obj, TArgs&&... args)
{
    REQUIRE_THROWS_MATCHES(ParseArgs<TStruct>(std::forward<TArgs>(args)...), TException, ExceptionMatcher{obj});
}

template <typename TStruct, typename... TArgs> std::vector<std::string> RequireGenerateHelpException(TArgs&&... args)
{
    TStruct                  data;
    std::string_view         testargv[] = {"test", std::forward<TArgs>(args)...};
    CommandLineArgs<TStruct> cli;
    cli.template Load<std::string_view>(data, testargv);
    return cli.HelpInfo();
}

TEST_CASE("CodeGen::CommandLineArgs::Simplecase", "[CommandLineArgs]")
{
    SECTION("workingdirectory")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions::Data>("--workingDirectory=c:\\abc");

        REQUIRE(options.workingDirectory() == "c:\\abc");
        REQUIRE(options.libraries().size() == 0);
        REQUIRE(options.scan().size() == 0);
        REQUIRE(options.httpsPort() == 3443);
        REQUIRE(options.daemon() == false);
    }

    SECTION("libraries")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions::Data>("--libraries=/abc");

        REQUIRE(options.workingDirectory().empty());
        REQUIRE(options.libraries().size() == 1);
        REQUIRE(options.libraries()[0] == "/abc");
        REQUIRE(options.scan().size() == 0);
        REQUIRE(options.httpsPort() == 3443);
        REQUIRE(options.daemon() == false);
    }

    SECTION("multilibraries")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions::Data>("--libraries=/abc:/def");

        REQUIRE(options.workingDirectory().empty());
        REQUIRE(options.libraries().size() == 2);
        REQUIRE(options.libraries()[0] == "/abc");
        REQUIRE(options.libraries()[1] == "/def");
        REQUIRE(options.scan().size() == 0);
        REQUIRE(options.httpsPort() == 3443);
        REQUIRE(options.daemon() == false);
    }
}

TEST_CASE("CodeGen::CommandLineArgs::Unions")
{
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("install", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::install);
        REQUIRE(options.install().ProductId() == "productid");
        REQUIRE(options.install().Repair() == false);
        REQUIRE(options.install().TargetVolume().empty());
        REQUIRE(options.install().User().empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("install", "productid", "--TargetVolume=/tmp");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::install);
        REQUIRE(options.install().ProductId() == (("productid")));
        REQUIRE(options.install().Repair() == false);
        REQUIRE(options.install().TargetVolume() == (("/tmp")));
        REQUIRE(options.install().User().empty());
    }
    {
        auto options
            = ParseArgs<::CLOpts2::CommandLineOptions::Data>("install", "productid", "productid", "--TargetVolume=/tmp", "--User=user1");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::install);
        REQUIRE(options.install().ProductId() == (("productid")));
        REQUIRE(options.install().Repair() == false);
        REQUIRE(options.install().TargetVolume() == (("/tmp")));
        REQUIRE(options.install().User() == (("user1")));
    }
    {
        auto options
            = ParseArgs<::CLOpts2::CommandLineOptions::Data>("install", "productid", "--TargetVolume=/tmp", "--User=user1", "--Repair");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::install);
        REQUIRE(options.install().ProductId() == (("productid")));
        REQUIRE(options.install().Repair() == true);
        REQUIRE(options.install().TargetVolume() == (("/tmp")));
        REQUIRE(options.install().User() == (("user1")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("queue", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::queue);
        REQUIRE(options.queue().ProductId() == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("pause", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::pause);
        REQUIRE(options.pause().ProductId() == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("cancel", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::cancel);
        REQUIRE(options.cancel().ProductId() == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("resume", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::resume);
        REQUIRE(options.resume().ProductId() == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("update", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::update);
        REQUIRE(options.update().ProductId() == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("update");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::update);
        REQUIRE(options.update().ProductId().empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions::Data>("hydrate", "productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::UnionType::hydrate);
        REQUIRE(options.hydrate().ProductId() == (("productid")));
    }
}

TEST_CASE("CodeGen::CommandLineArgs::Help")
{
    static_assert(std::is_base_of<CommandLineArgs<::CLOpts1::CommandLineOptions::Data>::Exception,
                                  CommandLineArgs<::CLOpts1::CommandLineOptions::Data>::HelpException>::value);
    std::vector<std::string> output;
    auto                     linesoflines = {RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("install", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("queue", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("pause", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("cancel", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("resume", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("update", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions::Data>("hydrate", "--help")};

    for (auto& lines : linesoflines)
    {
        for (auto& l : lines) output.push_back(l);
        output.push_back("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }

    CheckOutputAgainstResource(output, "testdata_output_CLOpts2_Union_Help.txt");
}

TEST_CASE("CodeGen::CommandLineArgs::CLOptsTest")
{
    SECTION("Positive: Simplecase")
    {
        auto options = ParseArgs<::CLOpts1::CLOptsTest::Data>("test",
                                                              "--key1=str0",
                                                              "--listofint=1:2:3:4",
                                                              "--listoflist={1:2}:{3:4}:{5:6}",
                                                              "--listofobj={str1:str2}:{str3:str4}",
                                                              "--objoflist",
                                                              "--field1=1:2:4",
                                                              "--key2=str5");
        REQUIRE(options.key1() == "str0");
        REQUIRE(options.listofint().size() == 4);
        REQUIRE(options.listofint()[3] == 4);
        REQUIRE(options.listoflist().size() == 3);
        REQUIRE(options.listoflist()[0].size() == 2);
        REQUIRE(options.listoflist()[2].size() == 2);
        REQUIRE(options.listoflist()[2][1] == 6);
        REQUIRE(options.listoflist()[1][1] == 4);
        REQUIRE(options.listoflist()[0][1] == 2);
        REQUIRE(options.listofobj().size() == 2);
        REQUIRE(options.listofobj()[1].field1() == "str3");
        REQUIRE(options.listofobj()[1].field2() == "str4");
        REQUIRE(options.objoflist().field1().size() == 3);
        REQUIRE(options.objoflist().field1()[2] == "4");
        REQUIRE(options.key2() == "str5");
    }

    SECTION("Negative: Extrabracket")
    {
#if _WIN32
        if (IsDebuggerPresent())
        {
            return;
        }
#endif

        REQUIRE_THROWS_MATCHES(ParseArgs<::CLOpts1::CLOptsTest::Data>("test",
                                                                      "--key1=str0",
                                                                      "--listofint=1:2:3:4",
                                                                      "--listoflist={1:2}:{3:4}:{5:6}}",
                                                                      "--listofobj={str1:str2}:{str3:str4}",
                                                                      "--objoflist",
                                                                      "--listofint=1:2:4",
                                                                      "--key2=str5"),
                               CommandLineArgsReader::Exception,
                               Catch::Message("Error processing args : Illegal Bracket usage\ntest --key1=str0 --listofint=1:2:3:4 \n ==> "
                                              "--listoflist={1:2}:{3:4}:{5:6}} <== \n"));
    }
}
