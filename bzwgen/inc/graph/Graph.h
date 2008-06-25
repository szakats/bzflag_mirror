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
 * @file Graph.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Graph class.
*/

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "graph/forward.h"
#include "graph/Node.h"
#include "graph/Edge.h"

namespace graph {

/** 
 * @class Graph
 * @brief Class defining double linked graph. 
 */
class Graph
{
private:
  /** The list of all Nodes. */
  NodeList nodeList;
  /** The list of all Edges. */
  EdgeList edgeList;
  /** Holds the number of nodes */
  size_t nodes;
  /** Holds the number of edges */
  size_t edges;
public:
  /** Default constructor. Creates an empty graph.*/
  Graph() : nodes(0), edges(0) {} 
  /** Adds a node to the graph. As convienience returns the passed edge. */
  virtual NodePtr addNode( NodePtr node ) {
    nodeList.add( node );
    node->setGraph( this );
    ++nodes;
    return node;
  }
  /** Adds a single edge to the graph. As convienience returns the passed edge. */
  virtual EdgePtr addEdge( EdgePtr edge ) {
    edgeList.add( edge );
    ++edges;
    return edge;
  }
  /** Returns the number of nodes */
  size_t nodeCount() const { 
    return nodes;
  }
  /** Returns the number of edges */
  size_t edgeCount() const { 
    return edges;
  }
  /** Returns node by ID */
  NodePtr getNode( size_t id ) {
    return nodeList.get(id);
  }
  /** Returns edge by ID */
  EdgePtr getEdge( size_t id ) {
    return edgeList.get(id);
  }
  /** Removes a given Edge from the graph. Note that it DOES dispose of the edge object. */
  void removeEdge( size_t id ) {
    delete edgeList.get(id);
    edgeList.clear(id);
  }
  /** 
   * Removes a given Node from the graph. Also removes all 
     connected edges, as they are no longer valid. Note that 
     it DOES dispose of the node object and edge objects. */
  void removeNode( size_t id ) {
    NodePtr node = nodeList.get(id);
    node->orphanize();
    delete node;
    nodeList.clear(id);
  }


};


};// end namespace graph


#endif // __GRAPH_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
