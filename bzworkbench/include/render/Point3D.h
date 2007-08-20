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

#ifndef POINT3D_H_
#define POINT3D_H_

#include <string>
#include <vector>
#include "../model/BZWParser.h"
#include "../ftoa.h"

#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec3>

class Point3D : public osg::Vec3
{
	public:
	
	Point3D() : osg::Vec3( 0, 0, 0 ) { }
	
	Point3D(float x, float y, float z) : osg::Vec3( x, y, z ) { }
	
	Point3D(double x, double y, double z) : osg::Vec3( x, y, z ) { }
	
	Point3D( osg::Vec3 pt ) : osg::Vec3( pt ) { }
	
	Point3D(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		
		// only initialize from the string if there are at least 3 elements
		if(points.size() >= 3) {
			this->set( atof( points[0].c_str() ),
					   atof( points[1].c_str() ),
					   atof( points[2].c_str() ));
		}
		else {
			this->set(0,0,0);
		}
	}
	
	Point3D( string desc ) { Point3D( desc.c_str() ); }
	
	string toString(void) {
		return string("") + string(ftoa(this->x())) + " " + string(ftoa(this->y())) + " " + string(ftoa(this->z())) + "\n";
	}
	
	virtual ~Point3D() { }
	
	// convert to an osg::Vec3
	osg::Vec3 asVec3() { return osg::Vec3( this->x(), this->y(), this->z() ); }
	
};

#endif /*POINT3D_H_*/
