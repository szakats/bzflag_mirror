/* BZWorkbench
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

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "dialogs/Fl_Dialog.h"
#include "widgets/QuickLabel.h"
#include "dialogs/WorldOptionsDialog.h"
#include "dialogs/MainMenu.h"

#include "model/Model.h"

#include "windows/MainWindow.h"
#include "windows/ConsoleWindow.h"

#include "model/BZWParser.h"

#include "Transform.h"

#include "dialogs/MasterConfigurationDialog.h"
#include "dialogs/Fl_Error.h"

#include "render/Point3D.h"
#include "render/RGBA.h"

#include "objects/bz2object.h"
#include "objects/box.h"
#include "objects/material.h"
#include "objects/group.h"
#include "objects/arc.h"
#include "objects/pyramid.h"
#include "objects/teleporter.h"
#include "objects/base.h"
#include "objects/meshbox.h"
#include "objects/meshpyr.h"
#include "objects/cone.h"
#include "objects/sphere.h"
#include "objects/tetra.h"
#include "objects/link.h"
#include "objects/zone.h"
#include "objects/weapon.h"
#include "objects/world.h"
#include "objects/options.h"
#include "objects/waterLevel.h"
#include "objects/texturematrix.h"
#include "objects/dynamicColor.h"
#include "objects/mesh.h"
#include "objects/define.h"

#include "TextUtils.h"

#include "BZWBPlugins.h"

#include "dialogs/AdvancedOptionsDialog.h"

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
	Model::registerObject("cone", NULL, "end", cone::init, ConeConfigurationDialog::init);
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

	printf("model addr: %x\n", model);

	// load the main window
	MainWindow* mw = new MainWindow(model);
	mw->resizable(mw);

	// show the main window
	mw->show();

	// load any plugins
	the_mainWindow = mw;
	initPlugins();

	// run the program
	return Fl::run();
}
