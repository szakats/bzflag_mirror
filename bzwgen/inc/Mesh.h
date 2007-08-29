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

#ifndef __MESH_H__
#define __MESH_H__

#include "globals.h"
#include "Output.h"
#include "Face.h"

class Mesh {
public:
  VertexVector v;
  TexCoordVector tc;
  FaceVector f;
  IntVector freeVertices;
  VertexVector inside;
  VertexVector outside;
  VertexVector vbase;
  bool passable;
  Mesh() : passable(false) {}
  int addVertex(Vertex vtx);
  int addTexCoord(TexCoord tcx);
  int addFace(Face* face) { f.push_back(face); return f.size()-1; }
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, int mat = 0);
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord ta, TexCoord tb, TexCoord tc, TexCoord td, int mat = 0);
  void extrudeFace(int fid, double amount, int mat = 0);
  IntVector* extrudeFaceR(int fid, double amount, int mat = 0);
  void expandFace(int fid, double amount);
  Vertex faceNormal(int fid);
  Vertex faceCenter(int fid);
  IntVector* repeatSubdivdeFace(int fid, double snap, bool horizontal);
  IntVector* subdivdeFace(int fid, int count, bool horizontal, double ssnap = 0.0);
  int partitionFace(int fid, double amount, bool horizontal);
  void chamferFace(int fid, double amount);
  void taperFace(int fid, double amount);
  void scaleFace(int fid, double x, double y);
  void freeFace(int fid);
  void translateFace(int fid, double x, double y, double z);
  void weldVertices(int a, int b, Vertex vx);
  void weldVertices(int a, int b);
  void output(Output& out, int materialCount);
  void textureFace(int fid, double snap, double tile);
  void textureFaceFull(int fid);
  void textureFaceQuad(int fid, double au, double av, double bu, double bv);
  int createNGon(Vertex center, double radius, int n);
  double faceH(int face) { return (v[f[face]->vtx->at(0)] - v[f[face]->vtx->at(1)]).length(); }
  double faceV(int face) { return (v[f[face]->vtx->at(3)] - v[f[face]->vtx->at(0)]).length(); }
  std::string faceToString(Face* face);
  void pushBase(int fid);
  ~Mesh();
private:
  Vertex extensionVertex(int ida, int idb, int idc);

};

typedef std::vector<Mesh*> MeshVector;
typedef MeshVector::iterator MeshVectIter;

#endif /* __MESH_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
