#include "../include/model/Model.h"

// global reference to the model so the static call will work
Model* modelRef;

Model::Model()
{
	worldData = new world();
	optionsData = new options();
	waterLevelData = new waterLevel();
	phys = vector<physics*>();
	dynamicColors = vector<dynamicColor*>();
	materials = vector<material*>();
	links = vector<Tlink*>();
	textureMatrices = vector<texturematrix*>();
	
	objects = vector<bz2object*>();
	modelRef = this;
	
	cmap = map<string, DataEntry* (*)(string&)>();
	
	this->supportedObjects = string("");
	this->objectHierarchy = string("");
	this->objectTerminators = string("");
	
	unusedData = vector<string>();
	
}

// constructor that takes information about which objects to support
Model::Model(const char* supportedObjects, const char* objectHierarchy, const char* objectTerminators) {
	worldData = new world();
	optionsData = new options();
	waterLevelData = new waterLevel();
	phys = vector<physics*>();
	dynamicColors = vector<dynamicColor*>();
	materials = vector<material*>();
	links = vector<Tlink*>();
	textureMatrices = vector<texturematrix*>();
	
	objects = vector<bz2object*>();
	modelRef = this;
	
	cmap = map<string, DataEntry* (*)(string&)>();
	
	this->supportedObjects = supportedObjects;
	this->objectHierarchy = objectHierarchy;
	this->objectTerminators = objectTerminators;
	
	unusedData = vector<string>();
	
}

Model::~Model()
{
	if(worldData)
		delete worldData;
	
	if(optionsData)
		delete optionsData;
		
	if(waterLevelData)
		delete waterLevelData;
		
	if(phys.size() > 0)
		for(vector<physics*>::iterator i = phys.begin(); i != phys.end(); i++)
			if((*i)) {
				delete *i;
				*i = NULL;	
			}
			
	if(materials.size() > 0)
		for(vector<material*>::iterator i = materials.begin(); i != materials.end(); i++)
			if((*i)) {
				delete *i;
				*i = NULL;	
			}
			
	if(objects.size() > 0)
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++)
			if((*i)) {
				delete *i;
				*i = NULL;	
			}
			
}

// the query method
string& Model::query(const char* command) { 
	return modelRef->_query(command);
}

// the *real* query method
string& Model::_query(const char* command) {
	// get the command sequence
	// vector<string> commandElements = BZW
	
	string str("");
	return str;
}

// the static build method
bool Model::build(vector<string>& bzworld) { return modelRef->_build(bzworld); }

