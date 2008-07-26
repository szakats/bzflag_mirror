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
  createInitialGraph( );
  graph.readFaces( );
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


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
