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


class Output {
  std::ofstream* file;
public:
  int vertices;
  int texcoords;
  int faces;
  Output(const char* filename) : vertices(0), texcoords(0), faces(0) {
    file = new std::ofstream(filename);
  }
  void vertex(Vertex v) { 
    vertices++;
    (*file) << "  vertex " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void vertex(Vertex v, const char* name) { 
    vertices++;
    (*file) << "  " << name << " " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void texcoord(TexCoord tc) { 
    texcoords++;
    (*file) << "  texcoord " << tc.s << " " << tc.t << "\n"; 
  }
  void face(Face* f, int lastmat = -1) { 
    if (!f->output) return;
    faces++;
    (*file) << "  face\n";

    if (lastmat >= 0) {
      if (f->mat != lastmat) matref(f->mat);
    }

    (*file) << "    vertices ";
    for (size_t i = 0; i < f->vtx->size(); i++) {
      (*file) << f->vtx->at(i) << " ";
    }
    (*file) << "\n";

    if (f->texcoords && f->tcd->size() > 0) {
      (*file) << "    texcoords ";
      for (size_t i = 0; i < f->tcd->size(); i++) {
        (*file) << f->tcd->at(i) << " ";
      }
      (*file) << "\n";
    }

    (*file) << "  endface\n";
  }
  void line(const char* textline) { 
    (*file) << textline << "\n";
  }
  void matref(int matref) { 
    (*file) << "  matref mat" << matref << "\n";
  }
  void footer() {
    (*file) << "\n\n# end of world\n";
    (*file) << "# " << vertices <<" vertices\n";
    (*file) << "# " << texcoords <<" texcoords\n";
    (*file) << "# " << faces <<" faces\n\n";
  }
  void material(int matref, std::string& filename, bool noradar) { 
    (*file) << "material\n";
    (*file) << "  name mat" << matref << "\n";
    (*file) << "  texture " << texturepath << filename;
    if (texturepath.size() > 0) { (*file) << ".png"; }
    (*file)  << "\n";
    if (noradar) (*file) << "  noradar\n";
    (*file) << "end\n\n";
  }
  void basezone(Coord2D A, Coord2D B, int color, bool ctfSafe) { 
    (*file) << "base\n";
    (*file) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
    (*file) << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
    (*file) << "  color " << color << "\n";
    (*file) << "end\n\n";

    if( ctfSafe ) {
      (*file) << "zone\n";
      (*file) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
      (*file) << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
      (*file) << "  safety " << color << "\n";
      (*file) << "end\n\n";
    }
  }
  void header(int size) { 
    (*file) << "world\n";
    (*file) << "  name BZWGen Generated City\n";
    (*file) << "  size " << int(size / 2) << "\n";
    (*file) << "end\n\n";
  }
  void info(int major, int minor, int revision) { 
    (*file) << "#\n# BZWGen (" << major << "." << minor << "." << revision << ") generated map file\n#\n\n";  
  }
  ~Output() {
    delete file;
  }
};

#endif /* __(*file)PUT_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
