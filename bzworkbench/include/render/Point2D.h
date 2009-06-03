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

#ifndef POINT2D_H_
#define POINT2D_H_

#include <string>
#include <vector>
#include "model/BZWParser.h"
#include "ftoa.h"

#include <osg/Vec2d>
#include <osg/Vec2f>
#include <osg/Vec2>

class Point2D : public osg::Vec2
{
	public:
	
	Point2D() : osg::Vec2( 0, 0 ) { }
	
	Point2D(float _x, float _y) : osg::Vec2( _x, _y ) { }
	
	Point2D(double _x, double _y) : osg::Vec2( _x, _y ) { }
	
	Point2D( osg::Vec2 pt ) : osg::Vec2( pt ) { }
	
	Point2D(const char* description) {
	  std::vector<string> points = BZWParser::getLineElements(description);
	  
	  // only initialize from the string if there are at least 2 elements
	  if(points.size() >= 2) {
			set( atof( points[0].c_str() ),
			     atof( points[1].c_str() ));
	  } else {
	    set(0,0);
	  }
	}
	
	Point2D( std::string desc ) { Point2D( desc.c_str() ); }
	
	string toString(void) {
	  return std::string("") + std::string(ftoa(x())) + " " + std::string(ftoa(y())) + "\n";
	}
	
	virtual ~Point2D() { }
	
	// convert to an osg::Vec2
	osg::Vec2 asVec2() { return osg::Vec2( x(), y() ); }
};

#endif /*POINT3D_H_*/
