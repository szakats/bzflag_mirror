/* bzflag
 * Copyright (c) 1993 - 2003 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "common.h"
#include "RemotePlayer.h"
#include "World.h"

RemotePlayer::RemotePlayer(const PlayerId& id, TeamColor team,
					const char* name, const char* email,
			   const PlayerType type) :
				Player(id, team, name, email, type)
{
  numShots = World::getWorld()->getMaxShots();
  shots = new RemoteShotPath*[numShots];
  for (int i = 0; i < numShots; i++)
    shots[i] = NULL;
}

RemotePlayer::~RemotePlayer()
{
  for (int i = 0; i < numShots; i++)
    delete shots[i];
  delete[] shots;
}

void			RemotePlayer::addShot(const FiringInfo& info)
{
  float newpos[3];
  const float *f = getForward();
  RemoteShotPath* newShot = new RemoteShotPath(info);
  int shotNum = int(newShot->getShotId() & 255);
  if (shots[shotNum]) delete shots[shotNum];
  shots[shotNum] = newShot;
  // Update tanks position and set dead reckoning for better lag handling
  // shot origin is center of tank for shockwave
  if (info.flag == Flags::ShockWave) {
    newpos[0] = info.shot.pos[0];
    newpos[1] = info.shot.pos[1];
    newpos[2] = info.shot.pos[2];
  }
  // shot origin is muzzle for other shots
  else {
	  float front = BZDB->eval(StateDatabase::BZDB_MUZZLEFRONT);
    if (info.flag == Flags::Obesity) front *= BZDB->eval(StateDatabase::BZDB_OBESEFACTOR);
    else if (info.flag == Flags::Tiny) front *= BZDB->eval(StateDatabase::BZDB_TINYFACTOR);
    else if (info.flag == Flags::Thief) front *= BZDB->eval(StateDatabase::BZDB_THIEFTINYFACTOR);
    newpos[0] = info.shot.pos[0]-(front * f[0]);
    newpos[1] = info.shot.pos[1]-(front * f[1]);
    newpos[2] = info.shot.pos[2]-(front * f[2])-BZDB->eval(StateDatabase::BZDB_MUZZLEHEIGHT);
  }
  move(newpos, getAngle());
  setDeadReckoning();
}

ShotPath*		RemotePlayer::getShot(int index) const
{
  return shots[index & 255];
}

bool			RemotePlayer::doEndShot(
				int id, bool isHit, float* pos)
{
  const int index = id & 255;
  const int salt = (id >> 8) & 127;

  // special id used in some messages (and really shouldn't be sent here)
  if (id == -1)
    return false;

  // ignore bogus shots (those with a bad index or for shots that don't exist)
  if (index < 0 || index >= World::getWorld()->getMaxShots() || !shots[index])
    return false;

  // ignore shots that already ending
  if (shots[index]->isExpired() || shots[index]->isExpiring())
    return false;

  // ignore shots that have the wrong salt.  since we reuse shot indices
  // it's possible for an old MsgShotEnd to arrive after we've started a
  // new shot.  that's where the salt comes in.  it changes for each shot
  // so we can identify an old shot from a new one.
  if (salt != ((shots[index]->getShotId() >> 8) & 127))
    return false;

  // don't stop if it's because were hitting something and we don't stop
  // when we hit something.
  if (isHit && !shots[index]->isStoppedByHit())
    return false;

  // end it
  const float* shotPos = shots[index]->getPosition();
  pos[0] = shotPos[0];
  pos[1] = shotPos[1];
  pos[2] = shotPos[2];
  shots[index]->setExpired();
  return true;
}

void			RemotePlayer::updateShots(float dt)
{
  for (int i = 0; i < numShots; i++)
    if (shots[i])
      shots[i]->update(dt);
}

// Local variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

