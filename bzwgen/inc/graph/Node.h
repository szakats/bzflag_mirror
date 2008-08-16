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
 * @file Node.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Node class model for the Graph class.
 */

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "Vector2D.h"
#include "graph/forward.h"
#include "graph/SortedEdgeList.h"

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
 */
class Node : public IObject
{
private:
  /**
   * The sorted list of outgoing edges. See SortedEdgeList.
   */
  SortedEdgeList outgoing;
  /**
   * The sorted list of incoming edges. See SortedEdgeList.
   */
  SortedEdgeList incoming;
  /**
   * Pointer to the graph of which this given Node is a belonging to.
   */
  PlanarGraph* graph;
  /**
   * The nodes position on the plane.
   */
  Vector2Df coord;
public:
  /**
   * Constructor. Takes the graph the node belongs to and 2D coordinates
   * as parameters. Does nothing except parameter initialization.
   */
  Node( PlanarGraph* _graph, float x, float y )
    : graph( _graph ), coord( x, y ) {}
  /**
   * Constructor. Takes the graph the node belongs to and vector
   * as parameters. Does nothing except parameter initialization.
   */
  Node( PlanarGraph* _graph, Vector2Df v )
    : graph( _graph ), coord( v ) {}
  /**
   * Copy from pointer constructor.
   */
  Node( Node* node, PlanarGraph* _graph = NULL ) : coord( node->coord ) {
    if (_graph)
      graph = node->graph;
    else
      graph = _graph;
  }
  /**
   * Adds a incoming edge.
   */
  void addIncoming( Edge* edge ) {
    incoming.insert( edge );
  }
  /**
   * Adds a outgoing edge.
   */
  void addOutgoing( Edge* edge ) {
    outgoing.insert( edge );
  }
  /**
   * Removes all the edges connected to the node. Edges are removed from
   * the associated graph class.
   */
  void orphanize( );
  /**
   * Returns itself as a float vector.
   */
  Vector2Df vector( ) const {
    return coord;
  }
  /**
   * Returns the first outgoing edge. First in this case is somewhat
   * meaningless -- the edge is the smallest one in the sense of clockwise
   * comparison to the (0,1) vector.
   */
  Edge* getFirstOutgoingEdge( ) {
    return outgoing.first( );
  }
  /**
   * Returns the first incoming edge.
   */
  Edge* getFirstIncomingEdge( ) {
    return incoming.first( );
  }
  /**
   * Gets the graph pointer of the graph owning this node.
   */
  PlanarGraph* getGraph() const {
    return graph;
  }
  /**
   * Returns a reference to the incoming EdgeList.
   */
  SortedEdgeList& getIncomingList( ) {
    return incoming;
  }
  /**
   * Returns a reference to the outgoing EdgeList.
   */
  SortedEdgeList& getOutgoingList( ) {
    return outgoing;
  }
  /**
   * Returns the distance to the given coord
   */
  float distanceTo( Vector2Df v ) {
    return (coord - v).length();
  }
  /**
   * Returns a string representation of the Node.
   */
  std::string toString() const {
    char buffer[80];
    sprintf( buffer, "[#%d,%.4f,%.4f]", ID, float(coord.x), float(coord.y) );
    return std::string( buffer );
  }
private:
  /**
   * Blocked default constructor.
   */
  Node() {}
  /**
   * Blocked copy constructor.
   */
  Node( const Node& ) {}

};


} // namespace end graph

#endif // __NODE_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
