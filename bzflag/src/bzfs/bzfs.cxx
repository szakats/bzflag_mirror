/* bzflag
 * Copyright (c) 1993 - 2001 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

static const char copyright[] = "Copyright (c) 1993 - 2001 Tim Riker";

// to enforce a game time limit
#define TIMELIMIT
// to dump score info to stdout
//#define PRINTSCORE to include code to dump score info to stdout


// Like verbose debug messages?
#define UDEBUG if (debug > 2) printf

// Like version status messages?
#define UMDEBUG if (debug > 3) printf

#define SERVERLOGINMSG true

const int MaxPlayers = 40;
const int MaxShots = 10;
const int udpBufSize = 128000;
#if defined(__sgi)
#define FD_SETSIZE (MaxPlayers + 10)
#endif /* defined(__sgi) */

// must be before network.h because that defines a close() macro which
// messes up fstreams.  luckily, we don't need to call the close() method
// on any fstream.
#include "bzfio.h"
#include <fstream.h>

// must be before windows.h
#include "network.h"
#include <iomanip.h>

#if defined(_WIN32)
#include <windows.h>
#define strcasecmp _stricmp
#define sleep(_x) Sleep(1000 * (_x))
#endif /* defined(_WIN32) */

#include <stdio.h>
#if !defined(_WIN32)
#include <fcntl.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include "bzsignal.h"
#include <time.h>
#include "common.h"
#include "global.h"
#include "Protocol.h"
#include "Address.h"
#include "Pack.h"
#include "TimeKeeper.h"
#include "Flag.h"
#include "Team.h"
#include "multicast.h"
#include "Ping.h"
#include "TimeBomb.h"

// DisconnectTimeout is how long to wait for a reconnect before
// giving up.  this should be pretty short to avoid bad clients
// from using up our resources, but long enough to allow for
// even a slow client/connection.
static const float DisconnectTimeout = 10.0f;

// every ListServerReAddTime server add ourself to the list
// server again.  this is in case the list server has reset
// or dropped us for some reason.
static const float ListServerReAddTime = 30.0f * 60.0f;

// maximum number of list servers to advertise ourself to
static const int MaxListServers = 5;

static const float FlagHalfLife = 45.0f;
// do NOT change
static int NotConnected = -1;
static int InvalidPlayer = -1;

//The minimum height above ground an object must be in order
//to have a flag appear beneath it
static float flagHeight = FlagAltitude;
// meters
//float WorldSize = 800.0f;

// custom server login message
static char *servermsg = NULL;

enum PlayerState {
  // unconnected or entered
  PlayerInLimbo,
  PlayerOnTeamDead,
  PlayerOnTeamAlive
};

struct ConnectInfo {
  public:
    // what fd we accepted on
    int accept;
    // what fd we're listening on
    int listen;
    // time accepted
    TimeKeeper time;
};

struct PacketQueue {
  public:
    unsigned short seqno;
    void *data;
    int length;
    struct PacketQueue *next;
};

struct PlayerInfo {
  public:
    // socket file descriptor
    int fd;
    // peer's network address
    Address peer;
    // current state of player
    PlayerState state;
    // player's id
    PlayerId id;
    // does player know his real id?
    int knowId;
    // what an old client thinks its id is
    PlayerId oldId;
    // type of player
    PlayerType type;
    // player's pseudonym
    char callSign[CallSignLen];
    // player's email address
    char email[EmailLen];
    // player's team
    TeamColor team;
    // flag index player has
    int flag;
    // player's score
    int wins, losses;
    // if player can't multicast
    boolean multicastRelay;

    // input buffers
    // bytes read in current msg
    int tcplen;
    // current TCP msg
    char tcpmsg[MaxPacketLen];
    // bytes read in current msg
    int udplen;
    // current UDP msg
    char udpmsg[MaxPacketLen];

    // output buffer
    int outmsgOffset;
    int outmsgSize;
    int outmsgCapacity;
    char *outmsg;

    // UDP connection
    boolean ulinkup;
    struct sockaddr_in uaddr;
    // TCP connection
    struct sockaddr_in taddr;

    // UDP message queue
    struct PacketQueue *uqueue;
    struct PacketQueue *dqueue;
    unsigned short lastRecvPacketNo;
    unsigned short lastSendPacketNo;

    boolean toBeKicked;

    boolean Admin;

    // lag measurement
    bool lagkillerpending;
    TimeKeeper lagkillertime;
    float lagavg,lagalpha;
    int lagcount,laglastwarn,lagwarncount;
};

#define SEND 1
#define RECEIVE 0

struct FlagInfo {
  public:
    // flag info
    Flag flag;
    // player index who has flag
    int player;
    // how many grabs before removed
    int grabs;
    // True if flag must be in game
    boolean required;
    // time flag will land
    TimeKeeper dropDone;
};

struct TeamInfo {
  public:
    Team team;
    // player index with radio
    int radio;
};

class WorldInfo {
  public:
    WorldInfo();
    ~WorldInfo();

    void addWall(float x, float y, float z, float r, float w, float h);
    void addBox(float x, float y, float z, float r, float w, float d, float h);
    void addPyramid(float x, float y, float z, float r, float w, float d, float h);
    void addTeleporter(float x, float y, float z, float r, float w, float d, float h, float b);
    void addLink(int from, int to);
    int packDatabase();
    void *getDatabase() const;
    int getDatabaseSize() const;
    
  private:
    boolean inRect(const float *p1, float angle, const float *size, float x, float y, float radius) const;
    boolean rectHitCirc(float dx, float dy, const float *p, float r) const;

  public:
    struct ObstacleLocation {
      public:
	float pos[3];
	float rotation;
	float size[3];
      ObstacleLocation &operator=( const ObstacleLocation &ol )
      {
	memcpy(pos, ol.pos, sizeof(float) * 3);
	rotation = ol.rotation;
	memcpy(size, ol.size, sizeof(float) * 3);
	return *this;
      }
    };

    struct Teleporter : public ObstacleLocation {
      public:
	float border;
	int to[2];
    };

    int inBuilding(ObstacleLocation **location, float x, float y, float z, float radius) const;

  private:
    int numWalls;
    int numBoxes;
    int numPyramids;
    int numTeleporters;
    int sizeWalls;
    int sizeBoxes;
    int sizePyramids;
    int sizeTeleporters;
    ObstacleLocation *walls;
    ObstacleLocation *boxes;
    ObstacleLocation *pyramids;
    Teleporter *teleporters;
    char *database;
    int databaseSize;
};

class ListServerLink {
  public:
    Address address;
    int port;
    int socket;
    const char *nextMessage;
};

// default port
static int wksPort = ServerPort;
// well known service socket
static int wksSocket;
static Address serverAddress;
// udpSocket should also be on serverAddress
static int udpSocket;
static boolean useGivenPort = False;
static boolean useFallbackPort = False;
// listen for pings here
static int pingInSocket;
static struct sockaddr_in pingInAddr;
// reply to pings here
static int pingOutSocket;
static struct sockaddr_in pingOutAddr;
// broadcast pings in/out here
static int pingBcastSocket;
static struct sockaddr_in pingBcastAddr;
// listen for player packets
static int relayInSocket;
static struct sockaddr_in relayInAddr;
// relay player packets
static int relayOutSocket;
static struct sockaddr_in relayOutAddr;
static const char *pingInterface = NULL;
static int pingTTL = DefaultTTL;
static int playerTTL = DefaultTTL;
static boolean handlePings = True;
static boolean noMulticastRelay = False;
static PingPacket pingReply;
// highest fd used
static int maxFileDescriptor;
// players list
static PlayerInfo player[MaxPlayers];
// network reconnection list
static ConnectInfo reconnect[MaxPlayers+1];
// team info
static TeamInfo team[NumTeams];
// flags list
static FlagInfo *flag = NULL;
// num flags in flag list
static int numFlags;
static int numFlagsInAir;
// types of extra flags allowed
static FlagId *allowedFlags = NULL;
static int numAllowedFlags;
// num randomly generated flags
static int numExtraFlags;
static boolean done = False;
// True if hit time/score limit
static boolean gameOver = True;
static int exitCode = 0;
// True if -b on cmd line
static boolean randomBoxes;
// True if -h on cmd line
static boolean randomHeights;
// True if -t on cmd line
static boolean useTeleporters;
// True if -fb on cmd line
static boolean flagsOnBuildings;
// True if -g on cmd line
static boolean oneGameOnly;
static int gameStyle;
static uint16_t maxPlayers = MaxPlayers;
// max simulataneous per player
static uint16_t maxShots;
static uint16_t maxTeam[NumTeams];
static uint16_t shakeWins = 0;
static uint16_t shakeTimeout = 0;
static float linearAcceleration;
static float angularAcceleration;
static int broadcastRadio = InvalidPlayer;
static int maxPlayerScore = 0;
static int maxTeamScore = 0;
static int debug = 0;

// True if only new clients allowed
static boolean requireUDP;
// True if UDP can be used in parallel to TCP connections
static boolean alsoUDP;

#ifdef PRINTSCORE
static boolean printScore = False;
#endif
#ifdef TIMELIMIT
static float timeLimit = 0.0f;
static float timeElapsed = 0.0f;
static TimeKeeper gameStartTime;
#endif
static boolean publicizeServer = False;
static BzfString publicizedAddress;
static boolean publicizedAddressGiven = False;
static const char *publicizedTitle = NULL;
static const char *listServerURL = DefaultListServerURL;
static TimeKeeper listServerLastAddTime;
static ListServerLink listServerLinks[MaxListServers];
static int listServerLinksCount = 0;

static WorldInfo *world = NULL;
static char *worldDatabase = NULL;
static int worldDatabaseSize = 0;
static float basePos[NumTeams][3];
static float baseRotation[NumTeams];
static float baseSize[NumTeams][3];
static float safetyBasePos[NumTeams][3];
static const char *worldFile = NULL;

static float lagwarnthresh = -1.0;
static int maxlagwarn = 10000;
static char *password = NULL;

static void stopPlayerPacketRelay();
static void removePlayer(int playerIndex);
static void resetFlag(int flagIndex);
static void releaseRadio(int playerIndex);

//
// types for reading world files
//

class WorldFileObject {
  public:
    WorldFileObject() { }
    virtual ~WorldFileObject() { }

    virtual bool read(const char *cmd, istream&) = 0;
    virtual void write(WorldInfo*) const = 0;
};

class WorldFileObstacle : public WorldFileObject {
  public:
    WorldFileObstacle();
    virtual bool read(const char *cmd, istream&);

  protected:
    float pos[3];
    float rotation;
    float size[3];
};

WorldFileObstacle::WorldFileObstacle()
{
   pos[0] = pos[1] = pos[2] = 0.0f;
   rotation = 0.0f;
   size[0] = size[1] = size[2] = 1.0f;
}

bool WorldFileObstacle::read(const char *cmd, istream& input)
{
  if (strcmp(cmd, "position") == 0)
    input >> pos[0] >> pos[1] >> pos[2];
  else if (strcmp(cmd, "rotation") == 0) {
    input >> rotation;
    rotation = rotation * M_PI / 180.0f;
  } else if (strcmp(cmd, "size") == 0)
    input >> size[0] >> size[1] >> size[2];
  else
    return False;
  return True;
}

class CustomBox : public WorldFileObstacle {
  public:
    CustomBox();
    virtual void write(WorldInfo*) const;
};

CustomBox::CustomBox()
{
  size[0] = size[1] = BoxBase;
  size[2] = BoxHeight;
}

void CustomBox::write(WorldInfo *world) const
{
  world->addBox(pos[0], pos[1], pos[2], rotation, size[0], size[1], size[2]);
}

class CustomPyramid : public WorldFileObstacle {
  public:
    CustomPyramid();
    virtual void write(WorldInfo*) const;
};

CustomPyramid::CustomPyramid()
{
  size[0] = size[1] = PyrBase;
  size[2] = PyrHeight;
}

void CustomPyramid::write(WorldInfo *world) const
{
  world->addPyramid(pos[0], pos[1], pos[2], rotation, size[0], size[2], size[2]);
}

class CustomGate : public WorldFileObstacle {
  public:
    CustomGate();
    virtual bool read(const char *cmd, istream&);
    virtual void write(WorldInfo*) const;

  protected:
    float border;
};

CustomGate::CustomGate()
{
  size[0] = 0.5f * TeleWidth;
  size[1] = TeleBreadth;
  size[2] = 2.0f * TeleHeight;
  border = TeleWidth;
}

bool CustomGate::read(const char *cmd, istream& input)
{
  if (strcmp(cmd, "border") == 0)
    input >> border;
  else
    return WorldFileObstacle::read(cmd, input);
  return True;
}

void CustomGate::write(WorldInfo *world) const
{
  world->addTeleporter(pos[0], pos[1], pos[2], rotation, size[0], size[1], size[2], border);
}

class CustomLink : public WorldFileObject {
  public:
    CustomLink();
    virtual bool read(const char *cmd, istream&);
    virtual void write(WorldInfo*) const;

  protected:
    int from;
    int to;
};

CustomLink::CustomLink()
{
  from = 0;
  to = 0;
}

bool CustomLink::read(const char *cmd, istream& input)
{
  if (strcmp(cmd, "from") == 0)
    input >> from;
  else if (strcmp(cmd, "to") == 0)
    input >> to;
  else
    return False;
  return True;
}

void CustomLink::write(WorldInfo *world) const
{
  world->addLink(from, to);
}

class CustomBase : public WorldFileObstacle {
  public:
    CustomBase();
    virtual bool read(const char *cmd, istream&);
    virtual void write(WorldInfo*) const;

  protected:
    int color;
};

CustomBase::CustomBase()
{
  pos[0] = pos[1] = pos[2] = 0.0f;
  rotation = 0.0f;
  size[0] = size[1] = BaseSize;
}

bool CustomBase::read(const char *cmd, istream& input) {
  if (strcmp(cmd, "color") == 0)
    input >> color;
  else
    return WorldFileObstacle::read(cmd, input);
  return True;
}

void CustomBase::write(WorldInfo* world) const {
  basePos[color][0] = pos[0];
  basePos[color][1] = pos[1];
  basePos[color][2] = pos[2];
  baseRotation[color] = rotation;
  baseSize[color][0] = size[0];
  baseSize[color][1] = size[1];
  safetyBasePos[color][2] = basePos[color][2];
}

class CustomWorld : public WorldFileObject {
  public:
    CustomWorld();
    virtual bool read(const char *cmd, istream&);
    virtual void write(WorldInfo*) const;

  protected:
    int size;
    int fHeight;
};

CustomWorld::CustomWorld()
{
  size = 800;
  fHeight = 0;
}

bool CustomWorld::read(const char *cmd, istream& input)
{
  if (strcmp(cmd, "size") == 0)
    input >> size;
  else if (strcmp(cmd, "flagHeight") == 0)
    input >> fHeight;
  else
    return False;
  return True;
}

void CustomWorld::write(WorldInfo * /*world*/) const
{
  flagHeight = (float) fHeight;
  //WorldSize = size;
  //world->addLink(from, to);
}

// list of world file objects
BZF_DEFINE_ALIST(WorldFileObjectList, WorldFileObject*);

static void emptyWorldFileObjectList(WorldFileObjectList& list)
{
  const int n = list.getLength();
  for (int i = 0; i < n; ++i)
    delete list[i];
  list.removeAll();
}

// WorldInfo
WorldInfo::WorldInfo() :
    numWalls(0),
    numBoxes(0),
    numPyramids(0),
    numTeleporters(0),
    sizeWalls(0),
    sizeBoxes(0),
    sizePyramids(0),
    sizeTeleporters(0),
    walls(NULL),
    boxes(NULL),
    pyramids(NULL),
    teleporters(NULL),
    database(NULL)
{
}

WorldInfo::~WorldInfo()
{
  free(walls);
  free(boxes);
  free(pyramids);
  free(teleporters);
  delete[] database;
}

void WorldInfo::addWall(float x, float y, float z, float r, float w, float h)
{
  if (numWalls >= sizeWalls) {
    sizeWalls = (sizeWalls == 0) ? 16 : 2 * sizeWalls;
    walls = (ObstacleLocation *)realloc(walls, sizeof(ObstacleLocation) * sizeWalls);
  }
  walls[numWalls].pos[0] = x;
  walls[numWalls].pos[1] = y;
  walls[numWalls].pos[2] = z;
  walls[numWalls].rotation = r;
  walls[numWalls].size[0] = w;
  // no depth to walls
  walls[numWalls].size[1] = 0.0f;
  walls[numWalls].size[2] = h;
  numWalls++;
}

void WorldInfo::addBox(float x, float y, float z, float r, float w, float d, float h)
{
  if (numBoxes >= sizeBoxes) {
    sizeBoxes = (sizeBoxes == 0) ? 16 : 2 * sizeBoxes;
    boxes = (ObstacleLocation *)realloc(boxes, sizeof(ObstacleLocation) * sizeBoxes);
  }
  boxes[numBoxes].pos[0] = x;
  boxes[numBoxes].pos[1] = y;
  boxes[numBoxes].pos[2] = z;
  boxes[numBoxes].rotation = r;
  boxes[numBoxes].size[0] = w;
  boxes[numBoxes].size[1] = d;
  boxes[numBoxes].size[2] = h;
  numBoxes++;
}

void WorldInfo::addPyramid(float x, float y, float z, float r, float w, float d, float h)
{
  if (numPyramids >= sizePyramids) {
    sizePyramids = (sizePyramids == 0) ? 16 : 2 * sizePyramids;
    pyramids = (ObstacleLocation *)realloc(pyramids, sizeof(ObstacleLocation) * sizePyramids);
  }
  pyramids[numPyramids].pos[0] = x;
  pyramids[numPyramids].pos[1] = y;
  pyramids[numPyramids].pos[2] = z;
  pyramids[numPyramids].rotation = r;
  pyramids[numPyramids].size[0] = w;
  pyramids[numPyramids].size[1] = d;
  pyramids[numPyramids].size[2] = h;
  numPyramids++;
}

void WorldInfo::addTeleporter(float x, float y, float z, float r, float w, float d, float h, float b)
{
  if (numTeleporters >= sizeTeleporters) {
    sizeTeleporters = (sizeTeleporters == 0) ? 16 : 2 * sizeTeleporters;
    teleporters = (Teleporter *)realloc(teleporters, sizeof(Teleporter) * sizeTeleporters);
  }
  teleporters[numTeleporters].pos[0] = x;
  teleporters[numTeleporters].pos[1] = y;
  teleporters[numTeleporters].pos[2] = z;
  teleporters[numTeleporters].rotation = r;
  teleporters[numTeleporters].size[0] = w;
  teleporters[numTeleporters].size[1] = d;
  teleporters[numTeleporters].size[2] = h;
  teleporters[numTeleporters].border = b;
  // default link through
  teleporters[numTeleporters].to[0] = numTeleporters * 2 + 1;
  teleporters[numTeleporters].to[1] = numTeleporters * 2;
  numTeleporters++;
}

void WorldInfo::addLink(int from, int to)
{
  // silently discard links from teleporters that don't exist
  if (from <= numTeleporters * 2 + 1) {
    teleporters[from / 2].to[from % 2] = to;
  }
}

boolean WorldInfo::rectHitCirc(float dx, float dy, const float *p, float r) const
{
  // Algorithm from Graphics Gems, pp51-53.
  const float rr = r * r, rx = -p[0], ry = -p[1];
  if (rx + dx < 0.0f) // west of rect
    if (ry + dy < 0.0f) //  sw corner
      return (rx + dx) * (rx + dx) + (ry + dy) * (ry + dy) < rr;
    else if (ry - dy > 0.0f) //  nw corner
      return (rx + dx) * (rx + dx) + (ry - dy) * (ry - dy) < rr;
    else //  due west
      return rx + dx > -r;

  else if (rx - dx > 0.0f) // east of rect
    if (ry + dy < 0.0f) //  se corner
      return (rx - dx) * (rx - dx) + (ry + dy) * (ry + dy) < rr;
    else if (ry - dy > 0.0f) //  ne corner
      return (rx - dx) * (rx - dx) + (ry - dy) * (ry - dy) < rr;
    else //  due east
      return rx - dx < r;

  else if (ry + dy < 0.0f) // due south
    return ry + dy > -r;

  else if (ry - dy > 0.0f) // due north
    return ry - dy < r;

  // circle origin in rect
  return True;
}

boolean WorldInfo::inRect(const float *p1, float angle, const float *size, float x, float y, float r) const
{
  // translate origin
  float pa[2];
  pa[0] = x - p1[0];
  pa[1] = y - p1[1];

  // rotate
  float pb[2];
  const float c = cosf(-angle), s = sinf(-angle);
  pb[0] = c * pa[0] - s * pa[1];
  pb[1] = c * pa[1] + s * pa[0];

  // do test
  return rectHitCirc(size[0], size[1], pb, r);
}

int WorldInfo::inBuilding(WorldInfo::ObstacleLocation **location, float x, float y, float z, float r) const
{
  int i;
  for (i = 0; i < numBoxes; i++)
    if ((inRect(boxes[i].pos, boxes[i].rotation, boxes[i].size, x, y, r) && boxes[i].pos[2] < 
	(z + flagHeight)) && (boxes[i].pos[2] + boxes[i].size[2]) > z) {
      if (location != NULL)
	*location = &boxes[i];
      return 1;
    }
  for (i = 0; i < numPyramids; i++) {
    float modSize[3];
    modSize[0] = pyramids[i].size[0] * ((z - pyramids[i].pos[2]) / pyramids[i].size[2]);
    modSize[1] = pyramids[i].size[1] * ((z - pyramids[i].pos[2]) / pyramids[i].size[2]);
    modSize[2] = pyramids[i].size[2];
    if ((inRect(pyramids[i].pos, pyramids[i].rotation, modSize,x,y,r)) && 
	pyramids[i].pos[2] < (z + flagHeight) && (pyramids[i].pos[2] + pyramids[i].size[2]) > z) {
      if (location != NULL)
	*location = &pyramids[i];
      return 2;
    }
  }
  for (i = 0; i < numTeleporters; i++)
    if (inRect(teleporters[i].pos, teleporters[i].rotation, teleporters[i].size, x, y, r) &&
	teleporters[i].pos[2] < (z + flagHeight) &&
	(teleporters[i].pos[2] + teleporters[i].size[2]) > z) {
      static ObstacleLocation __teleporter;
      __teleporter = teleporters[i];
      if (location != NULL)
	*location = &__teleporter;
      return 3;
    }
  if (location != NULL)
    *location = (ObstacleLocation *)NULL;
  return 0;
}

