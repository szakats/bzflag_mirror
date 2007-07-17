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

#include "../Observable.h"
#include "ObserverMessage.h"

#include <osg/ref_ptr>

using namespace std;

class Model : public Observable
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
	static vector< bz2object* >& getObjects();
	static vector< material* >& getMaterials();
	static vector< texturematrix* >& getTextureMatrices();
	static vector< physics* >& getPhysicsDrivers();
	static vector< Tlink* >& getTeleporterLinks();
	static vector< define* >& getGroups();
	
	static void addObject( bz2object* obj );
	static DataEntry* buildObject( const char* header );
	static void removeObject( bz2object* obj );
	static void setSelected( bz2object* obj );
	static void setUnselected( bz2object* obj );
	static void unselectAll();
	static bool isSelected( bz2object* obj );
	static vector< bz2object* >& getSelection();
	
	// editor-like methods (BZWB-specific)
	static bool cutSelection();
	static bool copySelection();
	static bool pasteSelection();
	
	// instantiated BZWB-specific API 
	world* _getWorldData() { return worldData; }
	options* _getOptionsData() { return optionsData; }
	waterLevel* _getWaterLevelData() { return waterLevelData; }
	vector< bz2object* >& _getObjects() { return this->objects; }
	vector< material* >& _getMaterials() { return this->materials; }
	vector< texturematrix* >& _getTextureMatrices() { return this->textureMatrices; }
	vector< dynamicColor* >& _getDynamicColors() { return this->dynamicColors; }
	vector< physics* >& _getPhysicsDrivers() { return this->phys; }
	vector< Tlink* >& _getTeleporterLinks() { return this->links; }
	vector< define* >& _getGroups() { return this->groups; }
	
	void _addObject( bz2object* obj );
	DataEntry* _buildObject( const char* header );
	void _removeObject( bz2object* obj );
	void _setSelected( bz2object* obj );
	void _setUnselected( bz2object* obj );
	void _unselectAll();
	bool _isSelected( bz2object* obj );
	vector< bz2object* >& _getSelection() { return this->selectedObjects; }
	
	// editor-like methods (BZWB-specific)--instantiated
	bool _cutSelection();
	bool _copySelection();
	bool _pasteSelection();
	
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
	vector< physics* > phys;
	
// materials
	vector< material* > materials;
	
// group definitions
	vector< define* > groups;
	
// dynamic colors
	vector< dynamicColor* > dynamicColors;

// links
	vector< Tlink* > links;

// texture matrices
	vector< texturematrix* > textureMatrices;
	
// objects
	vector< bz2object* > objects;
	
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
	
// vector of selected objects
	vector< bz2object* > selectedObjects;
	
// cut/copy buffer
	vector< osg::ref_ptr< bz2object > > objectBuffer;
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

// #include "../windows/View.h"

#endif /*MODEL_H_*/
