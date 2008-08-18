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

#include "graph/Face.h"
#include "graph/Edge.h"
#include "graph/PlanarGraph.h"

namespace graph {

PlanarGraph* Face::initializeSubgraph( ) {
  // if the face is degenerate then return NULL
  if ( edges.size() < 3 ) return NULL;

  subgraph = new PlanarGraph( );

  // create the first node
  Node* start = new Node( edges[0]->getSource(), subgraph );
  Node* prev = start;
  subgraph->addNode( prev );

  // add node-edges pairs for each edge except the last
  for ( size_t i = 0; i < edges.size()-1; i++ ) {
    Node* next = new Node( edges[i]->getTarget(), subgraph );
    subgraph->addNode( next );
    subgraph->addConnection( prev, next );
    prev = next;
  }

  // add the last edge that closes the loop
  subgraph->addConnection( prev, start );

  return subgraph;
}

NodeVector Face::getNodes() const {
  NodeVector result;
  for ( EdgeVector::const_iterator itr = edges.begin(); itr!= edges.end(); ++itr ) {
    result.push_back( (*itr)->getSource() );
  }
  return result;
}

size_t Face::addEdge( Edge* edge ) {
  assert( edge );
  edges.push_back( edge );
  edge->setFace( this );
  return edges.size() - 1;
}

Face::~Face( ) {
  if ( subgraph ) delete subgraph;
}

float Face::area( ) const {
  float result = 0.0f;
  for ( size_t i = 0; i < edges.size()-1; i++ ) {
    Vector2Df v1 = edges[i]->getSource()->vector();
    Vector2Df v2 = edges[i+1]->getSource()->vector();
    result += v1.cross( v2 );
  }
  Vector2Df v1 = edges[edges.size()-1]->getSource()->vector();
  Vector2Df v2 = edges[0]->getSource()->vector();
  result += v1.cross( v2 );
  return math::abs( result / 2.0f );
}

std::string Face::toString( ) const {
  std::string buffer = "<";
  for ( size_t i = 0; i < size(); i++ ) {
    buffer += edges[i]->getSource()->toString();
  }
  return buffer + ">";
}

bool Face::isConvex( ) {
  if ( size() < 3 ) return false;
  size_t xch = 0;
  size_t ych = 0;

  Vector2Df a = edges[0]->getSource()->vector() - edges[1]->getSource()->vector();

  for( size_t i =1; i < size()-1; i++ ) {
    Vector2Df b = edges[i]->getSource()->vector() - edges[i+1]->getSource()->vector();
    if ( math::sign( a.x ) != math::sign( b.x ) ) xch++;
    if ( math::sign( a.y ) != math::sign( b.y ) ) ych++;
    a = b;
  }

  Vector2Df b = edges[size()-1]->getSource()->vector() - edges[0]->getSource()->vector();
  if ( math::sign( a.x ) != math::sign( b.x ) ) xch++;
  if ( math::sign( a.y ) != math::sign( b.y ) ) ych++;

  return( xch <= 2 && ych <= 2 );
}

}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
