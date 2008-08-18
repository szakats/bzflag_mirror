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
     MATERIAL = 273,
     LOADMATERIAL = 274,
     LOADMATERIALNR = 275,
     SPAWNNGON = 276,
     UNCHAMFER = 277,
     ASSIGN = 278,
     DEFSIGN = 279,
     EXTRUDE = 280,
     EXTRUDET = 281,
     EXPAND = 282,
     RANDOM = 283,
     REPEATH = 284,
     REPEATV = 285,
     SPLITV = 286,
     SPLITH = 287,
     MULTIFACE = 288,
     FREE = 289,
     NGON = 290,
     REMOVE = 291,
     ADDFACE = 292,
     DETACHFACE = 293,
     DRIVETHROUGH = 294,
     NUMBER = 295,
     NONTERM = 296,
     ATTRIBUTE = 297
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
#define MATERIAL 273
#define LOADMATERIAL 274
#define LOADMATERIALNR 275
#define SPAWNNGON 276
#define UNCHAMFER 277
#define ASSIGN 278
#define DEFSIGN 279
#define EXTRUDE 280
#define EXTRUDET 281
#define EXPAND 282
#define RANDOM 283
#define REPEATH 284
#define REPEATV 285
#define SPLITV 286
#define SPLITH 287
#define MULTIFACE 288
#define FREE 289
#define NGON 290
#define REMOVE 291
#define ADDFACE 292
#define DETACHFACE 293
#define DRIVETHROUGH 294
#define NUMBER 295
#define NONTERM 296
#define ATTRIBUTE 297




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 16 "..\\..\\src\\parser.y"
typedef union _YYSTYPE {
  char *id;
  double fl;
  ProductVector* pv;
  Product* p;
  OperationVector* ov;
  StringVector* ids;
  ExpressionVector* ev;
  Operation* o;
  Expression* e;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 134 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



