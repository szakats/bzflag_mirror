#ifndef ARC_H_
#define ARC_H_

#include "bz2object.h"
#include "../BZWParser.h"
#include <string>
#include <vector>

using namespace std;

class arc : public bz2object {

public:

	arc() : 
		bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
		
		// define some basic values
		this->ratio = 1.0f;
		this->divisions = 16;
		this->angle = 180.0f;
		this->name = string("default_arc");
		this->materials = vector<string>();
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
	}
	
	arc(string& data) :
		bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
		
		// define some basic values
		this->ratio = 1.0f;
		this->divisions = 16;
		this->angle = 180.0f;
		this->name = string("default_arc");
		this->materials = vector<string>();
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	void update(string& data) {
		const char* header = this->getHeader().c_str();
		// get the chunk we need
		
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// check and see if the proper data segment was found
		if(lines[0] == BZW_NOT_FOUND)
			return;
			
		const char* arcData = lines[0].c_str();
			
		// get the name
		vector<string> names = BZWParser::getValuesByKey("name", header, arcData);
		if(!hasOnlyOne(names, "name"))
			return;
			
		// get the angle
		vector<string> angles = BZWParser::getValuesByKey("angle", header, arcData);
		if(!hasOnlyOne(angles, "angle"))
			return;
		
		// get the divisions
		vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, arcData);
		if(!hasOnlyOne(vDivisions, "divisions"))
			return;
			
		// get the ratio
		vector<string> ratios = BZWParser::getValuesByKey("ratio", header, arcData);
		if(!hasOnlyOne(ratios, "ratio"))
			return;
				
		// get flatshading
		vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, arcData);
		
		// get smoothbounce
		vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, arcData);
		
		// do base class update
		bz2object::update(data);
		
		// set the data
		this->name = names[0];
		this->angle = atof( angles[0].c_str() );
		this->divisions = atoi( vDivisions[0].c_str() );
		this->ratio = atof( ratios[0].c_str() );
		this->flatShading = (flatShadings.size() == 0 ? false : true);
		this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
	}
	
	// toString
	string toString(void) {
		return string("arc\n") +
					  this->BZWLines() +
					  "  angle " + string(ftoa(angle)) + "\n" +
					  "  ratio " + string(ftoa(ratio)) + "\n" +
					  "  divisions " + string(ftoa((float)(divisions))) + "\n" +
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
	float angle, ratio;
	int divisions;
	
};

#endif /*ARC_H_*/
