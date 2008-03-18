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

#include "objects/waterLevel.h"

waterLevel::waterLevel() : DataEntry("waterLevel", "<name><height><matref>") {
	name = string("");
	material = string("");
	height = -1.0f;
}

waterLevel::waterLevel(string& data) : DataEntry("waterLevel", "<name><height><matref>", data.c_str()) {
	name = string("");
	material = string("");
	height = -1.0f;
	
	update(data);
}

// get method
string waterLevel::get(void) {
	return toString();
}

// update method
int waterLevel::update(string& data) {
	
	const char* header = getHeader().c_str();
	// make sure there's only one of these
	vector<string> waterLevelObjs = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(waterLevelObjs[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(waterLevelObjs, "waterLevel"))
		return 0;
		
	if(waterLevelObjs.size() >= 2) {
		printf("waterLevel::update():  Error! waterLevel defined %d times!\n", (int)waterLevelObjs.size());
		return 0;
	}
	
	const char* waterData = waterLevelObjs[0].c_str();
	
	// get the names, but only choose the first
	vector<string> names = BZWParser::getValuesByKey("name", header, waterData);
	if(names.size() > 1) {
		printf("waterLevel::update(): error! defined \"name\" %d times!\n", (int)names.size());
		return 0;
	}
	
	// get the material names, but only choose the last
	vector<string> matNames = BZWParser::getValuesByKey("matref", header, waterData);
	
	// get the water heights, but only choose the last
	vector<string> heights = BZWParser::getValuesByKey("height", header, waterData);
	if(!hasOnlyOne(heights, "height"))
		return 0;
		
	// load the data
	if(!DataEntry::update(data))
		return 0;
	name = names[0];
	material = matNames[0];
	height = atof( heights[0].c_str() );
	
	return 1;
	
}

// toString method
string waterLevel::toString(void) {
	string waterLevelStr = string(ftoa(height));
	string materialString = material;

		
	return string(string("waterLevel\n") +
						 "  name " + name + "\n" +
						 "  height " + waterLevelStr + "\n" +
						 materialString +
						 getUnusedText() + 
						 "end\n");
}


waterLevel::~waterLevel() { }
