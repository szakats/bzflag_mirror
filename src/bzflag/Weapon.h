/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <vector>
#include "Flag.h"

class Weapon {
  public:
    Weapon() {
      type = Flags::Null;
      pos[0] = pos[1] = pos[2] = 0.0f;
      dir = 0.0f;
      initDelay = 0.0f;
      delay.clear();
    }
    FlagType *type;
    float pos[3];
    float dir;
    float initDelay;
    std::vector<float> delay;
};


#endif /* __WEAPON_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
