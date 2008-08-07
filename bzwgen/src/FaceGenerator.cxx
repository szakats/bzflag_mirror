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

    // Choose a random edge in the graph, split it into two, and run
    // grow roads on it?
    // ...
    sgraph->readFaces( );

    // pass the faces to subdivision
    graph::FaceVector sfaces = sgraph->getFaces();
    for ( size_t j = 0; j < sfaces.size(); j++ ) {
      subdivideFace( sfaces[j] );
    }
  }
}

void FaceGenerator::subdivideFace( graph::Face* face ) {
  graph::PlanarGraph* sgraph = face->initializeSubgraph( );
  // ...
  sgraph->readFaces( );
  graph::FaceVector sfaces = sgraph->getFaces();
  for ( size_t i = 0; i < sfaces.size(); i++ ) {
    // ...
  }
}

Vector2Df FaceGenerator::deviateVector( const Vector2Df v, double noise ) {
  float theta = float( Random::doubleMax( noise ) * PI * Random::sign() );
  return Vector2Df( cos( theta ) * v.x - sin( theta ) * v.y,
                    sin( theta ) * v.x + cos( theta ) * v.y );
}

void FaceGenerator::growRoads( graph::Node* node, int branching, double segmentLength, double noise ) {
  int branches = math::roundToInt( branching * Random::doubleRange( 1.0f - noise, 1.0f + noise ) );

  // lets get the owner of the node
  graph::PlanarGraph* graph = node->getGraph();

  // Identify the incoming edge and create a vector for it
  // ...

  for ( int i = 0; i < branches; i++ ) {

    Vector2Df direction;

    // initial direction choice, should be away from last

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


    // change the values below to something more sensible (parameters?)
    if ( ndist > 0.1 ) nnode = NULL;
    if ( edist > 0.1 ) nedge = NULL;

    if ( nnode ) {
      // do a connection to a node instead
      // ...
      continue;
    }
    if ( nedge ) {
      // split the edge and do a connection to an edge instead;
      // ...
      continue;
    }

    // TODO: this test must avoid hiting at the edges!
    if ( !graph->checkEdge( node->vector(), target ) ) continue;

    graph::Node * newnode = new graph::Node( graph, target );
    graph->addNode( newnode );
    graph->addConnection( node, newnode );

    // recurrential run
    growRoads( newnode, branching, segmentLength, noise );
  }
}



// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
