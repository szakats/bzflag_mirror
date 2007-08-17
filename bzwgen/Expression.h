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

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <vector>
#include "globals.h"

class Expression {
public:
  virtual float calculate() = 0;
  virtual ~Expression() {};
};

class ExpressionConst {
  float value;
public:
  ExpressionConst(float _value) : value(_value) {};
  float calculate() { return value; };
};

class ExpressionRandom {
  float min, max, step;
public:
  ExpressionRandom(float _min, float _max, float _step) : min(_min), max(_max), step(_step) {};
  float calculate() { 
    if (fabs(step) < 0.0001) return randomFloatRange(min,max);
    return randomFloatRangeStep(min,max,step); 
  };
};

#endif /* __EXPRESSION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8};

