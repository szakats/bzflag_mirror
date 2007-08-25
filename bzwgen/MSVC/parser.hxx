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
#line 115 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



