/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "PyramidSceneNodeGenerator.h"
#include "PyramidBuilding.h"
#include "bzfgl.h"
#include "TriWallSceneNode.h"
#include "QuadWallSceneNode.h"

//
// PyramidSceneNodeGenerator
//

PyramidSceneNodeGenerator::PyramidSceneNodeGenerator(
				const PyramidBuilding* _pyramid) :
				pyramid(_pyramid)
{
  // do nothing
}

PyramidSceneNodeGenerator::~PyramidSceneNodeGenerator()
{
  // do nothing
}

WallSceneNode*		PyramidSceneNodeGenerator::getNextNode(
				float uRepeats, float vRepeats, bool lod)
{

  bool isQuad = false;

  if (getNodeNumber() == 5) return NULL;

  fvec3 base, sCorner, tCorner;
  if (pyramid->getZFlip()) {
    switch (incNodeNumber()) {
      case 1:
	pyramid->getCorner(4, base);
	pyramid->getCorner(1, sCorner);
	pyramid->getCorner(0, tCorner);
	isQuad = false;
	break;
      case 2:
	pyramid->getCorner(4, base);
	pyramid->getCorner(2, sCorner);
	pyramid->getCorner(1, tCorner);
	isQuad = false;
	break;
      case 3:
	pyramid->getCorner(4, base);
	pyramid->getCorner(3, sCorner);
	pyramid->getCorner(2, tCorner);
	isQuad = false;
	break;
      case 4:
	pyramid->getCorner(4, base);
	pyramid->getCorner(0, sCorner);
	pyramid->getCorner(3, tCorner);
	isQuad = false;
	break;
      case 5:
	pyramid->getCorner(0, base);
	pyramid->getCorner(1, sCorner);
	pyramid->getCorner(3, tCorner);
	isQuad = true;
	break;
    }
  } else {
    switch (incNodeNumber()) {
      case 1:
	pyramid->getCorner(0, base);
	pyramid->getCorner(1, sCorner);
	pyramid->getCorner(4, tCorner);
	isQuad = false;
	break;
      case 2:
	pyramid->getCorner(1, base);
	pyramid->getCorner(2, sCorner);
	pyramid->getCorner(4, tCorner);
	isQuad = false;
	break;
      case 3:
	pyramid->getCorner(2, base);
	pyramid->getCorner(3, sCorner);
	pyramid->getCorner(4, tCorner);
	isQuad = false;
	break;
      case 4:
	pyramid->getCorner(3, base);
	pyramid->getCorner(0, sCorner);
	pyramid->getCorner(4, tCorner);
	isQuad = false;
	break;
      case 5:
	if ((pyramid->getPosition()[2] > 0.0f) || pyramid->getZFlip()) {
	  pyramid->getCorner(0, base);
	  pyramid->getCorner(3, sCorner);
	  pyramid->getCorner(1, tCorner);
	  isQuad = true;
	} else {
	  return NULL;
	}
	break;
    }
  }

  const fvec3 sEdge = sCorner - base;
  const fvec3 tEdge = tCorner - base;

  if (isQuad == false) {
    return new TriWallSceneNode(base, sEdge, tEdge, uRepeats, vRepeats, lod);
  } else {
    return new QuadWallSceneNode(base, sEdge, tEdge, uRepeats, vRepeats, lod);
  }
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
