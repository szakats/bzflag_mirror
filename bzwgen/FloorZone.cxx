/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
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
#include <iostream>


FloorZone::FloorZone(Coord2D a, Coord2D b, int astep, int matref, bool rotated) : Zone(a,b,astep)
{
  mesh.passable = true;
  mesh.noradar = true;
  if (rotated) {
    mesh.createNewFace(
      Vertex((float)A.x,(float)A.y,0.001f),
      Vertex((float)B.x,(float)A.y,0.001f),
      Vertex((float)B.x,(float)B.y,0.001f),
      Vertex((float)A.x,(float)B.y,0.001f),
      TexCoord(0.0f,0.0f),
      TexCoord(0.0f,float((B.x-A.x)/step)),
      TexCoord(float((B.y-A.y)/step),float((B.x-A.x)/step)),
      TexCoord(float((B.y-A.y)/step),0.0f),
      matref);
  } else {
    mesh.createNewFace(
      Vertex((float)A.x,(float)A.y,0.001f),
      Vertex((float)B.x,(float)A.y,0.001f),
      Vertex((float)B.x,(float)B.y,0.001f),
      Vertex((float)A.x,(float)B.y,0.001f),
      TexCoord(0,0),
      TexCoord(float((B.x-A.x)/step),0.0f),
      TexCoord(float((B.x-A.x)/step),float((B.y-A.y)/step)),
      TexCoord(0,float((B.y-A.y)/step)),
      matref);
  }
}

void FloorZone::output(Output& out) 
{
  mesh.output(out);
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
