#ifndef ORIENTATION_H_
#define ORIENTATION_H_

#include "transform/UVN.h"
#include "transform/TransOp.h"

#include <list>

/**
 * Meta-data class that stores a renderable object's orientation data.
 * This would include position, scaling, shearing, shifting, and spinning.
 */

class Orientation : public UVN {

public:
	
	// constructors
	Orientation();
	Orientation(float x, float y, float z);
	Orientation(Point3D& position);
	
	// methods for manipulating the transformation operator stack
	void pushTransOp(TransOp& top);
	TransOp popTransOp();
	TransOp peekTransOp();
	TransOp peekTransOp(unsigned int index);
	bool insertTransOp(TransOp& top, unsigned int index);
	bool removeTransOp(unsigned int index);
	bool hasTransOps();
	
	// Orientation-specific API
	void translate(Vector3D& t);
	void translate(float x, float y, float z);
	
private:
	
	// position
	Point3D position;
	
	// tranformation operator stack
	vector<TransOp> transformations;
};

#endif /*ORIENTATION_H_*/
