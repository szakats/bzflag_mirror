#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class waterLevel : public DataEntry {
public:

	waterLevel() : DataEntry("waterLevel", "<name><height><material>") {
		this->name = string("");
		this->materials = vector<string>();
		height = -1.0f;
	}
	
	waterLevel(string& data) : DataEntry("waterLevel", "<name><height><material>", data.c_str()) {
		this->name = string("");
		this->materials = vector<string>();
		height = -1.0f;
		
		this->update(data);
	}
	
	// get method
	string get(void) {
		return this->toString();
	}
	
	// update method
	int update(string& data) {
		
		const char* header = this->getHeader().c_str();
		// make sure there's only one of these
		vector<string> waterLevelObjs = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(waterLevelObjs[0] == BZW_NOT_FOUND)
			return 0;
			
		if(!hasOnlyOne(waterLevelObjs, "waterLevel"))
			return 0;
			
		if(waterLevelObjs.size() >= 2) {
			printf("waterLevel::update():  Error! waterLevel defined %d times!\n", waterLevelObjs.size());
			return 0;
		}
		
		const char* waterData = waterLevelObjs[0].c_str();
		
		// get the names, but only choose the first
		vector<string> names = BZWParser::getValuesByKey("name", header, waterData);
		if(!hasOnlyOne(names, "name"))
			return 0;
		
		// get the material names, but only choose the last
		vector<string> matNames = BZWParser::getValuesByKey("matref", header, waterData);
		
		// get the water heights, but only choose the last
		vector<string> heights = BZWParser::getValuesByKey("height", header, waterData);
		if(!hasOnlyOne(heights, "height"))
			return 0;
			
		// load the data
		if(!DataEntry::update(data))
			return 0;
		this->name = names[0];
		this->materials = matNames;
		this->height = atof( heights[0].c_str() );
		
		return 1;
		
	}
	
	// toString method
	string toString(void) {
		string waterLevelStr = string(ftoa(height));
		string materialString = string("");
		if(materials.size() >= 1)
			for(vector<string>::iterator i = materials.begin(); i != materials.end(); i++) {
				materialString += "  matref " + (*i) + "\n";
			}
		else
			materialString = string("# matref\n");
			
		return string(string("waterLevel\n") +
							 "  name " + name + "\n" +
							 "  height " + waterLevelStr + "\n" +
							 materialString +
							 this->getUnusedText() + 
							 "end\n");
	}
	
	// render method
	int render(void) { return 0; }
	
	virtual ~waterLevel() { }
	
private:
	string name;
	vector<string> materials;
	float height;
};

#endif /*WATERLEVEL_H_*/
