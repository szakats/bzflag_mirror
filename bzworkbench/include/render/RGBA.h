#ifndef RGBA_H_
#define RGBA_H_

#include "../model/BZWParser.h"
#include "../ftoa.h"

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
	
	RGBA(float r, float g, float b, float a) : osg::Vec4( r, g, b, a ) { }
	
	RGBA( const osg::Vec4d& vec ) : osg::Vec4( vec ) { }
	
	RGBA(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		float r, g, b, a;
		// only initialize from the string if there are at least 4 elements
		if(points.size() >= 4) {
			r = atof( points[0].c_str() );
			g = atof( points[1].c_str() );
			b = atof( points[2].c_str() );
			a = atof( points[3].c_str() );
		}
		else {
			r = g = b = a = 0;	
		}
		
		this->set( r, g, b, a );
	}
	
	string toString(void) {
		return string(ftoa(this->r())) + " " + string(ftoa(this->g())) + " " + string(ftoa(this->b())) + " " + string(ftoa(this->a())) + "\n";
	}
};
#endif /*RGBA_H_*/
