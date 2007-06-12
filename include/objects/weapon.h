#ifndef WEAPON_H_
#define WEAPON_H_

#include "bz2object.h"
#include "../flags.h"


class weapon : public bz2object {

public:
	
	// default constructor
	weapon();
	
	// constructor with data
	weapon(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	string type, trigger, eventTeam;
	float initdelay, tilt;
	vector<float> delay;
	int team;
};

#endif /*WEAPON_H_*/
