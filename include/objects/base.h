#ifndef BASE_H_
#define BASE_H_

#include "bz2object.h"
#include "../model/BZWParser.h"
#include "../defines.h"

#include <string>
#include <vector>

using namespace std;

class base : public bz2object {
public:
	
	// constructor
	base();
	
	// constructor with data
	base(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString();
	
	// render
	int render(void);
	
private:
	int team;
	string weapon;
};

#endif /*BASE_H_*/
