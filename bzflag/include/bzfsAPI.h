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

// all the exported functions for bzfs plugins

#ifndef _BZFS_API_H_
#define _BZFS_API_H_

/* system interface headers */
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>

/* DO NOT INCLUDE ANY OTHER HEADERS IN THIS FILE */
/* PLUGINS NEED TO BE BUILT WITHOUT THE BZ SOURCE TREE */
/* JUST THIS ONE FILE */

#ifdef _WIN32
#  ifdef INSIDE_BZ
#    define BZF_API __declspec(dllexport)
#  else
#    define BZF_API __declspec(dllimport)
#  endif
#  define BZF_PLUGIN_CALL
#  ifndef strcasecmp
#    if defined(_MSC_VER) && (_MSC_VER >= 1400)
#      define strcasecmp _stricmp
#    else
#      define strcasecmp stricmp
#    endif
#  endif
#else
#  define BZF_API
#  define BZF_PLUGIN_CALL extern "C"
#endif

#define BZ_API_VERSION 1

#define BZ_GET_PLUGIN_VERSION BZF_PLUGIN_CALL int bz_GetVersion(void) { return BZ_API_VERSION; }

/** This is so we can use gcc's "format string vs arguments"-check
 * for various printf-like functions, and still maintain compatability.
 * Not tested on other platforms yet, but should work. */
#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5)
#  define __attribute__(Spec) /* empty */
# endif
/* The __-protected variants of `format' and `printf' attributes
 *    are accepted by gcc versions 2.6.4 (effectively 2.7) and later.  */
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#  define __format__ format
#  define __printf__ printf
# endif
#endif

/** shorthand defines to make the code cleaner. */
#define _ATTRIBUTE34 __attribute__ ((__format__ (__printf__, 3, 4)))
#define _ATTRIBUTE23 __attribute__ ((__format__ (__printf__, 2, 3)))
#define _ATTRIBUTE12 __attribute__ ((__format__ (__printf__, 1, 2)))

#ifdef __cplusplus
#  ifndef DEFINED_FORCE_CAST
#    define DEFINED_FORCE_CAST
    template<class To, class From>
    inline To force_cast(From const & f)
    {
      union {
	From f;
	To t;
      } fc;
      fc.f = f;
      return fc.t;
    }
#  endif
#endif

class bz_BasePlayerRecord;
BZF_API bool bz_freePlayerRecord(bz_BasePlayerRecord *playerRecord);

// current time (leave method here, used in bz_EventData constructor)
BZF_API double bz_getCurrentTime(void);

// versioning
BZF_API int bz_APIVersion(void);

// event stuff

typedef enum {
  bz_eNullEvent = 0,
  bz_eCaptureEvent,
  bz_ePlayerDieEvent,
  bz_ePlayerSpawnEvent,
  bz_eZoneEntryEvent,
  bz_eZoneExitEvent,
  bz_ePlayerJoinEvent,
  bz_ePlayerPartEvent,
  bz_eRawChatMessageEvent,	// before filter
  bz_eFilteredChatMessageEvent,	// after filter
  bz_eUnknownSlashCommand,
  bz_eGetPlayerSpawnPosEvent,
  bz_eGetAutoTeamEvent,
  bz_eAllowPlayer,
  bz_eTickEvent,
  bz_eGetWorldEvent,
  bz_eGetPlayerInfoEvent,
  bz_eAllowSpawn,
  bz_eListServerUpdateEvent,
  bz_eBanEvent,
  bz_eHostBanNotifyEvent,
  bz_eHostBanModifyEvent,
  bz_eIdBanEvent,
  bz_eKickEvent,
  bz_eKillEvent,
  bz_ePlayerPausedEvent,
  bz_eMessageFilteredEvent,
  bz_eGameStartEvent,
  bz_eGameEndEvent,
  bz_eSlashCommandEvent,
  bz_ePlayerAuthEvent,
  bz_eServerMsgEvent,
  bz_eShotFiredEvent,
  bz_eAnointRabbitEvent,
  bz_eNewRabbitEvent,
  bz_eReloadEvent,
  bz_ePlayerUpdateEvent,
  bz_eNetDataSendEvent,
  bz_eNetDataReceiveEvent,
  bz_eLoggingEvent,
  bz_eFlagTransferredEvent,
  bz_eFlagGrabbedEvent,
  bz_eFlagDroppedEvent,
  bz_eShotEndedEvent,
  bz_eNewNonPlayerConnection,
  bz_eIdleNewNonPlayerConnection,
  bz_ePlayerCollision,
  bz_eFlagResetEvent,
  bz_eWorldFinalized,
  bz_eAllowCTFCaptureEvent,
  bz_eAllowFlagGrabEvent,
  bz_eAllowKillCommandEvent,
  bz_eReportFiledEvent,
  bz_eTeleportEvent,
  bz_ePlayerSentCustomData,
  bz_ePlayerCustomDataChanged,
  bz_eBZDBChange,
  bz_eAutoPilotChangeEvent,
  bz_eAllowAutoPilotChangeEvent,
  bz_eLastEvent    //this is never used as an event, just show it's the last one
} bz_eEventType;

// permision #defines
#define bz_perm_actionMessage  "actionMessage"
#define bz_perm_adminMessageReceive  "adminMessageReceive"
#define bz_perm_adminMessageSend  "adminMessageSend"
#define bz_perm_antiban   "antiban"
#define bz_perm_antikick   "antikick"
#define bz_perm_antikill   "antikill"
#define bz_perm_antipoll   "antipoll"
#define bz_perm_antipollban   "antipollban"
#define bz_perm_antipollkick   "antipollkick"
#define bz_perm_antipollkill   "antipollkill"
#define bz_perm_ban  "ban"
#define bz_perm_banlist  "banlist"
#define bz_perm_countdown  "countdown"
#define bz_perm_date  "date"
#define bz_perm_endGame  "endGame"
#define bz_perm_flagHistory  "flagHistory"
#define bz_perm_flagMod  "flagMod"
#define bz_perm_hideAdmin  "hideAdmin"
#define bz_perm_idleStats  "idleStats"
#define bz_perm_info  "info"
#define bz_perm_kick  "kick"
#define bz_perm_kill  "kill"
#define bz_perm_lagStats  "lagStats"
#define bz_perm_lagwarn  "lagwarn"
#define bz_perm_listPlugins  "listPlugins"
#define bz_perm_listPerms  "listPerms"
#define bz_perm_masterBan  "masterban"
#define bz_perm_mute  "mute"
#define bz_perm_playerList  "playerList"
#define bz_perm_poll  "poll"
#define bz_perm_pollBan  "pollBan"
#define bz_perm_pollKick  "pollKick"
#define bz_perm_pollKill  "pollKill"
#define bz_perm_pollSet  "pollSet"
#define bz_perm_pollFlagReset  "pollFlagReset"
#define bz_perm_privateMessage  "privateMessage"
#define bz_perm_record  "record"
#define bz_perm_rejoin  "rejoin"
#define bz_perm_removePerms  "removePerms"
#define bz_perm_replay  "replay"
#define bz_perm_say  "say"
#define bz_perm_sendHelp  "sendHelp"
#define bz_perm_setAll  "setAll"
#define bz_perm_setPerms  "setPerms"
#define bz_perm_setVar  "setVar"
#define bz_perm_showOthers  "showOthers"
#define bz_perm_shortBan  "shortBan"
#define bz_perm_shutdownServer  "shutdownServer"
#define bz_perm_spawn  "spawn"
#define bz_perm_superKill  "superKill"
#define bz_perm_talk  "talk"
#define bz_perm_unban  "unban"
#define bz_perm_unmute  "unmute"
#define bz_perm_veto  "veto"
#define bz_perm_viewReports  "viewReports"
#define bz_perm_vote  "vote"

typedef enum {
  eAutomaticTeam = -2,
  eNoTeam = -1,
  eRogueTeam = 0,
  eRedTeam,
  eGreenTeam,
  eBlueTeam,
  ePurpleTeam,
  eObservers,
  eRabbitTeam,
  eHunterTeam,
  eAdministrators
} bz_eTeamType;

#define BZ_SERVER		-2
#define BZ_ALLUSERS		-1
#define BZ_NULLUSER		-3


