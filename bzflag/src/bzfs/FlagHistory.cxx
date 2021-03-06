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

/* interface header */
#include "FlagHistory.h"

/* system headers */
#include <vector>
#include <string.h>

#define MAX_FLAG_HISTORY (10)

void FlagHistory::clear()
{
  flagHistory.clear();
}

void FlagHistory::get(char message[])
{
    char flag[MessageLen];
    std::vector<FlagType*>::iterator fhIt = flagHistory.begin();

    while (fhIt != flagHistory.end()) {
      FlagType * fDesc = (FlagType*)(*fhIt);
      if (fDesc->endurance == FlagNormal)
	snprintf(flag, MessageLen, "(*%c) ", fDesc->flagName.c_str()[0]);
      else
	snprintf(flag, MessageLen, "(%s) ", fDesc->flagAbbv.c_str());
      strncat(message, flag, sizeof(message) - strlen(message) - 1);
      fhIt++;
    }
}

void FlagHistory::add(FlagType* type)
{
  if (flagHistory.size() >= MAX_FLAG_HISTORY)
    flagHistory.erase(flagHistory.begin());
  flagHistory.push_back(type);
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
