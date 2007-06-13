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
	if(!hasOnlyOne(borders, "border")) {
		return 0;
	}
	
	// get the linkage name
	vector<string> linkageNames = BZWParser::getValuesByKey("teleporter", header, teleporterData);
	if(linkageNames[0] == "teleporter")	{	// name not found, so "teleporter" is returned instead
		printf("teleporter::update(): Warning! no linkage name defined!\n");
	}
	
	if(!bz2object::update(data))
		return 0;
	
	// set the data
	this->border = atof( borders[0].c_str() );
	this->lname = linkageNames[0];
	
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
