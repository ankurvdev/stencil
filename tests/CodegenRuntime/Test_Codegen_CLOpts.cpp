#include "CLOpts1.pidl.h"
#include "CLOpts2.pidl.h"

#include <stencil/protocol_cli.h>

#include "TestUtils.h"

static_assert(!Stencil::Category::IsPrimitive<CLOpts1::CommandLineOptions>());
static_assert(Stencil::ConceptIndexable<CLOpts1::CommandLineOptions>);
static_assert(!Stencil::ConceptPrimitive<CLOpts1::CommandLineOptions>);

// static_assert(Stencil::ConceptVariant<CLOpts2::CommandLineOptions>);

template <typename TStruct, typename... TArgs> auto ParseArgs(TArgs&&... args)
{
    TStruct          data;
    std::string_view testargv[] = {std::forward<TArgs>(args)...};
    auto             rslt       = Stencil::CLI::Parse<TStruct>(testargv);
    REQUIRE(!rslt.helpRequested);
    REQUIRE(rslt.success);
    return rslt.obj;
}

template <typename TStruct, typename... TArgs> auto RequireGenerateHelpException(TArgs&&... args)
{
    TStruct          data;
    std::string_view testargv[] = {std::forward<TArgs>(args)...};
    auto             rslt       = Stencil::CLI::Parse<TStruct>(testargv);
    REQUIRE(rslt.helpRequested);
    return Stencil::CLI::GenerateHelp(rslt.obj);
}

TEST_CASE("CodeGen::CommandLineArgs::Simplecase", "[CommandLineArgs]")
{
    SECTION("equalassignment")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--workingDirectory=c:\\abc");

        REQUIRE(options.workingDirectory == "c:\\abc");
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("delimitedassignment")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--workingDirectory", "/abc");

        REQUIRE(options.workingDirectory == "/abc");
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("explicitboolean")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--daemon", "on");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == true);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("implicitboolean")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--daemon");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == true);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("implicitboolean")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--daemon", "--workingDirectory", "/abc");

        REQUIRE(options.workingDirectory == "/abc");
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == true);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("implicitboolean")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--no-quiet");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
        REQUIRE(options.quiet == false);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("underscore_as_dash")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--out-dir=something");

        REQUIRE(options.out_dir == "something");
        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 0);
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
        REQUIRE(options.quiet == true);
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
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
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
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
    }

    SECTION("multilibraries")
    {
        auto options = ParseArgs<::CLOpts1::CommandLineOptions>("--libraries=/abc", "--libraries=/def");

        REQUIRE(options.workingDirectory.empty());
        REQUIRE(options.libraries.size() == 2);
        REQUIRE(options.libraries[0] == "/abc");
        REQUIRE(options.libraries[1] == "/def");
        REQUIRE(options.scan.size() == 0);
        REQUIRE(options.httpsPort == 3443);
        REQUIRE(options.daemon == false);
        REQUIRE(options.quiet == true);
        REQUIRE(options.out_dir == ".");
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

TEST_CASE("CodeGen::CommandLineArgs::Help")
{
    static_assert(Stencil::ConceptVariant<CLOpts2::CommandLineOptions>);

    std::vector<std::string> output;

    auto linesoflines = {
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("install", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("queue", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("pause", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("cancel", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("resume", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("update", "--help"),
        RequireGenerateHelpException<::CLOpts2::CommandLineOptions>("hydrate", "--help"),
    };

    for (auto& lines : linesoflines)
    {
        for (auto& l : lines) output.push_back(l);
        output.push_back("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }

    TestCommon::CheckResource<TestCommon::StrFormat>(output, "0");
}