#define BZ_BZDBPERM_NA		0
#define BZ_BZDBPERM_USER	1
#define BZ_BZDBPERM_SERVER	2
#define BZ_BZDBPERM_CLIENT	3


typedef enum {
  eTeamFFAGame = 0,
  eClassicCTFGame,
  eRabbitGame,
  eOpenFFAGame
} bz_eGameType;

typedef enum {
  eNoShot = 0,
  eStandardShot,
  eGMShot,
  eLaserShot,
  eThiefShot,
  eSuperShot,
  ePhantomShot,
  eShockWaveShot,
  eRicoShot,
  eMachineGunShot,
  eInvisibleShot,
  eCloakedShot,
  eRapidFireShot,
  eLastShotType
} bz_eShotType;

typedef enum {
  eGoodFlag = 0,
  eBadFlag,
  eLastFlagQuality
} bz_eFlagQuality;

//utility classes
class BZF_API bz_ApiString
{
 public:
  bz_ApiString();
  bz_ApiString(const char* c);
  bz_ApiString(const std::string &s);
  bz_ApiString(const bz_ApiString &r);

  ~bz_ApiString();

  bz_ApiString& operator = (const bz_ApiString& r);
  bz_ApiString& operator = (const std::string& r);
  bz_ApiString& operator = (const char* r);

  bool operator == (const bz_ApiString&r);
  bool operator == (const std::string& r);
  bool operator == (const char* r);

  bool operator != (const bz_ApiString&r);
  bool operator != (const std::string& r);
  bool operator != (const char* r);

  unsigned int size(void) const;

  const char* c_str(void) const;

  void format(const char* fmt, ...)_ATTRIBUTE23;

  void replaceAll(const char* target, const char* with);

  void tolower(void);
  void toupper(void);

  void urlEncode(void);

 protected:
  class dataBlob;

  dataBlob *data;
};

class BZF_API bz_APIIntList
{
 public:
  bz_APIIntList();
  bz_APIIntList(const bz_APIIntList &r);
  bz_APIIntList(const std::vector<int> &r);

  ~bz_APIIntList();

  void push_back(int value);
  int get(unsigned int i);

  const int& operator[] (unsigned int i) const;
  bz_APIIntList& operator = (const bz_APIIntList& r);
  bz_APIIntList& operator = (const std::vector<int>& r);

  unsigned int size(void);
  void clear(void);

 protected:
  class dataBlob;

  dataBlob *data;
};

BZF_API bz_APIIntList* bz_newIntList(void);
BZF_API void bz_deleteIntList(bz_APIIntList *l);

class BZF_API bz_APIFloatList
{
 public:
  bz_APIFloatList();
  bz_APIFloatList(const bz_APIFloatList &r);
  bz_APIFloatList(const std::vector<float> &r);

  ~bz_APIFloatList();

  void push_back(float value);
  float get(unsigned int i);

  const float& operator[] (unsigned int i) const;
  bz_APIFloatList& operator = (const bz_APIFloatList& r);
  bz_APIFloatList& operator = (const std::vector<float>& r);

  unsigned int size(void);
  void clear(void);

 protected:
  class dataBlob;

  dataBlob *data;
};

BZF_API bz_APIFloatList* bz_newFloatList(void);
BZF_API void bz_deleteFloatList(bz_APIFloatList *l);

class BZF_API bz_APIStringList
{
 public:
  bz_APIStringList();
  bz_APIStringList(const bz_APIStringList &r);
  bz_APIStringList(const std::vector<std::string> &r);

  ~bz_APIStringList();

  void push_back(const char* value);
  void push_back(const std::string &value);
  const bz_ApiString& get(unsigned int i);
  bool contains(const char* value);
  bool contains(const std::string &value);

  const bz_ApiString& operator[] (unsigned int i) const;
  bz_APIStringList& operator = (const bz_APIStringList& r);
  bz_APIStringList& operator = (const std::vector<std::string>& r);

  unsigned int size(void);
  void clear(void);

  void tokenize(const char* in, const char* delims, int maxTokens = 0, bool useQuotes = false);
 protected:
  class dataBlob;

  dataBlob *data;
};

BZF_API bz_APIStringList* bz_newStringList(void);
BZF_API void bz_deleteStringList(bz_APIStringList *l);


// update structs use by a number of others

typedef enum {
  eDead,		// not alive, not paused, etc.
  eAlive,		// player is alive
  ePaused,		// player is paused
  eExploding,		// currently blowing up
  eTeleporting,		// teleported recently
  eInBuilding		// has OO and is in a building
} bz_ePlayerStatus;

typedef struct bz_PlayerUpdateState {
  bz_ePlayerStatus	status;			// special states
  bool			falling;		// not driving on the ground or an obstacle
  bool			crossingWall;		// crossing an obstacle wall
  bool			inPhantomZone;		// zoned
  float			pos[3];			// position of tank
  float			velocity[3];		// velocity of tank
  float			rotation;		// orientation of tank
  float			angVel;			// angular velocity of tank
  int			phydrv;			// physics driver
} bz_PlayerUpdateState;


// event data types
class BZF_API bz_EventData
{
 public:
  bz_EventData(bz_eEventType type =  bz_eNullEvent)
    : version(1), eventType(type), eventTime( bz_getCurrentTime() )
    {
    }
  virtual ~bz_EventData() {};
  virtual void update() {};

  int version;
  bz_eEventType eventType;
  double eventTime;
};

class BZF_API bz_CTFCaptureEventData_V1 : public bz_EventData
{
 public:
  bz_CTFCaptureEventData_V1() : bz_EventData(bz_eCaptureEvent)
    , teamCapped(eNoTeam), teamCapping(eNoTeam), playerCapping(-1)
    , rot(0.0)
    {
    }

  bz_eTeamType teamCapped;
  bz_eTeamType teamCapping;
  int playerCapping;

  float pos[3];
  float rot;
};

class BZF_API bz_AllowCTFCaptureEventData_V1 : public bz_EventData
{
 public:
  bz_AllowCTFCaptureEventData_V1() : bz_EventData(bz_eAllowCTFCaptureEvent)
    , teamCapped(eNoTeam), teamCapping(eNoTeam), playerCapping(-1)
    , rot(0.0)
    , allow(false), killTeam(true)
    {
    }

  bz_eTeamType teamCapped;
  bz_eTeamType teamCapping;
  int playerCapping;

  float pos[3];
  float rot;

  bool allow;
  bool killTeam;
};
class BZF_API bz_PlayerDieEventData_V1 : public bz_EventData
{
 public:
  bz_PlayerDieEventData_V1() : bz_EventData(bz_ePlayerDieEvent)
    , playerID(-1), team(eNoTeam), killerID(-1), killerTeam(eNoTeam)
    , shotID(-1) //!!
    {
    }

  int playerID;
  bz_eTeamType team;
  int killerID;
  bz_eTeamType killerTeam;
  bz_ApiString flagKilledWith;
  int shotID;

  bz_PlayerUpdateState state;
};

class BZF_API bz_PlayerSpawnEventData_V1 : public bz_EventData
{
 public:
  bz_PlayerSpawnEventData_V1() : bz_EventData(bz_ePlayerSpawnEvent)
    , playerID(-1), team(eNoTeam)
    {
    }

  int playerID;
  bz_eTeamType team;
  bz_PlayerUpdateState state;
};

class BZF_API bz_ChatEventData_V1 : public bz_EventData
{
 public:
  bz_ChatEventData_V1() : bz_EventData(bz_eRawChatMessageEvent)
    , from(-1), to(-1), team(eNoTeam)
    {
    }

  int from;
  int to;
  bz_eTeamType team;
  bz_ApiString message;
};

class BZF_API bz_PlayerJoinPartEventData_V1 : public bz_EventData
{
 public:
  bz_PlayerJoinPartEventData_V1() : bz_EventData(bz_ePlayerJoinEvent)
    , playerID(-1), record(0)
    {
    }
  ~bz_PlayerJoinPartEventData_V1()
    {
      bz_freePlayerRecord(record);
    }

  int playerID;
  bz_BasePlayerRecord* record;
  bz_ApiString reason;
};

