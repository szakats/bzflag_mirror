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

#include "FaceGenerator.h"
#include "Zone.h"
#include "Random.h"
#include "FloorZone.h"
#include "BaseZone.h"
#include "BuildZone.h"

void FaceGenerator::parseOptions( CCommandLineArgs* opt ) {
  Generator::parseOptions( opt );
}

void FaceGenerator::run( ) {
  runPrimaryRoadGeneration( );
  runSecondaryRoadGeneration( );
  pushZones( );
  Generator::run( );
}

void FaceGenerator::createInitialGraph( ) {
  float hsize = float(size) / 2;
  graph::Node* n1 = graph.addNode( new graph::Node( &graph, -hsize, -hsize ) );
  graph::Node* n2 = graph.addNode( new graph::Node( &graph, hsize,  -hsize ) );
  graph::Node* n3 = graph.addNode( new graph::Node( &graph, hsize,  hsize  ) );
  graph::Node* n4 = graph.addNode( new graph::Node( &graph, -hsize, hsize  ) );

  // if these would be single-linked connections, then we could
  // save the problem of identifying the world face.
  graph.addConnection( n1, n2 );
  graph.addConnection( n2, n3 );
  graph.addConnection( n3, n4 );
  graph.addConnection( n4, n1 );
}

void FaceGenerator::runPrimaryRoadGeneration( ) {
  createInitialGraph( );

  // Split the graph into more or less regular zones (use subdivide face?)
  // ...

  graph.readFaces( );
}

void FaceGenerator::runSecondaryRoadGeneration( ) {
  graph::FaceVector faces = graph.getFaces( );

  // For each face in our graph we do the secondary generation
  for ( size_t i = 0; i < faces.size(); i++ ) {
    graph::PlanarGraph* sgraph = faces[i]->initializeSubgraph( );

    // Maybe use random edge instead?
    graph::Node* splitNode = sgraph->splitEdge( sgraph->longestEdge( ) );

    // This should be parameters, their value is somewhat meaningless now.
    size_t branching = 3;
    float segmentLength = 10.0f;
    float noiseValue = 0.1f;
    float roadThreshold = 0.01f;
    float subdivisionThreshold = 10.0f;
    float faceThreshold = 100.0f;

    // Create initial growing point targeting to the center from
    // the newly created Node at the split edge
    Vector2Df toCenter = ( sgraph->getCenter( ) - splitNode->vector( ) ).norm( );
    Vector2Df newCoord = splitNode->vector( ) + ( toCenter * segmentLength );

    graph::Node* newNode = sgraph->addNode( new graph::Node( sgraph, newCoord ) );
    sgraph->addConnection( splitNode, newNode );

    // Now run the recursive road growing on it.
    growRoads( newNode, branching, segmentLength, noiseValue, roadThreshold );

    sgraph->readFaces( );

    // pass the faces to subdivision
    graph::FaceVector sfaces = sgraph->getFaces();
    for ( size_t j = 0; j < sfaces.size(); j++ ) {
      if ( sfaces[j]->area( ) > faceThreshold )
        subdivideFace( sfaces[j], subdivisionThreshold );
      else
        lots.push_back( sfaces[i] );
    }
  }
}

void FaceGenerator::subdivideFace( graph::Face* face, float threshold ) {
  graph::PlanarGraph* sgraph = face->initializeSubgraph( );

  while ( true ) {
    graph::Edge* longest = sgraph->longestEdge( );
    // change this value to something meaningfull - parameter?
    if ( longest->length( ) < threshold ) break;
    // Current method -- perpendicular slice
    Vector2Df middle = ( longest->getSource( )->vector( ) +
                         longest->getTarget( )->vector( ) ) / 2.0f;
    Vector2Df dir = longest->getSource( )->vector( ) - middle;
    Vector2Df perp = Vector2Df( dir.y, -dir.x );

    sgraph->slice( middle, middle + perp );

    //   do a division of the face with a line perpendicular to the face
    // perpendicuar or parallel?
    // 3 methods
    // TODO: -- either two edges further and do a midpoint connection [easy]
    // TODO: -- perpendicular cast
    // TODO: -- parallel to the next edge cast
  }

  sgraph->readFaces( );
  graph::FaceVector sfaces = sgraph->getFaces();

  // Add the created faces into the list of "buildable" faces
  for ( size_t i = 0; i < sfaces.size(); i++ )
    lots.push_back( sfaces[i] );
}

Vector2Df FaceGenerator::deviateVector( const Vector2Df v, double noise ) {
  float theta = float( Random::doubleMax( noise ) * PI * Random::sign() );
  return Vector2Df( cos( theta ) * v.x - sin( theta ) * v.y,
                    sin( theta ) * v.x + cos( theta ) * v.y );
}

void FaceGenerator::growRoads( graph::Node* node, size_t branching, float segmentLength, float noise, float threshold ) {
  int branches = math::roundToInt( branching * Random::doubleRange( 1.0f - noise, 1.0f + noise ) );

  // lets get the owner of the node
  graph::PlanarGraph* graph = node->getGraph();

  // Identify the incoming edge and create a vector for it
  Vector2Df incoming = node->getFirstIncomingEdge()->getSource()->vector() - node->vector();

  // Single rotation
  float rotationValue = 2 * PI / ( branches + 1 );

  for ( int i = 0; i < branches; i++ ) {

    float rotation = ( i + 1 ) * rotationValue;

    // initial direction choice, should be away from last
    Vector2Df direction = Vector2Df(
      cos( rotation ) * incoming.x - sin ( rotation ) * incoming.y,
      sin( rotation ) * incoming.x - cos ( rotation ) * incoming.y
    );

    direction = deviateVector( direction, noise );
    direction = direction * (float)segmentLength * (float)Random::doubleRange( 1.0 - noise, 1.0 + noise );

    Vector2Df target = node->vector() + direction;

    graph::Node* nnode = graph->closestNode( target );
    graph::Edge* nedge = graph->closestEdge( target );

    float ndist = nnode->distanceTo( target );
    float edist = nedge->distanceTo( target );

    if ( ndist > edist )
      nnode = NULL;
    else
      nedge = NULL;

    if ( ndist > threshold ) nnode = NULL;
    if ( edist > threshold ) nedge = NULL;

    if ( nnode ) {
      if (!graph->checkConnection( node, nnode ) ) continue;
      graph->addConnection( node, nnode );
      continue;
    }
    if ( nedge ) {
      Vector2Df v = nedge->pointCast( target );
      graph::Node* split = graph->splitEdge( nedge, target );
      graph->addConnection( node, split );
      continue;
    }

    if ( !graph->checkEdge( node->vector(), target ) ) continue;

    graph::Node * newnode = new graph::Node( graph, target );
    graph->addNode( newnode );
    graph->addConnection( node, newnode );

    // recurrential run
    growRoads( newnode, branching, segmentLength, noise, threshold );
  }
}

void FaceGenerator::pushZones( ) {
  for ( size_t i = 0; i < lots.size(); i++ ) {
    addZone( new BuildZone( this, lots[i] ) );
  }
}



// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
