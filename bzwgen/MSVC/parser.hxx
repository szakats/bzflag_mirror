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
     ASSERTION = 258,
     FACE = 259,
     TAPER = 260,
     SPAWN = 261,
     CHAMFER = 262,
     UNCHAMFER = 263,
     ASSIGN = 264,
     DEFSIGN = 265,
     EXTRUDE = 266,
     EXPAND = 267,
     RANDOM = 268,
     SUBDIVIDEH = 269,
     SUBDIVIDEV = 270,
     PARTITIONH = 271,
     PARTITIONV = 272,
     MATERIAL = 273,
     NUMBER = 274,
     NONTERM = 275,
     ATTRIBUTE = 276
   };
#endif
/* Tokens.  */
#define ASSERTION 258
#define FACE 259
#define TAPER 260
#define SPAWN 261
#define CHAMFER 262
#define UNCHAMFER 263
#define ASSIGN 264
#define DEFSIGN 265
#define EXTRUDE 266
#define EXPAND 267
#define RANDOM 268
#define SUBDIVIDEH 269
#define SUBDIVIDEV 270
#define PARTITIONH 271
#define PARTITIONV 272
#define MATERIAL 273
#define NUMBER 274
#define NONTERM 275
#define ATTRIBUTE 276




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 16 "..\\..\\parser.y"
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
#line 91 "..\\parser.hxx"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



