#ifndef MODEL_H_
#define MODEL_H_

class Model;

#include "../DataEntry.h"

#include "../objects/arc.h"
#include "../objects/base.h"
#include "../objects/box.h"
#include "../objects/cone.h"
#include "../objects/dynamicColor.h"
#include "../objects/group.h"
#include "../objects/link.h"
#include "../objects/material.h"
#include "../objects/mesh.h"
#include "../objects/meshbox.h"
#include "../objects/meshpyr.h"
#include "../objects/options.h"
#include "../objects/physics.h"
#include "../objects/pyramid.h"
#include "../objects/sphere.h"
#include "../objects/teleporter.h"
#include "../objects/tetra.h"
#include "../objects/texturematrix.h"
#include "../objects/waterLevel.h"
#include "../objects/weapon.h"
#include "../objects/world.h"
#include "../objects/zone.h"

#include "../objects/bz2object.h"

#include <string>
#include <map>

// supported query commands.
#define MODEL_QUERY_COMMANDS "<get><set>"


using namespace std;

class Model
{
public:
	Model();
	virtual ~Model();
	
	// query method; data is a command
	// command syntax: "<command> <object> <name>[ data]", where
	// command is the query command, object is the type of object, and data is any additional data the command would need
	static string& query(const char* command);
	
	// the real query method
	string& _query(const char* command);
	
	// this method builds the model from a vector of BZW-formatted strings
	// returns true of it builds properly
	static bool build(vector<string>& bzworld);
	
	// the real build method
	// returns false if it fails
	bool _build(vector<string>& bzworld);
	
	// BZWB-specific API for built-in objects
	static world* getWorldData();
	static waterLevel* getWaterLevelData();
	static options* getOptionsData();
	
	// instantiated BZWB-specific API
	world* _getWorldData() { return worldData; }
	options* _getOptionsData() { return optionsData; }
	waterLevel* _getWaterLevelData() { return waterLevelData; }
	
	// plugin-specific API
	static bool registerObject(string name, DataEntry* (*init)(string&));
	bool _registerObject(string name, DataEntry* (*init)(string&));
	
private:

// world options
	world* worldData;
	options* optionsData;
	waterLevel* waterLevelData;
	
// physics
	vector<physics*> phys;
	
// materials
	vector<material*> materials;
	
// dynamic colors
	vector<dynamicColor*> dynamicColors;

// links
	vector<Tlink*> links;

// texture matrices
	vector<texturematrix*> textureMatrices;
	
// objects
	vector<bz2object*> objects;
	
// world data (array of all objects in BZW format)
	vector<string> data;
	
// map from strings to initializers
	map<string, DataEntry* (*)(string&)> cmap;
	
// build the default bzw objects in
	void buildDatabase();
	
};


#endif /*MODEL_H_*/
