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
	
	static DataEntry* init() { return new base(); }
	static DataEntry* init(string& data) { return new base(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString();
	
	// render
	int render(void);
	
	// BZWB-specific getters and setters
	int getTeam() { return this->team; }
	string& getWeapon() { return this->weapon; }
	
	void setTeam( int team ) { this->team = team; }
	void setWeapon( const char* weapon ) { this->weapon = weapon; }
	
private:
	int team;
	string weapon;
};

#endif /*BASE_H_*/
