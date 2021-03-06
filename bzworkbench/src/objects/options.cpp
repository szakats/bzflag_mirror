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

#include "objects/options.h"

// default constructor
options::options() : DataEntry("options", "") {
	optionsString = string("");
}

// constructor with data
options::options(string& data) : DataEntry("options", "", data.c_str()) { update(data); }

// get method
string options::get(void) {
	return toString();
}

// update method
int options::update(string& data) {
	
	const char* header = getHeader().c_str();
	// get options objects
	vector<string> optionses = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(optionses[0] == BZW_NOT_FOUND)
		return 0;
	
	if(!hasOnlyOne(optionses, "options"))
		return 0;
	
	const char* opts = optionses[0].c_str();
	
	// get the lines
	vector<string> options = BZWParser::getLines(header, opts);
	
	// concat the lines
	optionsString.clear();
	
	for(vector<string>::iterator i = options.begin(); i != options.end(); i++) {
		optionsString += *i + " ";
	}
	
	return (DataEntry::update(data));
	
}

// toString method
string options::toString(void) {
	return string(string("options\n") +
						"  " + optionsString + "\n" + 
						getUnusedText() + 
						"end\n");
}
