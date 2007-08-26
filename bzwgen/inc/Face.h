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

#ifndef __FACE_H__
#define __FACE_H__

#include "globals.h"
#include "Output.h"

class Face {
public:
  IntVector* vtx;
  IntVector* tcd;
  int mat;
  bool texcoords;
  bool output;
  Face() : mat(0), texcoords(true),output(true) {
    vtx = new IntVector;
    tcd = new IntVector;
  }
  Face(ID4 _vtx, ID4 _tcd, int _mat = 0) : mat(_mat), texcoords(true), output(true) {
    vtx = new IntVector;
    tcd = new IntVector;
    for (int i = 0; i < 4; i++) {
      vtx->push_back(_vtx[i]);
      tcd->push_back(_tcd[i]);
    }
  }
  Face(ID4 _vtx, int _mat = 0) : mat(_mat), texcoords(false), output(true) {
    vtx = new IntVector;
    tcd = new IntVector;
    for (int i = 0; i < 4; i++) {
      vtx->push_back(_vtx[i]);
    }
  }
  void setID4(ID4 id4) {
    vtx->clear();
    for (int i = 0; i < 4; i++) {
      vtx->push_back(id4[i]);
    }
  }
  int addVertex(int v) {
    texcoords = false;
    vtx->push_back(v);
    return vtx->size()-1;
  }
  int addVertex(int v, int tc) {
    vtx->push_back(v);
    tcd->push_back(tc);
    return vtx->size()-1;
  }
  int getVertexIndex(int vid) {
    for (size_t i = 0; i < vtx->size(); i++) if (vtx->at(i) == vid) return i;
    return -1;
  }
  int vertex(int vid) { 
    int size = vtx->size();
    if (vid >= 0) {
      return vtx->at(vid % size);
    } else {
      return vtx->at((vid + size*int(-vid/4+1)) % size);
    }
  }
  void insertVertexAfter(int index,int vid) {
    vtx->insert(vtx->begin()+index+1,vid);
  }
  int size() { return vtx->size(); }
  virtual bool isMultiFace() { return false; }
  virtual ~Face() {
    delete vtx;
    delete tcd;
  }
};

typedef std::vector<Face*> FaceVector;
typedef FaceVector::iterator FaceVectIter;

#endif /* __FACE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
