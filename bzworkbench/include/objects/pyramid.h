#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "bz2object.h"
#include <string>
#include <vector>

using namespace std;

/**
 * The 1.x pyramid object.
 */
 
class pyramid : public bz2object {
public:
	
	// default constructor
	pyramid();
		
	// constructor with data
	pyramid(string& data);
		
	// getter
	string get();
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
};

#endif /*PYRAMID_H_*/
