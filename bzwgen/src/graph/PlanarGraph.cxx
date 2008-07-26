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

#include <algorithm>
#include "graph/PlanarGraph.h"

namespace graph {

  void PlanarGraph::readFaces() {

      // Create a sorted list of Nodes by the x coordinate.
      NodeVector xnodes = nodeList.getCopy();
      std::sort( xnodes.begin(), xnodes.end(), compareNodesX );

      // Perform a sweep while reading faces
      for ( size_t i = 0; i < xnodes.size(); i++ ) {
        extractFaces( xnodes[i] );
      }
  }

  void PlanarGraph::extractFaces( Node* node ) {
    Edge* startEdge = node->getFirstOutgoingEdge();
    if ( !startEdge ) return;
    Edge* edge = startEdge;

    // Cycle through the outgoing edges.
    do {
      if ( edge->getFace() == NULL ) {
        extractFace( edge );
      }      
      edge = node->getOutgoingList().next( edge );
    } while ( edge != startEdge );
  }

  void PlanarGraph::extractFace( Edge* edge ) {
    Face* face = new Face( this );
    Edge* startEdge = edge;

    do {
      face->addEdge( edge );
      edge = edge->getTarget()->getOutgoingList().prev( edge );
    } while ( edge != startEdge );

    // check if the face is degenerate. 
    if ( face->size() < 3 ) {
      delete face;
      return;
    }

    faceList.add( face );
    ++faces;
  }

  Node* PlanarGraph::closestNode( const Vector2Df v ) {
    if (nodes == 0) return NULL;

    // get first existing node
    Node* result = NULL;
    {
      size_t i = 0;
      while ( result == NULL ) {
        result = nodeList.get(i);
        i++;
      }
    }
    float distance = result->distanceTo( v );

    for ( size_t i = 0; i < nodeList.size(); i++ ) {
      if ( nodeList.get(i) != NULL ) {
        if (nodeList.get(i)->distanceTo( v ) < distance) {
          result = nodeList.get(i);
          distance = result->distanceTo( v );
        }
      }
    }
    return result;
  }

  Edge* PlanarGraph::closestEdge( const Vector2Df v ) {
    if (edges == 0) return NULL;

    // get first existing node
    Edge* result = NULL;
    {
      size_t i = 0;
      while ( result == NULL ) {
        result = edgeList.get(i);
        i++;
      }
    }
    float distance = result->distanceTo( v );

    for ( size_t i = 0; i < edgeList.size(); i++ ) {
      if (edgeList.get( i ) != NULL) {
        if (edgeList.get( i )->distanceTo( v ) < distance) {
          result = edgeList.get( i );
          distance = result->distanceTo( v );
        }
      }
    }
    return result;
  }

} // end namespace graph

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8