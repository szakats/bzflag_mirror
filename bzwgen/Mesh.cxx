/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
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
  return addFace(Face(ID4(size-4,size-3,size-2,size-1),mat));
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
  return addFace(Face(ID4(size-4,size-3,size-2,size-1),ID4(tsize-4,tsize-3,tsize-2,tsize-1),mat));
}

ID4 Mesh::extrudeFace(int fid, float amount) {
  Vertex dir = faceNormal(fid)*amount;
  ID4 base = f[fid].vtx;
  int newface = createNewFace(v[base.a]+dir,v[base.b]+dir,v[base.c]+dir,v[base.d]+dir,f[fid].mat);
  f[fid] = f[newface];
  f.pop_back();
  ID4 top = f[fid].vtx;
  return ID4(
      addFace(Face(ID4(base[0],base[1],top[1],top[0]))),
      addFace(Face(ID4(base[1],base[2],top[2],top[1]))),
      addFace(Face(ID4(base[2],base[3],top[3],top[2]))),
      addFace(Face(ID4(base[3],base[0],top[0],top[3])))
    );
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


void Mesh::expandFace(int fid, float amount) {
  // needs to be uniform
  ID4 fv = f[fid].vtx;
  Vertex v0 = v[fv[0]] + extensionVertex(fv[3],fv[1],fv[0])*amount;
  Vertex v1 = v[fv[1]] + extensionVertex(fv[0],fv[2],fv[1])*amount;
  Vertex v2 = v[fv[2]] + extensionVertex(fv[1],fv[3],fv[2])*amount;
  Vertex v3 = v[fv[3]] + extensionVertex(fv[2],fv[0],fv[3])*amount;

  v[fv[0]] = v0;
  v[fv[1]] = v1;
  v[fv[2]] = v2;
  v[fv[3]] = v3;
}



Vertex Mesh::faceCenter(int fid) {
  Face face = f[fid];
  return (v[face.vtx.a]+v[face.vtx.b]+v[face.vtx.c]+v[face.vtx.d])/4;
}


Vertex Mesh::faceNormal(int fid) {
  Face face = f[fid];
  Vertex a = v[face.vtx.a]-v[face.vtx.b];
  Vertex b = v[face.vtx.a]-v[face.vtx.c];
  Vertex r = a.cross(b);
  float length = r.length();
  return r / length;
}


void Mesh::output(Output& out) {
  out.line("mesh");
  out.matref(matref); 
  int mat = matref;
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 
  if (noradar) out.line("  noradar"); 
  for (size_t i = 0; i < f.size(); i++) out.face(f[i],mat);
  out.line("end\n");
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
