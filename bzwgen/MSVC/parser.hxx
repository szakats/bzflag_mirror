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
     PARTITIONHS = 283,
     PARTITIONVS = 284,
     PARTITIONHIS = 285,
     PARTITIONVIS = 286,
     NUMBER = 287,
     NONTERM = 288,
     ATTRIBUTE = 289
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
#define PARTITIONHS 283
#define PARTITIONVS 284
#define PARTITIONHIS 285
#define PARTITIONVIS 286
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



