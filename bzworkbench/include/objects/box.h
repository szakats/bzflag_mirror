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

#ifndef BOX_H_
#define BOX_H_

#include "DataEntry.h"
#include "render/Point3D.h"
#include "ftoa.h"
#include "model/BZWParser.h"
#include "bz2object.h"

#include "model/SceneBuilder.h"

#include "render/GeometryExtractorVisitor.h"

#include <osg/Geometry>
#include <osg/PrimitiveSet>

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
	
private:
	
	// update the box's geometry
	void updateGeometry( UpdateMessage& msg );
	
};

#endif /*BOX_H_*/