int WorldInfo::packDatabase()
{
  databaseSize = (2 + 6 * 4) * numWalls +
		(2 + 7 * 4) * numBoxes +
		(2 + 7 * 4) * numPyramids +
		(2 + 8 * 4) * numTeleporters +
		(2 + 4) * 2 * numTeleporters;
  database = new char[databaseSize];
  void *databasePtr = database;

  // define i out here so we avoid the loop variable scope debates
  int i;
  // add walls
  for (i = 0 ; i < numWalls ; i++ ) {
    databasePtr = nboPackUShort(databasePtr, WorldCodeWall);
    databasePtr = nboPackFloat(databasePtr, walls[i].pos[0]);
    databasePtr = nboPackFloat(databasePtr, walls[i].pos[1]);
    databasePtr = nboPackFloat(databasePtr, walls[i].pos[2]);
    databasePtr = nboPackFloat(databasePtr, walls[i].rotation);
    databasePtr = nboPackFloat(databasePtr, walls[i].size[0]);
    // walls have no depth
    // databasePtr = nboPackFloat(databasePtr, walls[i].size[1]);
    databasePtr = nboPackFloat(databasePtr, walls[i].size[2]);
  }

  // add boxes
  for (i = 0 ; i < numBoxes ; i++ ) {
    databasePtr = nboPackUShort(databasePtr, WorldCodeBox);
    databasePtr = nboPackFloat(databasePtr, boxes[i].pos[0]);
    databasePtr = nboPackFloat(databasePtr, boxes[i].pos[1]);
    databasePtr = nboPackFloat(databasePtr, boxes[i].pos[2]);
    databasePtr = nboPackFloat(databasePtr, boxes[i].rotation);
    databasePtr = nboPackFloat(databasePtr, boxes[i].size[0]);
    databasePtr = nboPackFloat(databasePtr, boxes[i].size[1]);
    databasePtr = nboPackFloat(databasePtr, boxes[i].size[2]);
  }

  // add pyramids
  for (i = 0 ; i < numPyramids ; i++ ) {
    databasePtr = nboPackUShort(databasePtr, WorldCodePyramid);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].pos[0]);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].pos[1]);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].pos[2]);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].rotation);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].size[0]);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].size[1]);
    databasePtr = nboPackFloat(databasePtr, pyramids[i].size[2]);
  }

  // add teleporters
  for (i = 0 ; i < numTeleporters ; i++ ) {
    databasePtr = nboPackUShort(databasePtr, WorldCodeTeleporter);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].pos[0]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].pos[1]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].pos[2]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].rotation);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].size[0]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].size[1]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].size[2]);
    databasePtr = nboPackFloat(databasePtr, teleporters[i].border);
    // and each link
    databasePtr = nboPackUShort(databasePtr, WorldCodeLink);
    databasePtr = nboPackUShort(databasePtr, uint16_t(i * 2));
    databasePtr = nboPackUShort(databasePtr, uint16_t(teleporters[i].to[0]));
    databasePtr = nboPackUShort(databasePtr, WorldCodeLink);
    databasePtr = nboPackUShort(databasePtr, uint16_t(i * 2 + 1));
    databasePtr = nboPackUShort(databasePtr, uint16_t(teleporters[i].to[1]));
  }
  return 1;
}

void *WorldInfo::getDatabase() const
{
  return database;
}

int WorldInfo::getDatabaseSize() const
{
  return databaseSize;
}

//
// rest of server (no more classes, just functions)
//

void *getPacketFromClient(int playerIndex, uint16_t *length, uint16_t *rseqno)
{
  struct PacketQueue *moving = player[playerIndex].dqueue;
  struct PacketQueue *remindme = NULL;
  while (moving != NULL) {
    if (moving->next == NULL) {
      void *remember = moving->data;
      *length = moving->length;
      if (rseqno)
	*rseqno = moving->seqno;
      if (remindme)
	remindme->next = NULL;
      else
	player[playerIndex].dqueue = NULL;
      free(moving);
      return remember;
    }
    remindme = moving;
    moving = moving->next;
  }
  *length = 0;
  return NULL;
}

void printQueueDepth(int playerIndex)
{
  int d,u;
  struct PacketQueue *moving;
  moving = player[playerIndex].dqueue;
  d = 0;
  while (moving) {
    d++;
    moving = moving->next;
  }
  u = 0;
  moving = player[playerIndex].uqueue;
  while (moving) {
    u++;
    moving = moving->next;
  }
  UMDEBUG("Player %d RECV QUEUE %d   SEND QUEUE %d\n", playerIndex, d,u);
}

boolean enqueuePacket(int playerIndex, int op, int rseqno, void *msg, int n)
{
  struct PacketQueue *oldpacket;
  struct PacketQueue *newpacket;

  if (op == SEND)
    oldpacket = player[playerIndex].uqueue;
  else {
    oldpacket = player[playerIndex].dqueue;
  }

  if (oldpacket) {
    if (oldpacket->data)
      free(oldpacket->data);
    free(oldpacket);
  }

  newpacket = (struct PacketQueue *)malloc(sizeof(struct PacketQueue));
  newpacket->seqno = rseqno;
  newpacket->data = (unsigned char *)malloc(n);
  memcpy((unsigned char *)newpacket->data, (unsigned char *)msg, n);
  newpacket->length = n;
  newpacket->next = NULL;

  if (op == SEND)
    player[playerIndex].uqueue = newpacket;
  else
    player[playerIndex].dqueue = newpacket;

  return true;
}


void disqueuePacket(int playerIndex, int op, int /* rseqno */)
{
  struct PacketQueue *oldpacket;

  if (op == SEND)
    oldpacket = player[playerIndex].uqueue;
  else {
    oldpacket = player[playerIndex].dqueue;
  }

  if (oldpacket) {
    if (oldpacket->data)
      free(oldpacket->data);
    free(oldpacket);
  }

  if (op == SEND)
    player[playerIndex].uqueue = NULL;
  else
    player[playerIndex].dqueue = NULL;
}


void *assembleSendPacket(int playerIndex, int *len)
{
  struct PacketQueue *moving = player[playerIndex].uqueue;
  unsigned char *assemblybuffer;
  int n = MaxPacketLen, packets = 0, startseq = (-1), endseq = 0, noinqueue;
  unsigned char *buf;

  assemblybuffer = (unsigned char *)malloc(n);
  buf = assemblybuffer;

  buf = (unsigned char *)nboPackUShort(buf, 0xfeed);
  buf = (unsigned char *)nboPackUShort(buf, player[playerIndex].lastRecvPacketNo);
  n -= 4;

  // lets find how deep the send queue is
  noinqueue = 0;
  while (moving) {
    noinqueue++;
    moving = moving->next;
    if (moving)
      startseq = moving->seqno;
  }

  // lets see if it is too large (CAN'T BE, Queue is always 1 length)

  if (noinqueue > 128) {
    // we have more than 128 not aknowledged packets
    printf("%d Packets outstanding\n",noinqueue);
  }

  // this is actually the number of single
  // packets we send with a single write
  // 1 means we only send the most recent
  // 2 the last two most recent, etc...
  // it is currently advisable to use 1 here as
  // a. lines are resonable stable (not much loss)
  // b. an ISDN link will be flooded with 4 > player
  noinqueue -= 1;

  moving = player[playerIndex].uqueue;

  packets = 0;
  startseq = -1;

  while (moving) {
    packets++;
    if (packets > noinqueue) {
      if (startseq < 0)
	startseq = moving->seqno;
      endseq = moving->seqno;
      n -= 2;
      if (n <= 2)
	break;
      buf = (unsigned char *)nboPackUShort(buf, moving->length);
      n -= 2;
      if (n <= 2)
	break;
      buf = (unsigned char *)nboPackUShort(buf, moving->seqno);
      n -= moving->length;
      if (n <= 2)
	break;
      memcpy((unsigned char *)buf, (unsigned char *)moving->data, moving->length);
      buf += moving->length;
    } // noinqueue
    moving = moving->next;
  }
  buf = (unsigned char *)nboPackUShort(buf, 0xffff);
  n -= 2;
  if (n <= 2) {
    UDEBUG("ASSEMBLE SEND PACKET OVERRUN BUFFER\n");
    *len = 0;
    return assemblybuffer;
  }
  if (n < 4096)
    UMDEBUG("Warning: TOO Long a PACKET: %d %d\n",noinqueue, packets);
  *len = (MaxPacketLen - n);
  UDEBUG("ASSEMBLY %d packets, %d - %d\n",packets,startseq, endseq);
  return assemblybuffer;
}

void disassemblePacket(int playerIndex, void *msg, int *nopackets)
{
  unsigned short marker;
  unsigned short usdata;
  unsigned char *buf = (unsigned char *)msg;

  int npackets = 0;

  UDEBUG("::: Disassemble Packet\n");

  buf = (unsigned char *)nboUnpackUShort(buf, marker);
  if (marker!= 0xfeed) {
    UDEBUG("Reject UPacket because invalid header %04x\n", marker);
    return;
  }
  buf = (unsigned char *)nboUnpackUShort(buf, usdata);

  disqueuePacket(playerIndex, SEND, usdata);

  while (true) {
    unsigned short seqno;
    unsigned short length;
    int ilength;

    buf = (unsigned char *)nboUnpackUShort(buf, length);
    ilength = length;
    if (length == 0xffff)
      break;
    else
      if (ilength > 1024) {
	fprintf(stderr,"* RECEIVE PACKET BUFFER OVERFLOW ATTEMPT: %d sent %d Bytes\n",
	    playerIndex, ilength);
	break;
      }
    buf = (unsigned char *)nboUnpackUShort(buf, seqno);
    UDEBUG("SEQ RECV %d Enqueing now...\n",seqno);
    enqueuePacket(playerIndex, RECEIVE, seqno, buf, length);
    buf+= length;
    npackets++;
  }
  UMDEBUG("%d: Got %d packets\n",(int)time(0),npackets);
  // printQueueDepth(playerIndex);
  *nopackets = npackets;
}


const void *assembleUDPPacket(int playerIndex, const void *b, int *l)
{
  int length = *l;

  UDEBUG("ENQUEUE %d [%d]\n",length, player[playerIndex].lastSendPacketNo);
  enqueuePacket(playerIndex, SEND, player[playerIndex].lastSendPacketNo, (void *)b, length);

  player[playerIndex].lastSendPacketNo++;

  UDEBUG("ASSEMBLE\n");
  return assembleSendPacket(playerIndex, l);
}

// write an UDP packet down the link to the client, we don't know if it comes through
// so this code is using a queuing mechanism. As it turns out the queue is not strictly
// needed if we only use the Multicast messages...

static int puwrite(int playerIndex, const void *b, int l)
{
  PlayerInfo& p = player[playerIndex];
  const void *tobesend = b;

  //UDEBUG("INTO PUWRITE\n");

  tobesend = assembleUDPPacket(playerIndex,b,&l);

  if (!tobesend || (l == 0)) {
    removePlayer(playerIndex);
    fprintf(stderr, "Send Queue Overrun for player %d (%s)\n", playerIndex, p.callSign);
    if (tobesend)
      free((unsigned char *)tobesend);
    return -1;
  }

  UDEBUG("PUWRITE - ASSEMBLED UDP LEN %d for Player %d\n",l,playerIndex);
  // write as much data from buffer as we can in one send()

  int n;

#ifdef TESTLINK
  if ((random()%LINKQUALITY) != 0) {
#endif
    n = sendto(udpSocket, (const char *)tobesend, l, 0, (struct sockaddr*)&p.uaddr, sizeof(p.uaddr));
#ifdef TESTLINK
  } else
    printf("Drop Packet due to Test\n");
#endif
  if (tobesend)
    free((unsigned char *)tobesend);

  // handle errors
  if (n < 0) {
    // get error code
    const int err = getErrno();

    // just try again later if it's one of these errors
    if (err == EAGAIN || err == EINTR)
      return -1;

    // if socket is closed then give up
    if (err == ECONNRESET || err == EPIPE) {
      UMDEBUG("REMOVE: ECONNRESET/EPIPE\n");
      removePlayer(playerIndex);
      return -1;
    }

    // dump other errors and continue
    nerror("error on UDP write");
    fprintf(stderr, "player is %d (%s)\n", playerIndex, p.callSign);
    fprintf(stderr, "%d bytes\n", n);

    // we may actually run into not enough buffer space here
    // but as the link is unreliable anyway we never treat it as
    // hard error
  }

  return n;
}

static int prealwrite(int playerIndex, const void *b, int l)
{
  PlayerInfo& p = player[playerIndex];
  assert(p.fd != NotConnected && l > 0);

  // write as much data from buffer as we can in one send()
  const int n = send(p.fd, (const char *)b, l, 0);

  // handle errors
  if (n < 0) {
    // get error code
    const int err = getErrno();

    // just try again later if it's one of these errors
    if (err == EAGAIN || err == EINTR)
      return -1;

    // if socket is closed then give up
    if (err == ECONNRESET || err == EPIPE) {
      UMDEBUG("REMOVE: Reset socket (4)\n");
      removePlayer(playerIndex);
      return -1;
    }

    // dump other errors and remove the player
    nerror("error on write");
    fprintf(stderr, "player is %d (%s)\n", playerIndex, p.callSign);
    UMDEBUG("REMOVE: WRITE ERROR\n");
    removePlayer(playerIndex);
    return -1;
  }

  return n;
}

// try to write stuff from the output buffer
static void pflush(int playerIndex)
{
  PlayerInfo& p = player[playerIndex];
  if (p.fd == NotConnected || p.outmsgSize == 0)
    return;

  const int n = prealwrite(playerIndex, p.outmsg + p.outmsgOffset, p.outmsgSize);
  if (n > 0) {
    p.outmsgOffset += n;
    p.outmsgSize   -= n;
  }
}

// a hack to handle old clients
// old clients use <serverip><serverport><number> as id
// where serverip etc is as seen from the client
// new clients use <clientip><clientport><number> as id
// where clientip etc is as seen from the server
// here we patch up one id, from fromId to toId
static void patchPlayerId(PlayerId fromId, PlayerId toId, const void *msg, int offset)
{
  PlayerId id;

  id.unpack((char *)msg + offset);
  if (id == fromId) {
#ifdef DEBUG_PLAYERID
    fprintf(stderr, "patchPlayerId %c%c(%02x%02x)%08x:%u(%04x):%u(%04x)->",
	*((unsigned char *)msg + 2), *((unsigned char *)msg + 3),
	*((unsigned char *)msg + 2), *((unsigned char *)msg + 3),
	ntohl(*(int *)((char *)msg + offset)),
	ntohs(*((short *)((char *)msg + offset + 4))),
	ntohs(*((short *)((char *)msg + offset + 4))),
	ntohs(*((short *)((char *)msg + offset + 6))),
	ntohs(*((short *)((char *)msg + offset + 6))));
#endif
    toId.pack((char *)msg + offset);
#ifdef DEBUG_PLAYERID
    fprintf(stderr, "%08x:%u(%04x):%u(%04x)\n",
	ntohl(*(int *)((char *)msg + offset)),
	ntohs(*((short *)((char *)msg + offset + 4))),
	ntohs(*((short *)((char *)msg + offset + 4))),
	ntohs(*((short *)((char *)msg + offset + 6))),
	ntohs(*((short *)((char *)msg + offset + 6))));
#endif
  }
}

// Tim Riker <Tim@Rikers.org> is responsible for this
// butt ugly hack. All this to allow support for old clients
// patch all incoming or outgoing packets to old clients
// so they still see what they want to see.
static void patchMessage(PlayerId fromId, PlayerId toId, const void *msg)
{
  uint16_t len;
  uint16_t code;

  nboUnpackUShort((unsigned char *)msg, len);
  nboUnpackUShort((unsigned char *)msg + 2, code);
  switch (code) {
    case MsgAddPlayer:
    case MsgCaptureFlag:
    case MsgEnter:
    case MsgPlayerUpdate:
    case MsgRemovePlayer:
    case MsgScoreOver:
    case MsgShotBegin:
    case MsgShotEnd:
    case MsgTeleport:
      patchPlayerId(fromId, toId, msg, 4);
      break;
      ;;
    case MsgAlive:
      if (len == 32)
	patchPlayerId(fromId, toId, msg, 4);
      ;;
    case MsgDropFlag:
    case MsgGrabFlag:
      if (len > 28)
	// server version
	patchPlayerId(fromId, toId, msg, 4);
	patchPlayerId(fromId, toId, msg, 20);
      break;
      ;;
    case MsgFlagUpdate:
      patchPlayerId(fromId, toId, msg, 12);
      break;
      ;;
    case MsgGMUpdate:
      patchPlayerId(fromId, toId, msg, 4);
      patchPlayerId(fromId, toId, msg, 42);
      break;
      ;;
    case MsgKilled:
    case MsgMessage:
      patchPlayerId(fromId, toId, msg, 4);
      if (len > 16)
	// server version
	patchPlayerId(fromId, toId, msg, 12);
      break;
      ;;
    case MsgScore:
      if (len == 12)
	// server version
	patchPlayerId(fromId, toId, msg, 4);
      ;;
    case MsgAccept:
    case MsgClientVersion:
    case MsgExit:
    case MsgGetWorld:
    case MsgNetworkRelay:
    case MsgReject:
    case MsgSetTTL:
    case MsgSuperKill:
    case MsgTeamUpdate:
    case MsgUDPLinkEstablished:
    case MsgUDPLinkRequest:
      // No changes required
      break;
      ;;
    default:
#ifdef DEBUG_PLAYERID
      fprintf(stderr, "unhandled msg type: %c%c(%04x)\n", code >> 8, code, code);
#endif
      break;
      ;;
  }
}

static void pwrite(int playerIndex, const void *b, int l)
{
  PlayerInfo& p = player[playerIndex];
  if (p.fd == NotConnected || l == 0)
    return;

  patchMessage(player[playerIndex].id, player[playerIndex].oldId, b);
  // Check if UDP Link is used instead of TCP, if so jump into puwrite
  if (p.ulinkup) {
    uint16_t len, code;
    void *buf = (void *)b;
    buf = nboUnpackUShort(buf, len);
    buf = nboUnpackUShort(buf, code);

    // only send bulk messages by UDP
    switch (code) {
	case MsgShotBegin:
	case MsgShotEnd:
	case MsgPlayerUpdate:
	case MsgGMUpdate:
	  puwrite(playerIndex,b,l);
	  return;
    }
  }

  // try flushing buffered data
  pflush(playerIndex);

  //UDEBUG("TCP write\n");
  // if the buffer is empty try writing the data immediately
  if (p.fd != NotConnected && p.outmsgSize == 0) {
    const int n = prealwrite(playerIndex, b, l);
    if (n > 0) {
      b  = (const void*)(((const char*)b) + n);
      l -= n;
    }
  }

  // write leftover data to the buffer
  if (p.fd != NotConnected && l > 0) {
    // is there enough room in buffer?
    if (p.outmsgCapacity < p.outmsgSize + l) {
      // double capacity until it's big enough
      int newCapacity = (p.outmsgCapacity == 0) ? 512 : p.outmsgCapacity;
      while (newCapacity < p.outmsgSize + l)
	newCapacity <<= 1;

      // if the buffer is getting too big then drop the player.  chances
      // are the network is down or too unreliable to that player.
      // FIXME -- is 20kB to big?  to small?
      if (newCapacity >= 20 * 1024) {
	fprintf(stderr, "dropping unresponsive player %d (%s) with %d bytes queued\n",
	    playerIndex, p.callSign, p.outmsgSize + l);
	UMDEBUG("REMOVE: CAPACITY\n");
	removePlayer(playerIndex);
	return;
      }

      // allocate memory
      char *newbuf = new char[newCapacity];

      // copy old data over
      memmove(newbuf, p.outmsg + p.outmsgOffset, p.outmsgSize);

      // cutover
      delete[] p.outmsg;
      p.outmsg         = newbuf;
      p.outmsgOffset   = 0;
      p.outmsgCapacity = newCapacity;
    }

    // if we can't fit new data at the end of the buffer then move existing
    // data to head of buffer
    // FIXME -- use a ring buffer to avoid moving memory
    if (p.outmsgOffset + p.outmsgSize + l > p.outmsgCapacity) {
      memmove(p.outmsg, p.outmsg + p.outmsgOffset, p.outmsgSize);
      p.outmsgOffset = 0;
    }

    // append data
    memmove(p.outmsg + p.outmsgOffset + p.outmsgSize, b, l);
    p.outmsgSize += l;
  }
}

static void directMessage(int playerIndex, uint16_t code, int len, const void *msg)
{
  if (player[playerIndex].fd == NotConnected)
    return;

  // send message to one player
  char msgbuf[MaxPacketLen];
  void *buf = msgbuf;
  buf = nboPackUShort(buf, uint16_t(len));
  buf = nboPackUShort(buf, code);
  buf = nboPackString(buf, msg, len);
  pwrite(playerIndex, msgbuf, len + 4);
}

static void broadcastMessage(uint16_t code, int len, const void *msg)
{
  // send message to everyone
  for (int i = 0; i < maxPlayers; i++)
    if (player[i].state != PlayerInLimbo)
      directMessage(i, code, len, msg);
}

static void sendUDPupdate(int playerIndex)
{
  char  buffer[2];
  void *buf = (void*)buffer;
  buf = nboPackUShort(buf, wksPort);
  UMDEBUG("LOCAL Update to %d port %d\n",playerIndex,wksPort);
  // send it
  directMessage(playerIndex, MsgUDPLinkRequest, sizeof(buffer), buffer);
}

//static void sendUDPseqno(int playerIndex)
//{
//  char  buffer[2];
//  unsigned short seqno = player[playerIndex].lastRecvPacketNo;
//  void *buf = (void*)buffer;
//  buf = nboPackUShort(buf, seqno);
//  // send it
//  directMessage(playerIndex, MsgUDPLinkUpdate, sizeof(buffer), buffer);
//}

