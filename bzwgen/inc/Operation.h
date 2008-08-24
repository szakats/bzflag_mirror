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


#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "globals.h"
#include "Expression.h"
#include "Mesh.h"

class RuleSet; // To avoid .h file recursion

class Operation {
protected:
  RuleSet* ruleset;
public:
  Operation( RuleSet* _ruleset ) : ruleset( _ruleset ) {}
  virtual int runMesh( Mesh*, int ) = 0;
  virtual ~Operation() {}
};

typedef std::vector <Operation*> OperationVector;
typedef OperationVector::iterator OperationVectIter;

template < int SIZE >
class OperationTemplate : public Operation {
protected:
  typedef Expression* ExpressionPtr;
  ExpressionPtr exp[SIZE];
  double value[SIZE];
public:
  OperationTemplate( RuleSet* _ruleset )
    : Operation( _ruleset ) {
    for ( int i = 0; i < SIZE; ++i )
       exp[i] = NULL;
  }
  void flatten( Mesh* mesh, int face ) {
    for ( int i = 0; i < SIZE; ++i )
      if ( exp[i] )
        value[i] = exp[i]->calculate( mesh, face );
  }
  virtual ~OperationTemplate() {
    for ( int i = 0; i < SIZE; ++i )
      deletePointer( exp[i] );
  }
};

class OperationSingle : public OperationTemplate<1> {
public:
  OperationSingle( RuleSet* _ruleset, Expression* exp0 )
    : OperationTemplate<1>( _ruleset ) {
    exp[0] = exp0;
  }
};

class OperationDouble : public OperationTemplate<2> {
public:
  OperationDouble( RuleSet* _ruleset, Expression* exp0, Expression* exp1 )
    : OperationTemplate<2>( _ruleset ) {
    exp[0] = exp0;
    exp[1] = exp1;
  }
};

class OperationTriple : public OperationTemplate<3> {
public:
  OperationTriple( RuleSet* _ruleset, Expression* exp0, Expression* exp1, Expression* exp2 )
    : OperationTemplate<3>( _ruleset ) {
    exp[0] = exp0;
    exp[1] = exp1;
    exp[2] = exp2;
  }
};

class OperationQuad : public OperationTemplate<4> {
public:
  OperationQuad( RuleSet* _ruleset, Expression* exp0, Expression* exp1, Expression* exp2, Expression* exp3 )
    : OperationTemplate<4>( _ruleset ) {
    exp[0] = exp0;
    exp[1] = exp1;
    exp[2] = exp2;
    exp[3] = exp3;
  }
};

