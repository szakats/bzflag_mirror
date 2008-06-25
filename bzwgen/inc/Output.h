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

#include "globals.h"
#include "Face.h"


class Output {
  OutStream* outstream;
  String texturepath;
public:
  int vertices;
  int texcoords;
  int faces;
  Output(OutStream* _outstream, String _texturepath) : outstream(_outstream), texturepath(_texturepath ), vertices(0), texcoords(0), faces(0) {}
  void vertex(Vertex v) { 
    vertices++;
    (*outstream) << "  vertex " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void vertex(Vertex v, const char* name) { 
    vertices++;
    (*outstream) << "  " << name << " " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void texcoord(TexCoord tc) { 
    texcoords++;
    (*outstream) << "  texcoord " << tc.x << " " << tc.y << "\n"; 
  }
  void face(Face* f, int lastmat = -1) { 
    if (!f->outputable()) return;
    faces++;
    (*outstream) << "  face\n";

    if (lastmat >= 0) {
      if (f->getMaterial() != lastmat) matref(f->getMaterial());
    }

    (*outstream) << "    vertices ";
    for (size_t i = 0; i < f->vtx.size(); i++) {
      (*outstream) << f->vtx.at(i) << " ";
    }
    (*outstream) << "\n";

    if (f->hasTexCoords()) {
      (*outstream) << "    texcoords ";
      for (size_t i = 0; i < f->tcd.size(); i++) {
        (*outstream) << f->tcd.at(i) << " ";
      }
      (*outstream) << "\n";
    }

    (*outstream) << "  endface\n";
  }
  void line(const char* textline) { 
    (*outstream) << textline << "\n";
  }
  void matref(int matref) { 
    (*outstream) << "  matref mat" << matref << "\n";
  }
  void footer() {
    (*outstream) << "\n\n# end of world\n";
    (*outstream) << "# " << vertices <<" vertices\n";
    (*outstream) << "# " << texcoords <<" texcoords\n";
    (*outstream) << "# " << faces <<" faces\n\n";
  }
  void material(int matref, String& filename, bool noradar) { 
    (*outstream) << "material\n";
    (*outstream) << "  name mat" << matref << "\n";
    (*outstream) << "  texture " << texturepath << filename;
    if (texturepath.size() > 0) { (*outstream) << ".png"; }
    (*outstream)  << "\n";
    if (noradar) (*outstream) << "  noradar\n";
    (*outstream) << "end\n\n";
  }
  void basezone(Coord2D A, Coord2D B, int color, bool ctfSafe) { 
    (*outstream) << "base\n";
    (*outstream) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
    (*outstream) << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
    (*outstream) << "  color " << color << "\n";
    (*outstream) << "end\n\n";

    if( ctfSafe ) {
      (*outstream) << "zone\n";
      (*outstream) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
      (*outstream) << "  size " << abs(A.x-B.x)/2 << " " << abs(A.y-B.y)/2 << " 0\n";
      (*outstream) << "  safety " << color << "\n";
      (*outstream) << "end\n\n";
    }
  }
  void header(int size) { 
    (*outstream) << "world\n";
    (*outstream) << "  name BZWGen Generated City\n";
    (*outstream) << "  size " << int(size / 2) << "\n";
    (*outstream) << "end\n\n";
  }
  void info(int major, int minor, int revision) { 
    (*outstream) << "#\n# BZWGen (" << major << "." << minor << "." << revision << ") generated map file\n#\n\n";  
  }
};

#endif /* __(*outstream)PUT_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
