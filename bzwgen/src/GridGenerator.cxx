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

#include "GridGenerator.h"
#include "Zone.h"
#include "Random.h"
#include "FloorZone.h"
#include "BaseZone.h"
#include "BuildZone.h"

int BaseZone::colorCount = 1;

void GridGenerator::parseOptions( CCommandLineArgs* opt ) {
  Generator::parseOptions( opt );
  int worldSize  = getSize();
  gridSize   = 42;

  if ( opt->Exists( "g" ) )        { gridSize = opt->GetDataI( "g" ); }
  if ( opt->Exists( "gridsize" ) ) { gridSize = opt->GetDataI( "gridsize" ); }

  snap = 3;

  if ( opt->Exists( "p" ) )        { snap = opt->GetDataI("p"); }
  if ( opt->Exists( "gridsnap" ) ) { snap = opt->GetDataI("gridsnap"); }

  subdiv = 120;

  if ( opt->Exists( "v" ) )      { subdiv = opt->GetDataI("v"); }
  if ( opt->Exists( "subdiv" ) ) { subdiv = opt->GetDataI("subdiv"); }

  fullslice = 8;

  if ( opt->Exists( "f" ) )         { subdiv = opt->GetDataI( "f" ); }
  if ( opt->Exists( "fullslice" ) ) { subdiv = opt->GetDataI( "fullslice" ); }

  if ( fullslice > subdiv ) subdiv = fullslice;

  gridStep = worldSize / gridSize;

  map = new DiscreetMapNode[ ( gridSize + 1 ) * ( gridSize + 1 ) ];
  for ( int i = 0; i < ( gridSize + 1 ) * ( gridSize + 1 ); i++ ) {
    map[i].zone = -1;
    map[i].type = NONE;
  }
}

#define SETROAD(cx,cy)  { if (node(cx,cy).type > NONE) { node(cx,cy).type = ROADX;         } else { node(cx,cy).type = ROAD; } }
#define SETROADF(cx,cy) { if (node(cx,cy).type > NONE) { node(cx,cy).type = ROADX; break;  } else { node(cx,cy).type = ROAD; } }

void GridGenerator::plotRoad(int x, int y, bool horiz, bool collision) {
  if (!collision) {
    if (horiz) {
      for (int xx = 0; xx < gridSize; xx++) SETROAD(xx,y)
    } else {
      for (int yy = 0; yy < gridSize; yy++) SETROAD(x,yy)
    }
    return;
  }
  if (horiz) {
    for (int xx = x;   xx < gridSize; xx++) SETROADF(xx,y)
    for (int xx = x-1; xx >= 0; xx--)       SETROADF(xx,y)
  } else {
    for (int yy = y;   yy < gridSize; yy++) SETROADF(x,yy)
    for (int yy = y-1; yy >= 0; yy--)       SETROADF(x,yy)
  }

}

void GridGenerator::performSlice(bool full, int snapmod, bool horiz) {
  int bmod = bases > 0 ? 2 : 1;
  int x = Random::numberRangeStep(snap,gridSize-snap*bmod,snapmod*snap);
  int y = Random::numberRangeStep(snap,gridSize-snap*bmod,snapmod*snap);

  Logger.log( 3, "GridGenerator : slice (%d,%d)...", x, y );

  if (node(x,y).type == ROADX) return;

  if (horiz) {
    if (node(x+1,y).type > 0) return;
  } else {
    if (node(x,y+1).type > 0) return;
  }

  plotRoad(x,y,horiz,!full);
}

