#include "../../include/render/transform/scale.h"

// constructor
scale::scale(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;	
}

// operation
int scale::apply(Point3D* points, int numPoints) {
	for(int i = 0; i < numPoints; i++) {
		points[i].x *= this->x;
		points[i].y *= this->y;
		points[i].z *= this->z;	
	}	
	
	return 1;
}

