#ifndef TELEPORTER_H_
#define TELEPORTER_H_

#include "bz2object.h"
#include "../BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class teleporter : public bz2object {
public:
	
	// constructor
	teleporter() :
		bz2object("teleporter", "<position><size><rotation><name><border>") {
		border = 1.12;
	}
	
	// constructor with data
	teleporter(string& data) :
		bz2object("teleporter", "<position><size><rotation><name><border>", data.c_str()) {
		
		this->update(data);
	}
	
	// getter
	string get(void) {  return this->toString(); }
	
	// setter
	int update(string& data) {
		// get header
		const char* header = this->getHeader().c_str();
		
		// get the section
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		if(!hasOnlyOne(lines, "teleporter"))
			return 0;
			
		// get the data
		const char* teleporterData = lines[0].c_str();
		
		// get the border
		vector<string> borders = BZWParser::getValuesByKey("border", header, teleporterData);
		if(!hasOnlyOne(borders, "border")) {
			return 0;
		}
		
		if(!bz2object::update(data))
			return 0;
		
		// set the data
		this->border = atof( borders[0].c_str() );
		
		return 1;
	}
	
	// tostring
	string toString(void) {
		return string("teleporter\n") +
					  this->BZWLines() +
					  "  border " + string(ftoa(border)) + "\n" +
					  "end\n";	
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	
	float border;
};

#endif /*TELEPORTER_H_*/