static void createUDPcon(int t, int remote_port) {
  UMDEBUG("Message received: UDP request for remote port %d\n",remote_port);

  if (remote_port == 0)
    return;

  struct sockaddr_in addr;
  // now build the send structure for sendto()
  memset((char *)&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  player[t].peer.pack(&addr.sin_addr.s_addr);
  addr.sin_port = htons(remote_port);
  memcpy((char *)&player[t].uaddr,(char *)&addr, sizeof(addr));

  // show some message on the console
  UMDEBUG("UDP link created, remote %d %04x, local %d\n",
      remote_port,ntohl(addr.sin_addr.s_addr), wksPort);

  // init the queues
  player[t].uqueue = player[t].dqueue = NULL;
  player[t].lastRecvPacketNo = player[t].lastSendPacketNo = 0;

  // send client the message that we are ready for him
  sendUDPupdate(t);

  return;
}

// obsolete now
void OOBQueueUpdate(int t, uint32_t rseqno) {
  if (rseqno > 0)
    disqueuePacket(t, SEND, rseqno);
}

static int lookupPlayer(const PlayerId& id)
{
  for (int i = 0; i < maxPlayers; i++)
    if (player[i].fd != NotConnected && player[i].id == id)
      return i;
  return InvalidPlayer;
}

static void setNoDelay(int fd)
{
  // turn off TCP delay (collection).  we want packets sent immediately.
#if defined(_WIN32)
  BOOL on = TRUE;
#else
  int on = 1;
#endif
  struct protoent *p = getprotobyname("tcp");
  if (p && setsockopt(fd, p->p_proto, TCP_NODELAY, (SSOType)&on, sizeof(on)) < 0) {
    nerror("enabling TCP_NODELAY");
  }
}

// uread - interface to the UDP Receive routines
static int uread(int *playerIndex, int *nopackets)
{
  int n = 0;
  struct sockaddr_in uaddr;
  unsigned char ubuf[MaxPacketLen];
  AddrLen recvlen = sizeof(uaddr);
  //UDEBUG("Into UREAD\n");

  *nopackets = 0;

  if ((n = recvfrom(udpSocket, (char *)ubuf, MaxPacketLen, MSG_PEEK, (struct sockaddr*)&uaddr, &recvlen)) != -1) {
    uint16_t len, lseqno;
    void *pmsg;
    for (*playerIndex = 0; *playerIndex < MaxPlayers; (*playerIndex)++) {
      if ((player[*playerIndex].ulinkup) &&
	  (player[*playerIndex].uaddr.sin_port == uaddr.sin_port) &&
	  (memcmp(&player[*playerIndex].uaddr.sin_addr, &uaddr.sin_addr, sizeof(uaddr.sin_addr)) == 0)) {
	break;
      }
    }
    if (*playerIndex == MaxPlayers) {
      // didn't find player so test for exact match new player
      for (*playerIndex = 0; *playerIndex < MaxPlayers; (*playerIndex)++) {
	if (!player[*playerIndex].ulinkup &&
	    (player[*playerIndex].uaddr.sin_port == uaddr.sin_port) &&
	    (memcmp(&player[*playerIndex].uaddr.sin_addr, &uaddr.sin_addr, sizeof(uaddr.sin_addr)) == 0)) {
	  UMDEBUG("uread() exact udp up for player %d %s:%d\n",
	      *playerIndex, inet_ntoa(player[*playerIndex].uaddr.sin_addr),
	      ntohs(player[*playerIndex].uaddr.sin_port));
	  player[*playerIndex].ulinkup = true;
	  break;
	}
      }
    }
    if (*playerIndex == MaxPlayers) {
      // still didn't find player so test for just address not port (ipmasq fw etc.)
      for (*playerIndex = 0; *playerIndex < MaxPlayers; (*playerIndex)++) {
	if (!player[*playerIndex].ulinkup &&
	    memcmp(&uaddr.sin_addr, &player[*playerIndex].uaddr.sin_addr, sizeof(uaddr.sin_addr)) == 0) {
	  UMDEBUG("uread() fuzzy udp up for player %d %s:%d actual port %d\n",
	      *playerIndex, inet_ntoa(player[*playerIndex].uaddr.sin_addr),
	      ntohs(player[*playerIndex].uaddr.sin_port), ntohs(uaddr.sin_port));
	  player[*playerIndex].uaddr.sin_port = uaddr.sin_port;
	  player[*playerIndex].ulinkup = true;
	  break;
	}
      }
    }

    // get the packet
    n = recv(udpSocket, (char *)ubuf, MaxPacketLen, 0);
    if (*playerIndex == MaxPlayers) {
      // no match, discard packet
      UMDEBUG("uread() discard packet! %s:%d choices p(l) h:p", inet_ntoa(uaddr.sin_addr), ntohs(uaddr.sin_port));
      for (*playerIndex = 0; *playerIndex < MaxPlayers; (*playerIndex)++) {
	if (player[*playerIndex].fd != -1) {
	  UMDEBUG(" %d(%d) %s:%d",
	      *playerIndex, player[*playerIndex].ulinkup,
	      inet_ntoa(player[*playerIndex].uaddr.sin_addr),
	      ntohs(player[*playerIndex].uaddr.sin_port));
	}
      }
      UMDEBUG("\n");
      *playerIndex = 0;
      return 0;
    }

    UMDEBUG("uread() player %d %s:%d len %d from %s:%d on %i\n",
	*playerIndex, inet_ntoa(player[*playerIndex].uaddr.sin_addr),
	ntohs(player[*playerIndex].uaddr.sin_port), n, inet_ntoa(uaddr.sin_addr),
	ntohs(uaddr.sin_port), udpSocket);

    if (n > 0) {
      // got something! now disassemble the package block into single BZPackets
      // filling up the dqueue with these packets
      disassemblePacket(*playerIndex, ubuf, nopackets);

      // old code is obsolete
      // if (*nopackets > 6 )
      //   pucdwrite(playerIndex);
    }
    // have something in the receive buffer? so get it
    // due to the organization sequence and reliability is always granted
    // even if some packets are lost during transfer
    pmsg =  getPacketFromClient(*playerIndex, &len, &lseqno);
    if (pmsg != NULL) {
      int clen = len;
      if (clen < 1024) {
	memcpy(player[*playerIndex].udpmsg,pmsg,clen);
	player[*playerIndex].udplen = clen;
      }
      // be sure to free the packet again
      free(pmsg);
      UDEBUG("GOT UDP READ %d Bytes [%d]\n",len, lseqno);
      return player[*playerIndex].udplen;
    }
  }
  return 0;
}

static int pread(int playerIndex, int l)
{
  PlayerInfo& p = player[playerIndex];
  //fprintf(stderr,"pread,playerIndex,l %i %i\n",playerIndex,l);
  if (p.fd == NotConnected || l == 0)
    return 0;

  // read more data into player's message buffer
  const int e = recv(p.fd, p.tcpmsg + p.tcplen, l, 0);

  // accumulate bytes read
  if (e > 0) {
    p.tcplen += e;
  }

  // handle errors
  else if (e < 0) {
    // get error code
    const int err = getErrno();

    // ignore if it's one of these errors
    if (err == EAGAIN || err == EINTR)
      return 0;

    // if socket is closed then give up
    if (err == ECONNRESET || err == EPIPE) {
      UMDEBUG("REMOVE: Socket reset (2)\n");
      removePlayer(playerIndex);
      return -1;
    }

    // dump other errors and remove the player
    nerror("error on read");
    fprintf(stderr, "player is %d (%s)\n", playerIndex, p.callSign);
    UMDEBUG("REMOVE: READ ERROR\n");
    removePlayer(playerIndex);
    return -1;
  }

  else {
    // disconnected
    UMDEBUG("REMOVE: Disconnected (3)\n");
    removePlayer(playerIndex);
    return -1;
  }

  return e;
}

static void sendFlagUpdate(int flagIndex, int index = -1)
{
  char msg[2 + FlagPLen];
  void *buf = msg;
  buf = nboPackUShort(buf, flagIndex);
  buf = flag[flagIndex].flag.pack(buf);
  if (index == -1)
    broadcastMessage(MsgFlagUpdate, sizeof(msg), msg);
  else
    directMessage(index, MsgFlagUpdate, sizeof(msg), msg);
}

static void sendTeamUpdate(int teamIndex, int index = -1)
{
  char msg[TeamPLen];
  void *buf = msg;
  buf = nboPackUShort(buf, teamIndex);
  buf = team[teamIndex].team.pack(buf);
  if (index == -1)
    broadcastMessage(MsgTeamUpdate, sizeof(msg), msg);
  else
    directMessage(index, MsgTeamUpdate, sizeof(msg), msg);
}

static void sendPlayerUpdate(int playerIndex, int index = -1)
{
  char msg[PlayerIdPLen + 8 + CallSignLen + EmailLen];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, uint16_t(player[playerIndex].type));
  buf = nboPackUShort(buf, uint16_t(player[playerIndex].team));
  buf = nboPackUShort(buf, uint16_t(player[playerIndex].wins));
  buf = nboPackUShort(buf, uint16_t(player[playerIndex].losses));
  buf = nboPackString(buf, player[playerIndex].callSign, CallSignLen);
  buf = nboPackString(buf, player[playerIndex].email, EmailLen);
  if (index == -1)
    broadcastMessage(MsgAddPlayer, sizeof(msg), msg);
  else
    directMessage(index, MsgAddPlayer, sizeof(msg), msg);
}

static void closeListServer(int index)
{
  assert(index >= 0 && index < MaxListServers);
  if (index >= listServerLinksCount)
    return;

  ListServerLink& link = listServerLinks[index];
  if (link.socket != NotConnected) {
    shutdown(link.socket, 2);
    close(link.socket);
    UMDEBUG("Closing List server %d\n",index);
    link.socket = NotConnected;
    link.nextMessage = "";
  }
}

static void closeListServers()
{
  for (int i = 0; i < listServerLinksCount; ++i)
    closeListServer(i);
}

static void openListServer(int index)
{
  assert(index >= 0 && index < MaxListServers);
  if (index >= listServerLinksCount)
    return;

  ListServerLink& link = listServerLinks[index];
  link.nextMessage = "";

  // start opening connection if not already doing so
  if (link.socket == NotConnected) {
    link.socket = socket(AF_INET, SOCK_STREAM, 0);
    UMDEBUG("Opening List Server %d\n",index);
    if (link.socket == NotConnected) {
      closeListServer(index);
      return;
    }

    // set to non-blocking for connect
    if (BzfNetwork::setNonBlocking(link.socket) < 0) {
      closeListServer(index);
      return;
    }

    // connect.  this should fail with EINPROGRESS but check for
    // success just in case.
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(link.port);
    addr.sin_addr   = link.address;
    if (connect(link.socket, (CNCTType*)&addr, sizeof(addr)) < 0) {
#if defined(_WIN32)
#undef EINPROGRESS
#define EINPROGRESS EWOULDBLOCK
#endif
      if (getErrno() != EINPROGRESS) {
	nerror("connecting to list server");
	closeListServer(index);
      }
      else {
	if (maxFileDescriptor < link.socket)
	  maxFileDescriptor = link.socket;
      }
    }
  }
}

static void sendMessageToListServer(const char *msg)
{
  // ignore if not publicizing
  if (!publicizeServer)
    return;

  // start opening connections if not already doing so
  for (int i = 0; i < listServerLinksCount; i++) {
    openListServer(i);

    // record next message to send.  note that each message overrides
    // any other message, except SETNUM doesn't override ADD (cos ADD
    // sends SETNUM data anyway).
    ListServerLink& link = listServerLinks[i];
    if (strcmp(msg, "SETNUM") != 0 || strcmp(link.nextMessage, "ADD") != 0)
      link.nextMessage = msg;
  }
}

static void sendMessageToListServerForReal(int index)
{
  assert(index >= 0 && index < MaxListServers);
  if (index >= listServerLinksCount)
    return;

  // ignore if link not connected
  ListServerLink& link = listServerLinks[index];
  if (link.socket == NotConnected)
    return;

  char msg[1024];
  if (strcmp(link.nextMessage, "ADD") == 0) {
    // update player counts in ping reply.  pretend there are no players
    // if the game is over.
    if (gameOver) {
      pingReply.rogueCount = team[0].team.activeSize;
      pingReply.redCount = team[1].team.activeSize;
      pingReply.greenCount = team[2].team.activeSize;
      pingReply.blueCount = team[3].team.activeSize;
      pingReply.purpleCount = team[4].team.activeSize;
    }
    else {
      pingReply.rogueCount = 0;
      pingReply.redCount = 0;
      pingReply.greenCount = 0;
      pingReply.blueCount = 0;
      pingReply.purpleCount = 0;
    }

    // encode ping reply as ascii hex digits
    char gameInfo[PingPacketHexPackedSize];
    pingReply.packHex(gameInfo);

    // send ADD message
    sprintf(msg, "%s %s %d %s %.*s %.256s\n\n", link.nextMessage,
	(const char*)publicizedAddress,
	VERSION % 1000,
	ServerVersion,
	PingPacketHexPackedSize, gameInfo,
	publicizedTitle);
  }
  else if (strcmp(link.nextMessage, "REMOVE") == 0) {
    // send REMOVE
    sprintf(msg, "%s %s\n\n", link.nextMessage,
	(const char*)publicizedAddress);
  }
  else if (strcmp(link.nextMessage, "SETNUM") == 0) {
    // pretend there are no players if the game is over
    if (gameOver)
      sprintf(msg, "%s %s 0 0 0 0 0\n\n", link.nextMessage, (const char*)publicizedAddress);
    else
      sprintf(msg, "%s %s %d %d %d %d %d\n\n", link.nextMessage,
	  (const char*)publicizedAddress,
	  team[0].team.activeSize,
	  team[1].team.activeSize,
	  team[2].team.activeSize,
	  team[3].team.activeSize,
	  team[4].team.activeSize);
  }
  if (debug >= 3) {
      cerr << msg;
  }
  send(link.socket, msg, strlen(msg), 0);

  // hangup (we don't care about replies)
  closeListServer(index);
}

static void publicize()
{
  // hangup any previous list server sockets
  closeListServers();

  // list server initialization
  listServerLinksCount  = 0;

  // parse the list server URL if we're publicizing ourself
  if (publicizeServer && publicizedTitle) {
    // dereference URL, including following redirections.  get no
    // more than MaxListServers urls.
    BzfStringAList urls, failedURLs;
    urls.append(listServerURL);
    BzfNetwork::dereferenceURLs(urls, MaxListServers, failedURLs);

    for (int j = 0; j < failedURLs.getLength(); ++j)
      fprintf(stderr, "failed: %s\n", (const char*)failedURLs[j]);

    // check url list for validity
    for (int i = 0; i < urls.getLength(); ++i) {
      // parse url
      BzfString protocol, hostname, pathname;
      int port = ServerPort + 1;
      if (!BzfNetwork::parseURL(urls[i], protocol, hostname, port, pathname))
	continue;

      // ignore if not right protocol
      if (protocol != "bzflist")
	continue;

      // ignore if port is bogus
      if (port < 1 || port > 65535)
	continue;

      // ignore if bad address
      Address address = Address::getHostAddress(hostname);
      if (address.isAny())
	continue;

      // add to list
      listServerLinks[listServerLinksCount].address = address;
      listServerLinks[listServerLinksCount].port = port;
      listServerLinks[listServerLinksCount].socket  = NotConnected;
      listServerLinksCount++;
    }

    // schedule message for list server
    sendMessageToListServer("ADD");
  }
}

