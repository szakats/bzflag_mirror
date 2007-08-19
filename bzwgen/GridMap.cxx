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

#include "GridMap.h"
#include "FloorZone.h"
#include "BuildZone.h"
#include "GridGenerator.h"
#include <iostream>
#include <string>

void GridMap::initialize(Generator* _generator) 
{
  generator = _generator;
  gi = ((GridGenerator*)_generator)->getGridInfo();
  map = new DiscreetMapNode[gi.sizeX*gi.sizeY];
  clear();
}

void GridMap::clear() 
{
  for (int i = 0; i < gi.sizeX*gi.sizeY; i++) {
    map[i].z = 0;
    map[i].type = 0;
  }
}

void GridMap::output(Output& out) 
{
  for (int x = 0; x < gi.sizeX; x++)
    for (int y = 0; y < gi.sizeY; y++) 
      if ( getNode(x,y).type != 0 ) {
	out << "meshbox\n";
	out << "  position " << (x-int(gi.sizeX / 2))*gi.stepX << " " << (y-int(gi.sizeY / 2))*gi.stepY << " 0\n";
	if (getNode(x,y).z == 0) {
	  out << "  noradar\n";
	  out << "  size 5 5 0\n";
	} else {
	  out << "  size 5 5 " << getNode(x,y).z << "\n";
	}
	out << "  rotation 0\n";
	out << "end\n\n";
      }
}

void GridMap::pushZones() 
{
  int lasty = 0;
  int y = 0;
  do {
    if (getNode(0,y).type == CELLROAD) {
      int lastx = 0;
      for (int x = 0; x < gi.sizeX; x++) {
	if (typeCrossAround(x,y,CELLROAD) > 3) {
    zones.push_back(new BuildZone(generator,worldCoord(lastx,lasty),worldCoord(x-1,y-1),gi.stepX));
	  zones.push_back(new FloorZone(generator,worldCoord(lastx,y-1)  ,worldCoord(x-1,y)  ,gi.stepX, MATROAD, true));
	  zones.push_back(new FloorZone(generator,worldCoord(x-1,lasty)  ,worldCoord(x,y-1)  ,gi.stepX, MATROAD, false));
	  zones.push_back(new FloorZone(generator,worldCoord(x-1,y-1)    ,worldCoord(x,y)    ,gi.stepX, MATROADX, false));
	  lastx = x;
	} else if (x == gi.sizeX-1) {
	  zones.push_back(new BuildZone(generator,worldCoord(lastx,lasty),worldCoord(x-1,y-1),gi.stepX));
	  zones.push_back(new FloorZone(generator,worldCoord(lastx,y-1)  ,worldCoord(x-1,y)  ,gi.stepX, MATROAD, true));
	  lastx = x;
	}
      }
      lasty = y;
    } 
    y++;
  } while (y < gi.sizeY);
}

bool GridMap::isValid(int x, int y) {
  return (x > 0 && y > 0 && x < gi.sizeX && y < gi.sizeY); 
}


int GridMap::typeAround(int x, int y, int type) 
{
  int count = 0;
  for (int xx = x-1; xx <= x+1; xx++) 
    for (int yy = y-1; yy <= y+1; yy++)
      if (isValid(xx,yy) && getNode(xx,yy).type == type) count++;
  return count;
}

int GridMap::typeCrossAround(int x, int y, int type) 
{
  int count = 0;
  if (getNode(x,y).type == type) count++;
  if (isValid(x-1,y) && getNode(x-1,y).type == type) count++;
  if (isValid(x+1,y) && getNode(x+1,y).type == type) count++;
  if (isValid(x,y-1) && getNode(x,y-1).type == type) count++;
  if (isValid(x,y+1) && getNode(x,y+1).type == type) count++;
  return count;
}


Coord2D GridMap::emptyCoord() {
  int x,y;
  do {
    x = randomInt(gi.sizeX);
    y = randomInt(gi.sizeY);
  } while (getNode(x,y).type > 0);
  return Coord2D(x,y);
}

GridMap::~GridMap() {
  ZoneVectIter itr; 
  for (itr = zones.begin(); itr!= zones.end(); ++itr) delete (*itr);
	delete map;
}
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
