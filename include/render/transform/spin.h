#ifndef SPIN_H_
#define SPIN_H_

#include "TransOp.h"
#include "../Vector3D.h"

// spin transformation operator
class spin : public TransOp {
	
public:
	
	// constructors
	spin(float angle, float nx, float ny, float nz);
	
	// transform operator
	int apply(Point3D* mesh, int numElements);
	
private:

	// data
	float angle, nx, ny, nz;
};

#endif /*SPIN_H_*/
