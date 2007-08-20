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
#include "FloorZone.h"


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

#define SETROAD(cx,cy)  { if (map.getNode(cx,cy).type == CELLROAD) { map.settype(cx,cy,CELLROADX);         } else { map.settype(cx,cy,CELLROAD); } }
#define SETROADF(cx,cy) { if (map.getNode(cx,cy).type == CELLROAD) { map.settype(cx,cy,CELLROADX); return; } else { map.settype(cx,cy,CELLROAD); } }

void GridGenerator::plotRoad(int x, int y, bool horiz, int  collision) {
  if (collision == 0) {
    if (horiz) {
      for (int xx = 0; xx < gi.sizeX; xx++) SETROAD(xx,y)
    } else {
      for (int yy = 0; yy < gi.sizeX; yy++) SETROAD(x,yy)
    } 
    return;
  }
  if (horiz) {
    for (int xx = x;   xx < gi.sizeX; xx++) SETROADF(xx,y)
    for (int xx = x-1; xx >= 0; xx--)       SETROADF(xx,y)
  } else {
    for (int yy = y;   yy < gi.sizeY; yy++) SETROADF(x,yy)
    for (int yy = y-1; yy >= 0; yy--)       SETROADF(x,yy)
  } 
  
}


void GridGenerator::run() { 
  int x,y;
  Generator::run(); 
  map.clear();
  if (bases > 0) {
    plotRoad(snapX,snapY,true,0);
    plotRoad(snapX,snapY,false,0);
    plotRoad(gi.sizeX-snapX-1,gi.sizeY-snapY-1,true,0);
    plotRoad(gi.sizeX-snapX-1,gi.sizeY-snapY-1,false,0);
    for(x = 0; x < snapX; x++)                 for (y = 0; y < snapY; y++) map.settype(x,y,CELLBASE);
    for(x = gi.sizeX-snapX; x < gi.sizeX; x++) for (y = gi.sizeY-snapY; y < gi.sizeY; y++) map.settype(x,y,CELLBASE);
    if (bases > 2) {
      for(x = 0; x < snapX; x++)                 for (y = gi.sizeY-snapY; y < gi.sizeY; y++) map.settype(x,y,CELLBASE);
      for(x = gi.sizeX-snapX; x < gi.sizeX; x++) for (y = 0; y < snapY; y++) map.settype(x,y,CELLBASE);
    }
  }

  for (int i = 0; i < subdiv; i++) {
    // TODO: replace this with randomIntRange or the like
    if (bases > 0) {
      x = randomInt(int(gi.sizeX / snapX)-2)*snapX+snapX;
      y = randomInt(int(gi.sizeY / snapY)-2)*snapY+snapY;
    } else {
      x = randomInt(int(gi.sizeX / snapX)-1)*snapX+snapX;
      y = randomInt(int(gi.sizeY / snapY)-1)*snapY+snapY;
    }

    if (map.getNode(x,y).type > 0) continue;

    for (int ax = 0; ax < gi.sizeX; ax++) {
      if (map.getNode(ax,y).type == CELLROAD) {
        map.settype(ax,y,CELLROADX);
      } else {
        map.settype(ax,y,CELLROAD);
      }
    }
    for (int ay = 0; ay < gi.sizeY; ay++) {
      if (map.getNode(x,ay).type == CELLROAD) {
        map.settype(x,ay,CELLROADX);
        if (randomChance(33)) break;
      } else {
        map.settype(x,ay,CELLROAD);
      }
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