class BZF_API bz_UnknownSlashCommandEventData_V1 : public bz_EventData
{
 public:
  bz_UnknownSlashCommandEventData_V1() : bz_EventData(bz_eUnknownSlashCommand)
    , from(-1), handled(false)
    {
    }

  int from;

  bool handled;
  bz_ApiString message;
};

class BZF_API bz_GetPlayerSpawnPosEventData_V1 : public bz_EventData
{
 public:
  bz_GetPlayerSpawnPosEventData_V1() : bz_EventData(bz_eGetPlayerSpawnPosEvent)
    , playerID(-1), team(eNoTeam), handled(false)
    , rot(0.0)
    {
      pos[0] = pos[1] = pos[2] = 0.0f;
    }

  int playerID;
  bz_eTeamType team;

  bool handled;

  float pos[3];
  float rot;
};

class BZF_API bz_AllowPlayerEventData_V1 : public bz_EventData
{
 public:
  bz_AllowPlayerEventData_V1() : bz_EventData(bz_eAllowPlayer)
    , playerID(-1)
    , allow(true)
    {
    }

  int playerID;
  bz_ApiString callsign;
  bz_ApiString ipAddress;

  bz_ApiString reason;

  bool allow;
};

class BZF_API bz_TickEventData_V1 : public bz_EventData
{
 public:
  bz_TickEventData_V1() : bz_EventData(bz_eTickEvent)
    {
    }
};

class BZF_API bz_GetWorldEventData_V1 : public bz_EventData
{
 public:
  bz_GetWorldEventData_V1() : bz_EventData(bz_eGetWorldEvent)
    , generated(false)
    , ctf(false)
    , rabbit(false)
    , openFFA(false)
    , worldBlob(NULL)
    {
    }

  bool generated;
  bool ctf;
  bool rabbit;
  bool openFFA;

  bz_ApiString worldFile;
  char* worldBlob; // if assigned, the world will be read from this NUL
                   // terminated string. BZFS does not free this memory,
                   // so the plugin must do so (this can be done in the
                   // WorldFinalize event)
};

class BZF_API bz_GetPlayerInfoEventData_V1 : public bz_EventData
{
 public:
  bz_GetPlayerInfoEventData_V1() : bz_EventData(bz_eGetPlayerInfoEvent)
    , playerID(-1), team(eNoTeam)
    , admin(false), verified(false), registered(false)
    {
    }

  int playerID;
  bz_ApiString callsign;
  bz_ApiString ipAddress;

  bz_eTeamType team;

  bool admin;
  bool verified;
  bool registered;
};

class BZF_API bz_GetAutoTeamEventData_V1 : public bz_EventData
{
 public:
  bz_GetAutoTeamEventData_V1() : bz_EventData(bz_eGetAutoTeamEvent)
    , playerID(-1), team(eNoTeam)
    , handled(false)
    {
    }

  int playerID;
  bz_ApiString callsign;
  bz_eTeamType team;

  bool handled;
};

class BZF_API bz_AllowSpawnData_V1 : public bz_EventData
{
 public:
  bz_AllowSpawnData_V1() : bz_EventData(bz_eAllowSpawn)
    , playerID(-1), team(eNoTeam)
    , handled(false), allow(true)
    {
    }

  int playerID;
  bz_eTeamType team;

  bool handled;
  bool allow;
};

class BZF_API bz_ListServerUpdateEvent_V1 : public bz_EventData
{
 public:
  bz_ListServerUpdateEvent_V1() : bz_EventData(bz_eListServerUpdateEvent)
    , handled(false)
    {
    }

  bz_ApiString address;
  bz_ApiString description;
  bz_ApiString groups;

  bool handled;
};

class BZF_API bz_BanEventData_V1 : public bz_EventData
{
 public:
  bz_BanEventData_V1() : bz_EventData(bz_eBanEvent)
    , bannerID(-1), banneeID(-1), duration(-1)
    {
    }

  int bannerID;
  int banneeID;
  int duration;
  bz_ApiString ipAddress;
  bz_ApiString reason;
};

class BZF_API bz_HostBanEventData_V1 : public bz_EventData
{
 public:
  bz_HostBanEventData_V1() : bz_EventData(bz_eHostBanModifyEvent)
    , bannerID(-1), duration(-1)
    {
    }

  int bannerID;
  int duration;
  bz_ApiString hostPattern;
  bz_ApiString reason;
};

class BZF_API bz_IdBanEventData_V1 : public bz_EventData
{
 public:
  bz_IdBanEventData_V1() : bz_EventData(bz_eIdBanEvent)
    , bannerID(-1), banneeID(-1), duration(-1)
    {
    }

  int bannerID;
  int banneeID;
  int duration;
  bz_ApiString bzId;
  bz_ApiString reason;
};

class BZF_API bz_KickEventData_V1 : public bz_EventData
{
 public:
  bz_KickEventData_V1() : bz_EventData(bz_eKickEvent)
    , kickerID(-1), kickedID(-1)
    {
    }

  int kickerID;
  int kickedID;
  bz_ApiString reason;
};

class BZF_API bz_KillEventData_V1 : public bz_EventData
{
 public:
  bz_KillEventData_V1() : bz_EventData(bz_eKillEvent)
    , killerID(-1), killedID(-1)
    {
    }

  int killerID;
  int killedID;
  bz_ApiString reason;
};

class BZF_API bz_PlayerPausedEventData_V1 : public bz_EventData
{
 public:
  bz_PlayerPausedEventData_V1() : bz_EventData(bz_ePlayerPausedEvent)
    , playerID(-1), pause(false)
    {
    }

  int playerID;
  bool pause;
};

class BZF_API bz_MessageFilteredEventData_V1 : public bz_EventData
{
 public:
  bz_MessageFilteredEventData_V1() : bz_EventData(bz_eMessageFilteredEvent)
    , playerID(-1)
    {
    }

  int playerID;

  bz_ApiString rawMessage;
  bz_ApiString filteredMessage;
};

class BZF_API bz_GameStartEndEventData_V1 : public bz_EventData
{
 public:
  bz_GameStartEndEventData_V1() : bz_EventData(bz_eGameStartEvent)
    , duration(0.0)
    {
    }

  double duration;
};

class BZF_API bz_SlashCommandEventData_V1 : public bz_EventData
{
 public:
  bz_SlashCommandEventData_V1() : bz_EventData(bz_eSlashCommandEvent)
    , from(-1)
    {
    }

  int from;

  bz_ApiString message;
};


class BZF_API bz_PlayerAuthEventData_V1 : public bz_EventData
{
 public:
  bz_PlayerAuthEventData_V1() : bz_EventData(bz_ePlayerAuthEvent)
    , playerID(-1), password(false), globalAuth(false)
    {
    }

  int playerID;
  bool password;
  bool globalAuth;
};

class BZF_API bz_ServerMsgEventData_V1 : public bz_EventData
{
 public:
  bz_ServerMsgEventData_V1() : bz_EventData(bz_eServerMsgEvent)
    , to(-1), team(eNoTeam)
    {
    }

  int to;
  bz_eTeamType team;
  bz_ApiString message;
};

class BZF_API bz_ShotFiredEventData_V1 : public bz_EventData
{
public:
  bz_ShotFiredEventData_V1() : bz_EventData(bz_eShotFiredEvent)
    , changed(false)
    , playerID(-1)
  {
    pos[0] = pos[1] = pos[2] = 0;
  }

  bool changed;
  float pos[3];
  bz_ApiString type;
  int playerID;
};

class BZF_API bz_AnointRabbitEventData_V1 : public bz_EventData
{
public:
  bz_AnointRabbitEventData_V1() : bz_EventData(bz_eAnointRabbitEvent)
    , newRabbit(-1), swap(true)
  {
  }

  int newRabbit;
  bool swap;
};

class BZF_API bz_NewRabbitEventData_V1 : public bz_EventData
{
public:
  bz_NewRabbitEventData_V1() : bz_EventData(bz_eNewRabbitEvent)
    , newRabbit(-1)
  {
  }

  int newRabbit;
};

class BZF_API bz_ReloadEventData_V1 : public bz_EventData
{
public:
  bz_ReloadEventData_V1() : bz_EventData(bz_eReloadEvent)
    , playerID(-1)
  {
  }

