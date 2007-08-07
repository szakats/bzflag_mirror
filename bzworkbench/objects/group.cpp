#include "../include/objects/group.h"

// constructor
group::group() : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>") {
	this->team = 0;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
}

// constructor with data
group::group(string& data) : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>", data.c_str()) {
	this->team = 0;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
	
	this->update(data);	
}

// getter
string group::get(void) {
	return this->toString(); 
}

// setter
int group::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the section from the data
	const vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "group"))
		return 0;
	
	const char* groupData = lines[0].c_str();
	
	// get name (from the first line)
	vector<string> headers = BZWParser::getValuesByKey("group", header, groupData);
	
	// get tint
	vector<string> tints = BZWParser::getValuesByKey("tint", header, groupData);
	if(tints.size() > 1) {
		printf("group::update(): Error! Defined \"tint\" %d times\n", tints.size());
		return 0;	
	}
	
	// get team
	vector<string> teams = BZWParser::getValuesByKey("team", header, groupData);
	if(teams.size() > 1) {
		printf("group::update(): Error! Defined \"team\" %d times\n", tints.size());
		return 0;	
	}
	
	// get drivethrough
	vector<string> driveThroughs = BZWParser::getValuesByKey("drivethrough", header, groupData);
		
	// get shootthrough
	vector<string> shootThroughs = BZWParser::getValuesByKey("shootthrough", header, groupData);
		
	// do base class update
	if(!bz2object::update(data))
		return 0;
	
	// assign data
	this->setName( headers[0] );
	this->tintColor = (tints.size() > 0 ? RGBA( tints[0].c_str() ) : RGBA(-1, -1, -1, -1));
	this->team = (teams.size() > 0 ? (int)(atof( teams[0].c_str() )) : -1);
	this->driveThrough = (driveThroughs.size() == 0 ? false : true);
	this->shootThrough = (shootThroughs.size() == 0 ? false : true);
	
	return 1;
	
}

// toString
string group::toString(void) {
	string tintString = string(""), teamString = string("");
	if(tintColor.r() > 0 && tintColor.g() > 0 && tintColor.b() > 0 && tintColor.a() > 0)
		tintString = "  tint " + tintColor.toString();
	if(team > 0)
		teamString = "  team " + string(itoa(team)) + "\n";
	
	return string("group ") + this->getName() + "\n" +
				  tintString + 
				  teamString +
				  (driveThrough == true ? "  drivethrough\n" : "") +
				  (shootThrough == true ? "  shootThrough\n" : "") +
				  this->BZWLines() +
				  "end\n";
}

// render
int group::render(void) { return 0; }
