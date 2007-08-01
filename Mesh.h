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

#include "common.h"
#include "Output.h"

class Mesh {
public:
  VertexVector v;
  TexCoordVector tc;
  FaceVector f;
  bool passable;
  bool noradar;
  int matref; 
  Mesh() : noradar(false), passable(false) {}
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d);
  int createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord ta, TexCoord tb, TexCoord tc, TexCoord td);
  void output(Output& out);
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