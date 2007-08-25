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
     TRANSLATE = 258,
     SCALE = 259,
     TEST = 260,
     ROUND = 261,
     NEG = 262,
     ASSERTION = 263,
     FACE = 264,
     TAPER = 265,
     SPAWN = 266,
     CHAMFER = 267,
     TEXTURE = 268,
     TEXTUREFULL = 269,
     TEXTUREQUAD = 270,
     TEXTURECLEAR = 271,
     SPAWNNGON = 272,
     UNCHAMFER = 273,
     ASSIGN = 274,
     DEFSIGN = 275,
     EXTRUDE = 276,
     EXTRUDET = 277,
     EXPAND = 278,
     RANDOM = 279,
     MATERIAL = 280,
     SUBDIVIDEH = 281,
     SUBDIVIDEV = 282,
     PARTITIONH = 283,
     PARTITIONV = 284,
     PARTITIONHI = 285,
     PARTITIONVI = 286,
     MULTIFACE = 287,
     FREE = 288,
     NGON = 289,
     REMOVE = 290,
     ADDFACE = 291,
     NUMBER = 292,
     NONTERM = 293,
     ATTRIBUTE = 294
   };
#endif
/* Tokens.  */
#define TRANSLATE 258
#define SCALE 259
#define TEST 260
#define ROUND 261
#define NEG 262
#define ASSERTION 263
#define FACE 264
#define TAPER 265
#define SPAWN 266
#define CHAMFER 267
#define TEXTURE 268
#define TEXTUREFULL 269
#define TEXTUREQUAD 270
#define TEXTURECLEAR 271
#define SPAWNNGON 272
#define UNCHAMFER 273
#define ASSIGN 274
#define DEFSIGN 275
#define EXTRUDE 276
#define EXTRUDET 277
#define EXPAND 278
#define RANDOM 279
#define MATERIAL 280
#define SUBDIVIDEH 281
#define SUBDIVIDEV 282
#define PARTITIONH 283
#define PARTITIONV 284
#define PARTITIONHI 285
#define PARTITIONVI 286
#define MULTIFACE 287
#define FREE 288
#define NGON 289
#define REMOVE 290
#define ADDFACE 291
#define NUMBER 292
#define NONTERM 293
#define ATTRIBUTE 294




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
#line 127 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



