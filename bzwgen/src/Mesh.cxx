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

#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>

int Mesh::addVertex(Vertex vtx) { 
  if (freeVertices.size() > 0) {
    int free = freeVertices[freeVertices.size()-1];
    freeVertices.pop_back();
    v[free] = vtx;
    return free;
  } else {
    v.push_back(vtx); 
    return v.size()-1; 
  }
}

int Mesh::addTexCoord(TexCoord tcx) { 
  for (size_t i = 0; i < tc.size(); i++) {
    if (tc[i] == tcx) return i;
  }
  tc.push_back(tcx); 
  return tc.size()-1; 
}

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, int mat) {
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  int size = v.size();
  return addFace(new Face(ID4(size-4,size-3,size-2,size-1),mat));
}

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord tca, TexCoord tcb, TexCoord tcc, TexCoord tcd, int mat) {
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  int size = v.size();
  tc.push_back(tca);
  tc.push_back(tcb);
  tc.push_back(tcc);
  tc.push_back(tcd);
  int tsize = tc.size();
  return addFace(new Face(ID4(size-4,size-3,size-2,size-1),ID4(tsize-4,tsize-3,tsize-2,tsize-1),mat));
}

int Mesh::createNGon(Vertex center, double radius, int n) {
  Face* face = new Face();
  double step = (2*double(M_PI))/n;
  for (int i = 0; i < n; i++) {
    int vt = addVertex(center+Vertex(radius*cos(step*double(i)-step/2),radius*sin(step*double(i)-step/2),0.0));
    face->vtx->push_back(vt);
  }
  return addFace(face);
}


IntVector* Mesh::extrudeFaceR(int fid, double amount, int mat) {
  Vertex dir = faceNormal(fid)*amount;
  IntVector* base   = f[fid]->vtx;
  IntVector* result = new IntVector;
  IntVector* top    = new IntVector;

  int size = base->size();

  for (int i = 0; i < size; i++) {
    top->push_back(addVertex(v[base->at(i)]+dir));
  }
  f[fid]->vtx = top;

  for (int i = 0; i < size; i++) {
    result->push_back(addFace(new Face(ID4(base->at(i),base->at(modnext(i,size)),top->at(modnext(i,size)),top->at(i)),mat)));
  }

  delete base;
  return result;
}

void Mesh::extrudeFace(int fid, double amount, int mat) {
  Vertex dir = faceNormal(fid)*amount;
  IntVector* base   = f[fid]->vtx;
  IntVector* top    = new IntVector;

  int size = base->size();

  for (int i = 0; i < size; i++) {
    top->push_back(addVertex(v[base->at(i)]+dir));
  }
  f[fid]->vtx = top;

  for (int i = 0; i < size; i++) {
    addFace(new Face(ID4(base->at(i),base->at(modnext(i,size)),top->at(modnext(i,size)),top->at(i)),mat));
  }

  delete base;
}

Vertex Mesh::extensionVertex(int ida, int idb, int idc) {
  Vertex a = (v[idc]-v[ida]).norm();
  Vertex b = (v[idc]-v[idb]).norm();
  Vertex dir = ((a+b)/2).norm();
  double dot = a.dot(dir);
  double length = 1/sqrt(1-dot*dot);
  return dir*length;
}

void Mesh::taperFace(int fid, double amount) {
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  Vertex c = faceCenter(fid);
  for (int i = 0; i < size; i++) {
    Vertex vv = v[fv->at(i)];
    v[fv->at(i)] = (vv - c)*amount+c;
  }
}

void Mesh::scaleFace(int fid, double x, double y) {
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  Vertex c = faceCenter(fid);
  for (int i = 0; i < size; i++) {
    Vertex vc = v[fv->at(i)] - c;
    v[fv->at(i)].x = vc.x*x+c.x;
    v[fv->at(i)].y = vc.y*y+c.y;
  }
}

void Mesh::translateFace(int fid, double x, double y, double z) {
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  for (int i = 0; i < size; i++) {
    v[fv->at(i)].x += x;
    v[fv->at(i)].y += y;
    v[fv->at(i)].z += z;
  }
}


