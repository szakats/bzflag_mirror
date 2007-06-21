#include "../../include/render/transform/spin.h"

// constructor
spin::spin(float angle, float nx, float ny, float nz) {
	
	this->angle = angle;
	
	Vector3D tmp(nx, ny, nz);
	tmp.normalize();
	
	this->nx = tmp.x;
	this->ny = tmp.y;
	this->nz = tmp.z;
}

// transform operator
int spin::apply(Point3D* points, int numPoints) {
	
	return 1;	
}
