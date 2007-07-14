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


#ifndef __ZONE_H__
#define __ZONE_H__

#include <vector>
#include <fstream>
#include "common.h"


class Zone {
  GridInfo wi;
  Coord2D a,b;
  int type;
public:
  Zone(int ax, int ay, int bx, int by, int atype, GridInfo gridInfo);
  Coord2D worldA() { 
    return Coord2D((a.x-wi.sizeX/2)*wi.stepX,(a.y-wi.sizeY/2)*wi.stepY);
  }
  Coord2D worldB() { 
    return Coord2D((b.x-wi.sizeX/2)*wi.stepX,(b.y-wi.sizeY/2)*wi.stepY);
  }
  void output(std::ofstream& out);
};

typedef std::vector<Zone> ZoneVector;
typedef ZoneVector::iterator ZoneVectIter;

#endif /* __ZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
