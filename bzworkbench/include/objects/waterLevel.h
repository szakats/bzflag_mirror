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

#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "DataEntry.h"
#include <string>

using namespace std;

class waterLevel : public DataEntry {
public:

	// default constructor
	waterLevel();
	
	// constructor with data
	waterLevel(string& data);
	
	static DataEntry* init() { return new waterLevel(); }
	static DataEntry* init(string& data) { return new waterLevel(data); }
	
	// get method
	string get(void);
	
	// update method
	int update(string& data);
	
	// toString method
	string toString(void);
	
	// getters/setters
	string getName() { return name; }
	string getMaterial() { return material; }
	float getHeight() { return height; }
	
	void setName( string name ) { name = name; }
	void setMaterial( string material ) { material = material; }
	void setHeight( float height ) { height = height; }
	
	virtual ~waterLevel();
	
private:
	string name;
	string material;
	float height;
};

#endif /*WATERLEVEL_H_*/
