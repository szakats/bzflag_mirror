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

#ifndef GROUP_H_
#define GROUP_H_

#include "bz2object.h"
#include "../render/RGBA.h"
#include "../model/BZWParser.h"

#include <string>
#include <vector>

#include <osg/Vec3>

using namespace std;

class define;		// pre-declare define class

class group : public bz2object {
	
	public:
	
		// constructor
		group();
		
		// constructor with data
		group(string& data);
		
		static DataEntry* init() { return new group(); }
		static DataEntry* init(string& data) { return new group(data); }
		
		// getter
		string get(void);
		
		// setter
		int update(string& data);
		
		// event handler
		int update( UpdateMessage& msg);
		
		// toString
		string toString(void);
		
		// get the definition
		define* getDefine() { return this->def; }
		
		// set the definition
		void setDefine( define* def );
		
	private:
		// member data
		RGBA tintColor;
		bool driveThrough, shootThrough;
		int team;
		
		// reference to the definition
		define* def;
		
		// the ring structure around the groups
		osg::ref_ptr< osg::Geode > ring;
		
		// build the ring-like structure around the objects
		void buildGeometry();
		
		// update the child objects
		void updateObjects();
		
		// re-compute the children
		void computeChildren();
		
		osg::Vec3 pos;	// shift the group by pos (since groups have no concept of position)
		
};

#include "define.h"

#endif /*GROUP_H_*/