  int playerID;
};

class BZF_API bz_PlayerUpdateEventData_V1 : public bz_EventData
{
public:
  bz_PlayerUpdateEventData_V1() : bz_EventData(bz_ePlayerUpdateEvent)
    , playerID(-1), stateTime(0.0)
  {
  }

  int playerID;
  bz_PlayerUpdateState state;
  double stateTime;
};

class BZF_API bz_NetTransferEventData_V1 : public bz_EventData
{
public:
  bz_NetTransferEventData_V1() : bz_EventData(bz_eNetDataReceiveEvent)
    , send(false), udp(false), iSize(0), playerID(-1)
    , data(NULL)
  {
  }

  bool send;
  bool udp;
  unsigned int iSize;
  int playerID;

  // DON'T CHANGE THIS!!!
  unsigned char* data;
};

class BZF_API bz_LoggingEventData_V1 : public bz_EventData
{
public:
  bz_LoggingEventData_V1() : bz_EventData(bz_eLoggingEvent)
    , level(0)
  {
  }

  int level;
  bz_ApiString message;
};

class BZF_API bz_FlagTransferredEventData_V1 : public bz_EventData
{
public:
  enum Action {
    ContinueSteal = 0,
    CancelSteal = 1,
    DropThief = 2
  };

  bz_FlagTransferredEventData_V1() : bz_EventData(bz_eFlagTransferredEvent)
    , fromPlayerID(0), toPlayerID(0), flagType(NULL), action(ContinueSteal)
  {
  }

  int fromPlayerID;
  int toPlayerID;
  const char* flagType;
  enum Action action;
};


class BZF_API bz_FlagGrabbedEventData_V1 : public bz_EventData
{
public:

  bz_FlagGrabbedEventData_V1() : bz_EventData(bz_eFlagGrabbedEvent)
    , playerID(-1), flagID(-1)
    , shotType(eNoShot), flagType(NULL)
  {
    pos[0] = pos[1] = pos[2] = 0;
  }

  int playerID;
  int flagID;

  bz_eShotType shotType;

  const char* flagType;
  float pos[3];
};

class BZF_API bz_AllowFlagGrabEventData_V1 : public bz_EventData
{
public:

  bz_AllowFlagGrabEventData_V1() : bz_EventData(bz_eAllowFlagGrabEvent)
    , playerID(-1), flagID(-1), allow(false)
    , shotType(eNoShot), flagType(NULL)
  {
    pos[0] = pos[1] = pos[2] = 0;
  }

  int playerID;
  int flagID;
  bool allow;

  bz_eShotType shotType;

  const char* flagType;
  float pos[3];
};
class BZF_API bz_FlagDroppedEventData_V1 : public bz_EventData
{
public:

  bz_FlagDroppedEventData_V1() : bz_EventData(bz_eFlagDroppedEvent)
    , playerID(-1), flagID(-1), flagType(NULL)
  {
    pos[0] = pos[1] = pos[2] = 0;
  }

  int playerID;
  int flagID;

  const char* flagType;
  float pos[3];
};


class BZF_API bz_ShotEndedEventData_V1 : public bz_EventData
{
public:
  bz_ShotEndedEventData_V1() : bz_EventData(bz_eShotEndedEvent)
    , playerID(-1), shotID(-1), explode(false)
  {
  }

  int playerID;
  int shotID;
  bool explode;
};

class BZF_API bz_NewNonPlayerConnectionEventData_V1 : public bz_EventData
{
public:

  bz_NewNonPlayerConnectionEventData_V1() : bz_EventData(bz_eNewNonPlayerConnection)
    , connectionID(-1)
    , data(0), size(0)
  {
  }

  int connectionID;
  void* data;
  unsigned int size;
};

class BZF_API bz_PlayerCollisionEventData_V1 : public bz_EventData
{
public:
  bz_PlayerCollisionEventData_V1() : bz_EventData(bz_ePlayerCollision)
    , handled(false)
  {
    players[0] = players[1] = -1;
    pos[0] = pos[1] = pos[2] = 0;
  }

  int players[2];
  float pos[3];
  bool handled;
};

class BZF_API bz_FlagResetEventData_V1 : public bz_EventData
{
public:
  bz_FlagResetEventData_V1() : bz_EventData(bz_eFlagResetEvent)
    , flagID(-1), teamIsEmpty(false)
    , flagType(0)
    , changed(false)
  {
    pos[0] = pos[1] = pos[2] = 0;
  }

  int flagID;
  bool teamIsEmpty;
  const char* flagType;
  bool changed;
  float pos[3];
};

class BZF_API bz_AllowKillCommandEventData_V1 : public bz_EventData
{
public:
  bz_AllowKillCommandEventData_V1() : bz_EventData(bz_eAllowKillCommandEvent)
    , playerToKill(-1), playerKilling(-1)
    , allow(false)
  {
  }

  int playerToKill;
  int playerKilling;
  bool allow;
};

class BZF_API bz_ReportFiledEventData_V1 : public bz_EventData
{
public:
  bz_ReportFiledEventData_V1() : bz_EventData(bz_eReportFiledEvent){};

  bz_ApiString from;
  bz_ApiString message;
};

class BZF_API bz_TeleportEventData_V1 : public bz_EventData
{
public:
  bz_TeleportEventData_V1() : bz_EventData(bz_eTeleportEvent)
    , playerID(-1)
  {
  }

  int playerID;
  int from;
  int to;
};

class BZF_API bz_PlayerSentCustomData_V1 : public bz_EventData
{
public:
  bz_PlayerSentCustomData_V1() : bz_EventData(bz_ePlayerSentCustomData)
    , playerID(-1)
  {
  }

  int playerID;
  bz_ApiString key;
  bz_ApiString data;
};

class BZF_API bz_BZDBChangeData_V1 : public bz_EventData
{
public:
  bz_BZDBChangeData_V1(const std::string& _key, const std::string& _value)
  : bz_EventData(bz_eBZDBChange), key(_key), value(_value) {}

  bz_ApiString key;
  bz_ApiString value;
};

class BZF_API bz_AutoPilotChangeData_V1 : public bz_EventData
{
public:
	bz_AutoPilotChangeData_V1(bool _autopilot, bool _allow , int pID)
		: bz_EventData(bz_eAutoPilotChangeEvent),allow(_allow), autopilot(_autopilot), playerID(pID) {}

	bool allow;
	bool autopilot;
	int playerID;
};

class bz_EventHandler;

BZF_API bool bz_registerEvent(bz_eEventType eventType, bz_EventHandler* eventHandler);
BZF_API bool bz_removeEvent(bz_eEventType eventType, bz_EventHandler* eventHandler);
BZF_API bool bz_flushEvents(bz_EventHandler* eventHandler);

// event handler callback
class bz_EventHandler
{
public:
  virtual ~bz_EventHandler() { bz_flushEvents(this); }
  virtual void process(bz_EventData *eventData) = 0;
};

/**
 * RAII class to handle registration/deregistration
 *
 * Add a member of this class your event handler for each event handled
 */
class bz_EventHelper
{
public:
  bz_EventHelper(bz_eEventType eventType_, bz_EventHandler* eventHandler_)
    : eventType(eventType_), eventHandler(eventHandler_)
  { bz_registerEvent(eventType, eventHandler); }
  ~bz_EventHelper() { bz_removeEvent(eventType, eventHandler); }
private:
  bz_eEventType eventType;
  bz_EventHandler* eventHandler;
};

// non player data handlers

class bz_NonPlayerConnectionHandler
{
public:
  virtual ~bz_NonPlayerConnectionHandler() {};
  virtual void pending(int connectionID, void *data, unsigned int size) = 0;
  virtual void disconnect(int connectionID) { if (connectionID) return; };
};

BZF_API bool bz_registerNonPlayerConnectionHandler(int connectionID, bz_NonPlayerConnectionHandler* handler);
BZF_API bool bz_removeNonPlayerConnectionHandler(int connectionID, bz_NonPlayerConnectionHandler* handler);
BZF_API bool bz_sendNonPlayerData(int connectionID, const void *data, unsigned int size);
BZF_API bool bz_disconnectNonPlayerConnection(int connectionID);
BZF_API unsigned int bz_getNonPlayerConnectionOutboundPacketCount(int connectionID);

