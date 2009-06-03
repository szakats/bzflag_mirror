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

#ifndef POINT4D_H_
#define POINT4D_H_

#include <string>
#include <vector>
#include "model/BZWParser.h"
#include "ftoa.h"

#include <osg/Vec4d>
#include <osg/Vec4f>
#include <osg/Vec4>

class Point4D : public osg::Vec4
{
	public:
	
	Point4D() : osg::Vec4( 0, 0, 0, 0 ) { }
	
	Point4D(float _x, float _y, float _z, float _w) : osg::Vec4( _x, _y, _z, _w ) { }
	
	Point4D(double _x, double _y, double _z, double _w) : osg::Vec4( _x, _y, _z, _w ) { }
	
	Point4D( osg::Vec4 pt ) : osg::Vec4( pt ) { }
	
	Point4D(const char* description) {
	  std::vector<string> points = BZWParser::getLineElements(description);
	  
	  // only initialize from the string if there are at least 4 elements
	  if(points.size() >= 4) {
			set( atof( points[0].c_str() ),
			     atof( points[1].c_str() ),
			     atof( points[2].c_str() ),
				 atof( points[3].c_str() ));
	  } else {
	    set(0,0,0,0);
	  }
	}
	
	Point4D( std::string desc ) { Point4D( desc.c_str() ); }
	
	string toString(void) {
		return std::string("") + std::string(ftoa(x())) + " " + 
			std::string(ftoa(y())) + " " + std::string(ftoa(z())) + " " + 
			std::string(ftoa(w())) + "\n";
	}
	
	virtual ~Point4D() { }
	
	// convert to an osg::Vec4
	osg::Vec4 asVec4() { return osg::Vec4( x(), y(), z(), w() ); }
};

#endif /*POINT4D_H_*/