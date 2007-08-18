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
#include "globals.h"

class RuleSet; // To avoid .h file recursion

class Operation {
protected:
  RuleSet* ruleset;
public:
  Operation(RuleSet* _ruleset) : ruleset(_ruleset) {}
  virtual int runMesh(Mesh*,int) = 0;
  virtual ~Operation() {}
};

typedef std::vector <Operation*> OperationVector;
typedef OperationVector::iterator OperationVectIter;

class OperationNonterminal : public Operation {
  std::string ref;
public:
  OperationNonterminal(RuleSet* _ruleset, std::string& _ref) : Operation(_ruleset), ref(_ref) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationSingle : public Operation {
protected:
  Expression *exp;
  float value;
public:
  OperationSingle(RuleSet* _ruleset, Expression* _exp) : Operation(_ruleset), exp(_exp) { };
  void flatten() { value = exp->calculate(); }
  ~OperationSingle() {
    delete exp;
  }
};


class OperationMaterial : public OperationSingle {
public:
  OperationMaterial(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    flatten();
    mesh->f[face]->mat = round(value);
    return face; 
  };
};

class OperationExpand : public OperationSingle {
public:
  OperationExpand(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    flatten();
    mesh->expandFace(face,value);
    return face; 
  };
};

class OperationMultifaces : public OperationSingle {
protected:
  StringVector* facerules;
  IntVector* faces;
  bool allsame;
public:
  OperationMultifaces(RuleSet* _ruleset, Expression* _exp, StringVector* _facerules);
  int runMesh(Mesh* mesh,int);
  ~OperationMultifaces() {
    if (facerules != NULL) delete facerules;
    if (faces != NULL) delete faces;
  }  
};

class OperationExtrude : public OperationMultifaces {
public:
  OperationExtrude(RuleSet* _ruleset, Expression* _exp, StringVector* facerules) : OperationMultifaces(_ruleset,_exp,facerules) {}
  int runMesh(Mesh* mesh,int face);
};

class OperationSubdivide : public OperationMultifaces {
  bool horiz;
public:
  OperationSubdivide(RuleSet* _ruleset, Expression* _exp, bool _horiz, StringVector* facerules) : OperationMultifaces(_ruleset,_exp,facerules), horiz(_horiz) {}
  int runMesh(Mesh* mesh,int face);
};

class OperationPartition : public OperationMultifaces {
  bool horiz;
public:
  OperationPartition(RuleSet* _ruleset, Expression* _exp, bool _horiz, StringVector* facerules) : OperationMultifaces(_ruleset,_exp,facerules), horiz(_horiz) {}
  int runMesh(Mesh* mesh,int face);
};


#endif /* __OPERATION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
