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

// get our interface
#include "bzflag.h"

/* system headers */
#include <iostream>
#include <assert.h>
#include <ctype.h>
#include <fstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <vector>
#if defined(_WIN32)
#  include <shlobj.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <direct.h>
#else
#  include <pwd.h>
#  include <dirent.h>
#endif /* defined(_WIN32) */

/* common headers */
#include "Address.h"
#include "AnsiCodes.h"
#include "BZDBCache.h"
#include "BundleMgr.h"
#include "BzfMedia.h"
#include "BzfVisual.h"
#include "BzfWindow.h"
#include "BzfDisplay.h"
#include "CommandManager.h"
#include "CommandsStandard.h"
#include "ConfigFileManager.h"
#include "DirectoryNames.h"
#include "ErrorHandler.h"
#include "FileManager.h"
#include "FontManager.h"
#include "GUIOptionsMenu.h"
#include "KeyManager.h"
#include "OSFile.h"
#include "OpenGLGState.h"
#include "ParseColor.h"
#include "PlatformFactory.h"
#include "Protocol.h"
#include "Roaming.h"
#include "ServerListCache.h"
#include "HUDuiServerListCache.h"
#include "StateDatabase.h"
#include "Team.h"
#include "TextUtils.h"
#include "TextureManager.h"
#include "TimeBomb.h"
#include "WordFilter.h"
#include "World.h"
#include "bzfSDL.h"
#include "bzfgl.h"
#include "bzfio.h"
#include "vectors.h"

/* local headers */
#include "ActionBinding.h"
#include "ExportInformation.h"
#include "callbacks.h"
#include "playing.h"
#include "guiplaying.h"
#include "sound.h"
#include "SceneRenderer.h"

// invoke incessant rebuilding for build versioning
#include "version.h"

// defaults for bzdb
#include "defaultBZDB.h"

// client prefrences
#include "clientConfig.h"


const char* argv0;
std::string alternateConfig;
static bool noAudio = false;
struct tm userTime;
bool echoToConsole = false;
bool echoAnsi = false;


#ifdef ROBOT
// ROBOT -- tidy up
int numRobotTanks = 0;
#endif


//
// application initialization
//

// so that Windows can kill the wsa stuff if needed
int bail ( int returnCode )
{
#ifdef _WIN32
  WSACleanup();
#endif
  return returnCode;
}


static void setTeamColor(TeamColor team, const std::string &str)
{
  fvec4 color;
  if (parseColorString(str, color)) {
    color[3] = 1.0f;
    Team::setColors(team, color, Team::getRadarColor(team));
  }
}


static void setRadarColor(TeamColor team, const std::string &str)
{
  fvec4 color;
  if (parseColorString(str, color)) {
    color[3] = 1.0f;
    Team::setColors(team, Team::getTankColor(team), color);
  }
}


static void setVisual(BzfVisual *visual)
{
  // sine qua non
  visual->setLevel(0);
  visual->setDoubleBuffer(true);
  visual->setRGBA(1, 1, 1, 0);

  // always ask for the max depth buffer
  // GL's PFD will return a lower depth
  // if the max is unsuported.
  int depthLevel = 32;
  if (BZDB.isSet("forceDepthBits")) {
    depthLevel = BZDB.evalInt("forceDepthBits");
  }
  visual->setDepth(depthLevel);

  // always ask for at least 1 stencil bit
  int stencilBits = 1;
  if (BZDB.isSet("stencilBits")) {
    stencilBits = BZDB.evalInt("stencilBits");
  }
  visual->setStencil(stencilBits);

  if (BZDB.isTrue("multisample")) {
    visual->setMultisample(4);
  }

#ifdef USE_GL_STEREO
  if (BZDB.isSet("view") && BZDB.get("view") == configViewValues[1]) {
    visual->setStereo(true);
  }
#endif
}


static void usage()
{
  printFatalError("usage: %s [options] [callsign[:password]@]server[:port]\n"
		  "  -badwords <filterfile>\n"
		  "  -configdir <config dir name>\n"
		  "  -d | -debug\n"
		  "  -date <mm/dd/yyyy>\n"
		  "  {-dir | -directory} <data-directory>\n"
		  "  -e | -echo\n"
		  "  -ea | -echoAnsi\n"
		  "  -h | -help | --help\n"
		  "  -latitude <latitude>\n"
		  "  -longitude <longitude>\n"
		  "  -list <list-server-url>\n"
		  "  -nolist\n"
		  "  -locale <locale>\n"
		  "  -m | -mute\n"
		  "  -motd <motd-url>\n"
		  "  -nomotd\n"
		  "  -multisample\n"
#ifdef ROBOT
		  "  -solo <num-robots>\n"
#endif
		  "  -team <red|green|blue|purple|rogue|observer>\n"
		  "  -time <hh:mm:ss>\n"
		  "  -notime\n"
		  "  -v | -version | --version\n"
		  "  -view <normal|stereo|stacked|three|anaglyph|interlaced>\n"
		  "  -window <geometry-spec>\n"
		  "\n"
		  "Exiting.", argv0);
  if (display != NULL) {
    delete display;
    display=NULL;
  }
  exit(1);
}

static void checkArgc(int &i, int argc, const char *option, const char *type = "Missing")
{
  if ((i+1) == argc) {
    printFatalError("%s argument for %s\n", type, option);
    usage();
  }
  i++; // just skip the option argument string
}

