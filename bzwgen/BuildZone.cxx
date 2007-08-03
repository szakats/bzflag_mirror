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

BuildZone::BuildZone(Coord2D a, Coord2D b, int astep) : Zone(a,b,astep)
{
  int wall;
  if (rand()%2 == 0) {
    wall = MATWALL;
  } else {
    wall = MATWALL2;
  }

  mesh.matref = MATMESH;


  int base = mesh.createNewFace(
      Vertex((float)A.x,(float)A.y,0.01f),
      Vertex((float)B.x,(float)A.y,0.01f),
      Vertex((float)B.x,(float)B.y,0.01f),
      Vertex((float)A.x,(float)B.y,0.01f),
      MATMESH
  );

  int height = rand()%5+1;

  mesh.io = true;
  mesh.inside  = Vertex((float)B.x-A.x,(float)B.y-A.y,1.0f);
  mesh.outside = Vertex((float)B.x-A.x,(float)B.y-A.y,float(height)*10.0f);

  ID4 fs;

  for (int i = 0; i < height; i++) {
    fs = mesh.extrudeFace(base,3.7f,wall);
    for (int j = 0; j < 4; j++) {
      Vertex vv = mesh.v[mesh.f[fs[j]].vtx[0]]-mesh.v[mesh.f[fs[j]].vtx[1]];
      int sdcount = int(vv.length()/4.0f);
      IntVector* fcs = mesh.subdivdeFace(fs[j],sdcount,true);
      if (wall == MATWALL2) {
	for (int k = 0; k < int(fcs->size()); k++) {
	  mesh.extrudeFace(fcs->at(k),0.0f,wall);
	  mesh.expandFace(fcs->at(k),-0.4f);
	  mesh.extrudeFace(fcs->at(k),-0.2f,wall);
	}
      }
      delete fcs;
    }
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

void BuildZone::output(Output& out) 
{
  mesh.output(out);
/* out.inside((A.x+B.x)/2,(A.y+B.y)/2,1);
  out.outside((A.x+B.x)/2,(A.y+B.y)/2,100);
  out.outside((A.x)-1,(A.y)-1);
  out.outside((B.x)+1,(B.y)+1);
  out << "  color 0."<< rand()%10+80 << " 0." << rand()%20+80 << " 0."<< rand()%20+80<< " 1.0\n";
*/
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
