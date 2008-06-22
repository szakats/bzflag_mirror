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
 * @file Node.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Node class model for the Graph class.
 */

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "Vector2D.h"
#include "graph/forward.h"

// The Node class is a part of Graph class
namespace graph {

/** 
 * @class Node
 * @brief Class defining an node in a double linked planar graph. 
 * 
 * Due to the fact that the Graph is dual linked, each Node holds a 
 * set of pointers to other Nodes. The initial implementation uses vector
 * for holding the GraphNode pointers, but that probably will be changed to
 * sets in the future. Also, currently I use pointers, but as there will be heavy 
 * work on node addition, and maybe removal, I'll switch to boost::weak_ptr later.
 * 
 * The graph node inherits Vector2D<float>, cause the main definition of it's existence is 
 * it's place on the 2D planar graph.
 */
class Node : public Vector2Df
{
private:
  /**
   * The "ID" of the Node, which is it's index in the Graph NodeList.
   * Later this should be templatized from a common class for ID'd
   * vector handling, to prevent the ID's from getting invalid.
   */
  int ID;
  /** The list of outgoing edges */
  EdgeList outgoing;
  /** The list of incoming edges */
  EdgeList incoming;
  /**
   * Pointer to the graph of which this given Node is a belonging to.
   */
  GraphPtr graph;
public:
  /** 
   * Default constructor. Sets the underlying point to (0,0) and initializes
   * an empty list of connections.
   */
  Node() : graph(0), ID(-1) {} 
  /** Constructor which takes Point2D data as parameters. */
  Node(float x, float y) : Vector2Df(x,y), graph(0), ID(-1)  {} 
  /** Adds a incoming edge */
  void addIncoming(EdgePtr edge) { incoming.push_back(edge); }
  /** Adds a outgoing edge */
  void addOutgoing(EdgePtr edge) { outgoing.push_back(edge); }

};


}; // namespace end graph

#endif // __NODE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
