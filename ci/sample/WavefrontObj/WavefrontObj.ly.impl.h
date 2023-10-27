
#pragma warning(push, 3)
#include "WavefrontObj.yacc.h"         // Generated by bison
#pragma warning(pop)

#include <sstream>
class WavefrontObjFlexLexer;
namespace ModelFormat::WavefrontObj::impl
{
class Scanner : public WavefrontObjFlexLexer
{
public:
    Scanner(std::istream* in) : WavefrontObjFlexLexer(in){}
    Scanner() = delete;
    ~Scanner() override = default;
    Scanner(Scanner const&) = delete;
    Scanner(Scanner&&) = delete;

    // get rid of override virtual function warning
#undef yylex
    int yylex(BisonParser::semantic_type* const lval, BisonParser::location_type* location);

    int yylex() override { throw std::logic_error("Invalid call"); }
private:
    std::stringstream sstr;    // TODO  Move this outside
};

}
