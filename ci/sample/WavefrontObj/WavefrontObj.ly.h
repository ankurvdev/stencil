
#include "WavefrontObj.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace ModelFormat::WavefrontObj
{
    void Load(Context& context, std::istream& strm);

    inline void LoadString(Context& context, std::string_view const& str)
    {
        std::string temp(str); // TODO : avoid reallocating a string
        std::stringstream sstrm(temp);
        return Load(context, sstrm);
    }

    inline void LoadString(Context& context, std::string const& str)
    {
        std::stringstream sstrm(str);
        return Load(context, sstrm);
    }

    inline void LoadFile(Context& context, std::filesystem::path const& path)
    {
        std::ifstream file(path);
        if (!file.is_open()) throw std::invalid_argument("Cannot open file: " + path.string());
        return Load(context, file);
    }
}
