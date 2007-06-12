#ifndef POINT3D_H_
#define POINT3D_H_

#include <string>
#include <vector>
#include "../model/BZWParser.h"
#include "../ftoa.h"

class Point3D
{
	public:
	float x, y, z;
	
	Point3D() {
		this->x = 0; this->y = 0; this->z = 0;	
	}
	
	Point3D(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}
	
	Point3D(double x, double y, double z) {
		this->x = (float)x; this->y = (float)y; this->z = (float)z;
	}
	
	Point3D(const char* description) {
		vector<string> points = BZWParser::getLineElements(description);
		
		// only initialize from the string if there are at least 3 elements
		if(points.size() >= 3) {
			this->x = atof( points[0].c_str() );
			this->y = atof( points[1].c_str() );
			this->z = atof( points[2].c_str() );
		}
		else {
			this->x = this->y = this->z = 0;	
		}
	}
	
	string toString(void) {
		return string("") + string(ftoa(this->x)) + " " + string(ftoa(this->y)) + " " + string(ftoa(this->z)) + "\n";
	}
	
	virtual ~Point3D() { }
	
	int compareTo(Point3D p) {
		return (p.x - x + p.y - y + p.z - z != 0.0) ? 1 : 0;
	}
};

#endif /*POINT3D_H_*/
