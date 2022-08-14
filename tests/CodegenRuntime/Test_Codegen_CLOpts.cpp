#include "CLOpts1.pidl.h"
#include "CLOpts2.pidl.h"

#include "stencil/stencil.h"
#include <stencil/protocol_cli.h>

#include "TestUtils.h"
#include <iterator>

template <typename TStruct, typename... TArgs> TStruct ParseArgs(TArgs&&... args)
{
    TStruct          data;
    std::string_view testargv[] = {std::forward<TArgs>(args)...};
    return Stencil::CLI::Parse<TStruct>(testargv);
}
#if defined                                                              TODO1

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
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--libraries=/abc,/def");

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
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install().ProductId == "productid");
        REQUIRE(options.install().Repair == false);
        REQUIRE(options.install().TargetVolume.empty());
        REQUIRE(options.install().User.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "--productid=productid", "--TargetVolume=/tmp");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install().ProductId == (("productid")));
        REQUIRE(options.install().Repair == false);
        REQUIRE(options.install().TargetVolume == (("/tmp")));
        REQUIRE(options.install().User.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("install", "--productid=productid", "--TargetVolume=/tmp", "--User=user1");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install().ProductId == (("productid")));
        REQUIRE(options.install().Repair == false);
        REQUIRE(options.install().TargetVolume == (("/tmp")));
        REQUIRE(options.install().User == (("user1")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>(
            "install", "--productid=productid", "--TargetVolume=/tmp", "--User=user1", "--Repair=true");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::install);
        REQUIRE(options.install().ProductId == (("productid")));
        REQUIRE(options.install().Repair == true);
        REQUIRE(options.install().TargetVolume == (("/tmp")));
        REQUIRE(options.install().User == (("user1")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("queue", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::queue);
        REQUIRE(options.queue().ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("pause", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::pause);
        REQUIRE(options.pause().ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("cancel", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::cancel);
        REQUIRE(options.cancel().ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("resume", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::resume);
        REQUIRE(options.resume().ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("update", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::update);
        REQUIRE(options.update().ProductId == (("productid")));
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("update");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::update);
        REQUIRE(options.update().ProductId.empty());
    }
    {
        auto options = ParseArgs<::CLOpts2::CommandLineOptions>("hydrate", "--productid=productid");

        REQUIRE(options.Type() == CLOpts2::CommandLineOptions::VariantType::hydrate);
        REQUIRE(options.hydrate().ProductId == (("productid")));
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
