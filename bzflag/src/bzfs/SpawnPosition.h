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

// object that creates and contains a spawn position

#ifndef __SPAWNPOSITION_H__
#define __SPAWNPOSITION_H__

#include "PlayerInfo.h"

class SpawnPosition {

public:
  SpawnPosition(int playerId, bool onGroundOnly, bool notNearEdges);
  ~SpawnPosition();

  float getX() const;
  float getY() const;
  float getZ() const;
  float getAzimuth() const;

private:
  float enemyProximityCheck(float &enemyAngle) const;
  float distanceFrom(const float* farPos) const;
  bool  isImminentlyDangerous() const;
  bool  isFacing(const float *enemyPos, const float enemyAzimuth,
		       const float deviation) const;

  float	      azimuth;
  float       pos[3];

  TeamColor   team;
  float       testPos[3];
  int	      curMaxPlayers;

  float	      safeSWRadius;
  float	      safeSRRadius;
  float	      safeDistance;

};

inline float SpawnPosition::getX() const
{
  return pos[0];
}

inline float SpawnPosition::getY() const
{
  return pos[1];
}

inline float SpawnPosition::getZ() const
{
  return pos[2];
}

inline float SpawnPosition::getAzimuth() const
{
  return azimuth;
}

#endif  //__SPAWNPOSITION_H__

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
