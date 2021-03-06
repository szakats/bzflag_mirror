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
#include "MsgStrings.h"

// system headers
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

// common headers
#include "global.h"
#include "Protocol.h"
#include "Pack.h"
#include "AnsiCodes.h"
#include "TextUtils.h"
#include "Address.h"
#include "StateDatabase.h"
#include "PlayerState.h"
#include "Team.h"
#include "Flag.h"
#include "ShotUpdate.h"


typedef uint8_t  u8;
typedef uint16_t u16;

typedef std::map<u16, std::string> FlagListType;
typedef std::map<u16, std::string> PlayerListType;

typedef struct {
  u16 len;
  u16 code;
  const void *data;
} PacketInfo;


static FlagListType   FlagList;
static PlayerListType PlayerList;
// static bool UseDNS = true;
static bool Colorize = true;
static bool TrackState = true;

static MsgStringList listMsgBasics (PacketInfo *pi);
static void listPush (MsgStringList &list, int level, const char *fmt, ...);
static std::string strFlag (u16 id);
static std::string strFlagStatus (FlagStatus status);
static std::string strTeam (u16 id);
static std::string strPlayer (u16 id);
static std::string strVector (const fvec3& vector);
static std::string strKillReason (int16_t reason);
static std::string strAddress (Address& address);

/*
static std::string strHostName (); // ew, ADNS ?
*/

static MsgStringList handleMsgNull(PacketInfo *pi); // fake packet type
static MsgStringList handleMsgAccept(PacketInfo *pi);
static MsgStringList handleMsgAdminInfo(PacketInfo *pi);
static MsgStringList handleMsgAlive(PacketInfo *pi);
static MsgStringList handleMsgAllow(PacketInfo *pi);
static MsgStringList handleMsgAddPlayer(PacketInfo *pi);
static MsgStringList handleMsgAllowSpawn(PacketInfo *pi);
static MsgStringList handleMsgAutoPilot(PacketInfo *pi);
static MsgStringList handleMsgCapBits(PacketInfo *pi);
static MsgStringList handleMsgCaptureFlag(PacketInfo *pi);
static MsgStringList handleMsgCollide(PacketInfo *pi);
static MsgStringList handleMsgCustomSound(PacketInfo *pi);
static MsgStringList handleMsgCacheURL(PacketInfo *pi);
static MsgStringList handleMsgDropFlag(PacketInfo *pi);
static MsgStringList handleMsgEnter(PacketInfo *pi);
static MsgStringList handleMsgExit(PacketInfo *pi);
static MsgStringList handleMsgFlagType(PacketInfo *pi);
static MsgStringList handleMsgFlagUpdate(PacketInfo *pi);
static MsgStringList handleMsgFetchResources(PacketInfo *pi);
static MsgStringList handleMsgGrabFlag(PacketInfo *pi);
static MsgStringList handleMsgGMUpdate(PacketInfo *pi);
static MsgStringList handleMsgGetWorld(PacketInfo *pi);
static MsgStringList handleMsgGameSettings(PacketInfo *pi);
static MsgStringList handleMsgGameTime(PacketInfo *pi);
static MsgStringList handleMsgHandicap(PacketInfo *pi);
static MsgStringList handleMsgHit(PacketInfo *pi);
static MsgStringList handleMsgJoinServer(PacketInfo *pi);
static MsgStringList handleMsgKilled(PacketInfo *pi);
static MsgStringList handleMsgLagState(PacketInfo *pi);
static MsgStringList handleMsgLimboMessage(PacketInfo *pi);
static MsgStringList handleMsgMessage(PacketInfo *pi);
static MsgStringList handleMsgNewPlayer(PacketInfo *pi);
static MsgStringList handleMsgNearFlag(PacketInfo *pi);
static MsgStringList handleMsgNewRabbit(PacketInfo *pi);
static MsgStringList handleMsgNegotiateFlags(PacketInfo *pi);
static MsgStringList handleMsgPause(PacketInfo *pi);
static MsgStringList handleMsgPlayerInfo(PacketInfo *pi);
static MsgStringList handleMsgPlayerUpdate(PacketInfo *pi);
static MsgStringList handleMsgPlayerUpdateSmall(PacketInfo *pi);
static MsgStringList handleMsgQueryGame(PacketInfo *pi);
static MsgStringList handleMsgQueryPlayers(PacketInfo *pi);
static MsgStringList handleMsgReject(PacketInfo *pi);
static MsgStringList handleMsgRemovePlayer(PacketInfo *pi);
static MsgStringList handleMsgReplayReset(PacketInfo *pi);
static MsgStringList handleMsgShotBegin(PacketInfo *pi);
static MsgStringList handleMsgWShotBegin(PacketInfo *pi);
static MsgStringList handleMsgWhatTimeIsIt(PacketInfo *pi);
static MsgStringList handleMsgScore(PacketInfo *pi);
static MsgStringList handleMsgScoreOver(PacketInfo *pi);
static MsgStringList handleMsgShotEnd(PacketInfo *pi);
static MsgStringList handleMsgSuperKill(PacketInfo *pi);
static MsgStringList handleMsgSetShot(PacketInfo *pi);
static MsgStringList handleMsgSetTeam(PacketInfo *pi);
static MsgStringList handleMsgSetVar(PacketInfo *pi);
static MsgStringList handleMsgTangibilityUpdate(PacketInfo *pi);
static MsgStringList handleMsgTangibilityReset(PacketInfo *pi);
static MsgStringList handleMsgTimeUpdate(PacketInfo *pi);
static MsgStringList handleMsgTeleport(PacketInfo *pi);
static MsgStringList handleMsgTransferFlag(PacketInfo *pi);
static MsgStringList handleMsgTeamUpdate(PacketInfo *pi);
static MsgStringList handleMsgWantWHash(PacketInfo *pi);
static MsgStringList handleMsgWantSettings(PacketInfo *pi);
static MsgStringList handleMsgUDPLinkRequest(PacketInfo *pi);
static MsgStringList handleMsgUDPLinkEstablished(PacketInfo *pi);
static MsgStringList handleMsgServerControl(PacketInfo *pi);
static MsgStringList handleMsgLagPing(PacketInfo *pi);