static void parse(int argc, char **argv)
{
  // = 9;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-configdir") == 0) {
      checkArgc(i, argc, argv[i]);
      // the setting has already been done in parseConfigName()
    }
    else if ((strcmp(argv[i], "-dir") == 0) ||
	       (strcmp(argv[i], "-directory") == 0)) {
      checkArgc(i, argc, argv[i]);
      if (strlen(argv[i]) == 0)
	BZDB.unset("directory");
      else
	BZDB.set("directory", argv[i]);
    }
    else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "-echo") == 0) {
      echoToConsole = true;
    }
    else if (strcmp(argv[i], "-ea") == 0 || strcmp(argv[i], "-echoAnsi") == 0) {
      echoToConsole = true;
      echoAnsi = true;
    }
    else if (strcmp(argv[i], "-h") == 0 ||
	       strcmp(argv[i], "-help") == 0 ||
	       strcmp(argv[i], "--help") == 0) {
      usage();
    }
    else if (strcmp(argv[i], "-latitude") == 0) {
      checkArgc(i, argc, argv[i]);
      double latitude = atof(argv[i]);
      if (latitude < -90.0 || latitude > 90.0) {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
      BZDB.set("latitude", argv[i]);
    }
    else if (strcmp(argv[i], "-longitude") == 0) {
      checkArgc(i, argc, argv[i]);
      double longitude = atof(argv[i]);
      if (longitude < -180.0 || longitude > 180.0) {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
      BZDB.set("longitude", argv[i]);
    }
    else if (strcmp(argv[i], "-list") == 0) {
      checkArgc(i, argc, argv[i]);
      if (strcmp(argv[i], "default") == 0) {
	BZDB.set("list", BZDB.getDefault("list"));
      } else {
	startupInfo.listServerURL = argv[i];
	BZDB.set("list", argv[i]);
      }
    }
    else if (strcmp(argv[i], "-locale") == 0) {
      checkArgc(i, argc, argv[i]);
      BZDB.set("locale", argv[i]);
    }
    else if (strcmp(argv[i], "-motd") == 0) {
      checkArgc(i, argc, argv[i]);
      if (strcmp(argv[i], "default") == 0)
	BZDB.set("motdServer", BZDB.getDefault("motdServer"));
      else
	BZDB.set("motdServer", argv[i]);

      BZDB.unset("disableMOTD");
    }
    else if (strcmp(argv[i], "-nomotd") == 0) {
      BZDB.set("disableMOTD", "1");
    }
    else if (strcmp(argv[i], "-nolist") == 0) {
      startupInfo.listServerURL = "";
      BZDB.set("list", "");
    }
    else if (strcmp(argv[i], "-m") == 0 ||
	       strcmp(argv[i], "-mute") == 0) {
      noAudio = true;
    }
    else if (strcmp(argv[i], "-multisample") == 0) {
      BZDB.set("_multisample", "1");
    }
#ifdef ROBOT
    else if (strcmp(argv[i], "-solo") == 0) {
      checkArgc(i, argc, argv[i]);
      numRobotTanks = atoi(argv[i]);
      if (numRobotTanks < 1 || numRobotTanks > MAX_ROBOTS) {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
    }
#endif
    else if (strcmp(argv[i], "-team") == 0) {
      checkArgc(i, argc, argv[i]);
      if ((strcmp(argv[i], "a") == 0) ||
	  (strcmp(argv[i], "auto") == 0) ||
	  (strcmp(argv[i], "automatic") == 0)) {
	startupInfo.team = AutomaticTeam;
      }	else if (strcmp(argv[i], "r") == 0 || strcmp(argv[i], "red") == 0) {
	startupInfo.team = RedTeam;
      } else if (strcmp(argv[i], "g") == 0 || strcmp(argv[i], "green") == 0) {
	startupInfo.team = GreenTeam;
      } else if (strcmp(argv[i], "b") == 0 || strcmp(argv[i], "blue") == 0) {
	startupInfo.team = BlueTeam;
      } else if (strcmp(argv[i], "p") == 0 || strcmp(argv[i], "purple") == 0) {
	startupInfo.team = PurpleTeam;
      } else if (strcmp(argv[i], "z") == 0 || strcmp(argv[i], "rogue") == 0) {
	startupInfo.team = RogueTeam;
      } else if (strcmp(argv[i], "o") == 0 || strcmp(argv[i], "observer") == 0) {
	startupInfo.team = ObserverTeam;
      } else {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
    }
    else if (strcmp(argv[i], "-v") == 0 ||
	       strcmp(argv[i], "-version") == 0 ||
	       strcmp(argv[i], "--version") == 0) {
      printFatalError("BZFlag client %s (protocol %s) http://BZFlag.org/\n%s",
		      getAppVersion(),
		      getProtocolVersion(),
		      bzfcopyright);
      bail(0);
      exit(0);
    }
    else if (strcmp(argv[i], "-window") == 0) {
      BZDB.set("_window", "1");
      if ((i + 1 < argc) && (argv[i + 1][0] != '-')) {
	checkArgc(i, argc, argv[i]);
	int w, h, x, y, count;
	char xs = '+', ys = '+';
	if (strcmp(argv[i], "default") != 0) {
	  if (TextUtils::isNumeric(argv[i][0])) {
	    count = sscanf(argv[i], "%dx%d%c%d%c%d", &w, &h, &xs, &x, &ys, &y);
	    if ((count != 6 && count != 2) ||
		(xs != '-' && xs != '+') ||
		(ys != '-' && ys != '+')) {
	      printFatalError("Invalid argument for %s.\nCorrect format is <width>x<height>[+|-]<x>[+|-]<y>.",argv[i-1]);
	      usage();
	    }
	    BZDB.set("geometry", argv[i]);
	  } else {
	    // roll back, false match
	    i--;
	  }
	} else {
	  BZDB.set("geometry", argv[i]);
	}
      }
    }
    else if (strcmp(argv[i], "-date") == 0) {
      checkArgc(i, argc, argv[i]);
      int month, day, year;
      // FIXME: should use iso yyyy.mm.dd format
      if (sscanf(argv[i], "%d/%d/%d", &month, &day, &year) != 3 ||
	  day < 1 || day > 31 ||		// FIXME -- upper limit loose
	  month < 1 || month > 12 ||
	  (year < 0 || (year > 100 && (year < 1970 || year > 2100)))) {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
      if (year > 100) year = year - 1900;
      else if (year < 70) year += 100;
      userTime.tm_mday = day;
      userTime.tm_mon = month - 1;
      userTime.tm_year = year;
    }
    else if (strcmp(argv[i], "-time") == 0) {
      checkArgc(i, argc, argv[i]);
      BZDB.set("fixedTime", argv[i]);
    }
    else if (strcmp(argv[i], "-notime") == 0) {
      BZDB.unset("fixedTime");
    }
    else if (strcmp(argv[i], "-view") == 0) {
      checkArgc(i, argc, argv[i]);
      BZDB.set("view", argv[i]);
    }
    else if (strcmp(argv[i], "-zbuffer") == 0) {
      checkArgc(i, argc, argv[i]);
      if (strcmp(argv[i], "on") == 0) {
	BZDB.set("zbuffer", "1");
      } else if (strcmp(argv[i], "off") == 0) {
	BZDB.set("zbuffer", "disable");
      } else {
	printFatalError("Invalid argument for %s.", argv[i-1]);
	usage();
      }
    }
    else if (strcmp(argv[i], "-eyesep") == 0) {
      checkArgc(i, argc, argv[i]);
      BZDB.set("eyesep", argv[i]);
    }
    else if (strcmp(argv[i], "-focal") == 0) {
      checkArgc(i, argc, argv[i]);
      BZDB.set("focal", argv[i]);
    }
    else if (strncmp(argv[i], "-psn", 4) == 0) {
      std::vector<std::string> args;
      args.push_back(argv[i]);
      printError("Ignoring Finder argument \"{1}\"", &args);
      // ignore process serial number argument (-psn_x_xxxx for MacOS X
    }
    else if (strcmp(argv[i], "-badwords") == 0) {
      checkArgc(i, argc, argv[i], "Missing bad word filter file");
      BZDB.set("filterFilename", argv[i], StateDatabase::ReadOnly);
    }
    else if (argv[i][0] != '-') {
      if (i == argc-1) {

	// find the beginning of the server name, parse the callsign
	char *serverName;
	if ((serverName = strchr(argv[i], '@')) != NULL) {
	  char *password;
	  *serverName = '\0';
	  if (strlen(argv[i]) >= sizeof(startupInfo.callsign))
	    printFatalError("Callsign truncated.");
	  strncpy(startupInfo.callsign, argv[i],
		  sizeof(startupInfo.callsign) - 1);
	  startupInfo.callsign[sizeof(startupInfo.callsign) - 1] = '\0';
	  if ((password = strchr(startupInfo.callsign, ':')) != NULL) {
	    *(strchr(startupInfo.callsign, ':')) = '\0';
	    *password = '\0', ++password;
	    if (strlen(argv[i]) >= sizeof(startupInfo.password))
	      printFatalError("Password truncated.");
	    strncpy(startupInfo.password, password, sizeof(startupInfo.password) - 1);
	    startupInfo.password[sizeof(startupInfo.password) - 1] = '\0';
	  }
	  ++serverName;
	} else {
	  serverName = argv[i];
	}

	// find the beginning of the port number, parse it
	char *portNumber;
	if ((portNumber = strchr(serverName, ':')) != NULL) {
	  *portNumber = '\0';
	  ++portNumber;
	  startupInfo.serverPort = atoi(portNumber);
	  if (startupInfo.serverPort < 1 || startupInfo.serverPort > 65535) {
	    startupInfo.serverPort = ServerPort;
	    printFatalError("Bad port, using default %d.",
			    startupInfo.serverPort);
	  }
	}
	if (strlen(serverName) >= sizeof(startupInfo.serverName)) {
	  printFatalError("Server name too long.  Ignoring.");
	} else {
	  strncpy(startupInfo.serverName, serverName, ServerNameLen-1);
	  startupInfo.autoConnect = true;
	}
      } else {
	printFatalError("Unexpected: %s. Server must go after all options.", argv[i]);
      }
    }
    else if (strcmp(argv[i], "-debug") == 0) {
      debugLevel++;
    }
    // has to be the last option that starts with -d
    else if (strncmp(argv[i], "-d", 2) == 0) {
      const char num = argv[i][2];
      if ((num >= '0') && (num <= '9') && (argv[i][3] == 0)) {
        debugLevel = num - '0';
      }
      else {
        const char* c = argv[i] + 2;
        while (*c != 0) {
          if (*c != 'd') {
            printFatalError("Unknown option %s.", argv[i]);
            usage();
          }
          c++;
        }
        debugLevel += (int)((c - argv[i]) - 1);
      }
    }
    else {
      printFatalError("Unknown option %s.", argv[i]);
      usage();
    }
  }
}


static void parseConfigName(int argc, char **argv)
{
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-configdir") == 0) {
      checkArgc(i, argc, argv[i]);
      setCustomConfigDir(argv[i]);
      alternateConfig = getCurrentConfigFileName();
    }
  }
}


//
// resource database dumping.  used during initial startup to save
// preferences in case anything catastrophic goes wrong afterwards
// (so user won't have to wait through performance testing again).
//

void dumpResources()
{
  // collect new configuration

  // only dump username and password if we're allowed to, otherwise
  // erase them if they exist
  if (BZDB.eval("saveIdentity") > 0)
    BZDB.set("callsign", startupInfo.callsign);
  else
    BZDB.set("callsign", "");
  if (BZDB.eval("saveIdentity") > 1)
    BZDB.set("password", startupInfo.password);
  else
    BZDB.set("password", "");

  if (BZDB.eval("saveIdentity") > 0)
    BZDB.set("motto", startupInfo.motto.c_str());
  else
    BZDB.set("motto", "");


  BZDB.set("team", Team::getName(startupInfo.team));
  BZDB.set("server", startupInfo.serverName);
  if (startupInfo.serverPort != ServerPort) {
    BZDB.set("port", TextUtils::format("%d", startupInfo.serverPort));
  } else {
    BZDB.unset("port");
  }
  BZDB.set("list", startupInfo.listServerURL);
  if (SOUNDSYSTEM.active())
    BZDB.set("volume", TextUtils::format("%d",(int(SOUNDSYSTEM.getVolume()*10))));

  if (RENDERER.getWindow().getWindow()->hasGammaControl())
    BZDB.set("gamma", TextUtils::format("%f", RENDERER.getWindow().getWindow()->getGamma()));

  BZDB.set("quality", configQualityValues[RENDERER.useQuality()]);
  if (!BZDB.isSet("_window") && display->getResolution() != -1 &&
      display->getResolution(display->getResolution()))
    BZDB.set("resolution", display->getResolution(display->getResolution())->name);

  BZDB.set("panelopacity", TextUtils::format("%f", RENDERER.getPanelOpacity()));

  BZDB.set("radarsize", TextUtils::format("%d", RENDERER.getRadarSize()));

  BZDB.set("mouseboxsize", TextUtils::format("%d", RENDERER.getMaxMotionFactor()));

  // don't save these configurations
  BZDB.setPersistent("_window", false);
  BZDB.setPersistent("_multisample", false);

  const std::vector<std::string> list = getSilenceList();

  // add entries silencedPerson0 silencedPerson1 etc..
  // to the database. Stores silenceList
  // By only allowing up to a certain # of people can prevent
  // the vague chance of buffer overrun.
  const int bufferLength = 30;
  int maxListSize = 1000000; //do even that many play bzflag?
  char buffer [bufferLength];

  if ((int)list.size() < maxListSize) maxListSize = (int)list.size();
  for (int i = 0; i < maxListSize; i++) {
    sprintf(buffer, "silencedPerson%d", i);
    BZDB.set(TextUtils::format("silencedPerson%d", i), list[i]);
  }

  BZDB.set("serverCacheAge", TextUtils::format("%1d", (int)(ServerListCache::get())->getMaxCacheAge()));

  (ServerListCache::get())->saveCache();
  (HUDuiServerListCache::instance()).saveCache();
}


static bool needsFullscreen()
{
  // fullscreen if not in a window
  if (!BZDB.isSet("_window")) return true;

  // not fullscreen if view is default (normal)
  if (!BZDB.isSet("view")) return false;

  // fullscreen if view is not default
  const std::string value = BZDB.get("view");
  for (int i = 1; i < (int)configViewValues.size(); i++) {
    if (value == configViewValues[i]) {
      return true;
    }
  }

  // bogus view, default to normal so no fullscreen
  return false;
}


static void createCacheSignature ()
{
  // This file is to be used by archiving and mirroring tools avoid
  // this directory (and any of its sub-directories). Please see:
  // < http://www.brynosaurus.com/cachedir/ >

  const char cacheSignature[] = "Signature: 8a477f597d28d172789f06886806bc55\n";
  const char cacheComment[] =
    "# This file is a cache directory tag created by BZFlag.\n"
    "# For information about cache directory tags, see:\n"
    "#     http://www.brynosaurus.com/cachedir/\n";
  std::string cacheTagName =  getCacheDirName();
  cacheTagName += "CACHEDIR.TAG";
  std::ostream *cacheTag = FILEMGR.createDataOutStream(cacheTagName, true, true);
  if (cacheTag != NULL) {
    cacheTag->write(cacheSignature, (std::streamsize)strlen(cacheSignature));
    cacheTag->write(cacheComment, (std::streamsize)strlen(cacheComment));
  }
  delete cacheTag;

  return;
}


bool checkTimeBomb ( void )
{
  // check time bomb
  if (timeBombBoom()) {
    printFatalError("This release expired on %s. \n"
		    "Please upgrade to the latest release. \n"
		    "Exiting.", timeBombString());
    bail(0);
    return true;
  }
  return false;
}


void setupBZDB ( void )
{
  // set default DB entries
  for (unsigned int gi = 0; gi < numGlobalDBItems; ++gi) {
    assert(globalDBItems[gi].name != NULL);
    if (globalDBItems[gi].value != NULL) {
      BZDB.set(globalDBItems[gi].name, globalDBItems[gi].value);
      BZDB.setDefault(globalDBItems[gi].name, globalDBItems[gi].value);
    }
    BZDB.setPersistent(globalDBItems[gi].name, globalDBItems[gi].persistent);
    BZDB.setPermission(globalDBItems[gi].name, globalDBItems[gi].permission);
  }

  BZDBCache::init();
}


void setupConfigs ( void )
{
  // read resources
  if (alternateConfig != "") {
    if (CFGMGR.read(alternateConfig))
      startupInfo.hasConfiguration = true;
  }

  if (!startupInfo.hasConfiguration) {
    findConfigFile();
    if (CFGMGR.read(getCurrentConfigFileName())) {
      startupInfo.hasConfiguration = true;
      updateConfigFile();
    }
  }

  if (startupInfo.hasConfiguration)
    ActionBinding::instance().getFromBindings();
  else
    ActionBinding::instance().resetBindings();    // bind default keys

  ServerListCache::get()->loadCache();

  // restore some configuration (command line overrides these)
  if (startupInfo.hasConfiguration) {
    if (BZDB.isSet("callsign")) {
      strncpy(startupInfo.callsign, BZDB.get("callsign").c_str(),
	      sizeof(startupInfo.callsign) - 1);
      startupInfo.callsign[sizeof(startupInfo.callsign) - 1] = '\0';
    }
    if (BZDB.isSet("password")) {
      strncpy(startupInfo.password, BZDB.get("password").c_str(),
	      sizeof(startupInfo.password) - 1);
      startupInfo.password[sizeof(startupInfo.password) - 1] = '\0';
    }
    if (BZDB.isSet("motto")) {
      startupInfo.motto = BZDB.get("motto");
    }
    if (BZDB.isSet("team")) {
      std::string value = BZDB.get("team");
      startupInfo.team = Team::getTeam(value);
    }
    if (BZDB.isSet("server")) {
      strncpy(startupInfo.serverName, BZDB.get("server").c_str(),
	      sizeof(startupInfo.serverName) - 1);
      startupInfo.serverName[sizeof(startupInfo.serverName) - 1] = '\0';
    }
    if (BZDB.isSet("port")) {
      startupInfo.serverPort = BZDB.evalInt("port");
    }

    // check for reassigned team colors
    if (BZDB.isSet("roguecolor"))
      setTeamColor(RogueTeam, BZDB.get("roguecolor"));
    if (BZDB.isSet("redcolor"))
      setTeamColor(RedTeam, BZDB.get("redcolor"));
    if (BZDB.isSet("greencolor"))
      setTeamColor(GreenTeam, BZDB.get("greencolor"));
    if (BZDB.isSet("bluecolor"))
      setTeamColor(BlueTeam, BZDB.get("bluecolor"));
    if (BZDB.isSet("purplecolor"))
      setTeamColor(PurpleTeam, BZDB.get("purplecolor"));

    // check for reassigned radar colors
    if (BZDB.isSet("rogueradar"))
      setRadarColor(RogueTeam, BZDB.get("rogueradar"));
    if (BZDB.isSet("redradar"))
      setRadarColor(RedTeam, BZDB.get("redradar"));
    if (BZDB.isSet("greenradar"))
      setRadarColor(GreenTeam, BZDB.get("greenradar"));
    if (BZDB.isSet("blueradar"))
      setRadarColor(BlueTeam, BZDB.get("blueradar"));
    if (BZDB.isSet("purpleradar"))
      setRadarColor(PurpleTeam, BZDB.get("purpleradar"));

    // ignore window name in config file (it's used internally)
    BZDB.unset("_window");
    BZDB.unset("_multisample");

    // however, if the "__window" setting is enabled, let it through
    if (BZDB.isTrue("__window")) {
      BZDB.setBool("_window", true);
    }
  }
}


void setupXFire(void)
{
#if defined(_WIN32)
  if (!headless) {
    /* write HKEY_CURRENT_USER\Software\BZFlag\CurrentRunningPath with the
     * current path.  this lets Xfire know that this bzflag.exe running from
     * here really is bzflag, not some imposter.
     * since it may be useful to someone else, it's not protected by USE_XFIRE
     */

    // get our path
    char temppath[MAX_PATH], temppath2[MAX_PATH];
    char tempdrive[10];
    GetModuleFileName(NULL, temppath, MAX_PATH);
    // strip filename/extension
    _splitpath(temppath, tempdrive, temppath2, NULL, NULL);
    _makepath(temppath, tempdrive, temppath2, NULL, NULL);

    // write the registry key in question
    HKEY key = NULL;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\BZFlag",
		       0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL,
		       &key, NULL) == ERROR_SUCCESS) {
      RegSetValueEx(key, "CurrentRunningPath", 0, REG_SZ, (LPBYTE)temppath,
		    (DWORD)strlen(temppath));
    }
  }
