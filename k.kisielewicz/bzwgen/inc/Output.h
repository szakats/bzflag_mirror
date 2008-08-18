/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/** 
* @file Output.h
* @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
* @brief Defines a Output class for the generator.
*/

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "globals.h"
#include "Face.h"

/**
 * @class Output
 * @brief Output class for the generator
 *
 * Currently server as an abstraction over the BZW world format. Properly
 * rewritten (or made into an interface) could be used to output to any 
 * kind of output format.
 */
class Output {
  /**
   * The passed outstream to which data will be written.
   */
  OutStream* outstream;
  /**
   * URL to the textures, BZW specific.
   */
  String texturepath;
  /** 
   * Number of written vertices.
   */
  int vertices;
  /** 
   * Number of written texture coordinates.
   */
  int texcoords;
  /** 
   * Number of written faces.
   */
  int faces;
public:
  Output(OutStream* _outstream, String _texturepath) : outstream(_outstream), texturepath(_texturepath ), vertices(0), texcoords(0), faces(0) {}
  void meshStart() { 
    (*outstream) << "mesh\n"; 
  }
  void meshEnd() { 
    (*outstream) << "end\n\n"; 
  }
  void meshPassable() { 
    (*outstream) << " passable\n"; 
  }
  void vertex(Vertex v) { 
    vertices++;
    (*outstream) << "  vertex " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void vertex(Vertex v, const char* name) { 
    vertices++;
    (*outstream) << "  " << name << " " << v.x << " " << v.y << " " << v.z << "\n"; 
  }
  void texCoord(TexCoord tc) { 
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
    for (size_t i = 0; i < f->size(); i++) {
      (*outstream) << f->getVertex( i ) << " ";
    }
    (*outstream) << "\n";

    if (f->hasTexCoords()) {
      (*outstream) << "    texcoords ";
      for (size_t i = 0; i < f->size(); i++) {
        (*outstream) << f->getTexCoord( i ) << " ";
      }
      (*outstream) << "\n";
    }

    (*outstream) << "  endface\n";
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
  void baseZone(Vertex A, Vertex B, int color, bool ctfSafe) { 
    (*outstream) << "base\n";
    (*outstream) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
    (*outstream) << "  size " << fabs(A.x-B.x)/2 << " " << fabs(A.y-B.y)/2 << " 0\n";
    (*outstream) << "  color " << color << "\n";
    (*outstream) << "end\n\n";

    if( ctfSafe ) {
      (*outstream) << "zone\n";
      (*outstream) << "  position " << (A.x+B.x)/2 << " " << (A.y+B.y)/2 << " 0\n";
      (*outstream) << "  size " << fabs(A.x-B.x)/2 << " " << fabs(A.y-B.y)/2 << " 0\n";
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