static MsgStringList handleMsgPingCodeReply(PacketInfo *pi);
static MsgStringList handleMsgPingCodeRequest(PacketInfo *pi);
static MsgStringList handleMsgEchoRequest(PacketInfo *pi);
static MsgStringList handleMsgEchoResponse(PacketInfo *pi);

typedef struct {
  uint16_t code;
  const char *label;
  MsgStringList (*handler)(PacketInfo *pi);
} PacketListEntry;

// ick, a #define, but it cleans this up nicely
#define PACKET_LIST_ENTRY(x) {x, #x, handle ##x}
static PacketListEntry PacketList[] = {
  PACKET_LIST_ENTRY (MsgNull),
  PACKET_LIST_ENTRY (MsgAccept),
  PACKET_LIST_ENTRY (MsgAdminInfo),
  PACKET_LIST_ENTRY (MsgAlive),
  PACKET_LIST_ENTRY (MsgAllow),
  PACKET_LIST_ENTRY (MsgAddPlayer),
  PACKET_LIST_ENTRY (MsgAllowSpawn),
  PACKET_LIST_ENTRY (MsgAutoPilot),
  PACKET_LIST_ENTRY (MsgCapBits),
  PACKET_LIST_ENTRY (MsgCaptureFlag),
  PACKET_LIST_ENTRY (MsgCollide),
  PACKET_LIST_ENTRY (MsgCustomSound),
  PACKET_LIST_ENTRY (MsgCacheURL),
  PACKET_LIST_ENTRY (MsgDropFlag),
  PACKET_LIST_ENTRY (MsgEnter),
  PACKET_LIST_ENTRY (MsgExit),
  PACKET_LIST_ENTRY (MsgFlagType),
  PACKET_LIST_ENTRY (MsgFlagUpdate),
  PACKET_LIST_ENTRY (MsgFetchResources),
  PACKET_LIST_ENTRY (MsgGrabFlag),
  PACKET_LIST_ENTRY (MsgGMUpdate),
  PACKET_LIST_ENTRY (MsgGetWorld),
  PACKET_LIST_ENTRY (MsgGameSettings),
  PACKET_LIST_ENTRY (MsgGameTime),
  PACKET_LIST_ENTRY (MsgHandicap),
  PACKET_LIST_ENTRY (MsgHit),
  PACKET_LIST_ENTRY (MsgJoinServer),
  PACKET_LIST_ENTRY (MsgKilled),
  PACKET_LIST_ENTRY (MsgLagState),
  PACKET_LIST_ENTRY (MsgLimboMessage),
  PACKET_LIST_ENTRY (MsgMessage),
  PACKET_LIST_ENTRY (MsgNewPlayer),
  PACKET_LIST_ENTRY (MsgNearFlag),
  PACKET_LIST_ENTRY (MsgNewRabbit),
  PACKET_LIST_ENTRY (MsgNegotiateFlags),
  PACKET_LIST_ENTRY (MsgPause),
  PACKET_LIST_ENTRY (MsgPlayerInfo),
  PACKET_LIST_ENTRY (MsgPlayerUpdate),
  PACKET_LIST_ENTRY (MsgPlayerUpdateSmall),
  PACKET_LIST_ENTRY (MsgQueryGame),
  PACKET_LIST_ENTRY (MsgQueryPlayers),
  PACKET_LIST_ENTRY (MsgReject),
  PACKET_LIST_ENTRY (MsgRemovePlayer),
  PACKET_LIST_ENTRY (MsgReplayReset),
  PACKET_LIST_ENTRY (MsgShotBegin),
  PACKET_LIST_ENTRY (MsgWShotBegin),
  PACKET_LIST_ENTRY (MsgWhatTimeIsIt),
  PACKET_LIST_ENTRY (MsgScore),
  PACKET_LIST_ENTRY (MsgScoreOver),
  PACKET_LIST_ENTRY (MsgShotEnd),
  PACKET_LIST_ENTRY (MsgSuperKill),
  PACKET_LIST_ENTRY (MsgSetShot),
  PACKET_LIST_ENTRY (MsgSetTeam),
  PACKET_LIST_ENTRY (MsgSetVar),
  PACKET_LIST_ENTRY (MsgTangibilityUpdate),
  PACKET_LIST_ENTRY (MsgTangibilityReset),
  PACKET_LIST_ENTRY (MsgTimeUpdate),
  PACKET_LIST_ENTRY (MsgTeleport),
  PACKET_LIST_ENTRY (MsgTransferFlag),
  PACKET_LIST_ENTRY (MsgTeamUpdate),
  PACKET_LIST_ENTRY (MsgWantWHash),
  PACKET_LIST_ENTRY (MsgWantSettings),
  PACKET_LIST_ENTRY (MsgUDPLinkRequest),
  PACKET_LIST_ENTRY (MsgUDPLinkEstablished),
  PACKET_LIST_ENTRY (MsgServerControl),
  PACKET_LIST_ENTRY (MsgLagPing),

  PACKET_LIST_ENTRY (MsgPingCodeReply),
  PACKET_LIST_ENTRY (MsgPingCodeRequest),
  PACKET_LIST_ENTRY (MsgEchoRequest),
  PACKET_LIST_ENTRY (MsgEchoResponse)
};
static const int PacketListCount = sizeof (PacketList) / sizeof (PacketList[0]);


