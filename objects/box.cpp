#include "../include/objects/box.h"

// constructors
box::box() : bz2object("box", "<position><rotation><size>") {
	
}

box::box(string& data) : bz2object("box", "<position><rotation><size>", data.c_str()) {
	this->update(data);	
}

// nothing to destroy...
box::~box() { }

// getter
string box::get(void) {
	return this->toString();
}

// setter
int box::update(string& data) {
	return bz2object::update(data);		
}

// toString
string box::toString(void) {
	return string("box\n") +
				  this->BZWLines() +
				  "end\n";
}

// render
int box::render(void) {
	return 0;
}