void GridGenerator::growZone(int x,int y,CellType type) {
  Logger.log( 3, "GridGenerator : pushing zone at (%d,%d)", x, y );
  int xe = x;
  int ye = y;
  while(xe < gridSize) {
    xe++;
    int etype = node(xe,y).type;
    if (etype != type) break;
  }
  while(ye < gridSize) {
    ye++;
    int etype = node(x,ye).type;
    if (etype != type) break;
  }

  int zoneID = getZoneCount();
  for (int xx = x; xx < xe; xx++)
    for (int yy = y; yy < ye; yy++)
      node(xx,yy).zone = zoneID;

  graph::Face* face = createFakeFace(x,y,xe,ye);
  if (face) face->size();

  if (type == ROAD) {
    Logger.log( 3, "GridGenerator : road zone added (%d,%d * %d,%d)", x, y, xe, ye );
    addZone(new FloorZone(this,face,gridStep,roadid, x-xe < y-ye));
  } else if (type == ROADX) {
    Logger.log( 3, "GridGenerator : crossroads zone added (%d,%d * %d,%d)", x, y, xe, ye );
    addZone(new FloorZone(this,face,gridStep,roadxid,true));
  } else if (type == BASE) {
    Logger.log( 3, "GridGenerator : base zone added (%d,%d * %d,%d)", x, y, xe, ye );
    addZone(new BaseZone(this,face, ctfSafe));
  } else {
    Logger.log( 3, "GridGenerator : building zone added (%d,%d * %d,%d)", x, y, xe, ye );
    addZone(new BuildZone(this,face));
  }
  Logger.log( 4, "GridGenerator : zone successfuly created (%d,%d * %d,%d)", x, y, xe, ye );
}


void GridGenerator::run() {
  Logger.log( 2, "GridGenerator : running...");

  if (bases > 0) {
    plotRoad(snap,snap,true,0);
    plotRoad(snap,snap,false,0);
    plotRoad(gridSize-snap-1,gridSize-snap-1,true,0);
    plotRoad(gridSize-snap-1,gridSize-snap-1,false,0);

    setAreaType(0,0,snap,snap,BASE);
    setAreaType(gridSize-snap,gridSize-snap,gridSize,gridSize,BASE);
    if (bases > 2) {
      setAreaType(0,gridSize-snap,snap,gridSize,BASE);
      setAreaType(gridSize-snap,0,gridSize,snap,BASE);
    }
  }

  bool horiz = Random::coin();

  Logger.log( 2, "GridGenerator : full slices (%d)...", fullslice );
  for (int i = 0; i < fullslice; i++) {
    horiz = !horiz;
    performSlice(true,3,horiz);
  }

  Logger.log( 2, "GridGenerator : subdivision (%d)...", subdiv );
  for (int i = fullslice; i < subdiv; i++) {
    horiz = !horiz;
    performSlice(false,1,horiz);
  }

  Logger.log( 2, "GridGenerator : pushing zones..." );

  int y = 0;
  int x = 0;
  do {
    x = 0;
    do {
      if (node(x,y).zone == -1) {
        growZone(x,y,node(x,y).type);
      }
      x++;
    } while (x < gridSize);
    y++;
  } while (y < gridSize);

  Generator::run();
  Logger.log( 2, "GridGenerator : run completed.");
}

graph::Face* GridGenerator::createFakeFace(int ax, int ay, int bx, int by) {
  graph::Node* n1 = graph.addNode( new graph::Node( &graph, worldCoord(ax), worldCoord(ay) ) );
  graph::Node* n2 = graph.addNode( new graph::Node( &graph, worldCoord(bx), worldCoord(ay) ) );
  graph::Node* n3 = graph.addNode( new graph::Node( &graph, worldCoord(bx), worldCoord(by) ) );
  graph::Node* n4 = graph.addNode( new graph::Node( &graph, worldCoord(ax), worldCoord(by) ) );
  graph.addConnection(n1,n2);
  graph.addConnection(n2,n3);
  graph.addConnection(n3,n4);
  graph.addConnection(n4,n1);
  graph::Face* face = new graph::Face( &graph );
  face->addEdge( n1->getFirstOutgoingEdge() );
  face->addEdge( n2->getFirstOutgoingEdge() );
  face->addEdge( n3->getFirstOutgoingEdge() );
  face->addEdge( n4->getFirstOutgoingEdge() );
  return face;
}





// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
