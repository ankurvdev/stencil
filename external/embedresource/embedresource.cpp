#include "EmbeddedResource.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static std::string FilePathToSym(std::filesystem::path filepath)
{
    auto sym = filepath.filename().string();
    replace(sym.begin(), sym.end(), '.', '_');
    replace(sym.begin(), sym.end(), '-', '_');
    if (std::isdigit(sym[0])) { return "_" + sym; }
    return sym;
}

#define NL "\n"
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
        if (idx == 0) { throw std::invalid_argument("Invalid name for resource: " + std::string(arg)); }
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
        for (int i = 0; i < argc; i++) { std::cerr << " " << argv[i]; }

        std::cerr << NL << NL << "USAGE: %s {sym} {rsrc}..." << NL << NL << "  Creates {sym}.c from the contents of each {rsrc}\n"
                  << "  Each resource {rsrc} can be specified in the format [name:]filepath" << argv[0];
        return EXIT_FAILURE;
    }

    std::filesystem::path dst{argv[1]};
    create_directories(dst.parent_path());

    std::ofstream ofs{dst.string()};
    ofs << "#pragma clang diagnostic push" << NL << "#pragma clang diagnostic ignored \"-Wunused-macros\"" << NL;
    ofs << "#define EMBEDDED_RESOURCE_EXPORTED_API_IMPL 1" << NL;
    ofs << "#pragma clang diagnostic pop" << NL;
    ofs << "#include <EmbeddedResource.h>" << NL;

    auto                     colsym = FilePathToSym(dst.stem());
    std::vector<std::string> symbols;

    for (int i = 2; i < argc; i++)
    {
        auto [resname, src, sym] = ParseArg(std::string_view(argv[i]));
        std::ifstream ifs{src, std::ios::binary | std::ios::in};
        if (!ifs.is_open()) { throw std::invalid_argument("Cannot find file" + src.string()); }
        if (ifs.fail()) { throw std::logic_error("file corrupt: " + src.string()); }

        uint8_t c;
        ifs.read(reinterpret_cast<char*>(&c), sizeof(c));
        if (ifs.fail()) { continue; }
        symbols.push_back(FilePathToSym(src));
        ofs << "namespace EmbeddedResource::Data::" << colsym << "::Resources::" << sym << " {" << NL;
        ofs << "static constexpr uint8_t _ResourceData[] = {" << NL;

        for (size_t j = 0; !ifs.eof() && !ifs.fail(); j++, ifs.read(reinterpret_cast<char*>(&c), sizeof(c)))
        {
            if (j > 0) { ofs << ","; }
            ofs << "0x" << std::hex << static_cast<uint32_t>(c) << "u";
            if ((j + 1) % 10 == 0) { ofs << NL; }
        }

        ofs << "};" << NL;

        ofs << "static constexpr std::wstring_view _ResourceName = L\"" << resname << "\";" << NL;

        ofs << "}" << NL << NL;
    }

    for (const auto& ressym : symbols) { ofs << "DECLARE_RESOURCE(" << colsym << "," << ressym << ");" << NL; }

    for (const auto& ressym : symbols)
    {
        ofs << "DECLARE_RESOURCE(" << colsym << "," << ressym << ")" << NL;
        ofs << "{" << NL;
        ofs << "  auto nameptr = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceName.data();" << NL;
        ofs << "  auto namelen = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceName.size();" << NL;
        ofs << "  auto dataptr = EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceData;" << NL;
        ofs << "  auto datalen = std::size(EmbeddedResource::Data::" << colsym << "::Resources::" << ressym << "::_ResourceData);" << NL;
        ofs << "    return EmbeddedResource::ABI::ResourceInfo { { nameptr, namelen }, { dataptr, datalen } };" << NL;
        ofs << "}" << NL;
    }

    ofs << "namespace EmbeddedResource::Data::" << colsym << " {" << NL;
    ofs << "static constexpr EmbeddedResource::ABI::GetCollectionResourceInfo * const _ResourceTable[] = {" << NL;
    bool first = true;
    for (const auto& ressym : symbols)
    {
        if (!first) { ofs << ","; }
        else
        {
            first = false;
        }
        ofs << "EMBEDDEDRESOURCE_ABI_RESOURCE_FUNCNAME(" << colsym << "," << ressym << ", GetCollectionResourceInfo)" << NL;
    }
    ofs << "};" << NL;
    ofs << "}" << NL;
    ofs << "DECLARE_RESOURCE_COLLECTION(" << colsym << ");" << NL;

    ofs << "DECLARE_RESOURCE_COLLECTION(" << colsym << ")" << NL;
    ofs << "{" << NL;
    ofs << "    auto tableptr = EmbeddedResource::Data::" << colsym << "::_ResourceTable;" << NL;
    ofs << "    auto tablelen = std::size(EmbeddedResource::Data::" << colsym << "::_ResourceTable);" << NL;
    ofs << "    return EmbeddedResource::ABI::Data<EmbeddedResource::ABI::GetCollectionResourceInfo*> {tableptr, tablelen };" << NL;
    ofs << "}" << NL;

    ofs.close();
    return EXIT_SUCCESS;
} catch (std::exception const& ex)
{
    std::cerr << ex.what() << NL;
    return -1;
}
