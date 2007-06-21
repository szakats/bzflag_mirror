#ifndef TRANSOP_H_
#define TRANSOP_H_

#include <string>

#include "../Point3D.h"

/**
 * This is an abstract transformation operator class.
 * It stores a transformation algorithm that operates on a mesh
 */
 
class TransOp {
	
public:
	
	// constructors
	TransOp() { name = ""; }
	TransOp(const char* name) { this->name = name; }
	TransOp(string& name) { this->name = name; }
	
	// destructor
	virtual ~TransOp() { }
	
	// operator algorithm
	virtual int apply(Point3D* mesh, int numElements) { return 0; }
	
	// get name
	const char* getName() { return name.c_str(); }
	
	// comparison
	virtual bool operator ==(TransOp& top) {
		return (top.getName() == this->name);
	}
	
private:
	string name;
};

#endif /*TRANSOP_H_*/
