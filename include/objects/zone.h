#ifndef ZONE_H_
#define ZONE_H_

#include "bz2object.h"

/**
 * Zone object
 */

class zone : public bz2object {
	
public:

	/*
	 * Sub-class that contains a flag/quantity pair
	 */
	 
	class FlagElement {
	public:
	
		// simple construtor
		FlagElement(string flag, int qty) {
			this->flag = flag;
			this->qty = qty;
		}
		
		// self-parser toString() method
		string toString(void) {
			return flag + " " + string(itoa(qty));
		}
		
		// self-reader update() method
		void update(string data) {
			string fl = BZWParser::key(data.c_str());
			string n = BZWParser::value(fl.c_str(), data.c_str());
			
			// quit if there is no flag
			if(fl == BZW_NOT_FOUND)	
				return;
				
			// quit if there was no quantity
			if(n == BZW_NOT_FOUND)
				return;
				
			// load the data
			flag = fl;
			qty = atoi( n.c_str() );
		}
		
		string flag;
		int qty;
	};
	
	// constructor
	zone() : bz2object("zone", "<name><position><size><rotation><team><flag><safety><zoneflag>") {
		teams = vector<int>();
		safety = vector<int>();
	}
	
	// constructor with data
	zone(string& data) : bz2object("zone", "<name><position><size><rotation><team><flag><safety><zoneflag>", data.c_str()) {
		teams = vector<int>();
		safety = vector<int>();
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the sections
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// return if none were found
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		// return if more than one were found
		if(!hasOnlyOne(lines, header))
			return 0;
			
		// get the data
		const char* zoneData = lines[0].c_str();
		
		// get the team values
		vector<string> teams = BZWParser::getValuesByKey("team", header, zoneData);
		if(!hasOnlyOne(teams, "team"))
			return 0;
			
		// get the safety values
		vector<string> safties = BZWParser::getValuesByKey("safety", header, zoneData);
		if(!hasOnlyOne(safties, "safety"))
			return 0;
			
		// get the flag values
		vector<string> flagVals = BZWParser::getValuesByKey("flag", header, zoneData);
		if(!hasOnlyOne(flagVals, "flag"))
			return 0;
		
		// get the zoneflag values (multiple values possible)
		vector<string> zoneFlagVals = BZWParser::getValuesByKey("zoneflag", header, zoneData);
			
		// read in the given teams
		vector<string> teamElements = BZWParser::getLineElements( teams[0].c_str() );
		
		// read in the flags
		vector<string> flagElements = BZWParser::getLineElements(flagVals[0].c_str());
		
		// read the safeties
		vector<string> safetyElements = BZWParser::getLineElements(safties[0].c_str());
		
		// determine whether or not the given flags are valid (throw a warning if its invalid)
		for(vector<string>::iterator i = flagElements.begin(); i != flagElements.end(); i++) {
			if(	! (isFlag( i->c_str() )  || (*i) == "good" || (*i) == "bad") ) {
				printf("zone::update(): Warning! Unrecognized flag type \"%s\"\n", i->c_str());
			}
		}
		
		// candidate teams
		vector<int> teamCandidates = vector<int>();
		
		// determine whether or not the given teams are valid (throw a warning if invalid)
		for(vector<string>::iterator i = teamElements.begin(); i != teamElements.end(); i++) {
			int teamNumber = atoi( i->c_str() );
			if(teamNumber < 0 || teamNumber > 4) {
				printf("zone::update(): Warning! Undefined team \"%d\" in \"team\"\n", teamNumber);	
			}
			teamCandidates.push_back(teamNumber);
		}
		
		// candidate safeties
		vector<int> safetyCandidates = vector<int>();
		
		// determine whether or not the given safety values are valid (throw a warning if not)
		for(vector<string>::iterator i = safetyElements.begin(); i != safetyElements.end(); i++) {
			int teamNumber = atoi( i->c_str() );
			if(teamNumber < 0 || teamNumber > 4) {
				printf("zone::update(): Warning! Undefined team \"%d\" in \"safety\"\n", teamNumber);	
			}
			safetyCandidates.push_back(teamNumber);
		}
		
		// candidate flag/quantity pairs
		vector<FlagElement> zoneflagCandidates = vector<FlagElement>();
		
		// parse the zoneflag values, and make sure they're valid
		for(vector<string>::iterator i = zoneFlagVals.begin(); i != zoneFlagVals.end(); i++) {
			// read the flag
			string flag = BZWParser::key(i->c_str());
			// see if its valid
			if(!isFlag(flag.c_str())) {
				printf("zone::update(): Warning! Unrecognized flag type \"%s\"\n", flag.c_str());
			}
			// try to read the quantity
			string value = BZWParser::value(flag.c_str(), i->c_str());
			// try to atoi() the value
			int num = atoi( value.c_str() );
			// num will be zero if atoi() failed...
			if(num == 0) {
				printf("zone::update(): Warning! Unrecognized flag quantity \"%s\"\n", value.c_str());	
			}
			
			zoneflagCandidates.push_back(FlagElement(flag, num));
		}
		
		// fill in the data
		if(!bz2object::update(data))
			return 0;
		
		this->teams = teamCandidates;
		this->safety = safetyCandidates;
		this->zoneflags = zoneflagCandidates;
		this->flags = flagElements;
		
		return 1;
	}
	
