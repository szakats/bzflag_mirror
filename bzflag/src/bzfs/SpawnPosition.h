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

// object that creates and contains a spawn position

#ifndef __SPAWNPOSITION_H__
#define __SPAWNPOSITION_H__

#include "common.h"

// common headers
#include "vectors.h"

// local headers
#include "SpawnPolicy.h"


/** SpawnPosition represents a single spawn position and encapsulates
 *  the logic for selecting that new position based on the map, world
 *  configuration settings, and current game activity.
 */
class SpawnPosition {

public:
  SpawnPosition(int playerId, bool onGroundOnly, bool notNearEdges);
  ~SpawnPosition();

  /** used to override the default spawn policy.  call this before
   *  creating SpawnPosition objects.
   */
  static void SetSpawnPolicy(SpawnPolicy* policy);

  float getX() const;
  float getY() const;
  float getZ() const;
  float getAzimuth() const;

private:
  float	      azimuth;
  fvec3       pos;

  /* class data - determines how the pos and azimuth are determined */
  static SpawnPolicy *policy;
};

inline float SpawnPosition::getX() const
{
  return pos.x;
}

inline float SpawnPosition::getY() const
{
  return pos.y;
}

inline float SpawnPosition::getZ() const
{
  return pos.z;
}

/** returns the rotational orientation of this spawn position, from
 *  facing 'North' in radians.
 */
inline float SpawnPosition::getAzimuth() const
{
  return azimuth;
}

#else
class SpawnPosition;
#endif  /* __SPAWNPOSITION_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
