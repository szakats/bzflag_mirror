// commonItems.cpp : Defines the entry point for the DLL application.
//

#include "commonItems.h"
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>

UserInfo *userInfo = NULL;
Error *error = NULL;
GameInfo *gameInfo = NULL;

void initCommonItems ( Templateiser &ts )
{
  userInfo = new UserInfo(ts);
  error = new Error(ts);
  gameInfo = new GameInfo(ts);
}

std::vector<NewPageCallback*> newPageCallbacks;

void addNewPageCallback ( NewPageCallback *callback )
{
  if (callback)
    newPageCallbacks.push_back(callback);
}

void removeNewPageCallback ( NewPageCallback *callback )
{
  std::vector<NewPageCallback*> ::iterator itr = newPageCallbacks.begin();
  while (itr != newPageCallbacks.end())
  {
    if (*itr == callback)
    {
      newPageCallbacks.erase(itr);
      return;
    }
    itr++;
  }
}

void callNewPageCallbacks (  const std::string &pagename, const HTTPRequest &request )
{
  std::vector<NewPageCallback*> ::iterator itr = newPageCallbacks.begin();
  while (itr != newPageCallbacks.end())
    (*itr++)->newPage(pagename,request);
}

//----------------UserInfo
UserInfo::UserInfo(Templateiser &ts)
{
  ts.addKey("username",this);
}

void UserInfo::keyCallback (std::string &data, const std::string &/*key*/)
{
  data += userName;
}

//--------------Error
Error::Error(Templateiser &ts)
{
  ts.addKey("error",this);
  ts.addIF("error",this);
}

void Error::keyCallback (std::string &data, const std::string &/*key*/)
{
  data += errorMessage;
}

bool Error::ifCallback (const std::string &/*key*/)
{
  return errorMessage.size();
}

//--------------GameInfo

GameInfo::GameInfo(Templateiser &ts)
{
  ts.addKey("MapFile",this);
  ts.addKey("RedTeamScore",this);
  ts.addKey("RedTeamWins",this);
  ts.addKey("RedTeamLosses",this);
  ts.addKey("GreenTeamScore",this);
  ts.addKey("GreenTeamWins",this);
  ts.addKey("GreenTeamLosses",this);
  ts.addKey("BlueTeamScore",this);
  ts.addKey("BlueTeamWins",this);
  ts.addKey("BlueTeamLosses",this);
  ts.addKey("PurpleTeamScore",this);
  ts.addKey("PurpleTeamWins",this);
  ts.addKey("PurpleTeamLosses",this);

  bz_registerEvent(bz_eGetWorldEvent,this);

  mapFile = "Unknown";
}

void GameInfo::keyCallback (std::string &data, const std::string &key)
{
  if (key == "mapfile")
    data += mapFile;
  else if ( key == "redteamscore")
    data += format("%d",bz_getTeamScore(eRedTeam));
  else if ( key == "redteamwins")
    data += format("%d",bz_getTeamWins(eRedTeam));
  else if ( key == "redteamlosses")
    data += format("%d",bz_getTeamLosses(eRedTeam));
  else if ( key == "greenteamscore")
    data += format("%d",bz_getTeamScore(eGreenTeam));
  else if ( key == "greenteamwins")
    data += format("%d",bz_getTeamWins(eGreenTeam));
  else if ( key == "greenteamlosses")
    data += format("%d",bz_getTeamLosses(eGreenTeam));
  else if ( key == "blueteamscore")
    data += format("%d",bz_getTeamScore(eBlueTeam));
  else if ( key == "blueteamwins")
    data += format("%d",bz_getTeamWins(eBlueTeam));
  else if ( key == "blueteamlosses")
    data += format("%d",bz_getTeamLosses(eBlueTeam));
  else if ( key == "purpleteamscore")
    data += format("%d",bz_getTeamScore(ePurpleTeam));
  else if ( key == "purpletteamwins")
    data += format("%d",bz_getTeamWins(ePurpleTeam));
  else if ( key == "purpletteamlosses")
    data += format("%d",bz_getTeamLosses(ePurpleTeam));
}

void GameInfo::process(bz_EventData *eventData)
{
  if (eventData->eventType == bz_eGetWorldEvent)
  {
    bz_GetWorldEventData_V1 *data = (bz_GetWorldEventData_V1*)eventData;
    mapFile = data->worldFile.c_str();
    if (!mapFile.size())
    {
      if (data->worldBlob)
	mapFile = "Custom";
      else
	mapFile = "Random";
    }
  }
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
