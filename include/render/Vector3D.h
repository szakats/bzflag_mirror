#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include "Point3D.h"
#include <math.h>

class Vector3D : public Point3D
{
	public:
	
	// constructors
	Vector3D() : Point3D() {
	}
	
	Vector3D(float x, float y, float z) : Point3D(x, y, z) {
		
	}
	
	Vector3D(double x, double y, double z) : Point3D((float)x, (float)y, (float)z) {
		
	}
	
	Vector3D(const char* data) : Point3D(data) {
		
	}
	
	virtual ~Vector3D() { }
	
	// methods
	static void crossProduct(Vector3D& result, Vector3D& u, Vector3D& v) {
		result.x = u.y*v.z - u.z*v.y;
		result.y = u.z*v.x - u.x*v.z;
		result.z = u.x*v.y - u.y*v.x;
	}
	
	static double dotProduct(Vector3D u, Vector3D v) {
		
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}
	
	double magnitude() {
		return sqrt(x*x + y*y + z*z);
	}
	
	void normalize() {
		double len = this->magnitude();
		if(len == 0.0) return;
		
		x /= len;
		y /= len;
		z /= len;
	}
	
	void scale(double scaleFactor) {
		x *= scaleFactor;
		y *= scaleFactor;
		z *= scaleFactor;
	}
};

#endif /*VECTOR3D_H_*/
