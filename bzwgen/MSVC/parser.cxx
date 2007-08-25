/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TEST = 258,
     ROUND = 259,
     NEG = 260,
     ASSERTION = 261,
     FACE = 262,
     TAPER = 263,
     SPAWN = 264,
     CHAMFER = 265,
     TEXTURE = 266,
     TEXTUREFULL = 267,
     TEXTUREQUAD = 268,
     TEXTURECLEAR = 269,
     SPAWNNGON = 270,
     UNCHAMFER = 271,
     ASSIGN = 272,
     DEFSIGN = 273,
     EXTRUDE = 274,
     EXTRUDET = 275,
     EXPAND = 276,
     RANDOM = 277,
     MATERIAL = 278,
     SUBDIVIDEH = 279,
     SUBDIVIDEV = 280,
     PARTITIONH = 281,
     PARTITIONV = 282,
     PARTITIONHI = 283,
     PARTITIONVI = 284,
     MULTIFACE = 285,
     NUMBER = 286,
     NONTERM = 287,
     ATTRIBUTE = 288
   };
#endif
/* Tokens.  */
#define TEST 258
#define ROUND 259
#define NEG 260
#define ASSERTION 261
#define FACE 262
#define TAPER 263
#define SPAWN 264
#define CHAMFER 265
#define TEXTURE 266
#define TEXTUREFULL 267
#define TEXTUREQUAD 268
#define TEXTURECLEAR 269
#define SPAWNNGON 270
#define UNCHAMFER 271
#define ASSIGN 272
#define DEFSIGN 273
#define EXTRUDE 274
#define EXTRUDET 275
#define EXPAND 276
#define RANDOM 277
#define MATERIAL 278
#define SUBDIVIDEH 279
#define SUBDIVIDEV 280
#define PARTITIONH 281
#define PARTITIONV 282
#define PARTITIONHI 283
#define PARTITIONVI 284
#define MULTIFACE 285
#define NUMBER 286
#define NONTERM 287
#define ATTRIBUTE 288




/* Copy the first part of user declarations.  */
#line 1 "..\\..\\src\\parser.y"

#include <memory>
#include <string>
#include "globals.h"
#include "Rule.h"
#include "RuleSet.h"
#include "Product.h"
#include "Operation.h"
#include "Expression.h"

