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
 * @file BuildZone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a Build Zone class.
 */

#ifndef __BUILDZONE_H__
#define __BUILDZONE_H__

#include "Zone.h"
#include "Mesh.h"

/** 
 * @class BuildZone
 * @brief Class defining a build zone
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
  /** 
   * Constructor, sets the neccessart data, does nothing except that. 
   */
  BuildZone( Generator* _generator, graph::Face* _face ) 
    : Zone( _generator, _face ), meshes( NULL ) {};
  /** 
   * Runs the generator, results are stored in the meshes vector. 
   */
  virtual void run( );
  /** 
   * Outputs the generated meshes into the passed Output object. 
   */
  virtual void output( Output& out );
  /** 
   * Destructor, frees the allocated meshes. 
   */
  ~BuildZone( );
};

#endif /* __BUILDZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
