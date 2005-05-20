/* bzflag
 * Copyright (c) 1993 - 2005 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

// implementation-specific bzflag headers
#include "GameKeeper.h"

#include "TextUtils.h"
#include <string>
#include <map>

// parser for the server commands
void parseServerCommand(const char *message, int dstPlayerId);

class CustomSlashCommandHandaler
{
public:
	virtual ~CustomSlashCommandHandaler(){};
	virtual bool handle ( int playerID, std::string command, std::string message ) = 0;
};

typedef std::map<std::string, CustomSlashCommandHandaler*>	tmCustomSlashCommandMap;

void registerCustomSlashCommand ( std::string command, CustomSlashCommandHandaler* handaler );
void removeCustomSlashCommand ( std::string command );

#endif

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
