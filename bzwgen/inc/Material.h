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
 * @file Material.h
 * @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
 * @brief Defines a class for holding material info.
 */

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "globals.h"
#include "Output.h"

/** 
 * @class Material
 * @brief Class for holding material information.
 */
class Material {
  /** ID of the material, and at the same time it's "name". */
  int name;
  /** Path to the file with the texture. */
  String path;
  /** Wether this material doesn't appear on radar. */
  bool noradar;
public:
  /** Constructor, takes and initializes all the neccessary data. */
  Material( const int _name, const String& _path, bool _noradar = false ) 
    : name( _name ), path( _path ), noradar( _noradar ) {};
  /** Outputs the material to the passed output class. */
  void output( Output& out ) {
    out.material( name, path, noradar );
  }
};

/** Type definition for a vector of materials. */
typedef std::vector<Material> MaterialVector;
/** Type definition for a iterator over a vector of materials. */
typedef MaterialVector::iterator MaterialVectIter;

#endif /* __MATERIAL_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
