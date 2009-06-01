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

#include "objects/box.h"


// constructors
box::box() : bz2object("box", "<name><position><rotation><size>") {
	setDefaults();
}

box::box(string& data) : bz2object("box", "<name><position><rotation><size>") {
	setDefaults();

	this->update( data );
}

// nothing to destroy...
box::~box() { }

void box::setDefaults() {
	updateGeometry();

	setPos( osg::Vec3(0.0, 0.0, 0.0) );
	setSize( osg::Vec3( 10.0, 10.0, 10.0 ) );

	SceneBuilder::markUnselected( this );
}

// getter
string box::get(void) {
	return toString();
}

// setter (string data)
int box::update(string& data) {
	osg::Vec3 size = getSize();

	int result = bz2object::update( data );
	if( result == 0 )
		return result;

	// if size changes then UVs must be regenerated
	if( getSize() != size ) {
		setSize(getSize());
	}

	return result;
}

// setter (with binary data)
// NOTE: don't call superclass update method, because it deals only with transformations (which are n/a here)
int box::update(UpdateMessage& message) {

	int result = bz2object::update( message );

	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotationZ( message.getAsRotation()->z() );
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotationZ( getRotation().z() + message.getAsRotationFactor()->z() );
			break;

		case UpdateMessage::SET_SCALE: {	// handle a new scale (only scale one axis at a time)
			setSize( *(message.getAsScale()) );
			break;
		}

		case UpdateMessage::SET_SCALE_FACTOR: {	// handle a scaling factor (again, 1 axis at a time)
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;
		}
		default:	// unknown event; don't handle
			return result;
	}

	return 1;
}

// toString
string box::toString(void) {
	return string("box\n") +
				  BZWLines( this ) +
				  "end\n";
}

void box::setSize( osg::Vec3 newSize ) {
	Primitives::rebuildBoxUV((osg::Group*)getThisNode(), &newSize);
	bz2object::setSize( newSize );
}

void box::updateGeometry() {
	// make 1 by 1 by 1 box which can be scaled to the proper size
	osg::Group* group = Primitives::buildUntexturedBox( &osg::Vec3( 1, 1, 1 ) );

	// make UV coordinates
	Primitives::rebuildBoxUV( group, &getSize() );

	// load side texture
	osg::Texture2D* sideTexture = Primitives::loadTexture( "share/box/boxwall.png" );

	// load roof texture
	osg::Texture2D* roofTexture = Primitives::loadTexture( "share/box/roof.png" );

	// associate textures with nodes
	for (int i = 0; i < 4; i++)
		Primitives::setNodeTexture( group->getChild( i ), sideTexture );
	for (int i = 4; i < 6; i++)
		Primitives::setNodeTexture( group->getChild( i ), roofTexture );

	setThisNode( group );
}