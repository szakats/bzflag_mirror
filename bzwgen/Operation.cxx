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

#include "Operation.h"
#include "RuleSet.h"

int OperationNonterminal::runMesh(Mesh* mesh, int face) { 
  return ruleset->runMesh(mesh,face,ref);
}

int OperationSpawn::runMesh(Mesh* mesh, int face) { 
  ruleset->runNewMesh(mesh,face,ref);
  return face;
}

int OperationAssign::runMesh(Mesh* mesh, int face) { 
  flatten(mesh,face);
  ruleset->addAttr(attrname,value);
  return face; 
}

OperationMultifaces::OperationMultifaces(RuleSet* _ruleset, Expression* _exp, StringVector* _facerules) 
: OperationSingle(_ruleset, _exp), facerules(_facerules), faces(NULL), allsame(false) {
  if (facerules != NULL) {
    if (facerules->size() == 0) {
      delete facerules; 
      facerules = NULL;
    } else
      if (facerules->size() == 1 && facerules->at(0)[0] == '@') {
        allsame = true;
        facerules->at(0).erase(0,1);
      }
  }
}

int OperationMultifaces::runMesh(Mesh* mesh,int) {
  if (mesh == NULL) return 0;
  if (allsame) {
    for (size_t i = 0; i < faces->size(); i++)
      ruleset->runMesh(mesh,faces->at(i),facerules->at(0));
    return 0;
  }
  for (size_t i = 0; i < facerules->size(); i++) {
    if (facerules->at(i).empty()) continue;
    if (i >= faces->size()) break;
    ruleset->runMesh(mesh,faces->at(i),facerules->at(i));
  }

  return 0;
}


int OperationExtrude::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  if (facerules != NULL) {
    faces = mesh->extrudeFaceR(face,value,mesh->f[face]->mat);
    OperationMultifaces::runMesh(mesh,face);
  } else {
    mesh->extrudeFace(face,value,mesh->f[face]->mat);
  }
  return face; 
}

int OperationSubdivide::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  faces = mesh->subdivdeFace(face,round(value),horiz);
  if (facerules == NULL) {
    delete faces;
    faces = NULL;
  } else {
    OperationMultifaces::runMesh(mesh,face);
  }
  return face; 
}

int OperationPartition::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  if (facerules == NULL) {
    mesh->partitionFace(face,value,horiz);
  } else {
    faces = new IntVector();
    faces->push_back(mesh->partitionFace(face,value,horiz));
    OperationMultifaces::runMesh(mesh,face);
  }
  return face; 
}




// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
