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

int OperationLoadMaterial::runMesh(Mesh*, int face) {
  ruleset->loadMaterial(id,filename);
  return face;
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
: OperationSingle(_ruleset, _exp), facerules(_facerules), allsame(false) {
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
  IntVector* faces = ((MultiFace*)mesh->f[face])->detachFace(roundToInt(value));
  if (faces != NULL) {
    OperationMultifaces::runMesh(mesh,face,faces);
    delete faces;
  }
  return face;
}


int OperationMultifaces::runMesh(Mesh* mesh,int,IntVector* faces) {
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
    IntVector* faces = mesh->extrudeFaceR(face,value,mesh->f[face]->mat);
    OperationMultifaces::runMesh(mesh,face,faces);
    delete faces;
  } else {
    mesh->extrudeFace(face,value,mesh->f[face]->mat);
  }
  return face; 
}

int OperationExtrudeT::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  IntVector* faces = mesh->extrudeFaceR(face,value,mesh->f[face]->mat);\
  double snap = ruleset->getAttr("SNAP");
  double textile = ruleset->getAttr("TEXTILE");
  for (size_t i = 0; i < faces->size(); i++) {
    mesh->textureFace(faces->at(i),snap,textile);
  }
  if (facerules != NULL) {
    OperationMultifaces::runMesh(mesh,face,faces);
  }
  delete faces;
  return face; 
}

int OperationTexture::runMesh(Mesh* mesh, int face) {
  mesh->textureFace(face,ruleset->getAttr("SNAP"),ruleset->getAttr("TEXTILE"));
  return face;
}


int OperationSplitFace::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;

  DoubleVector* dv = new DoubleVector(splits->size());
  for (size_t i = 0; i < splits->size(); i++) 
    (*dv)[i] = splits->at(i)->calculate(mesh,face);

  IntVector* faces;
  if (exp != NULL)
    faces = mesh->splitFace(face,dv,horiz,exp->calculate(mesh,face));
  else
    faces = mesh->splitFace(face,dv,horiz);
  if (facerules != NULL) {
    OperationMultifaces::runMesh(mesh,face,faces);
  }
  delete faces;
  delete dv;
  return face; 
}

int OperationRepeat::runMesh(Mesh* mesh,int face) { 
  if (mesh == NULL) return 0;
  flatten(mesh,face);
  IntVector* faces = mesh->repeatSubdivdeFace(face,value,horiz);
  if (facerules != NULL) {
    OperationMultifaces::runMesh(mesh,face,faces);
  }
  delete faces;
  return face; 
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
  mesh->v[f1->vtx->at(0)] = mesh->v[f1->vtx->at(0)]+Vertex(0.0,+4.0,0.0);
  mesh->v[f1->vtx->at(1)] = mesh->v[f1->vtx->at(1)]+Vertex(0.0,+4.0,0.0);
  mesh->v[f1->vtx->at(2)] = mesh->v[f1->vtx->at(2)]+Vertex(0.0,-4.0,0.0);
  mesh->v[f1->vtx->at(3)] = mesh->v[f1->vtx->at(3)]+Vertex(0.0,-4.0,0.0);

  mesh->v[f2->vtx->at(0)] = mesh->v[f2->vtx->at(0)]+Vertex(+4.0,0.0,0.0);
  mesh->v[f2->vtx->at(1)] = mesh->v[f2->vtx->at(1)]+Vertex(-4.0,0.0,0.0);
  mesh->v[f2->vtx->at(2)] = mesh->v[f2->vtx->at(2)]+Vertex(-4.0,0.0,0.0);
  mesh->v[f2->vtx->at(3)] = mesh->v[f2->vtx->at(3)]+Vertex(+4.0,0.0,0.0);

  printf("Adding face1...\n");
  mf->addFace(f1);
  printf("Adding face2...\n");
  mf->addFace(f2);
  printf("Done. (%d)\n",mf->size());

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
