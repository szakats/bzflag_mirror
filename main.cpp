#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/dialogs/Fl_Dialog.h"
#include "include/widgets/QuickLabel.h"
#include "include/dialogs/WorldOptionsDialog.h"
#include "include/dialogs/MainMenu.h"

#include "include/model/Model.h"

#include "include/windows/MainWindow.h"
#include "include/windows/ConsoleWindow.h"

#include "include/model/BZWParser.h"

#include "include/Transform.h"

#include "include/dialogs/MasterConfigurationDialog.h"
#include "include/dialogs/Fl_Error.h"

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
#include "include/objects/meshbox.h"
#include "include/objects/meshpyr.h"
#include "include/objects/cone.h"
#include "include/objects/sphere.h"
#include "include/objects/tetra.h"
#include "include/objects/link.h"
#include "include/objects/zone.h"
#include "include/objects/weapon.h"
#include "include/objects/world.h"
#include "include/objects/options.h"
#include "include/objects/waterLevel.h"
#include "include/objects/texturematrix.h"
#include "include/objects/dynamicColor.h"
#include "include/objects/mesh.h"
#include "include/objects/define.h"

#include "include/TextUtils.h"

#include <string>
#include <vector>

// register the built-in objects
void buildDatabase() {
	Model::registerObject("arc", NULL, "end", arc::init);
	Model::registerObject("base", NULL, "end", base::init);
	Model::registerObject("box", NULL, "end", box::init);
	Model::registerObject("cone", NULL, "end", cone::init);
	Model::registerObject("define", "define:<arc><base><box><cone><group><mesh><meshbox><meshpyr><pyramid><sphere><teleporter><tetra>", "enddef", define::init);
	Model::registerObject("dynamicColor", NULL, "end", dynamicColor::init);
	Model::registerObject("group", NULL, "end", group::init);
	Model::registerObject("link", NULL, "end", Tlink::init);
	Model::registerObject("material", NULL, "end", material::init);
	Model::registerObject("mesh", "mesh:<face><drawinfo>", "end", mesh::init);
	Model::registerObject("meshbox", NULL, "end", meshbox::init);
	Model::registerObject("meshpyr", NULL, "end", meshpyr::init);
	Model::registerObject("options", NULL, "end", options::init);
	Model::registerObject("physics", NULL, "end", physics::init);
	Model::registerObject("pyramid", NULL, "end", pyramid::init);
	Model::registerObject("sphere", NULL, "end", sphere::init);
	Model::registerObject("teleporter", NULL, "end", teleporter::init);
	Model::registerObject("tetra", NULL, "end", tetra::init);
	Model::registerObject("texturematrix", NULL, "end", texturematrix::init);
	Model::registerObject("waterLevel", NULL, "end", waterLevel::init);
	Model::registerObject("weapon", NULL, "end", weapon::init);
	Model::registerObject("world", NULL, "end", world::init);
	Model::registerObject("zone", NULL, "end", zone::init);
}


