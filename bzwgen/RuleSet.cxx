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

#include "RuleSet.h"
#include "globals.h"

void RuleSet::addRule(std::string& name, Rule* rule) { 
  if (debugLevel > 1) {
    printf("Added rule '%s'.\n",rule->getName().c_str());
  }
  rules[name] = rule; 
}

float RuleSet::getAttr(std::string& name) {
  AttributeMap::iterator itr = attrmap.find(name); 
  if (itr == attrmap.end()) {
    printf("Warning : attribute '%s' not found!\n",name.c_str());
    return 0.0f;
  }
  return itr->second;
}



int RuleSet::runMesh(Mesh* mesh, int face, std::string& rulename) {
  if (recursion == -1) return -1;
  recursion++;
  if (recursion == MAX_RECURSION) {
    recursion = -1;
    printf("Warning : Recursion level 1000 reached! Are you sure you have no infinite loops?\n");
    return -1;
  }
  RuleMapIter itr = rules.find(rulename); 
  if (itr == rules.end()) {
    printf("Warning : rule '%s' not found!\n",rulename.c_str());
    return -1;
  }
  int result = itr->second->runMesh(mesh,face);
  recursion--;
  return result;
}

RuleSet::~RuleSet() { 
  RuleMapIter itr;
  for (itr = rules.begin();itr != rules.end(); ++itr) delete itr->second; 
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
