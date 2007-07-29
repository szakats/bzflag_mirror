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
	
};

#endif /*POINT3D_H_*/
