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

#include "Mesh.h"

int Mesh::addVertex( Vertex vertex ) {
  if (freeVertices.size() > 0) {
    int free = freeVertices[ freeVertices.size() - 1 ];
    freeVertices.pop_back();
    v[free] = vertex;
    return free;
  } else {
    v.push_back( vertex );
    return v.size()-1;
  }
}

int Mesh::addTexCoord( TexCoord texCoord ) {
  for ( size_t i = 0; i < tc.size(); i++ ) {
    if ( tc[i].equals( texCoord ) ) return i;
  }
  tc.push_back( texCoord );
  return tc.size() - 1;
}

int Mesh::createNGon(Vertex center, double radius, int n) {
  Face* face = new Face();
  double step = (2*double(M_PI))/n;
  for (int i = 0; i < n; i++) {
    int vt = addVertex(center+Vertex(radius*cos(step*double(i)-step/2),radius*sin(step*double(i)-step/2),0.0));
    face->addVertex(vt);
  }
  return addFace(face);
}

void Mesh::extrudeFace( int faceID, double amount, int mat, IntVector* result ) {
  Vertex dir = faceNormal(faceID)*amount;
  IntVector base   = f[faceID]->getVertices();
  IntVector top;

  int size = base.size();

  for (int i = 0; i < size; i++) {
    top.push_back(addVertex(v[base.at(i)]+dir));
  }
  f[faceID]->setVertices( top );

  for (int i = 0; i < size; i++) {
    Face* face = new Face();
    face->setMaterial(mat);
    face->addVertex( base.at(i) );
    face->addVertex( base.at(math::modNext(i,size)) );
    face->addVertex( top.at(math::modNext(i,size)) );
    face->addVertex( top.at(i) );
    int added = addFace( face );
    if (result) result->push_back( added );
  }
}

Vertex Mesh::extensionVertex(int ida, int idb, int idc) {
  Vertex a = (v[idc]-v[ida]).norm();
  Vertex b = (v[idc]-v[idb]).norm();
  Vertex dir = ((a+b)/2).norm();
  double dot = a.dot(dir);
  double length = 1/sqrt(1-dot*dot);
  return dir*length;
}

void Mesh::taperFace(int faceID, double amount) {
  int size = f[faceID]->size();
  Vertex c = faceCenter( faceID );
  for ( int i = 0; i < size; i++ ) {
    Vertex vv = getFaceVertex( faceID, i );
    v[ f[faceID]->getVertex( i ) ] = ( vv - c ) * amount + c;
  }
}

void Mesh::scaleFace( int faceID, double x, double y ) {
  int size = f[faceID]->size();
  Vertex c = faceCenter( faceID );
  for ( int i = 0; i < size; i++ ) {
    Vertex vc = getFaceVertex( faceID, i ) - c;
    int vertexID = f[faceID]->getVertex( i );
    v[vertexID].x = vc.x * x + c.x;
    v[vertexID].y = vc.y * y + c.y;
  }
}

void Mesh::translateFace( int faceID, double x, double y, double z ) {
  int size = f[faceID]->size();
  for ( int i = 0; i < size; i++ ) {
    int vertexID = f[faceID]->getVertex( i );
    v[vertexID].x += x;
    v[vertexID].y += y;
    v[vertexID].z += z;
  }
}


void Mesh::expandFace( int faceID, double amount  ) {
  Vertex normal = faceNormal( faceID );
  // needs to be uniform
  int size = f[faceID]->size();
  Vertex* nv = new Vertex[size];
  for ( int i = 0; i < size; i++ ) {
    Vertex a = getFaceEdge( faceID, math::modPrev( i, size ), math::modNext( i, size ) );
    Vertex b = getFaceEdge( faceID, math::modPrev( i, size ), i );
    double sign = math::sign( b.cross( a ).dot( normal ) );
    nv[i] = getFaceVertex( faceID, i) + extensionVertex(
      f[faceID]->getVertex( math::modPrev( i, size ) ),
      f[faceID]->getVertex( math::modNext( i, size ) ),
      f[faceID]->getVertex( i ) ) * amount * sign;
  }
  for ( int i = 0; i < size; i++ ) {
    v[ f[faceID]->getVertex( i ) ] = nv[i];
  }
  delete nv;
}

