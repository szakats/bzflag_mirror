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

#ifndef ARC_H_
#define ARC_H_

#include "bz2object.h"
#include "model/BZWParser.h"
#include <string>
#include <vector>

using namespace std;

class arc : public bz2object {

public:

	arc();
	
	arc(string& data);
	
	static DataEntry* init() { return new arc(); }
	static DataEntry* init(string& data) { return new arc(data); }
	
	void setDefault();

	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);

	void setSize( const osg::Vec3d& newSize);

private:
	bool flatShading, smoothbounce;
	float angle, ratio;
	int divisions;
	osg::Vec4 texsize;

	void updateGeometry();

	void makePie(osg::Geometry* sideMesh, osg::Geometry* topbotMesh, bool isCircle, float a, float r,
				   float h, float radius, float squish,
				   osg::Vec4& texsz);
	void makeRing(osg::Geometry* sideMesh, osg::Geometry* topbotMesh, bool isCircle, float a, float r,
				    float h, float inrad, float outrad,
					float squish, osg::Vec4& texsz);
};

#endif /*ARC_H_*/
