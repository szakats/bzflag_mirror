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

#include "objects/physics.h"

physics::physics() :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced() {
	name = string("");
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// osg-specific constructor
physics::physics( bool threadSafe ) :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced( threadSafe ) {
	name = string("");
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// osg-specific constructor
physics::physics( const osg::Referenced& ref ) :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced( ref ) {
	name = string("");
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// constructor with string data
physics::physics(string& data) : 
	DataEntry("physics", "<name><linear><angular><slide><death>", data.c_str()),
	osg::Referenced() {
	name = string("");
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
	
	update(data);	
}

// getter
string physics::get(void) { return toString(); }

// setter
int physics::update(string& data) {
	
	const char* header = getHeader().c_str();
	
	// get the chunk of string we need
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "physics"))
		return 0;
	
	const char* physicsData = lines[0].c_str();
	
	// get name
	vector<string> names = BZWParser::getValuesByKey("name", header, physicsData);
	if(!hasOnlyOne(names, "name"))
		return 0;
	
	// get linear velocity
	vector<string> linearVelocities = BZWParser::getValuesByKey("linear", header, physicsData);
	
	// get angular velocity
	vector<string> angularVelocities = BZWParser::getValuesByKey("angular", header, physicsData);
	
	// get slide
	vector<string> slides = BZWParser::getValuesByKey("slide", header, physicsData);
	
	// get death message
	vector<string> deathMessages = BZWParser::getValuesByKey("death", header, physicsData);
	
	// load the data in
	if(!DataEntry::update(data))
		return 0;
	name = names[0];
	linear = (linearVelocities.size() != 0 ? Point3D( linearVelocities[0].c_str() ) : Point3D( 0.0f, 0.0f, 0.0f ));
	angular = (angularVelocities.size() != 0 ? Point3D( angularVelocities[0].c_str() ): Point3D( 0.0f, 0.0f, 0.0f));
	slide = (slides.size() != 0 ? atof( slides[0].c_str() ) : 0.0f);
	
	if(deathMessages.size() >= 1)
		deathMessage = deathMessages[0];
		
	return 1;
}

// toString
string physics::toString(void) {
	return string("physics\n") +
				  "  name " + name + "\n" +
				  "  linear " + linear.toString() +
				  "  angular " + angular.toString() +
				  "  slide " + string(ftoa(slide)) + "\n" +
				  (deathMessage.length() != 0 ? "  death " : "# death") + deathMessage + "\n" +
				  getUnusedText() + 
				  "end\n";
}

// render
int physics::render(void) {
	return 0;	
}
