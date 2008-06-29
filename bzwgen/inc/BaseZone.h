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


#ifndef __BASEZONE_H__
#define __BASEZONE_H__

#include "globals.h"
#include "Zone.h"
#include "Mesh.h"


class BaseZone : public Zone {
protected:
  static int color;
  bool ctfSafe;
public:
  BaseZone(Generator* _generator, Coord2D a, Coord2D b, bool _ctfSafe) : Zone(_generator,a,b,0)
  {
    color = 1;
    ctfSafe = _ctfSafe;
  };
  virtual void run() {};
  void setColor(int _color) { color = _color; }
  virtual void output(Output& out) {
    out.baseZone(Vertex(A.x,A.y,0),Vertex(B.x,B.y,0),color,ctfSafe);
    color++;
  }
};

#endif /* __BASEZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
