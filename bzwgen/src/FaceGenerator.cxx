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
  Logger.log( 2, "FaceGenerator : running..." );
  runPrimaryRoadGeneration( );
  runSecondaryRoadGeneration( );
  pushZones( );
  Generator::run( );
  Logger.log( 2, "FaceGenerator : run completed." );
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
  Logger.log( 2, "FaceGenerator : primary road generation..." );
  createInitialGraph( );

  // Split the graph into more or less regular zones (use subdivide face?)
  // ...

////////////////////////////////////////
    graph::Node* splitNode = graph.splitEdge( graph.longestEdge( ) );
    Logger.log( 4, "FaceGenerator : splitnode %s", splitNode->toString( ).c_str() );

    // This should be parameters, their value is somewhat meaningless now.
    size_t branching = 3;
    float segmentLength = 400.0f;
    float noiseValue = 0.1f;
    float roadThreshold = 300.0f;

    // Create initial growing point targeting to the center from
    // the newly created Node at the split edge
    Vector2Df toCenter = ( graph.getCenter( ) - splitNode->vector( ) ).norm( );
    Vector2Df newCoord = splitNode->vector( ) + ( toCenter * segmentLength );

    graph::Node* newNode = graph.addNode( new graph::Node( &graph, newCoord ) );
    graph.addConnection( splitNode, newNode );

    Logger.log( 3, "FaceGenerator : primary road generation growing roads..." );
    // Now run the recursive road growing on it.
    growRoads( newNode, branching, segmentLength, noiseValue, roadThreshold );

    size_t rem = graph.removeDeadEnds( );
    Logger.log( 3, "FaceGenerator : removed %d dead ends.", rem );

///////////////////////////////////////

  Logger.log( 2, "FaceGenerator : reading primary faces..." );
  graph.readFaces( );
}

