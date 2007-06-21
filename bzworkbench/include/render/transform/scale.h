#ifndef SCALE_H_
#define SCALE_H_

#include "TransOp.h"

// scale transform operator
class scale : public TransOp {

public:

	// constructor
	scale(float x, float y, float z);
	
	// transform operation
	int apply(Point3D* mesh, int numElements);
	
private:
	
	// data
	float x, y, z;
};

#endif /*SCALE_H_*/
