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
 * @file Vector3D.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Three dimensional vector template
 */

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <string>
#include "MathUtils.h"
#include "Vector2D.h"


// I'll maybe think later of adding namespaces
//namespace bzwgen {
//namespace geometry {

/**
 * @class Vector3D
 * @brief Vector template in three dimensional space.
 *
 * Current implementation is based on templates. Typedefs for both float and
 * double version are provided for convieniece.
 */
template <class T>
class Vector3D {
public:
  /** X-coordinate. */
  T x;
  /** Y-coordinate. */
  T y;
  /** Z-coordinate. */
  T z;

  /** @name Constructors */
  /** Default constructor -- sets the values to zero. */
  Vector3D() : x(0), y(0), z(0) {}
  /** Constructor that sets the vector to the given coordinates. */
  Vector3D(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {}
  /** Constructor that sets the vector to the given array coordinates. */
  Vector3D(T v[3]) :x(v[0]), y(v[1]), z(v[2]) {}
  /** Copy constructor. */
  Vector3D(const Vector3D<T>& v) :x(v.x), y(v.y), z(v.z) {}

  /** @name Operators */
  /** Addition operator. */
  Vector3D<T> operator+(const Vector3D<T> &v) const { return Vector3D<T>(x + v.x, y + v.y, z + v.z); }
  /** Substraction operator. */
  Vector3D<T> operator-(const Vector3D<T> &v) const { return Vector3D<T>(x - v.x, y - v.y, z - v.z); }
  /** Multiplication operator. */
  Vector3D<T> operator*(const Vector3D<T> &v) const { return Vector3D<T>(x * v.x, y * v.y, z * v.z); }
  /** Scalar multiplication operator. */
  Vector3D<T> operator*(const T f) const { return Vector3D<T>(x * f, y * f, z * f); }
  /** Division operator. */
  Vector3D<T> operator/(const Vector3D<T> &v) const { return Vector3D<T>(x / v.x, y / v.y, z / v.z); }
  /** Scalar division operator. */
  Vector3D<T> operator/(const T f) const { return Vector3D<T>(x / f, y / f, z / f); }
  /** Operator for array-like access. */
  T &operator[](int i) const {
    switch (i) {
      case 0: return x; break;
      case 1: return y; break;
      case 2: return z; break;
      default: return x;
    }
  }
  /** Equality operator. Tests structural equality only. */
  bool operator==(const Vector3D<T>& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
  /** Inequality operator. Tests structural inequality only. */
  bool operator!=(const Vector3D<T>& v) const { return !((x == v.x) && (y == v.y) && (z == v.z)); }

  /** @name Other operations */
  /** Vector normalization in-place. */
  void normalize() { T l = length(); if (l == 0) return; x/=l; y/=l; z/=l; }
  /** Returns vector normal. */
  Vector3D<T> norm() const { T l = length(); return (l == 0) ? Vector3D<T>() : Vector3D<T>(x/l,y/l,z/l); }

  /** Converts vector to string representation. */
  String toString() const { char buffer[80]; sprintf(buffer, "[%.4f,%.4f,%.4f]",float(x),float(y),float(z)); return String(buffer); }

  /** Returns vector cross product. */
  Vector3D<T> cross(const Vector3D<T> &v) const {
    return Vector3D<T>(
      y * v.z - v.y * z,
      v.x * z - x * v.z,
      x * v.y - v.x * y
    );
  }

  /** Returns vector dot product. */
  T dot(const Vector3D<T> &v) const { return x*v.x + y*v.y + z*v.z; }

  /** Returns vector length. */
  T length() const { return T(sqrt(x*x + y*y + z*z)); }
  /** Returns vector length, squared. */
  T lengthsq() const { return x*x + y*y + z*z; }

  /** Sets the vector to the given coords in an array. */
  void set(T v[3]) { x = v[0], y = v[1], z = v[2]; }
  /** Sets the vector to the given coords. */
  void set(T _x, T _y, T _z) { x = _x, y = _y, z = _z; }

  /** Returns a 2D vector that shares this vectors x,y */
  Vector2D<T> toVector2D ( ) const {
    return Vector2D<T>(x,y);
  }

  /** Zero test. Test done using MathUtils isZero. */
  bool isZero( const T precision = EPSILON ) const { 
    return ( math::isZero( x, precision ) && 
             math::isZero( y, precision ) && 
             math::isZero( z, precision ) ); 
  }
  /** Equality test. Test done using MathUtils equals. */
  bool equals( const Vector3D<T>& v, const T precision = EPSILON ) const { 
    return ( math::equals( x, v.x, precision ) && 
             math::equals( y, v.y, precision ) && 
             math::equals( z, v.z, precision ) ); 
  }

};

typedef Vector3D<float>  Vector3Df;
typedef Vector3D<double> Vector3Dd;


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __VECTOR3D_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
