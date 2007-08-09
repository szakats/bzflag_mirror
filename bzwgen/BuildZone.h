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


#ifndef __BUILDZONE_H__
#define __BUILDZONE_H__

#include <fstream>
#include "globals.h"
#include "Zone.h"
#include "Mesh.h"


class BuildZone : public Zone {
  Mesh mesh;
public:
  BuildZone(Coord2D a, Coord2D b, int astep);
  void addDivider(int base, float width, float height, int mat, bool noNext = false);
  void subdivideWindows(int wall, int mat);
  void longerSide(int face, float* length, bool* orientation);
  void generateBuilding(int base, int wall);
  void generateSkyscraper(int base, int wall);
  virtual void output(Output& out);
};

#endif /* __BUILDZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
