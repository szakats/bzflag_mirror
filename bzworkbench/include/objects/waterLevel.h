#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class waterLevel : public DataEntry {
public:

	waterLevel() : DataEntry("waterLevel", "<name><height><material>") {
		this->name = string("default_name");
		this->materialName = string("");
		height = -1.0f;
	}
	
	waterLevel(string& data) : DataEntry("waterLevel", "<name><height><material>") { this->update(data); }
	
	// get method
	string get(void) {
		return this->toString();
	}
	
	// update method
	void update(string& data) {
		const char* header = this->getHeader().c_str();
		// make sure there's only one of these
		vector<string> waterLevelObjs = BZWParser::getSectionsByHeader(header, data.c_str());
		if(waterLevelObjs.size() > 1) {
			printf("waterLevel::update():  Warning! Multiple (%d) waterLevel objects; choosing the first one.\n", waterLevelObjs.size());
		}
		else if(waterLevelObjs.size() < 1) {
			printf("waterLevel::update():  Warning! No waterLevel object declared!\n");
			return;	
		}
		
		const char* waterData = waterLevelObjs[0].c_str();
		
		// get the names, but only choose the first
		vector<string> names = BZWParser::getValuesByKey("name", header, waterData);
		if(names.size() >= 1) {
			if(names.size() > 1) printf("waterLevel::update():  Warning! Multiple (%d) names; choosing the first one.\n", names.size());	
			this->name = names[0];
		}
		
		// get the material names, but only choose the last
		vector<string> matNames = BZWParser::getValuesByKey("material", header, waterData);
		if(matNames.size() >= 1) {
			if(matNames.size() > 1) printf("waterLevel::update():  Warning! Multiple (%d) materials; choosing the first one.\n", matNames.size());
			this->materialName = matNames[0];	
		}
		
		// get the water heights, but only choose the last
		vector<string> heights = BZWParser::getValuesByKey("height", header, waterData);
		if(heights.size() >= 1) {
			if(heights.size() > 1) printf("waterLevel::update:  Warning! Multiple (%d) heights; choosing the first one.\n", heights.size());
			this->height = atof( heights[0].c_str() );
		}
		
	}
	
	// toString method
	string toString(void) {
		string waterLevelStr = string(ftoa(height));
		return string(string("waterLevel\n") +
							 "  name " + name + "\n" +
							 "  height " + waterLevelStr + "\n" +
							 (materialName.length() == 0 ? "#" : " ") + " material " + materialName + "\n" +
							 "end\n");
	}
	
	// render method
	int render(void) { return 0; }
	
	virtual ~waterLevel() { }
	
private:
	string name;
	string materialName;
	float height;
};

#endif /*WATERLEVEL_H_*/
