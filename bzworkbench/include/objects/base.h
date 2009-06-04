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

#ifndef BASE_H_
#define BASE_H_

#include "bz2object.h"
#include "model/BZWParser.h"
#include "defines.h"

#include <string>
#include <vector>

using namespace std;

class base : public bz2object {
public:

	// constructor
	base();

	// constructor with data
	base(string& data);

	static DataEntry* init() { return new base(); }
	static DataEntry* init(string& data) { return new base(data); }

	// restore default values
	void setDefaults();

	// getter
	string get(void);

	// setter
	int update(string& data);

	// event handler
	int update( UpdateMessage& msg );

	// tostring
	string toString();

	// render
	int render(void);

	// BZWB-specific getters and setters
	int getTeam() { return team; }
	string& getWeapon() { return weapon; }

	void setTeam( int team );
	void setWeapon( const char* weapon ) { this->weapon = weapon; }

	void setSize( osg::Vec3 newSize );

private:
	int team;
	string weapon;

	void updateGeometry();

	void updateBaseUV( osg::Group* base, osg::Vec3 size );

	void setBaseColor( int team );
};

#endif /*BASE_H_*/
