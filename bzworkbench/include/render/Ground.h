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

#ifndef GROUND_H_
#define GROUND_H_

#include <osg/Geode>

#include "../objects/bz2object.h"
#include "../model/SceneBuilder.h"

/**
 * This class stores the ground
 */
class Ground : public Renderable {

public:
	
	// default size
	static const float DEFAULT_SIZE;
	
	// initialize
	Ground( float size, float waterLevel = -1.0f );
	
	// uninitialize
	~Ground();
	
	// getters
	float getSize();
	
	// setters
	void setSize( float size );
	
private:

	// this is the ground's grid
	osg::ref_ptr< Renderable > grid;
	
	// build the grid
	osg::ref_ptr< Renderable > buildGrid( float size, float unit );
	
	// the size of the ground
	float size;
};

#endif /*GROUND_H_*/
