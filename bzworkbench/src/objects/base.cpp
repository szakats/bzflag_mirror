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

#include "objects/base.h"

// constructor
base::base() :
	bz2object("base", "<position><rotation><size><color><oncap>") {
	
	setDefaults();
}

// constructor with data
base::base(string& data) :
	bz2object("base", "<position><rotation><size><color><oncap>", data.c_str()) {
		
	setDefaults();

	update( data );
}

void base::setDefaults() {
	updateGeometry();

	team = 0;
	weapon = "";
	
	setPos( osg::Vec3(0.0, 0.0, 0.0) );
	setSize( osg::Vec3(10.0, 10.0, 1.0) );

	SceneBuilder::markUnselected( this );
}

// getter
string base::get(void) { return toString(); }

// setter
int base::update(string& data) {
	// get the header
	const char* header = getHeader().c_str();
	
	// get the section
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	if(!hasOnlyOne(lines, "base"))
		return 0;
		
	// get the section data
	const char* baseData = lines[0].c_str();
	
	// get the team
	vector<string> teams = BZWParser::getValuesByKey("color", header, baseData);
	if(teams.size() > 1) {
		printf("base::update():  Error! Defined \"color\" %d times!\n", (int)teams.size());
		return 0;
	}
	if( teams.size() == 0 )
		teams.push_back("0");
		
	// get the weapon
	vector<string> weapons = BZWParser::getValuesByKey("oncap", header, baseData);
	if(weapons.size() > 1) {
		printf("base::update():  Error! Defined \"oncap\" %d times!\n", (int)weapons.size());
		return 0;
	}
		
	// make sure that the team value is sane
	int t = atoi(teams[0].c_str());
	if(!(t == BASE_RED || t == BASE_GREEN || t == BASE_BLUE || t == BASE_PURPLE)) {
		printf("base::update():  Warning! Base team (%d) is not recognized...\n", t);	
	}
	
	// do superclass update
	if(!bz2object::update(data))
		return 0;
	
	// load in the data
	setTeam( t );
	setWeapon( (weapons.size() > 0 ? weapons[0] : string("")).c_str() );
	
	return 1;
}

// tostring
string base::toString() {
	return string("base\n") +
				  BZWLines( this ) +
				  "  color " + string(itoa(team)) + "\n" +
				  (weapon.length() != 0 ? "  oncap " + weapon + "\n" : "") +
				  "end\n";	
}

// render
int base::render(void) {
	return 0;	
}

// set the current team
void base::setTeam( int t ) {
	// set the team
	team = t;
	
	// set new color
	setBaseColor( t );
}

// setter (with binary data)
// NOTE: don't call superclass update method, because it deals only with transformations (which are n/a here)
int base::update(UpdateMessage& message) {
	
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

void base::setSize( const osg::Vec3d& newSize ) {
	updateBaseUV( (osg::Group*)getThisNode(), &(osg::Vec3)newSize );

	bz2object::setSize( newSize );
}

void base::updateGeometry() {
	osg::Group* group = Primitives::buildUntexturedBox( &osg::Vec3( 1, 1, 1 ) );
	setThisNode( group );

	// make UV coordinates
	updateBaseUV( group, &getSize() );

	setBaseColor( getTeam() );
}


// regenerate base UVs
void base::updateBaseUV( osg::Group* base, const osg::Vec3* size ) {
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	// FIXME: properly generate side UVs for base
	float xSideUV = 1;
	float ySideUV = 1;
	float zSideUV = 1;
	float xTopUV = 1;
	float yTopUV = 1;

	// +x -x
	for ( int i = 0; i < 2; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, 0 ) );
	}

	// +y -y
	for ( int i = 2; i < 4; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, 0 ) );
	}

	// +z -z
	for ( int i = 4; i < 6; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, 0 ) );
	}

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)base->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

void base::setBaseColor( int team ) {
	osg::Group* group = (osg::Group*)getThisNode();

	string wallTexFile;
	string topTexFile;

	switch( team ) {
		case BASE_RED:
			wallTexFile = "share/base/red_basewall.png";
			topTexFile = "share/base/red_basetop.png";
			break;
		case BASE_GREEN:
			wallTexFile = "share/base/green_basewall.png";
			topTexFile = "share/base/green_basetop.png";
			break;
		case BASE_BLUE:
			wallTexFile = "share/base/blue_basewall.png";
			topTexFile = "share/base/blue_basetop.png";
			break;
		case BASE_PURPLE:
			wallTexFile = "share/base/purple_basewall.png";
			topTexFile = "share/base/purple_basetop.png";
			break;
		default:
			wallTexFile = "share/box/boxwall.png";
			topTexFile = "share/box/roof.png";	
	}

	// load side texture
	osg::Texture2D* wallTexture = Primitives::loadTexture( wallTexFile );

	// load roof texture
	osg::Texture2D* topTexture = Primitives::loadTexture( topTexFile );

	// associate textures with nodes
	for (int i = 0; i < 4; i++)
		Primitives::setNodeTexture( group->getChild( i ), wallTexture );
	for (int i = 4; i < 6; i++)
		Primitives::setNodeTexture( group->getChild( i ), topTexture );

}