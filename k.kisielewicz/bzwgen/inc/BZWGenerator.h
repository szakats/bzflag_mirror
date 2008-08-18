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
 * @file BZWGenerator.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an aplication class for BZWGen.
 */

#ifndef __BZWGENERATOR_H__
#define __BZWGENERATOR_H__

#include "globals.h"
#include "RuleSet.h"
#include "commandArgs.h"

/**
 * @class BZWGenerator
 * @brief Main application class.
 *
 * Depending on compilation mode (standalone or plugin) may
 * descend from bz_eventHandler. Both versions share as much code as possible.
 */
class BZWGenerator
{
  /** Class holding the ruleset directory. */
  COSDir ruledir;
  /** The read and parsed ruleset. */
  RuleSet* ruleset;
  /** Holds the path (or URL) to the texture directory. */
  String texturepath;
  /** Temporary boolean for running FaceGenerator instead of GridGenerator */
  bool experimental;
public:
  /** Standard default constructor, currently does nothing. */
  BZWGenerator() {}
  /** Parses the rulesets and config files. */
  int setup();
  /** Default destructor, does nothing. */
  ~BZWGenerator() {}
  /**
   * Runs the generator with the current settings. Output goes to outstream
   * using BZW format.
   */
  void generate(OutStream* outstream);
  /**
   * Loads a config file. Config files are simply sets of parameters that
   * normally would be passed via the command line.
   */
  void loadConfig( const char* configFile );
  /** Output file name, used only in standalone mode. */
  String outname;
protected:
  /**
   * Class for command line parsing. It is used in both deployments because
   * it also is used as a storage for passed options, and option files.
   */
  CCommandLineArgs cmd;
  /**
   * Retuns an int option based on it's short and long name if defined.
   * If not defined, returns false.
   */
  bool getOptionI ( int &val, const char* shortName, const char* longName );
  /**
   * Retuns an string option based on it's short and long name if defined.
   * If not defined, returns false.
   */
  bool getOptionS ( String &val, const char* shortName, const char* longName );
};

#endif /* __BZWGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
