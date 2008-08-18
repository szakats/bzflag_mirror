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

#include "Generator.h"

void Generator::parseOptions(CCommandLineArgs* opt) {
  size = 800;
  bases = 0;
  ctfSafe = false;

  if (opt->Exists("s"))    { size = opt->GetDataI("s"); }
  if (opt->Exists("size")) { size = opt->GetDataI("size"); }

  if (opt->Exists("b"))     { bases = opt->GetDataI("b"); }
  if (opt->Exists("bases")) { bases = opt->GetDataI("bases"); }

  if (opt->Exists("ctfsafe")) { ctfSafe = true; }
}

void Generator::run() {
  Logger.log( 2, "Generator : generating zones..." );
  for (ZoneVectIter itr = zones.begin(); itr!= zones.end(); ++itr) (*itr)->run();
}

void Generator::output(Output& out) {
  out.header(size);

  Logger.log( 2, "Generator : outputing materials..." );
  for (MaterialVectIter iter = mats.begin(); iter != mats.end(); ++iter) (*iter).output(out);
  if (ruleset != NULL) ruleset->output(out);

  Logger.log( 2, "Generator : outputing zones..." );
  for (ZoneVectIter iter = zones.begin(); iter != zones.end(); ++iter)  (*iter)->output(out);
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
