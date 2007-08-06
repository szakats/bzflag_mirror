/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "BuildZone.h"

#define INSET 1.0f

BuildZone::BuildZone(Coord2D a, Coord2D b, int astep) : Zone(a,b,astep)
{
  Vertex corners[4];
  corners[0] = Vertex((float)A.x,(float)A.y,0.2f);
  corners[1] = Vertex((float)B.x,(float)A.y,0.2f);
  corners[2] = Vertex((float)B.x,(float)B.y,0.2f);
  corners[3] = Vertex((float)A.x,(float)B.y,0.2f);

  int size = abs(A.x-B.x)*abs(A.y-B.y);
  int wall;
  int height;
  float hlev = 3.7f;
  if (rand()%2 == 0 && size < 5000) {
    wall = MATGLASS;
    height = rand()%6+4;
    hlev = 8.0f;
  } else if (rand()%2 == 0) {
    wall = MATWALL;
    height = rand()%3+1;
  } else {
    wall = MATWALL2;
    height = rand()%6+1;
  }

  Vertex insetX = Vertex(INSET,0.0f,0.0f);
  Vertex insetY = Vertex(0.0f,INSET,0.0f);

  Face* swface = new Face();
  swface->mat = MATROOF;
  swface->addVertex(mesh.addVertex(corners[0]+insetY));
  swface->addVertex(mesh.addVertex(corners[0]+insetX));
  swface->addVertex(mesh.addVertex(corners[1]-insetX));
  swface->addVertex(mesh.addVertex(corners[1]+insetY));
  swface->addVertex(mesh.addVertex(corners[2]-insetY));
  swface->addVertex(mesh.addVertex(corners[2]-insetX));
  swface->addVertex(mesh.addVertex(corners[3]+insetX));
  swface->addVertex(mesh.addVertex(corners[3]-insetY));

  int base = mesh.addFace(swface);
  mesh.expandFace(base,1.0f);
  mesh.extrudeFace(base,0.2f,MATMESH);
  mesh.extrudeFace(base,0.0f,MATMESH);
  if (wall == MATGLASS) {
    mesh.expandFace(base,-3.0f);
  } else {
    mesh.expandFace(base,-1.7f);
  }

  for (int i = 0; i < 4; i++) {
    mesh.weldVertices((*mesh.f[base]->vtx)[i],(*mesh.f[base]->vtx)[i+1],corners[i]+Vertex(0.0f,0.0f,0.2f));
  }

  if (wall == MATGLASS) {
    mesh.expandFace(base,-2.0f);
  } else {
    mesh.expandFace(base,-0.7f);
  }

  mesh.inside.push_back(mesh.faceCenter(base)+mesh.faceNormal(base));

  IntVector* fs;

  for (int i = 0; i < height; i++) {
    fs = mesh.extrudeFaceR(base,hlev,wall);
    for (int j = 0; j < 4; j++) {
      if (wall == MATWALL2) {
      Vertex vv = mesh.v[mesh.f[fs->at(j)]->vtx->at(0)]-mesh.v[mesh.f[fs->at(j)]->vtx->at(1)];
      int sdcount = int(vv.length()/4.0f);
        IntVector* fcs = mesh.subdivdeFace(fs->at(j),sdcount,true);
	for (int k = 0; k < int(fcs->size()); k++) {
	  mesh.extrudeFace(fcs->at(k),0.0f,wall);
	  mesh.expandFace(fcs->at(k),-0.4f);
	  mesh.extrudeFace(fcs->at(k),-0.2f,wall);
	}
      delete fcs;
      }
    }
    delete fs;
    if (i == height-1) break;

    if (wall != MATGLASS) {
      mesh.extrudeFace(base,0.0f,MATMESH);
      mesh.expandFace(base,0.15f);
      mesh.extrudeFace(base,0.3f,MATMESH);
      mesh.extrudeFace(base,0.0f,MATMESH);
      mesh.expandFace(base,-0.15f);
    }
  }

  if (wall == MATWALL && rand()%2 == 0 && height <= 3 && size < 5000) {
    mesh.extrudeFace(base,0.0f,MATROOF);
    mesh.expandFace(base,0.3f);
    mesh.extrudeFace(base,4.0f,MATROOFT);
    mesh.expandFace(base,-4.0f);

    return;
  }


  mesh.extrudeFace(base,0.0f,MATMESH);
  mesh.expandFace(base,0.2f);
  mesh.extrudeFace(base,0.5f,MATMESH);

  if (wall == MATWALL && rand()%2 == 0) {
    height = rand()%3+1;
    Vertex vh = mesh.v[mesh.f[base]->vtx->at(0)]-mesh.v[mesh.f[base]->vtx->at(1)];
    Vertex vv = mesh.v[mesh.f[base]->vtx->at(3)]-mesh.v[mesh.f[base]->vtx->at(0)];

    bool horiz = vh.length() > vv.length();
    float l;

    if (horiz) {
      l = vh.length();
    } else {
      l = vv.length();
    }
    
    l /= 2.0f;
    

    int newbase = mesh.partitionFace(base,l,horiz);
    if (rand()%2 == 0) {
      base = newbase;
    } else {
    }
    mesh.extrudeFace(base,0.0f,MATMESH);
    mesh.expandFace(base,-0.2f);

    for (int i = 0; i < height; i++) {
      mesh.extrudeFace(base,3.7f,wall);
      if (i == height-1) break;
      mesh.extrudeFace(base,0.0f,MATMESH);
      mesh.expandFace(base,0.15f);
      mesh.extrudeFace(base,0.3f,MATMESH);
      mesh.extrudeFace(base,0.0f,MATMESH);
      mesh.expandFace(base,-0.15f);
    }
    mesh.extrudeFace(base,0.0f,MATMESH);
    mesh.expandFace(base,0.2f);
    mesh.extrudeFace(base,0.5f,MATMESH);
  } 
}

void BuildZone::output(Output& out) 
{
  mesh.output(out);
/*   out << "  color 0."<< rand()%10+80 << " 0." << rand()%20+80 << " 0."<< rand()%20+80<< " 1.0\n";
*/
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
