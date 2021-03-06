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

#include "common.h"

/* system headers */
#include <string>
#include <string.h>

/* interface header */
#include "ServerItem.h"

/* common implementation headers */
#include "TextUtils.h"
#include "Protocol.h"
#include "bzfio.h"

/* local implementation headers */
#include "ServerListCache.h"


ServerItem::ServerItem() :  updateTime(0), cached(false), favorite(false), recent(false), recentTime(0)
{
}

ServerItem::~ServerItem()
{
  return;
}

void ServerItem::writeToFile(std::ostream& out) const
{
  char buffer[ServerListCache::max_string+1]; // ServerListCache::max_string is inherited from ServerListCache.h

  // write out desc.
  memset(buffer,0,sizeof(buffer));
  int copyLength = int(description.size() < ServerListCache::max_string ? description.size(): ServerListCache::max_string);
  strncpy(&buffer[0],description.c_str(),copyLength);
  out.write(buffer,sizeof(buffer));

  // write out name
  memset(buffer,0,sizeof(buffer));
  copyLength = int(name.size() < ServerListCache::max_string ? name.size(): ServerListCache::max_string);
  strncpy(&buffer[0],name.c_str(),copyLength);
  out.write(buffer,sizeof(buffer));

  // write out pingpacket
  ping.writeToFile(out);

  // write out favorite status
  nboPackUInt8(buffer, favorite);
  out.write(buffer, 1);

  // write out recent status
  nboPackUInt8(buffer, recent);
  out.write(buffer, 1);

  // write out recent time
  memset(buffer,0,sizeof(buffer));
  nboPackInt32(buffer,(int32_t)recentTime);
  out.write(&buffer[0], 4);

  // write out current time
  memset(buffer,0,sizeof(buffer));
  nboPackInt32(buffer,(int32_t)updateTime);
  out.write(&buffer[0], 4);
}

std::string ServerItem::getServerKey() const
{
  // Get the server's server key
  std::string serverKey = name;
  const unsigned int serverPort = (int)ntohs((unsigned short)port);
  if (serverPort != ServerPort) {
    char portBuf[20];
    sprintf(portBuf, "%d", serverPort);
    serverKey += ":";
    serverKey += portBuf;
  }
  return serverKey;
}

bool ServerItem::readFromFile(std::istream& in)
{
  char buffer[ServerListCache::max_string+1];

  //read description
  memset(buffer,0,sizeof(buffer));
  in.read(buffer,sizeof(buffer));
  if ((size_t)in.gcount() < sizeof(buffer)) return false; // failed to read entire string
  description = buffer;

  //read name
  memset(buffer,0,sizeof(buffer));
  in.read(buffer,sizeof(buffer));
  if ((size_t)in.gcount() < sizeof(buffer)) return false; // failed to read entire string
  name = buffer;

  bool pingWorked = ping.readFromFile(in);
  if (!pingWorked) return false; // pingpacket failed to read

  // read in favorite flag
  uint8_t fav;
  in.read(buffer, 1);
  nboUnpackUInt8(buffer, fav);
  favorite = (fav != 0);

  // read in recent flag
  uint8_t rec;
  in.read(buffer, 1);
  nboUnpackUInt8(buffer, rec);
  recent = (rec != 0);

  // read in recent time
  in.read(&buffer[0],4);
  if (in.gcount() < 4) return false;
  int32_t rTime;
  nboUnpackInt32(&buffer[0],rTime);
  recentTime = (time_t) rTime;

  // recent server from more than 10 days ago, unmark as recent
  if ((recent)&&((getNow() - recentTime) >= 60*60*24*10)) {
    recent = false;
    recentTime = 0;
  }

  // read in time
  in.read(&buffer[0],4);
  if (in.gcount() < 4) return false;
  int32_t theTime;
  nboUnpackInt32(&buffer[0],theTime);
  updateTime = (time_t) theTime;
  cached = true;
  return true;
}

// set the last updated time to now
void ServerItem::resetAge()
{
  updateTime = getNow();
}

/* set the age of this item (useful for updating entries while keeping
 * the server list sorted)
 */
