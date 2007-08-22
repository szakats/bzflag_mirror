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
     ROUND = 258,
     NEG = 259,
     ASSERTION = 260,
     FACE = 261,
     TAPER = 262,
     SPAWN = 263,
     CHAMFER = 264,
     TEXTUREFULL = 265,
     TEXTUREQUAD = 266,
     UNCHAMFER = 267,
     ASSIGN = 268,
     DEFSIGN = 269,
     EXTRUDE = 270,
     EXPAND = 271,
     RANDOM = 272,
     MATERIAL = 273,
     SUBDIVIDEH = 274,
     SUBDIVIDEV = 275,
     PARTITIONH = 276,
     PARTITIONV = 277,
     PARTITIONHI = 278,
     PARTITIONVI = 279,
     NUMBER = 280,
     NONTERM = 281,
     ATTRIBUTE = 282
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
#define TEXTUREFULL 265
#define TEXTUREQUAD 266
#define UNCHAMFER 267
#define ASSIGN 268
#define DEFSIGN 269
#define EXTRUDE 270
#define EXPAND 271
#define RANDOM 272
#define MATERIAL 273
#define SUBDIVIDEH 274
#define SUBDIVIDEV 275
#define PARTITIONH 276
#define PARTITIONV 277
#define PARTITIONHI 278
#define PARTITIONVI 279
#define NUMBER 280
#define NONTERM 281
#define ATTRIBUTE 282




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
#line 103 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



