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

#ifndef __GRIDGENERATOR_H__
#define __GRIDGENERATOR_H__

#include <fstream>
#include "Generator.h"
#include "Material.h"
#include "GridMap.h"
#include "globals.h"
#include "Output.h"
#include "RuleSet.h"

class GridGenerator : public Generator {
protected:
  GridInfo gi;
  GridMap map;
  int fullslice;
  int snapX, snapY;
  int subdiv;
public:
  GridGenerator(RuleSet* ruleset);
  void parseOptions(CCommandLineArgs* opt);
  void run();
  void output(Output& out);
  GridInfo getGridInfo() { return gi; }
  ~GridGenerator();
private:
  void plotRoad(int x, int y, bool horiz, int collision = 0);
  void performSlice(bool full, int snapmod, bool horiz);
};

#endif /* __GRIDGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
