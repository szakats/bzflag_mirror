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

#include "objects/zone.h"

#include <osg/PolygonMode>

// constructor
zone::zone() : bz2object("zone", "<name><position><size><rotation><team><flag><safety><zoneflag>") {
	setDefaults();
}

// constructor with data
zone::zone(string& data) : bz2object("zone", "<name><position><size><rotation><team><flag><safety><zoneflag>", data.c_str()) {
	setDefaults();
	
	update(data);
}

void zone::setDefaults() {
	updateGeometry();

	teams = vector<int>();
	safety = vector<int>();
	setSize( osg::Vec3( 10, 10, 10 ) );
}

// getter
string zone::get(void) { return toString(); }

// setter
int zone::update(string& data) {
	const char* header = getHeader().c_str();
	
	// get the sections
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// return if none were found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	// return if more than one were found
	if(!hasOnlyOne(lines, header))
		return 0;
		
	// get the data
	const char* zoneData = lines[0].c_str();
	
	// get the team values
	vector<string> tempTeams = BZWParser::getValuesByKey("team", header, zoneData);
		
	// get the safety values
	vector<string> safties = BZWParser::getValuesByKey("safety", header, zoneData);
		
	// get the flag values
	vector<string> flagVals = BZWParser::getValuesByKey("flag", header, zoneData);
	
	// get the zoneflag values (multiple values possible)
	vector<string> zoneFlagVals = BZWParser::getValuesByKey("zoneflag", header, zoneData);
		
	// data
	vector<string> teamElements = vector<string>(), flagElements = vector<string>(), safetyElements = vector<string>();
	
	// read in the given teams
	if(tempTeams.size() > 0)
		teamElements = BZWParser::getLineElements( tempTeams[0].c_str() );
	
	// read in the flags
	if(flagVals.size() > 0)
		flagElements = BZWParser::getLineElements(flagVals[0].c_str());
	
	// read the safeties
	if(safties.size() > 0)
		safetyElements = BZWParser::getLineElements(safties[0].c_str());
	
	// determine whether or not the given flags are valid (throw a warning if its invalid)
	if(flagElements.size() > 0) {
		for(vector<string>::iterator i = flagElements.begin(); i != flagElements.end(); i++) {
			if(	! (Flag::isFlag( i->c_str() )  || (*i) == "good" || (*i) == "bad") ) {
				printf("zone::update(): Error! Unrecognized flag type \"%s\"\n", i->c_str());
				return 0;
			}
		}
	}
	// candidate teams
	vector<int> teamCandidates = vector<int>();
	
	// determine whether or not the given teams are valid (throw a warning if invalid)
	for(vector<string>::iterator i = teamElements.begin(); i != teamElements.end(); i++) {
		int teamNumber = atoi( i->c_str() );
		if(teamNumber < 0 || teamNumber > 4) {
			printf("zone::update(): Error! Undefined team \"%d\" in \"team\"\n", teamNumber);	
			return 0;
		}
		teamCandidates.push_back(teamNumber);
	}
	
	// candidate safeties
	vector<int> safetyCandidates = vector<int>();
	
	// determine whether or not the given safety values are valid (throw a warning if not)
	if(safetyElements.size() > 0) {
		for(vector<string>::iterator i = safetyElements.begin(); i != safetyElements.end(); i++) {
			int teamNumber = atoi( i->c_str() );
			if(teamNumber < 0 || teamNumber > 4) {
				printf("zone::update(): Error! Undefined team \"%d\" in \"safety\"\n", teamNumber);	
				return 0;
			}
			safetyCandidates.push_back(teamNumber);
		}
	}
	
	// candidate flag/quantity pairs
	vector<FlagElement> zoneflagCandidates = vector<FlagElement>();
	
	// parse the zoneflag values, and make sure they're valid
	if(zoneFlagVals.size() > 0) {
		for(vector<string>::iterator i = zoneFlagVals.begin(); i != zoneFlagVals.end(); i++) {
			// read the flag
			string flag = BZWParser::key(i->c_str());
			// see if its valid
			if(!Flag::isFlag(flag.c_str())) {
				printf("zone::update(): Error! Unrecognized flag type \"%s\"\n", flag.c_str());
				return 0;
			}
			// try to read the quantity
			string value = BZWParser::value(flag.c_str(), i->c_str());
			// try to atoi() the value
			int num = atoi( value.c_str() );
			// num will be zero if atoi() failed...
			if(num == 0) {
				printf("zone::update(): Error! Unrecognized flag quantity \"%s\"\n", value.c_str());	
				return 0;
			}
			
			zoneflagCandidates.push_back(FlagElement(flag, num));
		}
	}
	
	// fill in the data
	if(!bz2object::update(data))
		return 0;
	 
	teams = teamCandidates;
	safety = safetyCandidates;
	zoneflags = zoneflagCandidates;
	flags = flagElements;
	
	return 1;
}

int zone::update( UpdateMessage& message ) {
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

// toString
string zone::toString(void) {
	// string-ify the teams, safeties, zoneflags, and flags
	string teamString = string("");
	string safetyString = string("");
	string zoneflagString = string("");
	string flagString = string("");
	
	// make the "safety" string
	if(safety.size() > 0) {
		for(vector<int>::iterator index = safety.begin(); index != safety.end(); index++) {
			safetyString += string(itoa(*index)) + " ";
		}
		safetyString += "\n";
	}
	
	if(teams.size() > 0) {
		// make the "team" string
		for(vector<int>::iterator index = teams.begin(); index != teams.end(); index++) {
			teamString += string(itoa(*index)) + " ";
		}
		teamString += "\n";
	}
	
	if(flags.size() > 0) {
		// make the "flag" string
		for(vector<string>::iterator index = flags.begin(); index != flags.end(); index++) {
			flagString += (*index) + " ";
		}
		flagString += "\n";
	}
	
	if(zoneflags.size() > 0) {
		// make the "zoneflag" strings
		for(vector<FlagElement>::iterator index = zoneflags.begin(); index != zoneflags.end(); index++) {
			zoneflagString += "  zoneflag " + index->toString() + "\n";	
		}
	}
	
	return string("zone\n") +
				  BZWLines( this ) +
				  (safetyString.length() == 0 ? "# safety\n" : "  safety " + safetyString) +
				  (teamString.length() == 0 ? "# team\n" : "  team " + teamString) +
				  (flagString.length() == 0 ? "# flag\n" : "  flag " + flagString) +
				  (zoneflagString.length() == 0 ? "# zoneflag\n" : zoneflagString) +
				  "end\n";
}

// render
int zone::render(void) {
	return 0;	
}

void zone::updateGeometry() {
	// create unit box
	osg::ref_ptr< osg::Geode > boxGeode = new osg::Geode();
	osg::ref_ptr< osg::Box > box = new osg::Box( osg::Vec3( 0, 0, 0.5f ), 2, 2, 1 );
	osg::ref_ptr< osg::ShapeDrawable > boxDrawable = new osg::ShapeDrawable( box );
	boxGeode->addDrawable( boxDrawable );

	// make material
	boxDrawable->setColor( osg::Vec4( 0, 0, 1, 1 ) );
	osg::ref_ptr< osg::StateSet > stateset = boxDrawable->getOrCreateStateSet();
	osg::ref_ptr< osg::PolygonMode > polyMode = new osg::PolygonMode();
	polyMode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	stateset->setAttribute( polyMode.get(), osg::StateAttribute::ON);

	setThisNode( boxGeode );
}