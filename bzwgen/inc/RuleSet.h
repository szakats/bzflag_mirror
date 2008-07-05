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


#ifndef __RULESET_H__
#define __RULESET_H__

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
  MeshVector* run(Mesh* initial_mesh, int initial_face, String& rulename);
  int runMesh(Mesh* mesh, int face, String& rulename);
  int runNewMesh(Mesh* old_mesh, int old_face, String& rulename);
  void loadMaterial(String& id, String& name, bool noradar);
  void initialize() { String init = String("initialize"); runMesh(NULL,0,init); }
  void addAttr(const char* name, double value) { String temp = name; addAttr(temp,value); }
  void addAttr(String& name, double value) { attrmap[name] = value; }
  double getAttr(String& name); 
  double getAttr(const char* name) { String temp = name; return getAttr(temp); }
  void addRule(String& name, Rule* rule);
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
