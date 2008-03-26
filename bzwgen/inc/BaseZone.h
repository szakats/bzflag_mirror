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

#include <vector>
#include <fstream>
#include <string>
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
  void setColor(int _color) { color = _color; }
  virtual void output(Output& out) {
    out << "base\n";
    out << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
    out << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
    out << "  color " << color << "\n";
    out << "end\n\n";

    if( ctfSafe ) {
      out << "zone\n";
      out << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
      out << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
      out << "  safety " << color << "\n";
      out << "end\n\n";
    }

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
