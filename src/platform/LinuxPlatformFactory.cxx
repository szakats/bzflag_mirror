/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "LinuxPlatformFactory.h"
#include "LinuxDisplay.h"
#include "XVisual.h"
#include "XWindow.h"
#ifdef HAVE_SDL
#include "SDLMedia.h"
#else
#include "LinuxMedia.h"
#endif

PlatformFactory*	PlatformFactory::getInstance()
{
  if (!instance) instance = new LinuxPlatformFactory;
  return instance;
}

LinuxPlatformFactory::LinuxPlatformFactory()
{
  // do nothing
}

LinuxPlatformFactory::~LinuxPlatformFactory()
{
  // do nothing
}

BzfDisplay*		LinuxPlatformFactory::createDisplay(
				const char* name, const char*)
{
  XDisplay* display = new XDisplay(name, new LinuxDisplayMode);
  if (!display || !display->isValid()) {
    delete display;
    return NULL;
  }
  return display;
}

BzfVisual*		LinuxPlatformFactory::createVisual(
				const BzfDisplay* display)
{
  return new XVisual((const XDisplay*)display);
}

BzfWindow*		LinuxPlatformFactory::createWindow(
				const BzfDisplay* display, BzfVisual* visual)
{
  return new XWindow((const XDisplay*)display, (XVisual*)visual);
}

BzfMedia*		LinuxPlatformFactory::createMedia()
{
#ifdef HAVE_SDL
  return new SDLMedia;
#else
  return new LinuxMedia;
#endif
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

