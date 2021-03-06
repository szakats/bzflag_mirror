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

#ifndef SPHERE_H_
#define SPHERE_H_

#include "bz2object.h"

class sphere : public bz2object {
public:
	// default constructor
	sphere();
	
	// constructor with data
	sphere(string& data);
	
	static DataEntry* init() { return new sphere(); }
	static DataEntry* init(string& data) { return new sphere(data); }

	// restore default values
	void setDefaults();
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	int update(UpdateMessage& message);
	
	// toString
	string toString(void);
	
	// render
	int render(void);

	void setSize( osg::Vec3 newSize );
	osg::Vec3 getSize();
	
private:
	bool flatShading, smoothbounce, hemisphere;
	int divisions;
	osg::Vec3 realSize;

	void updateGeometry();
};

#endif /*SPHERE_H_*/
