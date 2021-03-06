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

#ifndef __SERVERCOMMANDKEY_H__
#define __SERVERCOMMANDKEY_H__

// Ancestor class
#include "HUDuiDefaultKey.h"

#include <string>
#include "Address.h"

class ServerCommandKey : public HUDuiDefaultKey {
public:
  ServerCommandKey();
  bool		keyPress(const BzfKeyEvent&);
  bool		keyRelease(const BzfKeyEvent&);
  void		init();
  void		adminInit();
  void		nonAdminInit();

  enum Mode {
    Kick,
    Kill,
    BanIp,
    Ban1,
    Ban2,
    Ban3,
    Showgroup,
    Setgroup,
    Removegroup,
    Unban,
    Banlist,
    Playerlist,
    FlagReset,
    FlagUnusedReset,
    FlagUp,
    FlagShow,
    FlagHistory,
    IdleStats,
    ClientQuery,
    LagStats,
    Report,
    LagWarn,
    LagDrop,
    GameOver,
    CountDown,
    SuperKill,
    Shutdown,
    Grouplist,
    Groupperms,
    Vote,
    Poll,
    Veto,
    Password  // leave this as the last item
  };

private:
  std::string		makePattern(const InAddr& address);
  void		updatePrompt();

private:
  Mode mode;
  const int numModes;
};

#endif

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