BZF_API const char* bz_getNonPlayerConnectionIP(int connectionID);
BZF_API const char* bz_getNonPlayerConnectionHost(int connectionID);

// player info

BZF_API bool bz_getPlayerIndexList(bz_APIIntList *playerList);
BZF_API bz_APIIntList *bz_getPlayerIndexList(void);
BZF_API bz_BasePlayerRecord *bz_getPlayerByIndex(int index);
BZF_API bz_BasePlayerRecord *bz_getPlayerByCallsign(const char* name);
BZF_API bz_BasePlayerRecord *bz_getPlayerByBZID(int BZID);
BZF_API bool bz_updatePlayerData(bz_BasePlayerRecord *playerRecord);
BZF_API bool bz_hasPerm(int playerID, const char* perm);
BZF_API bool bz_grantPerm(int playerID, const char* perm);
BZF_API bool bz_revokePerm(int playerID, const char* perm);

BZF_API bool bz_getAdmin(int playerID);

BZF_API bool bz_validAdminPassword(const char* passwd);

BZF_API const char* bz_getPlayerFlag(int playerID);

BZF_API bool bz_getPlayerCurrentState(int playerID, bz_PlayerUpdateState &state);
BZF_API bool bz_getPlayerPosition(int playerID, float pos[3], bool extrapolate);
BZF_API bool bz_getPlayerRotation(int playerID, float *rot, bool extrapolate);
BZF_API bool bz_getPlayerVelocity(int playerID, float vel[3]);
BZF_API bool bz_getPlayerAngVel(int playerID, float *angvel);
BZF_API bool bz_getPlayerPhysicsDriver(int playerID, int* phydrv);

BZF_API bool bz_isPlayerPaused(int playerID);

BZF_API bool bz_canPlayerSpawn(int playerID);
BZF_API bool bz_setPlayerSpawnable(int playerID, bool spawn);
BZF_API bool bz_setPlayerLimboMessage(int playerID, const char* text);

BZF_API bz_eTeamType bz_getPlayerTeam(int playerID);
BZF_API const char* bz_getPlayerCallsign(int playerID);
BZF_API const char* bz_getPlayerIPAddress(int playerID);
BZF_API const char* bz_getPlayerReferrer(int playerID);

BZF_API bool bz_setPlayerCustomData(int playerID, const char* key, const char* data );
BZF_API const char* bz_getPlayerCustomData(int playerID, const char* key);

class BZF_API bz_BasePlayerRecord
{
 public:
  bz_BasePlayerRecord()
    : version(1), playerID(-1), team(eNoTeam)
    , currentFlagID(-1)
    , lastUpdateTime(0.0)
    , spawned(false), verified(false), globalUser(false)
    , admin(false), op(false), canSpawn(false)
    , lag(0), jitter(0), packetLoss(0.0)
    , rank(0.0), wins(0), losses(0), teamKills(0)
    {
    };

  ~bz_BasePlayerRecord() {};

  void update(void) { bz_updatePlayerData(this); } // call to update with current data

  bool hasPerm(const char* perm) { return bz_hasPerm(playerID,perm); }
  bool grantPerm(const char* perm) { return bz_grantPerm(playerID,perm); }
  bool revokePerm(const char* perm) { return bz_revokePerm(playerID,perm); }

  const char *getCustomData ( const char* key);
  bool setCustomData ( const char* key, const char* data);

  int version;
  int playerID;
  bz_ApiString callsign;

  bz_eTeamType team;

  bz_ApiString ipAddress;

  int currentFlagID;
  bz_ApiString currentFlag;
  bz_APIStringList flagHistory;

  double lastUpdateTime;
  bz_PlayerUpdateState lastKnownState;
  bz_PlayerUpdateState currentState;

  bz_ApiString clientVersion;
  bool spawned;
  bool verified;
  bool globalUser;
  bz_ApiString bzID;
  bool admin;
  bool op;
  bool canSpawn;
  bz_APIStringList groups;

  int lag;
  int jitter;
  float packetLoss;

  float rank;
  int wins;
  int losses;
  int teamKills;

};

BZF_API bool bz_setPlayerOperator(int playerId);

// team info
BZF_API unsigned int bz_getTeamPlayerLimit(bz_eTeamType team);

// player score
BZF_API bool bz_setPlayerWins(int playerId, int wins);
BZF_API bool bz_setPlayerLosses(int playerId, int losses);
BZF_API bool bz_setPlayerTKs(int playerId, int tks);

BZF_API float bz_getPlayerRank(int playerId);
BZF_API int bz_getPlayerWins(int playerId);
BZF_API int bz_getPlayerLosses(int playerId);
BZF_API int bz_getPlayerTKs(int playerId);

BZF_API bool bz_resetPlayerScore(int playerId);

// player shots
BZF_API bool bz_setPlayerShotType(int playerId, bz_eShotType shotType);

// player lag info
BZF_API int bz_getPlayerLag(int playerId);
BZF_API int bz_getPlayerJitter(int playerId);
BZF_API float bz_getPlayerPacketLoss(int playerId);

// player autopilot
BZF_API bool bz_setPlayerAutopilot(int playerId, bool autopilot);
BZF_API bool bz_getPlayerAutopilot(int playerId);


// groups API
BZF_API bz_APIStringList* bz_getGroupList(void);
BZF_API bz_APIStringList* bz_getGroupPerms(const char* group);
BZF_API bool bz_groupAllowPerm(const char* group, const char* perm);
BZF_API bz_APIStringList* bz_getStandardPermList ( void );

// message API
BZF_API bool bz_sendTextMessage(int from, int to, const char* message);
BZF_API bool bz_sendTextMessage(int from, bz_eTeamType to, const char* message);
BZF_API bool bz_sendTextMessagef(int from, int to, const char* fmt, ...) _ATTRIBUTE34;
BZF_API bool bz_sendTextMessagef(int from, bz_eTeamType to, const char* fmt, ...) _ATTRIBUTE34;
BZF_API bool bz_sendFetchResMessage(int playerID,  const char* URL);
BZF_API bool bz_sendJoinServer(int playerID, const char* address, int port,
                               int team, const char* referrer, const char* message);

// world weapons
BZF_API bool bz_fireWorldWep(const char* flagType, float lifetime, float *pos, float tilt, float direction, int shotID , float dt);
BZF_API int bz_fireWorldGM(int targetPlayerID, float lifetime, float *pos, float tilt, float direction, float dt);

// time API
BZF_API float bz_getMaxWaitTime(const char *name = NULL);
BZF_API void bz_setMaxWaitTime(float maxTime, const char *name = NULL);
BZF_API void bz_clearMaxWaitTime(const char *name);

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int dayofweek;
  bool daylightSavings;
} bz_Time;

BZF_API void bz_getLocaltime(bz_Time *ts);
BZF_API void bz_getUTCtime(bz_Time *ts);

// BZDB API
BZF_API double       bz_getBZDBDouble(const char* variable);
BZF_API bz_ApiString bz_getBZDBString(const char* variable);
BZF_API bz_ApiString bz_getBZDBDefault(const char* variable);
BZF_API bool         bz_getBZDBBool(const char* variable);
BZF_API int          bz_getBZDBInt(const char* variable);

BZF_API int bz_getBZDBItemPerms(const char* variable);
BZF_API bool bz_getBZDBItemPersistent(const char* variable);
BZF_API bool bz_BZDBItemExists(const char* variable);

BZF_API bool bz_setBZDBDouble(const char* variable, double val, int perms = 0, bool persistent = false);
BZF_API bool bz_setBZDBString(const char* variable, const char *val, int perms = 0, bool persistent = false);
BZF_API bool bz_setBZDBBool(const char* variable, bool val, int perms = 0, bool persistent = false);
BZF_API bool bz_setBZDBInt(const char* variable, int val, int perms = 0, bool persistent = false);

BZF_API bool bz_updateBZDBDouble(const char* variable, double val);
BZF_API bool bz_updateBZDBString(const char* variable, const char *val);
BZF_API bool bz_updateBZDBBool(const char* variable, bool val);
BZF_API bool bz_updateBZDBInt(const char* variable, int val);

