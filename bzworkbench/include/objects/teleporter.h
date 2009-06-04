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

#ifndef TELEPORTER_H_
#define TELEPORTER_H_

#include "bz2object.h"
#include "model/SceneBuilder.h"
#include "render/Point3D.h"

#include <string>
#include <vector>

class teleporter : public bz2object {
public:

	// constructor
	teleporter();

	// constructor with data
	teleporter(string& data);

	static DataEntry* init() {
		return new teleporter();
	}
	static DataEntry* init(std::string& data) { return new teleporter(data); }

	// restore default values
	void setDefaults();

	// getter
	std::string get(void);

	// setter
	int update(std::string& data);
	int update(UpdateMessage& message);

	// override the getSize and setSize methods
	osg::Vec3 getSize() { return osg::Vec3( realSize.x(), realSize.y(), realSize.z() ); }
	void setSize( osg::Vec3 newSize );

	// tostring
	string toString(void);

	// binary getters and setters
	float getBorder() { return border; }

	void setBorder( float newBorder );

private:

	float border;

	// the real size (not reflected in the node geometry)
	Point3D realSize;

	void updateGeometry();

	// rebuild the UVs for a teleporter
	void updateTeleporterUV( osg::Group* tele, osg::Vec3 size,
									 const float borderSize );

	// rebuild the mesh for a teleporter
	void updateTeleporterMesh( osg::Group* tele, osg::Vec3 size,
									   const float borderSize );

};
#endif /*TELEPORTER_H_*/
