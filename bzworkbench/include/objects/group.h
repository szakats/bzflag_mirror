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
#include "render/RGBA.h"
#include "model/BZWParser.h"

#include <string>
#include <vector>

#include <osg/Vec3>

// use this node name to identify transformation nodes containing bz2objects that do NOT support
// BZW2 features, such as spin, shift, etc.
#define BZW1_CONTAINER_NAME "bzw1_CONTAINER_name"

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
		define* getDefine() { return def; }
		
		// set the definition
		void setDefine( define* def );
		
	private:
		// member data
		RGBA tintColor;
		bool driveThrough, shootThrough;
		int team;
		
		// the actual object group container
		osg::ref_ptr< Renderable > container;
		
		// map objects in the group definition that do NOT support BZW2 features to transformation nodes
		// that will "undo" transformations applied to the container.  Essentially, these nodes will
		// exist between the container and the actual objects; we need to ref them to quickly distinguish
		// between BZW1 and BZW2 objects.
		map< osg::ref_ptr< bz2object >, osg::ref_ptr< Renderable > > bzw1_containers;
		
		// reference to the definition
		define* def;
		
		// build the ring-like structure around the objects
		void buildGeometry();
		
		// update the child objects
		void updateObjects();
		
		// re-compute the children
		void computeChildren();
		
		// the container node
		osg::ref_ptr< osg::Geode > geoRing;
		
		// propogate events to child objects
		void propogateEventToChildren( UpdateMessage& msg );
};

#include "define.h"

#endif /*GROUP_H_*/
