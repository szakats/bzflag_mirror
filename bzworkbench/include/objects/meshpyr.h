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

#ifndef MESHPYR_H_
#define MESHPYR_H_

#include "bz2object.h"

class meshpyr : public bz2object {
public:
	
	meshpyr();
	meshpyr(string& data);
	
	static DataEntry* init() { return new meshpyr(); }
	static DataEntry* init(string& data) { return new meshpyr(data); }
	
	// restore default values
	void setDefaults();

	// getter
	string get(void);
	
	// setter
	int update(string& data);
	int update( UpdateMessage& message );
	
	// tostring
	string toString(void);
	
	// render
	int render(void);

	void setSize( const osg::Vec3d& newSize );
	
private:

	void updateGeometry();
};

#endif /*MESHPYR_H_*/