static boolean serverStart()
{
#if defined(_WIN32)
  const BOOL optOn = TRUE;
  BOOL opt = optOn;
#else
  const int optOn = 1;
  int opt = optOn;
#endif
  maxFileDescriptor = 0;

  // init addr:port structure
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = serverAddress;

  // look up service name and use that port if no port given on
  // command line.  if no service then use default port.
  if (!useGivenPort) {
    struct servent *service = getservbyname("bzfs", "tcp");
    if (service) {
      wksPort = ntohs(service->s_port);
    }
  }
  pingReply.serverId.port = addr.sin_port = htons(wksPort);

  // open well known service port
  wksSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (wksSocket == -1) {
    nerror("couldn't make connect socket");
    return False;
  }
#ifdef SO_REUSEADDR
  /* set reuse address */
  opt = optOn;
  if (setsockopt(wksSocket, SOL_SOCKET, SO_REUSEADDR, (SSOType)&opt, sizeof(opt)) < 0) {
    nerror("serverStart: setsockopt SO_REUSEADDR");
    close(wksSocket);
    return False;
  }
#endif
  if (bind(wksSocket, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
    if (!useFallbackPort) {
      nerror("couldn't bind connect socket");
      close(wksSocket);
      return False;
    }

    // if we get here then try binding to any old port the system gives us
    addr.sin_port = htons(0);
    if (bind(wksSocket, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
      nerror("couldn't bind connect socket");
      close(wksSocket);
      return False;
    }

    // fixup ping reply
    AddrLen addrLen = sizeof(addr);
    if (getsockname(wksSocket, (struct sockaddr*)&addr, &addrLen) >= 0)
      pingReply.serverId.port = addr.sin_port;

    // fixup publicized name will want it here later
    wksPort = ntohs(addr.sin_port);
  }

  if (listen(wksSocket, 5) == -1) {
    nerror("couldn't make connect socket queue");
    close(wksSocket);
    return False;
  }
  maxFileDescriptor = wksSocket;

  if (alsoUDP) {
    int n;
    // we open a udp socket on the same port if alsoUDP
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      nerror("couldn't make udp connect socket");
      return False;
    }

    // increase send/rcv buffer size
#if defined(_WIN32)
    n = setsockopt(udpSocket,SOL_SOCKET,SO_SNDBUF,(const char *)&udpBufSize,sizeof(int));
#else
    n = setsockopt(udpSocket,SOL_SOCKET,SO_SNDBUF,(const void *)&udpBufSize,sizeof(int));
#endif
    if (n < 0) {
      nerror("couldn't increase udp send buffer size");
      close(wksSocket);
      close(udpSocket);
      return False;
    }

#if defined(_WIN32)
    n = setsockopt(udpSocket,SOL_SOCKET,SO_RCVBUF,(const char *)&udpBufSize,sizeof(int));
#else
    n = setsockopt(udpSocket,SOL_SOCKET,SO_RCVBUF,(const void *)&udpBufSize,sizeof(int));
#endif
    if (n < 0) {
      nerror("couldn't increase udp receive buffer size");
      close(wksSocket);
      close(udpSocket);
      return False;
    }
    if (bind(udpSocket, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
      nerror("couldn't bind udp listen port");
      close(wksSocket);
      close(udpSocket);
      return False;
    }
    // don't buffer info, send it immediately
    BzfNetwork::setNonBlocking(udpSocket);

    maxFileDescriptor = udpSocket;
  }

  // open sockets to receive and reply to pings
  Address multicastAddress(BroadcastAddress);
  pingInSocket = openMulticast(multicastAddress, ServerPort, NULL,
      pingTTL, pingInterface, "r", &pingInAddr);
  pingOutSocket = openMulticast(multicastAddress, ServerPort, NULL,
      pingTTL, pingInterface, "w", &pingOutAddr);
  pingBcastSocket = openBroadcast(BroadcastPort, NULL, &pingBcastAddr);
  if (pingInSocket == -1 || pingOutSocket == -1) {
    closeMulticast(pingInSocket);
    closeMulticast(pingOutSocket);
    pingInSocket = -1;
    pingOutSocket = -1;
  }
  else {
    maxFileDescriptor = pingOutSocket;
  }
  if (pingBcastSocket != -1) {
    if (pingBcastSocket > maxFileDescriptor)
      maxFileDescriptor = pingBcastSocket;
  }

  // initialize player packet relaying to be off
  relayInSocket = -1;
  relayOutSocket = -1;

  for (int i = 0; i < MaxPlayers; i++) {	// no connections
    player[i].fd = NotConnected;
    player[i].state = PlayerInLimbo;
    player[i].outmsg = NULL;
    player[i].outmsgSize = 0;
    player[i].outmsgOffset = 0;
    player[i].outmsgCapacity = 0;
    reconnect[i].accept = NotConnected;
    reconnect[i].listen = NotConnected;
  }

  listServerLinksCount = 0;
  publicize();
  return True;
}

static void serverStop()
{
  // shut down server
  // first ignore further attempts to kill me
  bzSignal(SIGINT, SIG_IGN);
  bzSignal(SIGTERM, SIG_IGN);

  // reject attempts to talk to server
  shutdown(wksSocket, 2);
  close(wksSocket);
  closeMulticast(pingBcastSocket);
  closeMulticast(pingInSocket);
  closeMulticast(pingOutSocket);
  stopPlayerPacketRelay();

  // tell players to quit
  int i;
  for (i = 0; i < MaxPlayers; i++)
    directMessage(i, MsgSuperKill, 0, NULL);

  // close connections
  for (i = 0; i < MaxPlayers; i++)
    if (player[i].fd != NotConnected) {
      shutdown(player[i].fd, 2);
      close(player[i].fd);
      delete[] player[i].outmsg;
    }

  // now tell the list servers that we're going away.  this can
  // take some time but we don't want to wait too long.  we do
  // our own multiplexing loop and wait for a maximum of 3 seconds
  // total.
  sendMessageToListServer("REMOVE");
  TimeKeeper start = TimeKeeper::getCurrent();
  do {
    // compute timeout
    float waitTime = 3.0f - (TimeKeeper::getCurrent() - start);
    if (waitTime <= 0.0f)
      break;

    // check for list server socket connection
    int fdMax = -1;
    fd_set write_set;
    FD_ZERO(&write_set);
    for (i = 0; i < listServerLinksCount; i++)
      if (listServerLinks[i].socket != NotConnected) {
	FD_SET(listServerLinks[i].socket, &write_set);
	fdMax = listServerLinks[i].socket;
      }
    if (fdMax == -1)
      break;

    // wait for socket to connect or timeout
    struct timeval timeout;
    timeout.tv_sec = long(floorf(waitTime));
    timeout.tv_usec = long(1.0e+6f * (waitTime - floorf(waitTime)));
    int nfound = select(fdMax + 1, NULL, (fd_set*)&write_set, 0, &timeout);
    // check for connection to list server
    if (nfound > 0)
      for (i = 0; i < listServerLinksCount; ++i)
	if (listServerLinks[i].socket != NotConnected &&
	    FD_ISSET(listServerLinks[i].socket, &write_set))
	  sendMessageToListServerForReal(i);
  } while (True);

  // stop list server communication
  closeListServers();
}

static boolean startPlayerPacketRelay(int playerIndex)
{
  // return true if already started
  if (noMulticastRelay || (relayInSocket != -1 && relayOutSocket != -1))
    return True;

  Address multicastAddress(BroadcastAddress);
  if (relayInSocket == -1)
    relayInSocket = openMulticast(multicastAddress, BroadcastPort, NULL,
				pingTTL, pingInterface, "r", &relayInAddr);
  if (relayOutSocket == -1)
    relayOutSocket = openMulticast(multicastAddress, BroadcastPort, NULL,
				pingTTL, pingInterface, "w", &relayOutAddr);
  if (relayInSocket == -1 || relayOutSocket == -1) {
    stopPlayerPacketRelay();

    // can't multicast.  can't just reject the player requesting
    // relaying because then it would be impossible for a server
    // that can't multicast to serve a game unless all players
    // could multicast.  since many platforms don't support
    // multicasting yet, we'll have to do it the hard way -- when
    // we can't multicast and a player wants relaying we must
    // force all players to start relaying.
    for (int i = 0; i < maxPlayers; i++)
      if (i != playerIndex &&
	  player[i].state != PlayerInLimbo && !player[i].multicastRelay) {
	directMessage(i, MsgNetworkRelay, 0, NULL);
	player[i].multicastRelay = True;
      }
    noMulticastRelay = True;

    return True;
  }
  if (maxFileDescriptor < relayOutSocket)
    maxFileDescriptor = relayOutSocket;
  return True;
}

static void stopPlayerPacketRelay()
{
  closeMulticast(relayInSocket);
  closeMulticast(relayOutSocket);
  relayInSocket = -1;
  relayOutSocket = -1;
  noMulticastRelay = False;
}

static void relayPlayerPacket()
{
  // XXX -- accumulate data until we've received all data in message
  // get packet from multicast port and multiplex to player's needing a relay.
  // first get packet header
  char buffer[MaxPacketLen];
  const int msglen = recvMulticast(relayInSocket, buffer, MaxPacketLen, NULL);
  if (msglen < 4) {
    fprintf(stderr, "incomplete read of player message header\n");
    return;
  }

  // verify length
  uint16_t len;
  nboUnpackUShort(buffer, len);
  if (int(len) != msglen - 4) {
    fprintf(stderr, "incomplete read of player message body\n");
    return;
  }

  // relay packet to all players needing multicast relay
  for (int i = 0; i < maxPlayers; i++)
    if (player[i].multicastRelay)
      pwrite(i, buffer, msglen);
}

static void relayPlayerPacket(int index, uint16_t len, const void *rawbuf)
{
  // broadcast it
  if (relayOutSocket != -1)
    sendMulticast(relayOutSocket, rawbuf, len + 4, &relayOutAddr);

  // relay packet to all players needing multicast relay
  for (int i = 0; i < maxPlayers; i++)
    if (i != index && player[i].multicastRelay)
      pwrite(i, rawbuf, len + 4);
}

static istream &readToken(istream& input, char *buffer, int n)
{
  int c = -1;

  // skip whitespace
  while (input.good() && (c = input.get()) != -1 && isspace(c) && c != '\n')
    ;

  // read up to whitespace or n - 1 characters into buffer
  int i = 0;
  if (c != -1 && c != '\n') {
    buffer[i++] = c;
    while (input.good() && i < n - 1 && (c = input.get()) != -1 && !isspace(c))
      buffer[i++] = (char)c;
  }

  // terminate string
  buffer[i] = 0;

  // put back last character we didn't use
  if (c != -1 && isspace(c))
    input.putback(c);

  return input;
}

static boolean readWorldStream(istream& input, const char *location, WorldFileObjectList& list)
{
  int line = 1;
  char buffer[1024];
  WorldFileObject *object    = NULL;
  WorldFileObject *newObject = NULL;
  while (!input.eof())
  {
    // watch out for starting a new object when one is already in progress
    if (newObject) {
      if (object) {
       cerr << location << "(" << line << ") : " << "discarding incomplete object" << endl;
       delete object;
      }
      object = newObject;
      newObject = NULL;
    }

    // read first token but do not skip newlines
    readToken(input, buffer, sizeof(buffer));
    if (strcmp(buffer, "") == 0) {
      // ignore blank line
    }

    else if (buffer[0] == '#') {
      // ignore comment
    }

    else if (strcmp(buffer, "end") == 0) {
      if (object) {
       list.append(object);
       object = NULL;
      }
      else {
       cerr << location << "(" << line << ") : " << "unexpected \"end\" token" << endl;
       return False;
      }
    }

    else if (strcmp(buffer, "box") == 0)
      newObject = new CustomBox;

    else if (strcmp(buffer, "pyramid") == 0)
      newObject = new CustomPyramid();

    else if (strcmp(buffer, "teleporter") == 0)
      newObject = new CustomGate();

    else if (strcmp(buffer, "link") == 0)
      newObject = new CustomLink();

    else if (strcmp(buffer, "base") == 0)
      newObject = new CustomBase;

    // FIXME - only load one object of the type CustomWorld!
    else if (strcmp(buffer, "world") == 0)
      newObject = new CustomWorld();

    else if (object) {
      if (!object->read(buffer, input)) {
       // unknown token
       cerr << location << "(" << line << ") : " <<
		       "invalid object parameter \"" << buffer << "\"" << endl;
       delete object;
       return False;
      }
    }

    // filling the current object
    else {
      // unknown token
      cerr << location << "(" << line << ") : " << "invalid object type \"" << buffer << "\"" << endl;
      delete object;
      return False;
    }

    // discard remainder of line
    while (input.good() && input.peek() != '\n')
      input.get(buffer, sizeof(buffer));
    input.getline(buffer, sizeof(buffer));
    ++line;
  }

  if (object) {
    cerr << location << "(" << line << ") : " << "missing \"end\" token" << endl;
    delete object;
    return False;
  }

  return True;
}

static WorldInfo *defineWorldFromFile(const char *filename, boolean is_CTF = false)
{
  // open file
  ifstream input(filename, ios::in | ios::nocreate);
  if (!input) {
    cerr << "could not find bzflag world file : " << filename << endl;
    return NULL;
  }

  // create world object
  world = new WorldInfo;
  if (!world)
    return NULL;

  // read file
  WorldFileObjectList list;
  if (!readWorldStream(input, filename, list)) {
    emptyWorldFileObjectList(list);
    delete world;
    return NULL;
  }

  // make walls
  world->addWall(0.0f, 0.5f * WorldSize, 0.0f, 1.5f * M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(0.5f * WorldSize, 0.0f, 0.0f, M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(0.0f, -0.5f * WorldSize, 0.0f, 0.5f * M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(-0.5f * WorldSize, 0.0f, 0.0f, 0.0f, 0.5f * WorldSize, WallHeight);

  // add objects
  const int n = list.getLength();
  for (int i = 0; i < n; ++i)
    list[i]->write(world);

  // if it's a CTF world, add bases
  if (is_CTF) {
    basePos[0][0] = 0.0f;
    basePos[0][1] = 0.0f;
    basePos[0][2] = 0.0f;
    safetyBasePos[0][0] = basePos[0][0];
    safetyBasePos[0][1] = basePos[0][1];
    safetyBasePos[0][2] = basePos[0][2];
    safetyBasePos[1][0] = basePos[1][0] + 0.5f * baseSize[1][0] + PyrBase;
    safetyBasePos[1][1] = basePos[1][1] + 0.5f * baseSize[1][1] + PyrBase;
    safetyBasePos[1][2] = basePos[1][2];
    safetyBasePos[2][0] = basePos[2][0] - 0.5f * baseSize[2][0] - PyrBase;
    safetyBasePos[2][1] = basePos[2][1] - 0.5f * baseSize[2][1] - PyrBase;
    safetyBasePos[2][2] = basePos[2][2];
    safetyBasePos[3][0] = basePos[3][0] - 0.5f * baseSize[3][0] - PyrBase;
    safetyBasePos[3][1] = basePos[3][1] + 0.5f * baseSize[3][1] + PyrBase;
    safetyBasePos[3][2] = basePos[3][2];
    safetyBasePos[4][0] = basePos[4][0] + 0.5f * baseSize[4][0] + PyrBase;
    safetyBasePos[4][1] = basePos[4][1] - 0.5f * baseSize[4][1] - PyrBase;
    safetyBasePos[4][2] = basePos[4][2];
  }

  // clean up
  emptyWorldFileObjectList(list);
  return world;
}


static WorldInfo *defineTeamWorld()
{
  if (!worldFile) {
    world = new WorldInfo();
    if (!world)
      return NULL;

    // set team base and team flag safety positions
    basePos[0][0] = 0.0f;
    basePos[0][1] = 0.0f;
    basePos[0][2] = 0.0f;
    baseRotation[0] = 0.0f;
    baseSize[0][0] = 0.0f;
    baseSize[0][1] = 0.0f;
    basePos[1][0] = (-WorldSize + BaseSize) / 2.0f;
    basePos[1][1] = 0.0f;
    basePos[1][2] = 0.0f;
    baseRotation[1] = 0.0f;
    baseSize[1][0] = BaseSize / 2.0f;
    baseSize[1][1] = BaseSize / 2.0f;
    basePos[2][0] = (WorldSize - BaseSize) / 2.0f;
    basePos[2][1] = 0.0f;
    basePos[2][2] = 0.0f;
    baseRotation[2] = 0.0f;
    baseSize[2][0] = BaseSize / 2.0f;
    baseSize[2][1] = BaseSize / 2.0f;
    basePos[3][0] = 0.0f;
    basePos[3][1] = (-WorldSize + BaseSize) / 2.0f;
    basePos[3][2] = 0.0f;
    baseRotation[3] = 0.0f;
    baseSize[3][0] = BaseSize / 2.0f;
    baseSize[3][1] = BaseSize / 2.0f;
    basePos[4][0] = 0.0f;
    basePos[4][1] = (WorldSize - BaseSize) / 2.0f;
    basePos[4][2] = 0.0f;
    baseRotation[4] = 0.0f;
    baseSize[4][0] = BaseSize / 2.0f;
    baseSize[4][1] = BaseSize / 2.0f;
    safetyBasePos[0][0] = basePos[0][0];
    safetyBasePos[0][1] = basePos[0][1];
    safetyBasePos[0][2] = basePos[0][2];
    safetyBasePos[1][0] = basePos[1][0] + 0.5f * BaseSize + PyrBase;
    safetyBasePos[1][1] = basePos[1][1] + 0.5f * BaseSize + PyrBase;
    safetyBasePos[1][2] = basePos[1][2];
    safetyBasePos[2][0] = basePos[2][0] - 0.5f * BaseSize - PyrBase;
    safetyBasePos[2][1] = basePos[2][1] - 0.5f * BaseSize - PyrBase;
    safetyBasePos[2][2] = basePos[2][2];
    safetyBasePos[3][0] = basePos[3][0] - 0.5f * BaseSize - PyrBase;
    safetyBasePos[3][1] = basePos[3][1] + 0.5f * BaseSize + PyrBase;
    safetyBasePos[3][2] = basePos[3][2];
    safetyBasePos[4][0] = basePos[4][0] + 0.5f * BaseSize + PyrBase;
    safetyBasePos[4][1] = basePos[4][1] - 0.5f * BaseSize - PyrBase;
    safetyBasePos[4][2] = basePos[4][2];

    // make walls
    world->addWall(0.0f, 0.5f * WorldSize, 0.0f, 1.5f * M_PI, 0.5f * WorldSize, WallHeight);
    world->addWall(0.5f * WorldSize, 0.0f, 0.0f, M_PI, 0.5f * WorldSize, WallHeight);
    world->addWall(0.0f, -0.5f * WorldSize, 0.0f, 0.5f * M_PI, 0.5f * WorldSize, WallHeight);
    world->addWall(-0.5f * WorldSize, 0.0f, 0.0f, 0.0f, 0.5f * WorldSize, WallHeight);

    // make pyramids
    // around red base
    world->addPyramid(
	basePos[1][0] + 0.5f * BaseSize - PyrBase,
	basePos[1][1] - 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[1][0] + 0.5f * BaseSize + PyrBase,
	basePos[1][1] - 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[1][0] + 0.5f * BaseSize + PyrBase,
	basePos[1][1] + 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[1][0] + 0.5f * BaseSize - PyrBase,
	basePos[1][1] + 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // around green base
    world->addPyramid(
	basePos[2][0] - 0.5f * BaseSize + PyrBase,
	basePos[2][1] - 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[2][0] - 0.5f * BaseSize - PyrBase,
	basePos[2][1] - 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[2][0] - 0.5f * BaseSize - PyrBase,
	basePos[2][1] + 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[2][0] - 0.5f * BaseSize + PyrBase,
	basePos[2][1] + 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // around blue base
    world->addPyramid(
	basePos[3][0] - 0.5f * BaseSize - PyrBase,
	basePos[3][1] + 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[3][0] - 0.5f * BaseSize + PyrBase,
	basePos[3][1] + 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[3][0] + 0.5f * BaseSize - PyrBase,
	basePos[3][1] + 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[3][0] + 0.5f * BaseSize + PyrBase,
	basePos[3][1] + 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // around purple base
    world->addPyramid(
	basePos[4][0] - 0.5f * BaseSize - PyrBase,
	basePos[4][1] - 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[4][0] - 0.5f * BaseSize + PyrBase,
	basePos[4][1] - 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[4][0] + 0.5f * BaseSize - PyrBase,
	basePos[4][1] - 0.5f * BaseSize - PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	basePos[4][0] + 0.5f * BaseSize + PyrBase,
	basePos[4][1] - 0.5f * BaseSize + PyrBase, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // in center
    world->addPyramid(
	-(BoxBase + 0.25f * AvenueSize),
	-(BoxBase + 0.25f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	(BoxBase + 0.25f * AvenueSize),
	-(BoxBase + 0.25f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	-(BoxBase + 0.25f * AvenueSize),
	(BoxBase + 0.25f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(
	(BoxBase + 0.25f * AvenueSize),
	(BoxBase + 0.25f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(0.0f, -(BoxBase + 0.5f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(0.0f,  (BoxBase + 0.5f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(-(BoxBase + 0.5f * AvenueSize), 0.0f, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid( (BoxBase + 0.5f * AvenueSize), 0.0f, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // halfway out from city center
    world->addPyramid(0.0f, -(3.0f * BoxBase + 1.5f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(0.0f,  (3.0f * BoxBase + 1.5f * AvenueSize), 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid(-(3.0f * BoxBase + 1.5f * AvenueSize), 0.0f, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);
    world->addPyramid( (3.0f * BoxBase + 1.5f * AvenueSize), 0.0f, 0.0f, 0.0f,
	PyrBase, PyrBase, PyrHeight);

    // add boxes, four at once with same height so no team has an advantage
    const float xmin = -0.5f * ((2.0f * BoxBase + AvenueSize) * (CitySize - 1));
    const float ymin = -0.5f * ((2.0f * BoxBase + AvenueSize) * (CitySize - 1));
    for (int j = 0; j <= CitySize/2; j++)
      for (int i = 0; i < CitySize/2; i++)
	if (i != CitySize/2 || j != CitySize/2) {
	  float h = BoxHeight;
	  if (randomHeights)
  	    h *= 2.0f * (float)bzfrand() + 0.5f;
	  world->addBox(
	      xmin + float(i) * (2.0f * BoxBase + AvenueSize),
	      ymin + float(j) * (2.0f * BoxBase + AvenueSize), 0.0f,
	      randomBoxes ? (0.5f * M_PI * ((float)bzfrand() - 0.5f)) : 0.0f,
	      BoxBase, BoxBase, h);
	  world->addBox(
	      -1.0f * (xmin + float(i) * (2.0f * BoxBase + AvenueSize)),
	      -1.0f * (ymin + float(j) * (2.0f * BoxBase + AvenueSize)), 0.0f,
	      randomBoxes ? (0.5f * M_PI * ((float)bzfrand() - 0.5f)) : 0.0f,
	      BoxBase, BoxBase, h);
	  world->addBox(
	      -1.0f * (ymin + float(j) * (2.0f * BoxBase + AvenueSize)),
	      xmin + float(i) * (2.0f * BoxBase + AvenueSize), 0.0f,
	      randomBoxes ? (0.5f * M_PI * ((float)bzfrand() - 0.5f)) : 0.0f,
	      BoxBase, BoxBase, h);
	  world->addBox(
	      ymin + float(j) * (2.0f * BoxBase + AvenueSize),
	      -1.0f * (xmin + float(i) * (2.0f * BoxBase + AvenueSize)), 0.0f,
	      randomBoxes ? (0.5f * M_PI * ((float)bzfrand() - 0.5f)) : 0.0f,
	      BoxBase, BoxBase, h);
	}

    // add teleporters
    if (useTeleporters) {
      const float xoff = BoxBase + 0.5f * AvenueSize;
      const float yoff = BoxBase + 0.5f * AvenueSize;
      world->addTeleporter( xmin - xoff,  ymin - yoff, 0.0f, 1.25f * M_PI,
  	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter( xmin - xoff, -ymin + yoff, 0.0f, 0.75f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter(-xmin + xoff,  ymin - yoff, 0.0f, 1.75f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter(-xmin + xoff, -ymin + yoff, 0.0f, 0.25f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter(-3.5f * TeleBreadth, -3.5f * TeleBreadth, 0.0f, 1.25f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter(-3.5f * TeleBreadth,  3.5f * TeleBreadth, 0.0f, 0.75f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter( 3.5f * TeleBreadth, -3.5f * TeleBreadth, 0.0f, 1.75f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);
      world->addTeleporter( 3.5f * TeleBreadth,  3.5f * TeleBreadth, 0.0f, 0.25f * M_PI,
	  0.5f * TeleWidth, TeleBreadth, 2.0f * TeleHeight, TeleWidth);

      world->addLink(0, 14);
      world->addLink(1, 7);
      world->addLink(2, 12);
      world->addLink(3, 5);
      world->addLink(4, 10);
      world->addLink(5, 3);
      world->addLink(6, 8);
      world->addLink(7, 1);
      world->addLink(8, 6);
      world->addLink(9, 0);
      world->addLink(10, 4);
      world->addLink(11, 2);
      world->addLink(12, 2);
      world->addLink(13, 4);
      world->addLink(14, 0);
      world->addLink(15, 6);
    }

    return world;
  } else {
  return defineWorldFromFile(worldFile, true);
  }
}

static WorldInfo *defineRandomWorld()
{
  const int numTeleporters = 8 + int(8 * (float)bzfrand());
  world = new WorldInfo();
  if (!world)
    return NULL;

  // make walls
  world->addWall(0.0f, 0.5f * WorldSize, 0.0f, 1.5f * M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(0.5f * WorldSize, 0.0f, 0.0f, M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(0.0f, -0.5f * WorldSize, 0.0f, 0.5f * M_PI, 0.5f * WorldSize, WallHeight);
  world->addWall(-0.5f * WorldSize, 0.0f, 0.0f, 0.0f, 0.5f * WorldSize, WallHeight);

  // make boxes
  int i;
  float h = BoxHeight;
  for (i = 0; i < CitySize * CitySize; i++) {
    if (randomHeights)
      h = BoxHeight * ( 2.0f * (float)bzfrand() + 0.5f );
      world->addBox(WorldSize * ((float)bzfrand() - 0.5f),
	  WorldSize * ((float)bzfrand() - 0.5f),
	  0.0f, 2.0f * M_PI * (float)bzfrand(),
	  BoxBase, BoxBase, h);
  }

  // make pyramids
  h = PyrHeight;
  for (i = 0; i < CitySize * CitySize; i++) {
    if (randomHeights)
      h = PyrHeight * ( 2.0f * (float)bzfrand() + 0.5f);
      world->addPyramid(WorldSize * ((float)bzfrand() - 0.5f),
	  WorldSize * ((float)bzfrand() - 0.5f),
	  0.0f, 2.0f * M_PI * (float)bzfrand(),
	  PyrBase, PyrBase, h);
  }

  if (useTeleporters) {
    // make teleporters
    int (*linked)[2] = (int(*)[2])new int[2 * numTeleporters];
    for (i = 0; i < numTeleporters;) {
      const float x = (WorldSize - 4.0f * TeleBreadth) * ((float)bzfrand() - 0.5f);
      const float y = (WorldSize - 4.0f * TeleBreadth) * ((float)bzfrand() - 0.5f);
      const float rotation = 2.0f * M_PI * (float)bzfrand();

      // if too close to building then try again
      if (world->inBuilding(NULL, x, y, 0, 1.75f * TeleBreadth))
	continue;

      world->addTeleporter(x, y, 0.0f, rotation,
	  0.5f*TeleWidth, TeleBreadth, 2.0f*TeleHeight, TeleWidth);
      linked[i][0] = linked[i][1] = 0;
      i++;
    }

    // make teleporter links
    int numUnlinked = 2 * numTeleporters;
    for (i = 0; i < numTeleporters; i++)
      for (int j = 0; j < 2; j++) {
	int a = (int)(numUnlinked * (float)bzfrand());
	if (linked[i][j])
	  continue;
	for (int k = 0, i2 = i; i2 < numTeleporters; ++i2)
	  for (int j2 = ((i2 == i) ? j : 0); j2 < 2; ++j2) {
	    if (linked[i2][j2])
	      continue;
	    if (k++ == a) {
	      world->addLink(2 * i + j, 2 * i2 + j2);
	      linked[i][j] = 1;
	      numUnlinked--;
	      if (i != i2 || j != j2) {
		world->addLink(2 * i2 + j2, 2 * i + j);
		linked[i2][j2] = 1;
		numUnlinked--;
	      }
	    }
	  }
      }
    delete[] linked;
  }

  return world;
}

static boolean defineWorld()
{
  // clean up old database
  delete world;
  delete[] worldDatabase;

  // make world and add buildings
   if (gameStyle & TeamFlagGameStyle)
   {
      world = defineTeamWorld();
   }
   else if (worldFile)
   {
      world = defineWorldFromFile(worldFile);
   }
   else
   {
      world = defineRandomWorld();
   }
   if (world == NULL)
   {
      return False;
   }

  // package up world
  world->packDatabase();
  // now get world packaged for network transmission
  worldDatabaseSize = 4 + 24 + world->getDatabaseSize() + 2;
  if (gameStyle & TeamFlagGameStyle)
    worldDatabaseSize += 4 * (4 + 9 * 4);
  worldDatabase = new char[worldDatabaseSize];
  void *buf = worldDatabase;
  buf = nboPackUShort(buf, WorldCodeStyle);
  buf = nboPackUShort(buf, 24);
  buf = nboPackUShort(buf, gameStyle);
  buf = nboPackUShort(buf, maxPlayers);
  buf = nboPackUShort(buf, maxShots);
  buf = nboPackUShort(buf, numFlags);
  buf = nboPackFloat(buf, linearAcceleration);
  buf = nboPackFloat(buf, angularAcceleration);
  buf = nboPackUShort(buf, shakeTimeout);
  buf = nboPackUShort(buf, shakeWins);
  // time-of-day will go here
  buf = nboPackUInt(buf, 0);
  if (gameStyle & TeamFlagGameStyle) {
    for (int i = 1; i < NumTeams; i++) {
      buf = nboPackUShort(buf, WorldCodeBase);
      buf = nboPackUShort(buf, uint16_t(i));
      buf = nboPackFloat(buf, basePos[i][0]);
      buf = nboPackFloat(buf, basePos[i][1]);
      buf = nboPackFloat(buf, basePos[i][2]);
      buf = nboPackFloat(buf, baseRotation[i]);
      buf = nboPackFloat(buf, baseSize[i][0]);
      buf = nboPackFloat(buf, baseSize[i][1]);
      buf = nboPackFloat(buf, safetyBasePos[i][0]);
      buf = nboPackFloat(buf, safetyBasePos[i][1]);
      buf = nboPackFloat(buf, safetyBasePos[i][2]);
    }
  }
  buf = nboPackString(buf, world->getDatabase(), world->getDatabaseSize());
  buf = nboPackUShort(buf, WorldCodeEnd);

  // reset other stuff
  int i;
  for (i = 0; i < NumTeams; i++) {
    team[i].team.size = 0;
    team[i].team.activeSize = 0;
    team[i].team.won = 0;
    team[i].team.lost = 0;
    team[i].radio = InvalidPlayer;
  }
  broadcastRadio = InvalidPlayer;
  numFlagsInAir = 0;
  for (i = 0; i < numFlags; i++)
    resetFlag(i);

  return True;
}

static TeamColor whoseBase(float x, float y)
{
  if (!(gameStyle & TeamFlagGameStyle))
    return NoTeam;

  // FIXME -- doesn't handle rotated bases
  for (int i = 1; i < NumTeams; i++)
    if (fabsf(x - basePos[i][0]) < 0.5 * BaseSize &&
	fabsf(y - basePos[i][1]) < 0.5 * BaseSize)
      return TeamColor(i);
  return NoTeam;
}

#ifdef PRINTSCORE
static void dumpScore()
{
  int i;

  if (!printScore)
    return;
#ifdef TIMELIMIT
  if (timeLimit > 0.0f)
    cout << "#time" << endl << int(timeLimit - timeElapsed) << endl;
#endif
  cout << "#teams" << endl;
  for (i = int(RedTeam); i < NumTeams; i++)
    cout << team[i].team.won << " " <<
	team[i].team.lost << " " <<
	Team::getName(TeamColor(i)) << endl;

  // sort players by team (do it in five easy pieces)
  cout << "#players" << endl;
  for (i = 0; i < NumTeams; i++)
    for (int j = 0; j < maxPlayers; j++)
      if (player[j].fd != NotConnected && int(player[j].team) == i) {
	cout << player[j].wins << " " <<
	    player[j].losses << " " <<
	    player[j].callSign << endl;
      }

  cout << "#end" << endl;
}
#endif

static void acceptClient()
{
  // client (not a player yet) is requesting service.
  // accept incoming connection on our well known port
  struct sockaddr_in addr;
  AddrLen addr_len = sizeof(addr);
  int fd = accept(wksSocket, (struct sockaddr*)&addr, &addr_len);
  if (fd == -1) {
    nerror("accepting on wks");
    return;
  }

  fprintf(stderr, "accept() from %s:%d on %i\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), fd);

  // don't buffer info, send it immediately
  setNoDelay(fd);
  BzfNetwork::setNonBlocking(fd);

  // find open slot in players list
  int playerIndex;
  for (playerIndex = 0; playerIndex < maxPlayers; playerIndex++)
    if (player[playerIndex].fd == NotConnected &&
	reconnect[playerIndex].accept == NotConnected &&
	reconnect[playerIndex].listen == NotConnected)
      break;

  // game is over;  no new players until everyone quits
  int gameListen = NotConnected;
  if (gameOver) {
    for (int i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected) {
	playerIndex = maxPlayers;
	break;
      }
  }
  if (playerIndex < maxPlayers) {
    // create a new socket to listen for reconnection
    addr.sin_family = AF_INET;
    // any open port
    addr.sin_port = 0;
    addr.sin_addr = serverAddress;
    addr_len = sizeof(addr);
    if ((gameListen = socket(AF_INET, SOCK_STREAM, 0)) == -1 ||
	bind(gameListen, (const struct sockaddr*)&addr, sizeof(addr)) == -1 ||
	getsockname(gameListen, (struct sockaddr*)&addr, &addr_len) == -1 ||
	listen(gameListen, 1) == -1) {
      nerror("creating client listen socket");
      if (gameListen != -1)
	close(gameListen);
      gameListen = NotConnected;
    }
  }

  // record what port we accepted on and are now listening on.
  reconnect[playerIndex].time = TimeKeeper::getCurrent();
  reconnect[playerIndex].listen = gameListen;
  reconnect[playerIndex].accept = fd;
  if (reconnect[playerIndex].listen > maxFileDescriptor)
    maxFileDescriptor = reconnect[playerIndex].listen;
  if (reconnect[playerIndex].accept > maxFileDescriptor)
    maxFileDescriptor = reconnect[playerIndex].accept;

  // if don't want another player or couldn't make socket then refuse
  // connection by returning an obviously bogus port (port zero).
  if (reconnect[playerIndex].listen == NotConnected)
    addr.sin_port = htons(0);

  // send server version and which port to reconnect to
  char buffer[8 + sizeof(addr.sin_port)];
  memcpy(buffer, ServerVersion, 8);
  memcpy(buffer + 8, &addr.sin_port, sizeof(addr.sin_port));
  send(fd, (const char*)buffer, sizeof(buffer), 0);

  // don't wait for client to reconnect here in case the client
  // is badly behaved and would cause us to hang on accept().
  // this also goes even if we're rejecting the connection.
}

static void addClient(int playerIndex)
{
  // accept game connection
  AddrLen addr_len = sizeof(player[playerIndex].taddr);
  player[playerIndex].fd = accept(reconnect[playerIndex].listen,
				(struct sockaddr*)&player[playerIndex].taddr, &addr_len);
  if (player[playerIndex].fd == NotConnected)
    nerror("accepting client connection");

  // done with listen socket
  close(reconnect[playerIndex].listen);
  reconnect[playerIndex].listen = NotConnected;

  // see if accept worked
  if (player[playerIndex].fd == NotConnected)
    return;

  // turn off packet buffering and set socket non-blocking
  setNoDelay(player[playerIndex].fd);
  BzfNetwork::setNonBlocking(player[playerIndex].fd);

  // now add client
  player[playerIndex].state = PlayerInLimbo;
  if (player[playerIndex].fd > maxFileDescriptor)
    maxFileDescriptor = player[playerIndex].fd;
  player[playerIndex].peer = Address(player[playerIndex].taddr);
  player[playerIndex].multicastRelay = False;
  player[playerIndex].tcplen = 0;
  player[playerIndex].udplen = 0;
  assert(player[playerIndex].outmsg == NULL);
  player[playerIndex].outmsgSize = 0;
  player[playerIndex].outmsgOffset = 0;
  player[playerIndex].outmsgCapacity = 0;

  // if game was over and this is the first player then game is on
  if (gameOver) {
    int count = 0;
    for (int i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected)
	count++;
    if (count == 1) {
      gameOver = False;
#ifdef TIMELIMIT
      gameStartTime = TimeKeeper::getCurrent();
      timeElapsed = 0.0f;
#endif
    }
  }
}

static void shutdownAcceptClient(int playerIndex)
{
  // close socket that client initially contacted us on
  close(reconnect[playerIndex].accept);
  reconnect[playerIndex].accept = NotConnected;

  // if we're still listening on reconnect port then give up
  if (reconnect[playerIndex].listen != NotConnected) {
    close(reconnect[playerIndex].listen);
    reconnect[playerIndex].listen = NotConnected;
  }
}

static void respondToPing(boolean broadcast = False)
{
  // get and discard ping packet
  int minReplyTTL;
  struct sockaddr_in addr;
  if (broadcast) {
    if (!PingPacket::isRequest(pingBcastSocket, &addr, &minReplyTTL)) return;
  }
  else {
    if (!PingPacket::isRequest(pingInSocket, &addr, &minReplyTTL)) return;
  }

  // if no output port then ignore
  if (!broadcast && pingOutSocket == -1)
    return;

  // if I'm ignoring pings and the ping is not from a connected host
  // then ignore the ping.
  if (!handlePings) {
    int i;
    Address remoteAddress(addr);
    for (i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected && player[i].peer == remoteAddress)
	break;
    if (i == maxPlayers)
      return;
  }

  // boost my reply ttl if ping requests it
  if (minReplyTTL > MaximumTTL)
    minReplyTTL = MaximumTTL;
  if (pingOutSocket != -1 && minReplyTTL > pingTTL) {
    pingTTL = minReplyTTL;
    setMulticastTTL(pingOutSocket, pingTTL);
  }

  // reply with current game info on pingOutSocket or pingBcastSocket
  pingReply.sourceAddr = Address(addr);
  pingReply.rogueCount = team[0].team.activeSize;
  pingReply.redCount = team[1].team.activeSize;
  pingReply.greenCount = team[2].team.activeSize;
  pingReply.blueCount = team[3].team.activeSize;
  pingReply.purpleCount = team[4].team.activeSize;
  if (broadcast)
    pingReply.write(pingBcastSocket, &pingBcastAddr);
  else
    pingReply.write(pingOutSocket, &pingOutAddr);
}

static void sendMessage(int playerIndex, const PlayerId& targetPlayer, TeamColor targetTeam, const char *message)
{
  // player is sending a message to a particular player, a team, or all.
  // send MsgMessage
  char msg[PlayerIdPLen + PlayerIdPLen + 2 + MessageLen];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = targetPlayer.pack(buf);
  buf = nboPackUShort(buf, uint16_t(targetTeam));
  buf = nboPackString(buf, message, MessageLen);
  broadcastMessage(MsgMessage, sizeof(msg), msg);
}

static void addPlayer(int playerIndex)
{
  // reject player if asks for bogus team or rogue and rogues aren't allowed
  // or if the team is full.

  // look if there is as name clash, we won't allow this
  int i;
  for (i = 0; i < maxPlayers; i++)
  {
    if (i == playerIndex)
      continue;
    if (strcasecmp(player[i].callSign,player[playerIndex].callSign) == 0)
      break;
  }
  if (i < maxPlayers)
  {
    // this is a hack; would better add a new reject type
    player[playerIndex].team = NoTeam;
  }

  TeamColor t = player[playerIndex].team;
  if ((t == NoTeam && (player[playerIndex].type == TankPlayer ||
	player[playerIndex].type == ComputerPlayer)) ||
	(t == RogueTeam && !(gameStyle & RoguesGameStyle)) ||
	(team[int(t)].team.activeSize >= maxTeam[int(t)])) {
    uint16_t code = RejectBadRequest;
    if (player[playerIndex].type != TankPlayer &&
	player[playerIndex].type != ComputerPlayer)
      code = RejectBadType;
    else if (t == NoTeam)
      code = RejectBadTeam;
    else if (t == RogueTeam && !(gameStyle & RoguesGameStyle))
      code = RejectNoRogues;
    else if (team[int(t)].team.activeSize >= maxTeam[int(t)]) {
      // if team is full then check if server is full
      code = RejectServerFull;
      for (int i = RogueTeam; i < NumTeams; i++)
	if (team[i].team.activeSize < maxTeam[i]) {
	  code = RejectTeamFull;
	  break;
	}
    }

    char msg[2];
    nboPackUShort(msg, code);
    directMessage(playerIndex, MsgReject, sizeof(msg), &msg);
    return;
  }

  player[playerIndex].ulinkup = false;
  player[playerIndex].toBeKicked = false;
  player[playerIndex].Admin = false;

  player[playerIndex].lastRecvPacketNo = 0;
  player[playerIndex].lastSendPacketNo = 0;

  player[playerIndex].uqueue = NULL;
  player[playerIndex].dqueue = NULL;

  player[playerIndex].lagkillerpending = false;
  player[playerIndex].lagavg = 0;
  player[playerIndex].lagcount = 0;
  player[playerIndex].laglastwarn = 0;
  player[playerIndex].lagwarncount = 0;
  player[playerIndex].lagalpha = 1;


  // accept player
  directMessage(playerIndex, MsgAccept, 0, NULL);

  // abort if we hung up on the client
  if (player[playerIndex].fd == NotConnected)
    return;

  // player is signing on (has already connected via addClient).
  player[playerIndex].state = PlayerOnTeamDead;
  player[playerIndex].flag = -1;
  player[playerIndex].wins = 0;
  player[playerIndex].losses = 0;

  // update team state and if first active player on team,
  // add team's flag and reset it's score
  boolean resetTeamFlag = False;
  int teamIndex = int(player[playerIndex].team);
  if (++team[teamIndex].team.size == 1) {
    team[teamIndex].radio = -1;
  }
  if ((player[playerIndex].type == TankPlayer ||
	player[playerIndex].type == ComputerPlayer) &&
	++team[teamIndex].team.activeSize == 1) {
    team[teamIndex].team.won = 0;
    team[teamIndex].team.lost = 0;
    if ((gameStyle & int(TeamFlagGameStyle)) && teamIndex != int(RogueTeam))
      // can't call resetFlag() here cos it'll screw up protocol for
      // player just joining, so do it later
      resetTeamFlag = True;
  }

  // send new player updates on each player, all existing flags, and all teams.
  // don't send robots any game info.  watch out for connection being closed
  // because of an error.
  if (player[playerIndex].type != ComputerPlayer) {
    int i;
    for (i = 0; i < NumTeams && player[playerIndex].fd != NotConnected; i++)
      sendTeamUpdate(i, playerIndex);
    for (i = 0; i < numFlags && player[playerIndex].fd != NotConnected; i++)
      if (flag[i].flag.status != FlagNoExist)
	sendFlagUpdate(i, playerIndex);
    for (i = 0; i < maxPlayers && player[playerIndex].fd != NotConnected; i++)
      if (player[i].fd != NotConnected && player[i].state != PlayerInLimbo && i != playerIndex)
	sendPlayerUpdate(i, playerIndex);
  }

  // if necessary force multicast relaying
  if (noMulticastRelay) {
    directMessage(playerIndex, MsgNetworkRelay, 0, NULL);
    player[playerIndex].multicastRelay = True;
  }

  // if new player connection was closed (because of an error) then stop here
  if (player[playerIndex].fd == NotConnected)
    return;

  // send MsgAddPlayer to everybody -- this concludes MsgEnter response
  // to joining player
  sendPlayerUpdate(playerIndex);

  // send update of info for team just joined
  sendTeamUpdate(teamIndex);

  // send time update to new player if we're counting down
  if (timeLimit > 0.0f && player[playerIndex].type != ComputerPlayer) {
    float timeLeft = timeLimit - (TimeKeeper::getCurrent() - gameStartTime);
    if (timeLeft < 0.0f) {
      // oops
      timeLeft = 0.0f;
    }
    char msg[2];
    void *buf = msg;
    buf = nboPackUShort(buf, (uint16_t)(int)timeLeft);
    directMessage(playerIndex, MsgTimeUpdate, sizeof(msg), msg);
  }

  // again check if player was disconnected
  if (player[playerIndex].fd == NotConnected)
    return;

  // reset that flag
  if (resetTeamFlag)
    resetFlag(teamIndex-1);

  // tell the list server the new number of players
  sendMessageToListServer("SETNUM");

#ifdef PRINTSCORE
  dumpScore();
#endif
  char message[MessageLen];

#ifdef SERVERLOGINMSG
  sprintf(message,"BZFlag server %d.%d%c%d, http://BZFlag.org/",
      (VERSION / 10000000) % 100, (VERSION / 100000) % 100,
      (char)('a' - 1 + (VERSION / 1000) % 100), VERSION % 1000);
  sendMessage(playerIndex, player[playerIndex].id, player[playerIndex].team, message);

  if (servermsg && (strlen(servermsg) > 0)) {
    sprintf(message,"%s",servermsg);
    sendMessage(playerIndex, player[playerIndex].id, player[playerIndex].team, message);
  }
#endif
}

static void addFlag(int flagIndex)
{
  if (flagIndex == -1) {
    // invalid flag
    return;
  }

  // flag in now entering game
  flag[flagIndex].flag.status = FlagComing;
  numFlagsInAir++;

  // compute drop time
  const float flightTime = 2.0f * sqrtf(-2.0f * FlagAltitude / Gravity);
  flag[flagIndex].flag.flightTime = 0.0f;
  flag[flagIndex].flag.flightEnd = flightTime;
  flag[flagIndex].flag.initialVelocity = -0.5f * Gravity * flightTime;
  flag[flagIndex].dropDone = TimeKeeper::getCurrent();
  flag[flagIndex].dropDone += flightTime;

  // how times will it stick around
  if (flag[flagIndex].flag.type == FlagSticky)
    flag[flagIndex].grabs = 1;
  else
    flag[flagIndex].grabs = int(floor(4.0f * (float)bzfrand())) + 1;
  sendFlagUpdate(flagIndex);
}

static void randomFlag(int flagIndex)
{
  // pick a random flag
  flag[flagIndex].flag.id = allowedFlags[int(numAllowedFlags * (float)bzfrand())];
  flag[flagIndex].flag.type = Flag::getType(flag[flagIndex].flag.id);
  addFlag(flagIndex);
}

static void resetFlag(int flagIndex)
{
  // NOTE -- must not be called until world is defined
  assert(world != NULL);
  if (flagIndex == -1) {
    // invalid flag
    return;
  }

  // reset a flag's info
  flag[flagIndex].player = -1;
  flag[flagIndex].flag.status = FlagNoExist;

  // if it's a random flag, reset flag id
  if (flagIndex >= numFlags - numExtraFlags)
    flag[flagIndex].flag.id = NullFlag;

  // reposition flag
  if (int(flag[flagIndex].flag.id) >= int(FirstTeamFlag) &&
	int(flag[flagIndex].flag.id) <= int(LastTeamFlag)) {
    int teamIndex = int(flag[flagIndex].flag.id);
    flag[flagIndex].flag.position[0] = basePos[teamIndex][0];
    flag[flagIndex].flag.position[1] = basePos[teamIndex][1];
    flag[flagIndex].flag.position[2] = basePos[teamIndex][2];
  }
  else {
    // random position (not in a building)
    float r = TankRadius;
    if (flag[flagIndex].flag.id == ObesityFlag)
       r *= 2.0f * ObeseFactor;
    do {
      if (gameStyle & TeamFlagGameStyle) {
	flag[flagIndex].flag.position[0] =
			0.5f * WorldSize * ((float)bzfrand() - 0.5f);
	flag[flagIndex].flag.position[1] =
			0.5f * WorldSize * ((float)bzfrand() - 0.5f);
	flag[flagIndex].flag.position[2] = 0.0f;
      }
      else {
	flag[flagIndex].flag.position[0] =
			(WorldSize - BaseSize) * ((float)bzfrand() - 0.5f);
	flag[flagIndex].flag.position[1] =
			(WorldSize - BaseSize) * ((float)bzfrand() - 0.5f);
	flag[flagIndex].flag.position[2] = 0.0f;
      }
    } while (world->inBuilding(NULL, flag[flagIndex].flag.position[0], flag[flagIndex].flag.position[1], 
	flag[flagIndex].flag.position[2], r));
  }

  // required flags mustn't just disappear
  if (flag[flagIndex].required) {
    if (int(flag[flagIndex].flag.id) >= FirstTeamFlag &&
	int(flag[flagIndex].flag.id) <= LastTeamFlag)
      if (team[int(flag[flagIndex].flag.id)].team.activeSize == 0)
	flag[flagIndex].flag.status = FlagNoExist;
      else
	flag[flagIndex].flag.status = FlagOnGround;
    else if (flag[flagIndex].flag.id == NullFlag)
      randomFlag(flagIndex);
    else
      addFlag(flagIndex);
  }

  sendFlagUpdate(flagIndex);
}

static void zapFlag(int flagIndex)
{
  // called when a flag must just disappear -- doesn't fly
  // into air, just *poof* vanishes.
  if (flagIndex == -1) {
    // invalid flag
    return;
  }

  // see if someone had grabbed flag.  tell 'em to drop it.
  const int playerIndex = flag[flagIndex].player;
  if (playerIndex != -1) {
    flag[flagIndex].player = -1;
    flag[flagIndex].flag.status = FlagNoExist;
    player[playerIndex].flag = -1;

    char msg[PlayerIdPLen + 2 + FlagPLen];
    void *buf = msg;
    buf = player[playerIndex].id.pack(buf);
    buf = nboPackUShort(buf, uint16_t(flagIndex));
    buf = flag[flagIndex].flag.pack(buf);
    broadcastMessage(MsgDropFlag, sizeof(msg), msg);
  }

  // if flag was flying then it flies no more
  if (flag[flagIndex].flag.status == FlagInAir ||
      flag[flagIndex].flag.status == FlagComing ||
      flag[flagIndex].flag.status == FlagGoing)
    numFlagsInAir--;

  // reset flag status
  resetFlag(flagIndex);
}

static void removePlayer(int playerIndex)
{
  // player is signing off or sent a bad packet.  since the
  // bad packet can come before MsgEnter, we must be careful
  // not to undo operations that haven't been done.
  // first shutdown connection

  // check if we are called again for a dropped player!
  if (player[playerIndex].fd == NotConnected)
    return;

  // status message
  UMDEBUG("Player %s [%d] is removed\n",player[playerIndex].callSign,playerIndex);

  // shutdown TCP socket
  shutdown(player[playerIndex].fd, 2);
  close(player[playerIndex].fd);

  // free up the UDP packet buffers
  if (player[playerIndex].uqueue)
    disqueuePacket(playerIndex, SEND, 65536);
  if (player[playerIndex].dqueue)
    disqueuePacket(playerIndex, RECEIVE, 65536);

  player[playerIndex].uqueue = NULL;
  player[playerIndex].dqueue = NULL;
  player[playerIndex].lastRecvPacketNo = 0;
  player[playerIndex].lastSendPacketNo = 0;

  // shutdown the UDP socket
  memset(&player[playerIndex].uaddr,0,sizeof(player[playerIndex].uaddr));

  // no UDP connection anymore
  player[playerIndex].ulinkup = false;
  player[playerIndex].toBeKicked = false;
  player[playerIndex].udplen = 0;

  player[playerIndex].fd = NotConnected;
  player[playerIndex].tcplen = 0;

  player[playerIndex].callSign[0] = 0;

  if (player[playerIndex].outmsg != NULL)
    delete[] player[playerIndex].outmsg;

  player[playerIndex].outmsg = NULL;

  // can we turn off relaying now?
  if (player[playerIndex].multicastRelay) {
    int i;
    player[playerIndex].multicastRelay = False;
    for (i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected && player[i].multicastRelay)
	break;
    if (i == maxPlayers)
      stopPlayerPacketRelay();
  }

  // player is outta here.  if player never joined a team then
  // don't count as a player.
  if (player[playerIndex].state == PlayerInLimbo)
    return;
  player[playerIndex].state = PlayerInLimbo;

  if (player[playerIndex].team != NoTeam) {
    // if player had flag then flag just disappears.  it'd be nice
    // to have it fly as if dropped, but we've no idea what the
    // player's position is.
    if (player[playerIndex].flag != -1)
      zapFlag(player[playerIndex].flag);

    // if player had radio then release it
    if (team[int(player[playerIndex].team)].radio == playerIndex)
      releaseRadio(playerIndex);

    // tell everyone player has left
    char msg[PlayerIdPLen];
    player[playerIndex].id.pack(msg);
    broadcastMessage(MsgRemovePlayer, sizeof(msg), msg);

    // decrease team size
    int teamNum = int(player[playerIndex].team);
    --team[teamNum].team.size;
    if (player[playerIndex].type == TankPlayer ||
		player[playerIndex].type == ComputerPlayer)
      --team[teamNum].team.activeSize;

    // if last active player on team then remove team's flag
    if (teamNum != int(RogueTeam) &&
	(player[playerIndex].type == TankPlayer ||
	player[playerIndex].type == ComputerPlayer) &&
	team[teamNum].team.activeSize == 0 &&
	(gameStyle & int(TeamFlagGameStyle)))
      zapFlag(teamNum-1);

    // send team update
    sendTeamUpdate(teamNum);
  }

  // tell the list server the new number of players
  sendMessageToListServer("SETNUM");

  // anybody left?
  int i;
  for (i = 0; i < maxPlayers; i++)
    if (player[i].fd != NotConnected && player[i].state != PlayerInLimbo)
      break;

  // if everybody left then reset world
  if (i == maxPlayers) {
    if (oneGameOnly) {
      done = True;
      exitCode = 0;
    }
    else if (!defineWorld()) {
      done = True;
      exitCode = 1;
    }
    else {
      // republicize ourself.  this dereferences the URL chain
      // again so we'll notice any pointer change when any game
      // is over (i.e. all players have quit).
      publicize();
    }
  }
}

static void sendWorld(int playerIndex, int ptr)
{
  // send another small chunk of the world database
  assert(world != NULL && worldDatabase != NULL);
  char buffer[258];
  void *buf = buffer;
  int size = 256, left = worldDatabaseSize - ptr;
  if (ptr >= worldDatabaseSize) {
    size = 0;
    left = 0;
  } else
    if (ptr + size >= worldDatabaseSize)
      size = worldDatabaseSize - ptr;
  buf = nboPackUShort(buf, uint16_t(left));
  buf = nboPackString(buf, (char*)worldDatabase + ptr, size);
  directMessage(playerIndex, MsgGetWorld, size + 2, buffer);
}

static void sendQueryGame(int playerIndex)
{
  // much like a ping packet but leave out useless stuff (like
  // the server address, which must already be known, and the
  // server version, which was already sent).
  char  buffer[36];
  void *buf = (void*)buffer;
  buf = nboPackUShort(buf, pingReply.gameStyle);
  buf = nboPackUShort(buf, pingReply.maxPlayers);
  buf = nboPackUShort(buf, pingReply.maxShots);
  buf = nboPackUShort(buf, team[0].team.activeSize);
  buf = nboPackUShort(buf, team[1].team.activeSize);
  buf = nboPackUShort(buf, team[2].team.activeSize);
  buf = nboPackUShort(buf, team[3].team.activeSize);
  buf = nboPackUShort(buf, team[4].team.activeSize);
  buf = nboPackUShort(buf, pingReply.rogueMax);
  buf = nboPackUShort(buf, pingReply.redMax);
  buf = nboPackUShort(buf, pingReply.greenMax);
  buf = nboPackUShort(buf, pingReply.blueMax);
  buf = nboPackUShort(buf, pingReply.purpleMax);
  buf = nboPackUShort(buf, pingReply.shakeWins);
  // 1/10ths of second
  buf = nboPackUShort(buf, pingReply.shakeTimeout);
  buf = nboPackUShort(buf, pingReply.maxPlayerScore);
  buf = nboPackUShort(buf, pingReply.maxTeamScore);
  buf = nboPackUShort(buf, pingReply.maxTime);

  // send it
  directMessage(playerIndex, MsgQueryGame, sizeof(buffer), buffer);
}

static void sendQueryPlayers(int playerIndex)
{
  int i, numPlayers = 0;

  // count the number of active players
  for (i = 0; i < maxPlayers; i++)
    if (player[i].fd != NotConnected && player[i].state != PlayerInLimbo)
      numPlayers++;

  // first send number of teams and players being sent
  char buffer[4];
  void *buf = (void*)buffer;
  buf = nboPackUShort(buf, NumTeams);
  buf = nboPackUShort(buf, numPlayers);
  directMessage(playerIndex, MsgQueryPlayers, sizeof(buffer), buffer);

  // now send the teams and players
  for (i = 0; i < NumTeams && player[playerIndex].fd != NotConnected; i++)
    sendTeamUpdate(i, playerIndex);
  for (i = 0; i < maxPlayers && player[playerIndex].fd != NotConnected; i++)
    if (player[i].fd != NotConnected && player[i].state != PlayerInLimbo)
      sendPlayerUpdate(i, playerIndex);
}

static void playerAlive(int playerIndex, const float *pos, const float *fwd)
{
  // player is coming alive.  strictly speaking, this can be inferred
  // from the multicast info, but it's nice to have a clear statement.
  // it also allows clients that don't snoop the multicast group to
  // find about it.
  player[playerIndex].state = PlayerOnTeamAlive;
  player[playerIndex].flag = -1;

  // send MsgAlive
  char msg[PlayerIdPLen + 24];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackFloat(buf, pos[0]);
  buf = nboPackFloat(buf, pos[1]);
  buf = nboPackFloat(buf, pos[2]);
  buf = nboPackFloat(buf, fwd[0]);
  buf = nboPackFloat(buf, fwd[1]);
  buf = nboPackFloat(buf, fwd[2]);
  broadcastMessage(MsgAlive, sizeof(msg), msg);
}

static void checkTeamScore(int playerIndex, int teamIndex)
{
  if (maxTeamScore == 0 || teamIndex == (int)RogueTeam) return;
  if (team[teamIndex].team.won - team[teamIndex].team.lost >= maxTeamScore) {
    char msg[PlayerIdPLen + 2];
    void *buf = msg;
    buf = player[playerIndex].id.pack(buf);
    buf = nboPackUShort(buf, uint16_t(teamIndex));
    broadcastMessage(MsgScoreOver, sizeof(msg), msg);
    gameOver = True;
  }
}

static void playerKilled(int victimIndex, int killerIndex,
							int16_t shotIndex)
{
  // victim has been destroyed.  keep score.
  if (killerIndex == InvalidPlayer ||
	player[victimIndex].state != PlayerOnTeamAlive) return;
  if (team[int(player[victimIndex].team)].radio == victimIndex)
    releaseRadio(victimIndex);
  player[victimIndex].state = PlayerOnTeamDead;

  // send MsgKilled
  char msg[PlayerIdPLen + PlayerIdPLen + 2];
  void *buf = msg;
  buf = player[victimIndex].id.pack(buf);
  buf = player[killerIndex].id.pack(buf);
  buf = nboPackShort(buf, shotIndex);
  broadcastMessage(MsgKilled, sizeof(msg), msg);

  // we try to estimate lag by measuring time between broadcast of MsgKilled
  // and MsgScore reply of the killer; should give us a good approximation
  //  of the killer's round trip time
  if (victimIndex != killerIndex &&            // suicide doesn't change score twice
      !player[killerIndex].lagkillerpending) { // two rapid kills
    player[killerIndex].lagkillerpending = true;
    player[killerIndex].lagkillertime = TimeKeeper::getCurrent();
  }

  // zap flag player was carrying.  clients should send a drop flag
  // message before sending a killed message, so this shouldn't happen.
  zapFlag(player[victimIndex].flag);

  // change the team scores -- rogues don't have team scores.  don't
  // change team scores for individual player's kills in capture the
  // flag mode.
  int winningTeam = (int)NoTeam;
  if (!(gameStyle & TeamFlagGameStyle)) {
    if (player[victimIndex].team == player[killerIndex].team) {
      if (player[killerIndex].team != RogueTeam)
	if (killerIndex == victimIndex)
	  team[int(player[victimIndex].team)].team.lost += 1;
	else
	  team[int(player[victimIndex].team)].team.lost += 2;
    }
    else {
      if (player[killerIndex].team != RogueTeam) {
	winningTeam = int(player[killerIndex].team);
	team[winningTeam].team.won++;
      }
      if (player[victimIndex].team != RogueTeam)
	team[int(player[victimIndex].team)].team.lost++;
      sendTeamUpdate(int(player[killerIndex].team));
    }
    sendTeamUpdate(int(player[victimIndex].team));
  }
#ifdef PRINTSCORE
  dumpScore();
#endif
  if (winningTeam != (int)NoTeam)
    checkTeamScore(killerIndex, winningTeam);
}

static void grabFlag(int playerIndex, int flagIndex)
{
  // player wants to take possession of flag
  if (player[playerIndex].state != PlayerOnTeamAlive ||
      player[playerIndex].flag != -1 ||
      flag[flagIndex].flag.status != FlagOnGround)
    return;
  // okay, player can have it
  flag[flagIndex].flag.status = FlagOnTank;
  flag[flagIndex].flag.owner = player[playerIndex].id;
  flag[flagIndex].player = playerIndex;
  player[playerIndex].flag = flagIndex;

  // send MsgGrabFlag
  char msg[PlayerIdPLen + 2 + FlagPLen];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, uint16_t(flagIndex));
  buf = flag[flagIndex].flag.pack(buf);
  broadcastMessage(MsgGrabFlag, sizeof(msg), msg);
}

static void dropFlag(int playerIndex, float pos[3])
{
  assert(world != NULL);
  WorldInfo::ObstacleLocation* container;
  int topmosttype;
  WorldInfo::ObstacleLocation* topmost = (WorldInfo::ObstacleLocation *)NULL;
  // player wants to drop flag.  we trust that the client won't tell
  // us to drop a sticky flag until the requirements are satisfied.
  const int flagIndex = player[playerIndex].flag;
  if (flagIndex == -1 || flag[flagIndex].flag.status != FlagOnTank)
    return;

  // okay, go ahead and drop it
  flag[flagIndex].player = -1;
  if (flag[flagIndex].flag.type == FlagNormal || --flag[flagIndex].grabs > 0)
    flag[flagIndex].flag.status = FlagInAir;
  else
    flag[flagIndex].flag.status = FlagGoing;
  numFlagsInAir++;

  for (float i = pos[2]; i >= 0.0f; i -= 0.1f) {
    topmosttype = world->inBuilding(&container, pos[0], pos[1], i, 0);
    if (topmosttype) {
      topmost = container;
      break;
    }
  }

  // figure out landing spot -- if flag in a Bad Place
  // when dropped, move to safety position or make it going
  TeamColor teamBase = whoseBase(pos[0], pos[1]);
  if (flag[flagIndex].flag.status == FlagGoing) {
    flag[flagIndex].flag.landingPosition[0] = pos[0];
    flag[flagIndex].flag.landingPosition[1] = pos[1];
    flag[flagIndex].flag.landingPosition[2] = pos[2];
  }
  else if (int(flag[flagIndex].flag.id) >= int(FirstTeamFlag) &&
      int(flag[flagIndex].flag.id) <= int(LastTeamFlag) &&
      teamBase != NoTeam && int(teamBase) != int(flag[flagIndex].flag.id)) {
    flag[flagIndex].flag.landingPosition[0] = safetyBasePos[int(teamBase)][0];
    flag[flagIndex].flag.landingPosition[1] = safetyBasePos[int(teamBase)][1];
    flag[flagIndex].flag.landingPosition[2] = safetyBasePos[int(teamBase)][2];
  }
  else if (topmosttype == 0) {
    flag[flagIndex].flag.landingPosition[0] = pos[0];
    flag[flagIndex].flag.landingPosition[1] = pos[1];
    flag[flagIndex].flag.landingPosition[2] = 0.0f;
  }
  else if (flagsOnBuildings && (topmosttype == 1)) {
    flag[flagIndex].flag.landingPosition[0] = pos[0];
    flag[flagIndex].flag.landingPosition[1] = pos[1];
    flag[flagIndex].flag.landingPosition[2] = topmost->pos[2] + topmost->size[2];
  }
  else if (int(flag[flagIndex].flag.id) >= int(FirstTeamFlag) &&
      int(flag[flagIndex].flag.id) <= int(LastTeamFlag)) {
    // people were cheating by dropping their flag above the nearest
    // convenient building which makes it fly all the way back to
    // your own base.  make it fly to the center of the board.
    flag[flagIndex].flag.landingPosition[0] = 0.0f;
    flag[flagIndex].flag.landingPosition[1] = 0.0f;
    flag[flagIndex].flag.landingPosition[2] = 0.0f;
  }
  else
    flag[flagIndex].flag.status = FlagGoing;

  flag[flagIndex].flag.position[0] = flag[flagIndex].flag.landingPosition[0];
  flag[flagIndex].flag.position[1] = flag[flagIndex].flag.landingPosition[1];
  flag[flagIndex].flag.position[2] = flag[flagIndex].flag.landingPosition[2];
  flag[flagIndex].flag.launchPosition[0] = pos[0];
  flag[flagIndex].flag.launchPosition[1] = pos[1];
  flag[flagIndex].flag.launchPosition[2] = pos[2] + TankHeight;

  // compute flight info -- flight time depends depends on start and end
  // altitudes and desired height above start altitude.
  const float thrownAltitude = (flag[flagIndex].flag.id == ShieldFlag) ?
      ShieldFlight * FlagAltitude : FlagAltitude;
  const float maxAltitude = pos[2] + TankHeight + thrownAltitude;
  const float upTime = sqrtf(-2.0f * thrownAltitude / Gravity);
  const float downTime = sqrtf(-2.0f * (maxAltitude - pos[2]) / Gravity);
  const float flightTime = upTime + downTime;

  // set flight info
  flag[flagIndex].dropDone = TimeKeeper::getCurrent();
  flag[flagIndex].dropDone += flightTime;
  flag[flagIndex].flag.flightTime = 0.0f;
  flag[flagIndex].flag.flightEnd = flightTime;
  flag[flagIndex].flag.initialVelocity = -Gravity * upTime;

  // player no longer has flag -- send MsgDropFlag
  player[playerIndex].flag = -1;
  char msg[PlayerIdPLen + 2 + FlagPLen];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, uint16_t(flagIndex));
  buf = flag[flagIndex].flag.pack(buf);
  broadcastMessage(MsgDropFlag, sizeof(msg), msg);

  // notify of new flag state
  sendFlagUpdate(flagIndex);
}

static void captureFlag(int playerIndex, TeamColor teamCaptured)
{
  // player captured a flag.  can either be enemy flag in player's own
  // team base, or player's own flag in enemy base.
  int flagIndex = int(player[playerIndex].flag);
  if (flagIndex < 0 ||
      int(flag[flagIndex].flag.id) < int(FirstTeamFlag) ||
      int(flag[flagIndex].flag.id) > int(LastTeamFlag))
    return;

  // player no longer has flag and put flag back at it's base
  player[playerIndex].flag = -1;
  resetFlag(flagIndex);

  // send MsgCaptureFlag
  char msg[PlayerIdPLen + 4];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, uint16_t(flagIndex));
  buf = nboPackUShort(buf, uint16_t(teamCaptured));
  broadcastMessage(MsgCaptureFlag, sizeof(msg), msg);

  // everyone on losing team is dead
  for (int i = 0; i < maxPlayers; i++)
    if (player[i].fd != NotConnected &&
	int(flag[flagIndex].flag.id) == int(player[i].team) &&
	player[i].state == PlayerOnTeamAlive) {
      if (team[int(player[i].team)].radio == i)
	releaseRadio(i);
      player[i].state = PlayerOnTeamDead;
    }

  // update score (rogues can't capture flags)
  int winningTeam = (int)NoTeam;
  if (int(flag[flagIndex].flag.id) != int(player[playerIndex].team)) {
    // player captured enemy flag
    winningTeam = int(player[playerIndex].team);
    team[winningTeam].team.won++;
    sendTeamUpdate(winningTeam);
  }
  team[int(flag[flagIndex].flag.id)].team.lost++;
  sendTeamUpdate(int(flag[flagIndex].flag.id));
#ifdef PRINTSCORE
  dumpScore();
#endif
  if (winningTeam != (int)NoTeam)
    checkTeamScore(playerIndex, winningTeam);
}

static void shotFired(int /*playerIndex*/, const void *buf, int len)
{
  // player has fired shot -- send MsgShotBegin
  broadcastMessage(MsgShotBegin, len, buf);
}

static void shotEnded(const PlayerId& id, int16_t shotIndex, uint16_t reason)
{
  // shot has ended prematurely -- send MsgShotEnd
  char msg[PlayerIdPLen + 4];
  void *buf = msg;
  buf = id.pack(buf);
  buf = nboPackShort(buf, shotIndex);
  buf = nboPackUShort(buf, reason);
  broadcastMessage(MsgShotEnd, sizeof(msg), msg);
}

static void scoreChanged(int playerIndex, uint16_t wins, uint16_t losses)
{
  // lag measurement
  // got reference time?
  if (player[playerIndex].lagkillerpending) {
    PlayerInfo &killer = player[playerIndex];
    TimeKeeper now = TimeKeeper::getCurrent(),&then = killer.lagkillertime;
    float timepassed = now-then;
    killer.lagkillerpending = false;
    // huge lags might be error!?
    if (timepassed < 10.0) {
      // time is smoothed exponentially using a dynamic smoothing factor
      killer.lagavg = killer.lagavg*(1-killer.lagalpha)+killer.lagalpha*timepassed;
      killer.lagalpha = killer.lagalpha/(0.9f+killer.lagalpha);
      killer.lagcount++;
      // warn players from time to time whose lag is > threshold (-lagwarn)
      if (lagwarnthresh > 0 && killer.lagavg > lagwarnthresh &&
	  killer.lagcount-killer.laglastwarn > 5 + 2 * killer.lagwarncount) {
	char message[MessageLen];
	sprintf(message,"*** Server Warning: your lag is too high (%d ms) ***",
	    int(killer.lagavg * 1000));
	sendMessage(playerIndex, player[playerIndex].id,
	    player[playerIndex].team,message);
	killer.laglastwarn = killer.lagcount;
	killer.lagwarncount++;;
	if (killer.lagwarncount++ > maxlagwarn) {
	  // drop the player
	  sprintf(message,"You have been kicked due to excessive lag (you have been warned %d times).",
	    maxlagwarn);
	  sendMessage(playerIndex, killer.id, killer.team, message);
	  removePlayer(playerIndex);
	}
      }
    }
  }

  char msg[PlayerIdPLen + 4];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, wins);
  buf = nboPackUShort(buf, losses);
  broadcastMessage(MsgScore, sizeof(msg), msg);
  player[playerIndex].wins = wins;
  player[playerIndex].losses = losses;
#ifdef PRINTSCORE
  dumpScore();
#endif

  // see if the player reached the score limit
  if (maxPlayerScore != 0 &&
      player[playerIndex].wins - player[playerIndex].losses >= maxPlayerScore) {
    char msg[PlayerIdPLen + 2];
    void *buf = msg;
    buf = player[playerIndex].id.pack(buf);
    buf = nboPackUShort(buf, uint16_t(NoTeam));
    broadcastMessage(MsgScoreOver, sizeof(msg), msg);
    gameOver = True;
  }
}

static void sendTeleport(int playerIndex, uint16_t from, uint16_t to)
{
  char msg[PlayerIdPLen + 4];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, from);
  buf = nboPackUShort(buf, to);
  broadcastMessage(MsgTeleport, sizeof(msg), msg);
}

static void acquireRadio(int playerIndex, uint16_t flags)
{
  // player wants to grab the radio (only one person per team can have it)
  // ignore request if player wants a radio already in use, or if a rogue
  // player asks for a team broadcast radio, or if the player is dead
  if (player[playerIndex].state != PlayerOnTeamAlive ||
      ((flags & RadioToAll) && broadcastRadio != InvalidPlayer) ||
      (!(flags & RadioToAll) && player[playerIndex].team == RogueTeam) ||
      team[int(player[playerIndex].team)].radio != InvalidPlayer)
    return;
  if (flags & RadioToAll)
    broadcastRadio = playerIndex;
  team[int(player[playerIndex].team)].radio = playerIndex;

  // send MsgAcquireRadio
  char msg[PlayerIdPLen + 2];
  void *buf = msg;
  buf = player[playerIndex].id.pack(buf);
  buf = nboPackUShort(buf, flags);
  broadcastMessage(MsgAcquireRadio, sizeof(msg), msg);
}

static void releaseRadio(int playerIndex)
{
  // player is releasing the radio (allowing a teammate to grab it)
  if (team[int(player[playerIndex].team)].radio != playerIndex)
    return;
  if (broadcastRadio == playerIndex)
    broadcastRadio = InvalidPlayer;
  team[int(player[playerIndex].team)].radio = InvalidPlayer;

  // send MsgReleaseRadio
  char msg[PlayerIdPLen];
  player[playerIndex].id.pack(msg);
  broadcastMessage(MsgReleaseRadio, sizeof(msg), msg);
}

// parse player comands (messages with leading /)
static void parseCommand(const char *message, int t)
{
  // /password command allows player to become operator
  if (strncmp(message + 1,"password ",9) == 0) {
    if (password && strncmp(message + 10, password, strlen(password)) == 0) {
      player[t].Admin = true;
      sendMessage(t, player[t].id, player[t].team, "You are now an administrator!");
    } else {
      sendMessage(t, player[t].id, player[t].team, "Wrong Password!");
    }
  // /flag command allows operator to control flags
  } else if (player[t].Admin && strncmp(message + 1, "flag ",5) == 0) {
    if (strncmp(message + 6, "reset", 5) == 0) {
      for (int i = 0; i < numFlags; i++) {
	// see if someone had grabbed flag.  tell 'em to drop it.
	const int playerIndex = flag[i].player;
	if (playerIndex != -1) {
	  flag[i].player = -1;
	  flag[i].flag.status = FlagNoExist;
	  player[playerIndex].flag = -1;

	  char msg[PlayerIdPLen + 2 + FlagPLen];
	  void *buf = msg;
	  buf = player[playerIndex].id.pack(buf);
	  buf = nboPackUShort(buf, uint16_t(i));
	  buf = flag[i].flag.pack(buf);
	  broadcastMessage(MsgDropFlag, sizeof(msg), msg);
	}
	resetFlag(i);
      }
    } else if (strncmp(message + 6, "up", 2) == 0) {
      for (int i = 0; i < numFlags; i++) {
	if (int(flag[i].flag.id) < int(FirstTeamFlag) ||
	    int(flag[i].flag.id) > int(LastTeamFlag)) {
	  // see if someone had grabbed flag.  tell 'em to drop it.
	  const int playerIndex = flag[i].player;
	  if (playerIndex != -1) {
	    flag[i].player = -1;
	    flag[i].flag.status = FlagNoExist;
	    player[playerIndex].flag = -1;

	    char msg[PlayerIdPLen + 2 + FlagPLen];
	    void *buf = msg;
	    buf = player[playerIndex].id.pack(buf);
	    buf = nboPackUShort(buf, uint16_t(i));
	    buf = flag[i].flag.pack(buf);
	    broadcastMessage(MsgDropFlag, sizeof(msg), msg);
	  }
	  flag[i].flag.status = FlagGoing;
	  if (!flag[i].required)
	    flag[i].flag.id = NullFlag;
	  sendFlagUpdate(i);
	}
      }
    } else if (strncmp(message + 6, "show", 4) == 0) {
      for (int i = 0; i < numFlags; i++) {
	char message[MessageLen]; // FIXME
	sprintf(message, "%d p:%d r:%d g:%d i:%s", i, flag[i].player,
	    flag[i].required, flag[i].grabs, Flag::getAbbreviation(flag[i].flag.id));
	sendMessage(t, player[t].id, player[t].team, message);
      }
    }
  // /kick command allows operator to remove players
  } else if (player[t].Admin && strncmp(message + 1, "kick ", 5) == 0) {
    int i;
    const char *victimname = message + 6;
    for (i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected && strcmp(player[i].callSign, victimname) == 0)
	break;
    if (i < maxPlayers) {
      char kickmessage[MessageLen];
      player[i].toBeKicked = false;
      sprintf(kickmessage,"Your were kicked off the server by %s", player[t].callSign);
      sendMessage(i, player[i].id, player[i].team, kickmessage);
      removePlayer(i);
    } else {
      char errormessage[MessageLen];
      sprintf(errormessage, "player %s not found", victimname);
      sendMessage(t, player[t].id, player[t].team, errormessage);
    }
  }
  // /lagstats gives simple statistics about players' lags
  else if (strncmp(message+1,"lagstats",8) == 0) {
    for (int i = 0; i < maxPlayers; i++)
      if (player[i].fd != NotConnected) {
	char reply[MessageLen];
	sprintf(reply,"%-12s : %4dms (%d)",player[i].callSign,
		int(player[i].lagavg*1000),player[i].lagcount);
	sendMessage(t,player[t].id,player[t].team,reply);
      }
  }
  else {
    sendMessage(t,player[t].id,player[t].team,"unknown command");
  }
}

static void handleCommand(int t, uint16_t code, uint16_t len, void *rawbuf)
{
  void *buf = (void*)((char*)rawbuf + 4);
  patchMessage(player[t].oldId, player[t].id, rawbuf);
  switch (code) {
    // player joining
    case MsgEnter: {
      // data: id, type, team, name, email
      uint16_t type, team;
      buf = player[t].oldId.unpack(buf);
      player[t].id.number = htons(t);
      player[t].id.port = player[t].taddr.sin_port;
      memcpy(&player[t].id.serverHost, &player[t].taddr.sin_addr.s_addr, sizeof(player[t].id.serverHost));
      buf = nboUnpackUShort(buf, type);
      buf = nboUnpackUShort(buf, team);
      player[t].type = PlayerType(type);
      player[t].team = TeamColor(team);
      buf = nboUnpackString(buf, player[t].callSign, CallSignLen);
      buf = nboUnpackString(buf, player[t].email, EmailLen);
      addPlayer(t);
      UMDEBUG("Player %s [%d] has joined\n", player[t].callSign, t);
      break;
    }

    // player closing connection
    case MsgExit:
      // data: <none>
      UMDEBUG("Player %s [%d] has dropped\n", player[t].callSign, t);
      UMDEBUG("REMOVE: DROPPED BY REQUEST\n");
      removePlayer(t);
      break;

    // player requesting new ttl
    case MsgSetTTL: {
      // data: ttl
      uint16_t ttl;
      nboUnpackUShort(buf, ttl);
      if (ttl > (uint16_t)MaximumTTL) ttl = (uint16_t)MaximumTTL;
      if ((int)ttl > playerTTL) {
	char msg[2];
	nboPackUShort(msg, ttl);
	broadcastMessage(MsgSetTTL, sizeof(msg), msg);
	playerTTL = (int)ttl;
      }
      break;
    }

    // player can't use multicast;  we must relay
    case MsgNetworkRelay:
      if (startPlayerPacketRelay(t)) {
	player[t].multicastRelay = True;
	directMessage(t, MsgAccept, 0, NULL);
      }
      else {
	directMessage(t, MsgReject, 0, NULL);
      }
      break;

    // player wants more of world database
    case MsgGetWorld: {
      // data: count (bytes read so far)
      // worlds shouldn't be too big, 64k is plenty (famous last words)
      uint16_t ptr;
      buf = nboUnpackUShort(buf, ptr);
      if (ptr == 0) {
	// update time of day in world database
	const uint32_t epochOffset = (uint32_t)time(NULL);
	void *epochPtr = ((char*)worldDatabase) + 24;
	nboPackUInt(epochPtr, epochOffset);
      }
      sendWorld(t, int(ptr));
      break;
    }

    case MsgQueryGame:
      sendQueryGame(t);
      break;

    case MsgQueryPlayers:
      sendQueryPlayers(t);
      break;

    // player is coming alive
    case MsgAlive: {
      // data: position, forward-vector
      float pos[3], fwd[3];
      buf = nboUnpackFloat(buf, pos[0]);
      buf = nboUnpackFloat(buf, pos[1]);
      buf = nboUnpackFloat(buf, pos[2]);
      buf = nboUnpackFloat(buf, fwd[0]);
      buf = nboUnpackFloat(buf, fwd[1]);
      buf = nboUnpackFloat(buf, fwd[2]);
      playerAlive(t, pos, fwd);
      break;
    }

    // player declaring self destroyed
    case MsgKilled: {
      // data: id of killer, shot id of killer
      PlayerId killer;
      int16_t shot;
      buf = killer.unpack(buf);
      buf = nboUnpackShort(buf, shot);
      playerKilled(t, lookupPlayer(killer), shot);
      break;
    }

    // player requesting to grab flag
    case MsgGrabFlag: {
      // data: flag index
      uint16_t flag;
      buf = nboUnpackUShort(buf, flag);
      grabFlag(t, int(flag));
      break;
    }

    // player requesting to drop flag
    case MsgDropFlag: {
      // data: position of drop
      float pos[3];
      buf = nboUnpackFloat(buf, pos[0]);
      buf = nboUnpackFloat(buf, pos[1]);
      buf = nboUnpackFloat(buf, pos[2]);
      dropFlag(t, pos);
      break;
    }

    // player has captured a flag
    case MsgCaptureFlag: {
      // data: team whose territory flag was brought to
      uint16_t team;
      buf = nboUnpackUShort(buf, team);
      captureFlag(t, TeamColor(team));
      break;
    }

    // shot fired
    case MsgShotBegin:
      // data: firing info
      // special case -- don't unpack firing info cos we just pass it on
      shotFired(t, buf, int(len));
      break;

    // shot ended prematurely
    case MsgShotEnd: {
      // data: shooter id, shot number, reason
      PlayerId sourcePlayer;
      int16_t shot;
      uint16_t reason;
      buf = sourcePlayer.unpack(buf);
      buf = nboUnpackShort(buf, shot);
      buf = nboUnpackUShort(buf, reason);
      shotEnded(sourcePlayer, shot, reason);
      break;
    }

    // player score changed
    case MsgScore: {
      // data: wins, losses
      uint16_t wins, losses;
      buf = nboUnpackUShort(buf, wins);
      buf = nboUnpackUShort(buf, losses);
      scoreChanged(t, wins, losses);
      break;
    }

    // player teleported
    case MsgTeleport: {
      uint16_t from, to;
      buf = nboUnpackUShort(buf, from);
      buf = nboUnpackUShort(buf, to);
      sendTeleport(t, from, to);
      break;
    }

    // player sending a message
    case MsgMessage: {
      // data: target player, target team, message string
      PlayerId targetPlayer;
      uint16_t targetTeam;
      char message[MessageLen];
      buf = targetPlayer.unpack(buf);
      buf = nboUnpackUShort(buf, targetTeam);
      buf = nboUnpackString(buf, message, sizeof(message));
      UMDEBUG("Player %s [%d]: %s\n",player[t].callSign, t, message);
      // check for command
      if (message[0] == '/') {
	parseCommand(message, t);
      }
      else
	sendMessage(t, targetPlayer, TeamColor(targetTeam), message);
      break;
    }

    // player wants to grab radio
    case MsgAcquireRadio: {
      // data: audio/video flags
      uint16_t flags;
      buf = nboUnpackUShort(buf, flags);
      acquireRadio(t, flags);
      break;
    }

    // player is releasing radio
    case MsgReleaseRadio:
      // data: <none>
      releaseRadio(t);
      break;

    // player is requesting an additional UDP connection, sending its own UDP port
    case MsgUDPLinkRequest: {
      if (alsoUDP) {
	uint16_t port;
	buf = nboUnpackUShort(buf, port);
	player[t].ulinkup = false;
	createUDPcon(t, port);
      }
      break;
    }

    // player is ready to receive data over UDP connection, sending 0
    case MsgUDPLinkEstablished: {
      uint16_t queueUpdate;
      buf = nboUnpackUShort(buf, queueUpdate);
      OOBQueueUpdate(t, queueUpdate);
      UDEBUG("STATUS: Up UDP CON received\n");
      // enable the downlink
      //player[t].ulinkup = true;
      if (!alsoUDP) {
	fprintf(stderr,"Clients sent MsgUDPLinkEstablished without MsgUDPLinkRequest!\n");
      }
      break;
    }

    // player is sending a Server Control Message not implemented yet
    case MsgServerControl:
      break;

    // player is sending multicast data
    case MsgPlayerUpdate:
    case MsgGMUpdate:
    case MsgAudio:
    case MsgVideo:
      if (player[t].multicastRelay)
	relayPlayerPacket(t, len, rawbuf);
      break;
  }
}

static void terminateServer(int /*sig*/)
{
  bzSignal(SIGINT, SIG_PF(terminateServer));
  bzSignal(SIGTERM, SIG_PF(terminateServer));
  exitCode = 0;
  done = True;
}

static const char *usageString =
"[-a <vel> <rot>] [-b] [-c] [+f {good|<id>}] [-f {bad|<id>}] [-g] "
"[-h] "
"[-i interface] "
"[-j] "
"[-lagdrop <num>] "
"[-lagwarn <time/ms>] "
"[-mp {<count>|[<count>],[<count>],[<count>],[<count>],[<count>]}] "
"[-mps <score>] "
"[-ms <shots>] "
"[-mts <score>] "
"[-p <port>] "
"[-noudp] "
"[-passwd <password>] "
#ifdef PRINTSCORE
"[-printscore] "
#endif
"[-public <server-description>] "
"[-publicaddr <server-hostname>[:<server-port>]] "
"[-publiclist <list-server-url>] "
"[+r] "
"[-r] "
"[-requireudp]"
"[{+s|-s} [<num>]] "
"[-sa] "
"[-st <time>] [-sw <num>] [-synctime] "
"[-t] "
#ifdef TIMELIMIT
"[-time <seconds>] "
#endif
"[-ttl <ttl>] "
"[-srvmsg <text>] "
"[-world <filename>]";


static void printVersion(ostream& out)
{
  out << copyright << endl;

  out << "BZFlag server, version " <<
      (VERSION / 10000000) % 100 << "." <<
      (VERSION / 100000) % 100 <<
      (char)('a' - 1 + (VERSION / 1000) % 100) <<
      VERSION % 1000 <<
      endl;

  out << "  protocol " << ServerVersion[4] << ".";
  if (ServerVersion[5] != '0')
    out << ServerVersion[5];
  out << ServerVersion[6] << (char)tolower(ServerVersion[7]) << endl;
}

static void usage(const char *pname)
{
  printVersion(cerr);
  cerr << "usage: " << pname << " " << usageString << endl;
  cerr << "\t -help: for more info" << endl;
  exit(1);
}

static void extraUsage(const char *pname)
{
  printVersion(cout);
  cout << "usage: " << pname << " " << usageString << endl;
  cout << "\t -a: maximum acceleration settings" << endl;
  cout << "\t -b: randomly oriented buildings" << endl;
  cout << "\t -c: capture-the-flag style game" << endl;
//  cout << "\t -d: increase debugging level" << endl;
  cout << "\t +f: always have flag <id> available" << endl;
  cout << "\t -f: never randomly generate flag <id>" << endl;
  cout << "\t -fb: allow flags on box buildings" << endl;
  cout << "\t -g: serve one game and then exit" << endl;
  cout << "\t -h: use random building heights" << endl;
  cout << "\t -i: listen on <interface>" << endl;
  cout << "\t -j: allow jumping" << endl;
  cout << "\t -mp: maximum players total or per team" << endl;
  cout << "\t -mps: set player score limit on each game" << endl;
  cout << "\t -ms: maximum simultaneous shots per player" << endl;
  cout << "\t -mts: set team score limit on each game" << endl;
  cout << "\t -noudp: never use the new UDP networking" << endl;
  cout << "\t -p: use alternative port (default is " <<
					ServerPort << ")" << endl;
#ifdef PRINTSCORE
  cout << "\t -printscore: write score to stdout whenever it changes" << endl;
#endif
  cout << "\t -public <server-description>" << endl;
  cout << "\t -publicaddr <effective-server-hostname>[:<effective-server-port>]" << endl;
  cout << "\t -publiclist <list-server-url>" << endl;
  cout << "\t -q: don't listen for or respond to pings" << endl;
  cout << "\t +r: all shots ricochet" << endl;
  cout << "\t -r: allow rogue tanks" << endl;
  cout << "\t -requireudp: require clients to use udp" << endl;
  cout << "\t +s: always have <num> super flags (default=16)" << endl;
  cout << "\t -s: allow up to <num> super flags (default=16)" << endl;
  cout << "\t -sa: insert antidote superflags" << endl;
  cout << "\t -srvmsg: specify a <msg> to print upon client login" << endl;
  cout << "\t -st: shake bad flags in <time> seconds" << endl;
  cout << "\t -sw: shake bad flags after <num> wins" << endl;
  cout << "\t -synctime: synchronize time of day on all clients" << endl;
  cout << "\t -t: allow teleporters" << endl;
#ifdef TIMELIMIT
  cout << "\t -time: set time limit on each game" << endl;
#endif
  cout << "\t -ttl: time-to-live for pings (default=" << pingTTL << ")" << endl;
  cout << "\t -world: world file to load" << endl;
  cout << "\t -passwd: specify a <password> for operator commands" << endl;
  cout << "\t -lagwarn: lag warning threshhold time [ms]" << endl;
  cout << "\t -lagdrop: drop player after this many lag warnings" << endl;
  cout << "\nFlag codes:" << endl;
  for (int f = int(FirstSuperFlag); f <= int(LastSuperFlag); f++)
    cout << "\t " << setw(2) << Flag::getAbbreviation(FlagId(f)) <<
	" " << setw(0) << Flag::getName(FlagId(f)) << endl;
  exit(0);
}

static int lookupFlag(const char *code)
{
  int f = atoi(code);

  if (f == 0)
    for (f = int(FirstSuperFlag); f <= int(LastSuperFlag); f++)
      if (strcasecmp(code, Flag::getAbbreviation(FlagId(f))) == 0)
	break;
  if (f < int(FirstSuperFlag) || f > int(LastSuperFlag))
    f = int(NoFlag);
  return f;
}

static boolean parsePlayerCount(const char *argv)
{
  // either a single number or 5 optional numbers separated by 4
  // (mandatory) commas.
  const char *scan = argv;
  while (*scan && *scan != ',') scan++;
  if (*scan == ',') {
    // okay, it's the comma separated list.  count commas
    int commaCount = 1;
    while (*++scan)
      if (*scan == ',')
	commaCount++;
    if (commaCount != 4) {
      cerr << "improper player count list" << endl;
      return False;
    }

    // reset the counts
    int i;
    // no limits by default
    for (i = 0; i < NumTeams; i++)
      maxTeam[i] = MaxPlayers;

    // now get the new counts

    // number of counts given
    int countCount = 0;
    scan = argv;
    for (i = 0; i < NumTeams; i++) {
      char *tail;
      long count = strtol(scan, &tail, 10);
      if (tail != scan) {
	// got a number
	countCount++;
	if (count < 0)
	  maxTeam[i] = 0;
	else
	  if (count > MaxPlayers)
	    maxTeam[i] = MaxPlayers;
	else
	  maxTeam[i] = uint16_t(count);
      }
      while (*tail && *tail != ',') tail++;
      scan = tail + 1;
    }

    // if rogues not allowed then zero that count
    if (!(gameStyle & int(RoguesGameStyle)))
      maxTeam[RogueTeam] = 0;

    // if all counts explicitly listed then add 'em up and set maxPlayers
    if (countCount == NumTeams) {
      maxPlayers = 0;
      for (i = 0; i < NumTeams; i++)
	maxPlayers += maxTeam[i];
    }
  }
  else {
    char *tail;
    long count = strtol(argv, &tail, 10);
    if (argv == tail) {
      cerr << "improper player count" << endl;
      return False;
    }
    if (count < 1)
      maxPlayers = 1;
    else
      if (count > MaxPlayers)
	maxPlayers = MaxPlayers;
    else maxPlayers = uint16_t(count);
  }
  return True;
}

static boolean setRequiredFlag(FlagInfo& flag, FlagId id)
{
  flag.required = True;
  flag.flag.id = id;
  flag.flag.type = Flag::getType(id);
  return True;
}

static void parse(int argc, char **argv)
{
  // initialize state
  gameStyle = PlainGameStyle;
  randomBoxes = False;
  randomHeights = False;
  useTeleporters = False;
  oneGameOnly = False;
  numExtraFlags = 0;
  maxShots = 1;
  linearAcceleration = 0.0f;
  angularAcceleration = 0.0f;
  numAllowedFlags = 0;
  requireUDP = false;
  alsoUDP = true;
  delete[] flag;  flag = NULL;
  delete[] allowedFlags;  allowedFlags = NULL;

  // prepare flag counts
  int f, i, flagCount[LastFlag + 1];
  boolean flagDisallowed[LastFlag + 1];
  for (i = int(FirstFlag); i <= int(LastFlag); i++) {
    flagCount[i] = 0;
    flagDisallowed[i] = False;
  }
  boolean allFlagsOut = False;

  // prepare team max counts
  // no limits by default
  for (i = 0; i < NumTeams; i++)
    maxTeam[i] = MaxPlayers;

  // parse command line
  int playerCountArg = 0;
  for (i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-noudp") == 0) {
	UMDEBUG("Setup: Server will use only TCP for connections\n");
	alsoUDP = false;
      } else
      if (strcmp(argv[i], "-requireudp") == 0) {
	UMDEBUG("Setup: Server requires (UDP) clients!\n");
	requireUDP = true;
      } else
      if (strcmp(argv[i], "-srvmsg") == 0) {
	 if (++i == argc) {
	   cerr << "argument expected for -srvmsg" << endl;
	   usage(argv[0]);
	 }
	 servermsg = argv[i];
      } else
      if (strcmp(argv[i], "-world") == 0) {
	 if (++i == argc) {
	   cerr << "argument expected for -world" << endl;
	   usage(argv[0]);
	 }
	 worldFile = argv[i];
      }
      else if (strcmp(argv[i], "+f") == 0) {
      // add required flag
      if (++i == argc) {
	cerr << "argument expected for +f" << endl;
	usage(argv[0]);
      }
      if (strcmp(argv[i], "good") == 0) {
	flagCount[VelocityFlag]++;
	flagCount[QuickTurnFlag]++;
	flagCount[OscOverthrusterFlag]++;
	flagCount[RapidFireFlag]++;
	flagCount[MachineGunFlag]++;
	flagCount[GuidedMissileFlag]++;
	flagCount[LaserFlag]++;
	flagCount[RicochetFlag]++;
	flagCount[SuperBulletFlag]++;
	flagCount[InvisibleBulletFlag]++;
	flagCount[StealthFlag]++;
	flagCount[TinyFlag]++;
	flagCount[NarrowFlag]++;
	flagCount[ShieldFlag]++;
	flagCount[SteamrollerFlag]++;
	flagCount[ShockWaveFlag]++;
	flagCount[PhantomZoneFlag]++;
	flagCount[GenocideFlag]++;
	flagCount[JumpingFlag]++;
	flagCount[IdentifyFlag]++;
	flagCount[CloakingFlag]++;
      }
      else {
	if ((f = lookupFlag(argv[i])) == int(NoFlag)) {
	  cerr << "invalid flag `" << argv[i] << "'" << endl;
	  usage(argv[0]);
	}
	flagCount[f]++;
      }
    }
    else if (strcmp(argv[i], "+r") == 0) {
      // all shots ricochet style
      gameStyle |= int(RicochetGameStyle);
    }
    else if (strcmp(argv[i], "+s") == 0) {
      // set required number of random flags
      if (i+1 < argc && isdigit(argv[i+1][0])) {
	++i;
	if ((numExtraFlags = atoi(argv[i])) == 0)
	  numExtraFlags = 16;
      }
      else {
	numExtraFlags = 16;
      }
      allFlagsOut = True;
    }
    else if (strcmp(argv[i], "-a") == 0) {
      // momentum settings
      if (i + 2 >= argc) {
	cerr << "two arguments expected for " << argv[i] << endl;
	usage(argv[0]);
      }
      linearAcceleration = (float)atof(argv[++i]);
      angularAcceleration = (float)atof(argv[++i]);
      if (linearAcceleration < 0.0f)
	linearAcceleration = 0.0f;
      if (angularAcceleration < 0.0f)
	angularAcceleration = 0.0f;
      gameStyle |= int(InertiaGameStyle);
    }
    else if (strcmp(argv[i], "-b") == 0) {
      // random rotation to boxes in capture-the-flag game
      randomBoxes = True;
    }
    else if (strcmp(argv[i], "-c") == 0) {
      // capture the flag style
      gameStyle |= int(TeamFlagGameStyle);
    }
    else if (strncmp(argv[i], "-d", 2) == 0) {
      // increase debug level
      int count = 0;
      char *scan;
      for (scan = argv[i]+1; *scan == 'd'; scan++) count++;
      if (*scan != '\0') {
	cerr << "bad argument " << argv[i] << endl;
	usage(argv[0]);
      }
      debug += count;
    }
    else if (strcmp(argv[i], "-fb") == 0) {
      // flags on buildings
      flagsOnBuildings = True;
    }
    else if (strcmp(argv[i], "-f") == 0) {
      // disallow given flag
      if (++i == argc) {
	cerr << "argument expected for -f" << endl;
	usage(argv[0]);
      }
      if (strcmp(argv[i], "bad") == 0) {
	flagDisallowed[ColorblindnessFlag] = True;
	flagDisallowed[ObesityFlag] = True;
	flagDisallowed[LeftTurnOnlyFlag] = True;
	flagDisallowed[RightTurnOnlyFlag] = True;
	flagDisallowed[MomentumFlag] = True;
	flagDisallowed[BlindnessFlag] = True;
	flagDisallowed[JammingFlag] = True;
	flagDisallowed[WideAngleFlag] = True;
      }
      else {
	if ((f = lookupFlag(argv[i])) == int(NoFlag)) {
	  cerr << "invalid flag `" << argv[i] << "'" << endl;
	  usage(argv[0]);
	}
	flagDisallowed[f] = True;
      }
    }
    else if (strcmp(argv[i], "-g") == 0) {
      oneGameOnly = True;
    }
    else if (strcmp(argv[i], "-h") == 0) {
      randomHeights = True;
    }
    else if (strcmp(argv[i], "-help") == 0) {
      extraUsage(argv[0]);
    }
    else if (strcmp(argv[i], "-i") == 0) {
      // use a different interface
      if (++i == argc) {
	cerr << "argument expected for -i" << endl;
	usage(argv[0]);
      }
      pingInterface = argv[i];
    }
    else if (strcmp(argv[i], "-j") == 0) {
      // allow jumping
      gameStyle |= int(JumpingGameStyle);
    }
    else if (strcmp(argv[i], "-mp") == 0) {
      // set maximum number of players
      if (++i == argc) {
	cerr << "argument expected for -mp" << endl;
	usage(argv[0]);
      }
      playerCountArg = i;
    }
    else if (strcmp(argv[i], "-ms") == 0) {
      // set maximum number of shots
      if (++i == argc) {
	cerr << "argument expected for -ms" << endl;
	usage(argv[0]);
      }
      int newMaxShots = atoi(argv[i]);
      if (newMaxShots < 1) {
	cerr << "using minimum number of shots of 1" << endl;
	maxShots = 1;
      }
      else if (newMaxShots > MaxShots) {
	cerr << "using maximum number of shots of " << MaxShots << endl;
	maxShots = uint16_t(MaxShots);
      }
      else maxShots = uint16_t(newMaxShots);
    }
    else if (strcmp(argv[i], "-mps") == 0) {
      // set maximum player score
      if (++i == argc) {
	cerr << "argument expected for -mps" << endl;
	usage(argv[0]);
      }
      maxPlayerScore = atoi(argv[i]);
      if (maxPlayerScore < 1) {
	cerr << "disabling player score limit" << endl;
	maxPlayerScore = 0;
      }
    }
    else if (strcmp(argv[i], "-mts") == 0) {
      // set maximum team score
      if (++i == argc) {
	cerr << "argument expected for -mts" << endl;
	usage(argv[0]);
      }
      maxTeamScore = atoi(argv[i]);
      if (maxTeamScore < 1) {
	cerr << "disabling team score limit" << endl;
	maxTeamScore = 0;
      }
    }
    else if (strcmp(argv[i], "-p") == 0) {
      // use a different port
      if (++i == argc) {
	cerr << "argument expected for -p" << endl;
	usage(argv[0]);
      }
      wksPort = atoi(argv[i]);
      if (wksPort < 1 || wksPort > 65535)
	wksPort = ServerPort;
      else
	useGivenPort = True;
    }
    else if (strcmp(argv[i], "-pf") == 0) {
      // try wksPort first and if we can't open that port then
      // let system assign a port for us.
      useFallbackPort = True;
    }
#ifdef PRINTSCORE
    else if (strcmp(argv[i], "-printscore") == 0) {
      // dump score whenever it changes
      printScore = True;
    }
#endif
    else if (strcmp(argv[i], "-public") == 0) {
      if (++i == argc) {
	cerr << "argument expected for -public" << endl;
	usage(argv[0]);
      }
      publicizeServer = True;
      publicizedTitle = argv[i];
      if (strlen(publicizedTitle) > 127) {
	argv[i][127] = '\0';
	cerr << "description too long... truncated" << endl;
      }
    }
    else if (strcmp(argv[i], "-publicaddr") == 0) {
      if (++i == argc) {
	cerr << "argument expected for -publicaddr" << endl;
	usage(argv[0]);
      }
      publicizedAddress = argv[i];
      publicizedAddressGiven = True;
    }
    else if (strcmp(argv[i], "-publiclist") == 0) {
      if (++i == argc) {
	cerr << "argument expected for -publiclist" << endl;
	usage(argv[0]);
      }
      listServerURL = argv[i];
    }
    else if (strcmp(argv[i], "-q") == 0) {
      // don't handle pings
      handlePings = False;
    }
    else if (strcmp(argv[i], "-r") == 0) {
      // allow rogues
      gameStyle |= int(RoguesGameStyle);
    }
    else if (strcmp(argv[i], "-s") == 0) {
      // allow up to given number of random flags
      if (i+1 < argc && isdigit(argv[i+1][0])) {
	++i;
	if ((numExtraFlags = atoi(argv[i])) == 0)
	  numExtraFlags = 16;
      }
      else {
	numExtraFlags = 16;
      }
      allFlagsOut = False;
    }
    else if (strcmp(argv[i], "-sa") == 0) {
      // insert antidote flags
      gameStyle |= int(AntidoteGameStyle);
    }
    else if (strcmp(argv[i], "-st") == 0) {
      // set shake timeout
      if (++i == argc) {
	cerr << "argument expected for -st" << endl;
	usage(argv[0]);
      }
      float timeout = (float)atof(argv[i]);
      if (timeout < 0.1f) {
	shakeTimeout = 1;
	cerr << "using minimum shake timeout of " << 0.1f * (float)shakeTimeout << endl;
      }
      else if (timeout > 300.0f) {
	shakeTimeout = 3000;
	cerr << "using maximum shake timeout of " << 0.1f * (float)shakeTimeout << endl;
      }
      else {
	shakeTimeout = uint16_t(timeout * 10.0f + 0.5f);
      }
      gameStyle |= int(ShakableGameStyle);
    }
    else if (strcmp(argv[i], "-sw") == 0) {
      // set shake win count
      if (++i == argc) {
	cerr << "argument expected for -sw" << endl;
	usage(argv[0]);
      }
      int count = atoi(argv[i]);
      if (count < 1) {
	shakeWins = 1;
	cerr << "using minimum shake win count of " << shakeWins << endl;
      }
      else if (count > 20) {
	shakeWins = 20;
	cerr << "using maximum ttl of " << shakeWins << endl;
      }
      else {
	shakeWins = uint16_t(count);
      }
      gameStyle |= int(ShakableGameStyle);
    }
    else if (strcmp(argv[i], "-synctime") == 0) {
      // client clocks should be synchronized to server clock
      gameStyle |= int(TimeSyncGameStyle);
    }
    else if (strcmp(argv[i], "-t") == 0) {
      // allow teleporters
      useTeleporters = True;
    }
#ifdef TIMELIMIT
    else if (strcmp(argv[i], "-time") == 0) {
      // allow teleporters
      if (++i == argc) {
	cerr << "argument expected for -time" << endl;
	usage(argv[0]);
      }
      timeLimit = (float)atof(argv[i]);
      if (timeLimit <= 0.0f) {
	timeLimit = 300.0f;
	cerr << "using time limit of " << (int)timeLimit << " seconds" << endl;
      }
      timeElapsed = timeLimit;
    }
#endif
    else if (strcmp(argv[i], "-ttl") == 0) {
      // use a different ttl
      if (++i == argc) {
	cerr << "argument expected for -ttl" << endl;
	usage(argv[0]);
      }
      pingTTL = atoi(argv[i]);
      if (pingTTL < 0) {
	pingTTL = 0;
	cerr << "using minimum ttl of " << pingTTL << endl;
      }
      else if (pingTTL > MaximumTTL) {
	pingTTL = MaximumTTL;
	cerr << "using maximum ttl of " << pingTTL << endl;
      }
    }
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-version") == 0) {
      printVersion(cout);
      exit(0);
    }
    else if (strcmp(argv[i], "-passwd") == 0 || strcmp(argv[i], "-password") == 0) {
      if (++i == argc) {
	cerr << "argument expected for " << argv[i] << endl;
	usage(argv[0]);
      }
      password = argv[i];
    }
    else if (strcmp(argv[i], "-lagwarn") == 0) {
      if (++i == argc) {
	cerr << "argument expected for " << argv[i] << endl;
	usage(argv[0]);
      }
      lagwarnthresh = atoi(argv[i])/1000.0f;
    }
    else if (strcmp(argv[i], "-lagdrop") == 0) {
      if (++i == argc) {
	cerr << "argument expected for " << argv[i] << endl;
	usage(argv[0]);
      }
      maxlagwarn = atoi(argv[i]);
    }
    else {
      cerr << "bad argument " << argv[i] << endl;
      usage(argv[0]);
    }
  }

  if (flagsOnBuildings && !(gameStyle & JumpingGameStyle)) {
    cerr << "flags on boxes requires jumping" << endl;
    usage(argv[0]);
  }

  // get player counts.  done after other arguments because we need
  // to ignore counts for rogues if rogues aren't allowed.
  if (playerCountArg > 0 && !parsePlayerCount(argv[playerCountArg]))
    usage(argv[0]);

  // make table of allowed extra flags
  if (numExtraFlags > 0) {
    // first disallow flags inconsistent with game style
    if (gameStyle & InertiaGameStyle)
      flagDisallowed[int(MomentumFlag)] = True;
    if (gameStyle & JumpingGameStyle)
      flagDisallowed[int(JumpingFlag)] = True;
    if (gameStyle & RicochetGameStyle)
      flagDisallowed[int(RicochetFlag)] = True;
    if (!useTeleporters)
      flagDisallowed[int(PhantomZoneFlag)] = True;

    // now count how many aren't disallowed
    for (i = int(FirstSuperFlag); i <= int(LastSuperFlag); i++)
      if (!flagDisallowed[i])
	numAllowedFlags++;

    // if none allowed then no extra flags either
    if (numAllowedFlags == 0) {
      numExtraFlags = 0;
    }

    // otherwise make table of allowed flags
    else {
      allowedFlags = new FlagId[numAllowedFlags];
      int j = 0;
      for (i = int(FirstSuperFlag); i <= int(LastSuperFlag); i++)
	if (!flagDisallowed[i])
	  allowedFlags[j++] = FlagId(i);
    }
  }

  // allocate space for flags
  numFlags = numExtraFlags;
  // rogues don't get a flag
  if (gameStyle & TeamFlagGameStyle)
    numFlags += NumTeams - 1;
  for (i = int(FirstFlag); i <= int(LastFlag); i++)
    numFlags += flagCount[i];
  flag = new FlagInfo[numFlags];

  // prep flags
  for (i = 0; i < numFlags; i++) {
    flag[i].flag.id = NullFlag;
    flag[i].flag.status = FlagNoExist;
    flag[i].flag.type = FlagNormal;
    flag[i].flag.owner.serverHost = Address();
    flag[i].flag.owner.port = 0;
    flag[i].flag.owner.number = 0;
    flag[i].flag.position[0] = 0.0f;
    flag[i].flag.position[1] = 0.0f;
    flag[i].flag.position[2] = 0.0f;
    flag[i].flag.launchPosition[0] = 0.0f;
    flag[i].flag.launchPosition[1] = 0.0f;
    flag[i].flag.launchPosition[2] = 0.0f;
    flag[i].flag.landingPosition[0] = 0.0f;
    flag[i].flag.landingPosition[1] = 0.0f;
    flag[i].flag.landingPosition[2] = 0.0f;
    flag[i].flag.flightTime = 0.0f;
    flag[i].flag.flightEnd = 0.0f;
    flag[i].flag.initialVelocity = 0.0f;
    flag[i].player = -1;
    flag[i].grabs = 0;
    flag[i].required = False;
  }
  f = 0;
  if (gameStyle & TeamFlagGameStyle) {
    flag[0].required = True;
    flag[0].flag.id = RedFlag;
    flag[0].flag.type = FlagNormal;
    flag[1].required = True;
    flag[1].flag.id = GreenFlag;
    flag[1].flag.type = FlagNormal;
    flag[2].required = True;
    flag[2].flag.id = BlueFlag;
    flag[2].flag.type = FlagNormal;
    flag[3].required = True;
    flag[3].flag.id = PurpleFlag;
    flag[3].flag.type = FlagNormal;
    f = 4;
  }
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "+f") == 0) {
      i++;
      if (strcmp(argv[i], "good") == 0) {
	if (setRequiredFlag(flag[f], VelocityFlag))
	  f++;
	if (setRequiredFlag(flag[f], QuickTurnFlag))
	  f++;
	if (setRequiredFlag(flag[f], OscOverthrusterFlag))
	  f++;
	if (setRequiredFlag(flag[f], RapidFireFlag))
	  f++;
	if (setRequiredFlag(flag[f], MachineGunFlag))
	  f++;
	if (setRequiredFlag(flag[f], GuidedMissileFlag))
	  f++;
	if (setRequiredFlag(flag[f], LaserFlag))
	  f++;
	if (setRequiredFlag(flag[f], RicochetFlag))
	  f++;
	if (setRequiredFlag(flag[f], SuperBulletFlag))
	  f++;
	if (setRequiredFlag(flag[f], InvisibleBulletFlag))
	  f++;
	if (setRequiredFlag(flag[f], StealthFlag))
	  f++;
	if (setRequiredFlag(flag[f], TinyFlag))
	  f++;
	if (setRequiredFlag(flag[f], NarrowFlag))
	  f++;
	if (setRequiredFlag(flag[f], ShieldFlag))
	  f++;
	if (setRequiredFlag(flag[f], SteamrollerFlag))
	  f++;
	if (setRequiredFlag(flag[f], ShockWaveFlag))
	  f++;
	if (setRequiredFlag(flag[f], PhantomZoneFlag))
	  f++;
	if (setRequiredFlag(flag[f], GenocideFlag))
	  f++;
	if (setRequiredFlag(flag[f], JumpingFlag))
	  f++;
	if (setRequiredFlag(flag[f], IdentifyFlag))
	  f++;
	if (setRequiredFlag(flag[f], CloakingFlag))
	  f++;
      }
      else {
	flag[f].flag.id = FlagId(lookupFlag(argv[i]));
	flag[f].required = True;
	flag[f].flag.type = Flag::getType(flag[f].flag.id);
	f++;
      }
      gameStyle |= int(SuperFlagGameStyle);
    }
  }
  for (; f < numFlags; f++) {
    flag[f].required = allFlagsOut;
    gameStyle |= int(SuperFlagGameStyle);
  }

  // debugging
  if (debug >= 1) {
    // print style
    cerr << "style: " << hex << gameStyle << dec << endl;
    if (gameStyle & int(TeamFlagGameStyle))
      cerr << "  capture the flag" << endl;
    if (gameStyle & int(SuperFlagGameStyle))
      cerr << "  super flags allowed" << endl;
    if (gameStyle & int(RoguesGameStyle))
      cerr << "  rogues allowed" << endl;
    if (gameStyle & int(JumpingGameStyle))
      cerr << "  jumping allowed" << endl;
    if (gameStyle & int(InertiaGameStyle))
      cerr << "  inertia: " << linearAcceleration << "," <<
	  angularAcceleration << endl;
    if (gameStyle & int(RicochetGameStyle))
      cerr << "  all shots ricochet" << endl;
    if (gameStyle & int(ShakableGameStyle))
      cerr << "  shakable bad flags: timeout=" <<
	  0.1f * float(shakeTimeout) <<
	  ", wins=" << shakeWins << endl;
    if (gameStyle & int(AntidoteGameStyle))
      cerr << "  antidote flags" << endl;
  }
}

int main(int argc, char **argv)
{
  int nfound;

  // check time bomb
  if (timeBombBoom()) {
    cerr << "This release expired on " << timeBombString() << "." << endl;
    cerr << "Please upgrade to the latest release." << endl;
    exit(0);
  }

  // print expiration date
  if (timeBombString()) {
    char bombMessage[80];
    cerr << "This release will expire on " << timeBombString() << "." << endl;
    sprintf(bombMessage, "Version %d.%d%c%d",
	(VERSION / 10000000) % 100, (VERSION / 100000) % 100,
	(char)('a' - 1 + (VERSION / 1000) % 100), VERSION % 1000);
    cerr << bombMessage << endl;
  }

  // trap some signals
  // let user kill server
  if (bzSignal(SIGINT, SIG_IGN) != SIG_IGN)
    bzSignal(SIGINT, SIG_PF(terminateServer));
  // ditto
  bzSignal(SIGTERM, SIG_PF(terminateServer));
// no SIGPIPE in Windows
#if !defined(_WIN32)
  // don't die on broken pipe
  bzSignal(SIGPIPE, SIG_IGN);
#endif

  // initialize
#if defined(_WIN32)
  {
    static const int major = 2, minor = 2;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(major, minor), &wsaData)) {
      fprintf(stderr, "Failed to initialize winsock.  Terminating.\n");
      return 1;
    }
    if (LOBYTE(wsaData.wVersion) != major ||
	HIBYTE(wsaData.wVersion) != minor) {
      fprintf(stderr, "Version mismatch in winsock;"
	  "  got %d.%d.  Terminating.\n",
	  (int)LOBYTE(wsaData.wVersion),
	  (int)HIBYTE(wsaData.wVersion));
      WSACleanup();
      return 1;
    }
  }
#endif /* defined(_WIN32) */
  bzfsrand(time(0));

  // parse arguments
  parse(argc, argv);

  if (pingInterface)
    serverAddress = Address::getHostAddress(pingInterface);
// TimR use 0.0.0.0 by default, multicast will need to have a -i specified for now.
//  if (!pingInterface)
//    pingInterface = serverAddress.getHostName();


  // my address to publish.  allow arguments to override (useful for
  // firewalls).  use my official hostname if it appears to be
  // canonicalized, otherwise use my IP in dot notation.
  // set publicized address if not set by arguments
  if (publicizedAddress.isNull()) {
    publicizedAddress = Address::getHostName();
    if (strchr(publicizedAddress, '.') == NULL)
      publicizedAddress = serverAddress.getDotNotation();
    if (wksPort != ServerPort) {
      char portString[20];
      sprintf(portString, ":%d", wksPort);
      publicizedAddress += portString;
    }
  }

  // prep ping reply
  pingReply.serverId.serverHost = serverAddress;
  pingReply.serverId.port = htons(wksPort);
  pingReply.serverId.number = 0;
  pingReply.gameStyle = gameStyle;
  pingReply.maxPlayers = maxPlayers;
  pingReply.maxShots = maxShots;
  pingReply.rogueMax = maxTeam[0];
  pingReply.redMax = maxTeam[1];
  pingReply.greenMax = maxTeam[2];
  pingReply.blueMax = maxTeam[3];
  pingReply.purpleMax = maxTeam[4];
  pingReply.shakeWins = shakeWins;
  pingReply.shakeTimeout = shakeTimeout;
  pingReply.maxTime = (int)timeLimit;
  pingReply.maxPlayerScore = maxPlayerScore;
  pingReply.maxTeamScore = maxTeamScore;

  // start listening and prepare world database
  if (!defineWorld() || !serverStart()) {
#if defined(_WIN32)
    WSACleanup();
#endif /* defined(_WIN32) */
    return 1;
  }
  if (debug >= 2) {
    // print networking info
    cerr << "listening on " <<
	serverAddress.getDotNotation() << ":" << wksPort << endl;
  }

  TimeKeeper lastSuperFlagInsertion = TimeKeeper::getCurrent();
  const float flagExp = -logf(0.5f) / FlagHalfLife;

  int i;
  while (!done) {
    // prepare select set
    fd_set read_set, write_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    for (i = 0; i < maxPlayers; i++) {
      if (player[i].fd != NotConnected) {
	//fprintf(stderr,"fdset fd,read %i %lx\n",player[i].fd,read_set);
	FD_SET(player[i].fd, &read_set);

	if (player[i].outmsgSize > 0)
	  FD_SET(player[i].fd, &write_set);
      }
      if (reconnect[i].accept != NotConnected)
	FD_SET(reconnect[i].accept, &read_set);
      if (reconnect[i].listen != NotConnected)
	FD_SET(reconnect[i].listen, &read_set);
    }
    // always listen for connections
    FD_SET(wksSocket, &read_set);
    if (alsoUDP)
      FD_SET(udpSocket, &read_set);
    // always listen for pings
    if (pingInSocket != -1)
      FD_SET(pingInSocket, &read_set);
    if (pingBcastSocket != -1)
      FD_SET(pingBcastSocket, &read_set);
    // always listen for packets to relay
    if (relayInSocket != -1)
      FD_SET(relayInSocket, &read_set);

    // check for list server socket connected
    for (i = 0; i < listServerLinksCount; i++)
      if (listServerLinks[i].socket != NotConnected)
	FD_SET(listServerLinks[i].socket, &write_set);

    // find timeout when next flag would hit ground
    TimeKeeper tm = TimeKeeper::getCurrent();
    // lets start by waiting 0.25 sec
    float waitTime = 3.0f;
#ifdef TIMELIMIT
    if (timeLimit > 0.0f)
	waitTime = 1.0f;
#endif
    if (numFlagsInAir > 0) {
      for (i = 0; i < numFlags; i++)
	if (flag[i].flag.status != FlagNoExist &&
	    flag[i].flag.status != FlagOnTank &&
	    flag[i].flag.status != FlagOnGround &&
	    flag[i].dropDone - tm < waitTime)
	  waitTime = flag[i].dropDone - tm;
    }

    // minmal waitTime
    if (waitTime < 0.0f)
      waitTime = 0.0f;

    // we have no pending packets
    nfound = 0;

    // wait for communication or for a flag to hit the ground
    struct timeval timeout;
    timeout.tv_sec = long(floorf(waitTime));
    timeout.tv_usec = long(1.0e+6f * (waitTime - floorf(waitTime)));
    nfound = select(maxFileDescriptor+1, (fd_set*)&read_set, (fd_set*)&write_set, 0, &timeout);
    //if (nfound)
    //  fprintf(stderr, "nfound,read,write %i,%08lx,%08lx\n", nfound, read_set, write_set);

    tm = TimeKeeper::getCurrent();

#ifdef TIMELIMIT
    // see if game time ran out
    if (!gameOver && timeLimit > 0.0f) {
      float newTimeElapsed = tm - gameStartTime;
      float timeLeft = timeLimit - newTimeElapsed;
      if (timeLeft <= 0.0f) {
	timeLeft = 0.0f;
	gameOver = True;
      }
      if (timeLeft == 0.0f || newTimeElapsed - timeElapsed >= 30.0f) {
	char msg[2];
	void *buf = msg;
	buf = nboPackUShort(buf, (uint16_t)(int)timeLeft);
	broadcastMessage(MsgTimeUpdate, sizeof(msg), msg);
	timeElapsed = newTimeElapsed;
      }
    }
#endif

    // if any flags were in the air, see if they've landed
    if (numFlagsInAir > 0) {
      for (i = 0; i < numFlags; i++) {
	if (flag[i].flag.status == FlagInAir ||
		flag[i].flag.status == FlagComing) {
	  if (flag[i].dropDone - tm <= 0.0f) {
	    flag[i].flag.status = FlagOnGround;
	    numFlagsInAir--;
	    sendFlagUpdate(i);
	  }
	}
	else if (flag[i].flag.status == FlagGoing) {
	  if (flag[i].dropDone - tm <= 0.0f) {
	    flag[i].flag.status = FlagNoExist;
	    numFlagsInAir--;
	    resetFlag(i);
	  }
	}
      }
    }

    // maybe add a super flag (only if game isn't over)
    if (!gameOver && numExtraFlags > 0) {
      float t = expf(-flagExp * (tm - lastSuperFlagInsertion));
      if ((float)bzfrand() > t) {
	// find an empty slot for an extra flag
	for (i = numFlags - numExtraFlags; i < numFlags; i++)
	  if (flag[i].flag.id == NullFlag)
	    break;
	if (i != numFlags)
	  randomFlag(i);
	lastSuperFlagInsertion = tm;
      }
    }

    // occasionally add ourselves to the list again (in case we were
    // dropped for some reason).
    if (publicizeServer)
      if (tm - listServerLastAddTime > ListServerReAddTime) {
	// if there are no list servers and nobody is playing then
	// try publicizing again because we probably failed to get
	// the list last time we published, and if we don't do it
	// here then unless somebody stumbles onto this server then
	// quits we'll never try publicizing ourself again.
	if (listServerLinksCount == 0) {
	  // count the number of players
	  int i;
	  for (i = 0; i < maxPlayers; i++)
	    if (player[i].fd != NotConnected &&
		player[i].state != PlayerInLimbo)
	      break;

	  // if nobody playing then publicize
	  if (i == maxPlayers)
	    publicize();
	}

	// send add request
	sendMessageToListServer("ADD");
	listServerLastAddTime = tm;
      }

    for (i = 0; i < maxPlayers; i++) {
      // kick any clients that don't speak UDP
      if (requireUDP && player[i].toBeKicked) {
	char message[MessageLen];
	player[i].toBeKicked = false;
	sprintf(message,"Your end is not using UDP, turn on udp");
	sendMessage(i, player[i].id, player[i].team, message);

	sprintf(message,"upgrade your client http://BZFlag.org/ or");
	sendMessage(i, player[i].id, player[i].team, message);

	sprintf(message,"Try another server, Bye!");
	sendMessage(i, player[i].id, player[i].team, message);

	fprintf(stderr, "*** Kicking Player - no UDP [%d]\n",i);
	removePlayer(i);
      }
    }
    // check messages
    if (nfound > 0) {
      //fprintf(stderr, "chkmsg nfound,read,write %i,%08lx,%08lx\n", nfound, read_set, write_set);
      // first check initial contacts
      if (FD_ISSET(wksSocket, &read_set))
	acceptClient();

      // now check pings
      if (pingInSocket != -1 && FD_ISSET(pingInSocket, &read_set))
	respondToPing();
      if (pingBcastSocket != -1 && FD_ISSET(pingBcastSocket, &read_set))
	respondToPing(True);

      // now check multicast for relay
      if (relayInSocket != -1 && FD_ISSET(relayInSocket, &read_set))
	relayPlayerPacket();

      // check for players that were accepted
      for (i = 0; i < maxPlayers; i++) {
	// first check for clients that are reconnecting
	if (reconnect[i].listen != NotConnected &&
		FD_ISSET(reconnect[i].listen, &read_set))
	  addClient(i);

	// now check the initial contact port.  if any activity or
	// we've waited a while, then shut it down
	if (reconnect[i].accept != NotConnected &&
		(FD_ISSET(reconnect[i].accept, &read_set) ||
		tm - reconnect[i].time > DisconnectTimeout))
	  shutdownAcceptClient(i);
      }

      // check for connection to list server
      for (i = 0; i < listServerLinksCount; ++i)
	if (listServerLinks[i].socket != NotConnected &&
	    FD_ISSET(listServerLinks[i].socket, &write_set))
	  sendMessageToListServerForReal(i);

      // check if we have any UDP packets pending
      if (FD_ISSET(udpSocket, &read_set)) {
	int numpackets;
	while (uread(&i, &numpackets) > 0) {
	  // read head
	  uint16_t len, code;
	  void *buf = player[i].udpmsg;
	  buf = nboUnpackUShort(buf, len);
	  buf = nboUnpackUShort(buf, code);

	  // clear out message
	  player[i].udplen = 0;

	  // handle the command for UDP
	  handleCommand(i, code, len, player[i].udpmsg);
	}
      }

      // now check messages from connected players and send queued messages
      for (i = 0; i < maxPlayers; i++) {

	if (player[i].fd != NotConnected && FD_ISSET(player[i].fd, &write_set)) {
	  pflush(i);
	}

	if (player[i].fd != NotConnected && FD_ISSET(player[i].fd, &read_set)) {
	  // read header if we don't have it yet
	  if (player[i].tcplen < 4) {
	    pread(i, 4 - player[i].tcplen);

	    // if header not ready yet then skip the read of the body
	    if (player[i].tcplen < 4)
	      continue;
	  }

	  // read body if we don't have it yet
	  uint16_t len, code;
	  void *buf = player[i].tcpmsg;
	  buf = nboUnpackUShort(buf, len);
	  buf = nboUnpackUShort(buf, code);
	  if (player[i].tcplen < 4 + (int)len) {
	    pread(i, 4 + (int)len - player[i].tcplen);

	    // if body not ready yet then skip the command handling
	    if (player[i].tcplen < 4 + (int)len)
	      continue;
	  }

	  // clear out message
	  player[i].tcplen = 0;

	  // simple ruleset, if player sends a MsgShotBegin over TCP
	  // and player is not using multicast
	  // he/she must not be using the UDP link
	  if (requireUDP && player[i].multicastRelay) {
	    if (code == MsgShotBegin) {
	      player[i].toBeKicked = true;
	    }
	  }

	  // handle the command
	  handleCommand(i, code, len, player[i].tcpmsg);
	}
      }
    }
    else if (nfound < 0) {
      if (getErrno() != EINTR) {
	// test code - do not uncomment, will cause big stuttering
	// sleep(1);
      }
    }
  }

  serverStop();

  // free misc stuff
  delete[] flag;  flag = NULL;
  delete[] allowedFlags;  allowedFlags = NULL;
  delete world;
  delete[] worldDatabase;
#if defined(_WIN32)
  WSACleanup();
#endif /* defined(_WIN32) */

  // done
  return exitCode;
}
