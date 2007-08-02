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
  mesh.matref = matref;

  mesh.passable = true;
  mesh.noradar = true;
  if (rotated) {
    mesh.createNewFace(
      Vertex(A.x,A.y,0.001),
      Vertex(B.x,A.y,0.001),
      Vertex(B.x,B.y,0.001),
      Vertex(A.x,B.y,0.001),
      TexCoord(0,0),
      TexCoord(0,int((B.x-A.x)/step)),
      TexCoord(int((B.y-A.y)/step),int((B.x-A.x)/step)),
      TexCoord(int((B.y-A.y)/step),0),
      matref);
  } else {
    mesh.createNewFace(
      Vertex(A.x,A.y,0.001),
      Vertex(B.x,A.y,0.001),
      Vertex(B.x,B.y,0.001),
      Vertex(A.x,B.y,0.001),
      TexCoord(0,0),
      TexCoord(int((B.x-A.x)/step),0),
      TexCoord(int((B.x-A.x)/step),int((B.y-A.y)/step)),
      TexCoord(0,int((B.y-A.y)/step)),
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
