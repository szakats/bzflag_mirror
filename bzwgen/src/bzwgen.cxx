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

#include "BZWGenerator.h"
#include <fstream>

int debugLevel = 2;

BZWGenerator BZWGen; 

#if COMPILE_PLUGIN

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

#endif

int main (int argc, char* argv[]) {
  if (BZWGen.parseCommandLine(argc,argv)) return 0;
  if (BZWGen.setup()) return 1;
  std::ofstream* outstream = new std::ofstream(BZWGen.outname.c_str());
  BZWGen.generate(outstream);
  delete outstream;
}

	
// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
