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

#include "MultiFace.h"

void MultiFace::updateFaces(float z) {
  for (size_t fi = 0; fi < comps->size(); fi++) {
    Face* f = comps->at(fi);
    for (int t = 0; t < f->size(); t++) {
      if (f->tcd->at(t) < 0) {
        mesh->v[f->vertex(t)].z = z;
      } else {
        (*f->vtx)[t] = vertex(f->tcd->at(t));
      }
    }
  }
}

void MultiFace::storeFaces() {
  for (size_t fi = 0; fi < comps->size(); fi++) {
    Face* f = comps->at(fi);
    f->tcd->resize(f->size());
    for (int t = 0; t < f->size(); t++) {
      (*f->tcd)[t] = getVertexIndex(f->vertex(t));
    }
  }
}


IntVector* MultiFace::detachFace(int id) {
  printf("Detach face\n");
  if (comps->size() < 2) return NULL;
  if (id >= int(comps->size())) return NULL;
  updateFaces(mesh->v[vertex(0)].z);
  
  IntVector* result  = new IntVector();
  IntVector* visited = new IntVector();

  Face* f = comps->at(id);

  int index = pickRemovalIndex(comps->at(id),visited);

  while (index >= 0) {
    printf("Element iteration...\n");
    IntVector* nvtx = new IntVector;

    int vid  = vertex(index);
    int svid = vertex(index);
    int lastgindex = index;

    do {
      int gindex = getVertexIndex(vid);
      printf("Vertex iteration...(%d,index = %d)\n",vid,gindex);
      if (gindex != -1) lastgindex = gindex;
      int findex = f->getVertexIndex(vid);
      int oindex;
      Face* oface = getOtherFaceWithVertex(f,vid);
      oindex = (oface == NULL) ? -1 : oface->getVertexIndex(vid);

      visited->push_back(vid);
      nvtx->push_back(vid);

      if (oindex < 0 ) {
        printf("Remove (%d)\n",vid);
        removeVertex(gindex);
        vid = f->vertex(findex+1);
      } else {
        int nextovid = oface->vertex(oindex-1);
        if (findex < 0) {
          if (gindex < 0) insertVertexBefore(lastgindex,vid);
          vid = nextovid;
        } else {
          int nextfvid = f->vertex(findex+1);
          int prevfvid = f->vertex(findex-1);
          int prevovid = oface->vertex(oindex-1);
          if (nextovid == nextfvid) {
            vid = nextfvid;
          } else if (prevfvid == nextovid) {
            vid = nextfvid;
          } else {
            vid = isLeftOfVectors(prevovid,prevfvid,vid,nextfvid) ? nextfvid : nextovid;
          }
        }
      }
    } while (vid != svid);

    Face* nface = new Face();
    nface->mat = mat;
    nface->vtx = nvtx;
    result->push_back(mesh->addFace(nface));
    index = pickRemovalIndex(comps->at(id),visited);
  }

  printf("Cleaning up\n");
  delete visited;
  delete comps->at(id);

  comps->erase(comps->begin() + id);

  if (result->size() == 0) {
    printf("Result Zero\n");
    delete result;
    result = NULL;
  }
  printf("Storing faces\n");
  storeFaces();
  printf("Done\n");
  return result;
}

bool MultiFace::isLeftOfVectors(int x, int a, int b, int c) {
  Vertex P = mesh->v[b];
  Vertex C = (mesh->v[c]-P).norm();
  Vertex A = (P-mesh->v[a]).norm();
  Vertex X = (mesh->v[x]-P).norm();
  Vertex Xline = A+X;
  Vertex Pline = A+C;

  Vertex normal = Vertex(0.0f,0.0f,1.0f);//faceNormal(fid);

  return fsign( Xline.cross(Pline).dot(normal) ) >= 0;
}

// BUUUUUG, doesn't handle inter-face jumps when doing the cut edge!

Face* MultiFace::getOtherFaceWithVertex(Face* f, int vid) {
  for (size_t fi = 0; fi < comps->size(); fi++) {
    if (comps->at(fi) != f && comps->at(fi)->hasVertex(vid)) return comps->at(fi);
  }
  return NULL;
}

int MultiFace::pickRemovalIndex(Face *f,IntVector* visited) {
  for (int i = 0; i < size(); i++) {
    if (f->hasVertex(vertex(i))) {
      bool found = false;
      if (visited->size() > 0) 
        for (size_t k = 0; k < visited->size(); k++)
          if (visited->at(k) == vertex(i)) {
            found = true;
            break;
          }
      if (!found) return i;
    } 
  }
  return -1;
}

