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
  gi.sizeX = gi.sizeY = 40;

  if (opt->Exists("g"))         { gi.sizeY = gi.sizeX = opt->GetDataI("g"); }
  if (opt->Exists("-gridsize")) { gi.sizeY = gi.sizeX = opt->GetDataI("-gridsize"); }

  snapX = 3;
  snapY = 3;

  if (opt->Exists("p"))         { snapX = snapY = opt->GetDataI("p"); }
  if (opt->Exists("-gridsnap")) { snapX = snapY = opt->GetDataI("-gridsnap"); }

  subdiv = 30;

  if (opt->Exists("v"))       { subdiv = opt->GetDataI("v"); }
  if (opt->Exists("-subdiv")) { subdiv = opt->GetDataI("-subdiv"); }

  gi.stepX = gi.size / gi.sizeX;
  gi.stepY = gi.size / gi.sizeY;
  map.initialize(this);
}

void GridGenerator::run() { 
  int x,y;
  Generator::run(); 
  map.clear();
  for (int i = 0; i < subdiv; i++) {
    // TODO: replace this with randomIntRange or the like
    x = randomInt(int(gi.sizeX / snapX)-1)*snapX+snapX;
    y = randomInt(int(gi.sizeY / snapY)-1)*snapY+snapY;

    for (int ax = 0; ax < gi.sizeX; ax++) {
      map.settype(ax,y,CELLROAD);
    }
    for (int ay = 0; ay < gi.sizeY; ay++) {
      if ((map.getNode(x,ay).type == CELLROAD) && randomChance(33)) break;
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
  mats.push_back(new Material(MATGRASS,"std_ground"));
}

void GridGenerator::output(Output& out) { 
  Generator::output(out); 

  for (MaterialVectIter iter = mats.begin(); iter != mats.end(); ++iter) 
    (*iter)->output(out);

  for (ZoneVectIter iter = map.zones.begin(); iter != map.zones.end(); ++iter) 
    (*iter)->output(out);
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