#endif
}


void checkStartupEnvirons()
{
  if (getenv("BZFLAGID")) {
    BZDB.set("callsign", getenv("BZFLAGID"));
    strncpy(startupInfo.callsign, getenv("BZFLAGID"),
	    sizeof(startupInfo.callsign) - 1);
    startupInfo.callsign[sizeof(startupInfo.callsign) - 1] = '\0';
  } else if (getenv("BZID")) {
    BZDB.set("callsign", getenv("BZID"));
    strncpy(startupInfo.callsign, getenv("BZID"),
	    sizeof(startupInfo.callsign) - 1);
    startupInfo.callsign[sizeof(startupInfo.callsign) - 1] = '\0';
  }
}


void clearWindowsStdOut()
{
#ifdef _WIN32
  // this is cheap but it will work on windows
  // clear out the stdout file
  if (echoToConsole) {
    FILE *fp = fopen ("stdout.txt","w");
    if (fp) {
      fprintf(fp,"stdout started\r\n" );
      fclose(fp);
    }
  }
#endif
}


int initWinsoc ( void )
{
#ifdef _WIN32
  // startup winsock
  static const int major = 2, minor = 2;
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(major, minor), &wsaData)) {
    printFatalError("Failed to initialize winsock.  Terminating.\n");
    return 1;
  }
  if (LOBYTE(wsaData.wVersion) != major ||
      HIBYTE(wsaData.wVersion) != minor) {
    printFatalError("Version mismatch in winsock;"
		    "  got %d.%d, expected %d.%d.  Terminating.\n",
		    (int)LOBYTE(wsaData.wVersion),
		    (int)HIBYTE(wsaData.wVersion),
		    major, minor);
    return bail(1);
  }
