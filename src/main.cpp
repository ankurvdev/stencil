#include "Generator.h"
#include "Thrift.h"

#include <cxxopts.hpp>
#include <iostream>
#include <string>

namespace stdfs = std::filesystem;

std::vector<stdfs::path> Execute(bool                            dryrun,
                                 bool                            loadBuiltIn,
                                 std::string const&              format,
                                 std::vector<stdfs::path> const& templates,
                                 stdfs::path const&              outdir,
                                 std::vector<stdfs::path> const& input)
{
    auto generator = Generator::Get(format);
    for (auto const& tp : templates)
    {
        generator->LoadTemplate(tp);
    }

    if (loadBuiltIn)
    {
        generator->LoadBuilltinTemplates();
    }

    if (!stdfs::is_directory(outdir))
    {
        throw std::invalid_argument("Output directory does not exist : " + outdir.string());
    }

    for (auto const& f : input)
    {
        generator->LoadFile(f);
    }

    auto outfiles = generator->Generate(dryrun, outdir);

    if (outfiles.size() == 0)
    {
        throw std::runtime_error("No output files generated");
    }

    return outfiles;
}

int main(int argc, char* argv[])
try
{
    stdfs::path              outdir{stdfs::current_path()};
    bool                     dryrun = false;
    std::vector<stdfs::path> templates;
    std::vector<stdfs::path> inputs;
    std::string              format("thrift");

    cxxopts::Options options(argv[0], " - command line options");
    options.positional_help("[optional args]").show_positional_help();
    options.add_options()("o,outdir", "Output Directory", cxxopts::value(outdir));
    options.add_options()("t,templates", "Load Template Files/Directory", cxxopts::value(templates));
    options.add_options()("n,dryrun", "Dryrun. Do not generate files. Show output file names only", cxxopts::value(dryrun));
    options.add_options()("i,input", "Input Files", cxxopts::value(inputs));
    options.add_options()("f,format", "Input File Format", cxxopts::value(format));

    options.parse_positional("input");

    auto result = options.parse(argc, argv);
    for (auto const& i : inputs)
    {
        auto outfiles = Execute(dryrun, true, format, templates, outdir, {i});
        for (auto const& o : outfiles)
        {
            std::cout << o.string() << std::endl;
        }
    }
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    // std::cerr << IDLDebug::ErrorAggregator::Get().GetErrors() << std::endl;
    return 1;
}