//============================================================================//

void MsgStrings::init ()
{
  Flags::init(); // Initialize the flags - FIXME - check if necessary
  MsgStrings::reset();
  return;
}


void MsgStrings::reset ()
{
  unsigned int i;

  //  UseDNS = true;
  Colorize = true;
  TrackState = true;

  FlagList.clear();
  PlayerList.clear();

  // setup default player names
  PlayerList[ServerPlayer] = "SERVER";
  PlayerList[AllPlayers] = "ALL";
  PlayerList[AdminPlayers] = "ADMIN";
  PlayerList[NoPlayer] = "NOPLAYER";
  for (i=244 ; i<=250; i++) {
    PlayerList[i] = Team::getName (TeamColor(250 - i));
  }

  // set default DB entries
  for (i = 0; i < numGlobalDBItems; ++i) {
    assert(globalDBItems[i].name != NULL);
    if (globalDBItems[i].value != NULL) {
      BZDB.set(globalDBItems[i].name, globalDBItems[i].value);
      BZDB.setDefault(globalDBItems[i].name, globalDBItems[i].value);
    }
    BZDB.setPersistent(globalDBItems[i].name, globalDBItems[i].persistent);
    BZDB.setPermission(globalDBItems[i].name, globalDBItems[i].permission);
  }

  return;
}


