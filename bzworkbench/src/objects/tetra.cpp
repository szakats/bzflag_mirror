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

#include "objects/tetra.h"

// no arg constructor
tetra::tetra() : bz2object("tetra", "<name><vertex><scale><shift><shear><spin><matref>") {
	for(int i = 0; i < 4; i++)
		vertexes[i] = Point3D(0.0f, 0.0f, 0.0f);
}

// data constructor
tetra::tetra(string& data) : bz2object("tetra", "<name><vertex><scale><shift><shear><spin><matref>", data.c_str()) {
	for(int i = 0; i < 4; i++)
		vertexes[i] = Point3D(0.0f, 0.0f, 0.0f);
		
	update(data);
}

// getter
string tetra::get(void) { return toString(); }

// setter
int tetra::update(string& data) {
	const char* header = getHeader().c_str();
	
	// get the sections
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	if(!hasOnlyOne(lines, "tetra"))
		return 0;
		
	// get the section we need
	const char* tetraData = lines[0].c_str();
	
	// read in the vertices
	vector<string> vertexDefs = BZWParser::getValuesByKey("vertex", header, tetraData);
	if(vertexDefs.size() != 4) {
		printf("tetra::update(): Error! You can only define 4 vertexes!\n");
		return 0;	
	}
	
	// update the base class
	if(!bz2object::update(data))
		return 0;
	
	// update this class's data
	int index = 0;
	for(vector<string>::iterator i = vertexDefs.begin(); i != vertexDefs.end(); i++) {
		vertexes[index++] = Point3D(i->c_str());	
	}
	
	return 1;
}

// tostring
string tetra::toString(void) {
	return string("tetra\n") +
				  BZWLines() +
				  "  vertex " + vertexes[0].toString() + 
				  "  vertex " + vertexes[1].toString() +
				  "  vertex " + vertexes[2].toString() +
				  "  vertex " + vertexes[3].toString() +
				  "end\n";
}

// render
int tetra::render(void) {
	return 0;	
}
