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

// interface header
#include "RCRobotPlayer.h"

// common implementation headers
#include "BZDBCache.h"

// local implementation headers
#include "World.h"
#include "Intersect.h"
#include "TargetingUtils.h"


RCRobotPlayer::RCRobotPlayer(const PlayerId& _id,
			     const char* _name,
			     ServerLink* _server) :
  RobotPlayer(_id, _name, _server),
  lastTickAt(0.0),
  tickDuration(2.0),
  speed(1.0),
  nextSpeed(1.0),
  turnRate(1.0),
  nextTurnRate(1.0),
  shoot(false),
  distanceRemaining(0.0),
  nextDistance(0.0),
  turnRemaining(0.0),
  nextTurn(0.0),
  hasStopped(false)
{
}


void RCRobotPlayer::doUpdate(float dt)
{
  LocalPlayer::doUpdate(dt);
}


void RCRobotPlayer::doUpdateMotion(float dt)
{
  if (isAlive()) {
    double timeNow = TimeKeeper::getCurrent().getSeconds();
    /* Is the tick still running? */
    if (lastTickAt + tickDuration >= timeNow) {
      const float *vel = getVelocity();
      distanceRemaining -= sqrt(vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2]) * dt;
      if (distanceRemaining > 0.0) {
        if (distanceForward)
          setDesiredSpeed((float)speed);
        else
          setDesiredSpeed((float)-speed);
      } else {
        setDesiredSpeed(0);
      }

      if (turnRemaining > 0.0) {
        if (turnLeft) {
          turnRemaining -= getAngularVelocity() * dt;

          if (turnRemaining <= 0.0)
            setDesiredAngVel(0);
          else if (turnRate * dt > turnRemaining)
            setDesiredAngVel((float)turnRemaining/dt);
          else
            setDesiredAngVel((float)turnRate);
        } else {
          turnRemaining += getAngularVelocity() * dt;
          if (turnRemaining <= 0.0)
            setDesiredAngVel(0);
          else if (turnRate * dt > turnRemaining)
            setDesiredAngVel((float)-turnRemaining/dt);
          else
            setDesiredAngVel((float)-turnRate);
        }
      } else {
        setDesiredAngVel(0);
      }
    } else {
      /*setDesiredAngVel(0);
      setDesiredSpeed(0);*/
    }

  }
  LocalPlayer::doUpdateMotion(dt);
}


void RCRobotPlayer::explodeTank()
{
  LocalPlayer::explodeTank();
}


void RCRobotPlayer::restart(const double* _pos, double _azimuth)
{
  fvec3 pos(_pos[0], _pos[1], _pos[2]);;
  LocalPlayer::restart(pos, (float)_azimuth);
}


bool RCRobotPlayer::isSteadyState()
{
  double timeNow = TimeKeeper::getCurrent().getSeconds();
  /* last tick done? */
  if (lastTickAt + tickDuration <= timeNow)
    return true;
  return false;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
