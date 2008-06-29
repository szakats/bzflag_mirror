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

#include "GridGenerator.h"
#include "Zone.h"
#include "FloorZone.h"
#include "Random.h"



void GridGenerator::parseOptions(CCommandLineArgs* opt) { 
  Generator::parseOptions(opt); 
  int worldSize  = getSize();
  int gridSize   = 42;

  if (opt->Exists("g"))        { gridSize = opt->GetDataI("g"); }
  if (opt->Exists("gridsize")) { gridSize = opt->GetDataI("gridsize"); }

  snap = 3;

  if (opt->Exists("p"))        { snap = opt->GetDataI("p"); }
  if (opt->Exists("gridsnap")) { snap = opt->GetDataI("gridsnap"); }

  subdiv = 120;

  if (opt->Exists("v"))      { subdiv = opt->GetDataI("v"); }
  if (opt->Exists("subdiv")) { subdiv = opt->GetDataI("subdiv"); }

  fullslice = 8;

  if (opt->Exists("f"))         { subdiv = opt->GetDataI("f"); }
  if (opt->Exists("fullslice")) { subdiv = opt->GetDataI("fullslice"); }

  if (fullslice > subdiv) subdiv = fullslice;

  map.initialize(this,worldSize,gridSize);
}

#define SETROAD(cx,cy)  { if (map.getNode(cx,cy).type > GridMap::NONE) { map.setType(cx,cy,GridMap::ROADX);         } else { map.setType(cx,cy,GridMap::ROAD); } }
#define SETROADF(cx,cy) { if (map.getNode(cx,cy).type > GridMap::NONE) { map.setType(cx,cy,GridMap::ROADX); break;  } else { map.setType(cx,cy,GridMap::ROAD); } }

void GridGenerator::plotRoad(int x, int y, bool horiz, bool collision) {
  if (!collision) {
    if (horiz) {
      for (int xx = 0; xx < map.getGridSize(); xx++) SETROAD(xx,y)
    } else {
      for (int yy = 0; yy < map.getGridSize(); yy++) SETROAD(x,yy)
    } 
    return;
  }
  if (horiz) {
    for (int xx = x;   xx < map.getGridSize(); xx++) SETROADF(xx,y)
    for (int xx = x-1; xx >= 0; xx--)           SETROADF(xx,y)
  } else {
    for (int yy = y;   yy < map.getGridSize(); yy++) SETROADF(x,yy)
    for (int yy = y-1; yy >= 0; yy--)           SETROADF(x,yy)
  } 
  
}

void GridGenerator::performSlice(bool full, int snapmod, bool horiz) {
  int bmod = bases > 0 ? 2 : 1;
  int x = Random::numberRangeStep(snap,map.getGridSize()-snap*bmod,snapmod*snap);
  int y = Random::numberRangeStep(snap,map.getGridSize()-snap*bmod,snapmod*snap);

  if (debugLevel > 2) printf("slice (%d,%d)...\n",x,y);

  if (map.getNode(x,y).type == CELLROADX) return;

  if (map.getNode(horiz ? Coord2D(x+1,y) : Coord2D(x,y+1)).type > 0) return;

  plotRoad(x,y,horiz,!full);
}

void GridGenerator::run() { 
  if (debugLevel > 1) printf("\nRunning GridGenerator...\n");

  map.clear();

  if (bases > 0) {
    plotRoad(snap,snap,true,0);
    plotRoad(snap,snap,false,0);
    plotRoad(map.getGridSize()-snap-1,map.getGridSize()-snap-1,true,0);
    plotRoad(map.getGridSize()-snap-1,map.getGridSize()-snap-1,false,0);

    map.setAreaType(Coord2D(0,0),Coord2D(snap,snap),GridMap::BASE);
    map.setAreaType(Coord2D(map.getGridSize()-snap,map.getGridSize()-snap),Coord2D(map.getGridSize(),map.getGridSize()),GridMap::BASE);
    if (bases > 2) {
      map.setAreaType(Coord2D(0,map.getGridSize()-snap),Coord2D(snap,map.getGridSize()),GridMap::BASE);
      map.setAreaType(Coord2D(map.getGridSize()-snap,0),Coord2D(map.getGridSize(),snap),GridMap::BASE);
    }
  }

  bool horiz = Random::coin();

  if (debugLevel > 1) printf("Full slices (%d)...\n",fullslice);
  for (int i = 0; i < fullslice; i++) {
    horiz = !horiz;
    performSlice(true,3,horiz);
  }

  if (debugLevel > 1) printf("Subdivision (%d)...\n",subdiv);
  for (int i = fullslice; i < subdiv; i++) {
    horiz = !horiz;
    performSlice(false,1,horiz);
  }

  if (debugLevel > 1) printf("Pushing zones...\n");
  map.pushZones();
  
  Generator::run(); 
}



  

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
