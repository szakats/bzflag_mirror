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
#include "Mesh.h"

class Operation {
public:
  virtual int runMesh(Mesh*,int) = 0;
  virtual ~Operation() {}
};

typedef std::vector <Operation*> OperationVector;
typedef OperationVector::iterator OperationVectIter;

class RuleSet; // To avoid .h file recursion

class OperationNonterminal : public Operation {
  std::string ref;
  RuleSet* ruleset;
public:
  OperationNonterminal(std::string& _ref, RuleSet* _ruleset) : ref(_ref), ruleset(_ruleset) { };
  int runMesh(Mesh*,int);
};

class OperationSingle : public Operation {
protected:
  Expression *exp;
  float value;
public:
  OperationSingle(Expression* _exp) : exp(_exp) { };
  void flatten() { value = exp->calculate(); }
  ~OperationSingle() {
    delete exp;
  }
};


class OperationExtrude : public OperationSingle {
public:
  OperationExtrude(Expression* _exp) : OperationSingle(_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    flatten();
    mesh->extrudeFace(face,value,MATGLASS);
    return face; 
  };
};
class OperationExpand : public OperationSingle {
public:
  OperationExpand(Expression* _exp) : OperationSingle(_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    flatten();
    mesh->expandFace(face,value);
    return face; 
  };
};
class OperationSubdivide : public OperationSingle {
public:
  OperationSubdivide(Expression* _exp) : OperationSingle(_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    flatten();
    mesh->subdivdeFace(face,int(value),true);
    return face; 
  };
};


#endif /* __OPERATION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
