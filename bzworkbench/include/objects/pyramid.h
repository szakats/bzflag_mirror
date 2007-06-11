#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "bz2object.h"
#include "../BZWParser.h"
#include <string>
#include <vector>

using namespace std;

/**
 * The 1.x pyramid object.
 */
 
class pyramid : public bz2object {
public:
	
	pyramid() :
		bz2object("pyramid", "<position><rotation><size>") { }
		
	pyramid(string& data) :
		bz2object("pyramid", "<position><rotation><size>", data.c_str()) { }
		
	// getter
	string get() { return this->toString(); }
	
	// setter
	int update(string& data) {
		return bz2object::update(data);	
	}
	
	// toString
	string toString(void) {
		return string("pyramid\n") +
					  this->BZWLines() +
					  "end\n";	
	}
	
	// render
	int render(void) {
		return 0;
	}
};

#endif /*PYRAMID_H_*/
