#ifndef SHEAR_H_
#define SHEAR_H_

#include "TransOp.h"

// shear transformation operator
class shear : public TransOp {

public:	
	
	// constructor
	shear(float x, float y, float z);
	
	// operator
	int apply(Point3D* mesh, int numElements);
	
private:

	// data
	float x, y, z;
};

#endif /*SHEAR_H_*/
