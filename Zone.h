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
protected:
  int step;
  Coord2D A,B;
public:
  Zone(Coord2D a, Coord2D b, int astep);
  virtual void output(std::ofstream& out) {};
};

typedef std::vector<Zone*> ZoneVector;
typedef ZoneVector::iterator ZoneVectIter;

#endif /* __ZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
