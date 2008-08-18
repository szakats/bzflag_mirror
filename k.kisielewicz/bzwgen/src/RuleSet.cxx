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

#include "RuleSet.h"
#include "globals.h"

void RuleSet::addRule(String& name, Rule* rule) {
  Logger.log( 3, "RuleSet : added rule '%s'.", rule->getName().c_str() );
  rules[name] = rule;
}

double RuleSet::getAttr(String& name) {
  AttributeMap::iterator itr = attrmap.find(name);
  if (itr == attrmap.end()) {
    Logger.log( "RuleSet : Warning : attribute '%s' not found!", name.c_str() );
    return 0.0;
  }
  return itr->second;
}



int RuleSet::runMesh(Mesh* mesh, int face, String& rulename) {
  if (recursion == -1) return -1;
  recursion++;
  if (recursion == MAX_RECURSION) {
    recursion = -1;
    Logger.log( "RuleSet : Warning : Recursion level 1000 reached! Are you sure you have no infinite loops?");
    return -1;
  }

  RuleMapIter itr = rules.find(rulename);
  if (itr == rules.end()) {
    Logger.log( "RuleSet : Warning : rule '%s' not found!", rulename.c_str() );
    return -1;
  }
  int result = itr->second->runMesh(mesh,face);

  recursion--;
  return result;
}

MeshVector* RuleSet::run(Mesh* initial_mesh, int initial_face, String& rulename) {
  meshes = new MeshVector();
  meshes->push_back(initial_mesh);
  initial_mesh->pushBase(initial_face);
  initial_mesh->addInsideVertex(initial_mesh->faceCenter(initial_face)+initial_mesh->faceNormal(initial_face)*0.05f);
  if (runMesh(initial_mesh,initial_face,rulename) == -1)
    Logger.log( "RuleSet : run failed with start rule '%s!'", rulename.c_str() );
  return meshes;
}

int RuleSet::runNewMesh(Mesh* old_mesh, int old_face, String& rulename) {
  Mesh* newmesh = new Mesh();
  Face* newface = new Face();
  size_t size = old_mesh->getFace(old_face)->size();
  for ( size_t i = 0; i < size; i++ ) {
    newface->addVertex( i );
    newmesh->addVertex( old_mesh->getFaceVertex( old_face, i ) );
  }
  int newfaceid = newmesh->addFace(newface);
  newmesh->pushBase(newfaceid);
  meshes->push_back(newmesh);
  newmesh->addInsideVertex(newmesh->faceCenter(newfaceid)+newmesh->faceNormal(newfaceid)*0.05f);
  return runMesh(newmesh,newfaceid,rulename);
}

void RuleSet::loadMaterial(String& id, String& name, bool noradar) {
  int matid = materials.size();
  addAttr(id,double(matid));
  materials.push_back( Material(matid,name,noradar) );
}

void RuleSet::output(Output& out ) {
  for (MaterialVectIter itr = materials.begin(); itr!= materials.end(); ++itr) (*itr).output(out);
}

RuleSet::~RuleSet() {
  for (RuleMapIter itr = rules.begin();itr != rules.end(); ++itr) delete itr->second;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
