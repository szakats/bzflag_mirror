#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class waterLevel : public DataEntry {
public:

	// default constructor
	waterLevel();
	
	// constructor with data
	waterLevel(string& data);
	
	static DataEntry* init() { return new waterLevel(); }
	static DataEntry* init(string& data) { return new waterLevel(data); }
	
	// get method
	string get(void);
	
	// update method
	int update(string& data);
	
	// toString method
	string toString(void);
	
	// getters/setters
	string getName() { return name; }
	string getMaterial() { return material; }
	float getHeight() { return height; }
	
	void setName( string name ) { this->name = name; }
	void setMaterial( string material ) { this->material = material; }
	void setHeight( float height ) { this->height = height; }
	
	virtual ~waterLevel();
	
private:
	string name;
	string material;
	float height;
};

#endif /*WATERLEVEL_H_*/
