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

#include "Operation.h"
#include "RuleSet.h"
#include "MultiFace.h"

int OperationNonterminal::runMesh( Mesh* mesh, int face ) {
  return ruleset->runMesh( mesh, face, ref );
}

int OperationLoadMaterial::runMesh( Mesh*, int face ) {
  ruleset->loadMaterial( id, filename, noradar );
  return face;
}


int OperationAddFace::runMesh( Mesh* mesh, int face ) {
  if (!mesh->getFace( face )->isMultiFace()) {
    Logger.log( "OperationAddFace: Error! addface passed on a non-MultiFace face!" );
    return face;
  }
  int newface = mesh->rePushBase();
  newface = ruleset->runMesh( mesh, newface, ref );
  ( (MultiFace*) mesh->getFace( face ) )->addFace( mesh->getFace( newface ) );
  return face;
}

int OperationSpawn::runMesh( Mesh* mesh, int face ) {
  ruleset->runNewMesh( mesh, face, ref );
  return face;
}

int OperationAssign::runMesh( Mesh* mesh, int face ) {
  flatten( mesh, face );
  ruleset->addAttr( attrname, value[0] );
  return face;
}

OperationMultifaces::OperationMultifaces( RuleSet* _ruleset, Expression* _exp, StringVector* _facerules )
  : OperationSingle( _ruleset, _exp ), facerules( _facerules ), allsame( false )
{
  if ( facerules != NULL ) {
    if ( facerules->size() == 0 ) {
      delete facerules;
      facerules = NULL;
    } else {
      if ( facerules->size() == 1 && facerules->at(0)[0] == '@' ) {
        allsame = true;
        facerules->at( 0 ).erase( 0, 1 );
      }
    }
  }
}

int OperationDetachFace::runMesh(Mesh* mesh,int face) {
  if ( !mesh->getFace( face)->isMultiFace() ) {
    Logger.log( "OperationDetachFace: Error! detachface passed on a non-MultiFace face!" );
    return face;
  }
  flatten( mesh,face );
  std::auto_ptr<IntVector> faces(( (MultiFace*)mesh->getFace( face ) )->detachFace( math::roundToInt(value[0]) ));
  if ( faces.get() != NULL ) {
    OperationMultifaces::runMesh( mesh, face, faces.get() );
  }
  return face;
}


int OperationMultifaces::runMesh( Mesh* mesh, int, IntVector* faces ) {
  if ( mesh == NULL ) return 0;
  if ( allsame ) {
    for ( size_t i = 0; i < faces->size(); i++ )
      ruleset->runMesh( mesh, faces->at(i), facerules->at(0) );
    return 0;
  }
  if ( facerules != NULL ) {
    for ( size_t i = 0; i < facerules->size(); i++ ) {
      if ( facerules->at(i).empty() ) continue;
      if ( i >= faces->size() ) break;
      ruleset->runMesh( mesh, faces->at(i), facerules->at(i) );
    }
  }
  return 0;
}


int OperationExtrude::runMesh( Mesh* mesh, int face )
{
  if ( mesh == NULL ) return 0;
  flatten( mesh, face );
  if ( facerules != NULL ) {
    IntVector faces;
    mesh->extrudeFace( face, value[0], mesh->getFace( face )->getMaterial(), &faces );
    OperationMultifaces::runMesh( mesh, face, &faces );
  } else {
    mesh->extrudeFace( face, value[0], mesh->getFace( face )->getMaterial() );
  }
  return face;
}

int OperationExtrudeT::runMesh( Mesh* mesh, int face )
{
  if (mesh == NULL) return 0;
  flatten( mesh, face );
  IntVector faces;
  mesh->extrudeFace( face, value[0], mesh->getFace( face )->getMaterial(), &faces );

  double snap    = ruleset->getAttr( "SNAP" );
  double textile = ruleset->getAttr( "TEXTILE" );

  for ( size_t i = 0; i < faces.size(); i++ ) {
    mesh->textureFace( faces.at(i), snap, textile );
  }
  if ( facerules != NULL ) {
    OperationMultifaces::runMesh( mesh, face, &faces );
  }
  return face;
}

int OperationTexture::runMesh( Mesh* mesh, int face ) {
  mesh->textureFace( face, ruleset->getAttr( "SNAP" ), ruleset->getAttr( "TEXTILE" ) );
  return face;
}


int OperationSplitFace::runMesh( Mesh* mesh,int face ) {
  if ( mesh == NULL ) return 0;

  DoubleVector dv( splits->size() );
  for ( size_t i = 0; i < splits->size(); i++ )
    dv[i] = splits->at(i)->calculate( mesh, face );

  double ssnap(0.0);
  if (exp[0] != NULL) ssnap = exp[0]->calculate(mesh, face);
  std::auto_ptr<IntVector> faces(mesh->splitFace( face, &dv, horiz, ssnap ));

  if (facerules != NULL) {
    OperationMultifaces::runMesh( mesh, face, faces.get() );
  }
  return face;
}

int OperationRepeat::runMesh( Mesh* mesh,int face )
{
  if (mesh == NULL) return 0;
  flatten( mesh, face );
  std::auto_ptr<IntVector> faces(mesh->repeatSubdivdeFace( face, value[0], horiz ));
  if (facerules != NULL) {
    OperationMultifaces::runMesh( mesh, face, faces.get() );
  }
  return face;
}

int OperationMultiFace::runMesh(Mesh* mesh, int face) {
  MultiFace* mf = new MultiFace(mesh);
  mf->addFace(mesh->getFace(face));
  mesh->substituteFace( face, mf );
  return face;
}




// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
