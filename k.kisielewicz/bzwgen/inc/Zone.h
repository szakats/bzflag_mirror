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
 * @file Zone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a map zone
 *
 * A map zone is a part of a planar division. Zones have their own generation
 * algorithm (be it scatter, L-Systems, or static mesh). Probably in the future,
 * there will be a way to define a ComplexZone that has it's own Zone division.
 * Currently however it's both the most discreet as well as the most global
 * division of the map. 
 */

#ifndef __ZONE_H__
#define __ZONE_H__

#include "graph/PlanarGraph.h"
#include "globals.h"
#include "Output.h"

// Forward declarations
class Generator;

/** 
 * @class Zone
 * @brief Class defining a map zone
 * 
 * A zone represents a part of the map -- this part can be of arbitrary 
 * shape, represented internally as a pointer to a graph::Face. Zone 
 * itself is a abstract class, because it doesn't hold any information 
 * about generation nor output.
 */
class Zone {
protected:
  /** 
   * Pointer to the Zone's generator.
   */
  Generator* generator;
  /** 
   * Pointer to the zone's graph face.
   */
  graph::Face* face;
public:
  /**
   * Constructor, initializes the necessary data, except that 
   * does nothing. Initialized zones should only store data. All
   * generations of meshes should be done using run().
   */
  Zone( Generator* _generator, graph::Face* _face ) 
    : generator( _generator ), face( _face ) {};
  /**
   * Runs mesh generation, preparing the zone for output. Pure 
   * virtual method to be overridden.
   */
  virtual void run( ) = 0;
  /**
   * Outputs the mesh information to the given Output object.
   * Pure virtual method to be overridden.
   */
  virtual void output( Output& ) = 0;
  /**
   * Virtual destructor to suppress warnings. 
   */
  virtual ~Zone( ) {};
private:
  /** 
   * Blocked default constructor.
   */
  Zone( ) {}
  /** 
   * Blocked copy constructor.
   */
  Zone( const Zone& ) {}
};

/** 
 * Type definition for a vector of zone pointers. 
 */
typedef std::vector< Zone* > ZoneVector;
/** 
 * Type definition for an iterator over a vector of zone pointers. 
 */
typedef ZoneVector::iterator ZoneVectIter;

#endif /* __ZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
