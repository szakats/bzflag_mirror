#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <vector>
#include <map>

// forward declarations of some objects
class bz2object;
class world;
class waterLevel;
class options;
class texturematrix;
class dynamicColor;
class physics;
class Tlink;
class define;
class DataEntry;
class material;

// supported query commands.
#define MODEL_QUERY_COMMANDS "<get><set>"

using namespace std;

class Model
{
public:
	Model();
	Model(const char* supportedObjects, const char* objectHierarchy, const char* terminators);
	
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
	
	// universal getter
	static string& toString(void);
	
	// the "real" universal getter
	string& _toString(void);
	
	// BZWB-specific API for built-in objects
	static world* getWorldData();
	static waterLevel* getWaterLevelData();
	static options* getOptionsData();
	
	// instantiated BZWB-specific API 
	world* _getWorldData() { return worldData; }
	options* _getOptionsData() { return optionsData; }
	waterLevel* _getWaterLevelData() { return waterLevelData; }
	
	// plugin-specific API
	static bool registerObject(string& name, DataEntry* (*init)(string&));
	static bool registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&));
	static bool isSupportedObject(const char* name);
	static bool isSupportedTerminator(const char* name, const char* end);
	static bool isSupportedHierarchy(const char* name);
	
	// model-specific API
	static const string getSupportedObjects();
	static const string getSupportedHierarchies();
	static const string getSupportedTerminators();
	
	// methods to manipulate the aforementioned strings
	static bool addObjectSupport(const char* name);
	bool _addObjectSupport(const char* name);
	
	static bool removeObjectSupport(const char* name);
	bool _removeObjectSupport(const char* name);
	
	static bool addTerminatorSupport(const char* name, const char* end);
	bool _addTerminatorSupport(const char* name, const char* end);
	
	static bool removeTerminatorSupport(const char* name, const char* end);
	bool _removeTerminatorSupport(const char* name, const char* end);
	
	static bool addSupportedHierarchy(const char* name);
	bool _addSupportedHierarchy(const char* name);
	
	static bool removeSupportedHierarchy(const char* name);
	bool _removeSupportedHierarchy(const char* name);
	
	// instantiated plug-in API
	bool _registerObject(string& name, DataEntry* (*init)(string&));
	bool _registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&));
	bool _isSupportedObject(const char* name);
	bool _isSupportedTerminator(const char* name, const char* end);
	bool _isSupportedHierarchy(const char* name);
	
	// model-specific API
	const string _getSupportedObjects();
	const string _getSupportedHierarchies();
	const string _getSupportedTerminators();
	
private:

// world options
	world* worldData;
	options* optionsData;
	waterLevel* waterLevelData;
	
// physics
	vector<physics*> phys;
	
// materials
	vector<material*> materials;
	
// group definitions
	vector<define*> groups;
	
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
	
// list of registered object keys
	string supportedObjects;
	
// list of object/terminator pairs (exceptions to object..end)
	string objectTerminators;
	
// hierarchy of objects and subobjects
	string objectHierarchy;
	
// save unused data chunks
	vector<string> unusedData;
};

#include "BZWParser.h"

#include "../DataEntry.h"

#include "../objects/arc.h"
#include "../objects/base.h"
#include "../objects/box.h"
#include "../objects/cone.h"
#include "../objects/define.h"
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


#endif /*MODEL_H_*/
