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
 * @file Plane.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a class representing a plane in space.
 */

#ifndef __PLANE_H__
#define __PLANE_H__

#include "Vector2D.h"

// I'll maybe think later of adding namespaces
//namespace bzwgen {
//namespace geometry {

// Declarations of classes to be implemented:
class Point : public Vector2Df;
class Edge;

/** 
 * @class Plane
 * @brief Class representing a plane in space.
 * 
 * This is a prototype for the class that will be needed to represent the
 * world division and/or road layout. Currently functions are protoyped on 
 * abstract classes, the exact implementation (QuadTree or other Computational
 * Geometry structure) will be decided once I know my exact needs for it.

 */
template <class Point, class Edge>
class Plane
{
  /** X-size of the Plane. Boundaries are size/2 */
  float sizex;
  /** Y-size of the Plane. Boundaries are size/2 */
  float sizey;
public:
  /** Default constructor. Creates an empty planar graph.*/
  Plane( float _sizex, float _sizey) : sizex(_sizex), sizey(_sizey) {} 
  /** Adds a point to the plane */
  void addPoint(Point point);
  /** Returns the point nearest to the coordinates v */
  Point& getNearestPoint(Vector2Df v);
  /** Returns the point nearest to the coordinates v */
  Edge& getNearestEdge(Vector2Df v);
};


//}; // namespace end geometry
//}; // namespace end bzwgen

#endif // __PLANE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