class OperationNonterminal : public Operation {
  String ref;
public:
  OperationNonterminal( RuleSet* _ruleset, const char* _ref )
    : Operation( _ruleset ), ref( _ref ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationLoadMaterial : public Operation {
  String id;
  String filename;
  bool noradar;
public:
  OperationLoadMaterial( RuleSet* _ruleset, const char* _id, const char* _filename, bool _noradar )
    : Operation( _ruleset ), id( _id ), filename( _filename ), noradar( _noradar ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationAddFace : public Operation {
  String ref;
public:
  OperationAddFace( RuleSet* _ruleset, const char* _ref )
    : Operation( _ruleset ), ref( _ref ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationMultiFace : public Operation {
public:
  OperationMultiFace( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationSpawn : public Operation {
  String ref;
public:
  OperationSpawn( RuleSet* _ruleset, const char* _ref )
    : Operation( _ruleset ), ref( _ref ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationUnchamfer : public Operation {
public:
  OperationUnchamfer( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    size_t size = mesh->getFace(face)->size();
    for ( size_t i = 0; i < size_t( size / 2 ); i++ ) {
      mesh->weldVertices(
        mesh->getFace( face )->getVertex( i ),
        mesh->getFace( face )->getVertex( i+1 )
      );
    }
    return face;
  }
};

class OperationFree : public Operation {
public:
  OperationFree( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    mesh->freeFace( face );
    return face;
  }
};

class OperationDriveThrough : public Operation {
public:
  OperationDriveThrough( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    mesh->setPassable();
    return face;
  }
};

class OperationRemove : public Operation {
public:
  OperationRemove( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    mesh->getFace( face )->setOutput( false );
    return face;
  }
};

class OperationTextureFull : public Operation {
public:
  OperationTextureFull( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    mesh->textureFaceFull( face );
    return face;
  }
};

class OperationTextureClear : public Operation {
public:
  OperationTextureClear( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face ) {
    mesh->getFace( face )->clearTexCoords();
    return face;
  }
};

class OperationTexture : public Operation {
public:
  OperationTexture( RuleSet* _ruleset ) : Operation( _ruleset ) { };
  int runMesh( Mesh* mesh, int face );
};

class OperationTextureQuad : public OperationQuad {
public:
  OperationTextureQuad( RuleSet* _ruleset, Expression* exp0, Expression* exp1, Expression* exp2, Expression* exp3 )
    : OperationQuad( _ruleset, exp0, exp1, exp2, exp3 ) { };
  int runMesh( Mesh* mesh, int face ) {
    flatten( mesh, face );
    mesh->textureFaceQuad( face, value[0], value[1], value[2], value[3] );
    return face;
  }
};

class OperationScale : public OperationDouble {
public:
  OperationScale(RuleSet* _ruleset, Expression* x, Expression* y)
    : OperationDouble(_ruleset, x, y ) { };
  int runMesh( Mesh* mesh, int face ) {
    if (mesh == NULL) return 0;
    flatten( mesh, face );
    mesh->scaleFace( face, value[0], value[1] );
    return face;
  }
};

class OperationTranslate : public OperationTriple {
public:
  OperationTranslate(RuleSet* _ruleset, Expression* x, Expression* y, Expression* z)
    : OperationTriple( _ruleset, x, y, z ) { };
  int runMesh( Mesh* mesh, int face ) {
    if (mesh == NULL) return 0;
    flatten( mesh, face );
    mesh->translateFace( face, value[0], value[1], value[2] );
    return face;
  }
};

class OperationTranslateR : public OperationTriple {
public:
  OperationTranslateR(RuleSet* _ruleset, Expression* x, Expression* y, Expression* z)
    : OperationTriple( _ruleset, x, y, z ) { };
  int runMesh( Mesh* mesh, int face ) {
    if (mesh == NULL) return 0;
    mesh->translateFace( face, value[0]*mesh->faceH(face),
                               value[1]*mesh->faceV(face),
                               value[2]*mesh->faceCenter(face).z);
    return face;
  }
};


class OperationNGon : public OperationDouble {
public:
  OperationNGon(RuleSet* _ruleset, Expression* _exp, Expression* _nsize = NULL)
    : OperationDouble( _ruleset, _exp, _nsize ) {}
  int runMesh( Mesh* mesh,int face) {
    flatten( mesh, face );
    mesh->freeFace(face);

    int sides = math::roundToInt( value[0] );
    double radius = value[1];
    if ( exp[1] == NULL ) {
      radius = math::min( mesh->faceH(face), mesh->faceV(face) ) / 2;
    }
    face = mesh->createNGon( mesh->faceCenter(face), radius, sides );

    return face;
  }
};

class OperationAssert : public OperationSingle {
public:
  OperationAssert( RuleSet* _ruleset, Expression* _exp )
    : OperationSingle( _ruleset, _exp ) { }
  int runMesh( Mesh* mesh, int face ) {
    flatten( mesh, face );
    return value[0] >= 0.0 ? face : -1;
  }
};

class OperationAssign : public OperationSingle {
protected:
  String attrname;
public:
  OperationAssign( RuleSet* _ruleset, Expression* _exp, const char* _attrname )
    : OperationSingle( _ruleset, _exp ), attrname( _attrname ) { }
  int runMesh( Mesh*, int face );
};


class OperationMaterial : public OperationSingle {
public:
  OperationMaterial( RuleSet* _ruleset, Expression* _exp )
    : OperationSingle( _ruleset, _exp ) { }
  int runMesh( Mesh* mesh, int face ) {
    if ( mesh == NULL ) return 0;
    flatten( mesh, face );
    mesh->getFace( face )->setMaterial( math::roundToInt( value[0] ) );
    return face;
  };
};

class OperationExpand : public OperationSingle {
public:
  OperationExpand( RuleSet* _ruleset, Expression* _exp )
    : OperationSingle( _ruleset, _exp ) { }
  int runMesh( Mesh* mesh,int face ) {
    if ( mesh == NULL ) return 0;
    flatten( mesh, face );
    mesh->expandFace( face, value[0] );
    return face;
  };
};

class OperationTaper : public OperationSingle {
public:
  OperationTaper( RuleSet* _ruleset, Expression* _exp )
    : OperationSingle( _ruleset, _exp ) { }
  int runMesh(Mesh* mesh,int face) {
    if ( mesh == NULL ) return 0;
    flatten( mesh, face );
    mesh->taperFace( face, value[0] );
    return face;
  };
};

class OperationChamfer : public OperationSingle {
public:
  OperationChamfer( RuleSet* _ruleset, Expression* _exp )
    : OperationSingle( _ruleset, _exp ) { }
  int runMesh( Mesh* mesh, int face ) {
    if ( mesh == NULL ) return 0;
    flatten( mesh, face );
    mesh->chamferFace( face, value[0] );
    return face;
  };
};

class OperationMultifaces : public OperationSingle {
protected:
  StringVector* facerules;
  bool allsame;
public:
  OperationMultifaces( RuleSet* _ruleset, Expression* _exp, StringVector* _facerules );
  int runMesh( Mesh* mesh, int, IntVector* faces );
  ~OperationMultifaces() {
    deletePointer( facerules );
  }
};

class OperationDetachFace : public OperationMultifaces {
public:
  OperationDetachFace( RuleSet* _ruleset, Expression* _exp, StringVector* _facerules )
    : OperationMultifaces( _ruleset, _exp, _facerules ) { }
  int runMesh( Mesh* mesh, int face );
};


class OperationExtrude : public OperationMultifaces {
public:
  OperationExtrude( RuleSet* _ruleset, Expression* _exp, StringVector* facerules )
    : OperationMultifaces( _ruleset, _exp, facerules ) { }
  int runMesh( Mesh* mesh, int face );
};

class OperationExtrudeT : public OperationMultifaces {
public:
  OperationExtrudeT( RuleSet* _ruleset, Expression* _exp, StringVector* facerules )
    : OperationMultifaces( _ruleset, _exp, facerules ) { }
  int runMesh( Mesh* mesh, int face );
};

class OperationSplitFace : public OperationMultifaces {
  bool horiz;
  ExpressionVector* splits;
public:
  OperationSplitFace( RuleSet* _ruleset, bool _horiz, StringVector* facerules, ExpressionVector* _splits, Expression* _esnap = NULL)
    : OperationMultifaces( _ruleset, _esnap, facerules ), horiz( _horiz ), splits( _splits ) {}
  int runMesh( Mesh* mesh, int face );
  ~OperationSplitFace() {
    deletePointerVector( splits );
  }
};

class OperationRepeat : public OperationMultifaces {
  bool horiz;
public:
  OperationRepeat( RuleSet* _ruleset, Expression* _exp, bool _horiz, StringVector* facerules )
    : OperationMultifaces( _ruleset, _exp, facerules ), horiz( _horiz ) {}
  int runMesh( Mesh* mesh, int face );
};


#endif /* __OPERATION_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
