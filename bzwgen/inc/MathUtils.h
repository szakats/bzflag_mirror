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
/** 
 * @file MathUtils.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Mathematical utility functions
 */

#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#define _USE_MATH_DEFINES
#include <math.h>

#define EPSILON 0.001

namespace math {

/** @name General utility functions */
/** 
 * Returns the sign of the passed float/double. Contrary to other 
 * functions implemented here, this one doesn't rely on precision.
 * The function is templated to be both used with doubles and floats. 
 */
template <class T>
inline T sign(T f) { 
  if (f == 0.0) return 0.0; 
  return (f < 0.0) ? -1.0 : 1.0; 
}

/** 
 * Returns the smaller of the two values. Thanks to templating may be 
 * used on any numeric type. 
 */
template <class T>
inline T min(T a,T b) { 
  return a < b ? a : b; 
}

/** 
 * Returns the larger of the two values. Thanks to templating may be 
 * used on any numeric type. 
 */
template <class T>
inline T max(T a,T b) { 
  return a > b ? a : b; 
}

/** 
 * Rounds a float/double to the nearest integer. Contrary to the usual 
 * round, the result is an int. The function is templated to be both 
 * used with doubles and floats. 
 */
template <class T>
inline int roundToInt(T f) { 
  return int(f+0.5f); 
}

/** 
 * Returns the angle (in radians), between (1,0) and (x,y). Contrary to
 * atan2 the result value is always positive, in the [0..2*PI) range.
 * The function is templated to be both used with doubles and floats. 
 */
template <class T>
inline T vectorAngle(T x, T y) { 
	T at = atan2(y,x);
	if (at < (T)0) at += T(2*M_PI); 
}


/** @name Precision functions */
/** 
 * Function returns wether a given float/double is equal to zero with the 
 * given precision. If no precision is given then EPSILON is used. The 
 * function is templated to be both used with doubles and floats. 
 */
template <class T>
inline bool isZero(T f, T precision = EPSILON) { 
  return fabs(f) < precision; 
}

/** 
 * Function returns wether two given floats/doubles are equal with the 
 * given precision. If no precision is given then EPSILON is used. The 
 * function is templated to be both used with doubles and floats. 
 */
template <class T>
inline bool equals(T x, T y, T precision = EPSILON) {
  return fabs(x-y) <= precision;
}

/** @name Circular space functions */
/** 
 * Decrementing in a circular space -- returns x-1, unless x is zero 
 * then it returns mod-1. 
 */
inline int modPrev(int x, int mod) { 
  return (x == 0) ? mod-1 : x-1; 
}

/** 
 * Incrementing in a circular space -- returns x+1, unless x is mod-1 
 * then it returns 0. 
 */
inline int modNext(int x, int mod) { 
  return (x == mod-1) ? 0 : x+1; 
}

/** @name Grid snap functions */
/** 
 * Rounds the passed double/float to the nearest multiple of snapval.
 * The function is templated to be both used with doubles and floats. 
 */
template <class T>
inline T snap(T f,T snapval) { 
  return T(roundToInt(f/snapval))*snapval; 
}

/**
 * Refines the given snap value. The effect is that the returned snap 
 * value will be rounded to the nearest number that is a "divisor" of
 * maxval. The function is templated to be both used with doubles and 
 * floats. 
 */
template <class T>
inline T refineSnap(T oldsnap, T maxval) { 
  return (maxval/T(roundToInt(maxval/oldsnap))); 
}

/** @name Range handling functions */
/** 
 * Returns true if x is in [r1..r2]. A precision may be optionally passed,
 * if not, EPSILON is assumed. Function is templated to work with both
 * floats and doubles. 
 */
template <class T>
inline bool inRange(T x, T r1, T r2, T precision = EPSILON) { 
  return (r2 > r1) ? (x > r1-precision && x < r2+precision) : (x > r2-precision && x < r1+precision); 
}

/** 
 * Returns true if a1..a2 and b1..b2 ranges have a common part. The part is 
 * returned as result1..result2. A precision may be optionally passed,
 * if not, EPSILON is assumed. Function is templated to work with both
 * floats and doubles. 
 */
template <class T>
inline bool commonRange(T a1, T a2, T b1, T b2, T &result1, T &result2, T precision = EPSILON) { 
  T mina = min(a1,a2);
  T maxa = max(a1,a2);
  T minb = min(b1,b2);
  T maxb = max(b1,b2);
  if (maxa < minb-precision || mina > maxb+precision) 
    return false;
  result1 = max(mina,minb);
  result2 = min(maxa,maxb);
  return true;
}

}

#endif /* __MATHUTILS_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
