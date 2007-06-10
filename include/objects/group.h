#ifndef GROUP_H_
#define GROUP_H_

#include "bz2object.h"
#include "../render/RGBA.h"
#include "../BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class group : public bz2object {
	
	public:
	
		// constructor
		group() : bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>") {
			this->team = 0;
			this->tintColor = RGBA(1, 1, 1, 1);
			this->driveThrough = false;
			this->shootThrough = false;
			this->name = string("");
		}
		
		group(string& data) : bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>") {
			this->team = 0;
			this->tintColor = RGBA(1, 1, 1, 1);
			this->driveThrough = false;
			this->shootThrough = false;
			this->name = string("");
			
			this->update(data);	
		}
		
		// getter
		string get(void) {
			return this->toString(); 
		}
		
		// setter
		void update(string& data) {
			const char* header = this->getHeader().c_str();
			
			// get the section from the data
			const vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
			
			if(lines[0] == BZW_NOT_FOUND)
				return;
			
			const char* groupData = lines[0].c_str();
			
			// get name (from the first line)
			vector<string> headers = BZWParser::getValuesByKey("group", header, groupData);
			
			// get tint
			vector<string> tints = BZWParser::getValuesByKey("tint", header, groupData);
			if(!hasOnlyOne(tints, "tint")) {
				return;
			}
				
			// get team
			vector<string> teams = BZWParser::getValuesByKey("team", header, groupData);
			if(!hasOnlyOne(teams, "team"))
				return;
				
			// get drivethrough
			vector<string> driveThroughs = BZWParser::getValuesByKey("drivethrough", header, groupData);
				
			// get shootthrough
			vector<string> shootThroughs = BZWParser::getValuesByKey("shootthrough", header, groupData);
				
			// do base class update
			bz2object::update(data);
			
			// assign data
			this->name = headers[0];
			this->tintColor = RGBA( tints[0].c_str() );
			this->team = (int)(atof( teams[0].c_str() ));
			this->driveThrough = (driveThroughs.size() == 0 ? false : true);
			this->shootThrough = (shootThroughs.size() == 0 ? false : true);
			
		}
		
		// toString
		string toString(void) {
			return string("group ") + name + "\n" +
						  "  tint " + tintColor.toString() +
						  "  team " + string(ftoa(team)) + "\n" +
						  (driveThrough == true ? "  drivethrough\n" : "") +
						  (shootThrough == true ? "  shootThrough\n" : "") +
						  this->BZWLines() +
						  "end\n";
		}
		
		// render
		int render(void) { return 0; }
		
	private:
		// member data
		RGBA tintColor;
		bool driveThrough, shootThrough;
		int team;
		
};

#endif /*GROUP_H_*/
