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

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>
#include <fstream>
#include "globals.h"
#include "Face.h"


class Output : public std::ofstream {
public:
  int vertices;
  int texcoords;
  int faces;
  Output(const char* filename) : std::ofstream(filename), vertices(0), texcoords(0), faces(0) {}
  void vertex(Vertex v) { 
    vertices++;
    (*this) << "  vertex " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void vertex(Vertex v, const char* name) { 
    vertices++;
    (*this) << "  " << name << " " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void texcoord(TexCoord tc) { 
    texcoords++;
    (*this) << "  texcoord " << tc.s << " " << tc.t << "\n"; 
  }
  void face(Face* f, int lastmat = -1) { 
    if (!f->output) return;
    faces++;
    (*this) << "  face\n";

    if (lastmat >= 0) {
      if (f->mat != lastmat) matref(f->mat);
    }

    (*this) << "    vertices ";
    for (size_t i = 0; i < f->vtx->size(); i++) {
      (*this) << f->vtx->at(i) << " ";
    }
    (*this) << "\n";

    if (f->texcoords && f->tcd->size() > 0) {
      (*this) << "    texcoords ";
      for (size_t i = 0; i < f->tcd->size(); i++) {
        (*this) << f->tcd->at(i) << " ";
      }
      (*this) << "\n";
    }

    (*this) << "  endface\n";
  }
  void line(const char* textline) { 
    (*this) << textline << "\n";
  }
  void matref(int matref) { 
    (*this) << "  matref mat" << matref << "\n";
  }
  void footer() {
    (*this) << "\n\n# end of world\n";
    (*this) << "# " << vertices <<" vertices\n";
    (*this) << "# " << texcoords <<" texcoords\n";
    (*this) << "# " << faces <<" faces\n\n";
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
