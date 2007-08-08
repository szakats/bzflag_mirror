/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "Token.h"
#include <string>
#include <assert.h>
#include <iostream>

Token::Token(const Token& token) {
  clear();
  type = token.type;
  switch(type) {
  case KEYWORD:
    keyword = new char[strlen(token.keyword) + 1];
    strcpy(keyword, token.keyword);
    break;
  case OPERATOR:
    op = new char[strlen(token.op) + 1];
    strcpy(op, token.op);
    break;
  case VARIABLE:
    variable = new char[strlen(token.variable) + 1];
    strcpy(variable, token.variable);
    break;
  case BOOLEAN:
    boolean = token.boolean;
    break;
  case INTEGER:
    integer = token.integer;
    break;
  case REAL:
    real = token.real;
    break;
  };
}

void Token::operator =( const Token& token ) {
  destroy();
  type = token.type;
  switch(type) {
  case KEYWORD:
    keyword = new char[strlen(token.keyword) + 1];
    strcpy(keyword, token.keyword);
    break;
  case OPERATOR:
    op = new char[strlen(token.op) + 1];
    strcpy(op, token.op);
    break;
  case VARIABLE:
    variable = new char[strlen(token.variable) + 1];
    strcpy(variable, token.variable);
    break;
  case BOOLEAN:
    boolean = token.boolean;
    break;
  case INTEGER:
    integer = token.integer;
    break;
  case REAL:
    real = token.real;
    break;
  };
}


void Token::clear() {
  type = UNKNOWN;
}


void Token::destroy() {
  switch(type) {
  case KEYWORD:
    delete[] keyword;
    break;
  case OPERATOR:
    delete[] op;
    break;
  case VARIABLE:
    delete[] variable;
    break;
  };
  clear();
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
