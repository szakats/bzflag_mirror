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

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d) {
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  int size = v.size();
  f.push_back(Face(ID4(size-4,size-3,size-2,size-1),ID4()));
  return f.size()-1;
}

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord tca, TexCoord tcb, TexCoord tcc, TexCoord tcd) {
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
  f.push_back(Face(ID4(size-4,size-3,size-2,size-1),ID4(tsize-4,tsize-3,tsize-2,tsize-1)));
  return f.size()-1;
}

void Mesh::output(Output& out) {
  out.line("mesh");
  out.matref(matref); // temp
  for (int i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (int i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 
  if (noradar) out.line("  noradar"); 
  for (int i = 0; i < f.size(); i++) out.face(f[i]);
  out.line("end\n");
};

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
