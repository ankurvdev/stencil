//#include "EmbeddedResource.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

static std::string FilePathToSym(std::filesystem::path filepath)
{
    auto sym = filepath.filename().string();
    replace(sym.begin(), sym.end(), '.', '_');
    replace(sym.begin(), sym.end(), '-', '_');
    if (std::isdigit(sym[0]))
    {
        return "_" + sym;
    }
    return sym;
}

static auto ParseArg(std::string_view arg)
{
    // If name is resname:filepath use resname or else convert filename into a symbol
    size_t                idx = static_cast<size_t>(arg.find('!'));
    std::string           resname;
    std::filesystem::path src;
    if (idx == std::string::npos)
    {
        src     = std::filesystem::path(arg);
        resname = src.filename().string();
    }
    else
    {
        if (idx == 0)
        {
            throw std::invalid_argument("Invalid name for resource: " + std::string(arg));
        }
        src     = std::filesystem::path(std::string(arg.begin() + static_cast<int>(idx + 1), arg.end()));
        resname = std::string_view(arg.data(), idx);
    }

    return std::make_tuple(std::move(resname), std::move(src), FilePathToSym(src));
}
int main(int argc, char** argv)
try
{
    if (argc < 3)
    {
        for (int i = 0; i < argc; i++)
        {
            std::cerr << " " << argv[i];
        }

        std::cerr << std::endl
                  << std::endl
                  << "USAGE: %s {sym} {rsrc}..." << std::endl
                  << std::endl
                  << "  Creates {sym}.c from the contents of each {rsrc}\n"
                  << "  Each resource {rsrc} can be specified in the format [name:]filepath" << argv[0];
        return EXIT_FAILURE;
    }

    std::filesystem::path dst{argv[1]};
    create_directories(dst.parent_path());

    std::ofstream ofs{dst.string()};
    ofs << "#include <EmbeddedResource.h>" << std::endl;

    auto                     colsym = FilePathToSym(dst.stem());
    std::vector<std::string> symbols;

    for (int i = 2; i < argc; i++)
    {
        auto [resname, src, sym] = ParseArg(std::string_view(argv[i]));
        std::ifstream ifs{src, std::ios::binary | std::ios::in};
        if (!ifs.is_open())
        {
            throw std::invalid_argument("Cannot find file" + src.string());
        }
        if (ifs.fail())
        {
            throw std::logic_error("file corrupt: " + src.string());
        }

        uint8_t c;
        ifs.read(reinterpret_cast<char*>(&c), sizeof(c));
        if (ifs.fail())
        {
            continue;
        }
        symbols.push_back(FilePathToSym(src));
        ofs << "namespace EmbeddedResource::Data::" << colsym << "::Resources::" << sym << " {" << std::endl;
        ofs << "static constexpr uint8_t _ResourceData[] = {" << std::endl;

        for (size_t j = 0; !ifs.eof() && !ifs.fail(); j++, ifs.read(reinterpret_cast<char*>(&c), sizeof(c)))
        {
            if (j > 0)
            {
                ofs << ",";
            }
            ofs << "0x" << std::hex << static_cast<uint32_t>(c) << "u";
            if ((j + 1) % 10 == 0)
            {
                ofs << std::endl;
            }
        }

        ofs << "};" << std::endl;

        ofs << "static constexpr std::wstring_view _ResourceName = L\"" << resname << "\";" << std::endl;

        ofs << "}" << std::endl << std::endl;
    }

    for (const auto& ressym : symbols)
    {
        ofs << "DECLARE_RESOURCE(" << colsym << "," << ressym << ");" << std::endl;
    }

    for (const auto& ressym : symbols)
    {
        ofs << "DECLARE_RESOURCE(" << colsym << "," << ressym << ")" << std::endl;
        ofs << "{" << std::endl;
        ofs << "  auto nameptr = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceName.data();" << std::endl;
        ofs << "  auto namelen = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceName.size();" << std::endl;
        ofs << "  auto dataptr = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceData;" << std::endl;
        ofs << "  auto datalen = std::size(EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceData);"
            << std::endl;
        ofs << "    return EmbeddedResource::ABI::ResourceInfo { { nameptr, namelen }, { dataptr, datalen } };" << std::endl;
        ofs << "}" << std::endl;
    }

    ofs << "namespace EmbeddedResource::Data::" << colsym << " {" << std::endl;
    ofs << "static constexpr EmbeddedResource::ABI::GetCollectionResourceInfo * const _ResourceTable[] = {" << std::endl;
    bool first = true;
    for (const auto& ressym : symbols)
    {
        if (!first)
        {
            ofs << ",";
        }
        else
        {
            first = false;
        }
        ofs << "EMBEDDEDRESOURCE_ABI_RESOURCE_FUNCNAME(" << colsym << "," << ressym << ", GetCollectionResourceInfo)" << std::endl;
    }
    ofs << "};" << std::endl;
    ofs << "}" << std::endl;
    ofs << "DECLARE_RESOURCE_COLLECTION(" << colsym << ");" << std::endl;

    ofs << "DECLARE_RESOURCE_COLLECTION(" << colsym << ")" << std::endl;
    ofs << "{" << std::endl;
    ofs << "    auto tableptr = EmbeddedResource::Data::" << colsym << "::_ResourceTable;" << std::endl;
    ofs << "    auto tablelen = std::size(EmbeddedResource::Data::" << colsym << "::_ResourceTable);" << std::endl;
    ofs << "    return EmbeddedResource::ABI::Data<EmbeddedResource::ABI::GetCollectionResourceInfo*> {tableptr, tablelen };" << std::endl;
    ofs << "}" << std::endl;

    ofs.close();
    return EXIT_SUCCESS;
}
catch (std::exception const& ex)
{
    std::cerr << ex.what() << std::endl;
    return -1;
}
