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

#include "objects/teleporter.h"

#include "model/Primitives.h"

// constructor
teleporter::teleporter() :
	bz2object("teleporter", "<position><size><rotation><name><border>", SceneBuilder::buildNode("teleporter")) {
	
	setBorder( 0.125f );
	
	osg::Vec3 scale = osg::Vec3( getBorder(), 10, 20 );
	realSize = scale;
	
	setSize( scale );
	
	SceneBuilder::markUnselected( this );
}

// constructor with data
teleporter::teleporter(string& data) :	// don't pass the data to the parent class (this prevents super-sized teleporters)
	bz2object("teleporter", "<position><size><rotation><name><border>", SceneBuilder::buildNode("teleporter")) {
	
	setBorder( 0.125f );

	osg::Vec3 scale = osg::Vec3( border, 10, 20 );
	realSize = scale;
	
	setSize( realSize );

	update( data );
	
	SceneBuilder::markUnselected( this );
}

// getter
string teleporter::get(void) {  return toString(); }

// setter
int teleporter::update(string& data) {
	// get header
	const char* header = getHeader().c_str();
	
	// get the section
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "teleporter"))
		return 0;
		
	// get the data
	const char* teleporterData = lines[0].c_str();
	
	// get the name if it's in the title
	vector<string> names = BZWParser::getValuesByKey( "teleporter", header, teleporterData );
	
	// get the border
	vector<string> borders = BZWParser::getValuesByKey( "border", header, teleporterData );
	if( borders.size() > 1 ) {
		printf("teleporter::update(): Error! Defined \"border\" %d times!\n", (int)borders.size());
	}

	// get the size, since the teleporter scales differently than other objects
	// the size must be parsed here
	vector<string> sizes = BZWParser::getValuesByKey( "size", header, teleporterData );
		
	// just go with the first size (only one should be defined)
	if(sizes.size() > 0)
		setSize( Point3D( sizes[0].c_str() ) );

	
	if( !bz2object::update( data ) )
		return 0;

	// reset the scale so that it doesn't effect the size of the object
	bz2object::setSize( osg::Vec3( 1, 1, 1 ) );
	
	if( names.size() > 0 ) setName( names[0] );
	
	// set the data
	setBorder( (borders.size() != 0 ? atof( borders[0].c_str() ) : 0.0f) );
	
	return 1;
}

// update with binary message
int teleporter::update( UpdateMessage& message ) {
	
	// superclass event handler
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
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;
			
		default:	// unknown event; don't handle
			return result;
	}
	
	return 1;
}

// tostring
string teleporter::toString(void) {
	
	// get the bz2object BZW lines
	string bzwlines = BZWLines( this );
	
	// finally, make that string
	return string("teleporter") + "\n" +
				  bzwlines +
				  "  border " + string(ftoa(border)) + "\n" +
				  "end\n";	
}

void teleporter::setSize( const osg::Vec3d& newSize ) {
	realSize = (osg::Vec3)newSize;
	
	Primitives::RebuildTeleporterMesh( (osg::Group*)getThisNode(), &getSize(), getBorder() );
	Primitives::RebuildTeleporterUV( (osg::Group*)getThisNode(), &getSize(), getBorder() );
}

void teleporter::setBorder( float newBorder ) { 
	border = newBorder;

	Primitives::RebuildTeleporterMesh( (osg::Group*)getThisNode(), &getSize(), getBorder() );
	Primitives::RebuildTeleporterUV( (osg::Group*)getThisNode(), &getSize(), getBorder() );
}