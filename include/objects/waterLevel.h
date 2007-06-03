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
	void get(string& data) {
		data.assign(this->toString());
	}
	
	// update method
	void update(string& data) {
		string header = this->getHeader();
		// make sure there's only one of these
		vector<string> waterLevelObjs = BZWParser::getSectionsByHeader(header, data);
		if(waterLevelObjs.size() > 1) {
			printf("waterLevel::update():  Warning! Multiple (%d) waterLevel objects; choosing the first one.\n", waterLevelObjs.size());
		}
		else if(waterLevelObjs.size() < 1) {
			printf("waterLevel::update():  Warning! No waterLevel object declared!\n");
			return;	
		}
		
		string waterData = waterLevelObjs[0];
		
		// get the names, but only choose the first
		string nameKey = string("name");
		vector<string> names = BZWParser::getValuesByKey(nameKey, header, waterData);
		if(names.size() >= 1) {
			if(names.size() > 1) printf("waterLevel::update():  Warning! Multiple (%d) names; choosing the first one.\n", names.size());	
			this->name = names[0];
		}
		
		// get the material names, but only choose the last
		string matNameKey = string("material");
		vector<string> matNames = BZWParser::getValuesByKey(matNameKey, header, waterData);
		if(matNames.size() >= 1) {
			if(matNames.size() > 1) printf("waterLevel::update():  Warning! Multiple (%d) materials; choosing the first one.\n", matNames.size());
			this->materialName = matNames[0];	
		}
		
		// get the water heights, but only choose the last
		string heightKey = string("height");
		vector<string> heights = BZWParser::getValuesByKey(heightKey, header, waterData);
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
	
	virtual ~waterLevel() { }
	
private:
	string name;
	string materialName;
	float height;
};

#endif /*WATERLEVEL_H_*/
