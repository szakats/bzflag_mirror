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

#ifndef CONE_H_
#define CONE_H_

#include "objects/bz2object.h"
#include "model/SceneBuilder.h"
#include "render/Point2D.h"

#include <math.h>

class cone : public bz2object {
public:
	enum {
		Edge,
		Bottom,
		StartFace,
		EndFace,
		MaterialCount
	};

	// default constructor
	cone();
	
	// constructor with data
	cone(string& data);
	
	static DataEntry* init() { return new cone(); }
	static DataEntry* init(string& data) { return new cone(data); }
	
	virtual void setDefaults();

	// getter
	string get(void);
	
	// setter
	int update(string& data);
	int update(UpdateMessage& msg);
	
	// toString
	string toString(void);
	
	// getters/setters
	bool hasFlatShading() { return flatShading; }
	bool hasSmoothBounce() { return smoothbounce; }
	float getSweepAngle() { return sweepAngle; }
	int getDivisions() { return divisions; }
	
	void setFlatShading(bool value);
	
	void setSmoothBounce(bool value) { smoothbounce = value; }
	
	void setSweepAngle(float value);
	
	void setDivisions(int value);
	
protected:
	static const char* sideNames[MaterialCount];

	bool flatShading;
	bool smoothbounce;
	bool flipz;
	int divisions;
	bool pyramidStyle;
	
	// sweep angle
	float sweepAngle;

	Point2D texsize;
	
	// helper method to build the geometry
	void buildGeometry();
	
	// helper method to change the node's shading (flat-shaded or smooth-shaded)
	void updateShadeModel();
	
};

#endif /*CONE_H_*/
