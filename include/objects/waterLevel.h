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
	
	waterLevel(string& data) : DataEntry("waterLevel", "<name><height><material>") {
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
	void update(string& data) {
		const char* header = this->getHeader().c_str();
		// make sure there's only one of these
		vector<string> waterLevelObjs = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(waterLevelObjs[0] == BZW_NOT_FOUND)
			return;
			
		if(waterLevelObjs.size() >= 2) {
			printf("waterLevel::update():  Error! waterLevel defined %d times!\n", waterLevelObjs.size());
			return;
		}
		
		const char* waterData = waterLevelObjs[0].c_str();
		
		// get the names, but only choose the first
		vector<string> names = BZWParser::getValuesByKey("name", header, waterData);
		if(!hasOnlyOne(names, "name"))
			return;
		
		// get the material names, but only choose the last
		vector<string> matNames = BZWParser::getValuesByKey("matref", header, waterData);
		
		// get the water heights, but only choose the last
		vector<string> heights = BZWParser::getValuesByKey("height", header, waterData);
		if(!hasOnlyOne(heights, "height"))
			return;
			
		// load the data
		this->name = names[0];
		this->materials = matNames;
		this->height = atof( heights[0].c_str() );
		
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