#endif
  return 0;
}


int initClient ( int argc, char **argv )
{
  // set the character handling locale
  setlocale(LC_CTYPE,"");

  // init libs
  if (initWinsoc() != 0)
    return 1;

  //init_packetcompression();

  if (checkTimeBomb())
    return 0;

  setupXFire();

  createCacheSignature();

  // initialize global objects and classes
  bzfsrand((unsigned int)time(0));

  /* try to set the processor affinity to prevent TimeKeeper from
   * reporting negative times
   */
  TimeKeeper::setProcessorAffinity();

  setupBZDB();

  Flags::init();

  checkStartupEnvirons();

  time_t timeNow;
  time(&timeNow);
  userTime = *localtime(&timeNow);

  CommandsStandard::add();

  initConfigData();
  loadBZDBDefaults();

  // parse for the config filename
  // the rest of the options are parsed after the config file
  // has been loaded to allow for command line overrides
  parseConfigName(argc, argv);

  setupConfigs();

  // let ExportInformation clients know our version
  ExportInformation &ei = ExportInformation::instance();
  ei.setInformation("Client Version", getAppVersion(), ExportInformation::eitPlayerInfo, ExportInformation::eipStandard);

  // parse arguments
  parse(argc, argv);

  // set the debug level
  BZDB.setInt("debugLevel", debugLevel);

  clearWindowsStdOut();

  if (BZDB.isSet("directory")) {
    //Convert to unix paths so that escaping isn't an issue
    std::string directory = BZDB.get("directory");
    OSFileOSToStdDir(directory);
    BZDB.set("directory", directory);
  }

  if (debugLevel >= 4) {
    BZDB.setDebug(true);
  }

  // set time from BZDB
  if (BZDB.isSet("fixedTime")) {
    int hours, minutes, seconds;
    if (sscanf(BZDB.get("fixedTime").c_str(), "%d:%d:%d", &hours, &minutes, &seconds) != 3 ||
	hours < 0 || hours > 23 ||
	minutes < 0 || minutes > 59 ||
	seconds < 0 || seconds > 59)
      printFatalError("Invalid argument for fixedTime = %s", BZDB.get("fixedTime").c_str());

    userTime.tm_sec = seconds;
    userTime.tm_min = minutes;
    userTime.tm_hour = hours;
  }

  // see if there is a _default_ badwords file
  if (!BZDB.isSet("filterFilename")) {
    std::string name;
    name = getConfigDirName();
    name += "badwords.txt";

    // create a new filter object if one does not exist already
    if (wordFilter == NULL)
      wordFilter = new WordFilter();

    // XXX should stat the file first and load with interactive feedback
    unsigned int count = wordFilter->loadFromFile(name, false);
    if (count > 0)
      std::cout << "Loaded " << count << " words from \"" << name << "\"" << std::endl;

  }

  // load the bad word filter, regardless of a default, if it was set
  if (BZDB.isSet("filterFilename")) {
    std::string filterFilename = BZDB.get("filterFilename");
    std::cout << "Filter file name specified is \"" << filterFilename << "\"" << std::endl;
    if (filterFilename.length() != 0) {
      if (wordFilter == NULL)
	wordFilter = new WordFilter();

      std::cout << "Loading " << filterFilename << std::endl;
      unsigned int count = wordFilter->loadFromFile(filterFilename, true);
      std::cout << "Loaded " << count << " words" << std::endl;
    } else {
      std::cerr << "WARNING: A proper file name was not given for the -badwords argument" << std::endl;
    }
  }

  return 0;
}


