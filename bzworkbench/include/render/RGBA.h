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

#ifndef RGBA_H_
#define RGBA_H_

#include "model/BZWParser.h"
#include "ftoa.h"

#include <string>
#include <vector>

#include <osg/Vec4>
#include <osg/Vec4d>
#include <osg/Vec4f>

using namespace std;

// simple RGBA construct
class RGBA : public osg::Vec4 {
public:
	
	RGBA() : osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) { }
	
	RGBA(float _r, float _g, float _b, float _a) : osg::Vec4( _r, _g, _b, _a ) { }
	
	RGBA( const osg::Vec4d& vec ) : osg::Vec4( vec ) { }
	
	RGBA(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		float _r, _g, _b, _a;
		// only initialize from the string if there are at least 4 elements
		if(points.size() >= 4) {
			_r = atof( points[0].c_str() );
			_g = atof( points[1].c_str() );
			_b = atof( points[2].c_str() );
			_a = atof( points[3].c_str() );
		}
		else {
			_r = _g = _b = _a = 0;	
		}
		
		this->set( _r, _g, _b, _a );
	}
	
	string toString(void) {
		return string(ftoa(r())) + " " + string(ftoa(g())) + " " + string(ftoa(b())) + " " + string(ftoa(a())) + "\n";
	}
};
#endif /*RGBA_H_*/
