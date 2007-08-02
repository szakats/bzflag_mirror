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

ID4 Mesh::extrudeFace(int fid, float amount, int mat) {
  Vertex dir = faceNormal(fid)*amount;
  ID4 base = f[fid].vtx;
  int newface = createNewFace(v[base.a]+dir,v[base.b]+dir,v[base.c]+dir,v[base.d]+dir,f[fid].mat);
  f[fid] = f[newface];
  f.pop_back();
  ID4 top = f[fid].vtx;
  return ID4(
      addFace(Face(ID4(base[0],base[1],top[1],top[0]),mat)),
      addFace(Face(ID4(base[1],base[2],top[2],top[1]),mat)),
      addFace(Face(ID4(base[2],base[3],top[3],top[2]),mat)),
      addFace(Face(ID4(base[3],base[0],top[0],top[3]),mat))
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

IntVector* Mesh::subdivdeFace(int fid, int count, bool horizontal) {
  ID4 cnr = f[fid].vtx;
  Vertex stepA, stepB;

  if (horizontal) {
    stepA = (v[cnr[2]]-v[cnr[3]]) / float(count);
    stepB = (v[cnr[1]]-v[cnr[0]]) / float(count);
  } else {
    stepA = (v[cnr[3]]-v[cnr[0]]) / float(count);
    stepB = (v[cnr[2]]-v[cnr[1]]) / float(count);
  }

  IntVector* result = new IntVector();

  int mat = f[fid].mat;
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;

  if (horizontal) {
    pai = cnr[3];
    pbi = cnr[0];
  } else {
    pai = cnr[0];
    pbi = cnr[1];
  }

  Vertex a = v[pai];
  Vertex b = v[pbi];

  result->push_back(fid);

  for (int i = 0; i < count-1; i++) {
    a = a + stepA;
    b = b + stepB;
    
    ai = addVertex(a);
    bi = addVertex(b);

    result->push_back(addFace(Face(ID4(pai,pbi,bi,ai),mat)));

    pai = ai;
    pbi = bi;
  }

  if (horizontal) {
    f[fid].vtx = ID4(ai,bi,cnr[1],cnr[2]);
  } else {
    f[fid].vtx = ID4(ai,bi,cnr[2],cnr[3]);
  }
  return result;

}


//IntVector& Mesh::subdivdeFaceV(int fid) {
//  
//}


void Mesh::output(Output& out) {
  out.line("mesh");
  out.matref(matref); 
  int mat = matref;
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 
  if (noradar) out.line("  noradar"); 
  for (int m = 0; m <= MAXMATERIALS; m++) {
    for (size_t i = 0; i < f.size(); i++) 
      if (f[i].mat == m) {
	if (mat != m) out.matref(m);
	mat = m;
	out.face(f[i],mat);
	mat = f[i].mat;
      }
  }
  out.line("end\n");
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
