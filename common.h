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

#ifndef __COMMON_H__
#define __COMMON_H__

#define CELLROAD 1

#define FULLMESH 1

#define EPSILON 0.00000001f

#define WORLDSIZE 400

struct DiscreetMapNode {
  int z;
  int type;
  int zone;
};

struct GridInfo {
  int size;
  int sizeX,sizeY;
  int stepX,stepY;
};

/* temporary */
typedef int Options;

struct Coord2D {
  int x;
  int y;
  Coord2D() { x=0; y=0; };
  Coord2D(int ax, int ay) : x(ax), y(ay) {};
  const Coord2D& operator += (const Coord2D& b) {
    x += b.x;
    y += b.y;
    return *this;
  }
  const Coord2D& operator -= (const Coord2D& b) {
    x -= b.x;
    y -= b.y;
    return *this;
  }	
  bool operator==(const Coord2D& b) {
    return (x == b.x && y == b.y);
  }
  bool operator != (const Coord2D& b) { return !operator==(b); }
};


#endif /* __COMMON_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
