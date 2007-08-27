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

#include "FloorZone.h"
#include <iostream>


FloorZone::FloorZone(Generator* _generator, Coord2D a, Coord2D b, int astep, int matref, bool rotated) : Zone(_generator,a,b,astep)
{
  if ((A.x == B.x) || (A.y == B.y)) {
    if (debugLevel > 0) { printf("Bad floor coords! (%d,%d)*(%d,%d)\n",A.x,A.y,B.x,B.y); }
    return;
  }
  mesh.passable = true;

  mesh.createNewFace(
    Vertex((double)A.x,(double)A.y,0.001f), Vertex((double)B.x,(double)A.y,0.001f),
    Vertex((double)B.x,(double)B.y,0.001f), Vertex((double)A.x,(double)B.y,0.001f),
    TexCoord(0.0,0.0),
    rotated ? TexCoord(0.0,double((B.x-A.x)/step))                  : TexCoord(double((B.x-A.x)/step),0.0),
    rotated ? TexCoord(double((B.y-A.y)/step),double((B.x-A.x)/step)) : TexCoord(double((B.x-A.x)/step),double((B.y-A.y)/step)),
    rotated ? TexCoord(double((B.y-A.y)/step),0.0)                  : TexCoord(0,double((B.y-A.y)/step)),
    matref);
}

void FloorZone::output(Output& out) 
{
  mesh.output(out,generator->getRuleSet()->materialsCount()+RESERVEDMATERIALS);
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
