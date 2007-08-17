/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <vector>
#include <fstream> 
#include "globals.h"
#include "RuleSet.h"
#include "Material.h"

class Generator {
protected:
  int size;
  RuleSet* ruleset;
  MaterialVector mats;
public:
  Generator(RuleSet* _ruleset) : ruleset(_ruleset) {}
  virtual void parseOptions(Options opt);
  virtual void run();
  inline int getSize() { return size; }
  virtual void output(std::ofstream& out);
  Material* getMaterial(int id) { return mats[id]; }
  virtual ~Generator() {
    MaterialVectIter itr; 
    for (itr = mats.begin(); itr!= mats.end(); ++itr) delete (*itr);
  }
};

typedef std::vector<Generator> GeneratorVector;
typedef GeneratorVector::iterator GeneratorVectIter;

#endif /* __GENERATOR_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
