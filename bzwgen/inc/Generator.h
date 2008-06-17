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

/** 
 * @class Generator
 * @brief Abstract class for the map generator. 
 *
 * This class is intended to be derived from. It holds a protected RuleSet
 * and takes care of material handling.
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
public:
  /** Material ID of the standard road material. 
      Probably shouldn't be public, will be refactored */
  int roadid;
  /** Material ID of the standard road crossing material. 
      Probably shouldn't be public, will be refactored */
  int roadxid;
  /** CTFSafe flag. Also shouldn't be public, will be refactored */
  bool ctfSafe;
  /** Standard constructor, takes a already loaded RuleSet as
      it's parameter. The ruleset needs to have MATROAD and 
      MATROADX defined. */
  Generator(RuleSet* _ruleset) : ruleset(_ruleset) {
    roadid  = math::roundToInt(ruleset->getAttr("MATROAD"));
    roadxid = math::roundToInt(ruleset->getAttr("MATROADX"));
  }
  /** Parse command line or config file options. Virtual so the
      specific generator can overload it. Generator parses the size,
      bases and ctfsafe options. */
  virtual void parseOptions(CCommandLineArgs* opt);
  /** Does nothing, should be overloaded. */
  virtual void run();
  /** Returns the size of the world. */
  inline int getSize() { return size; }
  /** Returns the loaded RuleSet. */
  inline RuleSet* getRuleSet() { return ruleset; }
  /** Outputs data. In case of the Generator class this handles
      output of the file header and materials. */
  virtual void output(Output& out);
  /** Returns the Material by material ID. */
  Material* getMaterial(int id) { return mats[id]; }
  /** Destructor, frees the materials, but not the ruleset. */
  virtual ~Generator() {
    MaterialVectIter itr; 
    for (itr = mats.begin(); itr!= mats.end(); ++itr) delete (*itr);
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
