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
 * @file PlanarGraph.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an Graph class.
 */

#ifndef __PLANARGRAPH_H__
#define __PLANARGRAPH_H__

#include "graph/forward.h"
#include "graph/Node.h"
#include "graph/Edge.h"
#include "graph/Face.h"
#include "Random.h"

namespace graph {

/**
 * @class PlanarGraph
 * @brief Class defining double linked planar graph.
 */
class PlanarGraph
{
private:
  /** The list of all Node s. */
  NodeList nodeList;
  /** The list of all Edge s. */
  EdgeList edgeList;
  /** The list of all Face s. */
  FaceList faceList;
  /** Holds the number of nodes */
  size_t nodes;
  /** Holds the number of edges */
  size_t edges;
  /** Holds the number of faces */
  size_t faces;
public:
  /**
   * Default constructor. Creates an empty graph.
   */
  PlanarGraph( ) : nodes( 0 ), edges( 0 ), faces( 0 ) {}
  /**
   * Adds a node to the graph. As convenience returns the added node.
   */
  Node* addNode( Node* node ) {
    nodeList.add( node );
    ++nodes;
    return node;
  }
  /**
   * Connects two nodes. Creates two edges and adds them to the graph. Note
   * that no planarity check is done. The arguments are symmetrical, so there's
   * no difference in which one was passed first.
   */
  void addConnection( Node* a, Node* b ) {
    Edge* ea = addEdge( new Edge( a, b ) );
    Edge* eb = addEdge( new Edge( b, a ) );
    ea->setReverse( eb );
  }
  /**
   * Returns the number of nodes.
   */
  size_t nodeCount( ) const {
    return nodes;
  }
  /**
   * Returns the number of edges.
   */
  size_t edgeCount( ) const {
    return edges;
  }
  /**
   * Returns the number of faces.
   */
  size_t faceCount( ) const {
    return faces;
  }
  /**
   * Returns node by ID.
   */
  Node* getNode( size_t id ) const {
    return nodeList.get( id );
  }
  /**
   * Returns edge by ID.
   */
  Edge* getEdge( size_t id ) const {
    return edgeList.get( id );
  }
  /**
   * Removes a given Edge from the graph. Note that it DOES dispose of the
   * edge object. It also disposes of the reversed edge.
   */
  void removeEdge( size_t id ) {
    Edge* edge    = edgeList.get( id );
    Edge* reverse = edge->getReversed();
    size_t rid    = reverse->ID;
    Node* source  = edge->getSource();
    Node* target  = edge->getTarget();
    source->getOutgoingList( ).remove( edge );
    target->getIncomingList( ).remove( edge );
    target->getOutgoingList( ).remove( reverse );
    source->getIncomingList( ).remove( reverse );
    delete edgeList.get( id );
    delete edgeList.get( rid );
    edgeList.clear( id );
    edgeList.clear( rid );
    edges -= 2;
  }
  /**
   * Removes a given Node from the graph. Also removes all
   * connected edges, as they are no longer valid. Note that
   * it DOES dispose of the node object and edge objects.
   */
  void removeNode( size_t id ) {
    Node* node = nodeList.get( id );
    node->orphanize( );
    delete node;
    nodeList.clear( id );
    nodes--;
  }
  /**
   * Checks whether two nodes can be connected with an edge, without
   * breaking planarity.
   */
  bool checkConnection( const Node* a, const Node* b ) const {
    return checkEdge( a->vector(), b->vector() );
  }
  /**
   * Checks whether an added edge would cause a planarity break.
   */
  bool checkEdge( const Vector2Df a, const Vector2Df b ) const {
    for ( size_t i = 0; i < edgeList.size( ); i++ ) {
      Edge* edge = edgeList.get( i );
      if ( edge && !edge->isReversed( ) && edge->intersects( a, b ) ) {
        return false;
      }
    }
    return true;
  }
  /**
   * Splits the edge into two edges, both connected to a newly created
   * Node at the passed coordinates. No planarity checking is done.
   * A pointer to the Node is returned. WARNING, edge is not valid
   * after the split!
   * TODO: may be optimised to reuse the removed edge?
   */
  Node* splitEdge( Edge* edge, Vector2Df point ) {
    Node* source  = edge->getSource();
    Node* target  = edge->getTarget();
    removeEdge( edge->ID );
    Node* node = addNode( new Node( this, point ) );
    addConnection( source, node );
    addConnection( node, target );
    return node;
  }
  /**
   * splitEdge version that takes only the edge and splits it at midpoint.
   * Returns splitpoint node.
   */
  Node* splitEdge( Edge* edge ) {
    Vector2Df splitPoint = ( edge->getSource( )->vector( )
                           + edge->getTarget( )->vector( ) ) / 2.0f;
    return splitEdge( edge, splitPoint );
  }