void MsgStrings::useDNS (bool /*value*/)
{
  //  UseDNS = value;
  return;
}

void MsgStrings::colorize (bool value)
{
  Colorize = value;
  return;
}


void MsgStrings::trackState (bool value)
{
  TrackState = value;
  return;
}


int MsgStrings::knownPacketTypes ()
{
  return (PacketListCount - 1);  // MsgNull doesn't count
}


const char * MsgStrings::strMsgCode (uint16_t code)
{
  int i;
  for (i = 0; i < PacketListCount; i++) {
    if (PacketList[i].code == code) {
      break;
    }
  }
  if (i < PacketListCount) {
    return PacketList[i].label;
  } else {
    static char buf[32];
    sprintf (buf, "MsgUnknown(0x%04X)", code);
    return buf;
  }
}


MsgStringList MsgStrings::msgFromServer (u16 len, u16 code, const void *data)
{
  int i;
  for (i = 0; i < PacketListCount; i++) {
    if (PacketList[i].code == code) {
      break;
    }
  }
  if (i < PacketListCount) {
    PacketInfo pi = {len, code, data};
    return PacketList[i].handler (&pi);
  } else {
    MsgStringList badcode;
    listPush (badcode, 0, "Unknown message code: 0x%04X\n", code);
    return badcode;
  }
}


MsgStringList MsgStrings::msgFromClient (u16 len, u16 code, const void *data)
{
  int i;
  for (i = 0; i < PacketListCount; i++) {
    if (PacketList[i].code == code) {
      break;
    }
  }
  if (i < PacketListCount) {
    PacketInfo pi = {len, code, data};
    MsgStringList list = listMsgBasics (&pi);
    list[0].text += "  << client to server messages unimplemented >>";
    return list;
  } else {
    MsgStringList list;
    listPush (list, 0, "Unknown message code: 0x%04X\n", code);
    list[0].text += "  << client to server messages unimplemented >>";
    return list;
  }
}


//============================================================================//

static void listPush (MsgStringList &list, int level, const char *fmt, ...)
{
//  std::string str;
  MsgString mstr = { level, "", "" };
  char buffer[256];
  va_list args;

  if (!fmt)
    return;

  va_start(args, fmt);
  vsnprintf (buffer, 256, fmt, args);
  va_end(args);
  mstr.text += buffer;
  if (Colorize) {
    switch (level) {
      case 0:
	mstr.color = ANSI_STR_FG_MAGENTA;
	break;
      case 1:
	mstr.color = ANSI_STR_FG_BLUE;
	break;
      case 2:
	mstr.color = ANSI_STR_FG_CYAN;
	break;
      case 3:
	mstr.color = ANSI_STR_FG_GREEN;
	break;
    }
  }

  list.push_back (mstr);
  return;
}


static MsgStringList listMsgBasics (PacketInfo *pi)
{
  MsgStringList list;
  // also include the length and code u16's for the total length
  listPush (list, 0, "%s <%i>", MsgStrings::strMsgCode (pi->code), pi->len + 4);
  return list;
}


static std::string strFlag (u16 id)
{
  FlagListType::iterator it = FlagList.find (id);
  std::string name = "Invalid";
  if (it != FlagList.end()) {
    name = (*it).second;
  }
  return TextUtils::format ("%-2s [%i]", name.c_str(), id);
}


static std::string strFlagStatus (FlagStatus status)
{
  std::string str;
#define STRING_CASE(x)  case (x): str = #x; break
  switch (status) {
    STRING_CASE (FlagNoExist);
    STRING_CASE (FlagOnGround);
    STRING_CASE (FlagOnTank);
    STRING_CASE (FlagInAir);
    STRING_CASE (FlagComing);
    STRING_CASE (FlagGoing);
    default:
      str = TextUtils::format ("UNKNOWN: 0x04%X", status);
      break;
  }
  return str;
}

