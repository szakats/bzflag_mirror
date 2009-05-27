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

#include "objects/arc.h"

// default constructor
arc::arc() : 
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	ratio = 1.0f;
	divisions = 16;
	angle = 180.0f;
	setName("default_arc");
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	
}

// data constructor
arc::arc(string& data) :
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
	
	// define some basic values
	ratio = 1.0f;
	divisions = 16;
	angle = 180.0f;
	setName("default_arc");
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	
	update(data);	
}


// getter
string arc::get(void) { return toString(); }

// setter
int arc::update(string& data) {
	const char* header = getHeader().c_str();
	// get the chunk we need
	
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "arc"))
		return 0;
		
	const char* arcData = lines[0].c_str();
		
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, arcData);
	if(!hasOnlyOne(names, "name"))
		return 0;
		
	// get the angle
	vector<string> angles = BZWParser::getValuesByKey("angle", header, arcData);
	if(!hasOnlyOne(angles, "angle"))
		return 0;
	
	// get the divisions
	vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, arcData);
	if(!hasOnlyOne(vDivisions, "divisions"))
		return 0;
		
	// get the ratio
	vector<string> ratios = BZWParser::getValuesByKey("ratio", header, arcData);
	if(!hasOnlyOne(ratios, "ratio"))
		return 0;
			
	// get flatshading
	vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, arcData);
	
	// get smoothbounce
	vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, arcData);
	
	// do base class update
	if(!bz2object::update(data))
		return 0;
	
	// set the data
	setName( names[0] );
	angle = atof( angles[0].c_str() );
	divisions = atoi( vDivisions[0].c_str() );
	ratio = atof( ratios[0].c_str() );
	flatShading = (flatShadings.size() == 0 ? false : true);
	smoothbounce = (smoothBounces.size() == 0 ? false : true);
	
	return 1;	
}

// toString
string arc::toString(void) {
	return string("arc\n") +
				  BZWLines( this ) +
				  "  angle " + string(ftoa(angle)) + "\n" +
				  "  ratio " + string(ftoa(ratio)) + "\n" +
				  "  divisions " + string(itoa(divisions)) + "\n" +
				  (flatShading == true ? "  flatshading\n" : "") +
				  (smoothbounce == true ? "  smoothbounce\n" : "") + 
				  "end\n";	
}

// render
int arc::render(void) {
	return 0;	
}

