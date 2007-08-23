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
     EXTRUDET = 271,
     EXPAND = 272,
     RANDOM = 273,
     MATERIAL = 274,
     SUBDIVIDEH = 275,
     SUBDIVIDEV = 276,
     PARTITIONH = 277,
     PARTITIONV = 278,
     PARTITIONHI = 279,
     PARTITIONVI = 280,
     NUMBER = 281,
     NONTERM = 282,
     ATTRIBUTE = 283
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
#define EXTRUDET 271
#define EXPAND 272
#define RANDOM 273
#define MATERIAL 274
#define SUBDIVIDEH 275
#define SUBDIVIDEV 276
#define PARTITIONH 277
#define PARTITIONV 278
#define PARTITIONHI 279
#define PARTITIONVI 280
#define NUMBER 281
#define NONTERM 282
#define ATTRIBUTE 283




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
#line 105 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



