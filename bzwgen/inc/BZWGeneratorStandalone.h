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
 * @file BZWGeneratorStandalone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an stanadalone aplication class for BZWGen.
 */

#ifndef __BZWGENERATORSTANDALONE_H__
#define __BZWGENERATORSTANDALONE_H__

#include "globals.h"
#include "BZWGenerator.h"

/** 
 * @class BZWGeneratorStandalone
 * @brief Main application class for standalone compilation.
 *
 * Standalone version of the BZWGenerator class. 
 */
class BZWGeneratorStandalone : public BZWGenerator
{
public:
  /** Standard default constructor, currently does nothing. */
  BZWGeneratorStandalone() : BZWGenerator() {}
  /** 
   * Parses the command line for valid switches. Used only in the standalone 
   * compilation of BZWGen.
   */
  int parseCommandLine(int argc, char* argv[]);
  /** Default destructor, does nothing. */
  ~BZWGeneratorStandalone() {}
  /** Output file name, used only in standalone mode. */
  String outname;
private:
  /** Prints the help screen. */
  void printHelp();
  /** Prints a single command for the help screen. */
  void printHelpCommand ( const char* shortName, const char* longName, const char* description );
};

#endif /* __BZWGENERATORSTANDALONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
