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
  VertexVector inside;
  VertexVector outside;
  bool noradar;
  bool passable;
  Mesh() : noradar(false), passable(false) {}
  int addVertex(Vertex vtx) { v.push_back(vtx); return v.size()-1; }
  int addTexCoord(TexCoord tcx) { tc.push_back(tcx); return tc.size()-1; }
  int addFace(Face* face) { f.push_back(face); return f.size()-1; }
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, int mat = 0);
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord ta, TexCoord tb, TexCoord tc, TexCoord td, int mat = 0);
  void extrudeFace(int fid, float amount, int mat = 0);
  IntVector* extrudeFaceR(int fid, float amount, int mat = 0);
  void expandFace(int fid, float amount);
  Vertex faceNormal(int fid);
  Vertex faceCenter(int fid);
  IntVector* subdivdeFace(int fid, int count, bool horizontal);
  int partitionFace(int fid, float amount, bool horizontal);
  void weldVertices(int a, int b, Vertex vx);
  void output(Output& out);
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
