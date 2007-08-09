#include "../include/objects/arc.h"

// default constructor
arc::arc() : 
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	this->ratio = 1.0f;
	this->divisions = 16;
	this->angle = 180.0f;
	this->setName("default_arc");
	this->physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	
}

// data constructor
arc::arc(string& data) :
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
	
	// define some basic values
	this->ratio = 1.0f;
	this->divisions = 16;
	this->angle = 180.0f;
	this->setName("default_arc");
	this->physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	
	this->update(data);	
}


// getter
string arc::get(void) { return this->toString(); }

// setter
int arc::update(string& data) {
	const char* header = this->getHeader().c_str();
	// get the chunk we need
	
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "arc"))
		return 0;
		
	const char* arcData = lines[0].c_str();
		
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, arcData);
	if(!hasOnlyOne(names, "name"))
		return 0;
		
	// get the angle
	vector<string> angles = BZWParser::getValuesByKey("angle", header, arcData);
	if(!hasOnlyOne(angles, "angle"))
		return 0;
	
	// get the divisions
	vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, arcData);
	if(!hasOnlyOne(vDivisions, "divisions"))
		return 0;
		
	// get the ratio
	vector<string> ratios = BZWParser::getValuesByKey("ratio", header, arcData);
	if(!hasOnlyOne(ratios, "ratio"))
		return 0;
			
	// get flatshading
	vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, arcData);
	
	// get smoothbounce
	vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, arcData);
	
	// do base class update
	if(!bz2object::update(data))
		return 0;
	
	// set the data
	this->setName( names[0] );
	this->angle = atof( angles[0].c_str() );
	this->divisions = atoi( vDivisions[0].c_str() );
	this->ratio = atof( ratios[0].c_str() );
	this->flatShading = (flatShadings.size() == 0 ? false : true);
	this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
	
	return 1;	
}

// toString
string arc::toString(void) {
	return string("arc\n") +
				  this->BZWLines() +
				  "  angle " + string(ftoa(this->angle)) + "\n" +
				  "  ratio " + string(ftoa(this->ratio)) + "\n" +
				  "  divisions " + string(itoa(this->divisions)) + "\n" +
				  (this->flatShading == true ? "  flatshading\n" : "") +
				  (this->smoothbounce == true ? "  smoothbounce\n" : "") + 
				  "end\n";	
}

// render
int arc::render(void) {
	return 0;	
}

