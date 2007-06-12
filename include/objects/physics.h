#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../model/BZWParser.h"

class physics : public DataEntry {

public:

	// default constructor
	physics();
	
	// constructor with data
	physics(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	
	Point3D linear, angular;
	float slide;
	string name, deathMessage;
};

#endif /*PHYSICS_H_*/
