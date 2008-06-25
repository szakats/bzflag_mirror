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
  int mat;
  bool output;
public:
  IntVector tcd;
  IntVector vtx;
  Face() : mat(0), output(true) {}
  void clear() {
    vtx.clear();
    tcd.clear();
  }
  void clearVertices() {
    vtx.clear();
  }
  void clearTexCoords() {
    tcd.clear();
  }
  void set4(int a, int b, int c, int d) {
    vtx.clear();
    vtx.push_back(a);
    vtx.push_back(b);
    vtx.push_back(c);
    vtx.push_back(d);
  }

  void addVertex(int v) {
    vtx.push_back(v);
  }
  void addVertex(int v, int tc) {
    vtx.push_back(v);
    tcd.push_back(tc);
  }
  void addTexCoord(int tc) {
    tcd.push_back(tc);
  }
  int getVertexIndex(int vid) {
    for (size_t i = 0; i < vtx.size(); i++) 
      if (vtx.at(i) == vid) return i;
    return -1;
  }
  bool hasVertex(int vid) {
    for (size_t i = 0; i < vtx.size(); i++) 
      if (vtx.at(i) == vid) return true;
    return false;
  }
  int vertex(int vid) { 
    int size = vtx.size();
    if (vid >= 0) {
      return vtx.at(vid % size);
    } else {
      return vtx.at((vid + size*int(-vid/size+1)) % size);
    }
  }
  void insertVertexAfter(int index,int vid) {
    vtx.insert(vtx.begin()+index+1,vid);
  }
  void insertVertexBefore(int index,int vid) {
    vtx.insert(vtx.begin()+index,vid);
  }
  void removeVertex(int index) {
    vtx.erase(vtx.begin()+index);
  }
  String toStringIDs() { 
    char buffer[80]; 
    sprintf(buffer, "(%d,%d,%d,%d)",vertex(0),vertex(1),vertex(2),vertex(3)); 
    return String(buffer); 
  }
  int size() { 
    return vtx.size(); 
  }
  void setMaterial( int materialID ) {
    mat = materialID;
  }
  int getMaterial() {
    return mat;
  }
  void setOutput( bool _output ) {
    output = _output;
  }
  bool outputable() {
    return output;
  }
  bool hasTexCoords( ) {
    return !tcd.empty();
  }
  virtual bool isMultiFace() { 
    return false; 
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