static std::string strTeam (u16 id)
{
  return std::string (Team::getName((TeamColor) id));
}


static std::string strPlayer (u16 id)
{
  PlayerListType::iterator it = PlayerList.find (id);
  std::string name;
  if (it != PlayerList.end()) {
    name = (*it).second;
  } else {
    name = "UnTracked";
  }
  return TextUtils::format ("%s [%i]", name.c_str(), id);
}


static std::string strVector (const fvec3& vector)
{
  std::string str = TextUtils::format ("(%8.3f, %8.3f, %8.3f)",
    vector[0], vector[1], vector[2]);
  return str;
}


static std::string strAddress (Address& address)
{
  return address.getDotNotation();
}


static std::string strKillReason (int16_t reason)
{
  switch (reason) {
    case 0:
      return std::string ("blowed up");
    case 1:
      return std::string ("shot");
    case 2:
      return std::string ("runover");
    case 3:
      return std::string ("captured");
    case 4:
      return std::string ("genocide");
    case 5:
      return std::string ("self destruct");
    default:
      return TextUtils::format ("unknown: %i", reason);
  }
}

//============================================================================//

static MsgStringList handleMsgNull (PacketInfo *pi)
{
  // not recorded, never seen?
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgAccept (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgAdminInfo (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 count, ipsize, player;
  Address address;
  u16 i;
  d = nboUnpackUInt8 (d, count);
  listPush (list, 1, "count: %i", count);
  for (i=0 ; i < count; i++) {
    d = nboUnpackUInt8 (d, ipsize);
    d = nboUnpackUInt8 (d, player);
    d = address.unpack (d);

    listPush (list, 2, "player:     %s", strPlayer(player).c_str());
    listPush (list, 2, "ipsize:     %i", ipsize);
    listPush (list, 2, "address:    %s", strAddress(address).c_str());
  }

  return list;
}


static MsgStringList handleMsgAlive (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player;
  fvec3 pos;
  float azimuth;
  d = nboUnpackUInt8(d, player);
  d = nboUnpackFVec3(d, pos);
  d = nboUnpackFloat(d, azimuth);
  listPush(list, 1, "player: %s", strPlayer(player).c_str());
  listPush(list, 2, "pos:    %s", strVector(pos).c_str());
  listPush(list, 2, "angle:  %-8.3f = %8.3f deg",
	   azimuth, azimuth * (180.0f / M_PI));

  return list;
}


static MsgStringList handleMsgAllow (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgAddPlayer (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 index;
  u16 type, team, wins, losses, tks;
  char callsign[CallSignLen];

  d = nboUnpackUInt8(d, index);
  d = nboUnpackUInt16(d, type);
  d = nboUnpackUInt16(d, team);
  d = nboUnpackUInt16(d, wins);
  d = nboUnpackUInt16(d, losses);
  d = nboUnpackUInt16(d, tks);
  d = nboUnpackString(d, callsign, CallSignLen);

  if (TrackState) {
    PlayerList[index] = callsign;
  }
  listPush (list, 1, "player: %s", strPlayer(index).c_str());
  listPush (list, 1, "team:   %s", strTeam(team).c_str());
  listPush (list, 1, "type:   %i", type);
  listPush (list, 2, "wins:   %i", wins);
  listPush (list, 2, "losses: %i", losses);
  listPush (list, 2, "tks:    %i", tks);

  return list;
}


static MsgStringList handleMsgAllowSpawn (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgAutoPilot (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgCapBits (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgCaptureFlag (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u16 team;
  d = nboUnpackUInt16 (d, team);
  listPush (list, 1, "team: %s", strTeam (team).c_str());

  return list;
}


static MsgStringList handleMsgCollide (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgCustomSound (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgCacheURL (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgDropFlag (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  Flag flag;
  u8 player;
  u16 flagid;
  d = nboUnpackUInt8 (d, player);
  d = nboUnpackUInt16 (d, flagid);
  d = flag.unpack (d);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "flag: %s", strFlag (flagid).c_str());

  return list;
}


static MsgStringList handleMsgEnter (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgExit (PacketInfo *pi)
{
  // not recorded, but bzfs will send a MsgRemovePlayer
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgFlagType (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgFlagUpdate (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u16 count, index;
  int i;
  d = nboUnpackUInt16 (d, count);
  listPush (list, 1, "count: %i", count);
  for (i = 0; i < (int) count; i++) {
    Flag flag;
    d = nboUnpackUInt16 (d, index);
    d = flag.unpack (d);
    if (TrackState) {
      FlagList[index] = flag.type->flagAbbv;
    }
    listPush (list, 2, "flag: %s", strFlag (index).c_str());
    listPush (list, 3, "owner:  %s", strPlayer (flag.owner).c_str());
    listPush (list, 3, "pos:    %s", strVector (flag.position).c_str());
    listPush (list, 3, "status: %s", strFlagStatus (flag.status).c_str());
  }

  return list;
}


static MsgStringList handleMsgFetchResources (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgGrabFlag (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  Flag flag;
  u8 player;
  u16 flagid;
  d = nboUnpackUInt8 (d, player);
  d = nboUnpackUInt16 (d, flagid);
  d = flag.unpack (d);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "flag: %s", strFlag (flagid).c_str());

  return list;
}


static MsgStringList handleMsgGMUpdate (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 target;
  ShotUpdate shot;
  d = shot.unpack (d);
  d = nboUnpackUInt8 (d, target);
  listPush (list, 1, "player:   %s", strPlayer(shot.player).c_str());
  listPush (list, 1, "target:   %s", strPlayer(target).c_str());
  listPush (list, 2, "id:       %i", shot.id);
  listPush (list, 2, "team:     %s", strTeam(shot.team).c_str());
  listPush (list, 2, "pos:      %s", strVector(shot.pos).c_str());
  listPush (list, 2, "vel:      %s", strVector(shot.vel).c_str());

  return list;
}


static MsgStringList handleMsgGetWorld (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgGameSettings (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgGameTime (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgHandicap (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgHit (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgKilled (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 victim, killer;
  int16_t reason, shot;
  FlagType* flagType;
  int32_t phydrv;
  d = nboUnpackUInt8(d, victim);
  d = nboUnpackUInt8(d, killer);
  d = nboUnpackInt16(d, reason);
  d = nboUnpackInt16(d, shot);
  d = FlagType::unpack(d, flagType);
  if (reason == PhysicsDriverDeath) {
    d = nboUnpackInt32(d, phydrv);
  }
  listPush (list, 1, "victim: %s", strPlayer(victim).c_str());
  listPush (list, 1, "killer: %s", strPlayer(killer).c_str());
  listPush (list, 1, "reason: %s", strKillReason(reason).c_str());
  listPush (list, 1, "shotid: %i", shot);

  if (flagType != Flags::Null) {
    listPush (list, 1, "flag:   %s", flagType->flagAbbv.c_str());
  } else {
    listPush (list, 1, "flag:   Null");
  }
  if (reason == PhysicsDriverDeath) {
    listPush (list, 1, "phydrv: %i", phydrv);
  }

  return list;
}


static MsgStringList handleMsgJoinServer (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;

  std::string addr;
  int32_t port;
  int32_t team;
  std::string referrer;
  std::string message;

  d = nboUnpackStdString(d, addr);
  d = nboUnpackInt32(d, port);
  d = nboUnpackInt32(d, team);
  d = nboUnpackStdString(d, referrer);
  d = nboUnpackStdString(d, message);

  listPush (list, 1, "addr: \"%s\"", addr.c_str());
  listPush (list, 1, "port: %i", port);
  listPush (list, 1, "team: %i", team);
  listPush (list, 1, "referrer: \"%s\"", referrer.c_str());
  listPush (list, 1, "message: \"%s\"", message.c_str());

  return list;
}


static MsgStringList handleMsgLagState (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgLimboMessage (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgMessage (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 src, dst;
  d = nboUnpackUInt8 (d, src);
  d = nboUnpackUInt8 (d, dst);
  listPush (list, 1, "src: %s", strPlayer(src).c_str());
  listPush (list, 1, "dst: %s", strPlayer(dst).c_str());
  listPush (list, 1, "message: \"%s\"", (char*) d);

  return list;
}


static MsgStringList handleMsgNewPlayer (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgNearFlag (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgNewRabbit (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player, paused;
  d = nboUnpackUInt8 (d, player);
  d = nboUnpackUInt8 (d, paused);

  return list;
}


static MsgStringList handleMsgNegotiateFlags (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgPause (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player, paused;
  d = nboUnpackUInt8 (d, player);
  d = nboUnpackUInt8 (d, paused);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "paused: %i", paused);

  return list;
}


static MsgStringList handleMsgPlayerInfo (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 count, player, properties;
  Address address;
  u16 i;
  d = nboUnpackUInt8 (d, count);
  listPush (list, 1, "count: %i", count);
  for (i=0 ; i < count; i++) {
    d = nboUnpackUInt8 (d, player);
    d = nboUnpackUInt8 (d, properties);

    std::string props;
    if (properties & IsRegistered) {
      props += "Registered ";
    }
    if (properties & IsVerified) {
      props += "Verified ";
    }
    if (properties & IsAdmin) {
      props += "Admin ";
    }

    listPush (list, 2, "player:     %s", strPlayer(player).c_str());
    listPush (list, 2, "properties: %s(0x%02X)", props.c_str(), properties);
  }

  return list;
}


static MsgStringList handleMsgPlayerUpdate (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  float timestamp;
  u8 index;
  PlayerState state;
  d = nboUnpackFloat (d, timestamp);
  d = nboUnpackUInt8 (d, index);
  d = state.unpack (d, pi->code);

  listPush (list, 1, "player: %s", strPlayer(index).c_str());
  listPush (list, 2, "state: 0x%04X  order: %i", state.status, state.order);
  listPush (list, 3, "pos:    %s", strVector (state.pos).c_str());
  listPush (list, 3, "vel:    %s", strVector (state.velocity).c_str());
  listPush (list, 3, "angle:  %-8.3f = %8.3f deg",
	   state.azimuth, state.azimuth * (180.0f / M_PI));
  listPush (list, 3, "angvel: %-8.3f = %8.3f deg/sec",
	   state.angVel, state.angVel * (180.0f / M_PI));

  return list;
}


static MsgStringList handleMsgPlayerUpdateSmall (PacketInfo *pi)
{
  // call the normal function while maintaining the same 'code' value
  return handleMsgPlayerUpdate (pi);
}


static MsgStringList handleMsgQueryGame (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgQueryPlayers (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgReject (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgRemovePlayer (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 index;
  d = nboUnpackUInt8 (d, index);
  listPush (list, 1, "player: %s", strPlayer(index).c_str());
  if (TrackState) {
    PlayerList.erase (index);
  }

  return list;
}


static MsgStringList handleMsgReplayReset (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgShotBegin (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  FiringInfo finfo;
  d = finfo.unpack (d);
  const ShotUpdate& shot = finfo.shot;
  listPush (list, 1, "player:   %s", strPlayer(shot.player).c_str());
  listPush (list, 1, "type:     %.2s", finfo.flagType->flagAbbv.c_str()); // FIXME ?
  listPush (list, 2, "id:       %i", shot.id);
  listPush (list, 2, "team:     %s", strTeam(shot.team).c_str());
  listPush (list, 2, "pos:      %s", strVector(shot.pos).c_str());
  listPush (list, 2, "vel:      %s", strVector(shot.vel).c_str());
  listPush (list, 2, "lifetime: %-8.3f", finfo.lifetime);

  return list;
}


static MsgStringList handleMsgWShotBegin (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgWhatTimeIsIt (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgScore (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 count, player;
  u16 wins, losses, tks, i;
  d = nboUnpackUInt8 (d, count);
  listPush (list, 1, "count: %i", count);
  for (i=0; i < count; i++) {
    d = nboUnpackUInt8 (d, player);
    d = nboUnpackUInt16 (d, wins);
    d = nboUnpackUInt16 (d, losses);
    d = nboUnpackUInt16 (d, tks);
    listPush (list, 2, "player: %s", strPlayer(player).c_str());
    listPush (list, 3, "wins: %i  losses: %i  tks: %i", wins, losses, tks);
  }

  return list;
}


static MsgStringList handleMsgScoreOver (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player;
  u16 team;
  d = nboUnpackUInt8(d, player);
  d = nboUnpackUInt16(d, team);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "team:   %i", strTeam(team).c_str());

  return list;
}


static MsgStringList handleMsgShotEnd (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player;
  u16 shotid;
  int16_t reason;
  d = nboUnpackUInt8(d, player);
  d = nboUnpackUInt16(d, shotid);
  d = nboUnpackInt16(d, reason);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "shotid: %i", shotid);
  listPush (list, 1, "reason: %i", reason); // FIXME

  return list;
}


static MsgStringList handleMsgSuperKill (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgSetShot (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgSetTeam (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgSetVar (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u16 i;
  u16 count;
  d = nboUnpackUInt16(d, count);
  listPush (list, 1, "count: %i", count);

  std::string name;
  std::string value;
  for (i = 0; i < count; i++) {
    d = nboUnpackStdString(d, name);
    d = nboUnpackStdString(d, value);
    listPush (list, 2, "%-20s = \"%s\"", name.c_str(), value.c_str());

    if (TrackState) {
      BZDB.set (name, value, StateDatabase::Locked);
    }
  }

  return list;
}


static MsgStringList handleMsgTangibilityUpdate (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgTangibilityReset (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgTimeUpdate (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);
  void *d = (void*)pi->data;
  int32_t timeLeft;
  d = nboUnpackInt32(d, timeLeft);
  listPush (list, 1, "timeLeft: %i", timeLeft);

  return list;
}


static MsgStringList handleMsgTeleport (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 player;
  u16 to, from;
  d = nboUnpackUInt8(d, player);
  d = nboUnpackUInt16(d, from);
  d = nboUnpackUInt16(d, to);
  listPush (list, 1, "player: %s", strPlayer(player).c_str());
  listPush (list, 1, "from:   %i", from);
  listPush (list, 1, "to:     %i", to);

  return list;
}


static MsgStringList handleMsgTransferFlag (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 to, from;
  u16 flagid;
  Flag flag;
  d = nboUnpackUInt8(d, from);
  d = nboUnpackUInt8(d, to);
  d = nboUnpackUInt16(d, flagid);
  d = flag.unpack (d);
  listPush (list, 1, "from: %s", strPlayer(from).c_str());
  listPush (list, 1, "to:   %s", strPlayer(to).c_str());
  listPush (list, 1, "flag: %s", strFlag(flagid).c_str());

  return list;
}


static MsgStringList handleMsgTeamUpdate (PacketInfo *pi)
{
  MsgStringList list = listMsgBasics (pi);

  void *d = (void*)pi->data;
  u8 count;
  u16 i, team, size, won, lost;
  d = nboUnpackUInt8(d, count);
  listPush (list, 1, "count: %i", count);
  for (i=0; i<count; i++) {
    d = nboUnpackUInt16(d, team);
    d = nboUnpackUInt16(d, size);
    d = nboUnpackUInt16(d, won);
    d = nboUnpackUInt16(d, lost);
    listPush (list, 2, "team: %s", strTeam(team).c_str());
    listPush (list, 3, "size = %i, won = %i, lost = %i", size, won, lost);
  }

  return list;
}


static MsgStringList handleMsgWantWHash (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgWantSettings (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgUDPLinkRequest (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgUDPLinkEstablished (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgServerControl (PacketInfo *pi)
{
  // packet type hasn't not been implemented
  MsgStringList list = listMsgBasics (pi);
  return list;
}


static MsgStringList handleMsgLagPing (PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}

static MsgStringList handleMsgPingCodeReply(PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}

static MsgStringList handleMsgPingCodeRequest(PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}

static MsgStringList handleMsgEchoRequest(PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}

static MsgStringList handleMsgEchoResponse(PacketInfo *pi)
{
  // not recorded
  MsgStringList list = listMsgBasics (pi);
  return list;
}



//============================================================================//

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
