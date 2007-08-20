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

IntVector* Mesh::extrudeFaceR(int fid, float amount, int mat) {
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

void Mesh::extrudeFace(int fid, float amount, int mat) {
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
  Vertex a = v[idc]-v[ida];
  Vertex b = v[idc]-v[idb];
  a.normalize();
  b.normalize();
  Vertex dir = (a+b)/2;
  dir.normalize();
  float dot = a.dot(dir);
  float length = 1/sqrtf(1-dot*dot);
  return dir*length;
}

void Mesh::taperFace(int fid, float amount) {
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  Vertex c = faceCenter(fid);
  for (int i = 0; i < size; i++) {
    Vertex vv = v[fv->at(i)];
    v[fv->at(i)] = (vv - c)*amount+c;
  }
}


void Mesh::expandFace(int fid, float amount) {
  // needs to be uniform
  IntVector* fv = f[fid]->vtx;
  int size = fv->size();
  Vertex* nv = new Vertex[size];
  for (int i = 0; i < size; i++) {
    nv[i] = v[fv->at(i)] + extensionVertex(fv->at(modprev(i,size)),fv->at(modnext(i,size)),fv->at(i))*amount;
  }
  for (int i = 0; i < size; i++) {
    v[fv->at(i)] = nv[i];
  }
  delete nv;
}

/* this operation leaves one unused vertex unfortunately */
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
}

void Mesh::weldVertices(int a, int b) {
  Vertex c = (v[a]+v[b])/2;
  weldVertices(a,b,c);
}



Vertex Mesh::faceCenter(int fid) {
  Face* face = f[fid];
  return (v[face->vtx->at(0)]+v[face->vtx->at(1)]+v[face->vtx->at(2)]+v[face->vtx->at(3)])/4;
}


Vertex Mesh::faceNormal(int fid) {
  Face* face = f[fid];
  Vertex a = v[face->vtx->at(0)]-v[face->vtx->at(1)];
  Vertex b = v[face->vtx->at(0)]-v[face->vtx->at(2)];
  Vertex r = a.cross(b);
  float length = r.length();
  return r / length;
}

IntVector* Mesh::subdivdeFace(int fid, int count, bool horizontal) {
  IntVector* cnr = f[fid]->vtx;
  Vertex stepA, stepB;

  if (horizontal) {
    stepA = (v[cnr->at(2)]-v[cnr->at(3)]) / float(count);
    stepB = (v[cnr->at(1)]-v[cnr->at(0)]) / float(count);
  } else {
    stepA = (v[cnr->at(3)]-v[cnr->at(0)]) / float(count);
    stepB = (v[cnr->at(2)]-v[cnr->at(1)]) / float(count);
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->mat;
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;

  if (horizontal) {
    pai = cnr->at(3);
    pbi = cnr->at(0);
  } else {
    pai = cnr->at(0);
    pbi = cnr->at(1);
  }

  Vertex a = v[pai];
  Vertex b = v[pbi];

  for (int i = 0; i < count-1; i++) {
    a = a + stepA;
    b = b + stepB;
    
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

int Mesh::partitionFace(int fid, float amount, bool horizontal) {
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


void Mesh::output(Output& out) {
  out.line("mesh");
  int mat = -1;
  for (size_t i = 0; i < inside.size(); i++) out.vertex(inside[i],"inside");
  for (size_t i = 0; i < outside.size(); i++) out.vertex(outside[i],"outside");
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 

  for (int m = 0; m <= MAXMATERIALS; m++) {
    for (size_t i = 0; i < f.size(); i++) 
      if (f[i]->mat == m) {
	if (mat != m) out.matref(m);
	mat = m;
	out.face(f[i],mat);
	mat = f[i]->mat;
      }
  }
  out.line("end\n");
}

// TODO: handle texcoords?
// TODO: handle previous?
void Mesh::chamferFace(int fid, float amount) {
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
    float af = (a.length()-amount)/a.length();
    float bf = (b.length()-amount)/b.length();
    a = in[modprev(i,size)]+a*af;
    b = in[modnext(i,size)]+b*bf;
    v[old[i]] = a;
    f[fid]->vtx->push_back(old[i]);
    f[fid]->vtx->push_back(addVertex(b));
  }
}


Mesh::~Mesh() {
  FaceVectIter itr; 
  for (itr = f.begin(); itr!= f.end(); ++itr) delete (*itr);
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
