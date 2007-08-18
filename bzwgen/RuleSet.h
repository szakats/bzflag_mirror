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

#ifndef __RULESET_H__
#define __RULESET_H__

#include <string>
#include "Rule.h"
#include "Mesh.h"
#define MAX_RECURSION 1000

class RuleSet {
  RuleMap rules;
  AttributeMap attrmap;
  int recursion;
public:
  RuleSet() : recursion(0) {}
  int runMesh(Mesh* mesh, int face, std::string& rulename);
  void initialize() { std::string init = std::string("initialize"); runMesh(NULL,0,init); }
  void addAttr(std::string& name, float value) { attrmap[name] = value; }
  float getAttr(std::string& name); 
  void addRule(std::string& name, Rule* rule);
  ~RuleSet();
};

#endif /* __RULESET_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
