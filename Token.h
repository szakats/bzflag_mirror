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

#include <vector>
#include <list>

#ifndef __TOKEN_H__
#define __TOKEN_H__

#pragma warning(disable:4996)

class Token 
{
public:
  enum TYPE {
    UNKNOWN = 0,
    KEYWORD,
    OPERATOR,
    VARIABLE,
    INTEGER,
    REAL,
    BOOLEAN
  };


  Token() { clear();  }
  Token(const Token& token);
  ~Token() { destroy(); }
  void clear();
  void operator= (const Token& token);

  inline void createKeyword(const char* _keyword) {
    type = KEYWORD;
    keyword = new char[strlen(_keyword) + 1];
    strcpy(keyword, _keyword);
  }

  inline void createOperator(const char* _operator) {
    type = OPERATOR;
    op = new char[strlen(_operator) + 1];
    strcpy(op, _operator);
  }

  inline void createVariable(const char* _variable) {
    type = VARIABLE;
    variable = new char[strlen(_variable) + 1];
    strcpy(variable, _variable);
  }

  inline void createInteger(int _integer) {
    type = INTEGER;
    integer = _integer;
  }

  inline void createReal(float _real) {
    type = REAL;
    real = _real;
  }

  inline void createBoolean(bool _boolean) {
    type = BOOLEAN;
    boolean = _boolean;
  }

  void destroy();

  inline void setVariable(const char* _variable) {
    delete[] variable;
    createVariable(_variable);
  }

  inline void setKeyword(const char* _keyword) {
    delete[] keyword;
    createKeyword(_keyword);
  }

  inline void setOperator(const char* _operator) {
    delete[] op;
    createOperator(_operator);
  }

  inline void setInteger(const int _integer) { createInteger(_integer); }
  inline void setBoolean(const bool _boolean) { createBoolean(_boolean); }
  inline void setReal(const float _real) { createReal(_real); }

  inline bool isUnknown() const	 { return (type == UNKNOWN);  }
  inline bool isKeyword() const  { return (type == KEYWORD);  }
  inline bool isOperator() const { return (type == OPERATOR);  }
  inline bool isVariable() const { return (type == VARIABLE);  }
  inline bool isInteger() const	 { return (type == INTEGER);  }
  inline bool isReal() const     { return (type == REAL);  }
  inline bool isBoolean() const	 { return (type == BOOLEAN);  }
  inline TYPE getType() const    { return type;  }

  inline const char* getKeyword() const	{ return (const char*)keyword; }
  inline const char* getOperator() const { return (const char*)op; }
  inline const char* getVariable() const { return (const char*)variable; }
  inline int getInteger() const	{ return integer; }
  inline float getReal() const	{ return real; }
  inline bool getBoolean() const { return boolean; }
  inline bool isNewline() const	{ return (isOperator() && (getOperator()[0] == '\n')); }  

private:
  TYPE type;

  union {
    char* keyword;
    char* op;
    char* variable;
    int   integer;
    float real;
    bool  boolean;
  };
};

typedef std::vector<Token> TokenVector;
typedef std::list<Token> TokenList;
typedef std::list<Token>::iterator TokenListIter;

#endif /* __TOKEN_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8