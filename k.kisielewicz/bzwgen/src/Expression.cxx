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

#include "Expression.h"
#include "RuleSet.h"

double ExpressionAttribute::calculate(Mesh*,int) {
  return ruleset->getAttr(attrname);
}

double ExpressionFaceAttribute::calculate(Mesh* mesh,int face) {
  if (attrname == "x") return mesh->faceCenter(face).x;
  if (attrname == "y") return mesh->faceCenter(face).y;
  if (attrname == "z") return mesh->faceCenter(face).z;
  if (attrname == "h") return mesh->faceH(face);
  if (attrname == "v") return mesh->faceV(face);
  if (attrname == "s") return mesh->faceH(face) * mesh->faceV(face);
  if (attrname == "n") return double(mesh->getFace(face)->size());
  if (attrname == "c") {
    if (mesh->getFace(face)->isMultiFace()) {
      return double(((MultiFace*)mesh->getFace(face))->componentCount());
    } else {
      Logger.log( 2, "Warning : face(c) called with non-MultiFace!");
    }
  }
  Logger.log( 2, "Warning : Unknown face() attribute : '%s'!\n", attrname.c_str() );
  return 0.0;
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
