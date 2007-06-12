#include "../include/objects/pyramid.h"

pyramid::pyramid() :
		bz2object("pyramid", "<position><rotation><size>") { }
	
pyramid::pyramid(string& data) :
	bz2object("pyramid", "<position><rotation><size>", data.c_str()) { }
	
// getter
string pyramid::get() { return this->toString(); }

// setter
int pyramid::update(string& data) {
	return bz2object::update(data);	
}

// toString
string pyramid::toString(void) {
	return string("pyramid\n") +
				  this->BZWLines() +
				  "end\n";	
}

// render
int pyramid::render(void) {
	return 0;
}
