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
#include "graph/forward.h"

// The Edge class is a part of Graph class
namespace graph {

/** 
 * @class Edge
 * @brief Class defining an Edge in a double linked planar graph. 
 */
class Edge 
{
private:
  /**
   * The ID of the given edge. Should match the index in the owning Graph
   * class.
   */
  int ID;
  /** Source node */
  NodePtr source;
  /** Target node */
  NodePtr target;
  /** Pointer to the reverse edge in a Graph, used for PlanarGraph */
  EdgePtr reverse;
public:
  /** Standard constructor */
  Edge() : ID(-1), source(NULL), target(NULL), reverse(NULL) {}
  /** 
   * Constructor that takes source and target as parameters,
   * adds the edge to the revelant nodes, DOESN'T check for
   * reverse EdgePtr however.
   */
  Edge(NodePtr _source, NodePtr _target) : source(_source), target(_target) {
    source->addOutgoing(this);
    target->addIncoming(this);
  }

};


}; // namespace end Graph

#endif // __EDGE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
