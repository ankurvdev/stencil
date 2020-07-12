LEXYACC:NAMESPACE IDL::Lang::TypeDefinition
LEXYACC:LEX:START

%s STRING_BEGIN
%s EXPR_BEGIN

HEX_D [a-fA-F0-9]
INT  [-]?[0-9]+
DOUBLE {INT}((\.[0-9]+)?([eE][-+]?[0-9]+)?)
WS [ \t\n\r]
IDENTIFIER    ([a-zA-Z_](\.[a-zA-Z_0-9]|[a-zA-Z_0-9])*)

UNESCAPED_CH [^\"\\\']
FALLBACK_CH .

whitespace    ([ \t\r\n]*)
sillycomm     ("/*""*"*"*/")
multicomm     ("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
doctext       ("/**"([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
comment       ("//"[^\n]*)
unixcomment   ("#"[^\n]*)

%%

<STRING_BEGIN>\"                 { BEGIN(INITIAL); sstr << '"'; lval->emplace<TextValue>(StrOps<TextValue>::Convert(sstr.str())); return token::TOKEN_STRING; }
<STRING_BEGIN>[^\"\\]+    { sstr << yytext; }
<STRING_BEGIN>\\\"                 { sstr << '"';  }
<STRING_BEGIN>\\\\                { sstr << '\\'; }
<STRING_BEGIN>\\\/                { sstr << '/';  }
<STRING_BEGIN>\\b                { sstr << '\b'; }
<STRING_BEGIN>\\f                { sstr << '\f'; }
<STRING_BEGIN>\\n                { sstr << '\n'; }
<STRING_BEGIN>\\r                { sstr << '\r'; }
<STRING_BEGIN>\\t                { sstr << '\t'; }
<STRING_BEGIN>\\u{HEX_D}{HEX_D}{HEX_D}{HEX_D}    { sstr << (char)strtoul(&yytext[2], nullptr, 16); }
<STRING_BEGIN>\\                { sstr << '\\'; }

<EXPR_BEGIN>\'                     { BEGIN(INITIAL); lval->emplace<TextValue>(StrOps<TextValue>::Convert(sstr.str()));  return token::TOKEN_EXPR; }
<EXPR_BEGIN>[^\'\\]+            { sstr << yytext; }
<EXPR_BEGIN>\\\'                 { sstr << '\'';  }
<EXPR_BEGIN>\\\\                { sstr << '\\'; }
<EXPR_BEGIN>\\\/                { sstr << '/';  }
<EXPR_BEGIN>\\b                    { sstr << '\b'; }
<EXPR_BEGIN>\\f                    { sstr << '\f'; }
<EXPR_BEGIN>\\n                    { sstr << '\n'; }
<EXPR_BEGIN>\\r                    { sstr << '\r'; }
<EXPR_BEGIN>\\t                    { sstr << '\t'; }
<EXPR_BEGIN>\\u{HEX_D}{HEX_D}{HEX_D}{HEX_D}    { sstr << (char)strtoul(&yytext[2], nullptr, 16); }
<EXPR_BEGIN>\\                    { sstr << '\\'; }


{whitespace}         { /* do nothing */                 }
{sillycomm}          { /* do nothing */                 }
{multicomm}          { /* do nothing */                 }
{comment}            { /* do nothing */                 }
{unixcomment}        { /* do nothing */                 }


\"                     { sstr.str(""); sstr << "\""; BEGIN(STRING_BEGIN);}
\'                     { sstr.str(""); BEGIN(EXPR_BEGIN);}
"{"                    { return token::TOKEN_LeftBrace; }
"}"                    { return token::TOKEN_RightBrace; }
"<"                    { return token::TOKEN_LessThan; }
">"                    { return token::TOKEN_GreaterThan; }
","                    { return token::TOKEN_Comma; }
"="                    { return token::TOKEN_Equal; }
"relationship"         { return token::TOKEN_RELATIONSHIP; }

{IDENTIFIER}    { lval->emplace<TextValue>(StrOps<TextValue>::Convert(yytext)); return token::TOKEN_IDENTIFIER; }
.               { throw std::runtime_error("Unknown Character:" + std::to_string((int)(yytext[0]))); }
%%
LEXYACC:LEX:END

LEXYACC:YACC:START

%token TOKEN_LeftBrace TOKEN_RightBrace TOKEN_Comma TOKEN_Newline TOKEN_Equal TOKEN_LessThan TOKEN_GreaterThan
%token TOKEN_STRING TOKEN_EXPR
%token TOKEN_IDENTIFIER
%token TOKEN_RELATIONSHIP
%token TOKEN_LEXERROR
%token END 0 "end of file"

%type<TextValue>            TOKEN_STRING TOKEN_IDENTIFIER TOKEN_EXPR TOKEN_LEXERROR
%type<NameWithTypeList>     name optionalname
%type<ContainerTypeList>    typelist containertypes
%type<Attributes>           definition values
%type<KVP>                  value
%type<RelationshipMap>      relationshipcomponents

%start lines
%%

lines : lines line
| line
;

line : TOKEN_RELATIONSHIP TOKEN_IDENTIFIER TOKEN_LeftBrace relationshipcomponents TOKEN_RightBrace { context.CreateRelationshipDefinition(std::move($2), std::move($4)); }
| name optionalname definition { context.CreateFieldDefinition(std::move($1), std::move($2), std::move($3)); }
|
;

optionalname: name { $$ = std::move($1); } | {} ;

definition: TOKEN_LeftBrace values TOKEN_RightBrace { $$ = std::move($2); };

name: TOKEN_IDENTIFIER containertypes { $$ = std::make_pair(std::move($1), std::move($2)); };

containertypes: TOKEN_LessThan typelist TOKEN_GreaterThan { $$ = std::move($2); }
| {}
;

typelist: typelist TOKEN_Comma TOKEN_IDENTIFIER { $1.push_back($3); $$ = std::move($1);}
| TOKEN_IDENTIFIER { $$.push_back(std::move($1)); }
| {}
;

relationshipcomponents: relationshipcomponents commaOrNewline TOKEN_IDENTIFIER TOKEN_Equal TOKEN_IDENTIFIER { $1[std::move($3)] = std::move($5); $$ = std::move($1); }
| TOKEN_IDENTIFIER TOKEN_Equal TOKEN_IDENTIFIER { $$[std::move($1)] = std::move($3);}
| {}
;
values: values commaOrNewline value { $$ = CreateAttributeMapEntry($1, $3); }
| value { $$ = CreateAttributeMapEntry($1); }
| {}
;

value: TOKEN_IDENTIFIER TOKEN_Equal TOKEN_STRING { $$ = std::make_pair(std::move($1), std::move($3)); }
| TOKEN_IDENTIFIER TOKEN_Equal TOKEN_EXPR { $$ = std::make_pair(std::move($1), std::move($3));}

commaOrNewline: TOKEN_Comma
| TOKEN_Newline
;
%%
LEXYACC:YACC:END
