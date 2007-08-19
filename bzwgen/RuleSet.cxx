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

MeshVector* RuleSet::run(Mesh* initial_mesh, int initial_face, std::string& rulename) {
  meshes = new MeshVector();
  meshes->push_back(initial_mesh);
  initial_mesh->inside.push_back(initial_mesh->faceCenter(initial_face));
  if (runMesh(initial_mesh,initial_face,rulename) == -1) {
    printf("RuleSet::run failed with start rule '%s!'\n",rulename.c_str());
  }
  return meshes;
}

int RuleSet::runNewMesh(Mesh* old_mesh, int old_face, std::string& rulename) {
  Mesh* newmesh = new Mesh();
  Face* newface = new Face();
  int size = old_mesh->f[old_face]->vtx->size();
  for (int i = 0; i < size; i++) {
    newface->vtx->push_back(i);
    newmesh->addVertex(old_mesh->v[old_mesh->f[old_face]->vtx->at(i)]);
  }
  int newfaceid = newmesh->addFace(newface);
  meshes->push_back(newmesh);
  newmesh->inside.push_back(newmesh->faceCenter(newfaceid));
  return runMesh(newmesh,newfaceid,rulename);
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
