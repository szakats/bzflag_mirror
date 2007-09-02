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
     SPAWNNGON = 275,
     UNCHAMFER = 276,
     ASSIGN = 277,
     DEFSIGN = 278,
     EXTRUDE = 279,
     EXTRUDET = 280,
     EXPAND = 281,
     RANDOM = 282,
     REPEATH = 283,
     REPEATV = 284,
     SPLITV = 285,
     SPLITH = 286,
     MULTIFACE = 287,
     FREE = 288,
     NGON = 289,
     REMOVE = 290,
     ADDFACE = 291,
     DETACHFACE = 292,
     DRIVETHROUGH = 293,
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
#define MATERIAL 273
#define LOADMATERIAL 274
#define SPAWNNGON 275
#define UNCHAMFER 276
#define ASSIGN 277
#define DEFSIGN 278
#define EXTRUDE 279
#define EXTRUDET 280
#define EXPAND 281
#define RANDOM 282
#define REPEATH 283
#define REPEATV 284
#define SPLITV 285
#define SPLITH 286
#define MULTIFACE 287
#define FREE 288
#define NGON 289
#define REMOVE 290
#define ADDFACE 291
#define DETACHFACE 292
#define DRIVETHROUGH 293
#define NUMBER 294
#define NONTERM 295
#define ATTRIBUTE 296




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
#line 132 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



