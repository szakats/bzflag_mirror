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
/** 
 * @file GridGenerator.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Implements a Generator class that works on a axis-aligned grid.
 */

#ifndef __GRIDGENERATOR_H__
#define __GRIDGENERATOR_H__

#include "Generator.h"
#include "Material.h"
#include "GridMap.h"
#include "globals.h"
#include "Output.h"

/** 
 * @class GridGenerator
 * @brief Abstract class for the map generator. 
 *
 * This class is a Generator class that works on a discreet and axis-aligned
 * grid. The zones for this generator are created from a discreet 2D map
 * that is created by subdivision of the world with axis-aligned rows and
 * collumns representing roads.
 */

class GridGenerator : public Generator {
  /** GridMap -- a discreet 2D grid map */
  GridMap map;
  /** Number of full slices to be done before subdivision */
  int fullslice;
  /** Snap values for the grid */
  int snap;
  /** Number of subdivisions to be done */
  int subdiv;
public:
  /** 
   * Constructor, just runs it's inherited constructor. 
   */
  GridGenerator(RuleSet* _ruleset) : Generator(_ruleset) {};
  /** 
   * Parses options. GridGenerator parses gridsnap, gridsize,
   * subdiv and fullslice options
   */
  void parseOptions(CCommandLineArgs* opt);
  /**
   * Runs the generator. First generates the GridMap, then pushes the calculated 
   * zones, and let's inherited run() handle zone generation.
   */
  void run();
private:
  /**
   * Plots a road from the given point, either horizontally or
   * vertically. Collision states wether the plotting should be
   * stopped if another road is hit, or should it go through the
   * whole map (full slice).
   */
  void plotRoad(int x, int y, bool horiz, bool collision = false);
  /**
   * Performs a random road slice on the map. Full states wether 
   * it should be a full slice or subdivided one, snapmod controls
   * snap value, and horiz states wether it should be horizontal or
   * vertical.
   */
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
