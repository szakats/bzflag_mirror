#ifndef SHIFT_H_
#define SHIFT_H_

#include "TransOp.h"

// shift transformation operator
class shift : public TransOp {

public:
	
	// default constructor
	shift(float x, float y, float z);
	
	// operation
	int apply(Point3D* mesh, int numElements);
	
private:

	// data
	float x, y, z;
};

#endif /*SHIFT_H_*/
