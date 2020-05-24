LEXYACC:NAMESPACE ModelFormat::WavefrontObj
LEXYACC:LEX:START

intconstant     ([+-]?[0-9]+)
dubconstant     ([+-]?[0-9]*(\.[0-9]+)?([eE][+-]?[0-9]+)?)
identifier      ([a-zA-Z_](\.[a-zA-Z_0-9]|[a-zA-Z_0-9])*)
whitespace      ([ \t\r]*)
unixcomment     ("#"[^\n]*)

%%
"\n"            {  return token::tok_newline; }
"/"             { return '/'; }
{whitespace}    { /* do nothing */}
{unixcomment}   { /* do nothing */}

"vt"            { return token::tok_texel; }
"vn"            { return token::tok_normal; }
"v"             { return token::tok_vertex; }
"f"             { return token::tok_face; }
"s"             { return token::tok_smooth; }
"o"             { return token::tok_object; }
"g"             { return token::tok_group;  }

"mtllib"        { return token::tok_material; }
"usemtl"        { return token::tok_usematerial; }

{intconstant}   { lval->emplace<int>((int)strtoll(yytext, NULL, 10));  return token::tok_int_constant; }
{dubconstant}   { lval->emplace<double>(atof(yytext));                 return token::tok_dub_constant;}
{identifier}    { lval->emplace<Id>(std::string(yytext));      return token::tok_identifier; }

.           { throw std::runtime_error("Unknown Character"); }

%%
LEXYACC:LEX:END

LEXYACC:YACC:START


%token tok_vertex
%token tok_texel
%token tok_normal
%token tok_face
%token tok_dub_constant
%token tok_int_constant
%token tok_identifier
%token tok_material
%token tok_usematerial
%token tok_object
%token tok_group
%token tok_newline
%token tok_smooth

%token END 0 "end of file"

%type<Id>           tok_identifier 
%type<int>          tok_int_constant 
%type<double>       tok_dub_constant
%type<FaceElem>     FaceElem
%type<FaceElems>    FaceElems
%%

File: Lines {}
    ;

Lines: Lines Line tok_newline
    | Line tok_newline
    ;

Line: 
      tok_vertex    tok_dub_constant tok_dub_constant tok_dub_constant  { context.CreateVertex($2, $3, $4); }
    | tok_texel     tok_dub_constant tok_dub_constant                   { context.CreateTexel($2, $3); }
    | tok_face      FaceElems                                           { context.CreateFace($2); }
    | tok_normal    tok_dub_constant tok_dub_constant tok_dub_constant  { context.CreateNormal($2, $3, $4); }
    | tok_object    tok_identifier                                      { context.SetCurrentObjectName($2); } 
    | tok_group     tok_identifier                                      { context.SetCurrentGroupName($2); }
    | tok_material  tok_identifier                                      { context.SetMaterialLibFile($2); }
    | tok_usematerial tok_identifier                                    { context.SetMaterialName($2); }
    | tok_smooth tok_int_constant                                                      {}
    | tok_smooth tok_identifier {}

    |
    ;

FaceElems: FaceElems FaceElem   { $$ = ListAdd($1, $2); }
    | FaceElem                  { $$ = ListCreate($1); }
    ;

FaceElem: tok_int_constant                                          { $$ = CreateFaceElem($1, InvalidTexel, InvalidNormal); }
    | tok_int_constant '/' tok_int_constant                         { $$ = CreateFaceElem($1, $3, InvalidNormal);           }
    | tok_int_constant '/' tok_int_constant '/' tok_int_constant    { $$ = CreateFaceElem($1, $3, $5);                      }
    ;
%%

LEXYACC:YACC:END