void FaceGenerator::runSecondaryRoadGeneration( ) {
  Logger.log( 2, "FaceGenerator : secondary road generation ( %d faces )...", graph.faceCount( ) );
  graph::FaceVector faces = graph.getFaces( );
  // For each face in our graph we do the secondary generation
  for ( size_t i = /****************/1/**************/; i < faces.size(); i++ ) {
    Logger.log( 3, "FaceGenerator : secondary road generation face #%d...", i );
    Logger.log( 4, "FaceGenerator : face %s...", faces[i]->toString( ).c_str() );
    graph::PlanarGraph* sgraph = faces[i]->initializeSubgraph( );

    // Maybe use random edge instead?
    graph::Node* splitNode = sgraph->splitEdge( sgraph->longestEdge( ) );
    Logger.log( 4, "FaceGenerator : splitnode %s", splitNode->toString( ).c_str() );

    // This should be parameters, their value is somewhat meaningless now.
    size_t branching = 3;
    float segmentLength = 70.0f;
    float noiseValue = 0.06f;
    float roadThreshold = 30.0f;
    float subdivisionThreshold = 10.0f;
    float faceThreshold = 100.0f;

    // Create initial growing point targeting to the center from
    // the newly created Node at the split edge
    Vector2Df toCenter = ( sgraph->getCenter( ) - splitNode->vector( ) ).norm( );
    Vector2Df newCoord = splitNode->vector( ) + ( toCenter * segmentLength );

    graph::Node* newNode = sgraph->addNode( new graph::Node( sgraph, newCoord ) );
    sgraph->addConnection( splitNode, newNode );

    Logger.log( 3, "FaceGenerator : secondary road generation growing roads..." );
    // Now run the recursive road growing on it.
    growRoads( newNode, branching, segmentLength, noiseValue, roadThreshold );
    Logger.log( 3, "FaceGenerator : secondary growing roads complete, %d nodes and %d edges ", sgraph->nodeCount(), sgraph->edgeCount() );
    sgraph->readFaces( );
    Logger.log( 2, "FaceGenerator : secondary run, face #%d - subdivided to %d faces",i,sgraph->faceCount( ));

    size_t rem = sgraph->removeDeadEnds( );
    Logger.log( 3, "FaceGenerator : removed %d dead ends.", rem );

    // pass the faces to subdivision
    graph::FaceVector sfaces = sgraph->getFaces();
    for ( size_t j = 0; j < sfaces.size(); j++ ) {
        assert( sfaces[j] );
        Logger.log( 4, "FaceGenerator : secondary generated face #%s...", sfaces[j]->toString( ).c_str() );
//      if ( sfaces[j]->area( ) > faceThreshold )
//        subdivideFace( sfaces[j], subdivisionThreshold );
//      else
        assert( sfaces[j]->size() > 2 );
        if ( sfaces[j]->size() > 4 && sfaces[j]->size() < 7 && sfaces[j]->isConvex() && sfaces[j]->area( ) > 400.0f ) lots.push_back( sfaces[j] );
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
  Logger.log( 4, "FaceGenerator : subdivision to %d faces", sgraph->faceCount( ));
}

Vector2Df FaceGenerator::deviateVector( const Vector2Df v, double noise ) {
  float theta = float( Random::doubleMax( noise ) * PI * Random::sign() );
  return Vector2Df( cos( theta ) * v.x - sin( theta ) * v.y,
                    sin( theta ) * v.x + cos( theta ) * v.y );
}

void FaceGenerator::growRoads( graph::Node* node, size_t branching,
                               float segmentLength, float noise,
                               float threshold ) {
  Logger.log( 4, "FaceGenerator : grow roads on node #%s..." , node->toString( ).c_str() );
  int branches = math::roundToInt( branching * Random::doubleRange( 1.0f - noise, 1.0f + noise ) );

  // lets get the owner of the node
  graph::PlanarGraph* graph = node->getGraph();

  // Identify the incoming edge and create a vector for it
  Vector2Df incoming = node->getFirstIncomingEdge()->getSource()->vector() - node->vector();

  // Single rotation
  float rotationValue = 2 * PI / ( branches + 1 );

  Logger.log( 4, "FaceGenerator : grow roads incoming %s" , incoming.toString( ).c_str() );
  Logger.log( 4, "FaceGenerator : rotationValue %f" , rotationValue );
  Logger.log( 4, "FaceGenerator : branches %d" , branches );

  for ( int i = 0; i < branches; i++ ) {
    Logger.log( 4, "FaceGenerator : branch %d" , i );

    float rotation = ( i + 1 ) * rotationValue;
    Logger.log( 4, "FaceGenerator : rotation %f" , rotation );

    // initial direction choice, should be away from last
    Vector2Df direction = Vector2Df(
      cos( rotation ) * incoming.x - sin ( rotation ) * incoming.y,
      sin( rotation ) * incoming.x - cos ( rotation ) * incoming.y
    ).norm();
    Logger.log( 4, "FaceGenerator : direction %s" , direction.toString( ).c_str() );

    direction = deviateVector( direction, noise );
    direction = direction * (float)segmentLength * (float)Random::doubleRange( 1.0 - noise, 1.0 + noise );
    //direction = math::precision( direction, 0.1f );

    Vector2Df target = node->vector() + direction;
    Logger.log( 4, "FaceGenerator : target %s->%s" , node->toString( ).c_str() , target.toString( ).c_str() );

    graph::Node* nnode = graph->closestNode( target );
    graph::Edge* nedge = graph->closestEdge( target );

    float ndist = nnode->distanceTo( target );
    float edist = nedge->distanceTo( target );

    if ( ndist > edist+0.01f )
      nnode = NULL;
    else
      nedge = NULL;

    if ( ndist > threshold ) nnode = NULL;
    if ( edist > threshold ) nedge = NULL;

    if ( nnode ) {
      if (!graph->checkConnection( node, nnode ) ) {
        Logger.log( 4, "FaceGenerator : result - node connection fail with %s", nnode->toString( ).c_str() );
        continue;
      }
      graph->addConnection( node, nnode );
      Logger.log( 4, "FaceGenerator : result - new connection with %s", nnode->toString( ).c_str() );
      continue;
    }
    if ( nedge ) {
      Logger.log( 4, "FaceGenerator : edge found %s", nedge->toString( ).c_str() );
      Vector2Df v = nedge->pointCast( target );
      //v = math::precision( v, 0.01f );
      graph::Node* split = graph->splitEdge( nedge, v );
      graph->addConnection( node, split );
      Logger.log( 4, "FaceGenerator : result - split connection with %s", split->toString( ).c_str() );
      continue;
    }

    if ( !graph->checkEdge( node->vector(), target ) ) continue;

    graph::Node * newnode = new graph::Node( graph, target );
    graph->addNode( newnode );
    graph->addConnection( node, newnode );

    Logger.log( 4, "FaceGenerator : result - add and recurrence %s", newnode->toString( ).c_str() );
    // recurrential run
    growRoads( newnode, branching, segmentLength, noise, threshold );
  }
}

void FaceGenerator::pushZones( ) {
  Logger.log( 2, "FaceGenerator : pushing zones (%d)...", lots.size() );
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
