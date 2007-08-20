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

#include "../include/objects/meshpyr.h"

// default constructor
meshpyr::meshpyr() : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>") { }

// constructor with data
meshpyr::meshpyr(string& data) : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>", data.c_str()) { this->update(data); }

// getter
string meshpyr::get(void) { return this->toString(); }

// setter
int meshpyr::update(string& data) {
	return bz2object::update(data);	
}

// tostring
string meshpyr::toString(void) {
	return string("meshpyr\n") +
				  this->BZWLines() +
				  "end\n";
}

// render
int meshpyr::render(void) {
	return 0;	
}
