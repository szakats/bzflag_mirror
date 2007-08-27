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

#ifndef ARC_H_
#define ARC_H_

#include "bz2object.h"
#include "model/BZWParser.h"
#include <string>
#include <vector>

using namespace std;

class arc : public bz2object {

public:

	arc();
	
	arc(string& data);
	
	static DataEntry* init() { return new arc(); }
	static DataEntry* init(string& data) { return new arc(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	bool flatShading, smoothbounce;
	float angle, ratio;
	int divisions;
	
};

#endif /*ARC_H_*/