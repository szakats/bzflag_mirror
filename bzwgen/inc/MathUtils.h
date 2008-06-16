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
/** @file Math.h
    @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
    @brief Mathematical utility functions
*/

#ifndef __BZMATH_H__
#define __BZMATH_H__

#define _USE_MATH_DEFINES
#include <math.h>

#define EPSILON 0.001

inline int modprev(int x, int mod) { return (x == 0) ? mod-1 : x-1; }
inline int modnext(int x, int mod) { return (x == mod-1) ? 0 : x+1; }

inline int roundToInt(double f) { return int(f+0.5f); }

inline double fsign(double f) { if (f == 0.0) return 0.0; return (f < 0.0) ? -1.0 : 1.0; }

inline double snap(double f,double snapval) { return double(roundToInt(f/snapval))*snapval; }
inline double refinesnap(double oldsnap, double max) { return (max/double(roundToInt(max/oldsnap))); }

inline double minf(double a,double b) { return a < b ? a : b; }
inline double maxf(double a,double b) { return a > b ? a : b; }

inline bool iszero(double f) { return fabs(f) < EPSILON; }

inline bool inrange(double a, double r1, double r2) { 
  return (r2 > r1) ? (a > r1-EPSILON && a < r2+EPSILON) : (a > r2-EPSILON && a < r1+EPSILON); 
}

inline bool commonrange(double a1, double a2, double b1, double b2, double &c1, double &c2) { 
  double mina = minf(a1,a2);
  double maxa = maxf(a1,a2);
  double minb = minf(b1,b2);
  double maxb = maxf(b1,b2);
  if (maxa < minb-EPSILON || mina > maxb+EPSILON) return false;
    c1 = maxf(mina,minb);
    c2 = minf(maxa,maxb);
  return true;
}

template <class T>
inline bool equals(T x, T y, T precision = EPSILON) {
  return fabs(x-y) <= precision;
}


#endif /* __BZMATH_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8