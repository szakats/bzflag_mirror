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

#include "include/dialogs/AdvancedOptionsDialog.h"

#include <string>
#include <vector>

#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateAttribute>
#include <osg/TexEnv>
#include <osg/Group>


// register the built-in objects
void buildModelDatabase() {
	Model::registerObject("arc", NULL, "end", arc::init);
	Model::registerObject("base", NULL, "end", base::init);
	Model::registerObject("box", NULL, "end", box::init);
	Model::registerObject("cone", NULL, "end", cone::init);
	Model::registerObject("dynamicColor", NULL, "end", dynamicColor::init);
	Model::registerObject("group", NULL, "end", group::init);
	Model::registerObject("link", NULL, "end", Tlink::init);
	Model::registerObject("material", NULL, "end", material::init);
	Model::registerObject("mesh", "<mesh:<face><drawinfo>><drawinfo:<lod>><lod:<matref>>", "end", mesh::init);
	// need to do this for faces
	Model::addTerminatorSupport("face", "endface");
	
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
	
	Model::registerObject("define", "<define:<arc><base><box><cone><group><mesh><meshbox><meshpyr><pyramid><sphere><teleporter><tetra>>", "enddef", define::init);
	
}


int main(int argc, char** argv) {
	
	// init the model
	Model* model = new Model();
	
	// add supported objects
	buildModelDatabase();
	
	// initialize the BZWParser
	BZWParser::init( model );
	
	// init the SceneBuilder
	SceneBuilder::init();
	
	// load the main window
	MainWindow* mw = new MainWindow(model);
	mw->resizable(mw);
	
	// show the main window
	mw->show();
	
	// run the program
	return Fl::run();
	
}