void Mesh::expandFace(int fid, double amount) {
  Vertex normal = faceNormal(fid);
  // needs to be uniform
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  Vertex* nv = new Vertex[size];
  for (int i = 0; i < size; i++) {
    Vertex a = v[fv->at(modnext(i,size))] - v[fv->at(modprev(i,size))];
    Vertex b = v[fv->at(i)] - v[fv->at(modprev(i,size))];
    double sign = fsign( b.cross(a).dot(normal) );
    nv[i] = v[fv->at(i)] + extensionVertex(fv->at(modprev(i,size)),fv->at(modnext(i,size)),fv->at(i))*amount*sign;
  }
  for (int i = 0; i < size; i++) {
    v[fv->at(i)] = nv[i];
  }
  delete nv;
}

void Mesh::weldVertices(int a, int b, Vertex vx) {
  for (size_t i = 0; i < f.size(); i++) {
    int indexa = -1;
    int indexb = -1;
    for (size_t j = 0; j < f[i]->vtx->size(); j++) {    
      if (f[i]->vtx->at(j) == b) {
        indexb = j;
      } else if (f[i]->vtx->at(j) == a) {
        indexa = j;
      }
    }
    if (indexb != -1) {
      if (indexa != -1) {
        f[i]->vtx->erase(f[i]->vtx->begin()+indexb);
        if (f[i]->texcoords) f[i]->tcd->erase(f[i]->vtx->begin()+indexb);  
      } else {
        (*f[i]->vtx)[indexb] = a;
      }
    }
  }
  v[a] = vx;  
  freeVertices.push_back(b);
}

void Mesh::weldVertices(int a, int b) {
  Vertex c = (v[a]+v[b])/2;
  weldVertices(a,b,c);
}



Vertex Mesh::faceCenter(int fid) {
  Face* face = f[fid];
  return (v[face->vtx->at(0)]+v[face->vtx->at(1)]+v[face->vtx->at(2)]+v[face->vtx->at(3)])/4;
}

// TODO : remove hack for multifaces;
Vertex Mesh::faceNormal(int fid) {
  Face* face = f[fid];
  if (face->isMultiFace()) return Vertex(0.0,0.0,1.0);
  Vertex a = v[face->vtx->at(0)]-v[face->vtx->at(1)];
  Vertex b = v[face->vtx->at(0)]-v[face->vtx->at(2)];
  Vertex r = a.cross(b);
  double length = r.length();
  return r / length;
}

IntVector* Mesh::repeatSubdivdeFace(int fid, double snap, bool horizontal) {
  double len = horizontal ? faceH(fid) : faceV(fid);
  len = refinesnap(snap,len);
  return subdivdeFace(fid,int(len/snap),horizontal);
}

IntVector* Mesh::splitFace(int fid, DoubleVector* splitData, bool horizontal) {
  IntVector* cnr = f[fid]->vtx;
  Vertex stepA, stepB;
  int splits = splitData->size()-1;

  double length = horizontal ? faceH(fid) : faceV(fid);
  double relsum = 0.0;
  double lsum = 0.0;
  for (int i = 0; i < splits+1; i++) {
    double value = splitData->at(i);
    if (value >= 0.0) lsum += value; else relsum += -value;
  }
  if (relsum > 0) {
    double relperunit = ( length-lsum ) / relsum;
    for (int i = 0; i < splits+1; i++) {
      double value = splitData->at(i);
      if (value < 0.0) (*splitData)[i] = -value*relperunit;
    }
  }  

  stepA = (v[cnr->at(2)]-v[cnr->at(3)]).norm();
  stepB = (v[cnr->at(1)]-v[cnr->at(0)]).norm();

  IntVector* result = new IntVector();

  int mat = f[fid]->mat;
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = cnr->at(3);
    bs = cnr->at(0);
  } else {
    as = cnr->at(0);
    bs = cnr->at(1);
  }
  pai = as;
  pbi = bs;

  Vertex a = v[as];
  Vertex b = v[bs];

  for (int i = 0; i < splits; i++) {
    a = a + stepA*splitData->at(i);
    b = b + stepB*splitData->at(i);

    ai = addVertex(a);
    bi = addVertex(b);

    if (horizontal) {
      result->push_back(addFace(new Face(ID4(pbi,bi,ai,pai),mat)));
    } else {
      result->push_back(addFace(new Face(ID4(pai,pbi,bi,ai),mat)));
    }

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

  if (horizontal) {
    f[fid]->setID4(ID4(bi,cnr->at(1),cnr->at(2),ai));
  } else {
    f[fid]->setID4(ID4(ai,bi,cnr->at(2),cnr->at(3)));
  }
  return result;
}


