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

#include "Mesh.h"

int Mesh::addVertex(Vertex vtx) { 
  if (freeVertices.size() > 0) {
    int free = freeVertices[freeVertices.size()-1];
    freeVertices.pop_back();
    v[free] = vtx;
    return free;
  } else {
    v.push_back(vtx); 
    return v.size()-1; 
  }
}

int Mesh::addTexCoord(TexCoord tcx) { 
  for (size_t i = 0; i < tc.size(); i++) {
    if (math::equals(tc[i].x,tcx.x) && math::equals(tc[i].y,tcx.y)) return i;
  }
  tc.push_back(tcx); 
  return tc.size()-1; 
}

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, int mat) {
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  int size = v.size();
  Face* face = new Face();
  for (size_t i = 4; i > 0; i--) {
    face->addVertex(size-i);
  }
  face->setMaterial(mat);
  return addFace(face);
}

int Mesh::createNewFace(Vertex a, Vertex b, Vertex c, Vertex d, TexCoord tca, TexCoord tcb, TexCoord tcc, TexCoord tcd, int mat) {
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  v.push_back(d);
  int size = v.size();
  tc.push_back(tca);
  tc.push_back(tcb);
  tc.push_back(tcc);
  tc.push_back(tcd);
  int tsize = tc.size();
  Face* face = new Face();
  for (size_t i = 4; i > 0; i--) {
    face->addVertex(size-i,tsize-i);
  }
  face->setMaterial(mat);
  return addFace(face);
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


IntVector* Mesh::extrudeFaceR(int fid, double amount, int mat) {
  Vertex dir = faceNormal(fid)*amount;
  IntVector base    = f[fid]->vtx;
  IntVector* result = new IntVector;
  IntVector top;

  int size = base.size();

  for (int i = 0; i < size; i++) {
    top.push_back(addVertex(v[base.at(i)]+dir));
  }
  f[fid]->vtx = top;

  for (int i = 0; i < size; i++) {
    Face* face = new Face();
    face->setMaterial(mat);
    face->addVertex( base.at(i) );
    face->addVertex( base.at(math::modNext(i,size)) );
    face->addVertex( top.at(math::modNext(i,size)) );
    face->addVertex( top.at(i) );
    result->push_back( addFace( face ) );
  }

  return result;
}

void Mesh::extrudeFace(int fid, double amount, int mat) {
  Vertex dir = faceNormal(fid)*amount;
  IntVector base   = f[fid]->vtx;
  IntVector top;

  int size = base.size();

  for (int i = 0; i < size; i++) {
    top.push_back(addVertex(v[base.at(i)]+dir));
  }
  f[fid]->vtx = top;

  for (int i = 0; i < size; i++) {
    Face* face = new Face();
    face->setMaterial(mat);
    face->addVertex( base.at(i) );
    face->addVertex( base.at(math::modNext(i,size)) );
    face->addVertex( top.at(math::modNext(i,size)) );
    face->addVertex( top.at(i) );
    addFace( face );
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

void Mesh::taperFace(int fid, double amount) {
  int size = f[fid]->size();
  Vertex c = faceCenter(fid);
  for (int i = 0; i < size; i++) {
    Vertex vv = v[f[fid]->vtx.at(i)];
    v[f[fid]->vtx.at(i)] = (vv - c)*amount+c;
  }
}

void Mesh::scaleFace(int fid, double x, double y) {
  int size = f[fid]->size();
  Vertex c = faceCenter(fid);
  for (int i = 0; i < size; i++) {
    Vertex vc = v[f[fid]->vtx.at(i)] - c;
    int vertexID = f[fid]->getVertex(i);
    v[vertexID].x = vc.x*x+c.x;
    v[vertexID].y = vc.y*y+c.y;
  }
}

void Mesh::translateFace(int fid, double x, double y, double z) {
  int size = f[fid]->size();
  for (int i = 0; i < size; i++) {
    int vertexID = f[fid]->getVertex(i);
    v[vertexID].x += x;
    v[vertexID].y += y;
    v[vertexID].z += z;
  }
}


void Mesh::expandFace(int fid, double amount) {
  Vertex normal = faceNormal(fid);
  // needs to be uniform
  int size = f[fid]->size();
  Vertex* nv = new Vertex[size];
  for (int i = 0; i < size; i++) {
    Vertex a = v[f[fid]->vtx.at(math::modNext(i,size))] - v[f[fid]->vtx.at(math::modPrev(i,size))];
    Vertex b = v[f[fid]->vtx.at(i)] - v[f[fid]->vtx.at(math::modPrev(i,size))];
    double sign = math::sign( b.cross(a).dot(normal) );
    nv[i] = v[f[fid]->vtx.at(i)] + extensionVertex( f[fid]->vtx.at(math::modPrev(i,size)) , f[fid]->vtx.at(math::modNext(i,size)), f[fid]->vtx.at(i) ) *amount*sign;
  }
  for (int i = 0; i < size; i++) {
    v[f[fid]->vtx.at(i)] = nv[i];
  }
  delete nv;
}

void Mesh::weldVertices(int a, int b, Vertex vx) {
  for (size_t i = 0; i < f.size(); i++) {
    int indexa = -1;
    int indexb = -1;
    for (size_t j = 0; j < f[i]->vtx.size(); j++) {    
      if (f[i]->vtx.at(j) == b) {
        indexb = j;
      } else if (f[i]->vtx.at(j) == a) {
        indexa = j;
      }
    }
    if (indexb != -1) {
      if (indexa != -1) {
        f[i]->vtx.erase(f[i]->vtx.begin()+indexb);
        if (f[i]->hasTexCoords()) f[i]->tcd.erase(f[i]->vtx.begin()+indexb);  
      } else {
        f[i]->vtx[indexb] = a;
      }
    }
  }
  v[a] = vx;  
  freeVertices.push_back(b);
}

void Mesh::weldVertices(int a, int b) {
  Vertex c = (v[a]+v[b])/2;
  weldVertices(a,b,c);
}



Vertex Mesh::faceCenter(int fid) {
  Face* face = f[fid];
  return (v[face->vtx.at(0)]+v[face->vtx.at(1)]+v[face->vtx.at(2)]+v[face->vtx.at(3)])/4;
}

// TODO : remove hack for multifaces;
Vertex Mesh::faceNormal(int fid) {
  Face* face = f[fid];
  if (face->isMultiFace()) return Vertex(0.0,0.0,1.0);
  Vertex a = v[face->vtx.at(0)]-v[face->vtx.at(1)];
  Vertex b = v[face->vtx.at(0)]-v[face->vtx.at(2)];
  Vertex r = a.cross(b);
  double length = r.length();
  return r / length;
}

IntVector* Mesh::repeatSubdivdeFace(int fid, double snap, bool horizontal) {
  double len = horizontal ? faceH(fid) : faceV(fid);
  snap = math::refineSnap(snap,len);
  int count = math::roundToInt(len/snap);

  Vertex stepA, stepB;

  if (horizontal) {
    stepA = (v[f[fid]->vtx.at(2)]-v[f[fid]->vtx.at(3)]) / double(count);
    stepB = (v[f[fid]->vtx.at(1)]-v[f[fid]->vtx.at(0)]) / double(count);
  } else {
    stepA = (v[f[fid]->vtx.at(3)]-v[f[fid]->vtx.at(0)]) / double(count);
    stepB = (v[f[fid]->vtx.at(2)]-v[f[fid]->vtx.at(1)]) / double(count);
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->getMaterial();
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = f[fid]->vtx.at(3);
    bs = f[fid]->vtx.at(0);
  } else {
    as = f[fid]->vtx.at(0);
    bs = f[fid]->vtx.at(1);
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
    if (horizontal) {
      face->set4(pbi,bi,ai,pai);
    } else {
      face->set4(pai,pbi,bi,ai);
    }
    result->push_back( addFace( face ) );

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

  if (horizontal) {
    //f[fid]->vtx.clear(); DEBUG?
    f[fid]->set4(bi,f[fid]->vtx.at(1),f[fid]->vtx.at(2),ai);
    //f[fid]->vtx.push_back( bi );
    //f[fid]->vtx.push_back( f[fid]->vtx.at(1) );
    //f[fid]->vtx.push_back( f[fid]->vtx.at(2) );
    //f[fid]->vtx.push_back( ai );
  } else {
    //f[fid]->vtx.clear();
    //f[fid]->vtx.push_back( ai );
    //f[fid]->vtx.push_back( bi );
    //f[fid]->vtx.push_back( f[fid]->vtx.at(2) );
    //f[fid]->vtx.push_back( f[fid]->vtx.at(3) );
    f[fid]->set4(ai,bi,f[fid]->vtx.at(2),f[fid]->vtx.at(3));
  }
  return result;
}

IntVector* Mesh::splitFace(int fid, DoubleVector* splitData, bool horizontal, double ssnap) {
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
    stepA = (v[f[fid]->vtx.at(2)]-v[f[fid]->vtx.at(3)]).norm();
    stepB = (v[f[fid]->vtx.at(1)]-v[f[fid]->vtx.at(0)]).norm();
  } else {
    stepA = (v[f[fid]->vtx.at(3)]-v[f[fid]->vtx.at(0)]).norm();
    stepB = (v[f[fid]->vtx.at(2)]-v[f[fid]->vtx.at(1)]).norm();
  }

  IntVector* result = new IntVector();

  int mat = f[fid]->getMaterial();
  
  int ai = 0 , bi = 0;
  int pai = 0, pbi = 0;
  int as = 0, bs = 0;

  if (horizontal) {
    as = f[fid]->vtx.at(3);
    bs = f[fid]->vtx.at(0);
  } else {
    as = f[fid]->vtx.at(0);
    bs = f[fid]->vtx.at(1);
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
    if (horizontal) {
      face->set4(pbi,bi,ai,pai);
    } else {
      face->set4(pai,pbi,bi,ai);
    }
    result->push_back(addFace(face));

    pai = ai;
    pbi = bi;
  }

  result->push_back(fid);

   if (horizontal) {
    f[fid]->set4(bi,f[fid]->vtx.at(1),f[fid]->vtx.at(2),ai);
   } else {
    f[fid]->set4(ai,bi,f[fid]->vtx.at(2),f[fid]->vtx.at(3));
   }
/*  if (horizontal) {
    f[fid]->clear();
    f[fid]->addVertex( bi );
    f[fid]->addVertex( f[fid]->vtx.at(1) );
    f[fid]->addVertex( f[fid]->vtx.at(2) );
    f[fid]->addVertex( ai );
  } else {
    f[fid]->clear();
    f[fid]->addVertex( ai );
    f[fid]->addVertex( bi );
    f[fid]->addVertex( f[fid]->vtx.at(2) );
    f[fid]->addVertex( f[fid]->vtx.at(3) );
  }*/

  delete sdata;
  return result;
}

void Mesh::output(Output& out, int materialCount) {
  out.line("mesh");
  int mat = -1;
  for (size_t i = 0; i < inside.size(); i++) out.vertex(inside[i],"inside");
  for (size_t i = 0; i < outside.size(); i++) out.vertex(outside[i],"outside");
  for (size_t i = 0; i < v.size(); i++) out.vertex(v[i]);
  for (size_t i = 0; i < tc.size(); i++) out.texcoord(tc[i]);
  if (passable) out.line("  passable"); 

  for (int m = 0; m <= materialCount; m++) {
    for (size_t i = 0; i < f.size(); i++) 
      if (f[i]->getMaterial() == m) {
        if (mat != m) out.matref(m);
        if (passable) out.line("  passable"); 
        mat = m;
        out.face(f[i],mat);
        mat = f[i]->getMaterial();
      }
  }
  out.line("end\n");
}

// TODO: handle texcoords?
// TODO: handle previous?
void Mesh::chamferFace(int fid, double amount) {
  IntVector old;
  int size = f[fid]->vtx.size();
  for (int i = 0; i < size; i++) {
    old.push_back(f[fid]->vtx.at(i));
  }
  f[fid]->vtx.clear();
  VertexVector in;
  VertexVector out;
  for (int i = 0; i < size; i++) {
    in.push_back(v[old[i]]);
  }
  for (int i = 0; i < size; i++) {
    Vertex a = in[i]-in[math::modPrev(i,size)];
    Vertex b = in[i]-in[math::modNext(i,size)];
    double af = (a.length()-amount) / a.length();
    double bf = (b.length()-amount) / b.length();
    a = in[math::modPrev(i,size)]+a*af;
    b = in[math::modNext(i,size)]+b*bf;
    v[old[i]] = a;
    f[fid]->vtx.push_back(old[i]);
    f[fid]->vtx.push_back(addVertex(b));
  }
}

// TODO: these coords could be reused!
void Mesh::textureFaceFull(int fid) {
  f[fid]->clearTexCoords();
  f[fid]->addTexCoord(addTexCoord(TexCoord(0.0,0.0)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(1.0,0.0)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(1.0,1.0)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(0.0,1.0)));
}

void Mesh::textureFaceQuad(int fid, double au, double av, double bu, double bv) {
  f[fid]->clearTexCoords();
  f[fid]->addTexCoord(addTexCoord(TexCoord(au,av)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(bu,av)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(bu,bv)));
  f[fid]->addTexCoord(addTexCoord(TexCoord(au,bv)));
}

void Mesh::textureFace(int fid, double snap, double tile) {
  textureFaceQuad(fid,0.0,0.0,math::roundToInt(faceH(fid)/snap)*tile,math::roundToInt(faceV(fid)/snap)*tile);
}

void Mesh::freeFace(int fid) {
  f[fid]->setOutput( false );
  for (size_t i = 0; i < f[fid]->vtx.size(); i++) {
    freeVertices.push_back(f[fid]->vtx.at(i));
  }
}

String Mesh::faceToString(Face* face) { 
  String result = "Face: ( ";
  for (size_t i = 0; i < face->vtx.size(); i++)
    result += v[face->vtx.at(i)].toString()+ " ";
  result += ")";
  return result;
}
void Mesh::pushBase(int fid) {
  vbase.clear();
  for (int i = 0; i < f[fid]->size(); i++) {
    vbase.push_back(v[f[fid]->getVertex(i)]);
  }
}

Mesh::~Mesh() {
  for (FaceVectIter itr = f.begin(); itr!= f.end(); ++itr) delete (*itr);
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