void initAudio ( void )
{
  // Change audio driver if requested
  if (BZDB.isSet("audioDriver"))
    PlatformFactory::getMedia()->setDriver(BZDB.get("audioDriver"));
  // Change audio device if requested
  if (BZDB.isSet("audioDevice"))
    PlatformFactory::getMedia()->setDevice(BZDB.get("audioDevice"));

  // set data directory if user specified
  if (BZDB.isSet("directory")) {
    PlatformFactory::getMedia()->setMediaDirectory(BZDB.get("directory"));
  } else {

    // !!! fix this stupid check.. GetMacOSXDataPath() is NULL without app bundle.

#if defined(__APPLE__)
    extern char *GetMacOSXDataPath(void);
    PlatformFactory::getMedia()->setMediaDirectory(GetMacOSXDataPath());
    BZDB.set("directory", GetMacOSXDataPath());
    BZDB.setPersistent("directory", false);
#elif (defined(_WIN32) || defined(WIN32))
    char exePath[MAX_PATH];
    GetModuleFileName(NULL,exePath,MAX_PATH);
    char *last = strrchr(exePath,'\\');
    if (last)
      *last = '\0';
    strcat(exePath,"\\data");
    PlatformFactory::getMedia()->setMediaDirectory(exePath);
#else
    // Only check existence of l10n directory
    DIR *localedir = opendir("data/l10n/");
    if (localedir == NULL)
      PlatformFactory::getMedia()->setMediaDirectory(BZFLAG_DATA);
    else
      closedir(localedir);
#endif
  }
}


