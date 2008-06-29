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
 * @file Edge.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Edge class model for the Graph class.
 */

#ifndef __EDGE_H__
#define __EDGE_H__

#include <vector>
#include <cassert>
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
  NodePtr source;
  /** Target node */
  NodePtr target;
  /** Pointer to the reverse edge in a PlanarGraph */
  EdgePtr reverse;
  /** 
   * Pointer to  face that this edge belongs to. Used when reconsructing
   * a face map for the given PlanarGraph
   */
  FacePtr face;
public:
  /** Standard constructor */
  Edge() : source(NULL), target(NULL), reverse(NULL) {}
  /** 
   * Constructor that takes source and target as parameters,
   * adds the edge to the revelant nodes, DOESN'T check for
   * reverse EdgePtr however.
   */
  Edge(NodePtr _source, NodePtr _target) : source(_source), target(_target) {
    source->addOutgoing(this);
    target->addIncoming(this);
  }
  /**
   * Sets the reverse edge. Also sets the reverse edge for the edge passed,
   * so the function needs to be called justr once for every edge pair.
   * If debug mode is on then assertions check the validness of the edge. 
   */
  void setReverse( EdgePtr edge ) {
    assert( source = edge->target );
    assert( target = edge->source );
    reverse = edge;
    edge->reverse = this;
  }

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
