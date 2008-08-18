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

#include "MultiFace.h"

void MultiFace::updateFaces(double z) {
  for ( size_t fi = 0; fi < comps->size(); fi++ ) {
    Face* f = comps->at( fi );
    IntVector vtx = f->getVertices();
    for ( size_t t = 0; t < f->size(); t++ ) {
      if (f->getTexCoord(t) < 0) {
        int vi = f->getCyclicVertex( t );
        Vertex v = mesh->getVertex( vi );
        v.z = z;
        mesh->substituteVertex( vi, v );
      } else {
        vtx[t] = getCyclicVertex( f->getTexCoord( t ) );
      }
    }
    f->setVertices( vtx );
  }
}

void MultiFace::storeFaces() {
  for (size_t fi = 0; fi < comps->size(); fi++) {
    Face* f = comps->at(fi);
    f->clearTexCoords();
    for (size_t t = 0; t < f->size(); t++) {
      f->addTexCoord( getVertexIndex( f->getCyclicVertex(t) ) );
    }
  }
}


IntVector* MultiFace::detachFace(int id) {
  printf("Detach face\n");
  if (comps->size() < 2) return NULL;
  if (id >= int(comps->size())) return NULL;
  updateFaces(mesh->getVertex(getVertex(0)).z);
  
  IntVector* result  = new IntVector();
  IntVector* visited = new IntVector();

  Face* f = comps->at(id);

  int index = pickRemovalIndex(comps->at(id),visited);

  while (index >= 0) {
    printf("Element iteration...\n");
    IntVector nvtx;

    int vid  = getCyclicVertex(index);
    int svid = getCyclicVertex(index);
    int lastgindex = index;

    do {
      int gindex = getVertexIndex(vid);
      printf("Vertex iteration...(%d,index = %d)\n",vid,gindex);
      if (gindex != -1) lastgindex = gindex;
      int findex = f->getVertexIndex(vid);
      int oindex;
      Face* oface = getOtherFaceWithVertex(f,vid);
      oindex = (oface == NULL) ? -1 : oface->getVertexIndex(vid);

      visited->push_back(vid);
      nvtx.push_back(vid);

      if (oindex < 0 ) {
        printf("Remove (%d)\n",vid);
        removeVertex(gindex);
        vid = f->getCyclicVertex(findex+1);
      } else {
        int nextovid = oface->getCyclicVertex(oindex-1);
        if (findex < 0) {
          if (gindex < 0) insertVertexBefore(lastgindex,vid);
          vid = nextovid;
        } else {
          int nextfvid = f->getCyclicVertex(findex+1);
          int prevfvid = f->getCyclicVertex(findex-1);
          int prevovid = oface->getCyclicVertex(oindex-1);
          if (nextovid == nextfvid) {
            vid = nextfvid;
          } else if (prevfvid == nextovid) {
            vid = nextfvid;
          } else {
            vid = isLeftOfVectors(prevovid,prevfvid,vid,nextfvid) ? nextfvid : nextovid;
          }
        }
      }
    } while (vid != svid);

    Face* nface = new Face();
    nface->setMaterial( getMaterial() );
    nface->setVertices( nvtx );
    result->push_back(mesh->addFace(nface));
    index = pickRemovalIndex(comps->at(id),visited);
  }

  printf("Cleaning up\n");
  delete visited;

  comps->erase(comps->begin() + id);

  if (result->size() == 0) {
    printf("Result Zero\n");
    delete result;
    result = NULL;
  }
  printf("Storing faces\n");
  storeFaces();
  printf("Done\n");
  return result;
}

bool MultiFace::isLeftOfVectors(int x, int a, int b, int c) {
  Vertex P = mesh->getVertex(b);
  Vertex A = (P-mesh->getVertex(a)).norm();
  Vertex Xline = A+(mesh->getVertex(x)-P).norm();
  Vertex Pline = A+(mesh->getVertex(c)-P).norm();

  Vertex normal = Vertex(0.0,0.0,1.0);//faceNormal(fid);

  return math::sign( Xline.cross(Pline).dot(normal) ) >= 0;
}

// BUUUUUG, doesn't handle inter-face jumps when doing the cut edge!

Face* MultiFace::getOtherFaceWithVertex(Face* f, int vid) {
  for (size_t fi = 0; fi < comps->size(); fi++) {
    if (comps->at(fi) != f && comps->at(fi)->hasVertex(vid)) return comps->at(fi);
  }
  return NULL;
}

int MultiFace::pickRemovalIndex( Face *f, IntVector* visited ) {
  for ( size_t i = 0; i < size(); i++ ) {
    if ( f->hasVertex( getCyclicVertex( i ) ) ) {
      bool found = false;
      if ( visited->size() > 0 ) 
        for ( size_t k = 0; k < visited->size(); k++ )
          if ( visited->at( k ) == getCyclicVertex( i ) ) {
            found = true;
            break;
          }
      if ( !found ) return i;
    } 
  }
  return -1;
}

