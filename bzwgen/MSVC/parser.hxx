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
     TEXTUREQUAD = 265,
     UNCHAMFER = 266,
     ASSIGN = 267,
     DEFSIGN = 268,
     EXTRUDE = 269,
     EXPAND = 270,
     RANDOM = 271,
     MATERIAL = 272,
     SUBDIVIDEH = 273,
     SUBDIVIDEV = 274,
     PARTITIONH = 275,
     PARTITIONV = 276,
     PARTITIONHI = 277,
     PARTITIONVI = 278,
     NUMBER = 279,
     NONTERM = 280,
     ATTRIBUTE = 281
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
#define TEXTUREQUAD 265
#define UNCHAMFER 266
#define ASSIGN 267
#define DEFSIGN 268
#define EXTRUDE 269
#define EXPAND 270
#define RANDOM 271
#define MATERIAL 272
#define SUBDIVIDEH 273
#define SUBDIVIDEV 274
#define PARTITIONH 275
#define PARTITIONV 276
#define PARTITIONHI 277
#define PARTITIONVI 278
#define NUMBER 279
#define NONTERM 280
#define ATTRIBUTE 281




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
#line 101 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



