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


#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>
#include <fstream>
#include "common.h"


class Output : public std::ofstream {
public:
  Output(const char* filename) : std::ofstream(filename) {}
  void vertex(float x,float y,float z = 0) { 
    (*this) << "  vertex " << x << " " << y << " " << z << "\n"; 
  }
  void inside(float x,float y,float z = 0) { 
    (*this) << "  inside " << x << " " << y << " " << z << "\n"; 
  }
  void outside(float x,float y,float z = 0) { 
    (*this) << "  outside " << x << " " << y << " " << z << "\n"; 
  }
  void texcoord(float s,float t) { 
    (*this) << "  texcoord " << s << " " << t << "\n"; 
  }
  void face(int a, int b, int c, int d) { 
    (*this) << "  face\n";
    (*this) << "    vertices " << a << " " << b << " " << c << " " << d << "\n"; 
    (*this) << "  endface\n";
  }
  void face(int a, int b, int c, int d, int ta, int tb, int tc, int td) { 
    (*this) << "  face\n";
    (*this) << "    vertices " << a << " " << b << " " << c << " " << d << "\n"; 
    (*this) << "    texcoords " << ta << " " << tb << " " << tc << " " << td << "\n"; 
    (*this) << "  endface\n";
  }
  void line(const char* textline) { 
    (*this) << textline << "\n";
  }
  void matref(const std::string& ref) { 
    (*this) << "  matref " << ref << "\n";
  }
};

#endif /* __OUTPUT_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
