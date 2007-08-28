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

#include "BZWBAPI.h"
#include "BZWBPlugins.h"
#include "FL/x.h"

// custom pluginHandler
BZWB_API bool bzwb_registerCustomPluginHandler(const char *extension, bzwb_APIPluginHandler *handler)
{
	if(!extension || !handler)
		return false;

	std::string ext=extension;

	return registerCustomPluginHandler(ext, handler);
}

//-------------------------------------------------------------------------

BZWB_API bool bzwb_removeCustomPluginHandler(const char *extension, bzwb_APIPluginHandler *handler)
{
	if(!extension || !handler)
		return false;

	std::string ext=extension;

	return removeCustomPluginHandler(ext, handler);
}

//-------------------------------------------------------------------------

BZWB_API unsigned int bzwb_getOSMainWindowHandle ( void )
{
	if (!the_mainWindow)
		return 0;
#ifdef _WIN32
	return (unsigned int)fl_xid(the_mainWindow);
#endif //_WIN32
	return 0;
}
