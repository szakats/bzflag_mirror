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
 * @file GridGenerator.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Implements a Generator class that works on a axis-aligned grid.
 */

#ifndef __GRIDGENERATOR_H__
#define __GRIDGENERATOR_H__

#include "Generator.h"
#include "Material.h"
#include "globals.h"
#include "Output.h"

/** 
 * @class GridGenerator
 * @brief Grid based generator class. 
 *
 * This class is a Generator class that works on a discreet and axis-aligned
 * grid. The zones for this generator are created from a discreet 2D map
 * that is created by subdivision of the world with axis-aligned rows and
 * columns representing roads.
 */

class GridGenerator : public Generator {
public:
  /** 
   * Constructor, just runs it's inherited constructor. 
   */
  GridGenerator( RuleSet* _ruleset ) : Generator( _ruleset ), map( NULL ) {};
  /** 
   * Parses options. GridGenerator parses gridsnap, gridsize,
   * subdiv and fullslice options
   */
  void parseOptions( CCommandLineArgs* opt );
  /**
   * Runs the generator. First generates the grid map, then pushes the 
   * calculated zones, and let's inherited run() handle zone generation.
   */
  void run( );
  /**
   * Destructor, frees the allocated map.
   */
  virtual ~GridGenerator( ) {
    delete map;
  }
private:
  /** Type of cell on the grid */
  enum CellType {
    /** None, will be treated as a build cell by default. */
    NONE,
    /** Straight road cell. */
    ROAD,
    /** Crossing road cell. */
    ROADX,
    /** Base cell. */
    BASE
  };
  /** A discreet map node. Contains the cell type and zoneID*/
  struct DiscreetMapNode {
    /** The node's CellType. */
    CellType type;
    /** Zone ID. The index in the zone array. -1 if no zone is assigned. */
    int zone;
  };
  /** GridMap -- a discreet 2D grid map. */
  DiscreetMapNode* map;
  /** Number of full slices to be done before subdivision */
  int fullslice;
  /** Snap values for the grid */
  int snap;
  /** Number of subdivisions to be done */
  int subdiv;
  /** Size of a single grid step in world units. */
  int gridStep;
  /** Size of the grid. */
  int gridSize;
  /**
   * Plots a road from the given point, either horizontally or
   * vertically. Collision states whether the plotting should be
   * stopped if another road is hit, or should it go through the
   * whole map (full slice).
   */
  void plotRoad( int x, int y, bool horiz, bool collision = false );
  /**
   * Performs a random road slice on the map. Full states whether 
   * it should be a full slice or subdivided one, snapmod controls
   * snap value, and horiz states whether it should be horizontal or
   * vertical.
   */
  void performSlice( bool full, int snapmod, bool horiz );
  /**
   * Takes the given coordinates, and creates a zone starting at 
   * those coordinates that is as big as possible (grows it until
   * it reaches another zone boundary).
   */
  void growZone( int x,int y,CellType type );
  /** 
   * Returns the DiscreetMapNode at the given coordinates. Returns 
   * a reference so the node can be easily modified.
   */
  inline DiscreetMapNode& node( int x, int y )	{ 
    return map[ y * gridSize + x ]; 
  }
  /** 
   * Fills the given area (ax,ay)x(bx,by) with given cell type.
   */
  void setAreaType( int ax, int ay, int bx, int by, CellType type ) {
    for ( int x = ax; x < bx; x++ ) 
      for ( int y = ay; y < by; y++ ) 
        node( x, y ).type = type;
  }
  /** 
   * Translates a grid coordinate into a world coordinate
   */
  float worldCoord( int a ) const { 
    return ( float ) ( a - gridSize / 2 ) * gridStep; 
  }
  /**
   * Creates a "fake face" in the Generator graph for a zone of the 
   * given coordinates.
   */
  graph::Face* createFakeFace( int ax, int ay, int bx, int by );
};

#endif /* __GRIDGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
