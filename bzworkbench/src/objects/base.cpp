/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
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
	
	team = 0;
	weapon = "";
	
	string meshname = getBaseMesh( team );
	
	baseNode = SceneBuilder::buildNode( meshname.c_str() ); 
	addChild( baseNode.get() );
	
	setName( SceneBuilder::nameNode( meshname.c_str() ) );
	
	setPos( osg::Vec3(0.0, 0.0, 0.0) );
	setSize( osg::Vec3(10.0, 10.0, 1.0) );
	SceneBuilder::markUnselected( this );
}

// constructor with data
base::base(string& data) :
	bz2object("base", "<position><rotation><size><color><oncap>", data.c_str()) {
		
	weapon = "";
	team = 0;
	
	update(data);
	
	string meshname = getBaseMesh( team );
	
	baseNode = SceneBuilder::buildNode( meshname.c_str() ); 
	addChild( baseNode.get() );
	
	setName( SceneBuilder::nameNode( meshname.c_str() ) );
	
	SceneBuilder::markUnselected( this );	
}

// constructor with binary data
base::base( osg::Vec3 position, float rotation, osg::Vec3 size, int team, string weapon ) :
	bz2object("base", "<position><rotation><size><color><oncap>") {
	
	weapon = weapon;
	team = team;
	
	string meshname = getBaseMesh( team );
	
	baseNode = SceneBuilder::buildNode( meshname.c_str() ); 
	addChild( baseNode.get() );
	
	setName( SceneBuilder::nameNode( meshname.c_str() ) );
	
	setPos( position );
	setRotationZ( rotation );
	setSize( size );
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
		printf("base::update():  Error! Defined \"color\" %d times!\n", teams.size());
		return 0;
	}
	if( teams.size() == 0 )
		teams.push_back("0");
		
	// get the weapon
	vector<string> weapons = BZWParser::getValuesByKey("oncap", header, baseData);
	if(weapons.size() > 1) {
		printf("base::update():  Error! Defined \"oncap\" %d times!\n", weapons.size());
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
	team = t;
	weapon = (weapons.size() > 0 ? weapons[0] : string(""));
	
	return 1;
}

// tostring
string base::toString() {
	return string("base\n") +
				  BZWLines() +
				  "  color " + string(itoa(team)) + "\n" +
				  (weapon.length() != 0 ? "  oncap " + weapon + "\n" : "") +
				  "end\n";	
}

// render
int base::render(void) {
	return 0;	
}

// get the base mesh name by team
string base::getBaseMesh( int t ) {
	switch( t ) {
		case BASE_RED:
			return "share/base/red_base.obj";
		case BASE_GREEN:
			return "share/base/green_base.obj";
		case BASE_BLUE:
			return "share/base/blue_base.obj";
		case BASE_PURPLE:
			return "share/base/purple_base.obj";
		default:
			return "share/base/unknown_base.obj";	
	}
}

// set the current team
void base::setTeam( int t ) {
	// get rid of the previous base node
	removeChild( baseNode.get() );
	
	// get the new base mesh name
	string name = getBaseMesh( t );
	
	// build the node and add it
	baseNode = SceneBuilder::buildNode( name.c_str() ); 
	addChild( baseNode.get() );
	
	// set the team
	team = t;
	
	// set the node name
	setName( SceneBuilder::nameNode( name.c_str()) );
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
