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

#include <string>
#include <vector>

using namespace std;



int main(int argc, char** argv) {
	
	MainWindow* win = new MainWindow(new Model());
	win->show(argc, argv);
	
	Fl_Error* error = new Fl_Error("You opened an error box!!!");
	error->show();
	
	return Fl::run();
	
	/*
	group obj = group();
	
	printf("|%s|\n", obj.toString().c_str());
	
	string data = string("group theCoolgroup\n") +
						 "  shift 1 2 3 \n" +
						 "  shear 4 5 6 \n" +
						 "  spin 7 0 1 0 \n" +
						 "  scale 10 20 30 \n" +
						 "  tint 0.5 0.5 0.5 1.0 \n" +
						 "  team 3 \n" +
						 "  phydrv myPhysicsDriver\n" +
						 "  matref myUberCoolMaterial\n" +
						 "  drivethrough\n" +
						 "  shootthrough\n" +
						 "end\n";
						 
	obj.update(data);
	
	printf("|%s|\n", obj.toString().c_str());
	
	*/
	
	/*
	material obj = material();
	
	printf("|%s|\n", obj.toString().c_str());
	
	string objData = string("material\n") +
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
							"end\n";
							 
	obj.update(objData);
	
	printf("|%s|\n", obj.toString().c_str());
	
	*/
	/*
	box theBox = box();
	printf("|%s|\n", theBox.toString().c_str());
	string boxData = string("box\n") + 
							"  position 1 2 3\n" +
							"  rotation 45\n" +
							"  size 51 52 53\n" +
							"  spin 45 1 0 0\n" +
							"  shear 45 55 65\n" +
							"  scale 10 11 12\n" +
							"  shift 100 101 102\n" +
							"end\n";
	
	theBox.update(boxData);
	
	printf("|%s|\n", theBox.toString().c_str());
	*/
	
	/*
	world worldObj = world();
	options optionsObj = options();
	waterLevel waterLevelObj = waterLevel();
	
	string worldName = string("theCoolWorld");
	string sizeString = string("800.0");
	string flagHeightString = string("100.0");
	bool noWalls = true;
	
	string worldStr = string("world\n") +
						 "  name " + worldName + "\n" +
						 "  name sniff\n" +
						 "  name yoink\n" +
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
	
	*/
}
