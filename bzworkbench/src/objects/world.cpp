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

#include "objects/world.h"

world::world() : DataEntry("world", "<name><size><flagHeight><noWalls>") {
	this->worldName = string("");
	this->size = 400.0;
	this->noWalls = false;
	this->flagHeight = 10.0f;
}

world::world(string& data) : DataEntry("world", "<name><size><flagHeight><noWalls>", data.c_str()) {
	this->worldName = string("");
	this->size = 400.0;
	this->noWalls = false;
	this->flagHeight = 10.0f;
	
	this->update(data);	
}

// send the data
string world::get(void) {
	return this->toString();
}

// receive the data
int world::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the world data objects
	vector<string> worldDatas = BZWParser::getSectionsByHeader(header, data.c_str());
	if(worldDatas[0] == BZW_NOT_FOUND) {
		return 0;
	}
		
	if(!hasOnlyOne(worldDatas, "world"))
		return 0;
	
	const char* worldData = worldDatas[0].c_str();
	
	// get the names, but only choose one
	vector<string> names = BZWParser::getValuesByKey("name", header, worldData);
	if(names.size() > 1) {
		printf("world::update(): Error! Defined \"name\" %d times!\n", names.size());
		return 0;
	}
	
	// get the sizes, but only choose one
	vector<string> sizes = BZWParser::getValuesByKey("size", header, worldData);
	if(!hasOnlyOne(sizes, "size"))
		return 0;
	
	// get the flagHeights, but only choose one
	vector<string> flagHeights = BZWParser::getValuesByKey("flagHeight", header, worldData);
	if(flagHeights.size() > 1) {
		printf("world::update(): Error! Defined \"flagHeight\" %d times!\n", flagHeights.size());
		return 0;	
	}
	
	// get the noWalls value, but only choose one
	vector<string> noWallses = BZWParser::getValuesByKey("noWalls", header, worldData);
	
	// fill in the data
	if(!DataEntry::update(data))
		return 0;
	this->worldName = (names.size() != 0 ? names[0] : "");
	this->size = atof( sizes[0].c_str() );
	this->flagHeight = (flagHeights.size() > 0 ? atof( flagHeights[0].c_str() ) : 1.0f);
	this->noWalls = (noWallses.size() == 0 ? false : true);
	
	return 1;
}

// toString method
string world::toString(void) {
	string sizeString = string(ftoa(size));
	string flagHeightString = string(ftoa(flagHeight));
	return string(string("world\n") +
						 (worldName.length() != 0 ? "  name " + worldName : "# name") + "\n" +
						 "  size " + sizeString + "\n" +
						 "  flagHeight " + flagHeightString + "\n" +
							(noWalls == true ? "  noWalls\n" : "# noWalls\n") +
							this->getUnusedText() +
						 "end\n");
				  
}

// render method
int world::render(void) { return 0; } 
