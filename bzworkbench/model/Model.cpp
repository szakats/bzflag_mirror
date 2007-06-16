#include "../include/model/Model.h"

// global reference to the model so the static call will work
Model* modelRef;

// register the built-in objects
void Model::buildDatabase() {
	registerObject("arc", arc::init);
	registerObject("base", base::init);
	registerObject("box", box::init);
	registerObject("cone", cone::init);
	registerObject("dynamicColor", dynamicColor::init);
	registerObject("group", group::init);
	registerObject("link", Tlink::init);
	registerObject("material", material::init);
	registerObject("mesh", mesh::init);
	registerObject("meshbox", meshbox::init);
	registerObject("meshpyr", meshpyr::init);
	registerObject("options", options::init);
	registerObject("physics", physics::init);
	registerObject("sphere", sphere::init);
	registerObject("teleporter", teleporter::init);
	registerObject("tetra", tetra::init);
	registerObject("texturematrix", texturematrix::init);
	registerObject("waterLevel", waterLevel::init);
	registerObject("weapon", weapon::init);
	registerObject("world", world::init);
	registerObject("zone", zone::init);
}

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
	// don't bother for empty vectors
	if(bzworld.size() == 0)
		return false;
		
	// load the data in
	bool foundWorld = false;
	for(vector<string>::iterator i = bzworld.begin(); i != bzworld.end(); i++) {
		string header = BZWParser::headerOf(i->c_str());
		
		// parse world
		if(header == "world") {
			this->worldData->update(*i);
			foundWorld = true;		// there must be a world
		}
		
		// parse waterLevel
		else if(header == "waterLevel") {
			this->waterLevelData->update(*i);	
		}
		
		// parse options
		else if(header == "options") {
			this->optionsData->update(*i);	
		}
		
		// parse materials
		else if(header == "material") {
			if(cmap[header] != NULL) {
				materials.push_back((material*)cmap[header](*i));
			}
		}
		
		// parse physics
		else if(header == "physics") {
			if(cmap[header] != NULL) {
				phys.push_back((physics*)cmap[header](*i));
			}
		}
		
		// parse dynamicColors
		else if(header == "dynamicColor") {
			if(cmap[header] != NULL) {
				dynamicColors.push_back((dynamicColor*)cmap[header](*i));	
			}	
		}
		
		// parse links
		else if(header == "link") {
			if(cmap[header] != NULL) {
				links.push_back((Tlink*)cmap[header](*i));	
			}
		}
		
		// parse texturematrices
		else if(header == "texturematrix") {
			if(cmap[header] != NULL) {
				textureMatrices.push_back((texturematrix*)cmap[header](*i));	
			}	
		}
		
		// parse all other objects
		else {
			if(cmap[header] != NULL) {
				objects.push_back((bz2object*)cmap[header](*i));
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
bool Model::registerObject(string name, DataEntry* (*init)(string&)) { return modelRef->_registerObject(name, init); }
bool Model::_registerObject(string name, DataEntry* (*init)(string&)) {
	if(this->cmap[name] == NULL) {
		this->cmap[name] = init;
		return true;
	}
	else
		return false;
}
