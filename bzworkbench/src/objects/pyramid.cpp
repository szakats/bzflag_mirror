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

#include "objects/pyramid.h"

#include "model/Primitives.h"

pyramid::pyramid() : bz2object("pyramid", "<position><rotation><size>") {
	setDefaults();
}

// constructor with string
pyramid::pyramid(string& data) : bz2object("pyramid", "<position><rotation><size>") {
	setDefaults();

	this->update(data);	
}

void pyramid::setDefaults() {
	updateGeometry();

	this->setName( SceneBuilder::nameNode("pyramid") );
	
	this->setPos( osg::Vec3(0.0, 0.0, 0.0) );
	this->setSize( osg::Vec3(10.0, 10.0, 10.0) );
	
	SceneBuilder::markUnselected( this );
}

// getter
string pyramid::get() { return toString(); }

// setter
int pyramid::update(string& data) {
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

// setter with messaging
// NOTE: don't call the superclass method, because it deals solely with transformations (which are n/a here)
int pyramid::update(UpdateMessage& message) {
	
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
	
	return 1;
}

// toString
string pyramid::toString(void) {
	return string("pyramid\n") +
				  BZWLines( this ) +
				  "end\n";	
}

void pyramid::setSize( osg::Vec3 newSize ) {
	Primitives::rebuildPyramidUV( (osg::Geode*)getThisNode(), &newSize );
	bz2object::setSize( newSize );
}

void pyramid::updateGeometry() {
	osg::Node* node = Primitives::buildPyramid( &osg::Vec3( 1, 1, 1 ) );

	setThisNode( node );
}