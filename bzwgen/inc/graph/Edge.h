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
 * @file Edge.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Edge class model for the Graph class.
 */

#ifndef __EDGE_H__
#define __EDGE_H__

#include <vector>
#include <cassert>
#include "MathUtils.h"
#include "graph/forward.h"

// The Edge class is a part of Graph class
namespace graph {

/** 
 * @class Edge
 * @brief Class defining an Edge in a double linked planar graph. 
 */
class Edge : public IObject
{
private:
  /** Source node */
  Node* source;
  /** Target node */
  Node* target;
  /** 
   * Pointer to the reverse edge in a PlanarGraph. value is NULL unless 
   * setReverse is called on this or the reverse edge.
   */
  Edge* reverse;
  /** 
   * Pointer to  face that this edge belongs to. Used when reconsructing
   * a face map for the given PlanarGraph.
   */
  Face* face;
  /**
   * A flag that notes if this is a reversed edge. The point is that only
   * one of each pair of reversed edges has this set. This allows to 
   * skip half of the intersection checks.
   */
  bool reversed;
public:
  /** 
   * Constructor that takes source and target as parameters,
   * adds the edge to the revelant nodes, DOESN'T check for
   * reverse Edge however.
   */
  Edge(Node* _source, Node* _target) 
    : source(_source), target(_target), reverse(NULL), face(NULL), reversed(false) 
  {
    source->addOutgoing(this);
    target->addIncoming(this);
  }
  /**
   * Sets the reverse edge. Also sets the reverse edge for the edge passed,
   * so the function needs to be called just once for every edge pair.
   * If debug mode is on then assertions check the validness of the edge. 
   */
  void setReverse( Edge* edge ) {
    assert( source == edge->target );
    assert( target == edge->source );
    reverse = edge;
    edge->reverse = this;
    edge->reversed = true;
  }
  /**
   * Returns source node pointer.
   */
  Node* getSource( ) const {
    return source;
  }
  /**
   * Returns target node pointer.
   */
  Node* getTarget( ) const {
    return target;
  }
  /** 
   * Returns the angle (in radians) between (0,1) vector and the edge.
   * The angle is always positive in the [0..2*PI) range. 
   */
  float getAngle( ) const {
	  Vector2Df v = target->vector() - source->vector();
	  return math::vectorAngle( v.x, v.y );
  }
  /**
   * Returns whether the edge intersects with the one passed.
   */
  bool intersects( Edge edge ) const {
    return math::intersect2D( source->vector(), target->vector(), 
      edge.source->vector(), edge.target->vector() );
  }
  /**
   * Returns whether a hypothetical edge intersects with this one.
   */
  bool intersects( Vector2Df a, Vector2Df b ) const {
    return math::intersect2D( source->vector(), target->vector(), a, b );
  }
  /**
   * Returns the reversed status of this edge.
   */
  bool isReversed( ) const {
    return reversed;
  }
private:
  /** Blocked default constructor */
  Edge( ) {}
  /** Blocked copy constructor */
  Edge( const Edge& ) {}
};


} // namespace end Graph

#endif // __EDGE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
