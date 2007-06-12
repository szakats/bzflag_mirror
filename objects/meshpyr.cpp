#include "../include/objects/meshpyr.h"

// default constructor
meshpyr::meshpyr() : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>") { }

// constructor with data
meshpyr::meshpyr(string& data) : bz2object("meshpyr", "<position><rotation><size><matref><phydrv>", data.c_str()) { this->update(data); }

// getter
string meshpyr::get(void) { return this->toString(); }

// setter
int meshpyr::update(string& data) {
	return bz2object::update(data);	
}

// tostring
string meshpyr::toString(void) {
	return string("meshpyr\n") +
				  this->BZWLines() +
				  "end\n";
}

// render
int meshpyr::render(void) {
	return 0;	
}
