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
class teleporter;
class group;

// supported query commands.
#define MODEL_GET "get"

#include "../Observable.h"
#include "ObserverMessage.h"

#include "../dialogs/ConfigurationDialog.h"

#include <osg/ref_ptr>
#include <osg/Vec3>

using namespace std;

class Model : public Observable
{
public:
	Model();
	Model(const char* supportedObjects, const char* objectHierarchy, const char* terminators);
	
	virtual ~Model();
	
	// string-based control mechanism
	// command syntax: "<command> <object> <name>[ data]", where
	// command is the query command, object is the type of object, the name is the object's name, and data is any additional data the command would need
	static DataEntry* command(const string& command, const string& object, const string& name, const string& data = "" );
	
	// the real query method
	DataEntry* _command(const string& command, const string& object, const string& name, const string& data = "" );
	
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
	static map< string, material* >& getMaterials();
	static map< string, texturematrix* >& getTextureMatrices();
	static map< string, physics* >& getPhysicsDrivers();
	static map< string, Tlink* >& getTeleporterLinks();
	static map< string, define* >& getGroups();
	
	static void addObject( bz2object* obj );
	static DataEntry* buildObject( const char* header );
	static void removeObject( bz2object* obj );
	static void setSelected( bz2object* obj );
	static void setUnselected( bz2object* obj );
	static void unselectAll();
	static bool isSelected( bz2object* obj );
	static vector< bz2object* >& getSelection();
	static void assignMaterial( const string& matref, bz2object* obj );
	static void assignMaterial( material* matref, bz2object* obj );
	static ConfigurationDialog* configureObject( DataEntry* obj );
	
	// editor-like methods (BZWB-specific)
	static bool cutSelection();
	static bool copySelection();
	static bool pasteSelection();
	static bool deleteSelection();
	static bool newWorld();
	static bool linkTeleporters( teleporter* t1, teleporter* t2 );
	static void groupObjects( vector< osg::ref_ptr< bz2object > >& objects );
	static void ungroupObjects( group* g );
	
	// instantiated BZWB-specific API 
	world* _getWorldData() { return worldData; }
	options* _getOptionsData() { return optionsData; }
	waterLevel* _getWaterLevelData() { return waterLevelData; }
	vector<bz2object* >& _getObjects() { return this->objects; }
	map< string, material* >& _getMaterials() { return this->materials; }
	map< string, texturematrix* >& _getTextureMatrices() { return this->textureMatrices; }
	map< string, dynamicColor* >& _getDynamicColors() { return this->dynamicColors; }
	map< string, physics* >& _getPhysicsDrivers() { return this->phys; }
	map< string, Tlink* >& _getTeleporterLinks() { return this->links; }
	map< string, define* >& _getGroups() { return this->groups; }
	
	void _addObject( bz2object* obj );
	DataEntry* _buildObject( const char* header );
	void _removeObject( bz2object* obj );
	void _setSelected( bz2object* obj );
	void _setUnselected( bz2object* obj );
	void _unselectAll();
	bool _isSelected( bz2object* obj );
	vector< bz2object* >& _getSelection() { return this->selectedObjects; }
	void _assignMaterial( const string& matref, bz2object* obj );
	void _assignMaterial( material* matref, bz2object* obj );
	ConfigurationDialog* _configureObject( DataEntry* obj );
	
	// editor-like methods (BZWB-specific)--instantiated
	bool _cutSelection();
	bool _copySelection();
	bool _pasteSelection();
	bool _deleteSelection();
	bool _newWorld();
	bool _linkTeleporters( teleporter* t1, teleporter* t2 );
	void _groupObjects( vector< osg::ref_ptr< bz2object > >& objects );
	void _ungroupObjects( group* g );
	
	// plugin-specific API
	static bool registerObject(string& name, DataEntry* (*init)(string&));
	static bool registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&), ConfigurationDialog* (*config)(DataEntry*) = NULL);
	static bool isSupportedObject(const char* name);
	static bool isSupportedTerminator(const char* name, const char* end);
	static bool isSupportedHierarchy(const char* name);
	
	// model-specific API
	static const string getSupportedObjects();
	static const string getSupportedHierarchies();
	static const string getSupportedTerminators();
	static bool hasInitializer( DataEntry* d );
	static bool hasConfigurationDialog( DataEntry* d );
	
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
	bool _registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&), ConfigurationDialog* (*config)(DataEntry*));
	bool _isSupportedObject(const char* name);
	bool _isSupportedTerminator(const char* name, const char* end);
	bool _isSupportedHierarchy(const char* name);
	
	// model-specific API
	const string _getSupportedObjects();
	const string _getSupportedHierarchies();
	const string _getSupportedTerminators();
	bool _hasInitializer( DataEntry* d ) { return ( cmap.count( d->getHeader() ) == 0 || cmap[ d->getHeader() ] == NULL ) ? false : true; }
	bool _hasConfigurationDialog( DataEntry* d ) { return ( configMap.count( d->getHeader() ) == 0 || configMap[ d->getHeader() ] == NULL ) ? false : true; }
	
	// misc API
	static vector< osg::ref_ptr< bz2object > > toRefList( vector< bz2object* >& objs );
	static vector< bz2object* > toNonRefList( vector< osg::ref_ptr< bz2object > >& objs );
	
	// instantiated misc API
	vector< osg::ref_ptr< bz2object > > _toRefList( vector< bz2object* >& objs );
	vector< bz2object* > _toNonRefList( vector< osg::ref_ptr< bz2object > >& objs );
	
	
private:

// world options
	world* worldData;
	options* optionsData;
	waterLevel* waterLevelData;
	
// physics (map refname to object )
	map< string, physics* > phys;
	
// materials (map refname to object )
	map< string, material* > materials;
	osg::ref_ptr< material > defaultMaterial; 		// default material
	
// group definitions (map refname to object)
	map< string, define* > groups;
	
// dynamic colors (map refname to the object )
	map< string, dynamicColor* > dynamicColors;

// links (map refname to the object )
	map< string, Tlink* > links;

// texture matrices (map refname to the object itself) 
	map< string, texturematrix* > textureMatrices;
	
// objects
	vector< bz2object* > objects;
	
// world data (array of all objects in BZW format)
	vector<string> data;
	
// map from strings to initializers
	map<string, DataEntry* (*)(string&)> cmap;
	
// map from strings to configuration dialog initializers
	map<string, ConfigurationDialog* (*)(DataEntry*)> configMap;
	
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