IntVector* Mesh::subdivdeFace(int fid, int count, bool horizontal, double ssnap) {
  IntVector* cnr = f[fid]->vtx;
  Vertex stepA, stepB;

  double s = ssnap;
  if (horizontal) {
    stepA = (v[cnr->at(2)]-v[cnr->at(3)]) / double(count);
    stepB = (v[cnr->at(1)]-v[cnr->at(0)]) / double(count);
    if (ssnap > EPSILON) s = refinesnap(ssnap,faceH(fid));
  } else {
    stepA = (v[cnr->at(3)]-v[cnr->at(0)]) / double(count);
    stepB = (v[cnr->at(2)]-v[cnr->at(1)]) / double(count);
    if (ssnap > EPSILON) s = refinesnap(ssnap,faceV(fid));
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->mat;
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = cnr->at(3);
    bs = cnr->at(0);
  } else {
    as = cnr->at(0);
    bs = cnr->at(1);
  }
  pai = as;
  pbi = bs;

  Vertex a = v[as];
  Vertex b = v[bs];

  for (int i = 0; i < count-1; i++) {
    a = a + stepA;
    b = b + stepB;

    if (ssnap > EPSILON) {
      Vertex A = v[as]-a;
      Vertex B = v[bs]-b;

      double la = A.length();
      double lb = B.length();

      Vertex sa = v[as]-(A.norm()*snap(la,s));
      Vertex sb = v[bs]-(B.norm()*snap(lb,s));

      ai = addVertex(sa);
      bi = addVertex(sb);
    } else {
      ai = addVertex(a);
      bi = addVertex(b);
    }

    if (horizontal) {
      result->push_back(addFace(new Face(ID4(pbi,bi,ai,pai),mat)));
    } else {
      result->push_back(addFace(new Face(ID4(pai,pbi,bi,ai),mat)));
    }

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

  if (horizontal) {
    f[fid]->setID4(ID4(bi,cnr->at(1),cnr->at(2),ai));
  } else {
    f[fid]->setID4(ID4(ai,bi,cnr->at(2),cnr->at(3)));
  }
  return result;

}

int Mesh::partitionFace(int fid, double amount, bool horizontal) {
  IntVector* cnr = f[fid]->vtx;
  Vertex A, B;

  if (horizontal) {
    A = (v[cnr->at(2)]-v[cnr->at(3)]);
    B = (v[cnr->at(1)]-v[cnr->at(0)]);
  } else {
    A = (v[cnr->at(3)]-v[cnr->at(0)]);
    B = (v[cnr->at(2)]-v[cnr->at(1)]);
  }

  A = A * (amount / A.length());
  B = B * (amount / B.length());

  int pai = 0, pbi = 0;

  if (horizontal) {
    pai = cnr->at(3);
    pbi = cnr->at(0);
  } else {
    pai = cnr->at(0);
    pbi = cnr->at(1);
  }

  int ai = addVertex(v[pai]+A);
  int bi = addVertex(v[pbi]+B);

  int result;
  if (horizontal) {
    result = addFace(new Face(ID4(pbi,bi,ai,pai),f[fid]->mat));
    f[fid]->setID4(ID4(bi,cnr->at(1),cnr->at(2),ai));
  } else {
    result = addFace(new Face(ID4(pai,pbi,bi,ai),f[fid]->mat));
    f[fid]->setID4(ID4(ai,bi,cnr->at(2),cnr->at(3)));
  }
  return result;
}


void Mesh::output(Output& out, int materialCount) {
  out.line("mesh");
  int mat = -1;
  for (size_t i = 0; i < inside.size(); i++) out.vertex(inside[i],"inside");
  for (size_t i = 0; i < outside.size(); i++) out.vertex(outside[i],"outside");
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 

  for (int m = 0; m <= materialCount; m++) {
    for (size_t i = 0; i < f.size(); i++) 
      if (f[i]->mat == m) {
        if (mat != m) out.matref(m);
        if (passable) out.line("  passable"); 
        mat = m;
        out.face(f[i],mat);
        mat = f[i]->mat;
      }
  }
  out.line("end\n");
}

// TODO: handle texcoords?
// TODO: handle previous?
void Mesh::chamferFace(int fid, double amount) {
  IntVector old;
  int size = f[fid]->vtx->size();
  for (int i = 0; i < size; i++) {
    old.push_back(f[fid]->vtx->at(i));
  }
  f[fid]->vtx->clear();
  VertexVector in;
  VertexVector out;
  for (int i = 0; i < size; i++) {
    in.push_back(v[old[i]]);
  }
  for (int i = 0; i < size; i++) {
    Vertex a = in[i]-in[modprev(i,size)];
    Vertex b = in[i]-in[modnext(i,size)];
    double af = (a.length()-amount)/a.length();
    double bf = (b.length()-amount)/b.length();
    a = in[modprev(i,size)]+a*af;
    b = in[modnext(i,size)]+b*bf;
    v[old[i]] = a;
    f[fid]->vtx->push_back(old[i]);
    f[fid]->vtx->push_back(addVertex(b));
  }
}

// TODO: these coords could be reused!
void Mesh::textureFaceFull(int fid) {
  IntVector* tcd = f[fid]->tcd;
  tcd->clear();
  tcd->push_back(addTexCoord(TexCoord(0.0,0.0)));
  tcd->push_back(addTexCoord(TexCoord(1.0,0.0)));
  tcd->push_back(addTexCoord(TexCoord(1.0,1.0)));
  tcd->push_back(addTexCoord(TexCoord(0.0,1.0)));
  f[fid]->texcoords = true;
}

void Mesh::textureFaceQuad(int fid, double au, double av, double bu, double bv) {
  IntVector* tcd = f[fid]->tcd;
  tcd->clear();
  tcd->push_back(addTexCoord(TexCoord(au,av)));
  tcd->push_back(addTexCoord(TexCoord(bu,av)));
  tcd->push_back(addTexCoord(TexCoord(bu,bv)));
  tcd->push_back(addTexCoord(TexCoord(au,bv)));
  f[fid]->texcoords = true;
}

void Mesh::textureFace(int fid, double snap, double tile) {
  textureFaceQuad(fid,0.0,0.0,roundToInt(faceH(fid)/snap)*tile,roundToInt(faceV(fid)/snap)*tile);
}

void Mesh::freeFace(int fid) {
  f[fid]->output = false;
  for (size_t i = 0; i < f[fid]->vtx->size(); i++) {
    freeVertices.push_back(f[fid]->vtx->at(i));
  }
}

std::string Mesh::faceToString(Face* face) { 
  std::string result = "Face: ( ";
  for (size_t i = 0; i < face->vtx->size(); i++)
    result += v[face->vtx->at(i)].toString()+ " ";
  result += ")";
  return result;
}
void Mesh::pushBase(int fid) {
  vbase.clear();
  for (int i = 0; i < f[fid]->size(); i++) {
    vbase.push_back(v[f[fid]->vertex(i)]);
  }
}

Mesh::~Mesh() {
  for (FaceVectIter itr = f.begin(); itr!= f.end(); ++itr) delete (*itr);
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
