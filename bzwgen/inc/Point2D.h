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
/** @file Point2D.h
    @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
    @brief Defines a point in two dimensional space
*/

#ifndef __POINT2D_H__
#define __POINT2D_H__

#include <string>

// I'll maybe think later of adding namespaces
//namespace bzwgen {
//namespace geometry {

/** @class Point2D
    @brief Point in two dimensional space.

    Current implementation is based on double-precision floats, but that
    may be reverted to float once we reach numerical stability. This 
    implementation of a 2D point is written especialy for the PlanarGraph 
    usage, and hence will be optimised for it's use.
*/
class Point2D
{
public:
  /** X-coordinate. */
  double x;
  /** Y-coordinate. */
  double y;
 
  /** /name Constructors */
  /** Default constructor -- sets the values to zero. */
  Point2D() : x(0.0), y(0.0) {}
  /** Constructor that sets the point to the given coordinates. */
  Point2D(double _x, double _y) :x(_x), y(_y) {}
  /** Constructor that sets the point to the given coordinates, given as an array. */
  Point2D(double v[2]) :x(v[0]), y(v[1]) {}
  
  /** /name Operators */
  /** Addition operator. */
  Point2D operator+(const Point2D &v) { return Point2D(x + v.x, y + v.y); }
  /** Substraction operator. */
  Point2D operator-(const Point2D &v) { return Point2D(x - v.x, y - v.y); }
  /** Multiplication operator. */
  Point2D operator*(const Point2D &v) { return Point2D(x * v.x, y * v.y); }
  /** Float multiplication operator. */
  Point2D operator*(const double f) { return Point2D(x * f, y * f); }
  /** Division operator. */
  Point2D operator/(const Point2D &v) { return Point2D(x / v.x, y / v.y); }
  /** Float division operator. */
  Point2D operator/(const double f) { return Point2D(x / f, y / f); }
  /** Operator for array-like member access. */
  double &operator[](int i) {
    switch (i) {
      case 0: return x; break;
      case 1: return y; break;
      default: return x;
    }
  }

  /** /name Other operations */
  /** Does a in-place normalization on the point, as treated as a 2D vector. */
  void normalize() { double l = length(); if (l == 0.0) return; x/=l; y/=l; }
  /** Returns a normalization on the point, as treated as a 2D vector. */
  Point2D norm() { double l = length(); return (l == 0.0) ? Point2D() : Point2D(x/l,y/l); }
  /** Returns a string representation of the Point. */
  std::string toString() { char buffer[80]; sprintf(buffer, "[%.4f,%.4f]",x,y); return std::string(buffer); }
  /** Returns the length of the point if treated as a vector. */
  double length() { return sqrt(x*x + y*y); }
  /** Returns the squared length of the point if treated as a vector. */
  double lengthsq() { return x*x + y*y; }

};


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __POINT2D_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
