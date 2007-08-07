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
	
	static DataEntry* init() { return new physics(); }
	static DataEntry* init(string& data) { return new physics(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	// getters
	string& getName() { return this->name; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	
private:
	
	Point3D linear, angular;
	float slide;
	string name, deathMessage;
};

#endif /*PHYSICS_H_*/
