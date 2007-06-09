#ifndef MODEL_H_
#define MODEL_H_

class Model;

#include "DataEntry.h"
#include "objects/world.h"
#include "objects/waterLevel.h"
#include "objects/options.h"
#include "objects/physics.h"
#include "objects/material.h"

#include <string>
#include <map>

using namespace std;

class Model
{
public:
	Model();
	virtual ~Model();
	
	static DataEntry* query(char* data);
	
	// the real query method
	DataEntry* _query(char* data) { return dataMap[string(data)]; }
	
	
private:

// world options
	world* worldData;
	options* optionsData;
	waterLevel* waterLevelData;
	
// maps keys to values
	void buildDatabase(void);
	
// the data map
	map<string, DataEntry*> dataMap;
	
// the physics drivers and materials
	map<string, physics> physicsDrivers;
	map<string, material> materials;
	
};


#endif /*MODEL_H_*/
