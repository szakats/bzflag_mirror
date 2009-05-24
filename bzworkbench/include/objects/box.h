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

#ifndef BOX_H_
#define BOX_H_

#include "DataEntry.h"
#include "render/Point3D.h"
#include "ftoa.h"
#include "model/BZWParser.h"
#include "bz2object.h"

#include "model/SceneBuilder.h"
#include "model/Primitives.h"

#include "render/GeometryExtractorVisitor.h"

#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/Group>

/**
 * Box data
 */
class box : public bz2object {

public:

	// constructor
	box();
	
	// constructor with data
	box(string& data);
	
	static DataEntry* init() { return new box(); }
	static DataEntry* init(string& data) { return new box( data ); }
	
	// nothing to destroy...
	virtual ~box();
	
	// getter
	string get(void);
	
	// setters
	int update(string& data);
	int update(UpdateMessage& msg);
	
	// toString
	string toString(void);

	virtual void setSize( const osg::Vec3d& newSize );
	
private:

	
};

#endif /*BOX_H_*/
