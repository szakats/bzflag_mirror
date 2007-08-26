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
  MultiFace(Mesh* _mesh) : Face(), mesh(_mesh) {
    comps = new FaceVector;
  }
  IntVector* detachFace(int id);
  void refineFace(Face* f);
  int addFace(Face* f);
  bool vertexInside(int vid);
  bool vertexNearestIntersect(int begin, int end, Vertex &P, int &index);
  virtual bool isMultiFace() { return true; }
  virtual ~MultiFace() {
    delete comps;
  }
};


#endif /* __MULTIFACE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
