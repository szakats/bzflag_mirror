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
* @file FaceGenerator.h
* @author Kornel Kisielewicz kornel.kisielewicz@gmail.com
* @brief Implements a Generator class that works on graph faces.
*/

#ifndef __FACEGENERATOR_H__
#define __FACEGENERATOR_H__

#include "Generator.h"
#include "Material.h"
#include "globals.h"
#include "Output.h"

/** 
* @class FaceGenerator
* @brief Class for the map generator working on graphs. 
*/
class FaceGenerator : public Generator {
public:
  /** 
  * Constructor, just runs it's inherited constructor. 
  */
  FaceGenerator( RuleSet* _ruleset ) : Generator( _ruleset ) {};
  /** 
  * Parses options. 
  */
  void parseOptions( CCommandLineArgs* /*opt*/ );
  /**
  * Runs the generator. 
  */
  void run( );
  /**
  * Destructor.
  */
  virtual ~FaceGenerator( ) {}
private:
};

#endif /* __FACEGENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