// globals
BzfVisual *visual = NULL;
BzfWindow *window = NULL;
BzfJoystick *joystick = NULL;
MainWindow *pmainWindow = NULL;
PlatformFactory *platformFactory = NULL;
BundleMgr *bm = NULL;


int initDisplay ( void )
{
  // make platform factory
  platformFactory = PlatformFactory::getInstance();

  // open display
  display = platformFactory->createDisplay(NULL, NULL);
  if (!display) {
    printFatalError("Can't open display.  Exiting.");
    return bail(1);
  }

  // choose visual
  visual = platformFactory->createVisual(display);
  setVisual(visual);

  // make the window
  window = platformFactory->createWindow(display, visual);
  if (!window->isValid()) {
    printFatalError("Can't create window.  Exiting.");
    return bail(1);
  }
  window->setTitle("bzflag");

  // create & initialize the joystick
  joystick = platformFactory->createJoystick();
  joystick->initJoystick(BZDB.get("joystickname").c_str());
  joystick->setXAxis(BZDB.get("jsXAxis"));
  joystick->setYAxis(BZDB.get("jsYAxis"));

  initAudio();

  // initialize font system
  FontManager &fm = FontManager::instance();
  // load fonts from data directory
  if (fm.loadAll(PlatformFactory::getMedia()->getMediaDirectory() + "/fonts") == 0) {
    printFatalError("No fonts found  (the -directory option may help).  Exiting");
    return bail(1);
  }

  bool setPosition = false, setSize = false;
  int x = 0, y = 0, w = 0, h = 0;

  // set window size (we do it here because the OpenGL context isn't yet bound)

  if (BZDB.isSet("geometry")) {
    char xs, ys;
    const std::string geometry = BZDB.get("geometry");
    const int count = sscanf(geometry.c_str(), "%dx%d%c%d%c%d", &w, &h, &xs, &x, &ys, &y);

    if (geometry == "default" || (count != 6 && count != 2) || w < 0 || h < 0) {
      BZDB.unset("geometry");
    } else if (count == 6 && ((xs != '-' && xs != '+') || (ys != '-' && ys != '+'))) {
      BZDB.unset("geometry");
    } else {
      setSize = true;
      if (w < 256)
	w = 256;
      if (h < 192)
	h = 192;
      if (count == 6) {
	if (xs == '-')
	  x = display->getWidth() - x - w;
	if (ys == '-')
	  y = display->getHeight() - y - h;
	setPosition = true;
      }
      // must call this before setFullscreen() is called
      display->setPassthroughSize(w, h);
    }
  }

  // set window size (we do it here because the OpenGL context isn't yet
  // bound )
  const bool useFullscreen = needsFullscreen();
  if (useFullscreen) {
    // tell window to be fullscreen
    window->setFullscreen(true);

    // set the size if one was requested.  this overrides the default
    // size (which is the display or passthrough size).
    if (setSize)
      window->setSize(w, h);
  } else if (setSize) {
    window->setSize(w, h);
  } else {
    window->setSize(640, 480);
  }
  if (setPosition)
    window->setPosition(x, y);

  // now make the main window wrapper.  this'll cause the OpenGL context
  // to be bound for the first time.
  pmainWindow = new MainWindow(window, joystick);
  if (pmainWindow->isInFault()) {
    printFatalError("Error creating window - Exiting");
    return bail(1);
  }

  std::string videoFormat;
  int format = -1;
  if (BZDB.isSet("resolution")) {
    videoFormat = BZDB.get("resolution");
    if (videoFormat.length() != 0) {
      format = display->findResolution(videoFormat.c_str());
      if (format >= 0) {
	display->setFullScreenFormat(format);
      }
    }
  };
  // set fullscreen again so MainWindow object knows it's full screen
  if (useFullscreen)
    // this will also call window create
    pmainWindow->setFullscreen();
  else
    window->create();


  // set main window's minimum size (arbitrary but should be big enough
  // to see stuff in control panel)
  pmainWindow->setMinSize(256, 192);

  // initialize graphics state
  pmainWindow->getWindow()->makeCurrent();

  // sanity check - make sure OpenGL is actually available or
  // there's no sense in continuing.
  const char* const glRenderer = (const char*)glGetString(GL_RENDERER);
  if (!glRenderer) {
    // bad code, no donut for you

    GLenum error = GL_NO_ERROR;
    while ((error = glGetError()) != GL_NO_ERROR) {
      switch (error) {
      case GL_INVALID_ENUM:
	std::cerr << "ERROR: GL_INVALID_ENUM" << std::endl;
	break;
      case GL_INVALID_VALUE:
	std::cerr << "ERROR: GL_INVALID_VALUE" << std::endl;
	break;
      case GL_INVALID_OPERATION:
	std::cerr << "ERROR: GL_INVALID_OPERATION" << std::endl;
	break;
      case GL_STACK_OVERFLOW:
	std::cerr << "ERROR: GL_STACK_OVERFLOW" << std::endl;
	break;
      case GL_STACK_UNDERFLOW:
	std::cerr << "ERROR: GL_STACK_UNDERFLOW" << std::endl;
	break;
      case GL_OUT_OF_MEMORY:
	std::cerr << "ERROR: GL_OUT_OF_MEMORY" << std::endl;
	break;
      case GL_TABLE_TOO_LARGE:
	std::cerr << "ERROR: GL_TABLE_TOO_LARGE" << std::endl;
	break;
      case GL_NO_ERROR:
	// should not reach
	std::cerr << "ERROR: GL_NO_ERROR" << std::endl;
	break;
      default:
	// should not reach
	std::cerr << "ERROR: UNKNOWN CODE: " << error << std::endl;
      }
    }

    // DIE
    printFatalError("ERROR: Unable to initialize an OpenGL renderer");
    if (display != NULL) {
      delete display;
      display = NULL;
    }
    bail(1);
    exit(1);
  }

  // initialize OpenGL state
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glClearStencil(0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_SCISSOR_TEST);
  //  glEnable(GL_CULL_FACE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  if (!OpenGLGState::haveGLContext()) {
    // DIE
    printFatalError("ERROR: Unable to initialize an OpenGL context");
    if (display != NULL) {
      delete display;
      display=NULL;
    }
    bail(1);
    exit(1);
  }
  OpenGLGState::init();

  // run-time GL version check
  if (!GLEW_VERSION_1_2) {
    // DIE
    printFatalError("ERROR: OpenGL version 1.1 or later is required");
    if (display != NULL) {
      delete display;
      display = NULL;
    }
    bail(1);
    exit(1);
  }

  // add the zbuffer callback here, after the OpenGL context is initialized
  BZDB.addCallback("zbuffer", Callbacks::setDepthBuffer, NULL);

  // set gamma if set in resources and we have gamma control
  if (BZDB.isSet("gamma")) {
    if (pmainWindow->getWindow()->hasGammaControl()) {
      pmainWindow->getWindow()->setGamma(BZDB.eval("gamma"));
    }
  }

  // set the scene renderer's window
  RENDERER.setWindow(pmainWindow);

  // restore rendering configuration
  if (startupInfo.hasConfiguration) {
    if (BZDB.isSet("quality")) {
      std::string value = BZDB.get("quality");
      const int qualityLevels = (int)configQualityValues.size();
      for (int j = 0; j < qualityLevels; j++) {
	if (value == configQualityValues[j]) {
	  RENDERER.setQuality(j);
	  break;
	}
      }
    }

    TextureManager &tm = TextureManager::instance();
    tm.setMaxFilter(BZDB.get("texture"));
    BZDB.set("texture", tm.getMaxFilterName());

    BZDB.set("texturereplace", (!BZDBCache::lighting &&
				RENDERER.useQuality() < _MEDIUM_QUALITY) ? "1" : "0");
    BZDB.setPersistent("texturereplace", false);
    if (BZDB.isSet("view")) {
      RENDERER.setViewType(SceneRenderer::Normal);
      std::string value = BZDB.get("view");
      for (size_t i = 0; i < configViewValues.size(); i++)
	if (value == configViewValues[i]) {
	  RENDERER.setViewType((SceneRenderer::ViewType)i);
	  break;
	}
    }

    if (BZDB.isSet("panelopacity"))
      RENDERER.setPanelOpacity(BZDB.eval("panelopacity"));

    if (BZDB.isSet("radarsize"))
      RENDERER.setRadarSize(BZDB.getIntClamped("radarsize", 0, GUIOptionsMenu::maxRadarSize));

    if (BZDB.isSet("mouseboxsize"))
      RENDERER.setMaxMotionFactor(BZDB.evalInt("mouseboxsize"));
  }

  // grab the mouse only if allowed
  if (BZDB.isSet("mousegrab") && !BZDB.isTrue("mousegrab")) {
    pmainWindow->setNoMouseGrab();
    pmainWindow->enableGrabMouse(false);
  } else {
    pmainWindow->enableGrabMouse(true);
  }

  // set window quadrant
  if (RENDERER.getViewType() == SceneRenderer::ThreeChannel)
    pmainWindow->setQuadrant(MainWindow::UpperRight);
  else if (RENDERER.getViewType() == SceneRenderer::Stacked)
    pmainWindow->setQuadrant(MainWindow::LowerHalf);
