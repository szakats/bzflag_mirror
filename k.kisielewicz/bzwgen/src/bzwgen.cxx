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
 *
 * bzwgen.cxx -- main program file and main plugin file for BZWGen.
 */

#include <fstream>
#include "globals.h"

#ifdef COMPILE_PLUGIN

#include "BZWGeneratorPlugin.h"
BZWGeneratorPlugin BZWGen; 

#else

#include "BZWGeneratorStandalone.h"
BZWGeneratorStandalone BZWGen; 

#endif


#ifdef COMPILE_PLUGIN

BZ_GET_PLUGIN_VERSION

BZF_PLUGIN_CALL int bz_Load ( const char* commandLine )
{
  bz_debugMessage(4,"bzwgen plugin loaded");
  if (strlen(commandLine) > 4) {
    BZWGen.loadConfig(commandLine);
    bz_debugMessage(4,"config file loaded");
  }
  bz_registerEvent(bz_eGetWorldEvent, &BZWGen);
  bz_registerEvent(bz_eWorldFinalized, &BZWGen);
  
  BZWGen.setup();

  return 0;
}

BZF_PLUGIN_CALL int bz_Unload ( void )
{
  bz_debugMessage(4,"bzwgen plugin unloaded");
  return 0;
}
#else

int main (int argc, char* argv[]) {
  if (BZWGen.parseCommandLine(argc,argv)) return 0;
  if (BZWGen.setup()) return 1;
  OutFileStream* outstream = new OutFileStream(BZWGen.outname.c_str());
  BZWGen.generate(outstream);
  delete outstream;
}
#endif

	
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
