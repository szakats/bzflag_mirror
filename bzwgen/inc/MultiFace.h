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
  void refineFace(Face* f) {
    int i = 0;
    while (i < int(f->vtx->size())) {
      Vertex ipoint;
      int index;
      printf("Intersect... (%d,%d)\n",i,modnext(i,f->vtx->size()));
      if (vertexNearestIntersect(f->vtx->at(i),f->vtx->at(modnext(i,f->vtx->size())),ipoint,index)) {
        printf("Nearerst found... (%d) : ",index);
        int ipid;
        if (samepointZ(ipoint,mesh->v[modnext(i,f->vtx->size())])) {
          printf("Is samepoint with next\n",index);
          vtx->insert(vtx->begin()+index+1,modnext(i,f->vtx->size()));
        } else if (samepointZ(ipoint,mesh->v[vtx->at(index)])) {
          printf("Is samepoint with itself\n",index);
          f->vtx->insert(f->vtx->begin()+i+1,vtx->at(index));
        } else {
          printf("Is normal\n",index);
          ipid = mesh->addVertex(ipoint);
          f->vtx->insert(f->vtx->begin()+i+1,ipid);
          vtx->insert(vtx->begin()+index+1,ipid);
        }
      }
      i++;
    }
  }
  int addFace(Face* f) {
    printf("Addface start... (%d,%d)\n",vtx->size(),f->vtx->size());
    f->output = false;
    if (comps->size() == 0) {
      for (size_t i = 0; i < f->vtx->size(); i++) 
        vtx->push_back(f->vtx->at(i));
    } else {
      refineFace(f);
      printf("Refined... (%d,%d)\n",vtx->size(),f->vtx->size());
      IntVector* newvtx = new IntVector;
      int fsize = f->vtx->size();
      int size = vtx->size();
      int index = 0;
      for (index = 0; index < fsize; index++) 
        if (getVertexIndex(f->vtx->at(index)) < 0 && !vertexInside(f->vtx->at(index))) break;
      // check if no vertices are inside?
      int end = index;
      bool newf = true;
      bool first = true;
      int getvert;
      int next;
      while (first || !newf || (index != end)) {
        first = false;
        if (newf) {
          newvtx->push_back(f->vtx->at(index));
          next = modnext(index,fsize);
          getvert = getVertexIndex(f->vtx->at(next));
        } else {
          newvtx->push_back(vtx->at(index));
          next = modnext(index,size);
          getvert = f->getVertexIndex(vtx->at(next));
        }
        if (getvert != -1) {
          newf = !newf;
          index = getvert;
        } else index = next;
      }
      delete vtx;
      vtx = newvtx;
    }
    comps->push_back(f);
    printf("Addface end... (%d,%d)\n",vtx->size(),f->vtx->size());
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
  bool vertexNearestIntersect(int begin, int end, Vertex &P, int &index) {
    Vertex A = mesh->v[begin];
    Vertex B = mesh->v[end];
    float length = (A-B).length();
    int size = vtx->size();
    float distance = length + 1.0f;
    Vertex R;
    for (int i = 0; i < size; i++) {
      if (intersectZ(A,B,mesh->v[vtx->at(i)],mesh->v[vtx->at(modnext(i,size))],R)) {
        float thisdistance = (A-R).length();
        if (thisdistance < EPSILON) continue;
        if (thisdistance < distance) {
          distance = thisdistance;
          P = R;
          index = i;
        }
      }
    }      
    return distance <= length;
  }
  virtual bool isMultiFace() { return true; }
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
