/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "globals.h"
#include "Mesh.h"

class RuleSet;

class Expression {
public:
  virtual float calculate(Mesh*,int) = 0;
  virtual ~Expression() {};
};

class ExpressionConst : public Expression {
  float value;
public:
  ExpressionConst(float _value) : value(_value) {};
  float calculate(Mesh*,int) { return value; };
};

class ExpressionAttribute : public Expression {
  RuleSet* ruleset;
  std::string attrname;
public:
  ExpressionAttribute(RuleSet* _ruleset, std::string& _attrname) : ruleset(_ruleset), attrname(_attrname) {};
  float calculate(Mesh*,int);
};

class ExpressionFaceAttribute : public Expression {
  std::string attrname;
public:
  ExpressionFaceAttribute(std::string& _attrname) : attrname(_attrname) { 
    std::transform(attrname.begin(), attrname.end(), attrname.begin(), tolower);
  };
  float calculate(Mesh* mesh,int face) {
    if (attrname == "x") return mesh->faceCenter(face).x;
    if (attrname == "y") return mesh->faceCenter(face).x;
    if (attrname == "z") return mesh->faceCenter(face).x;
    if (attrname == "h") 
      return (mesh->v[mesh->f[face]->vtx->at(0)] - mesh->v[mesh->f[face]->vtx->at(1)]).length();
    if (attrname == "v") 
      return (mesh->v[mesh->f[face]->vtx->at(3)] - mesh->v[mesh->f[face]->vtx->at(0)]).length();
    if (debugLevel > 1) printf("Unknown face() attribute : '%s'!\n",attrname.c_str());
    return 0.0f;
  };
};

class ExpressionRandom : public Expression {
  Expression* min;
  Expression* max;
  Expression* step;
public:
  ExpressionRandom(Expression* _min, Expression* _max, Expression* _step) {
    min = _min;
    max = _max;
    step = _step;
  };
  float calculate(Mesh* mesh,int face) { 
    float stepc = step->calculate(mesh,face);
    if (fabs(stepc) < 0.0001f) return randomFloatRange(min->calculate(mesh,face),max->calculate(mesh,face));
    return randomFloatRangeStep(min->calculate(mesh,face),max->calculate(mesh,face),stepc); 
  };
  ~ExpressionRandom() {
    delete min;
    delete max;
    delete step;
  }
};

class ExpressionDouble : public Expression {
  Expression* a;
  Expression* b;
public:
  ExpressionDouble(Expression* _a, Expression* _b) : a(_a), b(_b) {};
  virtual float calc(float av, float bv) = 0;
  float calculate(Mesh* mesh,int face) { 
    float av = a->calculate(mesh,face);
    float bv = b->calculate(mesh,face);
    return calc(av,bv);
  }
};

class ExpressionAdd : public ExpressionDouble {
public:
  ExpressionAdd(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  float calc(float av, float bv) { return av + bv; }
};
class ExpressionSub : public ExpressionDouble {
public:
  ExpressionSub(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  float calc(float av, float bv) { return av - bv; }
};
class ExpressionDiv : public ExpressionDouble {
public:
  ExpressionDiv(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  float calc(float av, float bv) { return av / bv; }
};
class ExpressionMult : public ExpressionDouble {
public:
  ExpressionMult(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  float calc(float av, float bv) { return av * bv; }
};
class ExpressionGreater : public ExpressionDouble {
public:
  ExpressionGreater(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  float calc(float av, float bv) { if(av > bv) return 1.0; else return -1.0; }
};




#endif /* __EXPRESSION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8};

