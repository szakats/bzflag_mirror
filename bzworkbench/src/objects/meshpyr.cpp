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
meshpyr::meshpyr() : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>") { 
	setDefaults();
}

// constructor with data
meshpyr::meshpyr(string& data) : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>", data.c_str()) {
	setDefaults();

	update(data); 
}

void meshpyr::setDefaults() {
	updateGeometry();

	setPos( osg::Vec3(0.0, 0.0, 0.0) );
	setSize( osg::Vec3(10.0, 10.0, 10.0) );
	SceneBuilder::markUnselected( this );
}

// getter
string meshpyr::get(void) { return toString(); }

// setter
int meshpyr::update(string& data) {
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

int meshpyr::update(UpdateMessage& message ) {
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
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;
			
		default:	// unknown event; don't handle
			return 0;
	}
}

// tostring
string meshpyr::toString(void) {
	return string("meshpyr\n") +
				  BZWLines( this ) +
				  "end\n";
}

// render
int meshpyr::render(void) {
	return 0;	
}

void meshpyr::setSize( osg::Vec3 newSize ) {
	Primitives::rebuildPyramidUV( (osg::Geode*)getThisNode(), &(osg::Vec3)newSize );
	bz2object::setSize( newSize );
}

void meshpyr::updateGeometry() {
	osg::Node* node = Primitives::buildPyramid( &osg::Vec3( 1, 1, 1 ) );

	setThisNode( node );
}