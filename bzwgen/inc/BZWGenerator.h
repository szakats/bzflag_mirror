/* bzflag
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
/** @file BZWGenerator.h
    @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
    @brief Defines an aplication class for BZWGen.
*/

#ifndef __BZWGENERATOR_H__
#define __BZWGENERATOR_H__

#include "globals.h"
#include "RuleSet.h"
#include "commandArgs.h"

// If compiling as a plugin we need the BZFS API.
#if COMPILE_PLUGIN
#include "bzfsAPI.h"
#include "plugin_utils.h"
#endif // COMPILE_PLUGIN

/** @class BZWGenerator
    @brief Main application class.

    Depending on compilation mode (standalone or plugin) may 
    descend from bz_eventHandler. Both versions share as much code as possible. */
class BZWGenerator 
#if COMPILE_PLUGIN
  : public bz_EventHandler 
#endif // COMPILE_PLUGIN
{
  /** Class for command line parsing. Later on should be ifdef'ed probaly */
  CCommandLineArgs  cmd;
  /** Class holding the ruleset directory. */
  COSDir ruledir;
  /** The read and parsed ruleset. */
  RuleSet* ruleset;
  /** Holds the path (or URL) to the texture directory. */
  String texturepath;
  /** This char pointer holds the data generated form the event handler, it is 
      unused when compiling standalone. */
#if COMPILE_PLUGIN
  char* cstr;
  /** Set to true if the world was generated. To prevent the event handler to 
      regenerate the world. Used only in plugin mode. */
  bool worldGenerated;
#else // COMPILE_PLUGIN = 0
  /** Class for command line parsing. Later on should be ifdef'ed probaly */
  CCommandLineArgs  cmd;
#endif // COMPILE_PLUGIN = 0
public:
  /** Standard default constructor, currently does nothing. */
  BZWGenerator() {}
  /** Parses the command line for valid switches. Used only in the standalone 
      compilation of BZWGen.*/
  int parseCommandLine(int argc, char* argv[]);
  /** Parses the rulesets and config files. */
  int setup();
  /** Default destructor, does nothing. */
  ~BZWGenerator() {}
  /** Runs the generator with the current settings. Output goes to outstream 
      using BZW format. */
  void generate(OutStream* outstream);
  /** Loads a config file. Config files are simply sets of parameters that 
      normally would be passed via the command line. */
  void loadConfig(const char* configFile);
  /** Output file name, used only in standalone mode. */
  String outname;

#if COMPILE_PLUGIN
  /** Event handler for the plugin mode. Processes bz_eGetWorldEvent 
      and bz_eWorldFinalized. */
  virtual void process(bz_EventData * eventData);
#endif

private:
  bool getOptionI ( int &val, char* shortName, char* longName );
  bool getOptionS ( String &val, char* shortName, char* longName );
  void printHelp();
  void printHelpCommand ( const char* shortName, const char* longName, const char* description );
};

#endif /* __BZWGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
