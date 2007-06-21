#include "../../include/render/transform/shear.h"

// constructor
shear::shear(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;	
}

// operator
int shear::apply(Point3D* points, int numPoints) {
	
	return 1;
}