void Mesh::weldVertices( int a, int b ) {
  Vertex c = ( v[a] + v[b] )/2;
  for (size_t i = 0; i < f.size(); i++) {
    int indexa = -1;
    int indexb = -1;
    for (size_t j = 0; j < f[i]->size(); j++) {
      if (f[i]->getVertex(j) == b) {
        indexb = j;
      } else if (f[i]->getVertex(j) == a) {
        indexa = j;
      }
    }
    if (indexb != -1) {
      IntVector vtx = f[i]->getVertices();
      if (indexa != -1) {
        vtx.erase(vtx.begin()+indexb);
        if (f[i]->hasTexCoords()) {
          IntVector tcd = f[i]->getTexCoords();
          tcd.erase(tcd.begin()+indexb);
          f[i]->setTexCoords( tcd );
        }
      } else {
        vtx[indexb] = a;
      }
      f[i]->setVertices( vtx );
    }
  }
  v[a] = c;
  freeVertices.push_back(b);
}



Vertex Mesh::faceCenter( int faceID ) {
  Vertex c;
  size_t size = f[ faceID ]->size();
  for ( size_t i = 0; i < size; i++ )
    c = c + getFaceVertex( faceID, i );
  return c / double(size);
}

// TODO : remove hack for multifaces;
// TODO : remove hack for non-quads;
Vertex Mesh::faceNormal( int faceID ) {
  if ( f[faceID]->isMultiFace() ) return Vertex( 0.0, 0.0, 1.0 );
  if ( f[faceID]->size() != 4 ) return Vertex( 0.0, 0.0, 1.0 );
  Vertex a = getFaceEdge( faceID, 1, 0 );
  Vertex b = getFaceEdge( faceID, 2, 0 );
  Vertex r = a.cross(b);
  double length = r.length();
  return r / length;
}

