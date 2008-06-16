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
  struct ID4 {
    int v[4];
    ID4(int a, int b, int c, int d) { v[0] = a; v[1] = b; v[2] = c; v[3] = d; }
  };
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
      vtx->push_back(_vtx.v[i]);
      tcd->push_back(_tcd.v[i]);
    }
  }
  Face(ID4 _vtx, int _mat = 0) : mat(_mat), texcoords(false), output(true) {
    vtx = new IntVector;
    tcd = new IntVector;
    for (int i = 0; i < 4; i++) {
      vtx->push_back(_vtx.v[i]);
    }
  }
  Face(int a, int b, int c, int d, int _mat = 0) : mat(_mat), texcoords(false), output(true) {
    vtx = new IntVector;
    tcd = new IntVector;
    vtx->push_back(a);
    vtx->push_back(b);
    vtx->push_back(c);
    vtx->push_back(d);
  }
  IntVector* vtxClone() {
    int fsize = size();
    IntVector* copy = new IntVector(fsize);
    for (int i = 0; i < fsize; i++) (*copy)[i] = vtx->at(i);
    return copy;
  }
  void setID4(ID4 id4) {
    vtx->clear();
    for (int i = 0; i < 4; i++) {
      vtx->push_back(id4.v[i]);
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
  bool hasVertex(int vid) {
    for (size_t i = 0; i < vtx->size(); i++) if (vtx->at(i) == vid) return true;
    return false;
  }
  int vertex(int vid) { 
    int size = vtx->size();
    if (vid >= 0) {
      return vtx->at(vid % size);
    } else {
      return vtx->at((vid + size*int(-vid/size+1)) % size);
    }
  }
  void insertVertexAfter(int index,int vid) {
    vtx->insert(vtx->begin()+index+1,vid);
  }
  void insertVertexBefore(int index,int vid) {
    vtx->insert(vtx->begin()+index,vid);
  }
  void removeVertex(int index) {
    vtx->erase(vtx->begin()+index);
  }
  String toStringIDs() { char buffer[80]; sprintf(buffer, "(%d,%d,%d,%d)",vertex(0),vertex(1),vertex(2),vertex(3)); return String(buffer); }
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
