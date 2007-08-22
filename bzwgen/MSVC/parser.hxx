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
     TEXTUREFULL = 264,
     UNCHAMFER = 265,
     ASSIGN = 266,
     DEFSIGN = 267,
     EXTRUDE = 268,
     EXPAND = 269,
     RANDOM = 270,
     MATERIAL = 271,
     SUBDIVIDEH = 272,
     SUBDIVIDEV = 273,
     PARTITIONH = 274,
     PARTITIONV = 275,
     PARTITIONHI = 276,
     PARTITIONVI = 277,
     NUMBER = 278,
     NONTERM = 279,
     ATTRIBUTE = 280
   };
#endif
/* Tokens.  */
#define NEG 258
#define ASSERTION 259
#define FACE 260
#define TAPER 261
#define SPAWN 262
#define CHAMFER 263
#define TEXTUREFULL 264
#define UNCHAMFER 265
#define ASSIGN 266
#define DEFSIGN 267
#define EXTRUDE 268
#define EXPAND 269
#define RANDOM 270
#define MATERIAL 271
#define SUBDIVIDEH 272
#define SUBDIVIDEV 273
#define PARTITIONH 274
#define PARTITIONV 275
#define PARTITIONHI 276
#define PARTITIONVI 277
#define NUMBER 278
#define NONTERM 279
#define ATTRIBUTE 280




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
#line 99 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



