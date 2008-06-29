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
/** 
 * @file BuildZone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a Build Zone class.
 */

#ifndef __BUILDZONE_H__
#define __BUILDZONE_H__

#include "globals.h"
#include "Zone.h"
#include "Mesh.h"

/** 
 * @class BuildZone
 * @brief Class defining a build zone
 * 
 * Current implementation defines an axis-aligned Zone, this will be
 * refactored to the description below:
 *
 * This Zone represents a zone on which a building algorithm is run through. 
 * A build zone can be made out of several meshes obtained from the algorithm 
 * of the generator.
 */
class BuildZone : public Zone {
  /** 
   * Pointer to the vector holding the Mesh es making up this Build zone.
   * Pointer is NULL until run() is called.
   */
  MeshVector* meshes;
public:
  /** Constructor, sets the neccessart data, does nothing except that. */
  BuildZone(Generator* _generator,Coord2D a, Coord2D b, int astep) 
    : Zone(_generator,a,b,astep), meshes(NULL) {};
  /** Runs the generator, results are stored in meshes. */
  virtual void run();
  /** Outputs the generated meshes into the passed Output object. */
  virtual void output(Output& out);
  /** Destructor, frees the allocated meshes. */
  ~BuildZone();
};

#endif /* __BUILDZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
