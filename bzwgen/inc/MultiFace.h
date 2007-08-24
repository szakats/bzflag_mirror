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

#ifndef __MULTIFACE_H__
#define __MULTIFACE_H__

#include "globals.h"
#include "Output.h"
#include "Face.h"
#include "Mesh.h"

class MultiFace : public Face {
public:
  FaceVector* comps;
  Mesh* mesh;
  MultiFace(Mesh* _mesh) : Face(), mesh(_mesh) {
    comps = new FaceVector;
  }
  int addFace(Face* f) {
    if (comps->size() == 0) {
      for (size_t i = 0; i < f->vtx->size(); i++) 
        vtx->push_back(f->vtx->at(i));
    } else {
      int size = f->vtx->size();
      bool inside = vertexInside(f->vtx->at(0));
      for (int i = 0; i < size; i++) {
//        addSegment(f->vtx->at(i),f->vtx->at(modnext(i,size)));
      }
    }
    comps->push_back(f);
    return comps->size()-1;
  }
  bool vertexInside(int vid) {
    Vertex A = mesh->v[vid];
    Vertex B = mesh->v[vid];
    B.y = 100000.0f; // sufficient to be out of range
    Vertex P;
    int count = 0;
    int size = vtx->size();
    for (int i = 0; i < size; i++) {
      if (intersectZ(A,B,mesh->v[vtx->at(i)],mesh->v[vtx->at(modnext(i,size))],P)) count++;
    }      
    return (count%2 == 1);
  }
  virtual ~MultiFace() {
    delete comps;
  }
};


#endif /* __MULTIFACE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