// the real build method
bool Model::_build(vector<string>& bzworld) {
	printf("Model::build() called...\n");
	
	// don't bother for empty vectors
	if(bzworld.size() == 0)
		return false;
		
	// load the data in
	bool foundWorld = false;
	for(vector<string>::iterator i = bzworld.begin(); i != bzworld.end(); i++) {
		string header = BZWParser::headerOf(i->c_str());
		// parse world
		if(header == "world") {
			if(cmap[header] != NULL) {
				printf("Found \"world\" object, parsing...\n");
				this->worldData->update(*i);
				foundWorld = true;		// there must be a world 
				continue;
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse waterLevel
		else if(header == "waterLevel") {
			if(cmap[header] != NULL) {
				printf("Found \"waterLevel\" object, parsing...\n");
				this->waterLevelData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse options
		else if(header == "options") {
			if(cmap[header] != NULL) {
				printf("Found \"options\" object, parsing...\n");
				this->optionsData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse materials
		else if(header == "material") {
			if(cmap[header] != NULL) {
				printf("Found \"material\" object, parsing...\n");
				materials.push_back((material*)cmap[header](*i));
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse physics
		else if(header == "physics") {
			if(cmap[header] != NULL) {
				printf("Found \"physics\" object, parsing...\n");
				phys.push_back((physics*)cmap[header](*i));
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse dynamicColors
		else if(header == "dynamicColor") {
			if(cmap[header] != NULL) {
				printf("Found \"dynamicColor\" object, parsing...\n");
				dynamicColors.push_back((dynamicColor*)cmap[header](*i));	
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse links
		else if(header == "link") {
			if(cmap[header] != NULL) {
				printf("Found \"link\" object, parsing...\n");
				links.push_back((Tlink*)cmap[header](*i));	
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse texturematrices
		else if(header == "texturematrix") {
			if(cmap[header] != NULL) {
				printf("Found \"texturematrix\" object, parsing...\n");
				textureMatrices.push_back((texturematrix*)cmap[header](*i));
				continue;	
			}	
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse all other objects
		else {
			if(cmap[header] != NULL) {
				printf("Found \"%s\" object, parsing...\n", header.c_str());
				objects.push_back((bz2object*)cmap[header](*i));
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
	}
	// if there's no world, then there's no level
	if(!foundWorld)
		return false;
	
	return true;
}

// BZWB-specific API
world* Model::getWorldData() { return modelRef->_getWorldData(); }
options* Model::getOptionsData() { return modelRef->_getOptionsData(); }
waterLevel* Model::getWaterLevelData() { return modelRef->_getWaterLevelData(); }

// plug-in specific API

/**
 * Register an object with the model.  It maps the object's header string with a function that will initialize the object
 * (usually, this is the static init() method in the built-in objects).
 * Returns true of the object was added; false if there's something already registered with that name
 */
bool Model::registerObject(string& name, DataEntry* (*init)(string&)) { return modelRef->_registerObject(name, init); }
bool Model::registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&))
	{ return modelRef->_registerObject(name, hierarchy, terminator, init); }
	
bool Model::_registerObject(string& name, DataEntry* (*init)(string&)) {
	return this->_registerObject( name.c_str(), "", "end", init);
}

bool Model::_registerObject(const char* _name, const char* _hierarchy, const char* _terminator, DataEntry* (*init)(string&)) {
	// catch NULLs
	if(_name == NULL)
		return false;
	if(_terminator == NULL)
		_terminator = "end";
	
	string name = _name;
	string hierarchy = (_hierarchy != NULL ? _hierarchy : "");
	string terminator = _terminator;
	
	printf("Registering \"%s\" with hierarchy \"%s\" and terminator \"%s\" in the model...\n", name.c_str(), hierarchy.c_str(), terminator.c_str());
	
	// add support for this object
	if(!this->addObjectSupport( _name ))
		return false;
		
	// add support for this terminator
	if(!this->addTerminatorSupport( _name, _terminator ))
		return false;
	
	// add support for this hierarchy
	this->addSupportedHierarchy( _hierarchy );
	
	// add the initializer for this object
	this->cmap[ name ] = init;
	
	printf("supportedObjects:  %s\n", supportedObjects.c_str());
	printf("objectHierarchy:   %s\n", objectHierarchy.c_str());
	printf("objectTerminators: %s\n", objectTerminators.c_str());
	printf("======================================================================\n");
	
	return true;
		
}

// is an object supported?
bool Model::isSupportedObject(const char* name) { return modelRef->_isSupportedObject(name); }
bool Model::_isSupportedObject( const char* name ) {
	if( name == NULL || strlen(name) == 0)
		return false;
		
	return (this->supportedObjects.find( string("<") + name + ">", 0 ) != string::npos);	
}

// is the terminator supported?
bool Model::isSupportedTerminator(const char* name, const char* end) { return modelRef->_isSupportedTerminator(name, end); }
bool Model::_isSupportedTerminator( const char* name, const char* end ) {
	if((name == NULL || strlen(name) == 0) || (end == NULL || strlen(end) == 0))
		return false;
		
	return (this->objectTerminators.find( string("<") + name + "|" + end + ">", 0 ) != string::npos);	
}

// is the hierarchy supported?
bool Model::isSupportedHierarchy(const char* name) { return modelRef->_isSupportedHierarchy(name); }
bool Model::_isSupportedHierarchy( const char* name ) {
	if(name == NULL || strlen(name) == 0)
		return false;
		
	return (this->objectHierarchy.find( name, 0 ) != string::npos);	
}

/**************************
 * 
 * Private methods to manipulate Model::supportedObjects, Model::objectTerminators, and Model::objectHierarchy
 * 
 **************************/
 
// add the name of an object to the list of supported objects; return false if it's already there
bool Model::addObjectSupport(const char* name) {
	if(name == NULL || this->_isSupportedObject( name ))
		return false;
		
	this->supportedObjects += string("<") + name + ">";
	return true;
}

// remove the name of an object from the list of supported objects; return false if not found
bool Model::removeObjectSupport(const char* name) {
	if(!this->_isSupportedObject( name ))
		return false;
		
	// get the start of the object
	string::size_type index = supportedObjects.find( string("<") + name + ">", 0 );
	
	// get the end of the object
	string::size_type end = supportedObjects.find( ">", index+1 );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = supportedObjects.substr(0, index);
	
	if(end < supportedObjects.size() - 1)
		endChunk = supportedObjects.substr(end + 1);
		
	// regroup the string
	supportedObjects = frontChunk + endChunk;
	
	return true;
	
}

// add support for an object hierarchy
bool Model::addSupportedHierarchy(const char* name) {
	if(name == NULL || this->_isSupportedHierarchy( name ))
		return false;
	
	this->objectHierarchy += string("<") + name + ">";
	return true;
}


// add support for an object terminator
bool Model::addTerminatorSupport(const char* name, const char* end) {
	if(name == NULL || this->_isSupportedTerminator( name, end ))
		return false;
		
	this->objectTerminators += string("<") + name + "|" + end + ">";
	return true;
}

// the universal getter--returns the entire working model as a string of BZW-formatted text
string& Model::toString() { return modelRef->_toString(); }
string& Model::_toString() {
	// iterate through all objects and have them print themselves out in string format
	static string ret = "";
	ret.clear();
	
	string worldDataString = (worldData != NULL ? worldData->toString() : "\n");
	string optionsDataString = (optionsData != NULL ? optionsData->toString() : "\n");
	string waterLevelString = (waterLevelData != NULL ? waterLevelData->toString() : "\n");
	
	// global data
	ret += "\n#--World-----------------------------------------\n\n" + worldDataString;
	ret += "\n#--Options---------------------------------------\n\n" + optionsDataString;
	ret += "\n#--Water Level-----------------------------------\n\n" + waterLevelString;
	
	// physics drivers
	ret += "\n#--Physics Drivers-------------------------------\n\n";
	if(phys.size() > 0) {
		for(vector<physics*>::iterator i = phys.begin(); i != phys.end(); i++) {
			ret += (*i)->toString() + "\n";
		}
	}
	
	// materials
	ret += "\n#--Materials-------------------------------------\n\n";
	if(materials.size() > 0) {
		for(vector<material*>::iterator i = materials.begin(); i != materials.end(); i++) {
			ret += (*i)->toString() + "\n";
		}	
	}
	
	// dynamic colors
	ret += "\n#--Dynamic Colors--------------------------------\n\n";
	if(dynamicColors.size() > 0) {
		for(vector<dynamicColor*>::iterator i = dynamicColors.begin(); i != dynamicColors.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}
	}
	
	// links
	ret += "\n#--Teleporter Links------------------------------\n\n";
	if(links.size() > 0) {
		for(vector<Tlink*>::iterator i = links.begin(); i != links.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}	
	}
	
	// texture matrices
	ret += "\n#--Texture Matrices------------------------------\n\n";
	if(textureMatrices.size() > 0) {
		for(vector<texturematrix*>::iterator i = textureMatrices.begin(); i != textureMatrices.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}
	}
	
	// all other objects
	ret += "\n#--Objects---------------------------------------\n\n";
	if(objects.size() > 0) {
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}	
	}
	
	// unused dats
	ret += "\n#--Unused Data-----------------------------------\n\n";
	if(unusedData.size() > 0) {
		for(vector<string>::iterator i = unusedData.begin(); i != unusedData.end(); i++) {
			ret += (*i) + "\n";
		}
	}
	
	return ret;
}
