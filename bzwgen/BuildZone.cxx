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

BuildZone::BuildZone(Generator* _generator, Coord2D a, Coord2D b, int astep) : Zone(_generator,a,b,astep)
{
  Mesh* mesh = new Mesh();
  meshes.push_back(mesh);

  Vertex corners[4];
  corners[0] = Vertex((float)A.x,(float)A.y,0.2f);
  corners[1] = Vertex((float)B.x,(float)A.y,0.2f);
  corners[2] = Vertex((float)B.x,(float)B.y,0.2f);
  corners[3] = Vertex((float)A.x,(float)B.y,0.2f);

  int wall;
  int height;
  float hlev = 3.7f;
  if (randomBool() && size() < 5000) {
    wall = MATGLASS;
    height = randomInt(6)+4;
    hlev = 8.0f;
  } else if (randomBool()) {
    wall = MATWALL;
    height = randomInt(3)+1;
  } else {
    wall = MATWALL2;
    height = randomInt(6)+1;
  }

  /* SIDEWALK */
  Vertex insetX = Vertex(INSET,0.0f,0.0f);
  Vertex insetY = Vertex(0.0f,INSET,0.0f);

  Face* swface = new Face();
  swface->mat = MATROOF;
  swface->addVertex(mesh->addVertex(corners[0]+insetY));
  swface->addVertex(mesh->addVertex(corners[0]+insetX));
  swface->addVertex(mesh->addVertex(corners[1]-insetX));
  swface->addVertex(mesh->addVertex(corners[1]+insetY));
  swface->addVertex(mesh->addVertex(corners[2]-insetY));
  swface->addVertex(mesh->addVertex(corners[2]-insetX));
  swface->addVertex(mesh->addVertex(corners[3]+insetX));
  swface->addVertex(mesh->addVertex(corners[3]-insetY));

  int base = mesh->addFace(swface);
  mesh->expandFace(base,1.0f);
  mesh->extrudeFace(base,0.2f,MATMESH);
  mesh->extrudeFace(base,0.0f,MATMESH);

  for (int i = 0; i < 4; i++) {
    mesh->weldVertices((*mesh->f[base]->vtx)[i],(*mesh->f[base]->vtx)[i+1],corners[i]+Vertex(0.0f,0.0f,0.2f));
  }

  if (randomChance(70)) {
    std::string name = "building";
    mesh->inside.push_back(mesh->faceCenter(base));
    generator->getRuleSet()->runMesh(mesh,base,name);
    return;
  }

  if (wall == MATGLASS) {
    mesh->expandFace(base,-2.0f);
  } else {
    mesh->expandFace(base,-0.7f);
  }

  bool horiz;
  float length;

  longerSide(mesh,base,&length,&horiz);

  if (length > 100.f && randomBool()) {
    int first = mesh->partitionFace(base,length/2-2.5f,horiz);
    mesh->partitionFace(base,5.0,horiz);
    generateBuilding(mesh,first,wall); 
  } 
  generateBuilding(mesh,base,wall); 
}

void BuildZone::generateSkyscraper(Mesh* mesh, int base, int wall) {
  int height = randomInt(6)+4;
  int high = false;
  if (randomChance(33)) high = true;
  float hlev = 8.0f;
  if (!high) {
    IntVector* sides;
    sides = mesh->extrudeFaceR(base,hlev*float(height),wall);
    for(size_t i = 0; i < sides->size(); i++) {
	    IntVector* fcs = mesh->subdivdeFace((*sides)[i],3,true);
	    mesh->extrudeFace((*fcs)[1],-5.0,wall);
      delete fcs;
    }
    delete sides;
    if (!high) addDivider(mesh,base,0.2f,0.5f,MATMESH,true);
  } else {
    while(height > 0) {
      height = randomInt(5)+2;
      mesh->extrudeFaceR(base,hlev*float(height),wall);
      if (randomChance(33)) return; 
      bool horiz;
      float length;
      longerSide(mesh,base,&length,&horiz);
      float divlength = length * 0.66f;
      if (randomBool()) {
	      base = mesh->partitionFace(base,divlength,horiz);
      } else {
	      mesh->partitionFace(base,length-divlength,horiz);
      }
    }
  }
}

