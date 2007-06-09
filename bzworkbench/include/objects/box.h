#ifndef BOX_H_
#define BOX_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../BZWParser.h"
#include "bz2object.h"

/**
 * Box data
 */
class box : public bz2object {

public:

	// constructors
	box() : bz2object("box", "<position><rotation><size>") {
		
	}
	
	box(string& data) : bz2object("box", "<position><rotation><size>", data) {
		this->update(data);	
	}
	
	// nothing to destroy...
	~box() { }
	
	// getter
	string get(void) {
		return this->toString();
	}
	
	// setter
	void update(string& data) {
		bz2object::update(data);		
	}
	
	// toString
	string toString(void) {
		return string("box\n") +
					  this->BZWLines() +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;
	}
	
private:
	Point3D size;
};

#endif /*BOX_H_*/
