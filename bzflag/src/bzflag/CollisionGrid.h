/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef	__COLLISION_GRID__
#define	__COLLISION_GRID__

#include "common.h"

/* system interface headers */
#include <vector>

/* common interface headers */
#include "WallObstacle.h"
#include "BoxBuilding.h"
#include "PyramidBuilding.h"
#include "BaseBuilding.h"
#include "Teleporter.h"
#include "Obstacle.h"

const int GridSizeX = 32;
const int GridSizeY = 32;

typedef struct {
  int count;
  std::vector<const Obstacle *>        objs;
  float pos[3]; // center of cell
} CollisionCell;

typedef std::vector<const CollisionCell *> CellList;

class CollisionGrid {

  public:
    CollisionGrid();
    ~CollisionGrid();
    
    void load (std::vector<BoxBuilding>     &boxes,
               std::vector<PyramidBuilding> &pyrs,
               std::vector<Teleporter>      &teles,
               std::vector<BaseBuilding>    &bases);
    
    void clear ();

    float getWorldSize () const; // to see if we need to reload
    
    CellList getCells (const float *pos, float radius) const;
    CellList getCells (const float* pos, float angle, float dx, float dy) const;
    CellList getCells (const float* oldPos, float oldAngle,
                       const float* pos, float angle,
                       float dx, float dy) const;
                       
  private:
    CollisionCell Cells[GridSizeX][GridSizeY];
    float WorldSize;
    float Sx;
    float Sy;
    float Hx;
    float Hy;
    float Offx;
    float Offy;
};

#endif /* __COLLISION_GRID__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
