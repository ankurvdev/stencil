LEXYACC:NAMESPACE IDL::Lang::Thrift
LEXYACC:LEX:START

intconstant     ([+-]?[0-9]+)
hexconstant     ("0x"[0-9A-Fa-f]+)
dubconstant     ([+-]?[0-9]*(\.[0-9]+)?([eE][+-]?[0-9]+)?)
identifier      ([a-zA-Z_](\.[a-zA-Z_0-9]|[a-zA-Z_0-9])*)
whitespace      ([ \t\r\n]*)
sillycomm       ("/*""*"*"*/")
multicomm       ("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
doctext         ("/**"([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
comment         ("//"[^\n]*)
unixcomment     ("#"[^\n]*)
symbol          ([:;\,\{\}\(\)\=<>\[\]])
literal_begin   (['\"])
UNESCAPED_CH    [^\"\\]
HEX_D           [a-fA-F0-9]

%s STRING_BEGIN

%%

<STRING_BEGIN>\"                { BEGIN(INITIAL); lval->emplace<Id>(StrOps<Id>::Convert(sstr.str())); return token::tok_literal; }
<STRING_BEGIN>{UNESCAPED_CH}+   { sstr << yytext; }
<STRING_BEGIN>\\\"              { sstr << 'o'; }
<STRING_BEGIN>\\\\              { sstr << '\\'; }
<STRING_BEGIN>\\\/              { sstr << '/'; }
<STRING_BEGIN>\\b               { sstr << '\b'; }
<STRING_BEGIN>\\f               { sstr << '\f'; }
<STRING_BEGIN>\\n               { sstr << '\n'; }
<STRING_BEGIN>\\r               { sstr << '\r'; }
<STRING_BEGIN>\\t               { sstr << '\t'; }
<STRING_BEGIN>\\u{HEX_D}{HEX_D}{HEX_D}{HEX_D} { sstr << (char)strtoul(&yytext[2], nullptr, 16); }
<STRING_BEGIN>\\                { sstr << '\\'; }

{whitespace}    { /* do nothing */}
{sillycomm}     { /* do nothing */}
{multicomm}     { /* do nothing */}
{comment}       { /* do nothing */}
{unixcomment}   { /* do nothing */}

{symbol}        { return yytext[0];}
"*"             { return yytext[0];}

"false" { lval->emplace<int>(0); return token::tok_int_constant; }
"true"  { lval->emplace<int>(1); return token::tok_int_constant; }

"const"         { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_const; }
"datasource"    { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_datasource; }
"event"         { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_event; }
"enum"          { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_enum; }
"exception"     { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_exception; }
"extends"       { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_extends; }
"include"       { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_include; }
"interface"     { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_interface; }
"namespace"     { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_namespace; }
"objectstore"   { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_objectstore; }
"oneway"        { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_oneway; }
"optional"      { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_optional; }
"relationship"  { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_relationship; }
"required"      { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_required; }
"static"        { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_static; }
"struct"        { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_struct; }
"throws"        { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_throws; }
"typedef"       { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_typedef; }
"union"         { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_union; }
"void"          { lval->emplace<Id>(StrOps<Id>::Convert(yytext));    return token::tok_void; }

{intconstant}   { lval->emplace<int>((int)strtoll(yytext, NULL, 10));  return token::tok_int_constant; }
{hexconstant}   { lval->emplace<int>((int)strtoll(yytext, NULL, 16));  return token::tok_int_constant; }
{dubconstant}   { lval->emplace<double>(atof(yytext));                    return token::tok_dub_constant;}
{identifier}    { lval->emplace<Id>(StrOps<Id>::Convert(yytext));              return token::tok_identifier; }

{literal_begin} { sstr.str(""); BEGIN(STRING_BEGIN);}

{doctext}   { }

.           { throw std::runtime_error("Unknown Character"); }

%%
LEXYACC:LEX:END

LEXYACC:YACC:START


%token tok_const
%token tok_datasource
%token tok_enum
%token tok_event
%token tok_exception
%token tok_extends
%token tok_include
%token tok_interface
%token tok_namespace
%token tok_objectstore
%token tok_oneway
%token tok_optional
%token tok_relationship
%token tok_required
%token tok_static
%token tok_struct
%token tok_throws
%token tok_typedef
%token tok_union
%token tok_void

%token tok_dub_constant
%token tok_identifier
%token tok_int_constant
%token tok_literal
%token tok_lexerror
%token END 0 "end of file"

%type<Id>
tok_const
tok_datasource
tok_enum
tok_event
tok_exception
tok_extends
tok_include
tok_interface
tok_namespace
tok_objectstore
tok_oneway
tok_optional
tok_relationship
tok_required
tok_static
tok_struct
tok_throws
tok_typedef
tok_union
tok_void

%type<Id>
tok_identifier
tok_literal
FieldRequiredness
tok_lexerror
Static

%type<int>          tok_int_constant FieldIdentifier
%type<double>       tok_dub_constant

%type<Typedef>              Typedef
%type<ConstValue>           ConstValue     FieldValue
%type<Struct>               Struct
%type<Union>                Union
%type<Interface>            Interface
%type<Interface>            Extends
%type<InterfaceMember>      InterfaceMember
%type<InterfaceFunction>    InterfaceFunction
%type<InterfaceEvent>       InterfaceEvent
%type<InterfaceObjectStore> InterfaceObjectStore
%type<Field>                Field
%type<FieldType>            FieldType FunctionType ContainerType ArrayType
%type<RelationshipComponent>           RelationshipComponent

%type<TypeAttribute>  TypeAttribute

%type<TypeAttributeList>        TypeAttributes TypeAttributeList

%type<FieldTypeList>  ContainerFieldList
%type<FieldList>      FieldList Throws
%type<InterfaceMemberList>   InterfaceMemberList
%type<ConstValueList>      ConstListContents
%type<ConstValueDict>       ConstMapContents
%type<IdList>            identifierlist

%type<RelationshipComponentList>          RelationshipComponents
%type<Program>        Program
%%

Program:  DataSource HeaderList DefinitionList { }
    ;

HeaderList: HeaderList Header
    |
    ;

Header: Include
    | tok_namespace tok_identifier tok_identifier
    | tok_namespace '*' tok_identifier
    ;

Include: tok_include tok_literal        { context.Import($2); }
    ;

DataSource: tok_datasource tok_literal  { context.InitializeModelDataSources($2); }
    |                                    { context.InitializeModelDataSources({}); }
    ;

DefinitionList: DefinitionList Definition
    |
    ;

Definition:

      Typedef
    | Struct
    | Relationship
    | Union
    | Interface
    /*
    | Enum
    | Const
    | Xception
    */
    ;

Typedef: tok_typedef FieldType tok_identifier TypeAttributes    { $$ = CreateTypedef(context, $2, $3, $4); }
    ;

CommaOrSemicolonOptional: ',' | ';' |
    ;
/*
Enum: tok_enum tok_identifier '{' EnumDefList '}' TypeAttributes  { $$ = CreateEnum($2, $4, $6); }
    ;

EnumDefList: EnumDefList EnumDef    {  $1.push_back($2); $$ = std::move($1);  }
    |
    ;

EnumDef: CaptureDocText tok_identifier '=' tok_int_constant TypeAttributes CommaOrSemicolonOptional    { $$ = EnumDef::Create($2, $4, $5); }
    | CaptureDocText tok_identifier TypeAttributes CommaOrSemicolonOptional                            { $$ = EnumDef::Create($2, $3); }
    ;
    */
/*
Const: tok_const FieldType tok_identifier '=' ConstValue CommaOrSemicolonOptional { $$ = Const::Create($2, $3, $5); }
    ;
    */
ConstValue: tok_int_constant    { $$ = CreateConstValue(std::move($1)); }
    | tok_dub_constant          { $$ = CreateConstValue(std::move($1)); }
    | tok_literal               { $$ = CreateConstValue(std::move($1)); }
    | tok_identifier            { $$ = FindConstValue(std::move($1));   }
    | '[' ConstListContents ']' { $$ = CreateConstValue(std::move($2)); }
    | '{' ConstMapContents '}'  { $$ = CreateConstValue(std::move($2)); }
    ;

ConstListContents: ConstListContents ConstValue CommaOrSemicolonOptional    {  $$ = ConstValueAddValue(std::move($1), std::move($2)); }
    | {}
;

ConstMapContents: ConstMapContents ConstValue ':' ConstValue CommaOrSemicolonOptional    { $$ = ConstValueAddValue(std::move($1), std::move($2), std::move($4)); }
    | {}
;

Struct: tok_struct tok_identifier  '{' FieldList '}' TypeAttributes    { $$ = CreateStruct(context, $2, $4, $6); }
;

Relationship : tok_relationship tok_identifier RelationshipComponents CommaOrSemicolonOptional { CreateRelationship(context, $2, $3); }
;

RelationshipComponents: RelationshipComponents ':' RelationshipComponent { $1[std::move($3.first)] = std::move($3.second); $$ = std::move($1); }
| RelationshipComponent { $$ = {}; $$[std::move($1.first)] = std::move($1.second); }
;

RelationshipComponent : tok_identifier '=' identifierlist { $$ = std::make_pair(std::move($1), std::move($3)); }
;

identifierlist : identifierlist ',' tok_identifier { $1.push_back($3); $$ = std::move($1); }
| tok_identifier { $$.push_back($1); }
;

Union:    tok_union  tok_identifier  '{' FieldList '}' TypeAttributes    { $$ = CreateUnion(context, $2, $4, $6); }
    ;
    /*
Xception: tok_xception tok_identifier '{' FieldList '}' TypeAttributes { $$ = CreateXception($2, $4, $6); }
    ;
    */
Interface: tok_interface tok_identifier Extends '{' FlagArgs InterfaceMemberList UnflagArgs '}' TypeAttributes { $$ = CreateInterface(context, $2, $3, $6, $9); }
    ;

Extends: tok_extends tok_identifier { $$ = FindInterface(context, $2); }
    | {}
    ;

InterfaceMemberList: InterfaceMemberList InterfaceMember        { $1.push_back($2); $$ = std::move($1); }
    | {}
    ;

InterfaceMember: InterfaceFunction { $$ = std::move($1); }
    | InterfaceEvent { $$ = std::move($1); }
    | InterfaceObjectStore { $$ = std::move($1); }
    ;

InterfaceEvent: CaptureDocText tok_event tok_identifier '(' FieldList ')' TypeAttributes
        CommaOrSemicolonOptional { $$ = InterfaceEvent($3, $5, $7); }
    ;
InterfaceObjectStore: CaptureDocText tok_objectstore FieldType tok_identifier TypeAttributes
        CommaOrSemicolonOptional { $$ = InterfaceObjectStore($3, $4, $5); }
    ;

InterfaceFunction: CaptureDocText Static FunctionType tok_identifier '(' FieldList ')' Throws TypeAttributes
                CommaOrSemicolonOptional { $$ = InterfaceFunction($2, $3, $4, $6, $9); }
    ;

Static: tok_static { $$ = std::move($1); }
    | {}
    ;

Throws: tok_throws '(' FieldList ')' { $$ = $3; }
    | { }
    ;

FieldList: FieldList Field    { $1.push_back($2); $$ = std::move($1); }
    | {}
    ;

Field: CaptureDocText FieldIdentifier FieldRequiredness FieldType tok_identifier
                FieldValue TypeAttributes CommaOrSemicolonOptional                        { $$ = Field($2, $3, $4, $5, $6, $7); }
    ;

FieldIdentifier: tok_int_constant ':'
    |                                    { $$ = -1; }
    ;

FieldRequiredness: tok_required
    | tok_optional
    | {}
    ;

FieldValue: '=' ConstValue        { $$ = std::move($2); }
    | {}
    ;

FunctionType: FieldType             { $$ = $1; }
    | tok_void                      { auto voidstr = Str::Create(L"void"); $$ = FindFieldType(context, voidstr); }
    ;

FieldType: tok_identifier { $$ = FindFieldType(context, $1); }
    | ContainerType
    | ArrayType
    ;

ArrayType: FieldType '[' tok_int_constant ']' TypeAttributes { $$ = CreateArrayType(context, $1, $3, $5); }
    ;

ContainerType: tok_identifier '<' ContainerFieldList '>' TypeAttributes { $$ = CreateContainerType(context, $1, $3, $5); }
    ;

ContainerFieldList: ContainerFieldList ',' FieldType    { $1.push_back($3); $$ = std::move($1); }
    | FieldType                                            { $$.push_back($1); }
    ;

TypeAttributes: '(' TypeAttributeList ')' { $$ = std::move($2); }
    | { $$ = NULL; }
    ;

TypeAttributeList: TypeAttributeList TypeAttribute    { $$ = CreateAttributeMapEntry($1, $2); }
    |                                                    { $$ = nullptr; }
    ;

TypeAttribute: tok_identifier '=' tok_literal CommaOrSemicolonOptional { $$ = std::make_pair($1, $3); }
    ;

CaptureDocText:;
FlagArgs: ;
UnflagArgs: ;
%%
LEXYACC:YACC:END
