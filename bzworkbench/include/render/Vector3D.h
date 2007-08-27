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

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include "Point3D.h"
#include <math.h>

class Vector3D : public Point3D
{
	public:
	
	// constructors
	Vector3D() : Point3D() { }
	
	Vector3D(float x, float y, float z) : Point3D(x, y, z) { }
	
	Vector3D(double x, double y, double z) : Point3D((float)x, (float)y, (float)z) { }
	
	Vector3D(const char* data) : Point3D(data) { }
	
	virtual ~Vector3D() { }
	
	// methods
	static void crossProduct(Vector3D& result, Vector3D& u, Vector3D& v) {
		result.set( u.y()*v.z() - u.z()*v.y(),
				    u.z()*v.x() - u.x()*v.z(),
					u.x()*v.y() - u.y()*v.x());
	}
	
	static double dotProduct(Vector3D u, Vector3D v) {
		
		return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
	}
	
	double magnitude() {
		return this->length();
	}
	
};

#endif /*VECTOR3D_H_*/