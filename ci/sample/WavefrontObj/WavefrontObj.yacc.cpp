// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   WavefrontObjlex



#include "WavefrontObj.yacc.h"


// Unqualified %code blocks.
#line 20 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"

// Included in CPP
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define yyFlexLexer WavefrontObjFlexLexer
#undef yylex

#pragma warning(push, 3)
#include <FlexLexer.h>
#pragma warning(pop)

#include "WavefrontObj.ly.impl.h"  // Generated us
#define yylex scanner.yylex


#line 67 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 4 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
namespace ModelFormat { namespace WavefrontObj { namespace impl {
#line 160 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"

  /// Build a parser object.
  BisonParser::BisonParser (ModelFormat::WavefrontObj::impl::Scanner &scanner_yyarg, ModelFormat::WavefrontObj::Context &context_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      context (context_yyarg)
  {}

  BisonParser::~BisonParser ()
  {}

  BisonParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  BisonParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.copy< FaceElem > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.copy< FaceElems > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.copy< Id > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.copy< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.copy< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  BisonParser::symbol_kind_type
  BisonParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  BisonParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  BisonParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.move< FaceElem > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.move< FaceElems > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.move< Id > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.move< double > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.move< int > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  BisonParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  BisonParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  BisonParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  BisonParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  BisonParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  BisonParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  BisonParser::symbol_kind_type
  BisonParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  BisonParser::symbol_kind_type
  BisonParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  BisonParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  BisonParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  BisonParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  BisonParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  BisonParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  BisonParser::symbol_kind_type
  BisonParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  BisonParser::stack_symbol_type::stack_symbol_type ()
  {}

  BisonParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.YY_MOVE_OR_COPY< FaceElem > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.YY_MOVE_OR_COPY< FaceElems > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.YY_MOVE_OR_COPY< Id > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  BisonParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.move< FaceElem > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.move< FaceElems > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.move< Id > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.move< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  BisonParser::stack_symbol_type&
  BisonParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.copy< FaceElem > (that.value);
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.copy< FaceElems > (that.value);
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.copy< Id > (that.value);
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.copy< int > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  BisonParser::stack_symbol_type&
  BisonParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FaceElem: // FaceElem
        value.move< FaceElem > (that.value);
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        value.move< FaceElems > (that.value);
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        value.move< Id > (that.value);
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        value.move< double > (that.value);
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        value.move< int > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  BisonParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  BisonParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  BisonParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  BisonParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  BisonParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  BisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BisonParser::debug_level_type
  BisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  BisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  BisonParser::state_type
  BisonParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  BisonParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  BisonParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  BisonParser::operator() ()
  {
    return parse ();
  }

  int
  BisonParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_FaceElem: // FaceElem
        yylhs.value.emplace< FaceElem > ();
        break;

      case symbol_kind::S_FaceElems: // FaceElems
        yylhs.value.emplace< FaceElems > ();
        break;

      case symbol_kind::S_tok_identifier: // tok_identifier
        yylhs.value.emplace< Id > ();
        break;

      case symbol_kind::S_tok_dub_constant: // tok_dub_constant
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_tok_int_constant: // tok_int_constant
        yylhs.value.emplace< int > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // File: Lines
#line 68 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
            {}
#line 790 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 5: // Line: tok_vertex tok_dub_constant tok_dub_constant tok_dub_constant
#line 76 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.CreateVertex(yystack_[2].value.as < double > (), yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 796 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 6: // Line: tok_texel tok_dub_constant tok_dub_constant
#line 77 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.CreateTexel(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 802 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 7: // Line: tok_face FaceElems
#line 78 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.CreateFace(yystack_[0].value.as < FaceElems > ()); }
#line 808 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 8: // Line: tok_normal tok_dub_constant tok_dub_constant tok_dub_constant
#line 79 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.CreateNormal(yystack_[2].value.as < double > (), yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 814 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 9: // Line: tok_object tok_identifier
#line 80 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.SetCurrentObjectName(yystack_[0].value.as < Id > ()); }
#line 820 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 10: // Line: tok_group tok_identifier
#line 81 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.SetCurrentGroupName(yystack_[0].value.as < Id > ()); }
#line 826 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 11: // Line: tok_material tok_identifier
#line 82 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.SetMaterialLibFile(yystack_[0].value.as < Id > ()); }
#line 832 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 12: // Line: tok_usematerial tok_identifier
#line 83 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                        { context.SetMaterialName(yystack_[0].value.as < Id > ()); }
#line 838 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 13: // Line: tok_smooth tok_int_constant
#line 84 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                                       {}
#line 844 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 14: // Line: tok_smooth tok_identifier
#line 85 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                {}
#line 850 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 16: // FaceElems: FaceElems FaceElem
#line 90 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                { yylhs.value.as < FaceElems > () = ListAdd(yystack_[1].value.as < FaceElems > (), yystack_[0].value.as < FaceElem > ()); }
#line 856 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 17: // FaceElems: FaceElem
#line 91 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                { yylhs.value.as < FaceElems > () = ListCreate(yystack_[0].value.as < FaceElem > ()); }
#line 862 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 18: // FaceElem: tok_int_constant
#line 94 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                    { yylhs.value.as < FaceElem > () = CreateFaceElem(yystack_[0].value.as < int > (), InvalidTexel, InvalidNormal); }
#line 868 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 19: // FaceElem: tok_int_constant '/' tok_int_constant
#line 95 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                    { yylhs.value.as < FaceElem > () = CreateFaceElem(yystack_[2].value.as < int > (), yystack_[0].value.as < int > (), InvalidNormal);           }
#line 874 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;

  case 20: // FaceElem: tok_int_constant '/' tok_int_constant '/' tok_int_constant
#line 96 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
                                                                    { yylhs.value.as < FaceElem > () = CreateFaceElem(yystack_[4].value.as < int > (), yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());                      }
#line 880 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"
    break;


#line 884 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  BisonParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  BisonParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char BisonParser::yypact_ninf_ = -5;

  const signed char BisonParser::yytable_ninf_ = -16;

  const signed char
  BisonParser::yypact_[] =
  {
      10,    -1,    11,    12,     9,    15,    17,    18,    19,    -4,
      29,    -3,    16,    24,    25,    26,    20,     9,    -5,    -5,
      -5,    -5,    -5,    -5,    -5,    -5,    21,    -5,    27,    -5,
      30,    31,    -5,    -5,    -5,    -5,    22,    32,    -5
  };

  const signed char
  BisonParser::yydefact_[] =
  {
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     0,     0,     0,     0,    18,     7,    17,    11,
      12,     9,    10,    13,    14,     1,     0,     4,     0,     6,
       0,     0,    16,     3,     5,     8,    19,     0,    20
  };

  const signed char
  BisonParser::yypgoto_[] =
  {
      -5,    -5,    -5,    33,    -5,    28
  };

  const signed char
  BisonParser::yydefgoto_[] =
  {
       0,    10,    11,    12,    17,    18
  };

  const signed char
  BisonParser::yytable_[] =
  {
       1,     2,     3,     4,    23,    24,    13,     5,     6,     7,
       8,   -15,     9,     1,     2,     3,     4,    16,    14,    15,
       5,     6,     7,     8,    19,     9,    20,    21,    22,    25,
      27,    28,    29,    30,    34,    33,    31,    35,    37,    36,
      38,     0,     0,     0,    26,    32
  };

  const signed char
  BisonParser::yycheck_[] =
  {
       3,     4,     5,     6,     8,     9,     7,    10,    11,    12,
      13,    14,    15,     3,     4,     5,     6,     8,     7,     7,
      10,    11,    12,    13,     9,    15,     9,     9,     9,     0,
      14,     7,     7,     7,     7,    14,    16,     7,    16,     8,
       8,    -1,    -1,    -1,    11,    17
  };

  const signed char
  BisonParser::yystos_[] =
  {
       0,     3,     4,     5,     6,    10,    11,    12,    13,    15,
      18,    19,    20,     7,     7,     7,     8,    21,    22,     9,
       9,     9,     9,     8,     9,     0,    20,    14,     7,     7,
       7,    16,    22,    14,     7,     7,     8,    16,     8
  };

  const signed char
  BisonParser::yyr1_[] =
  {
       0,    17,    18,    19,    19,    20,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    20,    21,    21,    22,    22,
      22
  };

  const signed char
  BisonParser::yyr2_[] =
  {
       0,     2,     1,     3,     2,     4,     3,     2,     4,     2,
       2,     2,     2,     2,     2,     0,     2,     1,     1,     3,
       5
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const BisonParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "tok_vertex",
  "tok_texel", "tok_normal", "tok_face", "tok_dub_constant",
  "tok_int_constant", "tok_identifier", "tok_material", "tok_usematerial",
  "tok_object", "tok_group", "tok_newline", "tok_smooth", "'/'", "$accept",
  "File", "Lines", "Line", "FaceElems", "FaceElem", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  BisonParser::yyrline_[] =
  {
       0,    68,    68,    71,    72,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    87,    90,    91,    94,    95,
      96
  };

  void
  BisonParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  BisonParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  BisonParser::symbol_kind_type
  BisonParser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    16,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
    };
    // Last valid token kind.
    const int code_max = 270;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 4 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"
} } } // ModelFormat::WavefrontObj::impl
#line 1259 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.yacc.cpp"

#line 98 "/home/ankurv/build_with_vcpkg/x64-linux_Test_Debug/WavefrontObj_lexyacc_WavefrontObj/WavefrontObj.y"




void ModelFormat::WavefrontObj::impl::BisonParser::error(const location_type&l, const std::string& message)
{
    std::cerr << "Error(WavefrontObj): " << message << " at " << l << "\n";
}

namespace ModelFormat::WavefrontObj
{
void Load(Context& ctx, std::istream& strm)
{
    impl::Scanner scanner(&strm);
    impl::BisonParser p(scanner, ctx);
    p.set_debug_level(ctx.Debug());
    try
    {
        auto rslt= p.parse();
        if (rslt != 0) throw std::logic_error("Unexpected Error : " + std::to_string(rslt));
    }
    catch(std::exception const& ex)
    {
        ctx.NotifyError(scanner.lineno(), -1, ex.what());
        throw;
    }
}
}