void BuildZone::generateBuilding(Mesh* mesh, int base, int wall) {

  mesh->inside.push_back(mesh->faceCenter(base)+mesh->faceNormal(base));

  if (wall == MATGLASS) {
    generateSkyscraper(mesh,base,wall);
    return;
  }

  int height = 1;
  float hlev = 3.7f;
  if (wall == MATWALL) {
    height = randomInt(3)+1;
  } else if (wall == MATWALL2) {
    height = randomInt(6)+1;
  }

  float length;
  bool horiz;
  
  IntVector* fs;

  for (int i = 0; i < height; i++) {
    if (wall == MATWALL2) {
      fs = mesh->extrudeFaceR(base,hlev,wall);
      for (int j = 0; j < 4; j++) {
	      subdivideWindows(mesh,fs->at(j),wall);
      }
      delete fs;
    } else {
      mesh->extrudeFace(base,hlev,wall);
    }

    if (i == height-1) break;

    addDivider(mesh,base,0.15f,0.3f,MATMESH);
  }

  if (wall == MATWALL && randomBool() && height <= 3 && size() < 5000) {
    mesh->extrudeFace(base,0.0f,MATROOF);
    mesh->expandFace(base,0.3f);
    mesh->extrudeFace(base,4.0f,MATROOFT);
    mesh->expandFace(base,-4.0f);
    return;
  }

  addDivider(mesh,base,0.2f,0.5f,MATMESH,true);

  if (wall == MATWALL && randomBool()) {
    height = randomInt(3)+1;

    longerSide(mesh,base,&length,&horiz);
    
    length *= 0.4f+(0.1f*float(randomInt(3)));
    
    int newbase = mesh->partitionFace(base,length,horiz);
    if (randomBool()) {
      base = newbase;
    } else {
    }

    mesh->extrudeFace(base,0.0f,MATMESH);
    mesh->expandFace(base,-0.2f);

    for (int i = 0; i < height; i++) {
      mesh->extrudeFace(base,3.7f,wall);
      if (i == height-1) break;
      addDivider(mesh,base,0.15f,0.3f,MATMESH);
    }
    addDivider(mesh,base,0.2f,0.5f,MATMESH,true);
  }
}


void BuildZone::longerSide(Mesh* mesh, int face, float* length, bool* orientation) {
  Vertex vh = mesh->v[mesh->f[face]->vtx->at(0)]-mesh->v[mesh->f[face]->vtx->at(1)];
  Vertex vv = mesh->v[mesh->f[face]->vtx->at(3)]-mesh->v[mesh->f[face]->vtx->at(0)];

  (*orientation) = vh.length() > vv.length();
  if (*orientation) {
    (*length) = vh.length();
  } else {
    (*length) = vv.length();
  }
}

void BuildZone::addDivider(Mesh* mesh, int base, float width, float height, int mat, bool noNext) {
  mesh->extrudeFace(base,0.0f,mat);
  mesh->expandFace(base,width);
  mesh->extrudeFace(base,height,mat);
  if (noNext) return;
  mesh->extrudeFace(base,0.0f,mat);
  mesh->expandFace(base,-width);
}

void BuildZone::subdivideWindows(Mesh* mesh, int wall, int mat) {
  Vertex vv = mesh->v[mesh->f[wall]->vtx->at(0)]-mesh->v[mesh->f[wall]->vtx->at(1)];
  int sdcount = int(vv.length()/4.0f);
  IntVector* fcs = mesh->subdivdeFace(wall,sdcount,true);
  for (int k = 0; k < int(fcs->size()); k++) {
    mesh->extrudeFace(fcs->at(k),0.0f,mat);
    mesh->expandFace(fcs->at(k),-0.4f);
    mesh->extrudeFace(fcs->at(k),-0.2f,mat);
  }
  delete fcs;
}


void BuildZone::output(Output& out) 
{
  MeshVectIter itr; 
  for (itr = meshes.begin(); itr!= meshes.end(); ++itr) (*itr)->output(out);
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
