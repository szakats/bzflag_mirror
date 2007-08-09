/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
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

class GridGenerator : public Generator {
protected:
  GridInfo gi;
  MaterialVector mats;
  GridMap map;
public:
  GridGenerator();
  void parseOptions(Options opt);
  void run();
  void output(Output& out);
  ~GridGenerator();
};

#endif /* __GRIDGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
