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
 * @file FloorZone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a Floor (road) Zone class.
 */

#ifndef __FLOORZONE_H__
#define __FLOORZONE_H__

#include "Zone.h"
#include "Mesh.h"

/** 
 * @class FloorZone
 * @brief Class defining a floor (road) zone
 * 
 * This Zone represents a floor part. Floor is characterized by the fact that it
 * has no collision checks (it is made passthrough), and that it constitutes of 
 * a single mesh (actually a single face). 
 */
class FloorZone : public Zone {
  /** 
   * Stepsize of the world -- to be removed.
   */
  int step;
  /** 
   * Holds the passed material ID of the floor.
   */
  int materialID;
  /** 
   * Flag to set wether the floor part is rotated texture-wise. 
   */
  bool rotated;
  /** 
   * The generated floor mesh. Is empty until run() is called.
   */
  Mesh mesh;
public:
  /** 
   * Constructor defining the zone. Only sets the required parameters. 
   */
  FloorZone( Generator* _generator, graph::Face* _face, int _step, int _materialID, bool _rotated ) 
    : Zone( _generator, _face ), step( _step ), materialID( _materialID ), rotated( _rotated ) {};
  /** 
   * Generates the floor mesh, and assigned texture information. 
   * WARNING: currently works ONLY with quad faces. 
   */
  virtual void run( );
  /** 
   * Outputs the generated mesh to the passed Output object. 
   */
  virtual void output( Output& out );
};

#endif /* __FLOORZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
