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

#ifndef __RANDOMSPAWNPOLICY_H__
#define __RANDOMSPAWNPOLICY_H__

// interface header
#include "SpawnPolicy.h"

// common headers
#include "vectors.h"


/** a RandomSpawnPolicy is a SpawnPolicy that just generates a purely
 *  random (yet hopefully still valid) position and drops you there.
 */
class RandomSpawnPolicy : public SpawnPolicy
{
public:
  RandomSpawnPolicy();
  virtual ~RandomSpawnPolicy();

  virtual const char *Name() const {
    static const char *name = "Random";
    return name;
  }

  virtual void getPosition(fvec3& pos, int playerId, bool onGroundOnly, bool notNearEdges);
  virtual void getAzimuth(float &azimuth);
};

#endif  /*__RANDOMSPAWNPOLICY_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
