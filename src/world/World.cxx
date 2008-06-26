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

#include "BZW/World.h"

namespace BZW
{

  /// Default constructor
  World::World()
  {

  }

  /// Read/Create a world from a stream
  World::World(std::istream &input)
  {
    read(in);
  }

  /// Destructor
  World::~World()
  {

  }

  /// Read a world from stream
  void World::read(std::istream &input)
  {
    /*
    Parser parser;
    Parser::Object box(true);
    Parser::Parameter position(REAL, 3);
    box.manage(position);
    parser.manageObject(box);
    parser.parse(in);
    std::multimap<string, Parser::Object> objects = parser.getObjects();
    //Iterate through objects, do stuff
    */
  }

  /// Write a world to a stream
  void World::write(std::ostream &output)
  {

  }

}
// Local Variables: ***
// Mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
