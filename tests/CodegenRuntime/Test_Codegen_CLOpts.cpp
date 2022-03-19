#include "CLOpts1.pidl.h"
#include "CLOpts2.pidl.h"

#include "stencil/stencil.h"
#include <stencil/protocol_cli.h>

#define CATCH_CONFIG_MAIN
#include "TestUtils.h"
#include <iterator>

template <typename TStruct, typename... TArgs> TStruct ParseArgs(TArgs&&... args)
{
    TStruct          data;
    std::string_view testargv[] = {"test", std::forward<TArgs>(args)...};
    return Stencil::CLI::Parse<TStruct>(testargv);
}

template <typename TException> struct ExceptionMatcher : public Catch::MatcherBase<TException>
{
    bool match(TException const& se) const override { return _stricmp(se.what(), _expected->what()) == 0; }

    std::string describe() const override { return typeid(TException).name(); }

    ExceptionMatcher(const TException& obj) : _expected(&obj) {}

    const TException* _expected;
};
#if defined TODO1
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
#endif
TEST_CASE("CodeGen::CommandLineArgs::Simplecase", "[CommandLineArgs]")
{
    SECTION("workingdirectory")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--workingDirectory=c:\\abc");

        REQUIRE(options.workingDirectory == "c:\\abc");
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
    }

    SECTION("libraries")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--libraries=/abc");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 1);
        REQUIRE(options.libraries[0] == "/abc");
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
    }

    SECTION("multilibraries")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--libraries=/abc:/def");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 2);
        REQUIRE(options.libraries[0] == "/abc");
        REQUIRE(options.libraries[1] == "/def");
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
    }
}

TEST_CASE("CodeGen::CommandLineArgs::Variants")
{
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install.ProductId == "productid");
        REQUIRE(options.install.Repair == false);
        REQUIRE(options.install.TargetVolume.empty());
        REQUIRE(options.install.User.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "productid", "--TargetVolume=/tmp");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install.ProductId == (("productid")));
        REQUIRE(options.install.Repair == false);
        REQUIRE(options.install.TargetVolume == (("/tmp")));
        REQUIRE(options.install.User.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "productid", "productid", "--TargetVolume=/tmp", "--User=user1");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install.ProductId == (("productid")));
        REQUIRE(options.install.Repair == false);
        REQUIRE(options.install.TargetVolume == (("/tmp")));
        REQUIRE(options.install.User == (("user1")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "productid", "--TargetVolume=/tmp", "--User=user1", "--Repair");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install.ProductId == (("productid")));
        REQUIRE(options.install.Repair == true);
        REQUIRE(options.install.TargetVolume == (("/tmp")));
        REQUIRE(options.install.User == (("user1")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("queue", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::queue);
        REQUIRE(options.queue.ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("pause", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::pause);
        REQUIRE(options.pause.ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("cancel", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::cancel);
        REQUIRE(options.cancel.ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("resume", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::resume);
        REQUIRE(options.resume.ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("update", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::update);
        REQUIRE(options.update.ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("update");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::update);
        REQUIRE(options.update.ProductId.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("hydrate", "productid");

        REQUIRE(options.type == CLOpts2::CommandLineOptions::VariantType::hydrate);
        REQUIRE(options.hydrate.ProductId == (("productid")));
    }
}
#if defined TODO1
TEST_CASE("CodeGen::CommandLineArgs::Help")
{
    static_assert(std::is_base_of<CommandLineArgs<::CLOpts1::CommandLineOptions>::Exception,
                                  CommandLineArgs<::CLOpts1::CommandLineOptions>::HelpException>::value);
    std::vector<std::string> output;
    auto                     linesoflines = {RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("install", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("queue", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("pause", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("cancel", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("resume", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("update", "--help"),
                         RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("hydrate", "--help")};

    for (auto& lines : linesoflines)
    {
        for (auto& l : lines) output.push_back(l);
        output.push_back("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }

    CheckOutputAgainstResource(output, "testdata_output_CLOpts2_Variant_Help.txt");
}
#endif
TEST_CASE("CodeGen::CommandLineArgs::CLOptsTest")
{
    SECTION("Positive: Simplecase")
    {
        auto options = ParseArgs<::CLOpts1::CLOptsTest>("test",
                                                        "--key1=str0",
                                                        "--listofint=1:2:3:4",
                                                        "--listoflist={1:2}:{3:4}:{5:6}",
                                                        "--listofobj={str1:str2}:{str3:str4}",
                                                        "--objoflist",
                                                        "--field1=1:2:4",
                                                        "--key2=str5");
        REQUIRE(options.key1 == "str0");
        REQUIRE(options.listofint.size() == 4);
        REQUIRE(options.listofint[3] == 4);
        REQUIRE(options.listoflist.size() == 3);
        REQUIRE(options.listoflist[0].size() == 2);
        REQUIRE(options.listoflist[2].size() == 2);
        REQUIRE(options.listoflist[2][1] == 6);
        REQUIRE(options.listoflist[1][1] == 4);
        REQUIRE(options.listoflist[0][1] == 2);
        REQUIRE(options.listofobj.size() == 2);
        REQUIRE(options.listofobj[1].field1 == "str3");
        REQUIRE(options.listofobj[1].field2 == "str4");
        REQUIRE(options.objoflist.field1.size() == 3);
        REQUIRE(options.objoflist.field1[2] == "4");
        REQUIRE(options.key2 == "str5");
    }

    SECTION("Negative: Extrabracket")
    {
#if defined _WIN32
        if (IsDebuggerPresent()) { return; }
#endif

        REQUIRE_THROWS_MATCHES(ParseArgs<::CLOpts1::CLOptsTest>("test",
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