BZF_API int bz_getBZDBVarList(bz_APIStringList *varList);

BZF_API void bz_resetBZDBVar(const char* variable);
BZF_API void bz_resetALLBZDBVars(void);

// logging
BZF_API void bz_debugMessage(int level, const char* message);
BZF_API void bz_debugMessagef(int level, const char* fmt, ...) _ATTRIBUTE23;
BZF_API int bz_getDebugLevel(void);
BZF_API int bz_setDebugLevel(int debugLevel);

// admin
BZF_API bool bz_kickUser(int playerIndex, const char* reason, bool notify);
BZF_API bool bz_IPBanUser(const char* ip, const char* source, int duration, const char* reason);
BZF_API bool bz_IDBanUser(const char *bzID, const char* source , int duration, const char *reason);
BZF_API bool bz_HostBanUser(const char* hostmask, const char* source, int duration, const char* reason);
BZF_API bool bz_IPUnbanUser(const char* ip);
BZF_API bool bz_IDUnbanUser(const char* bzID);
BZF_API bool bz_HostUnbanUser(const char* hostmask);

// ban lists
typedef enum
{
  eIPList,
  eIDList,
  eHostList
}bz_eBanListType;

BZF_API unsigned int bz_getBanListSize( bz_eBanListType listType );
BZF_API const char* bz_getBanItem ( bz_eBanListType listType, unsigned int item );
BZF_API const char* bz_getBanItemReason ( bz_eBanListType listType, unsigned int item );
BZF_API const char* bz_getBanItemSource ( bz_eBanListType listType, unsigned int item );
BZF_API double bz_getBanItemDuration ( bz_eBanListType listType, unsigned int item );
BZF_API bool bz_getBanItemIsFromMaster ( bz_eBanListType listType, unsigned int item );

// report
BZF_API bz_APIStringList* bz_getReports(void);

BZF_API unsigned int bz_getReportCount(void);
BZF_API const char* bz_getReportSource(unsigned int id);
BZF_API const char* bz_getReportBody(unsigned int id);
BZF_API const char* bz_getReportTime(unsigned int id);

BZF_API bool bz_clearReport(unsigned int id);
BZF_API bool bz_clearAllReports(void);

BZF_API bool bz_fileReport(const char* message, const char* from);

// lagwarn
BZF_API int bz_getLagWarn(void);
BZF_API bool bz_setLagWarn(int lagwarn);

// timelimit
BZF_API bool bz_setTimeLimit(float timeLimit);
BZF_API float bz_getTimeLimit(void);
BZF_API bool bz_isTimeManualStart(void);

// countdown
BZF_API bool bz_isCountDownActive(void);
BZF_API bool bz_isCountDownInProgress(void);

// polls
BZF_API bool bz_pollActive(void);
BZF_API bool bz_pollVeto(void);

// help
BZF_API bz_APIStringList* bz_getHelpTopics(void);
BZF_API bz_APIStringList* bz_getHelpTopic(const char* name);

// custom commands

class bz_CustomSlashCommandHandler
{
public:
  virtual ~bz_CustomSlashCommandHandler() {};
  virtual bool handle(int playerID, bz_ApiString command, bz_ApiString message, bz_APIStringList *params) = 0;
  virtual const char* help(bz_ApiString /* command */) { return NULL; }
};

BZF_API bool bz_registerCustomSlashCommand(const char* command, bz_CustomSlashCommandHandler *handler);
BZF_API bool bz_removeCustomSlashCommand(const char* command);

// spawning
BZF_API bool bz_getStandardSpawn(int playerID, float pos[3], float *rot);

// dying
BZF_API bool bz_killPlayer(int playerID, bool spawnOnBase, int killerID = -1, const char* flagID = NULL);

// flags
BZF_API bool bz_givePlayerFlag(int playerID, const char* flagType, bool force);
BZF_API bool bz_removePlayerFlag(int playerID);
BZF_API void bz_resetFlags(bool onlyUnused);

BZF_API unsigned int bz_getNumFlags(void);
BZF_API const bz_ApiString bz_getFlagName(int flag);
BZF_API bool bz_resetFlag(int flag);
BZF_API int bz_flagPlayer(int flag);
BZF_API bool bz_getFlagPosition(int flag, float* pos);
BZF_API bool bz_moveFlag(int flag, float pos[3], bool reset = true);

// world
BZF_API bool bz_setWorldSize(float size, float wallHeight = -1.0);
BZF_API void bz_setClientWorldDownloadURL(const char* URL);
BZF_API const bz_ApiString bz_getClientWorldDownloadURL(void);
BZF_API bool bz_saveWorldCacheFile(const char* file);
BZF_API unsigned int bz_getWorldCacheSize(void);
BZF_API unsigned int bz_getWorldCacheData(unsigned char *data);

// custom map objects

typedef struct bz_CustomMapObjectInfo
{
  bz_ApiString     fileName;
  int              lineNum;
  bz_ApiString     name;
  bz_ApiString     args;
  bz_APIStringList data;
  bz_ApiString     newData;
} bz_CustomMapObjectInfo;

class bz_CustomMapObjectHandler
{
public:
  virtual ~bz_CustomMapObjectHandler() {};
  virtual bool handle(bz_ApiString object, bz_CustomMapObjectInfo *data) = 0;
};

BZF_API bool bz_registerCustomMapObject(const char* object, bz_CustomMapObjectHandler *handler);
BZF_API bool bz_registerCustomMapObject2(const char* object, const char* end,
                                         bz_CustomMapObjectHandler *handler);
BZF_API bool bz_removeCustomMapObject(const char* object);


// info about the world
typedef enum {
  eNullObject,
  eSolidObject,
  eTeleporterField,
  eWorldWeapon
} bz_eWorldObjectType;

class BZF_API bz_APIBaseWorldObject
{
 public:
  bz_APIBaseWorldObject() { type = eNullObject; }
  virtual ~bz_APIBaseWorldObject() {};
  bz_eWorldObjectType type;
  bz_ApiString name;
  unsigned int id;
};

typedef enum {
  eWallObject,
  eBoxObject,
  eBaseObject,
  ePyramidObject,
  eMeshObject,
  eArcObject,
  eConeObject,
  eSphereObject,
  eUnknownObject
} bz_eSolidWorldObjectType;

class BZF_API bz_SolidObjectPassableAtributes
{
 public:
  bz_SolidObjectPassableAtributes()
    {
      red = false;
      green = false;
      blue = false;
      purple = false;
      rogue = false;
    }

  void setAll(bool val)
  {
    red = val;
    green = val;
    blue = val;
    purple = val;
    rogue = val;
  }

  bool allTrue(void) const { return red && green && blue && purple && rogue; }
  bool allFalse(void) const { return !red && !green && !blue && !purple && !rogue; }

  bool red;
  bool green;
  bool blue;
  bool purple;
  bool rogue;
};

class BZF_API bz_APISolidWorldObject_V1 : public bz_APIBaseWorldObject
{
 public:
  bz_APISolidWorldObject_V1();
  virtual ~bz_APISolidWorldObject_V1();

  bz_eSolidWorldObjectType  solidType;
  int			    subID;

  float center[3];
  float maxAABBox[3];
  float minAABBox[3];
  float rotation[3];
  float maxBBox[3];
  float minBBox[3];

  virtual bool collide(float pos[3], float rad, float* hit);

  bz_SolidObjectPassableAtributes   shootThru;
  bz_SolidObjectPassableAtributes   driveThru;
};

class BZF_API bz_CTFBaseWorldObject_V1 : public bz_APISolidWorldObject_V1
{
 public:
  bz_CTFBaseWorldObject_V1();
  virtual ~bz_CTFBaseWorldObject_V1();

  bz_eTeamType team;
};


class BZF_API bz_APITeleporterField_V1 : public bz_APIBaseWorldObject
{
 public:
  bz_APITeleporterField_V1() {};
  virtual ~bz_APITeleporterField_V1() {};

  float center[3];
  float maxAABBox[3];
  float minAABBox[3];
  float rotation[3];
  float maxBBox[3];
  float minBBox[3];

  bz_ApiString name;
  bz_APIStringList targets[2];
};

