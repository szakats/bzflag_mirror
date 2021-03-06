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

#ifdef _MSC_VER
#pragma warning( 4: 4786 )
#endif

#include "common.h"
#include <math.h>
#include <string.h>

#include "WorldFileObstacle.h"

WorldFileObstacle::WorldFileObstacle()
{
  driveThrough = 0;
  shootThrough = 0;
  ricochet = false;
}


bool WorldFileObstacle::read(const char *cmd, std::istream& input)
{
  if (strcasecmp(cmd, "drivethrough") == 0) {
    driveThrough = 0xFF;
  }
  else if (strcasecmp(cmd, "shootthrough") == 0) {
    shootThrough = 0xFF;
  }
  else if (strcasecmp(cmd, "passable") == 0) {
    driveThrough = shootThrough = 0xFF;
  }
  else if (strcasecmp(cmd, "ricochet") == 0) {
    ricochet = true;
  }
  else {
    return WorldFileLocation::read(cmd, input);
  }
  return true;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
