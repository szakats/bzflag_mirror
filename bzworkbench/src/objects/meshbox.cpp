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

#include "objects/meshbox.h"

#include "model/Primitives.h"

#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880
#endif

// construct an empty box
meshbox::meshbox() {
	setDefaults();
}

// construct a box from data
meshbox::meshbox(string& data) {
	setDefaults();

	update(data);
}

void meshbox::setDefaults() {
	// define some basic values
	ratio = 1.0f;
	divisions = 4;
	angle = 360.0f;
	physicsDriver = NULL;
	flatShading = true;
	smoothbounce = false;
	texsize.set( -8.0f, -8.0f, -8.0f, -8.0f  );
	boxStyle = true;

	osg::Group* group = new osg::Group();
	for (int i = 0; i < MaterialCount; i++)
		group->addChild( new osg::Geode() );

	SceneBuilder::assignTexture( "roof", group->getChild( 0 ) );
	SceneBuilder::assignTexture( "roof", group->getChild( 1 ) );
	SceneBuilder::assignTexture( "boxwall", group->getChild( 3 ) );

	// transform to the correct orientation and size
	Renderable::setRotationZ( 45 );
	bz2object::setSize( osg::Vec3( (float)M_SQRT2, (float)M_SQRT2, 1 ) ); 

	setThisNode( group );

	setSize( osg::Vec3( 10, 10, 10 ) );

}
