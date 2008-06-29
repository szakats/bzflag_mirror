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
 * @file BaseZone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a map zone with a base in it.
 */

#ifndef __BASEZONE_H__
#define __BASEZONE_H__

#include "globals.h"
#include "Zone.h"

/** 
 * @class BaseZone
 * @brief Class defining a map zone with a base in it.
 * 
 * Current implementation defines an axis-aligned Zone, this will be
 * refactored to the description below:
 *
 * This class defines a Zone that has a base on it. BZFlag bases are 
 * rectangles, yet rectangles don't look good because of texture 
 * Hence, this Zone should be passed a square for best results. 
 */
class BaseZone : public Zone {
protected:
  /** Color (Team color) of this base. */
  int color;
  /** CTF safe flag */
  bool ctfSafe;
  /** 
   * A static integer to keep the number of bases generated. Used to
   * assign each generated base a unique color.
   */
  static int colorCount;
public:
  /**
   * Constructor, sets all the needed data for generation.
   */
  BaseZone( Generator* _generator, graph::FacePtr _face, bool _ctfSafe ) 
    : Zone( _generator,_face ), ctfSafe( _ctfSafe ), color( 1 ) {};
  /**
   * Runs the Zone generation. As a BZFlag base is a native BZW object, this method 
   * only assignes each base a color.
   */
  virtual void run() {
    color = colorCount;
    colorCount++;
  };
  /** 
   * Outputs the zone to the given Output object. Currently
   * assumes that a quad is passed as the face.
   */
  virtual void output(Output& out) {
    graph::NodeVector nodes = face->getNodes();
    out.baseZone(Vertex(nodes[0]->x,nodes[0]->y,0.0f),Vertex(nodes[2]->x,nodes[2]->y,0.0f),color,ctfSafe);
    color++;
  }
};

#endif /* __BASEZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