int main(int argc, char** argv) {
	/*
	MainWindow* win = new MainWindow(new Model());
	win->show(argc, argv);
	
	Fl_Error* error = new Fl_Error("You opened an error box!!!");
	error->show();
	
	return Fl::run();
	*/
	
	/*
	Model* model = new Model();
	
	buildDatabase();
	
	MainWindow* mw = new MainWindow(model);
	// ConsoleWindow* cw = new ConsoleWindow(10);
	
	// cw->show();
	mw->show();
	
	return Fl::run();
	*/
	/*
	vector<string> objects = BZWParser::loadFile("share/H2O-R1.bzw");
	
	Model::build( objects );
	
	printf("\n==============================================================\n");
	printf("\n     Model.toString() output\n");
	printf("\n==============================================================\n");
	printf("%s\n", Model::toString().c_str());
	
	return 0;
	*/
	
	
	vector<string> objects = BZWParser::loadFile("share/H2O-R1.bzw");
	for(vector<string>::iterator i = objects.begin(); i != objects.end(); i++) {
		printf("==============================================\n");
		printf("%s", i->c_str());	
	}
	/*
	mesh meshobj = mesh();
	
	printf("|%s|\n", meshobj.toString().c_str());
	
	string meshData = string("mesh\n") +
							 "  inside 0 1 2\n" +
							 "  inside 2 3 4\n" +
							 "  inside 1 2 3\n" +
							 
							 "  outside 0 0 1\n" +
							 "  outside 6 6 6\n" +
							 
							 "  vertex 50 50 50\n" +
							 "  vertex 100 100 100\n" +
							 "  vertex 60 60 60\n" +
							 
							 "  texcoord 0.5 0.1\n" +
							 "  texcoord 1 0\n" +
							 "  texcoord 6 7\n" +
							 
							 "  normal 1 2 3\n" +
							 "  normal 6 7 8\n" +
							 "  normal 7 8 9\n" +
							 "  normal 10 10 10\n" +
							 
							 "  noclusters\n" +
							 "  smoothbounce\n" +
							 
							 "  phydrv sniffphydrv\n" +
							 
							 "  face\n" +
							 "    vertices 10 1 2\n" +
							 "    texcoords 10 1 2\n" +
							 "    normals 10 1 2\n" +
							 "    phydrv myPhysics\n" +
							 "    passable\n" +
							 "  endface\n" +
							 
							 "  matref sniff\n" +
							 
							 "  face\n" +
							 "    vertices 0 1 2\n" +
							 "    texcoords 0 1 2\n" +
							 "    normals 0 1 2\n" +
							 "    phydrv myPhysics\n" +
							 "    passable\n" +
							 "  endface\n" +
							 
							 "  matref yoink\n" +
							 
							 "  face\n" +
							 "    vertices 4 1 2\n" +
							 "    texcoords 4 1 2\n" +
							 "    normals 4 1 2\n" +
							 "    phydrv myPhysics\n" +
							 "  endface\n" +
							 
							 "  face\n" +
							 "    vertices 5 1 2\n" +
							 "    texcoords 5 1 2\n" +
							 "    normals 5 1 2\n" +
							 "    phydrv myPhysics\n" +
							 "    passable\n" +
							 "  endface\n" +
							 
							"  drawinfo\n" +
							"    dlist\n" +
							"    angvel 0.5\n" +
							"    vertex 0 0 0\n" +
							"    vertex 1 1 1\n" +
							"    vertex 2 2 2\n" +
							"    texcoord 4 5\n" +
							"    texcoord 6 7\n" +
							"    texcoord 8 9\n" +
							"    normal 1 2 3\n" +
							"    normal 4 5 6\n" +
							"    normal 7 8 9\n" + 
							"    corner 0 0 0\n" +
							"    corner 1 1 1\n" +
							"    corner 2 2 2\n" +
							"    lod\n" +
					        "      lengthPerPixel 0\n" +
					        "      matref -1 \n" +
					        "        dlist\n" +
					        "        sphere 0 0 0.5 2.25\n" +
					        "        quads 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15\n" +
					        "      end\n" +
					      	"    end\n" +
					      	"  end\n" +
					      	"end\n";
					      	
	meshobj.update(meshData);
	
	printf("|%s|\n", meshobj.toString().c_str());
	
	printf("========================================\n");
	
	MeshFace face = MeshFace();
	
	printf("|%s|\n", face.toString().c_str());
	
	string faceData = string("face\n") +
							 "  vertices 1 2 3 4\n" +
							 "  texcoords 5 6 7 8\n" +
							 "  normals 8 9 10 11\n" +
							 "  phydrv sniff\n" +
							 "endface\n";
							 
	face.update(faceData);
	
	printf("|%s|\n", face.toString().c_str());
	
	printf("========================================\n");
	
	
	DrawInfo lodobj = DrawInfo();
	
	printf("|%s|\n", lodobj.toString().c_str());
	
	string lodData = string("drawinfo\n") +
							"  dlist\n" +
							"  angvel 0.5\n" +
							"  vertex 0 0 0\n" +
							"  vertex 1 1 1\n" +
							"  vertex 2 2 2\n" +
							"  texcoord 4 5\n" +
							"  texcoord 6 7\n" +
							"  texcoord 8 9\n" +
							"  normal 1 2 3\n" +
							"  normal 4 5 6\n" +
							"  normal 7 8 9\n" + 
							"  corner 0 0 0\n" +
							"  corner 1 1 1\n" +
							"  corner 2 2 2\n" +
							"  lod\n" +
					        "    lengthPerPixel 0\n" +
					        "    matref -1 \n" +
					        "      dlist\n" +
					        "      sphere 0 0 0.5 2.25\n" +
					        "      quads 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15\n" +
					        "    end\n" +
					      	"  end\n" +
					      	"end\n";
					      
	lodobj.update(lodData);
	
	printf("|%s|\n", lodobj.toString().c_str());
	
	printf("========================================\n");
	
	
	dynamicColor dcobj = dynamicColor();
	
	printf("|%s|\n", dcobj.toString().c_str());
	
	string dcdata = string("dynamicColor\n") +
						  "  name sniff\n" +
						  "  red sinusoid 0.1 0.0 0.5\n" +
						  "  red clampUp 0.1 0.0 0.71\n" +
						  "  green clampDown 0.0 0.0 1.0\n" +
						  "  blue sequence 0.0 0.0 2 0 1 0 2 1 1 0\n" +
						  "  red limits 0.0 1.0\n" +
						  "end\n";
						  
	dcobj.update(dcdata);
	
	printf("|%s|\n", dcobj.toString().c_str());
	
	printf("========================================\n");
	
	
	texturematrix tmobj = texturematrix();
	
	printf("|%s|\n", tmobj.toString().c_str());
	
	string tmdata = string("texturematrix\n") +
						   "  name foobar\n" +
						   "  scale 1.0 2.0 21.0 22.0\n" +
						   "  spin 3.0\n" +
						   "  shift 4.0 5.0\n" +
						   "  center 6.0 7.0\n" +
						   "  fixedscale 8.0 9.0\n" +
						   "  fixedspin 10.0\n" +
						   "  fixedshift 11.0 12.0\n" +
						   "end\n";
						   
	tmobj.update(tmdata);
	
	printf("|%s|\n", tmobj.toString().c_str());
	
	printf("========================================\n");
	
	
	weapon wobj = weapon();
	
	printf("|%s|\n", wobj.toString().c_str());
	
	string wdata = string("weapon\n") +
						  "  position 1 2 3\n" +
						  "  rotation 45\n" +
						  "  name sniff\n" +
						  "  color 1\n" +
						  "  tilt 90\n" +
						  "  initdelay 10\n" +
						  "  delay 1 2 3 5\n" +
						  "  type SW\n" +
						  "  trigger flagcap\n" +
						  "  eventteam G\n" +
						  "end\n";
						  
	wobj.update(wdata);
	
	printf("|%s|\n", wobj.toString().c_str());
	
	printf("========================================\n");
	 
	
	zone zobj = zone();
	
	printf("|%s|\n", zobj.toString().c_str());
	
	string zdata = string("zone\n") +
						  "  name poot\n" +
						  "  position 1 2 3\n" +
						 // "  rotation 45\n" +
						  "  size 4 5 6\n" +
						 // "  safety 1 2 3\n" +
						 // "  team 1 2 3\n" +
						  "  flag GM MG good\n" +
						  "  zoneflag GM 3\n" +
						  "  zoneflag V 5\n" +
						  "end\n";
						  
	zobj.update(zdata);
	printf("|%s|\n", zobj.toString().c_str());

	printf("========================================\n");
	
	
	Tlink lobj = Tlink();
	
	printf("|%s|\n", lobj.toString().c_str());
	
	string ldata = string("link\n") +
					 	  "  name sniff\n" +
					 	  "  from tele1:f\n" +
					 	  "  to tele2:b\n" +
					 	  "end\n";
	
	lobj.update(ldata);
	
	printf("|%s|\n", lobj.toString().c_str());
	
	printf("========================================\n");
	
	
	tetra ttobj = tetra();
	
	printf("|%s|\n", ttobj.toString().c_str());
	
	string ttdata = string("tetra\n") +
						   "  matref sniff1\n" +
						   "  matref sniff2\n" +
						   "  matref sniff3\n" +
						   "  shift 1 2 3\n" +
						   "  shear 4 5 6\n" +
						   "  spin 45 0 1 0\n" +
						   "  scale 10 20 30\n" +
						   "  vertex 0 0 0\n" +
						   "  vertex 10 5 0\n" +
						   "  vertex 0 10 0\n" +
						   "  vertex 5 5 5\n" +
						  // "  vertex 100 100 100\n" +
						   "end\n";
	ttobj.update(ttdata);
	
	printf("|%s|\n", ttobj.toString().c_str());
	
	printf("========================================\n");
	
	
	sphere sobj = sphere();
	
	printf("|%s|\n", sobj.toString().c_str());
	
	string sdata = string("sphere\n") +
						  "  name sniffedsphere\n" +
						  "  divisions 22\n" +
						  "  flatshading\n" +
						  "  position 100 200 300\n" +
						  "  rotation 45\n" +
						  "  radius 10\n" +
						  // "  size 20 30 40\n" +
						  "  scale 4 5 6\n" +
						  "  shear 7 8 9\n" +
						  "  shift 10 11 12\n" +
						  "  spin 56 0 1 0\n" +
						  "  phydrv my_physics_driver\n" +
						  "  smoothbounce\n" +
						  "  matref coolMaterial1\n" +
						  "  matref coolmaterial2\n" +
						  "end\n";
						  
	sobj.update(sdata);
	
	printf("|%s|\n", sobj.toString().c_str());
	
	printf("========================================\n");
	
	cone cobj = cone();
	
	printf("|%s|\n", cobj.toString().c_str());
	
	string cdata = string("cone\n") +
						  "  name sniffedCone\n" +
						  "  divisions 22\n" +
						  "  flatshading\n" +
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
						  
	cobj.update(cdata);
	
	printf("|%s|\n", cobj.toString().c_str());
	
	printf("========================================\n");
	
	meshpyr mpobj = meshpyr();
	
	printf("|%s|\n", mpobj.toString().c_str());
	
	string mpdata = string("meshpyr\n") +
						   "  position 1 2 3\n" +
						   "  rotation 45\n" +
						   "  size 5 6 7\n" +
						   "  matref sniff\n" +
						   "  matref yoink\n" +
						   "  phydrv poke\n" +
						   "end\n";
						   
	mpobj.update(mpdata);
	
	printf("|%s|\n", mpobj.toString().c_str());
	
	printf("========================================\n");
	
	
	meshbox mbobj = meshbox();
	
	printf("|%s|\n", mbobj.toString().c_str());
	
	string mbdata = string("meshbox\n") +
						   "  position 1 2 3\n" +
						   "  rotation 5\n" +
						   "  size 4 5 6\n" +
						   "  top matref sniff\n" +
						   "  outside matref yoink\n" +
						   "  matref narf\n" +
						   "  phydrv poop\n" +
						   "end\n";
						   
	mbobj.update(mbdata);
	
	printf("|%s|\n", mbobj.toString().c_str());
	
	printf("========================================\n");
	
	
	base bobj = base();
	
	printf("|%s|\n", bobj.toString().c_str());
	
	string bdata = string("base\n") +
						  "  position 1 2 3\n" +
						  "  size 4 5 6\n" +
						  "  rotation 7\n" +
						  "  color 1\n" +
						  "  oncap HM\n" +
						  "  sniff yoink\n" +
						  "  narf\n" +
						  "end\n";
	
	bobj.update(bdata);
	
	printf("|%s|\n", bobj.toString().c_str());
	
	printf("========================================\n");
	
	
	teleporter tobj = teleporter();
	printf("|%s|\n", tobj.toString().c_str());
	
	string tdata = string("teleporter Linkage\n") +
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
	
	printf("==================================================\n");
	
	define defobj = define();
	
	printf("|%s|\n", defobj.toString().c_str());
	
	string defdata = string("define sniff\n") + meshData + "\n" + adata + "\n" + bdata + "\n" + gdata + "\n" + tdata + "\n" + pyramidData + "\n" + mbdata + "\n" + mpdata + "\n" + "enddef\n";
	
	defobj.update(defdata);
	
	printf("|%s|\n", defobj.toString().c_str());
	
	return 0;*/
}
