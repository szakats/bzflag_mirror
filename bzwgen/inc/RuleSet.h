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


#ifndef __RULESET_H__
#define __RULESET_H__

#include <string>
#include "Rule.h"
#include "Mesh.h"
#include "Material.h"
#define MAX_RECURSION 1000

class RuleSet {
  RuleMap rules;
  AttributeMap attrmap;
  int recursion;
  MeshVector* meshes;
  MaterialVector materials;
public:
  RuleSet() : recursion(0), meshes(NULL) { }
  MeshVector* run(Mesh* initial_mesh, int initial_face, std::string& rulename);
  int runMesh(Mesh* mesh, int face, std::string& rulename);
  int runNewMesh(Mesh* old_mesh, int old_face, std::string& rulename);
  void loadMaterial(std::string& id, std::string& name);
  void initialize() { std::string init = std::string("initialize"); runMesh(NULL,0,init); }
  void addAttr(const char* name, double value) { std::string temp = name; addAttr(temp,value); }
  void addAttr(std::string& name, double value) { attrmap[name] = value; }
  double getAttr(std::string& name); 
  double getAttr(const char* name) { std::string temp = name; return getAttr(temp); }
  void addRule(std::string& name, Rule* rule);
  void output(Output& out );
  int materialsCount() { return materials.size(); }
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