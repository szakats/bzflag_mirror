#ifndef UVN_H_
#define UVN_H_

#include "../Vector3D.h"
#include <math.h>

/**
 * This class stores a U-V-N vector orientation, which
 * stores a "forward" vector, a "right" vector, and an "up" vector,
 * which are all perpendicular to one another.
 * 
 * This class also provides an Euler-style API for manipulating
 * the vectors (i.e. rotation by a given angle about one of the vectors), where
 * rotation around the x axis rotates the "up" and "look" vectors around the "right", 
 * rotation around the y axis rotates the "look" and "right" vectors around the "up", and
 * rotation around the z axis rotates the "right" and "up" vectors around the "look".
 * 
 * In this class, angles are meausured in degrees, not radians.
 */

class UVN {

public:

	// default constructor
	UVN();
	
	// build a UVN from elements of a "look" vector
	UVN(float look_x, float look_y, float look_z);
	
	// build a UVN from a given "look" vector
	UVN(Vector3D& lookVector);
	
	// getters
	Vector3D& getLookVector();
	Vector3D& getRightVector();
	Vector3D& getUpVector();
	Vector3D& getAngularOrientation();
	
	// manipulation (Euler-style)
	void rotate_x(float angle);
	void rotate_y(float angle);
	void rotate_z(float angle);

	// manipulation (UVN-style)
	void look_along(float look_x, float look_y, float look_z);
	void look_along(Vector3D& lookVector);
	void rotate_look(float angle);		// calls rotate_z
	void rotate_right(float angle);		// calls rotate_x
	void rotate_up(float angle);		// calls rotate_y
	
	
protected:
	// the three original vectors
	Vector3D lookVector;
	Vector3D rightVector;
	Vector3D upVector;
	
	// working copies of the vectors
	Vector3D work_lookVector;
	Vector3D work_rightVector;
	Vector3D work_upVector;
	
	// stores data for Euler-style manipulation
	Vector3D angularOrientation;
	
	// low-level manipulation of work_* vectors
	void work_rotate_x(float a);
	void work_rotate_y(float a);
	void work_rotate_z(float a);
};

#endif /*UVN_H_*/
