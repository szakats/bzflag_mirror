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

#include "Zone.h"

Zone::Zone(int ax, int ay, int bx, int by, int atype, GridInfo gridInfo)
{
  a = Coord2D(ax,ay);
  b = Coord2D(bx,by);
  type = atype;
  wi = gridInfo;
};

void Zone::output(std::ofstream& out) 
{
  Coord2D A = worldA();
  Coord2D B = worldB();
  switch(type) {
    case ZONEBUILD: 
      if (FULLMESH) {
	int height = ((rand()%5)*4+5);
	out << "mesh\n";
	out << "  matref bwall\n";
	out << "  inside " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 1\n";
	out << "  outside " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 100\n";
	out << "  outside " << (A.x)-1 << " " << (A.y)-1 << " 0\n";
	out << "  outside " << (B.x)+1 << " " << (B.y)+1 << " 0\n";
	out << "  vertex " << A.x << " " << A.y << " 0\n";
	out << "  vertex " << B.x << " " << A.y << " 0\n";
	out << "  vertex " << B.x << " " << B.y << " 0\n";
	out << "  vertex " << A.x << " " << B.y << " 0\n";
	out << "  vertex " << A.x << " " << A.y << " " << height << "\n";
	out << "  vertex " << B.x << " " << A.y << " " << height << "\n";
	out << "  vertex " << B.x << " " << B.y << " " << height << "\n";
	out << "  vertex " << A.x << " " << B.y << " " << height << "\n";
	out << "  texcoord 0 0\n";
	out << "  texcoord " << (b.x-a.x+1) << " 0 \n";
	out << "  texcoord " << (b.x-a.x+1) << " " << (b.y-a.y+1) << " \n";
	out << "  texcoord 0 " << (b.y-a.y+1) << " \n";
	out << "  face\n";
	out << "    vertices 0 1 5 4\n";
	out << "  endface\n";
	out << "  face\n";
	out << "    vertices 1 2 6 5\n";
	out << "  endface\n";
	out << "  face\n";
	out << "    vertices 2 3 7 6\n";
	out << "  endface\n";
	out << "  face\n";
	out << "    vertices 3 0 4 7\n";
	out << "  endface\n";
	out << "  face\n";
	out << "    matref top\n";
	out << "    vertices 4 5 6 7\n";
	out << "  endface\n";
	out << "end\n\n";
      } else {
	out << "meshbox\n";
	out << "  position " << int((a.x+b.x-wi.sizeX-1)*wi.stepX/2) << " " << int((a.y+b.y-wi.sizeY-1)*wi.stepY/2) << " 0\n";
	out << "  size " << int((b.x-a.x+1)* wi.stepX/2) << " " << int((b.y-a.y+1)* wi.stepY/2) << " " << ((rand()%5)*4+5) << "\n";
	//out << "  size 10 10 " << (rand()%9+3) << "\n";
	out << "  rotation 0\n";
	out << "end\n\n";
      }
      break;
    case ZONEROADH: 
    case ZONEROADV: 
    case ZONEROADC: 
      out << "mesh\n";
      out << "  noradar\n";
      if (type == ZONEROADC) {
	out << "  matref roadx\n";
      } else {
	out << "  matref road\n";
      }
      out << "  vertex " << A.x << " " << A.y << " 0.001\n";
      out << "  vertex " << B.x << " " << A.y << " 0.001\n";
      out << "  vertex " << B.x << " " << B.y << " 0.001\n";
      out << "  vertex " << A.x << " " << B.y << " 0.001\n";
      out << "  texcoord 0 0\n";
      if (type == ZONEROADH) {
	out << "  texcoord 0 " << (b.x-a.x) << " \n";
	out << "  texcoord " << (b.y-a.y) << " " << (b.x-a.x) << " \n";
	out << "  texcoord " << (b.y-a.y) << " 0 \n";
      } else {
	out << "  texcoord " << (b.x-a.x) << " 0 \n";
	out << "  texcoord " << (b.x-a.x) << " " << (b.y-a.y) << " \n";
	out << "  texcoord 0 " << (b.y-a.y) << " \n";
      }
      out << "  face\n";
      out << "    vertices 0 1 2 3\n";
      out << "    texcoords 0 1 2 3\n";
      out << "    passable\n";
      out << "  endface\n";
      out << "end\n\n";
      break;
  }
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
