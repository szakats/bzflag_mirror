#include "../include/objects/teleporter.h"

// constructor
teleporter::teleporter() :
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	border = 1.12;
}

// constructor with data
teleporter::teleporter(string& data) :
	bz2object("teleporter", "<position><size><rotation><name><border>", data.c_str()) {
	
	this->update(data);
}

// getter
string teleporter::get(void) {  return this->toString(); }

// setter
int teleporter::update(string& data) {
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
	if(borders.size() > 1) {
		printf("teleporter::update(): Error! Defined \"border\" %d times!\n", borders.size());
	}	
	
	// get the linkage name
	vector<string> lnames = BZWParser::getValuesByKey("teleporter", header, data.c_str());
	
	if(!bz2object::update(data))
		return 0;
	
	// set the data
	this->border = (borders.size() != 0 ? atof( borders[0].c_str() ) : 0.0f);
	this->lname = (lnames.size() != 0 ? lnames[0] : "defaultLinkageName");
	
	return 1;
}

// tostring
string teleporter::toString(void) {
	return string("teleporter ") + lname + "\n" +
				  this->BZWLines() +
				  "  border " + string(ftoa(border)) + "\n" +
				  "end\n";	
}

// render
int teleporter::render(void) {
	return 0;	
}
