#ifndef TELEPORTER_H_
#define TELEPORTER_H_

#include "bz2object.h"

#include <string>
#include <vector>

using namespace std;

class teleporter : public bz2object {
public:
	
	// constructor
	teleporter();
	
	// constructor with data
	teleporter(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// render
	int render(void);
	
private:
	
	float border;
	string lname;	// used for linkage; not the official name
};

#endif /*TELEPORTER_H_*/