void MultiFace::refineFace( Face* f, Face* target ) {
  size_t i = 0;
  while ( i < f->size() ) {
    //printf("Intersect... (%d,%d)\n",i,modnext(i,f->size()));
    //      printf("Multi%s\n",mesh->faceToString(this).c_str());
    Vector2Dd ipoint;
    int index;
    Vector2Dd fvi     = mesh->getVertex( f->getCyclicVertex(i) ).toVector2D();
    Vector2Dd fvinext = mesh->getVertex( f->getCyclicVertex(i+1) ).toVector2D();

    if (vertexNearestIntersect(fvi,fvinext,ipoint,index,target)) {
      //printf("Nearerst found... (%d) %s : ",index,ipoint.toString().c_str());
      if (fvinext.equals(mesh->getVertex( target->getCyclicVertex( index+1 ) ).toVector2D())) {
        //printf("Is common\n");
      } else if (ipoint.equals(fvinext)) {
        //printf("Is samepoint with next\n");
        target->insertVertexAfter(index,f->getCyclicVertex(i+1));
      } else if (ipoint.equals(mesh->getVertex( target->getCyclicVertex(index+1) ).toVector2D())) {
        //printf("Is samepoint with itself\n");
        f->insertVertexAfter(i,target->getCyclicVertex(index+1));
      } else {
        //printf("Is normal\n");
        int ipid = mesh->addVertex( Vertex( ipoint.x, ipoint.y, mesh->getVertex( f->getCyclicVertex( i ) ).z ) );
        f->insertVertexAfter(i,ipid);
        target->insertVertexAfter(index,ipid);
      }
    }
    i++;
  }
}


int MultiFace::addFace( Face* f ) {
  //printf("Addface start... (%d,%d)\n",size(),f->size());
  //    printf("Multi%s\n",mesh->faceToString(this).c_str());
  //    printf("Add%s\n",mesh->faceToString(f).c_str());
  f->setOutput( false );
  if ( comps->size() == 0 ) {
    for ( size_t i = 0; i < f->size(); i++ ) 
      vtx.push_back( f->getVertex( i ) );
  } else {
    refineFace(f,this);
    for (size_t i = 0; i < comps->size(); i++) refineFace(f,comps->at(i));
    int fsize = f->size();
    int tsize = size();
    //printf("Refined... (%d,%d)\n",tsize,fsize);
    IntVector newvtx;
    int index = 0;
    for (index = 0; index < fsize; index++) 
      if (getVertexIndex(f->getCyclicVertex(index)) < 0 && !vertexInside(f->getCyclicVertex(index))) break;
    // check if no vertices are inside?
    int end = index;
    bool newf = true;
    bool first = true;
    int getvert;
    int next;
    while (first || !newf || (index != end)) {
      //printf("--- %d,%d\n",index,newf);
      first = false;
      if (newf) {
        newvtx.push_back(f->getCyclicVertex(index));
	next = math::modNext(index,fsize);
        getvert = getVertexIndex(f->getCyclicVertex(next));
      } else {
        newvtx.push_back(getCyclicVertex(index));
        next = math::modNext(index,tsize);
        getvert = f->getVertexIndex(getCyclicVertex(next));
      }
      if (getvert != -1) {
        newf = !newf;
        index = getvert;
      } else index = next;
    }
    vtx = newvtx;
  }
  comps->push_back(f);
  storeFaces();
  //printf("Addface end... (%d,%d)\n",size(),f->size());
  return comps->size()-1;
}


bool MultiFace::vertexInside( int vid ) {
  Vector2Dd A = mesh->getVertex( vid ).toVector2D();
  Vector2Dd B( 100000.0, 200000.0 ); // sufficient to be out of range

  int count = 0;
  for ( size_t i = 0; i < size(); i++ ) {
    if ( math::intersect2D( A, B, mesh->getVertex( getCyclicVertex(i) ).toVector2D(),
                                  mesh->getVertex( getCyclicVertex(i+1) ).toVector2D())
       ) 
      count++;
  }      
  return ( count % 2 == 1);
}


bool MultiFace::vertexNearestIntersect(const Vector2Dd A, const Vector2Dd B, Vector2Dd &P, int &index, Face* face) {
  double length = (A-B).length();
  size_t tsize = face->size();
  double distance = length + 1.0;
  Vector2Dd R1;
  Vector2Dd R2;
  for ( size_t i = 0; i < tsize; i++ ) {
    Vector2Dd C = mesh->getVertex( face->getCyclicVertex( i ) ).toVector2D();
    Vector2Dd D = mesh->getVertex( face->getCyclicVertex(i+1) ).toVector2D();
    int r = math::intersect2D(A,B,C,D,R1,R2);
    if (r > 0) {
      if (!R1.equals(C)) {
        double thisdistance = (A-R1).length();
        if (!math::isZero(thisdistance)) {
//          printf("ICH:%s\n",R1.toString().c_str());
          if (thisdistance < distance) {
            distance = thisdistance;
            P = R1;
            index = i;
          }
        }
      }
    }
    if (r == 2) {
      if (!R2.equals(C)) {
        double thisdistance = (A-R2).length();
        if (math::isZero(thisdistance)) continue;
//        printf("ICH(2):%s\n",R2.toString().c_str());
        if (thisdistance < distance) {
          distance = thisdistance;
          P = R2;
          index = i;
        }
      }
    }
  }      
  return distance <= length+EPSILON;
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
