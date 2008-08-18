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
 * @file Vector2D.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Two dimensional vector template
 */

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include <string>
#include "MathUtils.h"

// I'll maybe think later of adding name spaces
//namespace bzwgen {
//namespace geometry {

/**
 * @class Vector2D
 * @brief Vector template in two dimensional space.
 *
 * Current implementation is based on templates. Typedefs for both float and
 * double version are provided for convenience.
 */
template <class T>
class Vector2D
{
public:
  /** X-coordinate. */
  T x;
  /** Y-coordinate. */
  T y;

  /** @name Constructors */
  /**
   * Default constructor -- sets the values to zero.
   */
  inline Vector2D( ) : x( 0 ), y( 0 ) { }
  /**
   * Constructor that sets the point to the given coordinates.
   */
  inline Vector2D( T _x, T _y ) : x( _x ), y( _y ) { }
  /**
   * Constructor that sets the point to the given coordinates,
   * given as an array.
   */
  inline Vector2D( const T v[2] ) : x( v[0] ), y( v[1] ) { }
  /**
   * Copy constructor.
   */
  inline Vector2D( const Vector2D<T>& v) : x( v.x ), y( v.y ) { }

  /** @name Operators */
  /**
   * Addition operator.
   */
  Vector2D<T> operator+ ( const Vector2D<T> &v ) const {
    return Vector2D<T>( x + v.x, y + v.y );
  }
  /**
   * Substraction operator.
   */
  Vector2D<T> operator- ( const Vector2D<T> &v ) const {
    return Vector2D<T>( x - v.x, y - v.y );
  }
  /**
   * Multiplication operator.
   */
  Vector2D<T> operator* ( const Vector2D<T> &v ) const {
    return Vector2D<T>( x * v.x, y * v.y );
  }
  /**
   * Scalar multiplication operator.
   */
  Vector2D<T> operator* ( const T f ) const {
    return Vector2D<T>( x * f, y * f );
  }
  /**
   * Division operator.
   */
  Vector2D<T> operator/ ( const Vector2D<T> &v ) const {
    return Vector2D<T>( x / v.x, y / v.y );
  }
  /**
   * Scalar division operator.
   */
  Vector2D<T> operator/ ( const T f ) const {
    return Vector2D<T>( x / f, y / f );
  }
  /**
   * Operator for array-like member access.
   */
  inline T &operator[] ( int i ) const {
    switch ( i ) {
      case 0: return x; break;
      case 1: return y; break;
      default: return x;
    }
  }
  /**
   * Equality operator. Tests structural equality only.
   */
  bool operator== ( const Vector2D<T>& v ) const {
    return ( x == v.x ) && ( y == v.y );
  }
  /**
   * Inequality operator. Tests structural inequality only.
   */
  bool operator!= ( const Vector2D<T>& v ) const {
    return !( ( x == v.x ) && ( y == v.y ) );
  }

  /** @name Other operations */
  /**
   * Does a in-place normalization on the point, as treated as a 2D vector.
   */
  void normalize() {
    T l = length();
    if ( l == 0 ) return;
    x /= l;
    y /= l;
  }
  /**
   * Returns a normalization on the point, as treated as a 2D vector.
   */
  Vector2D<T> norm() const {
    T l = length();
    return (l == 0) ? Vector2D<T>( ) : Vector2D<T>( x/l, y/l );
  }
  /**
   * Returns a string representation of the Point.
   */
  std::string toString() const {
    char buffer[80];
    sprintf( buffer, "[%.4f,%.4f]", float(x), float(y) );
    return std::string( buffer );
  }
  /**
   * Returns the length of the point if treated as a vector.
   */
  T length() const {
    return T( sqrt( x*x + y*y ) );
  }
  /**
   * Returns the squared length of the point if treated as a vector.
   */
  T lengthsq() const {
    return x*x + y*y;
  }

  /**
   * Returns vector cross product.
   */
  T cross( const Vector2D<T> &v ) const {
    return ( x * v.y - v.x * y );
  }
  /**
   * Returns the dot product
   */
  T dot( const Vector2D<T> &v ) const {
      return ( x * v.x + y * v.y );
  }
  /**
   * Returns distance to the other vector
   */
  T distanceTo( const Vector2D<T> &v ) const {
      return ( *this - v ).length();
  }
  /**
   * Lineary interpolates to the given value. Ratio = 0 equals
   * this vector, ratio = 1 the other.
   */
  Vector2D<T> lerp( const Vector2D<T> &v, T ratio ) const {
      return (*this) * ( 1 - ratio ) + v * ratio;
  }
  /**
   * Zero test. Test done using MathUtils isZero.
   */
  bool isZero( const T precision = EPSILON ) const {
    return ( math::isZero( x, precision ) &&
             math::isZero( y, precision ) );
  }
  /**
   * Equality test. Test done using MathUtils equals.
   */
  bool equals( const Vector2D<T>& v, const T precision = EPSILON ) const {
    return ( math::equals( x, v.x, precision ) &&
             math::equals( y, v.y, precision ) );
  }
};

typedef Vector2D<float>  Vector2Df;
typedef Vector2D<double> Vector2Dd;


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __VECTOR2D_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
