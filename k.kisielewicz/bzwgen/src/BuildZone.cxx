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

  Face* swface = new Face();
  swface->setMaterial( 0 );
  for (size_t i = 0; i < nodes.size(); i++) {
    swface->addVertex(mesh->addVertex(Vertex(nodes[i]->vector().x,nodes[i]->vector().y,0.0f)));
  }

  int base = mesh->addFace(swface);

  String rulename = String("start");
  meshes = generator->getRuleSet()->run(mesh,base,rulename);
}

void BuildZone::output( Output& out ) {
  if (meshes == NULL) return;
  for (MeshVectIter itr = meshes->begin(); itr!= meshes->end(); ++itr)
    (*itr)->output(out,generator->getRuleSet()->materialsCount());
}

BuildZone::~BuildZone( ) {
  if (meshes == NULL) return;
  for (MeshVectIter itr = meshes->begin(); itr!= meshes->end(); ++itr)
    delete (*itr);
  delete meshes;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
