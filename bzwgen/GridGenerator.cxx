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

#include "GridGenerator.h"
#include "Zone.h"


GridGenerator::GridGenerator(RuleSet* _ruleset) : Generator(_ruleset) { 
}

void GridGenerator::parseOptions(Options opt) { 
  Generator::parseOptions(opt); 
  gi.size  = getSize();
  gi.sizeX = 40;
  gi.sizeY = 40;
  gi.stepX = gi.size / gi.sizeX;
  gi.stepY = gi.size / gi.sizeY;
  map.initialize(this);
}

void GridGenerator::run() { 
  int x,y;
  Generator::run(); 
  for (int i = 0; i < 30; i++) {
    // TODO: replace this with randomIntRange or the like
    x = randomInt(int(gi.sizeX / 4)-1)*4+4;
    y = randomInt(int(gi.sizeY / 4)-1)*4+4;

    for (int ax = 0; ax < gi.sizeX; ax++) {
      map.setZ(ax,y,0);
      map.settype(ax,y,CELLROAD);
    }
    for (int ay = 0; ay < gi.sizeY; ay++) {
      if ((map.getNode(x,ay).type == CELLROAD) && randomChance(33)) break;
      map.setZ(x,ay,0);
      map.settype(x,ay,CELLROAD);
    }
  }
  map.pushZones();

  mats.push_back(new Material(MATROAD,"road",true));
  mats.push_back(new Material(MATROADX,"roadx",true));
  mats.push_back(new Material(MATWALL,"bwall"));
  mats.push_back(new Material(MATWALL2,"bwall2"));
  mats.push_back(new Material(MATMESH,"mesh"));
  mats.push_back(new Material(MATROOF,"roof"));
  mats.push_back(new Material(MATROOFT,"rooft"));
  mats.push_back(new Material(MATGLASS,"glass"));
}

void GridGenerator::output(Output& out) { 
  Generator::output(out); 

  for (MaterialVectIter iter = mats.begin(); iter != mats.end(); ++iter) 
    (*iter)->output(out);

  for (ZoneVectIter iter = map.zones.begin(); iter != map.zones.end(); ++iter) 
    (*iter)->output(out);
  /*
  for (int y = 0; y < wi.sizeY; y++) {
    for (int x = 0; x < wi.sizeX; x++) {
      if (map.getNode(x,y).type == CELLROAD) {
	printf("x");
      } else {
      printf("#");
    }
  }
  printf("\n");
  */

}

GridGenerator::~GridGenerator() { 
}


  

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
