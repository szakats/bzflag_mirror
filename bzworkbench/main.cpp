#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/Fl_Dialog.h"
#include "include/QuickLabel.h"
#include "include/WorldOptionsDialog.h"
#include "include/MainMenu.h"

#include "include/Model.h"

#include "include/MainWindow.h"

#include "include/BZWParser.h"

#include "include/Transform.h"

#include "include/MasterConfigurationDialog.h"
#include "include/Fl_Error.h"

#include "include/render/Point3D.h"
#include "include/render/RGBA.h"

#include "include/objects/bz2object.h"
#include "include/objects/box.h"
#include "include/objects/material.h"
#include "include/objects/group.h"
#include "include/objects/arc.h"
#include "include/objects/pyramid.h"
#include "include/objects/teleporter.h"
#include "include/objects/base.h"

#include <string>
#include <vector>

using namespace std;



int main(int argc, char** argv) {
	/*
	MainWindow* win = new MainWindow(new Model());
	win->show(argc, argv);
	
	Fl_Error* error = new Fl_Error("You opened an error box!!!");
	error->show();
	
	return Fl::run();
	*/
	
	base bobj = base();
	
	printf("|%s|\n", bobj.toString().c_str());
	
	string bdata = string("base\n") +
						  "  position 1 2 3\n" +
						  "  size 4 5 6\n" +
						  "  rotation 7\n" +
						  "  color 1\n" +
						  "  oncap HM\n" +
						  "end\n";
	
	bobj.update(bdata);
	
	printf("|%s|\n", bobj.toString().c_str());
	
	printf("========================================\n");
	
	
	teleporter tobj = teleporter();
	printf("|%s|\n", tobj.toString().c_str());
	
	string tdata = string("teleporter\n") +
						  "  position 1 2 3\n" +
						  "  size 4 5 6\n" +
						  "  rotation 7\n" +
						  "  name theTeleporter\n" +
						  "  border 3.14\n" +
						  "end\n";
						  
	tobj.update(tdata);
	
	printf("|%s|\n", tobj.toString().c_str());
	
	printf("========================================\n");
	
	pyramid thePyramid = pyramid();
	printf("|%s|\n", thePyramid.toString().c_str());
	string pyramidData = string("pyramid\n") + 
								"  position 1 2 3\n" +
								"  rotation 45\n" +
								"  size 10 20 30\n" +
								"end\n";
	
	thePyramid.update(pyramidData);
	
	printf("|%s|\n", thePyramid.toString().c_str());
	
	printf("========================================\n");
	
	arc aobj = arc();
	
	printf("|%s|\n", aobj.toString().c_str());
	
	string adata = string("arc\n") +
						  "  name sniffedArc\n" +
						  "  divisions 22\n" +
						  "  flatshading\n" +
						  "  angle 120\n" +
						  "  ratio 0.5\n" +
						  "  position 100 200 300\n" +
						  "  rotation 45\n" +
						  "  size 20 30 40\n" +
						  "  scale 4 5 6\n" +
						  "  shear 7 8 9\n" +
						  "  shift 10 11 12\n" +
						  "  spin 56 0 1 0\n" +
						  "  phydrv my_physics_driver\n" +
						  "  smoothbounce\n" +
						  "  matref coolMaterial1\n" +
						  "  matref coolmaterial2\n" +
						  "end\n";
						  
	aobj.update(adata);
	
	printf("|%s|\n", aobj.toString().c_str());
	
	printf("========================================\n");
	
	group gobj = group();
	
	printf("|%s|\n", gobj.toString().c_str());
	
	string gdata = string("group theCoolgroup\n") +
						 "  shift 1 2 3 \n" +
						 "  shear 4 5 6 \n" +
						 "  spin 7 0 1 0 \n" +
						 "  scale 10 20 30 \n" +
						 "  tint 0.5 0.5 0.5 1.0 \n" +
						 "  team 3 \n" +
						 "  matref my1stmaterial\n" +
						 "  phydrv myPhysicsDriver\n" +
						 "  matref myUberCoolMaterial\n" +
						 "  drivethrough\n" +
						 //"  shootthrough\n" +
						 "end\n";
						 
	gobj.update(gdata);
	
	printf("|%s|\n", gobj.toString().c_str());
	
	
	printf("========================================\n");
	
	
	material mobj = material();
	
	printf("|%s|\n", mobj.toString().c_str());
	
	string mobjData = string("material\n") +
							"  name sniffMat\n" +
							"  texture myCoolTex\n" +
							"  addtexture coolTex1\n" +
							"  addtexture coolTex2\n" +
							"  notextures\n" +
							"  notexcolor\n" +
							"  notexalpha\n" +
							"  spheremap\n" +
							"  occluder\n" +
							"  nosort\n" +
							"  alphathresh 0.5\n" +
							"  texmat myTexMat\n" +
							"  dyncol myDynCol\n" +
							"  ambient 0.5 0.5 0.5 0.5\n" +
							"  color 0.1 0.1 0.1 0.1\n" +
							"  specular 0.9 0.9 0.9 0.9\n" +
							"  emission 0.6 0.6 0.6 0.6\n" +
							"  noradar\n" +
							"  nolighting\n" +
							"  noculling\n" +
							"  noshadow\n" +
							"  shininess 64\n" +
							"  matref nextMaterial1\n" +
							"  matref sniff\n" +
							"  resetmat\n" +
							"end\n";
							 
	mobj.update(mobjData);
	
	printf("|%s|\n", mobj.toString().c_str());
	
	printf("========================================\n");
	
	box theBox = box();
	printf("|%s|\n", theBox.toString().c_str());
	string boxData = string("box\n") + 
							"  position 1 2 3\n" +
							"  rotation 45\n" +
							"  size 51 52 53\n" +
							"end\n";
	
	theBox.update(boxData);
	
	printf("|%s|\n", theBox.toString().c_str());
	
	
	printf("========================================\n");
	
	world worldObj = world();
	options optionsObj = options();
	waterLevel waterLevelObj = waterLevel();
	
	string worldName = string("theCoolWorld");
	string sizeString = string("800.0");
	string flagHeightString = string("100.0");
	bool noWalls = true;
	
	string worldStr = string("world\n") +
						 "  name " + worldName + "\n" +
						 "  size " + sizeString + "\n" +
						 "  flagHeight " + flagHeightString + "\n" +
							(noWalls == true ? "  noWalls\n" : "# noWalls\n") +
						 "end\n" +
						 "\n";
	
	string optionsStr = string("options\n") +
							"  set -j -s\n" +
							"  sniff\n" +
							"end\n";
							
	string waterLevelStr = string("waterLevel\n") +
								  "  height 1.0\n" +
								  // "  material sniff\n" +
								  "  name yoink\n" +
								  "end\n";
								  
	
	worldObj.update(worldStr);
	
	// string header = string("world");
	// string key = string("name");
	// BZWParser::getValuesByKey(key, header, worldStr);
	
	printf("%s\n", worldObj.toString().c_str());
	
	optionsObj.update(optionsStr);
	
	printf("%s\n", optionsObj.toString().c_str());
	
	waterLevelObj.update(waterLevelStr);
	
	printf("%s\n", waterLevelObj.toString().c_str());
	return 0;
	
	
}
