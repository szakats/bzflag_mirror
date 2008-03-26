/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
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

BuildZone::BuildZone(Generator* _generator, Coord2D a, Coord2D b, int astep) : Zone(_generator,a,b,astep) {
  meshes = NULL;

  if ((A.x == B.x) || (A.y == B.y)) {
    if (debugLevel > 0) { printf("Bad building coords! (%d,%d)*(%d,%d)\n",A.x,A.y,B.x,B.y); }
    return;
  }
  Mesh* mesh = new Mesh();

  Vertex corners[4];
  corners[0] = Vertex((double)A.x,(double)A.y,0.0f);
  corners[1] = Vertex((double)B.x,(double)A.y,0.0f);
  corners[2] = Vertex((double)B.x,(double)B.y,0.0f);
  corners[3] = Vertex((double)A.x,(double)B.y,0.0f);

  Face* swface = new Face();
  swface->mat = 0;
  for (int i = 0; i < 4; i++) {
    swface->addVertex(mesh->addVertex(corners[i]));
  }

  int base = mesh->addFace(swface);

  std::string rulename = std::string("start");
  meshes = generator->getRuleSet()->run(mesh,base,rulename);
  return;
}

void BuildZone::output(Output& out) {
  if (meshes == NULL) return;
  MeshVectIter itr; 
  for (itr = meshes->begin(); itr!= meshes->end(); ++itr) (*itr)->output(out,generator->getRuleSet()->materialsCount());
}

BuildZone::~BuildZone () { 
  if (meshes == NULL) return;
  MeshVectIter itr; 
  for (itr = meshes->begin(); itr!= meshes->end(); ++itr) delete (*itr);
  delete meshes;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
