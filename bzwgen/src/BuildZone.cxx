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

#include "BuildZone.h"
#include "Generator.h"

void BuildZone::run() {
  Logger.log( 4, "BuildZone : running at %s...", face->toString().c_str() );
  Mesh* mesh = new Mesh();

  graph::NodeVector nodes = face->getNodes();

  Face* baseFace = new Face();
  baseFace->setMaterial( 0 );
  for ( size_t i = 0; i < nodes.size(); i++ ) {
    baseFace->addVertex( mesh->addVertex( Vertex( nodes[i]->vector().x, nodes[i]->vector().y, 0.0f ) ) );
  }

  int baseFaceID = mesh->addFace( baseFace );

  Logger.log( 4, "BuildZone : running ruleset 'start' rule..." );
  String rulename = String("start");
  // Possible memory leak here?
  meshes = generator->getRuleSet()->run( mesh, baseFaceID, rulename );
  Logger.log( 4, "BuildZone : complete" );
}

void BuildZone::output( Output& out ) {
  if (meshes == NULL) return;
  for ( MeshVectIter itr = meshes->begin(); itr!= meshes->end(); ++itr )
    (*itr)->output( out, generator->getRuleSet()->materialsCount() );
}

BuildZone::~BuildZone( ) {
  deletePointerVector( meshes );
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
