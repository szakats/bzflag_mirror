#ifndef BASE_H_
#define BASE_H_

#include "bz2object.h"
#include "../BZWParser.h"
#include "../defines.h"

#include <string>
#include <vector>

using namespace std;

class base : public bz2object {
public:
	
	// constructor
	base() :
		bz2object("base", "<position><rotation><size><color><oncap>") {
		
		team = 0;
		weapon = string("SW");		
	}
	
	// constructor with data
	base(string& data) :
		bz2object("base", "<position><rotation><size><color><oncap>", data.c_str()) {
			
		weapon = string("SW");
		this->team = 0;
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		// get the header
		const char* header = this->getHeader().c_str();
		
		// get the section
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
		
		if(!hasOnlyOne(lines, "base"))
			return 0;
			
		// get the section data
		const char* baseData = lines[0].c_str();
		
		// get the team
		vector<string> teams = BZWParser::getValuesByKey("color", header, baseData);
		if(!hasOnlyOne(teams, "color"))
			return 0;
			
		// get the weapon
		vector<string> weapons = BZWParser::getValuesByKey("oncap", header, baseData);
		if(weapons.size() > 1) {
			printf("base::update():  Error! Defined \"oncap\" %d times!\n", weapons.size());
			return 0;
		}
			
		// make sure that the team value is sane
		int t = atoi(teams[0].c_str());
		if(!(t == BASE_RED || t == BASE_GREEN || t == BASE_BLUE || t == BASE_PURPLE)) {
			printf("base::update():  Warning! Base team is not recognized...\n");	
		}
		
		// do superclass update
		if(!bz2object::update(data))
			return 0;
		
		// load in the data
		this->team = t;
		this->weapon = (weapons.size() > 0 ? weapons[0] : string(""));
		
		return 1;
	}
	
	// tostring
	string toString() {
		return string("base\n") +
					  this->BZWLines() +
					  "  color " + string(itoa(team)) + "\n" +
					  (weapon.length() != 0 ? "  oncap " + weapon + "\n" : "") +
					  "end\n";	
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	int team;
	string weapon;
};

#endif /*BASE_H_*/
