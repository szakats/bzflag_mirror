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
/**
 * @file MathUtils.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Mathematical utility functions
 */

#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#define _USE_MATH_DEFINES
#include <math.h>

#define PI M_PI
#define EPSILON 0.001

template<class T> class Vector2D;

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
 * Returns the absolute value of the passed float/double. The function
 * is templated to be both used with any numeric type.
 */
template <class T>
inline T abs(T f) {
  if (f >= 0) return f;
  return -f;
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
 * Returns the angle (in radians), between (0,1) and (x,y). Contrary to
 * atan2 the result value is always positive, in the [0..2*PI) range.
 * The function is templated to be both used with doubles and floats.
 */
template <class T>
inline T vectorAngle(T x, T y) {
	T at = atan2(y,x);
	if (at < (T)0) at += T(2*M_PI);
  return at;
}


/** @name Precision functions */
/**
 * Function returns whether a given float/double is equal to zero with the
 * given precision. If no precision is given then EPSILON is used. The
 * function is templated to be both used with doubles and floats.
 */
template <class T>
inline bool isZero(T f, T precision = EPSILON) {
  return fabs(f) < precision;
}

/**
 * Function returns whether two given floats/doubles are equal with the
 * given precision. If no precision is given then EPSILON is used. The
 * function is templated to be both used with doubles and floats.
 */
template <class T>
inline bool equals(T x, T y, T precision = EPSILON) {
  return fabs(x-y) <= precision;
}
/**
 * Rounds the floating point nuber to precision p. The
 * function is templated to be both used with doubles and floats.
 */
template <class T>
inline T precision(T f, T p) {
  return T( roundToInt( f / p ) ) * p;
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

/**
 * Checks whether two segments intersect. Negative precision in this case
 * is the amount of "closeness" that is allowed.
 * The reasoning behind this is to allow an intersection test that
 * doesn't say that two edges that share a point intersect.
 */
template <class T>
bool intersect2D( Vector2D<T> A, Vector2D<T> B, Vector2D<T> C, Vector2D<T> D, T precision = EPSILON )
{
  Vector2D<T> AB = B - A;
  Vector2D<T> CD = D - C;
  T d = AB.x*CD.y - AB.y*CD.x;
  T r = (A.y-C.y)*CD.x-(A.x-C.x)*CD.y;
  T s = (A.y-C.y)*AB.x-(A.x-C.x)*AB.y;
  if (isZero(d)) { // parallel
    if (isZero(r)) { // parallel and coincident
      T e;
      T f;
      if (isZero(A.y-C.y) && isZero(B.y-D.y)) { // parallel on X
        return commonRange(A.x,B.x,C.x,D.x,e,f,precision);
      } else { // parallel on Y
        return commonRange(A.y,B.y,C.y,D.y,e,f,precision);
      }
    } else return false; // parallel but not coincident
  }
  r /= d;
  s /= d;
  return (r > 0-precision && r < 1+precision && s > 0-precision && s < 1+precision);
}

/**
 * Checks whether two segments intersect. Result is 0 if there is no
 * intersection, 1 if there is a intersection point, 2 if there is a
 * parallel coincident section.
 *
 * WARNING: I just noted that this code may have a bug, when returning
 * the intersection segment. Only the x or y values of the result are
 * affected, the other value is copied from A. This may be the reason
 * for faulty MultiFace behavior?
 */
template <class T>
int intersect2D( Vector2D<T> A, Vector2D<T> B, Vector2D<T> C, Vector2D<T> D,
                 Vector2D<T>& P1, Vector2D<T>& P2 )
{
  Vector2D<T> AB = B - A;
  Vector2D<T> CD = D - C;
  T d = AB.x*CD.y - AB.y*CD.x;
  T r = (A.y-C.y)*CD.x-(A.x-C.x)*CD.y;
  T s = (A.y-C.y)*AB.x-(A.x-C.x)*AB.y;
  if (isZero(d)) { // parallel
    if (isZero(r)) { // parallel and coincident
      T e;
      T f;
      if (isZero(A.y-C.y) && isZero(B.y-D.y)) { // parallel on X
        if (commonRange(A.x,B.x,C.x,D.x,e,f)) { // AB and CD have common point
          P1 = P2 = A;
          P1.x = e;
          P2.x = f;
          return isZero(e-f) ? 1 : 2;
        } else return 0;
      } else { // parallel on Y
        if (commonRange(A.y,B.y,C.y,D.y,e,f)) { // AB and CD have common point
          P1 = P2 = A;
          P1.y = e;
          P2.y = f;
          return isZero(e-f) ? 1 : 2;
        } else return 0;
      }
    } else return 0; // parallel but not coincident
  }
  r /= d;
  s /= d;
  if (r > 0-EPSILON && r < 1+EPSILON && s > 0-EPSILON && s < 1+EPSILON) {
    P1.x = A.x+r*AB.x;
    P1.y = A.y+r*AB.y;
    return 1;
  } else return 0;
}

/**
 * Rounds the given vector2D to precision p. The
 * function is templated to be both used with doubles and floats.
 */
template <class T>
inline Vector2D<T> precision( Vector2D<T> v, T p ) {
  return Vector2D<T>( precision( v.x, p ), precision( v.y, p ) );
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