#ifndef USE_GL_STEREO
  else if (RENDERER.getViewType() == SceneRenderer::Stereo)
    pmainWindow->setQuadrant(MainWindow::UpperRight);
#endif

  // clear the grid graphics if they are not accessible
#if !defined(DEBUG_RENDERING)
  if (debugLevel <= 0) {
    BZDB.setBool("showCullingGrid",   false);
    BZDB.setBool("showCollisionGrid", false);
  }
#endif

  return 0;
}


int postWindowInit ( void )
{
  // initialize locale system

  bm = new BundleMgr(PlatformFactory::getMedia()->getMediaDirectory(), "bzflag");
  World::setBundleMgr(bm);

  std::string locale = BZDB.isSet("locale") ? BZDB.get("locale") : "default";
  World::setLocale(locale);
  bm->getBundle(World::getLocale());


  // get sound files.  must do this after creating the window because
  // DirectSound is a bonehead API.
  if (!noAudio)
  {
    if (BZDB.isSet("soundsystem")) // set the sound system they want
      SoundManager::instance().activateSoundSystem(BZDB.get("soundsystem"));

    SOUNDSYSTEM.startup();
    if (startupInfo.hasConfiguration && BZDB.isSet("volume"))
      SOUNDSYSTEM.setVolume(static_cast<int>(BZDB.eval("volume"))*0.1f);

    BZDB.set("soundsystem",SOUNDSYSTEM.name()); // save the sound system they get
  }

  // set server list URL
  if (BZDB.isSet("list"))
    startupInfo.listServerURL = BZDB.get("list");

  // setup silence list
  std::vector<std::string> &list = getSilenceList();

  // search for entries silencedPerson0 silencedPerson1 etc..
  // to the database. Stores silenceList
  // By only allowing up to a certain # of people can prevent
  // the vague chance of buffer overrun.
  const int bufferLength = 30;
  const int maxListSize = 1000000; // do even that many play bzflag?
  char buffer [bufferLength];
  bool keepGoing = true;

  for (int s = 0; keepGoing && (s < maxListSize); s++) {
    sprintf(buffer,"silencedPerson%d",s); // could do %-10d

    if (BZDB.isSet(buffer)) {
      list.push_back(BZDB.get(buffer));
      // remove the value from the database so when we save
      // it saves the list's new values in order
      BZDB.unset(buffer);
    } else {
      keepGoing = false;
    }
  }

  if (BZDB.isSet("serverCacheAge")) {
    (ServerListCache::get())->setMaxCacheAge(BZDB.evalInt("serverCacheAge"));
  }

  return 0;
}