class BZF_API bz_APIWorldObjectList
{
 public:
  bz_APIWorldObjectList();
  bz_APIWorldObjectList(const bz_APIWorldObjectList &r);
  ~bz_APIWorldObjectList();
  void push_back(bz_APIBaseWorldObject *value);
  bz_APIBaseWorldObject *get(unsigned int i);
  const bz_APIBaseWorldObject* operator[] (unsigned int i) const;
  bz_APIWorldObjectList& operator = (const bz_APIWorldObjectList& r);
  unsigned int size(void);
  void clear(void);

 protected:
  class dataBlob;
  dataBlob *data;
};

BZF_API void bz_getWorldSize(float *size, float *wallHeight);
BZF_API unsigned int bz_getWorldObjectCount(void);
BZF_API bz_APIWorldObjectList* bz_getWorldObjectList(void);
BZF_API void bz_releaseWorldObjectList(bz_APIWorldObjectList *list);
BZF_API unsigned int bz_findWorldObject(const char *name);
BZF_API bz_APIBaseWorldObject* bz_getWorldObjectByID(unsigned int id);

BZF_API int          bz_getLinkSrcID(const char* srcName, int lastID = -1);
BZF_API int          bz_getLinkDstID(const char* dstName, int lastID = -1);
BZF_API const char*  bz_getLinkSrcName(int linkSrcID);
BZF_API const char*  bz_getLinkDstName(int linkDstID);

BZF_API int          bz_getPhyDrvID(const char* phyDrvName);
BZF_API const char*  bz_getPhyDrvName(unsigned int phyDrvID);

BZF_API bool bz_SetWorldObjectTangibility(int id, const bz_SolidObjectPassableAtributes &atribs);
BZF_API bool bz_GetWorldObjectTangibility(int id, bz_SolidObjectPassableAtributes &atribs);
BZF_API void bz_ResetWorldObjectTangibilities(void);

// collision methods
typedef enum {
  eNoCol,
  eInSolid,
  eInBase,
  eInTP
} bz_eAPIColType;

// these realy need to return the object they came from
bz_eAPIColType bz_cylinderInMapObject(float pos[3], float height, float radius, bz_APIBaseWorldObject **object);
bz_eAPIColType bz_boxInMapObject(float pos[3], float size[3], float angle, bz_APIBaseWorldObject **object);

void bz_freeWorldObjectPtr(bz_APIBaseWorldObject *ptr);

// public server info
BZF_API bool bz_getPublic(void);
BZF_API bz_ApiString bz_getPublicAddr(void);
BZF_API int bz_getPublicPort(void);
BZF_API bz_ApiString bz_getPublicDescription(void);

// plug-in management
BZF_API int bz_getLoadedPlugins(bz_APIStringList *list);
BZF_API bool bz_loadPlugin(const char* path, const char* params);
BZF_API bool bz_unloadPlugin(const char* path);

// bz_load path functions
// only valid inside the load function for a plugin
BZF_API const char* bz_pluginBinPath(void);


// custom client sounds
BZF_API bool bz_sendPlayCustomLocalSound(int playerID, const char* soundName);

class bz_APIPluginHandler
{
public:
  virtual ~bz_APIPluginHandler() {};
  virtual bool handle(bz_ApiString plugin, bz_ApiString param) = 0;
};
// custom pluginHandler
BZF_API bool bz_registerCustomPluginHandler(const char* extension, bz_APIPluginHandler * handler);
BZF_API bool bz_removeCustomPluginHandler(const char* extension, bz_APIPluginHandler * handler);

class bz_GenericCallback
{
public:
  virtual ~bz_GenericCallback() {};
  virtual bool call(void *param) = 0;
};

typedef bool (*bz_GenericCallbackFunc) (void *param);

BZF_API bool bz_registerCallBack(const char* name, bz_GenericCallback *callback);
BZF_API bool bz_registerCallBack(const char* name, bz_GenericCallbackFunc callback);
BZF_API bool bz_removeCallBack(const char* name, bz_GenericCallback *callback);
BZF_API bool bz_removeCallBack(const char* name, bz_GenericCallbackFunc callback);

BZF_API bool bz_callCallback(const char* name, void *param);

BZF_API bool bz_callbackExists(const char* name);


// team info
BZF_API int bz_getTeamCount(bz_eTeamType team);
BZF_API int bz_getTeamScore(bz_eTeamType team);
BZF_API int bz_getTeamWins(bz_eTeamType team);
BZF_API int bz_getTeamLosses(bz_eTeamType team);

BZF_API void bz_setTeamWins(bz_eTeamType team, int wins);
BZF_API void bz_setTeamLosses(bz_eTeamType team, int losses);

BZF_API void bz_resetTeamScore(bz_eTeamType team);
BZF_API void bz_resetTeamScores(void);

// list server
BZF_API void bz_updateListServer(void);

// url API
class bz_BaseURLHandler
{
public:
  bz_BaseURLHandler() { version = 1; }
  virtual ~bz_BaseURLHandler() {};

  virtual void done(const char* URL, void * data, unsigned int size, bool complete) = 0;
  virtual void timeout(const char* /*URL*/, int /*errorCode*/) {};
  virtual void error(const char* /*URL*/, int /*errorCode*/, const char * /*errorString*/) {};

protected:
  int version;
};

BZF_API bool bz_addURLJob(const char* URL, bz_BaseURLHandler* handler = NULL, const char* postData = NULL);
BZF_API bool bz_removeURLJob(const char* URL);
BZF_API size_t bz_addURLJobForID(const char* URL,
                                 bz_BaseURLHandler* handler = NULL,
                                 const char* postData = NULL);
BZF_API bool bz_removeURLJobByID(size_t id);
BZF_API bool bz_stopAllURLJobs(void);

// inter plugin communication
BZF_API bool bz_clipFieldExists(const char *name);
BZF_API const char* bz_getclipFieldString(const char *name);
BZF_API float bz_getclipFieldFloat(const char *name);
BZF_API int bz_getclipFieldInt(const char *name);

BZF_API bool bz_setclipFieldString(const char *name, const char* data);
BZF_API bool bz_setclipFieldFloat(const char *name, float data);
BZF_API bool bz_setclipFieldInt(const char *name, int data);

class bz_ClipFiledNotifier
{
public:
  virtual ~bz_ClipFiledNotifier() {};
  virtual void fieldChange(const char* /*field*/) = 0;
};

BZF_API bool bz_addClipFieldNotifier(const char *name, bz_ClipFiledNotifier *cb);
BZF_API bool bz_removeClipFieldNotifier(const char *name, bz_ClipFiledNotifier *cb);

// path checks
BZF_API bz_ApiString bz_filterPath(const char* path);

// Record-Replay
BZF_API bool bz_saveRecBuf(const char * _filename, int seconds);
BZF_API bool bz_startRecBuf(void);
BZF_API bool bz_stopRecBuf(void);

// cheap Text Utils
BZF_API const char *bz_format(const char* fmt, ...)_ATTRIBUTE12;
BZF_API const char *bz_toupper(const char* val);
BZF_API const char *bz_tolower(const char* val);
BZF_API const char *bz_urlEncode(const char* val);

// game countdown
BZF_API void bz_pauseCountdown(const char *pausedBy);
BZF_API void bz_resumeCountdown(const char *resumedBy);
BZF_API void bz_startCountdown(int delay, float limit, const char *byWho);

// server control
BZF_API void bz_shutdown();
BZF_API bool bz_restart(void);
BZF_API void bz_superkill();
BZF_API void bz_gameOver(int playerID, bz_eTeamType = eNoTeam);

BZF_API void bz_reloadLocalBans();
BZF_API void bz_reloadMasterBans();
BZF_API void bz_reloadGroups();
BZF_API void bz_reloadUsers();
BZF_API void bz_reloadHelp();

// rabbit control
BZF_API void bz_newRabbit(int player, bool swap);
BZF_API void bz_removeRabbit(int player);

// team control
BZF_API void bz_changeTeam(int player, bz_eTeamType team);

// player control
BZF_API bool bz_zapPlayer(int player ); // forces a respawn

// player listing
BZF_API bz_APIIntList* bz_getPlayerIndexList(void);
BZF_API int bz_getPlayerCount(void);
BZF_API bool bz_anyPlayers(void);

