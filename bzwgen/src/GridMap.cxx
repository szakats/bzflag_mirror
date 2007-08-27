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

#include "GridMap.h"
#include "FloorZone.h"
#include "BaseZone.h"
#include "BuildZone.h"
#include "GridGenerator.h"
#include <iostream>
#include <string>

int BaseZone::color = 1;

void GridMap::initialize(Generator* _generator) 
{
  generator = _generator;
  gi = ((GridGenerator*)_generator)->getGridInfo();
  map = new DiscreetMapNode[(gi.sizeX+1)*(gi.sizeY+1)];
  clear();
}

void GridMap::clear() 
{
  for (int i = 0; i < (gi.sizeX+1)*(gi.sizeY+1); i++) {
    map[i].zone = -1;
    map[i].type = 0;
  }
}

void GridMap::growZone(int x,int y,int type) {
  if (debugLevel > 3) { printf("Pushing zone at : (%d,%d)\n",x,y); }
  int xe = x;
  int ye = y;
  while(xe < gi.sizeX) {
    xe++;
    int etype = getNode(xe,y).type;
    if (etype != type) break;
  }
  while(ye < gi.sizeY) {
    ye++;
    int etype = getNode(x,ye).type;
    if (etype != type) break;
  }

  setAreaZone(Coord2D(x,xe),Coord2D(y,ye),zones.size());

  if (type == CELLROAD) {
    if (debugLevel > 2) { printf("Road zone added : (%d,%d * %d,%d)\n",x,y,xe,ye); }
    zones.push_back(new FloorZone(generator,worldCoord(x,y)  ,worldCoord(xe,ye)  ,gi.stepX, MATROAD, x-xe < y-ye));
  } else if (type == CELLROADX) {
    if (debugLevel > 2) { printf("Crossroads zone added : (%d,%d * %d,%d)\n",x,y,xe,ye); }
    zones.push_back(new FloorZone(generator,worldCoord(x,y)  ,worldCoord(xe,ye)  ,gi.stepX, MATROADX,true));
  } else if (type == CELLBASE) {
    if (debugLevel > 2) { printf("Base zone added : (%d,%d * %d,%d)\n",x,y,xe,ye); }
    zones.push_back(new BaseZone(generator,worldCoord(x,y)  ,worldCoord(xe,ye), generator->ctfSafe));
  } else {
    if (debugLevel > 2) { printf("Building zone added : (%d,%d * %d,%d)\n",x,y,xe,ye); }
    zones.push_back(new BuildZone(generator,worldCoord(x,y)  ,worldCoord(xe,ye)  ,gi.stepX));
  }
  if (debugLevel > 3) { printf("Zone successfuly created : (%d,%d * %d,%d)\n",x,y,xe,ye); }
}

void GridMap::setAreaType(Coord2D a, Coord2D b, int type) {
  for (int xx = a.x; xx < b.x; xx++) 
    for (int yy = a.y; yy < b.y; yy++) 
      settype(xx,yy,type);
}

void GridMap::setAreaZone(Coord2D a, Coord2D b, int zone) {
  for (int xx = a.x; xx < b.x; xx++) 
    for (int yy = a.y; yy < b.y; yy++) 
      setzone(xx,yy,zone);
}


void GridMap::pushZones() 
{
  int y = 0;
  int x = 0;
  do {
    x = 0;
    do {
      if (getNode(x,y).zone == -1) {
        growZone(x,y,getNode(x,y).type);
      }
      x++;
    } while (x < gi.sizeX);
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