void yyerror(RuleSet *ruleset, char* s);
int yylex();
void yyunput(int, char*);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 16 "..\\..\\src\\parser.y"
typedef union _YYSTYPE {
  char *id;
  float fl;
  ProductVector* pv;
  Product* p;
  OperationVector* ov;
  StringVector* ids;
  Operation* o;
  Expression* e;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 177 "..\\parser.cxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 189 "..\\parser.cxx"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   539

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  11
/* YYNRULES -- Number of rules. */
#define YYNRULES  65
/* YYNRULES -- Number of states. */
#define YYNSTATES  202

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,     2,
      44,    45,    41,    40,    50,    39,     2,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    43,
      36,    38,    37,     2,    48,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    35,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     9,    10,    14,    15,    18,    24,
      28,    29,    32,    35,    36,    41,    45,    46,    51,    55,
      56,    59,    65,    71,    76,    81,    86,    91,    95,    99,
     103,   107,   118,   124,   130,   138,   146,   152,   158,   164,
     170,   178,   186,   194,   202,   207,   211,   218,   222,   227,
     234,   236,   245,   249,   254,   259,   263,   267,   271,   275,
     279,   283,   287,   291,   296,   298
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      52,     0,    -1,    -1,    52,    32,    54,    43,    -1,    -1,
      44,    61,    45,    -1,    -1,    54,    55,    -1,    18,    31,
      46,    53,    59,    -1,    18,    53,    59,    -1,    -1,    56,
      32,    -1,    56,    41,    -1,    -1,    47,    48,    32,    49,
      -1,    47,    56,    49,    -1,    -1,    47,    48,    32,    49,
      -1,    47,    32,    49,    -1,    -1,    59,    60,    -1,    19,
      44,    61,    45,    57,    -1,    20,    44,    61,    45,    57,
      -1,    21,    44,    61,    45,    -1,     8,    44,    61,    45,
      -1,    10,    44,    61,    45,    -1,     6,    44,    61,    45,
      -1,    16,    44,    45,    -1,    11,    44,    45,    -1,    12,
      44,    45,    -1,    14,    44,    45,    -1,    13,    44,    61,
      50,    61,    50,    61,    50,    61,    45,    -1,    24,    44,
      61,    45,    57,    -1,    25,    44,    61,    45,    57,    -1,
      24,    44,    61,    50,    61,    45,    57,    -1,    25,    44,
      61,    50,    61,    45,    57,    -1,    26,    44,    61,    45,
      58,    -1,    27,    44,    61,    45,    58,    -1,    28,    44,
      61,    45,    58,    -1,    29,    44,    61,    45,    58,    -1,
      26,    44,    61,    50,    61,    45,    58,    -1,    27,    44,
      61,    50,    61,    45,    58,    -1,    28,    44,    61,    50,
      61,    45,    58,    -1,    29,    44,    61,    50,    61,    45,
      58,    -1,    23,    44,    61,    45,    -1,    30,    44,    45,
      -1,    17,    44,    32,    38,    61,    45,    -1,     3,    44,
      45,    -1,     9,    44,    32,    45,    -1,    15,    44,    32,
      50,    61,    45,    -1,    32,    -1,    22,    44,    61,    50,
      61,    50,    61,    45,    -1,    44,    61,    45,    -1,     5,
      44,    61,    45,    -1,     4,    44,    61,    45,    -1,    61,
      39,    61,    -1,    61,    41,    61,    -1,    61,    42,    61,
      -1,    61,    37,    61,    -1,    61,    36,    61,    -1,    61,
      34,    61,    -1,    61,    38,    61,    -1,    61,    35,    61,
      -1,     7,    44,    32,    45,    -1,    31,    -1,    33,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    44,    44,    45,    50,    51,    53,    54,    56,    57,
      59,    60,    61,    63,    64,    65,    67,    68,    69,    71,
      72,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TEST", "ROUND", "NEG", "ASSERTION",
  "FACE", "TAPER", "SPAWN", "CHAMFER", "TEXTURE", "TEXTUREFULL",
  "TEXTUREQUAD", "TEXTURECLEAR", "SPAWNNGON", "UNCHAMFER", "ASSIGN",
  "DEFSIGN", "EXTRUDE", "EXTRUDET", "EXPAND", "RANDOM", "MATERIAL",
  "SUBDIVIDEH", "SUBDIVIDEV", "PARTITIONH", "PARTITIONV", "PARTITIONHI",
  "PARTITIONVI", "MULTIFACE", "NUMBER", "NONTERM", "ATTRIBUTE", "'&'",
  "'|'", "'<'", "'>'", "'='", "'-'", "'+'", "'*'", "'/'", "';'", "'('",
  "')'", "':'", "'['", "'@'", "']'", "','", "$accept", "ruleset", "cond",
  "products", "product", "faces", "faceparam", "singleface", "ops", "op",
  "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    38,   124,    60,    62,    61,    45,
      43,    42,    47,    59,    40,    41,    58,    91,    64,    93,
      44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    51,    52,    52,    53,    53,    54,    54,    55,    55,
      56,    56,    56,    57,    57,    57,    58,    58,    58,    59,
      59,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     4,     0,     3,     0,     2,     5,     3,
       0,     2,     2,     0,     4,     3,     0,     4,     3,     0,
       2,     5,     5,     4,     4,     4,     4,     3,     3,     3,
       3,    10,     5,     5,     7,     7,     5,     5,     5,     5,
       7,     7,     7,     7,     4,     3,     6,     3,     4,     6,
       1,     8,     3,     4,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,     6,     0,     4,     3,     7,     0,     0,
      19,     4,     0,     0,     0,     0,    64,    65,     0,     0,
       9,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      20,     8,     0,     0,     0,     0,    52,    60,    62,    59,
      58,    61,    55,    56,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    53,
      63,     0,    47,     0,     0,     0,     0,    28,    29,     0,
      30,     0,    27,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,    26,    24,    48,    25,
       0,     0,     0,    13,    13,    23,    44,    13,     0,    13,
       0,    16,     0,    16,     0,    16,     0,    16,     0,     0,
       0,     0,     0,    10,    21,    22,    32,     0,    33,     0,
       0,    36,     0,    37,     0,    38,     0,    39,     0,     0,
       0,    49,    46,     0,     0,    13,    13,     0,     0,    16,
      16,    16,    16,    51,     0,     0,    11,    12,    15,    34,
      35,    18,     0,    40,    41,    42,    43,     0,    14,    17,
       0,    31
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    10,     4,     7,   174,   154,   161,    20,    60,
      19
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -132
static const short int yypact[] =
{
    -132,     2,  -132,  -132,   -17,   -12,  -132,  -132,   -29,    16,
    -132,   -22,   -19,   -16,    -3,    -2,  -132,  -132,    16,   254,
      76,  -132,    16,    16,     7,    16,   266,    16,    16,    16,
      16,    16,    16,    16,    16,  -132,    -1,     4,    10,    13,
      18,    19,    36,    37,    50,    54,    63,    65,    66,    67,
      71,    72,    73,    74,    75,    77,    79,    81,    83,  -132,
    -132,    76,   278,   290,   -15,   204,  -132,    14,    14,   108,
     108,   108,    -5,  -132,  -132,     1,    16,    16,    97,    16,
      93,    96,    16,   106,   110,   114,   130,    16,    16,    16,
      16,    16,    16,    16,    16,    16,    16,   118,  -132,  -132,
    -132,    16,  -132,   302,   314,   120,   326,  -132,  -132,   214,
    -132,   116,  -132,   129,   338,   350,   362,   374,    98,   119,
     136,   153,   170,   187,  -132,   224,  -132,  -132,  -132,  -132,
      16,    16,    16,   121,   121,  -132,  -132,   121,    16,   121,
      16,   133,    16,   133,    16,   133,    16,   133,    16,    16,
     234,   386,   398,   128,  -132,  -132,  -132,   410,  -132,   422,
      -8,  -132,   434,  -132,   446,  -132,   458,  -132,   470,   482,
      16,  -132,  -132,   150,   -14,   121,   121,   134,   152,   133,
     133,   133,   133,  -132,   244,   144,  -132,  -132,  -132,  -132,
    -132,  -132,   147,  -132,  -132,  -132,  -132,    16,  -132,  -132,
     494,  -132
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -132,  -132,   174,  -132,  -132,  -132,  -131,  -114,   176,  -132,
     -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short int yytable[] =
{
      26,     5,     2,   155,    62,    63,   156,    65,   158,    67,
      68,    69,    70,    71,    72,    73,    74,    11,   186,     8,
      12,    13,     9,    14,   177,    22,     6,   187,    23,   163,
     100,   165,     9,   167,     3,   188,    33,    34,    15,    64,
     178,    24,    25,    75,   189,   190,   102,    16,    76,    17,
      29,    30,    31,    32,    77,    33,    34,    78,   103,   104,
      18,   106,    79,    80,   109,   193,   194,   195,   196,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    36,
      81,    82,    37,   125,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    83,    48,    49,    50,    84,    51,
      52,    53,    54,    55,    56,    57,    58,    85,    59,    86,
      87,    88,   150,   151,   152,    89,    90,    91,    92,    93,
     157,    94,   159,    95,   162,    96,   164,    97,   166,   105,
     168,   169,    27,    28,    29,    30,    31,    32,   107,    33,
      34,   108,   111,   137,    -1,    -1,    -1,    32,   138,    33,
      34,   110,   184,    27,    28,    29,    30,    31,    32,   112,
      33,    34,   113,   124,   139,   128,   131,   132,   153,   140,
      27,    28,    29,    30,    31,    32,   173,    33,    34,   200,
     160,   141,   185,   191,   192,    21,   142,    27,    28,    29,
      30,    31,    32,   198,    33,    34,   199,    61,   143,     0,
       0,     0,     0,   144,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   145,     0,     0,     0,     0,
     146,    27,    28,    29,    30,    31,    32,     0,    33,    34,
       0,     0,   147,     0,     0,     0,     0,   148,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   101,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   130,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   149,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   170,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   197,    33,    34,     0,     0,    35,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,    66,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,    98,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,    99,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   126,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   127,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   129,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   133,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   134,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   135,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   136,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   171,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   172,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   175,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   176,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   179,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   180,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   181,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   182,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   183,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   201
};

static const short int yycheck[] =
{
      18,    18,     0,   134,    22,    23,   137,    25,   139,    27,
      28,    29,    30,    31,    32,    33,    34,    46,    32,    31,
       4,     5,    44,     7,    32,    44,    43,    41,    44,   143,
      45,   145,    44,   147,    32,    49,    41,    42,    22,    32,
      48,    44,    44,    44,   175,   176,    45,    31,    44,    33,
      36,    37,    38,    39,    44,    41,    42,    44,    76,    77,
      44,    79,    44,    44,    82,   179,   180,   181,   182,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,     3,
      44,    44,     6,   101,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    44,    19,    20,    21,    44,    23,
      24,    25,    26,    27,    28,    29,    30,    44,    32,    44,
      44,    44,   130,   131,   132,    44,    44,    44,    44,    44,
     138,    44,   140,    44,   142,    44,   144,    44,   146,    32,
     148,   149,    34,    35,    36,    37,    38,    39,    45,    41,
      42,    45,    32,    45,    36,    37,    38,    39,    50,    41,
      42,    45,   170,    34,    35,    36,    37,    38,    39,    45,
      41,    42,    32,    45,    45,    45,    50,    38,    47,    50,
      34,    35,    36,    37,    38,    39,    48,    41,    42,   197,
      47,    45,    32,    49,    32,    11,    50,    34,    35,    36,
      37,    38,    39,    49,    41,    42,    49,    21,    45,    -1,
      -1,    -1,    -1,    50,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    -1,    45,    -1,    -1,    -1,    -1,
      50,    34,    35,    36,    37,    38,    39,    -1,    41,    42,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    50,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    34,    35,
      36,    37,    38,    39,    50,    41,    42,    -1,    34,    35,
      36,    37,    38,    39,    50,    41,    42,    -1,    34,    35,
      36,    37,    38,    39,    50,    41,    42,    -1,    34,    35,
      36,    37,    38,    39,    50,    41,    42,    -1,    34,    35,
      36,    37,    38,    39,    50,    41,    42,    -1,    -1,    45,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      -1,    45,    34,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    -1,    45,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    -1,    45,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    -1,    45,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    -1,    45,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      -1,    45,    34,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    -1,    45,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    -1,    45,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    -1,    45,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    -1,    45,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      -1,    45,    34,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    -1,    45,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    -1,    45,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    -1,    45,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    -1,    45,
      34,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      -1,    45,    34,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    -1,    45,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    -1,    45,    34,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    -1,    45,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    52,     0,    32,    54,    18,    43,    55,    31,    44,
      53,    46,     4,     5,     7,    22,    31,    33,    44,    61,
      59,    53,    44,    44,    44,    44,    61,    34,    35,    36,
      37,    38,    39,    41,    42,    45,     3,     6,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    19,    20,
      21,    23,    24,    25,    26,    27,    28,    29,    30,    32,
      60,    59,    61,    61,    32,    61,    45,    61,    61,    61,
      61,    61,    61,    61,    61,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    45,    45,
      45,    50,    45,    61,    61,    32,    61,    45,    45,    61,
      45,    32,    45,    32,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    45,    61,    45,    45,    45,    45,
      50,    50,    38,    45,    45,    45,    45,    45,    50,    45,
      50,    45,    50,    45,    50,    45,    50,    45,    50,    50,
      61,    61,    61,    47,    57,    57,    57,    61,    57,    61,
      47,    58,    61,    58,    61,    58,    61,    58,    61,    61,
      50,    45,    45,    48,    56,    45,    45,    32,    48,    45,
      45,    45,    45,    45,    61,    32,    32,    41,    49,    57,
      57,    49,    32,    58,    58,    58,    58,    50,    49,    49,
      61,    45
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (ruleset, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (RuleSet *ruleset);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (RuleSet *ruleset)
#else
int
yyparse (ruleset)
    RuleSet *ruleset;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 45 "..\\..\\src\\parser.y"
    { 
    std::string name = std::string((yyvsp[-2].id));
    ruleset->addRule(name,new Rule(name,(yyvsp[-1].pv)));
  ;}
    break;

  case 4:
#line 50 "..\\..\\src\\parser.y"
    { (yyval.e) = NULL; ;}
    break;

  case 5:
#line 51 "..\\..\\src\\parser.y"
    { (yyval.e) = (yyvsp[-1].e); ;}
    break;

  case 6:
#line 53 "..\\..\\src\\parser.y"
    { (yyval.pv) = new ProductVector(); ;}
    break;

  case 7:
#line 54 "..\\..\\src\\parser.y"
    { (yyval.pv) = (yyvsp[-1].pv); (yyval.pv)->push_back((yyvsp[0].p)); ;}
    break;

  case 8:
#line 56 "..\\..\\src\\parser.y"
    { (yyval.p) = new Product((yyvsp[0].ov),(yyvsp[-3].fl),(yyvsp[-1].e)); ;}
    break;

  case 9:
#line 57 "..\\..\\src\\parser.y"
    { (yyval.p) = new Product((yyvsp[0].ov),1.0f,(yyvsp[-1].e)); ;}
    break;

  case 10:
#line 59 "..\\..\\src\\parser.y"
    { (yyval.ids) = new StringVector(); ;}
    break;

  case 11:
#line 60 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[0].id)); (yyval.ids)->push_back(name); ;}
    break;

  case 12:
#line 61 "..\\..\\src\\parser.y"
    { std::string name = std::string(""); (yyval.ids)->push_back(name); ;}
    break;

  case 13:
#line 63 "..\\..\\src\\parser.y"
    { (yyval.ids) = NULL; ;}
    break;

  case 14:
#line 64 "..\\..\\src\\parser.y"
    { std::string name = '@'+std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 15:
#line 65 "..\\..\\src\\parser.y"
    { (yyval.ids) = (yyvsp[-1].ids); ;}
    break;

  case 16:
#line 67 "..\\..\\src\\parser.y"
    { (yyval.ids) = NULL; ;}
    break;

  case 17:
#line 68 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 18:
#line 69 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 19:
#line 71 "..\\..\\src\\parser.y"
    { (yyval.ov) = new OperationVector(); ;}
    break;

  case 20:
#line 72 "..\\..\\src\\parser.y"
    { (yyval.ov) = (yyvsp[-1].ov); (yyval.ov)->push_back((yyvsp[0].o)); ;}
    break;

  case 21:
#line 74 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExtrude(ruleset,(yyvsp[-2].e),(yyvsp[0].ids)); ;}
    break;

  case 22:
#line 75 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExtrudeT(ruleset,(yyvsp[-2].e),(yyvsp[0].ids)); ;}
    break;

  case 23:
#line 76 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExpand(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 24:
#line 77 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTaper(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 25:
#line 78 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationChamfer(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 26:
#line 79 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationAssert(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 27:
#line 80 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationUnchamfer(ruleset); ;}
    break;

  case 28:
#line 81 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTexture(ruleset); ;}
    break;

  case 29:
#line 82 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureFull(ruleset); ;}
    break;

  case 30:
#line 83 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureClear(ruleset); ;}
    break;

  case 31:
#line 84 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureQuad(ruleset,(yyvsp[-7].e),(yyvsp[-5].e),(yyvsp[-3].e),(yyvsp[-1].e)); ;}
    break;

  case 32:
#line 85 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids)); ;}
    break;

  case 33:
#line 86 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids)); ;}
    break;

  case 34:
#line 87 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),(yyvsp[-2].e)); ;}
    break;

  case 35:
#line 88 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),(yyvsp[-2].e)); ;}
    break;

  case 36:
#line 89 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids)); ;}
    break;

  case 37:
#line 90 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids)); ;}
    break;

  case 38:
#line 91 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids),true); ;}
    break;

  case 39:
#line 92 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids),true); ;}
    break;

  case 40:
#line 93 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),false,(yyvsp[-2].e)); ;}
    break;

  case 41:
#line 94 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),false,(yyvsp[-2].e)); ;}
    break;

  case 42:
#line 95 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),true,(yyvsp[-2].e)); ;}
    break;

  case 43:
#line 96 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),true,(yyvsp[-2].e)); ;}
    break;

  case 44:
#line 97 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationMaterial(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 45:
#line 98 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationMultiFace(ruleset); ;}
    break;

  case 46:
#line 99 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationAssign(ruleset,(yyvsp[-1].e),(yyvsp[-3].id)); ;}
    break;

  case 47:
#line 100 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTest(ruleset); ;}
    break;

  case 48:
#line 101 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSpawn(ruleset,(yyvsp[-1].id)); ;}
    break;

  case 49:
#line 102 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSpawnNGon(ruleset,(yyvsp[-3].id),(yyvsp[-1].e)); ;}
    break;

  case 50:
#line 103 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationNonterminal(ruleset,(yyvsp[0].id)); ;}
    break;

  case 51:
#line 105 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionRandom((yyvsp[-5].e),(yyvsp[-3].e),(yyvsp[-1].e)); ;}
    break;

  case 52:
#line 106 "..\\..\\src\\parser.y"
    { (yyval.e) = (yyvsp[-1].e); ;}
    break;

  case 53:
#line 107 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionNeg((yyvsp[-1].e)); ;}
    break;

  case 54:
#line 108 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionRound((yyvsp[-1].e)); ;}
    break;

  case 55:
#line 109 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionSub((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 56:
#line 110 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionMult((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 57:
#line 111 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionDiv((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 58:
#line 112 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionGreater((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 59:
#line 113 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionGreater((yyvsp[0].e),(yyvsp[-2].e)); ;}
    break;

  case 60:
#line 114 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionAnd((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 61:
#line 115 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionEqual((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 62:
#line 116 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionOr((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 63:
#line 117 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionFaceAttribute((yyvsp[-1].id)); ;}
    break;

  case 64:
#line 118 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionConst((yyvsp[0].fl)); ;}
    break;

  case 65:
#line 119 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionAttribute(ruleset,(yyvsp[0].id)); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1717 "..\\parser.cxx"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (ruleset, yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (ruleset, YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (ruleset, YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (ruleset, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 121 "..\\..\\src\\parser.y"