void ServerItem::setAge(time_t minutes, time_t seconds)
{
  updateTime = getNow() - (minutes * 60) - seconds;
}

// get current age in minutes
time_t ServerItem::getAgeMinutes() const
{
  time_t time = (getNow() - updateTime)/(time_t)60;
  return time;
}

// get current age in seconds
time_t ServerItem::getAgeSeconds() const
{
  time_t time = (getNow() - updateTime);
  return time;
}

// get a simple string which describes the age of item
std::string ServerItem::getAgeString() const
{
  std::string returnMe;
  char buffer [80];
  time_t age = getAgeMinutes();
  float fAge;
  if (age < 60){ // < 1 hr
    if (age < 1){
      time_t ageSecs = getAgeSeconds();
      sprintf(buffer,"%-3ld secs",(long)ageSecs);
    } else {
      sprintf(buffer,"%-3ld mins",(long)age);
    }
  } else { // >= 60 minutes
    if (age < (24*60)){ // < 24 hours & > 1 hr
      fAge = ((float)age / 60.0f);
      sprintf(buffer, "%-2.1f hrs", fAge);
    } else  { // > 24 hrs
      if (age < (24*60*99)){  // > 1 day & < 99 days
	fAge = ((float) age / (60.0f*24.0f));
	sprintf(buffer, "%-2.1f days", fAge);
      } else { // over 99 days
	fAge = ((float) age / (60.0f*24.0f));
	sprintf(buffer, "%-3f days", fAge);  //should not happen
      }
    }
  }
  returnMe = buffer;
  return returnMe;
}

// get the current time
time_t ServerItem::getNow() const
{
#if defined(_WIN32)
  return time(NULL);
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec;
#endif
}

int ServerItem::getPlayerCount() const
{
  // if null ping we return a 0 player count
  int curPlayer = 0;
  if (&ping != 0) {
    int maxPlayer = ping.maxPlayers;
    curPlayer = ping.rogueCount + ping.redCount + ping.greenCount +
      ping.blueCount + ping.purpleCount + ping.observerCount;
    if (curPlayer > maxPlayer)
      curPlayer = maxPlayer;
  }
  return curPlayer;
}

std::string ServerItem::getAddrName() const
{
  if (name.size() <= 0) {
    return std::string("");
  }
  return TextUtils::format("%s:%d", name.c_str(), ntohs(ping.serverId.port));
}

unsigned int ServerItem::getSortFactor() const
{
  // if null ping we return a 0 player count
  unsigned int value = 0;
  if (&ping != 0) {
    // real players are worth a 1000
    value = ping.rogueCount + ping.redCount + ping.greenCount +
	    ping.blueCount + ping.purpleCount;
    value *= 10000;
    // include the lowly observers, 1 point each
    value += ping.observerCount;
  }
  return value;
}


bool operator<(const ServerItem &left, const ServerItem &right)
{
  // sorted "from least to greatest" in the list
  // so "return true" to go up, "return false" to go down

  // cached servers go to the bottom of the list
  if (left.cached && !right.cached) {
    return false;
  } else if (!left.cached && right.cached) {
    return true;
  }

  // sort by player count - more first
  if (left.getSortFactor() > right.getSortFactor()) {
    return true;
  } else if (left.getSortFactor() < right.getSortFactor()) {
    return false;
  }

  // sort by age - youngest first
  // note that noncached servers always have equal ages
  time_t ageLeft = (left.getAgeMinutes() * 60) + left.getAgeSeconds();
  time_t ageRight = (right.getAgeMinutes() * 60) + right.getAgeSeconds();
  if (ageLeft < ageRight) {
    return true;
  } else if (ageLeft > ageRight) {
    return false;
  }

  // hostname alphabetic sort - first goes first
  if (left.name < right.name) {
    return true;
  } else if (left.name > right.name) {
    return false;
  }

  // sort by port - default first, then lowest to highest
  if (left.port == ServerPort) {
    return true;
  } else if (right.port == ServerPort) {
    return false;
  } else if (left.port < right.port) {
    return true;
  } else if (left.port > right.port) {
    return false;
  }

  logDebugMessage(0, "Error: operator<: equality detected.\n");
  return false; // arbitrary
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