IntVector* Mesh::repeatSubdivdeFace( int fid, double snap, bool horizontal ) {
  if ( f[ fid ]->size() != 4 ) {
    IntVector* result = new IntVector();
    result->push_back( fid );
    return result;
  }
  double len = horizontal ? faceH(fid) : faceV(fid);
  snap = math::refineSnap(snap,len);
  int count = math::roundToInt(len/snap);

  Vertex stepA, stepB;

  if (horizontal) {
    stepA = getFaceEdge( fid, 3, 2 ) / double(count);
    stepB = getFaceEdge( fid, 0, 1 ) / double(count);
  } else {
    stepA = getFaceEdge( fid, 0, 3 ) / double(count);
    stepB = getFaceEdge( fid, 1, 2 ) / double(count);
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->getMaterial();

  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = f[fid]->getVertex(3);
    bs = f[fid]->getVertex(0);
  } else {
    as = f[fid]->getVertex(0);
    bs = f[fid]->getVertex(1);
  }
  pai = as;
  pbi = bs;

  Vertex a = v[as];
  Vertex b = v[bs];

  for (int i = 0; i < count-1; i++) {
    a = a + stepA;
    b = b + stepB;

    ai = addVertex(a);
    bi = addVertex(b);

    Face* face = new Face();
    face->setMaterial( mat );
    face->clearVertices();
    if (horizontal) {
      face->addVertex( pbi );
      face->addVertex( bi );
      face->addVertex( ai );
      face->addVertex( pai );
    } else {
      face->addVertex( pai );
      face->addVertex( pbi );
      face->addVertex( bi );
      face->addVertex( ai );
    }
    result->push_back( addFace( face ) );

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

  if (horizontal) {
    int idx1 = f[fid]->getVertex(1);
    int idx2 = f[fid]->getVertex(2);
    f[fid]->clearVertices();
    f[fid]->addVertex( bi );
    f[fid]->addVertex( idx1 );
    f[fid]->addVertex( idx2 );
    f[fid]->addVertex( ai );
  } else {
    int idx2 = f[fid]->getVertex(2);
    int idx3 = f[fid]->getVertex(3);
    f[fid]->clearVertices();
    f[fid]->addVertex( ai );
    f[fid]->addVertex( bi );
    f[fid]->addVertex( idx2 );
    f[fid]->addVertex( idx3 );
  }
  return result;
}

IntVector* Mesh::splitFace(int fid, DoubleVector* splitData, bool horizontal, double ssnap) {
  if ( f[ fid ]->size() != 4 ) {
    IntVector* result = new IntVector();
    result->push_back( fid );
    return result;
  }
  Vertex stepA, stepB;

  DoubleVector* sdata = new DoubleVector(splitData->size());
  int splits = splitData->size()-1;

  double length = horizontal ? faceH(fid) : faceV(fid);
  double relsum = 0.0;
  double lsum = 0.0;
  for (int i = 0; i < splits+1; i++) {
    double value = splitData->at(i);
    (*sdata)[i] = value;
    if (value >= 0.0) lsum += value; else relsum += -value;
  }
  if (relsum > 0) {
    double relperunit = ( length-lsum ) / relsum;
    for (int i = 0; i < splits+1; i++) {
      double value = sdata->at(i);
      if (value < 0.0) (*sdata)[i] = -value*relperunit;
    }
  }

  double s = ssnap;
  if (ssnap > EPSILON) s = math::refineSnap(ssnap,length);

  if (horizontal) {
    stepA = getFaceEdge( fid, 3, 2 ).norm();
    stepB = getFaceEdge( fid, 0, 1 ).norm();
  } else {
    stepA = getFaceEdge( fid, 0, 3 ).norm();
    stepB = getFaceEdge( fid, 1, 2 ).norm();
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->getMaterial();

  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = f[fid]->getVertex(3);
    bs = f[fid]->getVertex(0);
  } else {
    as = f[fid]->getVertex(0);
    bs = f[fid]->getVertex(1);
  }
  pai = as;
  pbi = bs;

  Vertex a = v[as];
  Vertex b = v[bs];

  for (int i = 0; i < splits; i++) {
    a = a + stepA*sdata->at(i);
    b = b + stepB*sdata->at(i);

    if (ssnap > EPSILON) {
      Vertex A = a-v[as];
      Vertex B = b-v[bs];

      double la = A.length();
      double lb = B.length();

      Vertex sa = v[as]+(A.norm() * math::snap(la,s));
      Vertex sb = v[bs]+(B.norm() * math::snap(lb,s));

      ai = addVertex(sa);
      bi = addVertex(sb);
    } else {
      ai = addVertex(a);
      bi = addVertex(b);
    }

    Face* face = new Face();
    face->setMaterial( mat );
    face->clearVertices();
    if (horizontal) {
      face->addVertex( pbi );
      face->addVertex( bi );
      face->addVertex( ai );
      face->addVertex( pai );
    } else {
      face->addVertex( pai );
      face->addVertex( pbi );
      face->addVertex( bi );
      face->addVertex( ai );
    }
    result->push_back(addFace(face));

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

  if (horizontal) {
    int idx1 = f[fid]->getVertex(1);
    int idx2 = f[fid]->getVertex(2);
    f[fid]->clearVertices( );
    f[fid]->addVertex( bi );
    f[fid]->addVertex( idx1 );
    f[fid]->addVertex( idx2 );
    f[fid]->addVertex( ai );
  } else {
    int idx2 = f[fid]->getVertex(2);
    int idx3 = f[fid]->getVertex(3);
    f[fid]->clearVertices( );
    f[fid]->addVertex( ai );
    f[fid]->addVertex( bi );
    f[fid]->addVertex( idx2 );
    f[fid]->addVertex( idx3 );
  }

  delete sdata;
  return result;
}

void Mesh::output(Output& out, int materialCount) {
  out.meshStart();
  int mat = -1;
  for (size_t i = 0; i < inside.size(); i++) out.vertex(inside[i],"inside");
  for (size_t i = 0; i < outside.size(); i++) out.vertex(outside[i],"outside");
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texCoord(tc[i]);
  if (passable) out.meshPassable();

  for (int m = 0; m <= materialCount; m++) {
    for (size_t i = 0; i < f.size(); i++)
      if (f[i]->getMaterial() == m) {
        if (mat != m) out.matref(m);
        if (passable) out.meshPassable();
        mat = m;
        out.face(f[i],mat);
        mat = f[i]->getMaterial();
      }
  }
  out.meshEnd();
}

// TODO: handle texcoords?
// TODO: handle previous?
void Mesh::chamferFace( int faceID, double amount ) {
  IntVector old;
  int size = f[faceID]->size();
  for (int i = 0; i < size; i++) {
    old.push_back( f[faceID]->getVertex( i ) );
  }
  f[faceID]->clearVertices();
  VertexVector in;
  VertexVector out;
  for (int i = 0; i < size; i++) {
    in.push_back(v[old[i]]);
  }
  for (int i = 0; i < size; i++) {
    Vertex a = in[i] - in[ math::modPrev( i, size ) ];
    Vertex b = in[i] - in[ math::modNext( i, size ) ];
    double af = ( a.length() - amount) / a.length();
    double bf = ( b.length() - amount) / b.length();
    a = in[ math::modPrev( i, size ) ] + a * af;
    b = in[ math::modNext( i, size ) ] + b * bf;
    v[old[i]] = a;
    f[faceID]->addVertex( old[i] );
    f[faceID]->addVertex( addVertex(b) );
  }
}

// TODO: these coords could be reused!
void Mesh::textureFaceFull( int faceID ) {
  f[faceID]->clearTexCoords();
  f[faceID]->addTexCoord( addTexCoord( TexCoord( 0.0, 0.0 ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( 1.0, 0.0 ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( 1.0, 1.0 ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( 0.0, 1.0 ) ) );
}

void Mesh::textureFaceQuad( int faceID, double au, double av, double bu, double bv ) {
  f[faceID]->clearTexCoords();
  f[faceID]->addTexCoord( addTexCoord( TexCoord( au, av ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( bu, av ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( bu, bv ) ) );
  f[faceID]->addTexCoord( addTexCoord( TexCoord( au, bv ) ) );
}

void Mesh::textureFace( int faceID, double snap, double tile ) {
  textureFaceQuad( faceID, 0.0, 0.0,
    math::roundToInt( faceH( faceID ) / snap ) * tile,
    math::roundToInt( faceV( faceID ) / snap ) * tile);
}

void Mesh::freeFace( int faceID ) {
  f[faceID]->setOutput( false );
  for ( size_t i = 0; i < f[faceID]->size(); i++ ) {
    freeVertices.push_back( f[faceID]->getVertex( i ) );
  }
}

String Mesh::faceToString(Face* face) {
  String result = "Face: ( ";
  for ( size_t i = 0; i < face->size(); i++ )
    result += v[ face->getVertex( i ) ].toString() + " ";
  result += ")";
  return result;
}
void Mesh::pushBase( int faceID ) {
  vbase.clear();
  for ( size_t i = 0; i < f[faceID]->size(); i++ ) {
    vbase.push_back( v[ f[faceID]->getVertex( i ) ] );
  }
}

int Mesh::rePushBase( ) {
  Face* clone = new Face();
  int fsize = vbase.size();
  for (int i = 0; i < fsize; i++) {
    clone->addVertex( addVertex( vbase[i] ) );
  }
  return addFace(clone);
}


Mesh::~Mesh() {
  for ( FaceVectIter itr = f.begin(); itr!= f.end(); ++itr )
    delete (*itr);
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