  /**
   * Reads face information from the graph. The reading is achieved by
   * a x-coordinate sweep calculating the Minimal Cycle Basis. Unless the
   * outer rim of the graph is marked as assigned, or is single linked,
   * the method will also return the bounding (outer) face.
   */
  void readFaces();
  /**
   * Returns the Node closest to the given coordinate. Currently the method
   * just linearly compares the vector to all Nodes in the graph -- maybe
   * later some proper CG algorithm will be implemented. Returns NULL if no
   * Node is present in the graph.
   */
  Node* closestNode( const Vector2Df v );
  /**
   * Returns the Edge closest to the given coordinate. Currently the method
   * just linearly compares the vector to all Nodes in the graph -- maybe
   * later some proper CG algorithm will be implemented. Returns NULL if no
   * Node is present in the graph.
   */
  Edge* closestEdge( const Vector2Df v );
  /**
   * Returns a random edge belonging to the graph. If no edge is present
   * will return NULL.
   */
  Edge* randomEdge( ) {
    if ( edges == 0 ) return NULL;
    Edge* edge = NULL;
    while ( edge == NULL ) {
      edge = edgeList.get( Random::numberMax( edgeList.size( ) ) );
    };
    return edge;
  }
  /**
   * Returns the longest edge belonging to the graph. If no edge is present
   * will return NULL.
   */
  Edge* longestEdge( ) {
    if ( edges == 0 ) return NULL;
    Edge* edge = NULL;
    size_t count = 0;
    while ( edge == NULL ) {
      edge = edgeList.get( count );
      count++;
    }
    float length = edge->length( );
    for ( size_t i = count; i < edgeList.size(); i++ ) {
      if ( edgeList.get( count ) != NULL ) {
        if ( edgeList.get( count )->length( ) > length ) {
          edge = edgeList.get( count );
          length = edge->length( );
        }
      }
    }
    return edge;
  }
  /**
   * Returns the "center" of the graph. In this case it's a average of
   * all the graph's nodes. Returns (0,0) if no nodes are present.
   */
  Vector2Df getCenter( ) const {
    Vector2Df center;
    size_t count = 0;
    for ( size_t i = 0; i < nodeList.size( ); i++ ) {
      Node* node = nodeList.get( i );
      if ( node == NULL ) continue;
      center = center + node->vector( );
      count++;
    }
    if ( count > 0 ) center = center / float( count );
    return center;
  }

  /**
   * Returns a copy of the vector with the list of generated faces.
   */
  inline FaceVector getFaces( ) const {
    return faceList.getCopy( );
  }
  /**
   * Does a slice on the graph using the given coords as points
   * on the slice line. Edges are split at the intersection points
   * and edges are added into the split faces.
   * TODO: fix intersect2D result!
   */
  void slice( Vector2Df a, Vector2Df b );
   /**
    * Removes nodes that are dead ends (either orphaned or one pair of edges).
    */
  size_t removeDeadEnds( );
private:
  /**
   * Extracts the faces assigned to the current node. Works only on
   * edges that don't have a face assigned to them. Extracted face
   * edges are given pointers to the extracted face.
   */
  void extractFaces( Node* node );
  /**
   * Extracts the face assigned with the passed edge. Marks all face's
   * edges to point to the extracted face.
   */
  void extractFace( Edge* edge );
  /**
   * Does a comparison on nodes by their X coordinate. Used by the
   * Minimal Cycle Basis extraction algorithm in readFaces. Second sorting
   * is done via y coordinate.
   */
  static bool compareNodesX ( const Node* a, const Node* b ) {
    return (a->vector().x < b->vector().x || a->vector().y < b->vector().y );
  }
  /**
   * Adds a single edge to the graph. As convenience returns the passed
   * edge.
   */
  Edge* addEdge( Edge* edge ) {
    edgeList.add( edge );
    ++edges;
    return edge;
  }
};


}// end namespace graph


#endif // __PLANARGRAPH_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
