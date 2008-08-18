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
 * @file BaseZone.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a map zone with a base in it.
 */

#ifndef __BASEZONE_H__
#define __BASEZONE_H__

#include "Zone.h"

/** 
 * @class BaseZone
 * @brief Class defining a map zone with a base in it.
 *
 * This class defines a Zone that has a base on it. BZFlag bases are 
 * rectangles, yet rectangles don't look good because of texture 
 * Hence, this Zone should be passed a square for best results. 
 */
class BaseZone : public Zone {
public:
  /**
   * Constructor, sets all the needed data for generation.
   */
  BaseZone( Generator* _generator, graph::Face* _face, bool _ctfSafe ) 
    : Zone( _generator,_face ), color( 1 ), ctfSafe( _ctfSafe ) {};
  /**
   * Runs the Zone generation. As a BZFlag base is a native BZW object, this
   * method only assigns each base a color.
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
    out.baseZone(
      Vertex( nodes[0]->vector().x, nodes[0]->vector().y, 0.0f ),
      Vertex( nodes[2]->vector().x, nodes[2]->vector().y, 0.0f ),
      color,ctfSafe
    );
    color++;
  }
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

};

#endif /* __BASEZONE_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
