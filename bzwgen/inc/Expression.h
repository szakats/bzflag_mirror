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
  virtual double calculate( Mesh*, int ) = 0;
  virtual ~Expression() {};
};

typedef std::vector<Expression*> ExpressionVector;

template < int SIZE >
class ExpressionTemplate : public Expression {
protected:
  typedef Expression* ExpressionPtr;
  ExpressionPtr exp[SIZE];
  double value[SIZE];
public:
  virtual double calc() = 0;
  double calculate( Mesh* mesh, int face ) {
    for ( int i = 0; i < SIZE; ++i )
      if ( exp[i] )
        value[i] = exp[i]->calculate( mesh, face );
    return calc();
  }
  ~ExpressionTemplate() {
    for ( int i = 0; i < SIZE; ++i )
      deletePointer( exp[i] );
  }
};

class ExpressionSingle : public ExpressionTemplate<1> {
public:
  ExpressionSingle( Expression* exp0 ) {
    exp[0] = exp0;
  };
};

class ExpressionDouble : public ExpressionTemplate<2> {
public:
  ExpressionDouble( Expression* exp0, Expression* exp1 ) {
    exp[0] = exp0;
    exp[1] = exp1;
  };
};

class ExpressionTriple : public ExpressionTemplate<3> {
public:
  ExpressionTriple( Expression* exp0, Expression* exp1, Expression* exp2 ) {
    exp[0] = exp0;
    exp[1] = exp1;
    exp[2] = exp2;
  };
};

class ExpressionConst : public Expression {
  double value;
public:
  ExpressionConst( double _value ) : value( _value ) {};
  double calculate( Mesh*, int ) { return value; };
};

class ExpressionAttribute : public Expression {
  RuleSet* ruleset;
  String attrname;
public:
  ExpressionAttribute( RuleSet* _ruleset, const char* _attrname )
    : ruleset( _ruleset ), attrname( _attrname ) {};
  double calculate( Mesh*, int );
};

class ExpressionFaceAttribute : public Expression {
  String attrname;
public:
  ExpressionFaceAttribute( const char* _attrname ) : attrname( _attrname ) {
    std::transform( attrname.begin(), attrname.end(), attrname.begin(), tolower );
  };
  double calculate( Mesh* mesh, int face );
};

class ExpressionRandom : public ExpressionTriple {
public:
  ExpressionRandom( Expression* min, Expression* max, Expression* step)
    : ExpressionTriple( min, max, step ) { };
  double calc() {
    if (fabs( value[2] ) < 0.0001f)
      return Random::doubleRange( value[0], value[1] );
    return Random::doubleRangeStep( value[0], value[1], value[2] );
  };
};

class ExpressionNeg : public ExpressionSingle {
public:
  ExpressionNeg( Expression* _a ) : ExpressionSingle( _a ) { };
  double calc() {
    return -value[0];
  }
};

class ExpressionRound : public ExpressionSingle {
public:
  ExpressionRound( Expression* _a ) : ExpressionSingle( _a ) { };
  double calc() {
    return math::roundToInt( value[0] );
  }
};

/*
// The following classes could also be generated using this macro :P

#define DOUBLEEXPRESSION(name,operation) \
  class Expression##name : public ExpressionDouble { \
  public: \
    Expression##name ( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { } \
    double calc() { return (operation); } \
  };

DOUBLEEXPRESSION( Add,      value[0] + value[1] )
DOUBLEEXPRESSION( Sub,      value[0] - value[1] )
DOUBLEEXPRESSION( Div,      value[0] / value[1] )
DOUBLEEXPRESSION( Mult,     value[0] * value[1] )
DOUBLEEXPRESSION( Greater,  value[0] > value[1] ? 1.0 : -1.0 )
DOUBLEEXPRESSION( Equal,    math::abs( value[0] - value[1] ) < 0.001f ? 1.0 : -1.0 )
DOUBLEEXPRESSION( And,      ( value[0] >= 0.0 && value[1] >= 0.0 ) ? 1.0 : -1.0 )
DOUBLEEXPRESSION( Or,       ( value[0] >= 0.0 && value[1] >= 0.0 ) ? 1.0 : -1.0 )
*/

class ExpressionAdd : public ExpressionDouble {
public:
  ExpressionAdd( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return value[0] + value[1]; }
};
class ExpressionSub : public ExpressionDouble {
public:
  ExpressionSub( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return value[0] - value[1]; }
};
class ExpressionDiv : public ExpressionDouble {
public:
  ExpressionDiv( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return value[0] / value[1]; }
};
class ExpressionMult : public ExpressionDouble {
public:
  ExpressionMult( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return value[0] * value[1]; }
};

class ExpressionGreater : public ExpressionDouble {
public:
  ExpressionGreater( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return value[0] > value[1] ? 1.0 : -1.0; }
};

class ExpressionEqual : public ExpressionDouble {
public:
  ExpressionEqual( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return math::abs( value[0] - value[1] ) < 0.001f ? 1.0 : -1.0; }
};

class ExpressionAnd : public ExpressionDouble {
public:
  ExpressionAnd( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return ( value[0] >= 0.0 && value[1] >= 0.0 ) ? 1.0 : -1.0; }
};

class ExpressionOr : public ExpressionDouble {
public:
  ExpressionOr( Expression* exp0, Expression* exp1 ) : ExpressionDouble( exp0, exp1 ) { }
  double calc() { return ( value[0] >= 0.0 || value[1] >= 0.0 ) ? 1.0 : -1.0; }
};

#endif /* __EXPRESSION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8};

