/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
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

#include "globals.h"
#include "Mesh.h"
#include "MultiFace.h"
#include "Random.h"

class RuleSet;

class Expression {
public:
  virtual double calculate(Mesh*,int) = 0;
  virtual ~Expression() {};
};

typedef std::vector<Expression*> ExpressionVector;

class ExpressionConst : public Expression {
  double value;
public:
  ExpressionConst(double _value) : value(_value) {};
  double calculate(Mesh*,int) { return value; };
};

class ExpressionAttribute : public Expression {
  RuleSet* ruleset;
  String attrname;
public:
  ExpressionAttribute(RuleSet* _ruleset, const char* _attrname) : ruleset(_ruleset), attrname(_attrname) {};
  double calculate(Mesh*,int);
};

class ExpressionFaceAttribute : public Expression {
  String attrname;
public:
  ExpressionFaceAttribute(const char* _attrname) : attrname(_attrname) { 
    std::transform(attrname.begin(), attrname.end(), attrname.begin(), tolower);
  };
  double calculate(Mesh* mesh,int face);
};

class ExpressionRandom : public Expression {
  Expression* vmin;
  Expression* vmax;
  Expression* step;
public:
  ExpressionRandom(Expression* _min, Expression* _max, Expression* _step) : vmin(_min), vmax(_max), step(_step) { };
  double calculate(Mesh* mesh,int face) { 
    double stepc = step->calculate(mesh,face);
    if (fabs(stepc) < 0.0001f) return Random::doubleRange(vmin->calculate(mesh,face),vmax->calculate(mesh,face));
    return Random::doubleRangeStep(vmin->calculate(mesh,face),vmax->calculate(mesh,face),stepc); 
  };
  ~ExpressionRandom() {
    delete vmin;
    delete vmax;
    delete step;
  }
};

class ExpressionNeg : public Expression {
  Expression* a;
public:
  ExpressionNeg(Expression* _a) : a(_a) {};
  double calculate(Mesh* mesh,int face) { 
    return -a->calculate(mesh,face);
  }
};

class ExpressionRound : public Expression {
  Expression* a;
public:
  ExpressionRound(Expression* _a) : a(_a) {};
  double calculate(Mesh* mesh,int face) { 
    return math::roundToInt(a->calculate(mesh,face));
  }
};

class ExpressionDouble : public Expression {
  Expression* a;
  Expression* b;
public:
  ExpressionDouble(Expression* _a, Expression* _b) : a(_a), b(_b) {};
  virtual double calc(double av, double bv) = 0;
  double calculate(Mesh* mesh,int face) { 
    double av = a->calculate(mesh,face);
    double bv = b->calculate(mesh,face);
    return calc(av,bv);
  }
};

class ExpressionAdd : public ExpressionDouble {
public:
  ExpressionAdd(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { return av + bv; }
};
class ExpressionSub : public ExpressionDouble {
public:
  ExpressionSub(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { return av - bv; }
};
class ExpressionDiv : public ExpressionDouble {
public:
  ExpressionDiv(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { return av / bv; }
};
class ExpressionMult : public ExpressionDouble {
public:
  ExpressionMult(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { return av * bv; }
};
class ExpressionGreater : public ExpressionDouble {
public:
  ExpressionGreater(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { if(av > bv) return 1.0; else return -1.0; }
};

class ExpressionEqual : public ExpressionDouble {
public:
  ExpressionEqual(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { if (fabs(av - bv) < 0.001f) return 1.0; else return -1.0; }
};

class ExpressionAnd : public ExpressionDouble {
public:
  ExpressionAnd(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { if (av >= 0.0 && bv >= 0.0) return 1.0; else return -1.0; }
};

class ExpressionOr : public ExpressionDouble {
public:
  ExpressionOr(Expression* _a, Expression* _b) : ExpressionDouble(_a,_b) {}
  double calc(double av, double bv) { if (av >= 0.0 || bv >= 0.0)return 1.0; else return -1.0; }
};

#endif /* __EXPRESSION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8};

