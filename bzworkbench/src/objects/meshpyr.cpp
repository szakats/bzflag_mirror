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

#include "objects/meshpyr.h"

// default constructor
meshpyr::meshpyr() {
	setDefaults();
}

// constructor with data
meshpyr::meshpyr(string& data) {
	setDefaults();

	update(data);
}

void meshpyr::setDefaults() {
	// define some basic values
	divisions = 4;
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	flipz = false;
	pyramidStyle = true;

	sweepAngle = 360.0f;

	// make group and geodes
	osg::Group* group = new osg::Group();
	for (int i = 0; i < 4; i++)
		group->addChild( new osg::Geode() );
	setThisNode( group );

	// assign default textures
	SceneBuilder::assignTexture( "pyrwall", group->getChild( 0 ) );
	SceneBuilder::assignTexture( "pyrwall", group->getChild( 1 ) );
	SceneBuilder::assignTexture( "pyrwall", group->getChild( 2 ) );
	SceneBuilder::assignTexture( "pyrwall", group->getChild( 3 ) );

	// default size is 10x10x10
	setSize( osg::Vec3( 10, 10, 10 ) );

	// build the geometry
	buildGeometry();
}