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


FloorZone::FloorZone(Coord2D a, Coord2D b, int astep, const std::string& amatref, bool arotated) : Zone(a,b,astep)
{
  rotated = arotated;
  matref = amatref;
}

void FloorZone::output(std::ofstream& out) 
{
  out << "mesh\n";
  out << "  noradar\n";
  out << "  matref " << matref <<"\n";
  out << "  vertex " << A.x << " " << A.y << " 0.001\n";
  out << "  vertex " << B.x << " " << A.y << " 0.001\n";
  out << "  vertex " << B.x << " " << B.y << " 0.001\n";
  out << "  vertex " << A.x << " " << B.y << " 0.001\n";
  out << "  texcoord 0 0\n";
  if (rotated) {
    out << "  texcoord 0 " << int((B.x-A.x)/step) << " \n";
    out << "  texcoord " << int((B.y-A.y)/step) << " " << int((B.x-A.x)/step) << " \n";
    out << "  texcoord " << int((B.y-A.y)/step) << " 0 \n";
  } else {
    out << "  texcoord " << int((B.x-A.x)/step) << " 0 \n";
    out << "  texcoord " << int((B.x-A.x)/step) << " " << int((B.y-A.y)/step) << " \n";
    out << "  texcoord 0 " << int((B.y-A.y)/step) << " \n";
  }
  out << "  face\n";
  out << "    vertices 0 1 2 3\n";
  out << "    texcoords 0 1 2 3\n";
  out << "    passable\n";
  out << "  endface\n";
  out << "end\n\n";
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
