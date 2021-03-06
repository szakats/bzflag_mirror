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

// Phoenix.cpp : Defines the entry point for the DLL application.
//

#include "bzfsAPI.h"
#include "plugin_utils.h"
#include <map>

typedef struct
{
   float x,y,z,a;
}trDeathPos;

std::map<int,trDeathPos>  lastDeaded;

class PhoenixEvents : public bz_EventHandler
{
  virtual void process ( bz_EventData *eventData )
  {
    switch(eventData->eventType)
    {
      case bz_eCaptureEvent:
	lastDeaded.clear();
	break;

      case bz_ePlayerDieEvent:
	{
	  bz_PlayerDieEventData_V1* data = (bz_PlayerDieEventData_V1*)eventData;

	  trDeathPos pos;
	  pos.x = data->state.pos[0];
	  pos.y = data->state.pos[1];
	  pos.z = data->state.pos[2];
	  pos.a = data->state.rotation;

	  lastDeaded[data->playerID] = pos;
	}
      break;

      case bz_eGetPlayerSpawnPosEvent:
	{
	  bz_GetPlayerSpawnPosEventData_V1* data = (bz_GetPlayerSpawnPosEventData_V1*)eventData;

	  if (lastDeaded.find(data->playerID) == lastDeaded.end())
	    break;

	  trDeathPos &pos = lastDeaded[data->playerID];

	  data->handled = true;
	  data->pos[0] = pos.x;
	  data->pos[1] = pos.y;
	  data->pos[2] = pos.z;
	  data->rot = pos.a;
	}
	break;

      case bz_ePlayerPartEvent:
	{
	  bz_PlayerJoinPartEventData_V1* data = (bz_PlayerJoinPartEventData_V1*)eventData;
	  if (lastDeaded.find(data->playerID) != lastDeaded.end())
	    lastDeaded.erase(lastDeaded.find(data->playerID));
	}
	break;
    }
  }
};

PhoenixEvents phoenixEvents;

BZ_GET_PLUGIN_VERSION

BZF_PLUGIN_CALL int bz_Load ( const char* /*commandLine*/ )
{
  lastDeaded.clear();

  bz_registerEvent(bz_ePlayerDieEvent,&phoenixEvents);
  bz_registerEvent(bz_eGetPlayerSpawnPosEvent,&phoenixEvents);
  bz_registerEvent(bz_ePlayerPartEvent,&phoenixEvents);
  bz_registerEvent(bz_eCaptureEvent,&phoenixEvents);

  bz_debugMessage(4,"Phoenix plugin loaded");
  return 0;
}

BZF_PLUGIN_CALL int bz_Unload ( void )
{
  bz_removeEvent(bz_ePlayerDieEvent,&phoenixEvents);
  bz_removeEvent(bz_eGetPlayerSpawnPosEvent,&phoenixEvents);
  bz_removeEvent(bz_ePlayerPartEvent,&phoenixEvents);
  bz_removeEvent(bz_eCaptureEvent,&phoenixEvents);

  lastDeaded.clear();
  bz_debugMessage(4,"Phoenix plugin unloaded");
  return 0;
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
