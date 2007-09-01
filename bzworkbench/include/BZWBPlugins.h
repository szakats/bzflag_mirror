/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef BZWB_PLUGINS_H_
#define BZWB_PLUGINS_H_

#include <string>
#include <vector>
#include "BZWBAPI.h"

#include "windows/MainWindow.h"

// plugins need this, we just need a global to hang this on
extern Fl_Window *the_mainWindow;

void initPlugins ( void );

bool loadPlugin ( std::string plugin, std::string config );
bool unloadPlugin ( std::string plugin );
void unloadPlugins ( void );

bool registerCustomPluginHandler ( std::string extension, bzwb_APIPluginHandler *handler );
bool removeCustomPluginHandler ( std::string extension, bzwb_APIPluginHandler *handler );

std::vector<std::string> getPluginList ( void );

#endif /*BZWB_PLUGINS_H_*/

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

