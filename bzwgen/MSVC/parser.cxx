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
     ROUND = 258,
     NEG = 259,
     ASSERTION = 260,
     FACE = 261,
     TAPER = 262,
     SPAWN = 263,
     CHAMFER = 264,
     TEXTURE = 265,
     TEXTUREFULL = 266,
     TEXTUREQUAD = 267,
     TEXTURECLEAR = 268,
     UNCHAMFER = 269,
     ASSIGN = 270,
     DEFSIGN = 271,
     EXTRUDE = 272,
     EXTRUDET = 273,
     EXPAND = 274,
     RANDOM = 275,
     MATERIAL = 276,
     SUBDIVIDEH = 277,
     SUBDIVIDEV = 278,
     PARTITIONH = 279,
     PARTITIONV = 280,
     PARTITIONHI = 281,
     PARTITIONVI = 282,
     NUMBER = 283,
     NONTERM = 284,
     ATTRIBUTE = 285
   };
#endif
/* Tokens.  */
#define ROUND 258
#define NEG 259
#define ASSERTION 260
#define FACE 261
#define TAPER 262
#define SPAWN 263
#define CHAMFER 264
#define TEXTURE 265
#define TEXTUREFULL 266
#define TEXTUREQUAD 267
#define TEXTURECLEAR 268
#define UNCHAMFER 269
#define ASSIGN 270
#define DEFSIGN 271
#define EXTRUDE 272
#define EXTRUDET 273
#define EXPAND 274
#define RANDOM 275
#define MATERIAL 276
#define SUBDIVIDEH 277
#define SUBDIVIDEV 278
#define PARTITIONH 279
#define PARTITIONV 280
#define PARTITIONHI 281
#define PARTITIONVI 282
#define NUMBER 283
#define NONTERM 284
#define ATTRIBUTE 285




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
#line 171 "..\\parser.cxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 183 "..\\parser.cxx"

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
#define YYLAST   534

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  11
/* YYNRULES -- Number of rules. */
#define YYNRULES  62
/* YYNRULES -- Number of states. */
#define YYNSTATES  190

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    31,     2,
      41,    42,    38,    37,    47,    36,     2,    39,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    40,
      33,    35,    34,     2,    45,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    44,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    32,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
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
     170,   178,   186,   194,   202,   207,   214,   219,   221,   230,
     234,   239,   244,   248,   252,   256,   260,   264,   268,   272,
     276,   281,   283
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      49,     0,    -1,    -1,    49,    29,    51,    40,    -1,    -1,
      41,    58,    42,    -1,    -1,    51,    52,    -1,    16,    28,
      43,    50,    56,    -1,    16,    50,    56,    -1,    -1,    53,
      29,    -1,    53,    38,    -1,    -1,    44,    45,    29,    46,
      -1,    44,    53,    46,    -1,    -1,    44,    45,    29,    46,
      -1,    44,    29,    46,    -1,    -1,    56,    57,    -1,    17,
      41,    58,    42,    54,    -1,    18,    41,    58,    42,    54,
      -1,    19,    41,    58,    42,    -1,     7,    41,    58,    42,
      -1,     9,    41,    58,    42,    -1,     5,    41,    58,    42,
      -1,    14,    41,    42,    -1,    10,    41,    42,    -1,    11,
      41,    42,    -1,    13,    41,    42,    -1,    12,    41,    58,
      47,    58,    47,    58,    47,    58,    42,    -1,    22,    41,
      58,    42,    54,    -1,    23,    41,    58,    42,    54,    -1,
      22,    41,    58,    47,    58,    42,    54,    -1,    23,    41,
      58,    47,    58,    42,    54,    -1,    24,    41,    58,    42,
      55,    -1,    25,    41,    58,    42,    55,    -1,    26,    41,
      58,    42,    55,    -1,    27,    41,    58,    42,    55,    -1,
      24,    41,    58,    47,    58,    42,    55,    -1,    25,    41,
      58,    47,    58,    42,    55,    -1,    26,    41,    58,    47,
      58,    42,    55,    -1,    27,    41,    58,    47,    58,    42,
      55,    -1,    21,    41,    58,    42,    -1,    15,    41,    29,
      35,    58,    42,    -1,     8,    41,    29,    42,    -1,    29,
      -1,    20,    41,    58,    47,    58,    47,    58,    42,    -1,
      41,    58,    42,    -1,     4,    41,    58,    42,    -1,     3,
      41,    58,    42,    -1,    58,    36,    58,    -1,    58,    38,
      58,    -1,    58,    39,    58,    -1,    58,    34,    58,    -1,
      58,    33,    58,    -1,    58,    31,    58,    -1,    58,    35,
      58,    -1,    58,    32,    58,    -1,     6,    41,    29,    42,
      -1,    28,    -1,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    43,    43,    44,    49,    50,    52,    53,    55,    56,
      58,    59,    60,    62,    63,    64,    66,    67,    68,    70,
      71,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ROUND", "NEG", "ASSERTION", "FACE",
  "TAPER", "SPAWN", "CHAMFER", "TEXTURE", "TEXTUREFULL", "TEXTUREQUAD",
  "TEXTURECLEAR", "UNCHAMFER", "ASSIGN", "DEFSIGN", "EXTRUDE", "EXTRUDET",
  "EXPAND", "RANDOM", "MATERIAL", "SUBDIVIDEH", "SUBDIVIDEV", "PARTITIONH",
  "PARTITIONV", "PARTITIONHI", "PARTITIONVI", "NUMBER", "NONTERM",
  "ATTRIBUTE", "'&'", "'|'", "'<'", "'>'", "'='", "'-'", "'+'", "'*'",
  "'/'", "';'", "'('", "')'", "':'", "'['", "'@'", "']'", "','", "$accept",
  "ruleset", "cond", "products", "product", "faces", "faceparam",
  "singleface", "ops", "op", "expr", 0
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
     285,    38,   124,    60,    62,    61,    45,    43,    42,    47,
      59,    40,    41,    58,    91,    64,    93,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    48,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    53,    54,    54,    54,    55,    55,    55,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     4,     0,     3,     0,     2,     5,     3,
       0,     2,     2,     0,     4,     3,     0,     4,     3,     0,
       2,     5,     5,     4,     4,     4,     4,     3,     3,     3,
       3,    10,     5,     5,     7,     7,     5,     5,     5,     5,
       7,     7,     7,     7,     4,     6,     4,     1,     8,     3,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,     6,     0,     4,     3,     7,     0,     0,
      19,     4,     0,     0,     0,     0,    61,    62,     0,     0,
       9,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    20,     8,     0,
       0,     0,     0,    49,    57,    59,    56,    55,    58,    52,
      53,    54,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    50,    60,     0,     0,     0,     0,     0,
      28,    29,     0,    30,    27,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,    24,    46,
      25,     0,     0,    13,    13,    23,    44,    13,     0,    13,
       0,    16,     0,    16,     0,    16,     0,    16,     0,     0,
       0,     0,    10,    21,    22,    32,     0,    33,     0,     0,
      36,     0,    37,     0,    38,     0,    39,     0,     0,     0,
      45,     0,     0,    13,    13,     0,     0,    16,    16,    16,
      16,    48,     0,     0,    11,    12,    15,    34,    35,    18,
       0,    40,    41,    42,    43,     0,    14,    17,     0,    31
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    10,     4,     7,   162,   143,   150,    20,    57,
      19
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -122
static const short int yypact[] =
{
    -122,     1,  -122,  -122,   -14,    -9,  -122,  -122,   -21,    17,
    -122,   -17,   -16,    -7,    -5,    -3,  -122,  -122,    17,   257,
     121,  -122,    17,    17,     2,    17,   269,    17,    17,    17,
      17,    17,    17,    17,    17,  -122,    -2,    -1,     0,     8,
       9,    10,    11,    12,    15,    18,    20,    21,    22,    33,
      34,    35,    47,    50,    60,    64,  -122,  -122,   121,   281,
     293,    65,   207,  -122,    61,    61,   495,   495,   495,   -10,
    -122,  -122,    17,    17,    63,    17,    66,    67,    17,    69,
      71,    73,    17,    17,    17,    17,    17,    17,    17,    17,
      17,    17,  -122,  -122,  -122,    17,   305,   317,    75,   329,
    -122,  -122,   217,  -122,  -122,    80,   341,   353,   365,   377,
      51,   122,   139,   156,   173,   190,   227,  -122,  -122,  -122,
    -122,    17,    17,    62,    62,  -122,  -122,    62,    17,    62,
      17,    78,    17,    78,    17,    78,    17,    78,    17,    17,
     237,   389,    74,  -122,  -122,  -122,   401,  -122,   413,   -12,
    -122,   425,  -122,   437,  -122,   449,  -122,   461,   473,    17,
    -122,    94,   -11,    62,    62,    79,    95,    78,    78,    78,
      78,  -122,   247,    81,  -122,  -122,  -122,  -122,  -122,  -122,
      91,  -122,  -122,  -122,  -122,    17,  -122,  -122,   485,  -122
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -122,  -122,   138,  -122,  -122,  -122,  -121,   -89,   130,  -122,
     -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short int yytable[] =
{
      26,     2,     5,   144,    59,    60,   145,    62,   147,    64,
      65,    66,    67,    68,    69,    70,    71,   165,   174,     8,
      12,    13,    11,    14,     9,    22,     6,   175,    33,    34,
       3,    61,     9,   166,    23,   176,    24,    15,    25,    72,
      73,    74,   177,   178,   152,    16,   154,    17,   156,    75,
      76,    77,    78,    79,    96,    97,    80,    99,    18,    81,
     102,    82,    83,    84,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    85,    86,    87,   116,   181,   182,
     183,   184,    27,    28,    29,    30,    31,    32,    88,    33,
      34,    89,    98,   127,    29,    30,    31,    32,   128,    33,
      34,    90,   105,   140,   141,    91,   142,    94,   100,   101,
     146,   103,   148,   104,   151,   122,   153,   119,   155,   161,
     157,   158,   149,   173,   180,   179,    36,   186,    37,    38,
      39,    40,    41,    42,    43,    44,    45,   187,    46,    47,
      48,   172,    49,    50,    51,    52,    53,    54,    55,    21,
      56,    58,     0,    27,    28,    29,    30,    31,    32,     0,
      33,    34,     0,     0,   129,     0,     0,   188,     0,   130,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   131,     0,     0,     0,     0,   132,    27,    28,    29,
      30,    31,    32,     0,    33,    34,     0,     0,   133,     0,
       0,     0,     0,   134,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   135,     0,     0,     0,     0,
     136,    27,    28,    29,    30,    31,    32,     0,    33,    34,
       0,     0,   137,     0,     0,     0,     0,   138,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,    27,    28,
      29,    30,    31,    32,    95,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   121,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   139,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   159,    33,    34,     0,    27,    28,
      29,    30,    31,    32,   185,    33,    34,     0,     0,    35,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,    63,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,    92,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,    93,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   117,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   118,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   120,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   123,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   124,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   125,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   126,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   160,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   163,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   164,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   167,    27,    28,
      29,    30,    31,    32,     0,    33,    34,     0,     0,   168,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       0,   169,    27,    28,    29,    30,    31,    32,     0,    33,
      34,     0,     0,   170,    27,    28,    29,    30,    31,    32,
       0,    33,    34,     0,     0,   171,    27,    28,    29,    30,
      31,    32,     0,    33,    34,     0,     0,   189,    -1,    -1,
      -1,    32,     0,    33,    34
};

static const short int yycheck[] =
{
      18,     0,    16,   124,    22,    23,   127,    25,   129,    27,
      28,    29,    30,    31,    32,    33,    34,    29,    29,    28,
       3,     4,    43,     6,    41,    41,    40,    38,    38,    39,
      29,    29,    41,    45,    41,    46,    41,    20,    41,    41,
      41,    41,   163,   164,   133,    28,   135,    30,   137,    41,
      41,    41,    41,    41,    72,    73,    41,    75,    41,    41,
      78,    41,    41,    41,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    41,    41,    41,    95,   167,   168,
     169,   170,    31,    32,    33,    34,    35,    36,    41,    38,
      39,    41,    29,    42,    33,    34,    35,    36,    47,    38,
      39,    41,    29,   121,   122,    41,    44,    42,    42,    42,
     128,    42,   130,    42,   132,    35,   134,    42,   136,    45,
     138,   139,    44,    29,    29,    46,     5,    46,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    46,    17,    18,
      19,   159,    21,    22,    23,    24,    25,    26,    27,    11,
      29,    21,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    -1,    42,    -1,    -1,   185,    -1,    47,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    47,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    47,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      47,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    31,    32,
      33,    34,    35,    36,    47,    38,    39,    -1,    31,    32,
      33,    34,    35,    36,    47,    38,    39,    -1,    31,    32,
      33,    34,    35,    36,    47,    38,    39,    -1,    31,    32,
      33,    34,    35,    36,    47,    38,    39,    -1,    31,    32,
      33,    34,    35,    36,    47,    38,    39,    -1,    -1,    42,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      -1,    42,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    -1,    42,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    -1,    42,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    -1,    42,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    -1,    42,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      -1,    42,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    -1,    42,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    -1,    42,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    -1,    42,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    -1,    42,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      -1,    42,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    -1,    42,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    -1,    42,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    -1,    42,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    -1,    42,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      -1,    42,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    -1,    42,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    -1,    42,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    -1,    42,    33,    34,
      35,    36,    -1,    38,    39
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    49,     0,    29,    51,    16,    40,    52,    28,    41,
      50,    43,     3,     4,     6,    20,    28,    30,    41,    58,
      56,    50,    41,    41,    41,    41,    58,    31,    32,    33,
      34,    35,    36,    38,    39,    42,     5,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    17,    18,    19,    21,
      22,    23,    24,    25,    26,    27,    29,    57,    56,    58,
      58,    29,    58,    42,    58,    58,    58,    58,    58,    58,
      58,    58,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    42,    42,    42,    47,    58,    58,    29,    58,
      42,    42,    58,    42,    42,    29,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    42,    42,    42,
      42,    47,    35,    42,    42,    42,    42,    42,    47,    42,
      47,    42,    47,    42,    47,    42,    47,    42,    47,    47,
      58,    58,    44,    54,    54,    54,    58,    54,    58,    44,
      55,    58,    55,    58,    55,    58,    55,    58,    58,    47,
      42,    45,    53,    42,    42,    29,    45,    42,    42,    42,
      42,    42,    58,    29,    29,    38,    46,    54,    54,    46,
      29,    55,    55,    55,    55,    47,    46,    46,    58,    42
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
#line 44 "..\\..\\src\\parser.y"
    { 
    std::string name = std::string((yyvsp[-2].id));
    ruleset->addRule(name,new Rule(name,(yyvsp[-1].pv)));
  ;}
    break;

  case 4:
#line 49 "..\\..\\src\\parser.y"
    { (yyval.e) = NULL; ;}
    break;

  case 5:
#line 50 "..\\..\\src\\parser.y"
    { (yyval.e) = (yyvsp[-1].e); ;}
    break;

  case 6:
#line 52 "..\\..\\src\\parser.y"
    { (yyval.pv) = new ProductVector(); ;}
    break;

  case 7:
#line 53 "..\\..\\src\\parser.y"
    { (yyval.pv) = (yyvsp[-1].pv); (yyval.pv)->push_back((yyvsp[0].p)); ;}
    break;

  case 8:
#line 55 "..\\..\\src\\parser.y"
    { (yyval.p) = new Product((yyvsp[0].ov),(yyvsp[-3].fl),(yyvsp[-1].e)); ;}
    break;

  case 9:
#line 56 "..\\..\\src\\parser.y"
    { (yyval.p) = new Product((yyvsp[0].ov),1.0f,(yyvsp[-1].e)); ;}
    break;

  case 10:
#line 58 "..\\..\\src\\parser.y"
    { (yyval.ids) = new StringVector(); ;}
    break;

  case 11:
#line 59 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[0].id)); (yyval.ids)->push_back(name); ;}
    break;

  case 12:
#line 60 "..\\..\\src\\parser.y"
    { std::string name = std::string(""); (yyval.ids)->push_back(name); ;}
    break;

  case 13:
#line 62 "..\\..\\src\\parser.y"
    { (yyval.ids) = NULL; ;}
    break;

  case 14:
#line 63 "..\\..\\src\\parser.y"
    { std::string name = '@'+std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 15:
#line 64 "..\\..\\src\\parser.y"
    { (yyval.ids) = (yyvsp[-1].ids); ;}
    break;

  case 16:
#line 66 "..\\..\\src\\parser.y"
    { (yyval.ids) = NULL; ;}
    break;

  case 17:
#line 67 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 18:
#line 68 "..\\..\\src\\parser.y"
    { std::string name = std::string((yyvsp[-1].id)); (yyval.ids) = new StringVector(); (yyval.ids)->push_back(name); ;}
    break;

  case 19:
#line 70 "..\\..\\src\\parser.y"
    { (yyval.ov) = new OperationVector(); ;}
    break;

  case 20:
#line 71 "..\\..\\src\\parser.y"
    { (yyval.ov) = (yyvsp[-1].ov); (yyval.ov)->push_back((yyvsp[0].o)); ;}
    break;

  case 21:
#line 73 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExtrude(ruleset,(yyvsp[-2].e),(yyvsp[0].ids)); ;}
    break;

  case 22:
#line 74 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExtrudeT(ruleset,(yyvsp[-2].e),(yyvsp[0].ids)); ;}
    break;

  case 23:
#line 75 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationExpand(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 24:
#line 76 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTaper(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 25:
#line 77 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationChamfer(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 26:
#line 78 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationAssert(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 27:
#line 79 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationUnchamfer(ruleset); ;}
    break;

  case 28:
#line 80 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTexture(ruleset); ;}
    break;

  case 29:
#line 81 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureFull(ruleset); ;}
    break;

  case 30:
#line 82 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureClear(ruleset); ;}
    break;

  case 31:
#line 83 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationTextureQuad(ruleset,(yyvsp[-7].e),(yyvsp[-5].e),(yyvsp[-3].e),(yyvsp[-1].e)); ;}
    break;

  case 32:
#line 84 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids)); ;}
    break;

  case 33:
#line 85 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids)); ;}
    break;

  case 34:
#line 86 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),(yyvsp[-2].e)); ;}
    break;

  case 35:
#line 87 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSubdivide(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),(yyvsp[-2].e)); ;}
    break;

  case 36:
#line 88 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids)); ;}
    break;

  case 37:
#line 89 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids)); ;}
    break;

  case 38:
#line 90 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),true,(yyvsp[0].ids),true); ;}
    break;

  case 39:
#line 91 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-2].e),false,(yyvsp[0].ids),true); ;}
    break;

  case 40:
#line 92 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),false,(yyvsp[-2].e)); ;}
    break;

  case 41:
#line 93 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),false,(yyvsp[-2].e)); ;}
    break;

  case 42:
#line 94 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),true,(yyvsp[0].ids),true,(yyvsp[-2].e)); ;}
    break;

  case 43:
#line 95 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationPartition(ruleset,(yyvsp[-4].e),false,(yyvsp[0].ids),true,(yyvsp[-2].e)); ;}
    break;

  case 44:
#line 96 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationMaterial(ruleset,(yyvsp[-1].e)); ;}
    break;

  case 45:
#line 97 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationAssign(ruleset,(yyvsp[-1].e),(yyvsp[-3].id)); ;}
    break;

  case 46:
#line 98 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationSpawn(ruleset,(yyvsp[-1].id)); ;}
    break;

  case 47:
#line 99 "..\\..\\src\\parser.y"
    { (yyval.o) = new OperationNonterminal(ruleset,(yyvsp[0].id)); ;}
    break;

  case 48:
#line 101 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionRandom((yyvsp[-5].e),(yyvsp[-3].e),(yyvsp[-1].e)); ;}
    break;

  case 49:
#line 102 "..\\..\\src\\parser.y"
    { (yyval.e) = (yyvsp[-1].e); ;}
    break;

  case 50:
#line 103 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionNeg((yyvsp[-1].e)); ;}
    break;

  case 51:
#line 104 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionRound((yyvsp[-1].e)); ;}
    break;

  case 52:
#line 105 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionSub((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 53:
#line 106 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionMult((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 54:
#line 107 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionDiv((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 55:
#line 108 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionGreater((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 56:
#line 109 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionGreater((yyvsp[0].e),(yyvsp[-2].e)); ;}
    break;

  case 57:
#line 110 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionAnd((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 58:
#line 111 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionEqual((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 59:
#line 112 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionOr((yyvsp[-2].e),(yyvsp[0].e)); ;}
    break;

  case 60:
#line 113 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionFaceAttribute((yyvsp[-1].id)); ;}
    break;

  case 61:
#line 114 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionConst((yyvsp[0].fl)); ;}
    break;

  case 62:
#line 115 "..\\..\\src\\parser.y"
    { (yyval.e) = new ExpressionAttribute(ruleset,(yyvsp[0].id)); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1687 "..\\parser.cxx"

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


#line 117 "..\\..\\src\\parser.y"


