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

/* interface header */

#include "ClientIntangibilityManager.h"
#include "ObstacleMgr.h"

template <>
ClientIntangibilityManager* Singleton<ClientIntangibilityManager>::_instance = (ClientIntangibilityManager*)0;

void ClientIntangibilityManager::setWorldObjectTangibility ( unsigned int objectGUID, unsigned char tangible )
{
  tangibilityMap[objectGUID] = tangible;
}

unsigned char ClientIntangibilityManager::getWorldObjectTangiblity ( unsigned int objectGUID )
{
  std::map<unsigned int, unsigned char>::iterator itr = tangibilityMap.find(objectGUID);
  if ( itr != tangibilityMap.end())
    return itr->second;

  Obstacle *obs = OBSTACLEMGR.getObstacleFromID(objectGUID);
  if (!obs)
    return _INVALID_TANGIBILITY;

  return obs->isDriveThrough();
}

void ClientIntangibilityManager::resetTangibility ( void )
{
  tangibilityMap.clear();
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8