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

#ifndef __OPERATION_H__
#define __OPERATION_H__

#include <vector>
#include <string>
#include "Expression.h"

class Operation {
public:
  ~Operation() {}
};

typedef std::vector <Operation*> OperationVector;
typedef OperationVector::iterator OperationVectIter;

class OperationNonterminal : public Operation {
  std::string ref;
public:
  OperationNonterminal(std::string& _ref) : ref(_ref) { };
};

class OperationSingle : public Operation {
  Expression *exp;
public:
  OperationSingle(Expression* _exp) : exp(_exp) { };
  ~OperationSingle() {
    delete exp;
  }
};


class OperationExtrude : public OperationSingle {
public:
  OperationExtrude(Expression* _exp) : OperationSingle(_exp) {}
};
class OperationExpand : public OperationSingle {
public:
  OperationExpand(Expression* _exp) : OperationSingle(_exp) {}
};
class OperationSubdivide : public OperationSingle {
public:
  OperationSubdivide(Expression* _exp) : OperationSingle(_exp) {}
};


#endif /* __OPERATION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
