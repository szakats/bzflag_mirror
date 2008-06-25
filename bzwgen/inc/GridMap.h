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

#ifndef __MAP_H__
#define __MAP_H__

#define CELLROAD  1
#define CELLROADX 2
#define CELLBASE  3


#include "globals.h"
#include "Zone.h"
#include "Generator.h"

class GridMap {
public:
  enum CellType {
    NONE,
    ROAD,
    ROADX,
    BASE
  };
private:
  struct DiscreetMapNode {
    CellType type;
    int zone;
  };
  DiscreetMapNode* map;
  Generator* generator;
  int worldSize;
  int gridSize;
  int gridStep;
  ZoneVector zones;
public:
  GridMap() { map = NULL; };
  int getGridSize() const { return gridSize; }
  void initialize(Generator* _generator, int _worldSize, int _gridSize);
  inline DiscreetMapNode getNode(int x, int y)	{ return map[y*gridSize+x]; }
  inline DiscreetMapNode getNode(Coord2D c)  { return map[c.y*gridSize+c.x]; }
  void clear();
  Coord2D worldCoord(int x, int y) { return Coord2D((x-gridSize/2)*gridStep,(y-gridSize/2)*gridStep); }
  void setAreaType(Coord2D a, Coord2D b, CellType type);
  void setAreaZone(Coord2D a, Coord2D b, int zone);
  inline void setType(int x, int y, CellType type) { map[y*gridSize+x].type = type; }
  inline void setZone(int x, int y, int zone) { map[y*gridSize+x].zone = zone; }
  inline void setType(Coord2D c, CellType type) { map[c.y*gridSize+c.x].type = type; }
  void growZone(int x,int y,CellType type);
  void pushZones();
  bool isValid(int x, int y);
  int typeAroundToInt(int x, int y, CellType type);
  int typeCrossAroundToInt(int x, int y, CellType type); 
  void output(Output& out);
  Coord2D emptyCoord();
  ~GridMap();
};

#endif /* __MAP_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
