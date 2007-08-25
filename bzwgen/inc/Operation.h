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


#ifndef __OPERATION_H__
#define __OPERATION_H__

#include <vector>
#include <string>
#include "Expression.h"
#include "Mesh.h"
#include "globals.h"

class RuleSet; // To avoid .h file recursion

class Operation {
protected:
  RuleSet* ruleset;
public:
  Operation(RuleSet* _ruleset) : ruleset(_ruleset) {}
  virtual int runMesh(Mesh*,int) = 0;
  virtual ~Operation() {}
};

typedef std::vector <Operation*> OperationVector;
typedef OperationVector::iterator OperationVectIter;

class OperationNonterminal : public Operation {
  std::string ref;
public:
  OperationNonterminal(RuleSet* _ruleset, const char* _ref) : Operation(_ruleset), ref(_ref) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationAddFace : public Operation {
  std::string ref;
public:
  OperationAddFace(RuleSet* _ruleset, const char* _ref) : Operation(_ruleset), ref(_ref) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationTest : public Operation {
public:
  OperationTest(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationMultiFace : public Operation {
public:
  OperationMultiFace(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationSpawn : public Operation {
  std::string ref;
public:
  OperationSpawn(RuleSet* _ruleset, const char* _ref) : Operation(_ruleset), ref(_ref) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationUnchamfer : public Operation {
public:
  OperationUnchamfer(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face) {
    int size = mesh->f[face]->vtx->size();
    for (int i = 0; i < int(size / 2); i++) {
      mesh->weldVertices(mesh->f[face]->vtx->at(i),mesh->f[face]->vtx->at(i+1));
    }
    return face;
  }
};

class OperationFree : public Operation {
public:
  OperationFree(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face) {
    mesh->freeFace(face);
    return face;
  }
};

class OperationRemove : public Operation {
public:
  OperationRemove(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face) {
    mesh->f[face]->output = false;
    return face;
  }
};

class OperationTextureFull : public Operation {
public:
  OperationTextureFull(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face) {
    mesh->textureFaceFull(face);
    return face;
  }
};

class OperationTextureClear : public Operation {
public:
  OperationTextureClear(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face) {
    mesh->f[face]->tcd->clear();
    return face;
  }
};

class OperationTexture : public Operation {
public:
  OperationTexture(RuleSet* _ruleset) : Operation(_ruleset) { };
  int runMesh(Mesh* mesh, int face);
};

class OperationTextureQuad : public Operation {
  Expression* a;
  Expression* b;
  Expression* c;
  Expression* d;
public:
  OperationTextureQuad(RuleSet* _ruleset, Expression* _a, Expression* _b, Expression* _c, Expression* _d) : Operation(_ruleset), a(_a), b(_b), c(_c), d(_d) { };
  int runMesh(Mesh* mesh, int face) {
    mesh->textureFaceQuad(face,a->calculate(mesh,face),b->calculate(mesh,face),c->calculate(mesh,face),d->calculate(mesh,face));
    return face;
  }
};

class OperationScale : public Operation {
  Expression* x;
  Expression* y;
public:
  OperationScale(RuleSet* _ruleset, Expression* _x, Expression* _y) : Operation(_ruleset), x(_x), y(_y) { };
  int runMesh(Mesh* mesh, int face) {
    if (mesh == NULL) return 0;
    mesh->scaleFace(face,x->calculate(mesh,face),y->calculate(mesh,face));
    return face; 
  }
};

class OperationTranslate : public Operation {
  Expression* x;
  Expression* y;
  Expression* z;
public:
  OperationTranslate(RuleSet* _ruleset, Expression* _x, Expression* _y, Expression* _z) : Operation(_ruleset), x(_x), y(_y), z(_z) { };
  int runMesh(Mesh* mesh, int face) {
    if (mesh == NULL) return 0;
    mesh->translateFace(face,x->calculate(mesh,face),y->calculate(mesh,face),z->calculate(mesh,face));
    return face; 
  }
};

class OperationSingle : public Operation {
protected:
  Expression *exp;
  float value;
public:
  OperationSingle(RuleSet* _ruleset, Expression* _exp) : Operation(_ruleset), exp(_exp) { };
  void flatten(Mesh* mesh,int face) { value = exp->calculate(mesh,face); }
  ~OperationSingle() {
    delete exp;
  }
};

class OperationNGon : public OperationSingle {
public:
  OperationNGon(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) {
    flatten(mesh,face);
    mesh->freeFace(face);
    face = mesh->createNGon(mesh->faceCenter(face),minf(mesh->faceH(face),mesh->faceV(face))/2,round(value));
    return face;
  }
};

class OperationAssert : public OperationSingle {
public:
  OperationAssert(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) {
    flatten(mesh,face);
    if (value >= 0.0f) return face; else return -1;
  }
};

class OperationAssign : public OperationSingle {
protected:
  std::string attrname;
public:
  OperationAssign(RuleSet* _ruleset, Expression* _exp, const char* _attrname ) : OperationSingle(_ruleset,_exp), attrname(_attrname) {}
  int runMesh(Mesh*,int face);
};


class OperationMaterial : public OperationSingle {
public:
  OperationMaterial(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    if (mesh == NULL) return 0;
    flatten(mesh,face);
    mesh->f[face]->mat = round(value);
    return face; 
  };
};

class OperationExpand : public OperationSingle {
public:
  OperationExpand(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    if (mesh == NULL) return 0;
    flatten(mesh,face);
    mesh->expandFace(face,value);
    return face; 
  };
};

class OperationTaper : public OperationSingle {
public:
  OperationTaper(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    if (mesh == NULL) return 0;
    flatten(mesh,face);
    mesh->taperFace(face,value);
    return face; 
  };
};

class OperationChamfer : public OperationSingle {
public:
  OperationChamfer(RuleSet* _ruleset, Expression* _exp) : OperationSingle(_ruleset,_exp) {}
  int runMesh(Mesh* mesh,int face) { 
    if (mesh == NULL) return 0;
    flatten(mesh,face);
    mesh->chamferFace(face,value);
    return face; 
  };
};

class OperationMultifaces : public OperationSingle {
protected:
  StringVector* facerules;
  IntVector* faces;
  bool allsame;
public:
  OperationMultifaces(RuleSet* _ruleset, Expression* _exp, StringVector* _facerules);
  int runMesh(Mesh* mesh,int);
  ~OperationMultifaces() {
    if (facerules != NULL) delete facerules;
    if (faces != NULL) delete faces;
  }  
};

class OperationExtrude : public OperationMultifaces {
public:
  OperationExtrude(RuleSet* _ruleset, Expression* _exp, StringVector* facerules) : OperationMultifaces(_ruleset,_exp,facerules) {}
  int runMesh(Mesh* mesh,int face);
};

class OperationExtrudeT : public OperationMultifaces {
public:
  OperationExtrudeT(RuleSet* _ruleset, Expression* _exp, StringVector* facerules) : OperationMultifaces(_ruleset,_exp,facerules) {}
  int runMesh(Mesh* mesh,int face);
};

class OperationSubdivide : public OperationMultifaces {
  bool horiz;
  Expression* esnap;
public:
  OperationSubdivide(RuleSet* _ruleset, Expression* _exp, bool _horiz, StringVector* facerules, Expression* _esnap = NULL) : OperationMultifaces(_ruleset,_exp,facerules), horiz(_horiz), esnap(_esnap) {}
  int runMesh(Mesh* mesh,int face);
};

class OperationPartition : public OperationMultifaces {
  bool horiz;
  bool inverse;
  Expression* esnap;
public:
  OperationPartition(RuleSet* _ruleset, Expression* _exp, bool _horiz, StringVector* facerules, bool _inverse = false, Expression* _esnap = NULL) : OperationMultifaces(_ruleset,_exp,facerules), horiz(_horiz), inverse(_inverse), esnap(_esnap) {}
  int runMesh(Mesh* mesh,int face);
};


#endif /* __OPERATION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
