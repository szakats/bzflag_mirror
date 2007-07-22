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
}

void BuildZone::output(Output& out) 
{
  if (FULLMESH) {
    int height = ((rand()%5)*4+4);
    if (rand()%4 == 0) height += (rand()%5)*4;
    out.line("mesh");
    if (rand()%2 == 0) {
      out.matref("bwall");
    } else {
      out.matref("bwall2");
    }
    out.inside((A.x+B.x)/2,(A.y+B.y)/2,1);
    out.outside((A.x+B.x)/2,(A.y+B.y)/2,100);
    out.outside((A.x)-1,(A.y)-1);
    out.outside((B.x)+1,(B.y)+1);
    out.vertex(A.x,A.y);
    out.vertex(B.x,A.y);
    out.vertex(B.x,B.y);
    out.vertex(A.x,B.y);
    out.vertex(A.x,A.y,height);
    out.vertex(B.x,A.y,height);
    out.vertex(B.x,B.y,height);
    out.vertex(A.x,B.y,height);
    out << "  color 0."<< rand()%10+80 << " 0." << rand()%20+80 << " 0."<< rand()%20+80<< " 1.0\n";
    out.texcoord(0,0);
    out.texcoord(int((B.x-A.x)/step),0);
    out.texcoord(int((B.x-A.x)/step),int((B.y-A.y)/step));
    out.texcoord(0,int((B.x-A.x)/step));
    out.face(0,1,5,4);
    out.face(1,2,6,5);
    out.face(2,3,7,6);
    out.face(3,0,4,7);
    out.matref("top");
    out.face(4,5,6,7);
    out.line("end\n");
  } else {
    out << "meshbox\n";
    out << "  position " << int((A.x+B.x)/2) << " " << int((A.y+B.y)/2) << " 0\n";
    out << "  size " << int((B.x-A.x)/2) << " " << int((B.y-A.y)/2) << " " << ((rand()%5)*4+5) << "\n";
    out << "  size 10 10 " << (rand()%9+3) << "\n";
    out << "  rotation 0\n";
    out << "end\n\n";
  }
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
