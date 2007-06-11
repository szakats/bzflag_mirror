#ifndef CONE_H_
#define CONE_H_

#include "bz2object.h"

class cone : public bz2object {
public:
	cone() : 
		bz2object("cone", "<position><rotation><size><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
		
		// define some basic values
		this->divisions = 16;
		this->name = string("default_cone");
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
	}
	
	cone(string& data) :
		bz2object("cone", "<position><rotation><size><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
		
		// define some basic values
		this->divisions = 16;
		this->name = string("default_cone");
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		// get the chunk we need
		
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// check and see if the proper data segment was found
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		if(!hasOnlyOne(lines, "cone"))
			return 0;
			
		const char* coneData = lines[0].c_str();
			
		// get the name
		vector<string> names = BZWParser::getValuesByKey("name", header, coneData);
		if(!hasOnlyOne(names, "name"))
			return 0;
			
		// get the divisions
		vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, coneData);
		if(!hasOnlyOne(vDivisions, "divisions"))
			return 0;
			
		// get flatshading
		vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, coneData);
		
		// get smoothbounce
		vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, coneData);
		
		// do base class update
		if(!bz2object::update(data))
			return 0;
		
		// set the data
		this->name = names[0];
		this->divisions = atoi( vDivisions[0].c_str() );
		this->flatShading = (flatShadings.size() == 0 ? false : true);
		this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
		
		return 1;
	}
	
	// toString
	string toString(void) {
		return string("cone\n") +
					  this->BZWLines() +
					  "  divisions " + string(itoa(divisions)) + "\n" +
					  (flatShading == true ? "  flatshading\n" : "") +
					  (smoothbounce == true ? "  smoothbounce\n" : "") + 
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	bool flatShading, smoothbounce;
	int divisions;
	
};

#endif /*CONE_H_*/
