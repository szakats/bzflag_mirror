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
     SCALE = 258,
     TEST = 259,
     ROUND = 260,
     NEG = 261,
     ASSERTION = 262,
     FACE = 263,
     TAPER = 264,
     SPAWN = 265,
     CHAMFER = 266,
     TEXTURE = 267,
     TEXTUREFULL = 268,
     TEXTUREQUAD = 269,
     TEXTURECLEAR = 270,
     SPAWNNGON = 271,
     UNCHAMFER = 272,
     ASSIGN = 273,
     DEFSIGN = 274,
     EXTRUDE = 275,
     EXTRUDET = 276,
     EXPAND = 277,
     RANDOM = 278,
     MATERIAL = 279,
     SUBDIVIDEH = 280,
     SUBDIVIDEV = 281,
     PARTITIONH = 282,
     PARTITIONV = 283,
     PARTITIONHI = 284,
     PARTITIONVI = 285,
     MULTIFACE = 286,
     NUMBER = 287,
     NONTERM = 288,
     ATTRIBUTE = 289
   };
#endif
/* Tokens.  */
#define SCALE 258
#define TEST 259
#define ROUND 260
#define NEG 261
#define ASSERTION 262
#define FACE 263
#define TAPER 264
#define SPAWN 265
#define CHAMFER 266
#define TEXTURE 267
#define TEXTUREFULL 268
#define TEXTUREQUAD 269
#define TEXTURECLEAR 270
#define SPAWNNGON 271
#define UNCHAMFER 272
#define ASSIGN 273
#define DEFSIGN 274
#define EXTRUDE 275
#define EXTRUDET 276
#define EXPAND 277
#define RANDOM 278
#define MATERIAL 279
#define SUBDIVIDEH 280
#define SUBDIVIDEV 281
#define PARTITIONH 282
#define PARTITIONV 283
#define PARTITIONHI 284
#define PARTITIONVI 285
#define MULTIFACE 286
#define NUMBER 287
#define NONTERM 288
#define ATTRIBUTE 289




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
#line 117 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



