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

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "DataEntry.h"
#include "render/Point3D.h"
#include "ftoa.h"
#include "model/BZWParser.h"

#include <osg/Referenced>

class physics : public DataEntry, public osg::Referenced {

public:

	// default constructor
	physics();
	
	// constructors for osg::Referenced
	physics( bool threadSafe );
	physics( const osg::Referenced& ref );
	
	// constructor with data
	physics(string& data);
	
	static DataEntry* init() { return new physics(); }
	static DataEntry* init(string& data) { return new physics(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	// getters
	string& getName() { return this->name; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	
private:
	
	Point3D linear, angular;
	float slide;
	string name, deathMessage;
};

#endif /*PHYSICS_H_*/
