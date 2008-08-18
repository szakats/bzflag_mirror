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

#include "FloorZone.h"
#include "Generator.h"


void FloorZone::run() {
  mesh.setPassable();

  graph::NodeVector nodes = face->getNodes();

  Face* swface = new Face();
  swface->setMaterial( materialID );
  for (size_t i = 0; i < nodes.size(); i++) {
    swface->addVertex( mesh.addVertex( Vertex( nodes[i]->vector().x, nodes[i]->vector().y, 0.001f ) ) );
  }

  double texX = (nodes[2]->vector().x - nodes[0]->vector().x) / step;
  double texY = (nodes[2]->vector().y - nodes[0]->vector().y) / step;

  swface->addTexCoord( mesh.addTexCoord( TexCoord(0.0,0.0) ) );
  swface->addTexCoord( mesh.addTexCoord( rotated ? TexCoord(0.0 ,texX) : TexCoord(texX,0.0) ) );
  swface->addTexCoord( mesh.addTexCoord( rotated ? TexCoord(texY,texX) : TexCoord(texX,texY) ) );
  swface->addTexCoord( mesh.addTexCoord( rotated ? TexCoord(texY,0.0)  : TexCoord(0,texY) ) );

  mesh.addFace(swface);
}


void FloorZone::output(Output& out) 
{
  mesh.output(out,generator->getRuleSet()->materialsCount());
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