	// toString
	string toString(void) {
		// string-ify the teams, safeties, zoneflags, and flags
		string teamString = string("");
		string safetyString = string("");
		string zoneflagString = string("");
		string flagString = string("");
		
		// make the "safety" string
		if(safety.size() > 0) {
			for(vector<int>::iterator index = this->safety.begin(); index != this->safety.end(); index++) {
				safetyString += string(itoa(*index)) + " ";
			}
			safetyString += "\n";
		}
		
		if(teams.size() > 0) {
			// make the "team" string
			for(vector<int>::iterator index = this->teams.begin(); index != this->teams.end(); index++) {
				teamString += string(itoa(*index)) + " ";
			}
			teamString += "\n";
		}
		
		if(flags.size() > 0) {
			// make the "flag" string
			for(vector<string>::iterator index = this->flags.begin(); index != this->flags.end(); index++) {
				flagString += (*index) + " ";
			}
			flagString += "\n";
		}
		
		if(zoneflags.size() > 0) {
			// make the "zoneflag" strings
			for(vector<FlagElement>::iterator index = this->zoneflags.begin(); index != this->zoneflags.end(); index++) {
				zoneflagString += "  zoneflag " + index->toString() + "\n";	
			}
		}
		
		return string("zone\n") +
					  this->BZWLines() +
					  (safetyString.length() == 0 ? "# safety\n" : "  safety " + safetyString) +
					  (teamString.length() == 0 ? "# team\n" : "  team " + teamString) +
					  (flagString.length() == 0 ? "# flag\n" : "  flag " + flagString) +
					  (zoneflagString.length() == 0 ? "# zoneflag\n" : zoneflagString) +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}

private:
	vector<int> teams;
	vector<int> safety;
	vector<FlagElement> zoneflags;
	vector<string> flags;
	
	/*
	 * Helper method:  determines whether or not flag is valid
	 */
	 bool isFlag(const char* flag) {
	 	// compare against team flags
	 	return (isTeamFlag(flag) || isGoodFlag(flag) || isBadFlag(flag));
	 }
	 
	 /*
	  * Helper method:  determines whether or not a flag is a team flag
	  */
	 bool isTeamFlag(const char* flag) {
	 	for(int i = 0; i < NUM_TEAMFLAGS; i++) {
	 		if(_teamFlags[i] == flag)
	 			return true;	
	 	}
	 	return false;
	 }
	 
	 /*
	  * Helper method: determines whether or not a flag is a good flag
	  */
	 
	 bool isGoodFlag(const char* flag) {
	 	for(int i = 0; i < NUM_GOODFLAGS; i++) {
	 		if(_goodFlags[i] == flag)
	 			return true;	
	 	}
	 	return false;
	 }
	 
	 /*
	  * Helper method: determines whether or not a flag is a bad flag
	  */
	 bool isBadFlag(const char* flag) {
	 	for(int i = 0; i < NUM_BADFLAGS; i++) {
	 		if(_badFlags[i] == flag)
	 			return true;	
	 	}	
	 	return false;
	 }
	
};

#endif /*ZONE_H_*/
