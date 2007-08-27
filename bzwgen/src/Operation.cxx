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
#include "MultiFace.h"

int OperationNonterminal::runMesh(Mesh* mesh, int face) { 
  return ruleset->runMesh(mesh,face,ref);
}

int OperationAddFace::runMesh(Mesh* mesh, int face) { 
  if (!mesh->f[face]->isMultiFace()) {
    printf("Error: addface passed on a non-MultiFace face!");
    return face;
  }
  Face* clone = new Face();
  int fsize = mesh->vbase.size();
  clone->vtx->resize(fsize);
  for (int i = 0; i < fsize; i++) {
    (*clone->vtx)[i] = mesh->addVertex(mesh->vbase[i]);
  }
  int newface = mesh->addFace(clone);
  newface = ruleset->runMesh(mesh,newface,ref);
  ((MultiFace*)mesh->f[face])->addFace(mesh->f[newface]);
  return face;
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

int OperationDetachFace::runMesh(Mesh* mesh,int face) {
  if (!mesh->f[face]->isMultiFace()) {
    printf("Error: detachface passed on a non-MultiFace face!");
    return face;
  }
  flatten(mesh,face);
  faces = ((MultiFace*)mesh->f[face])->detachFace(round(value));
  if (faces != NULL) {
    OperationMultifaces::runMesh(mesh,face);
    delete faces;
  }
  return face;
}


int OperationMultifaces::runMesh(Mesh* mesh,int) {
  if (mesh == NULL) return 0;
  if (allsame) {
    for (size_t i = 0; i < faces->size(); i++)
      ruleset->runMesh(mesh,faces->at(i),facerules->at(0));
    return 0;
  }
  if (facerules != NULL) {
    for (size_t i = 0; i < facerules->size(); i++) {
      if (facerules->at(i).empty()) continue;
      if (i >= faces->size()) break;
      ruleset->runMesh(mesh,faces->at(i),facerules->at(i));
    }
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

int OperationExtrudeT::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  faces = mesh->extrudeFaceR(face,value,mesh->f[face]->mat);\
  float snap = ruleset->getAttr("SNAP");
  float textile = ruleset->getAttr("TEXTILE");
  for (size_t i = 0; i < faces->size(); i++) {
    mesh->textureFace(faces->at(i),snap,textile);
  }
  if (facerules != NULL) {
    OperationMultifaces::runMesh(mesh,face);
  }
  return face; 
}

int OperationTexture::runMesh(Mesh* mesh, int face) {
  mesh->textureFace(face,ruleset->getAttr("SNAP"),ruleset->getAttr("TEXTILE"));
  return face;
}


int OperationSubdivide::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  float snap = 0.0f;
  if (esnap != NULL) snap = esnap->calculate(mesh,face);
  faces = mesh->subdivdeFace(face,round(value),horiz,snap);
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
  if (esnap != NULL) {
    float s = esnap->calculate(mesh,face);
    float l;
    if (horiz) { l = mesh->faceH(face); } else { l = mesh->faceV(face); }
    s = refinesnap(s,l);
    value = snap(value,s);
  }
  int other;
  if (facerules == NULL) {
    other = mesh->partitionFace(face,value,horiz);
  } else {
    faces = new IntVector();
    other = mesh->partitionFace(face,value,horiz);
    if (inverse) {
      faces->push_back(face);
    } else {
      faces->push_back(other);
    }
    OperationMultifaces::runMesh(mesh,face);
  }
  if (inverse) {
    return other; 
  } else {
    return face;
  }
}

int OperationTest::runMesh(Mesh* mesh, int face) {
  MultiFace* mf = new MultiFace(mesh);
  Face* f1 = new Face();
  Face* f2 = new Face();
  for (int i = 0; i < 4; i++) {
    int f1vid = mesh->f[face]->vtx->at(i);
    int f2vid = mesh->addVertex(mesh->v[mesh->f[face]->vtx->at(i)]);
    f1->vtx->push_back(f1vid);
    f2->vtx->push_back(f2vid);
  }
  mesh->v[f1->vtx->at(0)] = mesh->v[f1->vtx->at(0)]+Vertex(0.0f,+4.0f,0.0f);
  mesh->v[f1->vtx->at(1)] = mesh->v[f1->vtx->at(1)]+Vertex(0.0f,+4.0f,0.0f);
  mesh->v[f1->vtx->at(2)] = mesh->v[f1->vtx->at(2)]+Vertex(0.0f,-4.0f,0.0f);
  mesh->v[f1->vtx->at(3)] = mesh->v[f1->vtx->at(3)]+Vertex(0.0f,-4.0f,0.0f);

  mesh->v[f2->vtx->at(0)] = mesh->v[f2->vtx->at(0)]+Vertex(+4.0f,0.0f,0.0f);
  mesh->v[f2->vtx->at(1)] = mesh->v[f2->vtx->at(1)]+Vertex(-4.0f,0.0f,0.0f);
  mesh->v[f2->vtx->at(2)] = mesh->v[f2->vtx->at(2)]+Vertex(-4.0f,0.0f,0.0f);
  mesh->v[f2->vtx->at(3)] = mesh->v[f2->vtx->at(3)]+Vertex(+4.0f,0.0f,0.0f);

  printf("Adding face1...\n");
  mf->addFace(f1);
  printf("Adding face2...\n");
  mf->addFace(f2);
  printf("Done. (%d)\n",mf->vtx->size());

  delete mesh->f[face];
  mesh->f[face] = mf;
  return face;
}

int OperationMultiFace::runMesh(Mesh* mesh, int face) {
  MultiFace* mf = new MultiFace(mesh);
  mf->addFace(mesh->f[face]);
  mesh->f[face] = mf;
  return face;
}




// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
