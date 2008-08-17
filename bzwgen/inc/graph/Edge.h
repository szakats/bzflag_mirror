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
#include "Vector2D.h"
#include "MathUtils.h"
#include "graph/forward.h"
#include "graph/Node.h"


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
   * Pointer to  face that this edge belongs to. Used when reconstructing
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
   * adds the edge to the revel ant nodes, DOESN'T check for
   * reverse Edge however.
   */
  Edge( Node* _source, Node* _target )
    : source( _source ), target( _target ), reverse( NULL ), face( NULL ),
      reversed( false )
  {
    source->addOutgoing( this );
    target->addIncoming( this );
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
   * Returns the face (if assigned) that this edge is part of.
   */
  Face* getFace( ) const {
    return face;
  }
  /**
   * Sets the face that this edge is part of.
   */
  void setFace( Face* _face ) {
    face = _face;
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
   * Edges edges sharing a point do NOT intersect.
   */
  bool intersects( Edge edge ) const {
    return math::intersect2D( source->vector(), target->vector(),
      edge.source->vector(), edge.target->vector(), -0.0001f );
  }
  /**
   * Returns whether a hypothetical edge intersects with this one.
   * Edges edges sharing a point do NOT intersect.
   */
  bool intersects( Vector2Df a, Vector2Df b ) const {
    return math::intersect2D( source->vector(), target->vector(), a, b, -0.0001f );
  }
  /**
   * Returns the reversed status of this edge.
   */
  bool isReversed( ) const {
    return reversed;
  }
  /**
   * Returns the to the given coordinate
   */
  float distanceTo( Vector2Df v ) const {
    Vector2Df a = source->vector();
    Vector2Df b = target->vector();
    Vector2Df d = b - a;
    float ratio = ((v.x - a.x) * d.x + (v.y - a.y) * d.y) / ( d.lengthsq() );
    if ( ratio < 0.0f ) return v.distanceTo( a );
    if ( ratio > 1.0f ) return v.distanceTo( b );
    return v.distanceTo( a.lerp( b, ratio ) );
  }
  /**
   * Returns a point that is perpendicular to the given point and
   * lies on the edge. Note that the point doesn't have to lie on the
   * edge!
   */
  Vector2Df pointCast( Vector2Df v ) const {
    Vector2Df dir = (target->vector() - source->vector()).norm();
    return source->vector() + dir * ( ( v - source->vector() ).dot( dir ) / dir.dot( dir ) );
  }
  /**
   * Returns the length of the edge.
   */
  float length( ) const {
    return ( target->vector() - source->vector() ).length( );
  }
  /**
   * Returns the corresponding reversed edge.
   */
  Edge* getReversed() const {
    return reverse;
  }
  /**
   * Returns a string representation of the Edge.
   */
  std::string toString() const {
    char buffer[80];
    sprintf( buffer, "[#%d,(%.4f,%.4f)->(%.4f,%.4f)]", ID,
      float(source->vector().x), float(source->vector().y),
      float(target->vector().x), float(target->vector().y)
    );
    return std::string( buffer );
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
