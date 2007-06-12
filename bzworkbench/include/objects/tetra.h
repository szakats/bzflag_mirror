#ifndef TETRAHEDRON_H_
#define TETRAHEDRON_H_

#include "bz2object.h"

class tetra : public bz2object {
public:
	
	// no arg constructor
	tetra();
	
	// data constructor
	tetra(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// render
	int render(void);
	
private:
	Point3D vertexes[4];
};


#endif /*TETRAHEDRON_H_*/