// info about the world
BZF_API bz_eTeamType bz_checkBaseAtPoint(float pos[3]);

// game info
BZF_API bz_eGameType bz_getGameType(void);

BZF_API bool bz_allowJumping(void);


// utility
BZF_API const char* bz_MD5(const char * str);
BZF_API const char* bz_MD5(const void * data, size_t size);

BZF_API const char* bz_getServerVersion(void);
BZF_API const char* bz_getProtocolVersion(void);

// server side player API

typedef struct {
  int index;
  char type[2];
  int status;
  int endurance;
  int owner;
  float position[3];
  float launchPosition[3];
  float landingPosition[3];
  float flightTime;
  float flightEnd;
  float initialVelocity;
} bz_FlagUpdateRecord;

typedef struct {
  float rank;
  int wins;
  int losses;
  int tks;
} bz_ScoreRecord;

typedef enum {
  eRegularPlayer,
  eRobotPlayer,
  eObserverPlayer
} bz_ePlayerType;

typedef struct {
  int index;
  bz_ePlayerType type;
  bz_eTeamType team;
  bz_ScoreRecord score;
  char callsign[32];
} bz_PlayerInfoUpdateRecord;

typedef struct {
  int id;
  int size;
  int wins;
  int losses;
} bz_TeamInfoRecord;

typedef enum {
  eRejectBadRequest,
  eRejectBadTeam,
  eRejectBadType,
  eRejectUNUSED,
  eRejectTeamFull,
  eRejectServerFull,
  eRejectBadCallsign,
  eRejectRepeatCallsign,
  eRejectRejoinWaitTime,
  eRejectIPBanned,
  eRejectHostBanned,
  eRejectIDBanned
} bz_eRejectCodes;

typedef struct {
  int player;
  int handicap;
} bz_HandicapUpdateRecord;

typedef enum {
  eGotKilled,
  eGotShot,
  eGotRunOver,
  eGotCaptured,
  eGenocideEffect,
  eSelfDestruct,
  eWaterDeath,
  ePhysicsDriverDeath
} bz_ePlayerDeathReason;

class BZF_API bz_ServerSidePlayerHandler
{
 public:
   bz_ServerSidePlayerHandler();
  virtual ~bz_ServerSidePlayerHandler() {};

  int getPlayerID ( void ){return playerID;}

  void update ( void );

  // you must call setPlayerData when this is called.
  virtual void added(int player) = 0; // it is required that the bot provide this method

  // lower level events for various things that happen in the game
  virtual void removed(void) {};
  virtual void playerRemoved(int player);
  virtual void playerRejected(bz_eRejectCodes code, const char *reason);
  virtual void playerAccepted(void) {};
  virtual void playerSpawned(int player, const float pos[3], float rot);
  virtual void textMessage(int dest, int source, const char *text);
  virtual void playerKilledMessage(int victimIndex, int killerIndex,
				   bz_ePlayerDeathReason reason, int shotIndex,
				   const char *flagType, int phydrv);
  virtual void scoreLimitReached(int player, bz_eTeamType team);
  virtual void flagCaptured(int player, int flag, bz_eTeamType team);

  virtual void flagUpdate(int count, bz_FlagUpdateRecord **flagList);
  virtual void playerInfoUpdate(bz_PlayerInfoUpdateRecord *playerRecord);
  virtual void teamUpdate(int count, bz_TeamInfoRecord **teamList);
  virtual void handicapUpdate(int count,
			      bz_HandicapUpdateRecord **handicapList);
  virtual void playerIPUpdate(int player, const char *ipAddress);
  virtual void playerStateUpdate(int player, bz_PlayerUpdateState *playerState,
				 double timestamp);
  virtual void playerScoreUpdate(int player, float rank, int wins, int losses, int TKs);
  virtual void flagTransfer(int from, int to, int flagID, bz_eShotType shotType);
  virtual void nearestFlag(const char* flagName, const float pos[3]);
  virtual void grabFlag(int player, int flagID, const char* flagType, bz_eShotType shotType);
  virtual void setShotType(int player, bz_eShotType shotType);
  virtual void shotFired(int player, unsigned short shotID, bz_eShotType shotType);
  virtual void shotEnded(int player, unsigned short shotID, unsigned short reason);
  virtual void playerTeleported(int player, unsigned short from, unsigned short to);
  virtual void playerAutopilot(int player, bool autopilot);
  virtual void allowSpawn(bool canSpawn);

  // higher level functions for events that happen to the bot
  typedef enum
  {
    eWorldDeath,
    eServerDeath,
    eOtherDeath
  }SmiteReason;

  virtual void spawned(void); // the bot has spawned
  virtual void died ( int killer ); // the bot has died from gameplay
  virtual void smote ( SmiteReason reason = eOtherDeath ); // the bot has died from some other manner
  virtual void jumped ( void ){}; // the bot has left the ground
  virtual void landed ( void ){}; // the bot has landed
  virtual void collide ( bz_APISolidWorldObject_V1* /*object*/, float* /*pos*/ ){}; // the bot ran into an object
  virtual void outOfBounds (  float /*pos*/[3]){}; // the bot ran into the outer walls
  virtual void flagPickup ( const char* /*flag*/ ){}; // bot got a flag
  virtual void shotChange ( bz_eShotType /*shotType*/ ){}; // bot got shot change

  // give the bot time to do it's processing
  virtual bool think(void); // return true to kill and delete the bot;

  void setPlayerID ( int id ){playerID = id;}

  // actions to make
  void setPlayerData(const char *callsign,
		     const char *token, const char *clientVersion,
		     bz_eTeamType team);

  void joinGame(void);
  void respawn(void);
  void getCurrentState(bz_PlayerUpdateState *state);

  void sendChatMessage(const char* text, int targetPlayer = BZ_ALLUSERS);
  void sendTeamChatMessage(const char *text, bz_eTeamType targetTeam);

  void dropFlag( void );
  void setMovement(float forward, float turn);
  bool fireShot(void);
  bool jump(void);

  // state info
  bool canJump(void);
  bool canShoot(void);
  bool canMove(void);
  bool falling (void);
  bz_eShotType getShotType(void);

  void getPosition ( float *p );
  void getVelocity ( float *v );
  float getFacing ( void );

  float getMaxLinSpeed ( void );
  float getMaxRotSpeed ( void );

  // state actions
  void updatePhysics(void);
  void setAutoSpawn ( bool s = true ){autoSpawn = s;}

  int playerID;

 private:
  float input[2];
  bool	wantToJump;

  bool autoSpawn;

public:
  class BZF_API UpdateInfo
  {
  public:
    float pos[3];
    float vec[3];  // FIXME -- vel for velocity?
    float rot;     // FIXME -- radians or degrees?
    float rotVel;
    double time;

    UpdateInfo()
      : rot(0), rotVel(0), time(0)
    {
      for (int i = 0; i < 3; i++)
	pos[i] = vec[0] =0;
    }

    UpdateInfo& operator = ( const UpdateInfo& u )
    {
      memcpy(pos,u.pos,sizeof(float)*3);
      memcpy(vec,u.vec,sizeof(float)*3);
      rot = u.rot;
      rotVel = u.rotVel;
      time = u.time;

      return *this;
    }
    float getDelta( const UpdateInfo & state);
  };

private:
  UpdateInfo lastUpdate;
  UpdateInfo currentState;

  int flaps;

  bool alive;
};

// *** NOTE *** support for server side players in incomplete.
//  there WILL be crashes if you add one.
// this message will be removed when the code is complete.
BZF_API int bz_addServerSidePlayer(bz_ServerSidePlayerHandler *handler);
BZF_API bool bz_removeServerSidePlayer(int playerID, bz_ServerSidePlayerHandler *handler); // you have to pass in the handler to ensure you "own" the player

// Note: there is NO bz_UnregisterCustomFlag, 'cause that would jack up connected clients.
// If you really need to unregister a flag, shut down the server.
BZF_API bool bz_RegisterCustomFlag(const char* abbr, const char* name,
				   const char* helpString, bz_eShotType shotType,
				   bz_eFlagQuality quality);

#endif //_BZFS_API_H_

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
