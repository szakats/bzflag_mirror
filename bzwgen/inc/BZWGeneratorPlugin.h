/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/** 
 * @file BZWGeneratorPlugin.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Plugin version of the BZWGenerator aplication class for BZWGen.
 */

#ifndef __BZWGENERATORPLUGIN_H__
#define __BZWGENERATORPLUGIN_H__

#include "globals.h"
#include "BZWGenerator.h"

// If compiling as a plugin we need the BZFS API.
#include "bzfsAPI.h"
#include "plugin_utils.h"

/** 
 * @class BZWGeneratorPlugin
 * @brief Main application class, plugin version
 *
 * Extension of the BZWGenerator class for Plugin compilation. Hence implements 
 * bz_eventHandler. 
 */
class BZWGeneratorPlugin : public BZWGenerator, public bz_EventHandler 
{
  char* cstr;
  /** 
   * Set to true if the world was generated. To prevent the event handler to 
   * regenerate the world. Used only in plugin mode. 
   */
  bool worldGenerated;
public:
  /** Standard default constructor, currently does nothing. */
  BZWGeneratorPlugin() : BZWGenerator(), worldGenerated(false), cstr(0) { }
  /** 
   * Event handler for the plugin mode. Processes bz_eGetWorldEvent 
   * and bz_eWorldFinalized. 
   */
  virtual void process(bz_EventData * eventData);
};

#endif /* __BZWGENERATORPLUGIN_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
