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

#ifndef __SPAWNPOLICY_H__
#define __SPAWNPOLICY_H__

#include "common.h"

// common headers
#include "vectors.h"


/** a SpawnPolicy is used to determine a new SpawnPosition.  Policies
 *  are defined to describe various spawning behaviors such as purely
 *  random, aggressive, or classical behavior.  Factors that can be
 *  taken into account are the proximity to other players, bullets,
 *  flags, etc.
 */
class SpawnPolicy
{
public:
  SpawnPolicy();
  virtual ~SpawnPolicy();

  virtual const char *Name() const = 0;

  virtual void getPosition(fvec3& pos, int playerId,
                           bool onGroundOnly, bool notNearEdges) = 0;
  virtual void getAzimuth(float &azimuth) = 0;

protected:
  virtual bool isFacing(const fvec3& selfPos, const fvec3& enemyPos,
                        const float enemyAzimuth, const float deviation) const;
  virtual bool isImminentlyDangerous(const fvec3& selfPos) const;
};

#endif  /*__SPAWNPOLICY_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
