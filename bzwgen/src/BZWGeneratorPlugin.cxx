/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifdef COMPILE_PLUGIN
#include "BZWGeneratorPlugin.h"
#include <sstream>

typedef std::ostringstream OutStringStream;

void BZWGeneratorPlugin::process(bz_EventData *eventData) {
  if (eventData->eventType == bz_eWorldFinalized) {
    // This bothers me because this pointer has been given
    // out. Potential pointer into freed memory...
    delete[] cstr;
    cstr = 0;
    return;
  }

  if (eventData->eventType != bz_eGetWorldEvent)
    return;


  Logger.log( 2, "BZWGeneratorPlugin : bz_eGetWorldEvent called!" );

  if (worldGenerated) {
    bz_GetWorldEventData_V1 *getWorldData = (bz_GetWorldEventData_V1 *) eventData;
    getWorldData->worldBlob = cstr;
    getWorldData->generated = true;
    return;
  }

  OutStringStream outstream(OutStringStream::out);
  generate(&outstream);

  bz_GetWorldEventData_V1 *getWorldData = (bz_GetWorldEventData_V1 *) eventData;

  // The following code is so ugly that I barely can look at it.
  delete[] cstr;
  cstr = new char [outstream.str().size()+1];
  strcpy (cstr, outstream.str().c_str());
  getWorldData->worldBlob = cstr;
  getWorldData->generated = true;
  worldGenerated = true;
}
#endif // COMPILE_PLUGIN


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

