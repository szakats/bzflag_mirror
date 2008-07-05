/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
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
  FaceVector* comps;
  Mesh* mesh;
public:
  MultiFace(Mesh* _mesh) : Face(), mesh(_mesh) { comps = new FaceVector; }
  IntVector* detachFace(int id);
  int addFace(Face* f);
  virtual bool isMultiFace() const { return true; }
  virtual ~MultiFace() { delete comps; }
  int componentCount() { return comps->size(); }
private:
  void updateFaces(double z);
  void storeFaces();
  void refineFace(Face* f, Face* target);
  bool vertexInside(int vid);
  bool vertexNearestIntersect(const Vector2Dd A, const Vector2Dd B, Vector2Dd &P, int &index, Face* face);
  int pickRemovalIndex(Face *f, IntVector* visited);
  Face* getOtherFaceWithVertex(Face* f, int vid);
  bool isLeftOfVectors(int x, int a, int b, int c);
};


#endif /* __MULTIFACE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