void MultiFace::refineFace(Face* f, Face* target) {
  int i = 0;
  while (i < f->size()) {
    Vertex ipoint;
    int index;
    //printf("Intersect... (%d,%d)\n",i,modnext(i,f->size()));
    //      printf("Multi%s\n",mesh->faceToString(this).c_str());
    if (vertexNearestIntersect(f->vertex(i),f->vertex(i+1),ipoint,index,target)) {
      //printf("Nearerst found... (%d) %s : ",index,ipoint.toString().c_str());
      int ipid;
      if (samepointZ(mesh->v[f->vertex(i+1)],mesh->v[target->vertex(index+1)])) {
        //printf("Is common\n");
      } else if (samepointZ(ipoint,mesh->v[f->vertex(i+1)])) {
        //printf("Is samepoint with next\n");
        target->insertVertexAfter(index,f->vertex(i+1));
      } else if (samepointZ(ipoint,mesh->v[target->vertex(index+1)])) {
        //printf("Is samepoint with itself\n");
        f->insertVertexAfter(i,target->vertex(index+1));
      } else {
        //printf("Is normal\n");
        ipid = mesh->addVertex(ipoint);
        f->insertVertexAfter(i,ipid);
        target->insertVertexAfter(index,ipid);
      }
    }
    i++;
  }
}


int MultiFace::addFace(Face* f) {
  //printf("Addface start... (%d,%d)\n",size(),f->size());
  //    printf("Multi%s\n",mesh->faceToString(this).c_str());
  //    printf("Add%s\n",mesh->faceToString(f).c_str());
  f->output = false;
  if (comps->size() == 0) {
    for (int i = 0; i < f->size(); i++) 
      vtx->push_back(f->vertex(i));
  } else {
    refineFace(f,this);
    for (size_t i = 0; i < comps->size(); i++) refineFace(f,comps->at(i));
    int fsize = f->size();
    int tsize = size();
    //printf("Refined... (%d,%d)\n",tsize,fsize);
    IntVector* newvtx = new IntVector;
    int index = 0;
    for (index = 0; index < fsize; index++) 
      if (getVertexIndex(f->vertex(index)) < 0 && !vertexInside(f->vertex(index))) break;
    // check if no vertices are inside?
    int end = index;
    bool newf = true;
    bool first = true;
    int getvert;
    int next;
    while (first || !newf || (index != end)) {
      //printf("--- %d,%d\n",index,newf);
      first = false;
      if (newf) {
        newvtx->push_back(f->vertex(index));
        next = modnext(index,fsize);
        getvert = getVertexIndex(f->vertex(next));
      } else {
        newvtx->push_back(vertex(index));
        next = modnext(index,tsize);
        getvert = f->getVertexIndex(vertex(next));
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
  storeFaces();
  //printf("Addface end... (%d,%d)\n",size(),f->size());
  return comps->size()-1;
}


bool MultiFace::vertexInside(int vid) {
  Vertex A = mesh->v[vid];
  Vertex B = mesh->v[vid];
  B.y = 100000.0f; // sufficient to be out of range
  B.x = 200000.0f; // sufficient to be out of range
  Vertex P1;
  Vertex P2;
  int count = 0;
  for (int i = 0; i < size(); i++) {
    if (intersectZ(A,B,mesh->v[vertex(i)],mesh->v[vertex(i+1)],P1,P2) > 0) count++;
  }      
  return (count%2 == 1);
}


bool MultiFace::vertexNearestIntersect(int begin, int end, Vertex &P, int &index, Face* face) {
  Vertex A = mesh->v[begin];
  Vertex B = mesh->v[end];
  float length = (A-B).length();
  int tsize = face->size();
  float distance = length + 1.0f;
  Vertex R1;
  Vertex R2;
  for (int i = 0; i < tsize; i++) {
    int r = intersectZ(A,B,mesh->v[face->vertex(i)],mesh->v[face->vertex(i+1)],R1,R2);
    if (r > 0) {
      if (!samepointZ(mesh->v[face->vertex(i)],R1)) {
        float thisdistance = (A-R1).length();
        if (thisdistance > EPSILON) {
//          printf("ICH:%s\n",R1.toString().c_str());
          if (thisdistance < distance) {
            distance = thisdistance;
            P = R1;
            index = i;
          }
        }
      }
    }
    if (r == 2) {
      if (!samepointZ(mesh->v[face->vertex(i)],R2)) {
        float thisdistance = (A-R2).length();
        if (thisdistance < EPSILON) continue;
//        printf("ICH(2):%s\n",R2.toString().c_str());
        if (thisdistance < distance) {
          distance = thisdistance;
          P = R2;
          index = i;
        }
      }
    }
  }      
  return distance <= length+EPSILON;
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