void cleanupClient ( void )
{
  // shut down
  if (wordFilter != NULL)
    delete wordFilter;
  wordFilter = NULL;
  if (display)
    display->setDefaultResolution();
  delete pmainWindow;
  delete joystick;
  delete window;
  delete visual;
  SOUNDSYSTEM.shutdown();
  delete display;
  delete platformFactory;
  delete bm;
  Flags::kill();
  FontManager::instance().clear();

#if defined(_WIN32)
  {
    /* clear HKEY_CURRENT_USER\Software\BZFlag\CurrentRunningPath if it exists */
    HKEY key = NULL;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\BZFlag",
		     0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
      RegSetValueEx(key, "CurrentRunningPath", 0, REG_SZ, (LPBYTE)"\0", 1);
    }
  }
#endif

#ifdef _WIN32
  // clean up
  WSACleanup();
  stripAnsiCodes(NULL); // free the stripping buffer
#endif

}

void saveSettings ( void )
{
  // save resources
  if (BZDB.isTrue("saveSettings")) {
    dumpResources();
    if (alternateConfig == "")
      CFGMGR.write(getCurrentConfigFileName());
    else
      CFGMGR.write(alternateConfig);
  }
}

void saveStartupInfo ( void )
{
  std::string conf = getConfigDirName();
  conf += "bzflag.dir";
  FILE *fp = fopen(conf.c_str(),"wt");
  if (fp) {
    std::string exepath = getModuleDir();
    exepath += "\n";
    fwrite(exepath.c_str(),exepath.size(),1,fp);
    exepath = getModuleName();
    exepath += "\n";
    fwrite(exepath.c_str(),exepath.size(),1,fp);
    fclose(fp);
  }
}


//
// main()
//	initialize application and enter event loop
//

#if defined(_WIN32) && !defined(HAVE_SDL)
int myMain(int argc, char **argv)
#else /* defined(_WIN32) */
  int main(int argc, char **argv)
#endif /* defined(_WIN32) */
{
  argv0 = argv[0];

  saveStartupInfo();

  if (initClient(argc,argv)!= 0)
    return -1;

  if (initDisplay()!= 0)
    return -1;

  if (postWindowInit()!= 0)
    return -1;

  // start playing
  startPlaying();

  // cleanup and die
  saveSettings();
  cleanupClient();
  // clean up singletons
  //  delete FILEMGR;
  //  delete CMDMGR;
  //  delete BZDB;

  return 0;
}
//
#if defined(_WIN32) && !defined(HAVE_SDL)


//
// WinMain()
//	windows entry point.  forward to main()
//

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR _cmdLine, int)
{
  // convert command line to argc and argv.  note that it's too late
  // to do this right because spaces that were embedded in a single
  // argument now look like like normal spaces.  not much we can do
  // about that.
  // FIXME -- argc and argv can be accessible;  use them instead of this.
  char *cmdLine = strdup(_cmdLine);

  // count number of arguments
  int argc = 1;
  char *scan = cmdLine;
  scan = const_cast<char*>(TextUtils::skipWhitespace(scan));

  while (*scan) {
    argc++;
    // If we have a double quote (ASCII 34) then read the whole quoted string
    // as one argument
    if (*scan == 34) {
      // Move past the starting double quote
      scan++;
      // Keep going until we reach the end, or we hit another double quote
      // FIXME: Support escaping the double quotes
      while (*scan != 0 && *scan != 34)
	scan++;
      // Move past the ending double quote
      scan++;
    } else {
      // Not a double quote, so just make sure we aren't hitting any spaces.
      scan = const_cast<char*>(TextUtils::skipNonWhitespace(scan));
    }

    // Skip past any spaces
    scan = const_cast<char*>(TextUtils::skipWhitespace(scan));
  }

  // get path to application.  this is ridiculously simple.
  char appName[MAX_PATH];
  GetModuleFileName(instance,appName,MAX_PATH);

  // make argument list and assign arguments
  char **argv = new char *[argc];
  argc = 0;
  argv[argc++] = appName;
  scan = cmdLine;
  scan = const_cast<char*>(TextUtils::skipWhitespace(scan));

  while (*scan) {
    // If we have a double quote (ASCII 34) then read the whole quoted string
    // as one argument
    if (*scan == 34) {
      // Move past the starting double quote
      // Set the double quote to null just in case
      *scan++ = 0;

      argv[argc++] = scan;
      // Keep going until we reach the end, or we hit another double quote
      // FIXME: Support escaping the double quotes
      while (*scan != 0 && *scan != 34)
	scan++;
      // Move past the ending double quote
      // Set it to null so we end this paramater
      if (*scan) *scan++ = 0;
    }
    // Not a double quote, so just make sure we aren't hitting any spaces.
    else {
      argv[argc++] = scan;
      scan = const_cast<char*>(TextUtils::skipNonWhitespace(scan));

      if (*scan) *scan++ = 0;
    }
    // Skip past spaces
    scan = const_cast<char*>(TextUtils::skipWhitespace(scan));
  }

  // stdout access on windows
  // see: http://www.geocities.com/patchnpuki/tips/index.htm#w5
  setbuf(stdout, NULL);

  const int exitCode = myMain(argc, argv);

  // clean up and return exit code
  delete[] argv;
  free(cmdLine);
  return exitCode;
}


#endif /* defined(_WIN32) */


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
