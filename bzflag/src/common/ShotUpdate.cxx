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

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "common.h"
#include "ShotUpdate.h"

//
// ShotUpdate
//

void*			ShotUpdate::pack(void* buf) const
{
  buf = nboPackUByte(buf, player);
  buf = nboPackUShort(buf, id);
  buf = nboPackVector(buf, pos);
  buf = nboPackVector(buf, vel);
  buf = nboPackFloat(buf, dt);
  buf = nboPackShort(buf, team);
  return buf;
}

void*			ShotUpdate::unpack(void* buf)
{
  buf = nboUnpackUByte(buf, player);
  buf = nboUnpackUShort(buf, id);
  buf = nboUnpackVector(buf, pos);
  buf = nboUnpackVector(buf, vel);
  buf = nboUnpackFloat(buf, dt);
  short temp;
  buf = nboUnpackShort(buf, temp);
  team = (TeamColor)temp;
  return buf;
}

//
// FiringInfo
//

FiringInfo::FiringInfo()
{
  // do nothing -- must be prepared before use by unpack() or assignment
}

void*			FiringInfo::pack(void* buf) const
{
  buf = shot.pack(buf);
  buf = flagType->pack(buf);
  buf = nboPackFloat(buf, lifetime);
  return buf;
}

void*			FiringInfo::unpack(void* buf)
{
  buf = shot.unpack(buf);
  buf = FlagType::unpack(buf, flagType);
  buf = nboUnpackFloat(buf, lifetime);
 return buf;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
