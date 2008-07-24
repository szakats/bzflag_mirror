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
 * @file Generator.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines an abstract Generator class for map generation.
 */
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "globals.h"
#include "RuleSet.h"
#include "Material.h"
#include "commandArgs.h"
#include "Zone.h"
#include "graph/PlanarGraph.h"

/** 
 * @class Generator
 * @brief Abstract class for the map generator. 
 *
 * This class is intended to be derived from. It holds a protected RuleSet
 * and takes care of material handling. The generator also stores zones and 
 * handles them (removal, output and running) but the addition of new zones 
 * is left to the Generator classes that inherit from Generator.
 */
class Generator {
protected:
  /** Size of the world in world units. */
  int size;
  /** Pointer to the loaded RuleSet. */
  RuleSet* ruleset;
  /** A list of all the materials used. */
  MaterialVector mats;
  /** Number of bases on the map. */
  int bases;
  /** Vector holding the zones generated on the given map. */
  ZoneVector zones;
  /** Material ID of the standard road material. */
  int roadid;
  /** Material ID of the standard road crossing material. */
  int roadxid;
  /** CTFSafe flag. */
  bool ctfSafe;
  /** The planar graph of faces for zones. */
  graph::PlanarGraph graph;
public:
  /** 
   * Standard constructor, takes a already loaded RuleSet as
   * it's parameter. The ruleset needs to have MATROAD and 
   * MATROADX defined. 
   */
  Generator( RuleSet* _ruleset ) : ruleset( _ruleset ) {
    roadid  = math::roundToInt( ruleset->getAttr( "MATROAD" ) );
    roadxid = math::roundToInt( ruleset->getAttr( "MATROADX" ) );
  }
  /** 
   * Parse command line or config file options. Virtual so the
   * specific generator can overload it. Generator parses the size,
   * bases and ctfsafe options. 
   */
  virtual void parseOptions( CCommandLineArgs* opt );
  /** 
   * Runs the generator. Calls run on every stored zone. 
   */
  virtual void run( );
  /** 
   * Returns the size of the world. 
   */
  inline int getSize( ) const { 
    return size; 
  }
  /** 
   * Returns the current count of added zones.
   */
  inline int getZoneCount() const { 
    return zones.size(); 
  }
  /** 
   * Adds a zone pointer to the zones handled by this Generator.
   * The generator will dispose of the zone at destruction.
   */
  inline void addZone( Zone* zone ) {
    zones.push_back( zone );
  }
  /** 
   * Returns the loaded RuleSet. 
   */
  inline RuleSet* getRuleSet() { 
    return ruleset; 
  }
  /** 
   * Outputs data. Goes through all the stored zones and outputs them.
   */
  virtual void output( Output& out );
  /** 
   * Returns the requested Material by material ID. 
   */
  Material* getMaterial( int id ) { 
    return &mats[ id ]; 
  }
  /** 
   * Destructor, frees the materials, the zones, but not the ruleset. 
   */
  virtual ~Generator( ) {
    for ( ZoneVectIter itr = zones.begin(); itr!= zones.end(); ++itr ) 
      delete ( *itr );
  }
};

#endif /* __GENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
