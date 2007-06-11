#ifndef MESHPYR_H_
#define MESHPYR_H_

#include "bz2object.h"

class meshpyr : public bz2object {
public:
	
	meshpyr() : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>") { }
	meshpyr(string& data) : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>", data.c_str()) { this->update(data); }
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		return bz2object::update(data);	
	}
	
	// tostring
	string toString(void) {
		return string("meshpyr\n") +
					  this->BZWLines() +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
};

#endif /*MESHPYR_H_*/
