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
	
	// constructor with binary data (BZWB-specific)
	base( osg::Vec3 position, float rotation, osg::Vec3 size, int team, string weapon );
	
	static DataEntry* init() { return new base( osg::Vec3( 0.0, 0.0, 0.0 ), 0.0f, osg::Vec3( 10.0, 10.0, 1.0 ), 0, "" ); }
	static DataEntry* init(string& data) {
		if( data.size() <= 1 )
			return new base( osg::Vec3( 0.0, 0.0, 0.0 ), 0.0f, osg::Vec3( 10.0, 10.0, 1.0 ), 0, "" );
		else
			return new base(data);
	}
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
	
private:
	int team;
	string weapon;
	
	// helper method: get the appropriate mesh filename by team
	string getBaseMesh( int team );
	
	// the current base node
	osg::ref_ptr< osg::Node > baseNode;
};

#endif /*BASE_H_*/
