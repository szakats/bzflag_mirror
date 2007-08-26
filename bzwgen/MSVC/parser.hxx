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
     TRANSLATER = 258,
     TRANSLATE = 259,
     SCALE = 260,
     TEST = 261,
     ROUND = 262,
     NEG = 263,
     ASSERTION = 264,
     FACE = 265,
     TAPER = 266,
     SPAWN = 267,
     CHAMFER = 268,
     TEXTURE = 269,
     TEXTUREFULL = 270,
     TEXTUREQUAD = 271,
     TEXTURECLEAR = 272,
     SPAWNNGON = 273,
     UNCHAMFER = 274,
     ASSIGN = 275,
     DEFSIGN = 276,
     EXTRUDE = 277,
     EXTRUDET = 278,
     EXPAND = 279,
     RANDOM = 280,
     MATERIAL = 281,
     SUBDIVIDEH = 282,
     SUBDIVIDEV = 283,
     PARTITIONH = 284,
     PARTITIONV = 285,
     PARTITIONHI = 286,
     PARTITIONVI = 287,
     MULTIFACE = 288,
     FREE = 289,
     NGON = 290,
     REMOVE = 291,
     ADDFACE = 292,
     DETACHFACE = 293,
     NUMBER = 294,
     NONTERM = 295,
     ATTRIBUTE = 296
   };
#endif
/* Tokens.  */
#define TRANSLATER 258
#define TRANSLATE 259
#define SCALE 260
#define TEST 261
#define ROUND 262
#define NEG 263
#define ASSERTION 264
#define FACE 265
#define TAPER 266
#define SPAWN 267
#define CHAMFER 268
#define TEXTURE 269
#define TEXTUREFULL 270
#define TEXTUREQUAD 271
#define TEXTURECLEAR 272
#define SPAWNNGON 273
#define UNCHAMFER 274
#define ASSIGN 275
#define DEFSIGN 276
#define EXTRUDE 277
#define EXTRUDET 278
#define EXPAND 279
#define RANDOM 280
#define MATERIAL 281
#define SUBDIVIDEH 282
#define SUBDIVIDEV 283
#define PARTITIONH 284
#define PARTITIONV 285
#define PARTITIONHI 286
#define PARTITIONVI 287
#define MULTIFACE 288
#define FREE 289
#define NGON 290
#define REMOVE 291
#define ADDFACE 292
#define DETACHFACE 293
#define NUMBER 294
#define NONTERM 295
#define ATTRIBUTE 296




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
#line 131 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



