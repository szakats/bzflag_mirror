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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NEG = 258,
     ASSERTION = 259,
     FACE = 260,
     TAPER = 261,
     SPAWN = 262,
     CHAMFER = 263,
     UNCHAMFER = 264,
     ASSIGN = 265,
     DEFSIGN = 266,
     EXTRUDE = 267,
     EXPAND = 268,
     RANDOM = 269,
     SUBDIVIDEH = 270,
     SUBDIVIDEV = 271,
     PARTITIONH = 272,
     PARTITIONV = 273,
     MATERIAL = 274,
     NUMBER = 275,
     NONTERM = 276,
     ATTRIBUTE = 277
   };
#endif
/* Tokens.  */
#define NEG 258
#define ASSERTION 259
#define FACE 260
#define TAPER 261
#define SPAWN 262
#define CHAMFER 263
#define UNCHAMFER 264
#define ASSIGN 265
#define DEFSIGN 266
#define EXTRUDE 267
#define EXPAND 268
#define RANDOM 269
#define SUBDIVIDEH 270
#define SUBDIVIDEV 271
#define PARTITIONH 272
#define PARTITIONV 273
#define MATERIAL 274
#define NUMBER 275
#define NONTERM 276
#define ATTRIBUTE 277




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
/* Line 1447 of yacc.c.  */
#line 93 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



