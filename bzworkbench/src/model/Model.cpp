/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "model/Model.h"
#include "windows/View.h"

Model* Model::modRef;

Model::Model() : Observable()
{
	this->worldData = new world();
	this->optionsData = new options();
	this->waterLevelData = new waterLevel();
	this->phys = map< string, physics* >();
	this->dynamicColors = map< string, dynamicColor* >();
	this->materials = map< string, material* >();
	this->links = map< string, Tlink* >();
	this->textureMatrices = map< string, texturematrix* >();
	this->groups = map< string, define* >();
	
	// make a default material
	this->defaultMaterial = new material();
	this->defaultMaterial->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	this->defaultMaterial->setDiffuse( osg::Vec4( 1, 1.0, 1.0, 1.0 ) );
	this->defaultMaterial->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0) );
	this->defaultMaterial->setEmissive( osg::Vec4( 0.0, 0.0, 0.0, 1.0) );
	
	vector< osg::ref_ptr<osg::Texture2D> > defaultTextures = vector< osg::ref_ptr< osg::Texture2D > >();
	defaultTextures.push_back( SceneBuilder::buildTexture2D( "share/tetrawall.png" ) );
	
	this->defaultMaterial->setTextures( defaultTextures );
	
	this->objects = objRefList();
	modRef = this;
	
	// this->cmap = map<string, DataEntry* (*)(string&)>();
	
	this->supportedObjects = string("");
	this->objectHierarchy = string("");
	this->objectTerminators = string("");
	
	this->unusedData = vector<string>();
	
}

// constructor that takes information about which objects to support
Model::Model(const char* _supportedObjects, const char* _objectHierarchy, const char* _objectTerminators) : Observable() {
	this->worldData = new world();
	this->optionsData = new options();
	this->waterLevelData = new waterLevel();
	this->phys = map< string, physics* >();
	this->dynamicColors = map< string, dynamicColor* >();
	this->materials = map< string, material* >();
	this->links = map< string, Tlink* >();
	this->textureMatrices = map< string, texturematrix* >();
	this->groups = map< string, define* >();
	
	// make a default material
	this->defaultMaterial = new material();
	this->defaultMaterial->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	this->defaultMaterial->setDiffuse( osg::Vec4( 1, 1.0, 1.0, 1.0 ) );
	this->defaultMaterial->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0) );
	this->defaultMaterial->setEmissive( osg::Vec4( 0.0, 0.0, 0.0, 1.0) );
	
	vector< osg::ref_ptr<osg::Texture2D> > defaultTextures = vector< osg::ref_ptr< osg::Texture2D > >();
	defaultTextures.push_back( SceneBuilder::buildTexture2D( "share/tetrawall.png" ) );
	
	this->defaultMaterial->setTextures( defaultTextures );
	
	this->objects = objRefList();
	modRef = this;
	
	// this->cmap = map<string, DataEntry* (*)(string&)>();
	
	this->supportedObjects = _supportedObjects;
	this->objectHierarchy = _objectHierarchy;
	this->objectTerminators = _objectTerminators;
	
	this->unusedData = vector<string>();
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
		for(map<string, physics*>::iterator i = phys.begin(); i != phys.end(); i++)
			if((i->second)) {
				delete i->second;
				i->second = NULL;	
			}
			
	if(materials.size() > 0)
		for(map<string, material*>::iterator i = materials.begin(); i != materials.end(); i++)
			if((i->second)) {
				delete i->second;
				i->second = NULL;	
			}
			
	if(objects.size() > 0)
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++)
			if((i->get())) {
				*i = NULL;	// this will deref the object, calling a destructor
			}
	
	if(groups.size() > 0)
		for(map<string, define*>::iterator i = groups.begin(); i != groups.end(); i++)
			if((i->second)) {
				delete i->second;
				i->second = NULL;	
			}
		
}

// getters specific to the model
const string Model::getSupportedObjects() { return modRef->_getSupportedObjects(); }
const string Model::_getSupportedObjects() { return supportedObjects; }

const string Model::getSupportedHierarchies() { return modRef->_getSupportedHierarchies(); }
const string Model::_getSupportedHierarchies() { return objectHierarchy; }

const string Model::getSupportedTerminators() { return modRef->_getSupportedTerminators(); }
const string Model::_getSupportedTerminators() { return objectTerminators; }

// the query method
DataEntry* Model::command(const string& command, const string& object, const string& name, const string& data) { 
	return modRef->_command(command, object, name, data);
}

// the *real* query method
// TODO: add more features
DataEntry* Model::_command(const string& command, const string& object, const string& name, const string& data) {
	// see if this is a "get" command
	if( command == MODEL_GET ) {
		// determine which type of object
		
		// handle dynamicColor
		if( object == "dynamicColor" ) {
			return ( this->dynamicColors.count( name ) > 0 ? this->dynamicColors[name] : NULL );
		}
		
		// handle texturematrices
		else if( object == "texturematrix" ) {
			return ( this->textureMatrices.count( name ) > 0 ? this->textureMatrices[name] : NULL );
		}
		
		// handle physics drivers
		else if( object == "phydrv" ) {
			return ( this->phys.count( name ) > 0 ? this->phys[name] : NULL );
		}
		
		// handle materials
		else if( object == "material" ) {
			return (this->materials.count( name ) > 0 ? this->materials[name] : NULL );
		}
		
		// handle teleporter links
		else if( object == "link" ) {
			return (this->links.count( name ) > 0 ? this->links[name] : NULL );
		}
		
		// handle definitions
		else if( object == "define" ) {
			return (this->groups.count( name ) > 0 ? this->groups[name] : NULL );
		}
		
		// handle all other objects
		else {
			for( objRefList::iterator i = this->objects.begin(); i != this->objects.end(); i++) {
				if( (*i)->getName() == name )
					return i->get();
			}
		}
	}
	
	return NULL;
}

// the static build method
bool Model::build(vector<string>& bzworld) { return modRef->_build(bzworld); }

// the real build method
bool Model::_build(vector<string>& bzworld) {
	
	// don't bother for empty vectors
	if(bzworld.size() == 0)
		return false;
	
	// clear out the previous objects
	this->materials.clear();
	this->phys.clear();
	this->dynamicColors.clear();
	this->links.clear();
	this->textureMatrices.clear();
	this->_unselectAll();
	if( this->objects.size() > 0 ) {
		objRefList::iterator itr = this->objects.begin();
		while( itr != this->objects.end() ) {
			
			ObserverMessage obs( ObserverMessage::REMOVE_OBJECT, itr->get() );
			notifyObservers( &obs );
			
			this->objects.erase( itr );
			itr = this->objects.begin();
			
		}
	}
	this->objects.clear();
	this->notifyObservers( NULL );
	
	// load the data in
	bool foundWorld = false;
	for(vector<string>::iterator i = bzworld.begin(); i != bzworld.end(); i++) {
		string header = BZWParser::headerOf(i->c_str());
		if(header == "world") {
			if(this->cmap.count(header) > 0) {
				this->worldData->update(*i);
				foundWorld = true;		// there must be a world
				// tell the observers we have a different world
				ObserverMessage obs( ObserverMessage::UPDATE_WORLD, worldData );
				this->notifyObservers( &obs );
				continue;
			}
			else {
				printf("Model::build(world): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse waterLevel
		else if(header == "waterLevel") {
			if(this->cmap.count(header) > 0) {
				this->waterLevelData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(waterLevel): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse options
		else if(header == "options") {
			if(this->cmap.count(header) > 0) {
				this->optionsData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(options): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse materials
		else if(header == "material") {
			if(this->cmap.count(header) > 0) {
				material* mat = ((material*)this->cmap[header](*i));
				if( mat ) {
					this->materials[ mat->getName() ] = mat;
				}
			}
			else {
				printf("Model::build(material): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse physics
		else if(header == "physics") {
			if(this->cmap.count(header) > 0) {
				physics* p = (physics*)this->cmap[header](*i);
				if( p ) {
					this->phys[ p->getName() ] = p;
				}
			}
			else {
				printf("Model::build(physics): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse dynamicColors
		else if(header == "dynamicColor") {
			if(this->cmap.count(header) > 0) {
				dynamicColor* dynCol = (dynamicColor*)this->cmap[header](*i);
				if( dynCol ) {
					this->dynamicColors[ dynCol->getName() ] = dynCol;
				}
			}
			else {
				printf("Model::build(dynamicColor): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse group definitions
		else if(header == "define") {
			if(this->cmap.count(header) > 0) {
				define* def = (define*)this->cmap[header](*i);
				if( def ) {
					this->groups[ def->getName() ] = def;
				}
			}
			else {
				printf("Model::build(define): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse links
		else if(header == "link") {
			if(this->cmap.count(header) > 0) {
				Tlink* link = (Tlink*)this->cmap[header](*i);
				if( link ) {
					this->links[ link->getName() ] = link;
				}
			}
			else {
				printf("Model::build(link): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse texturematrices
		else if(header == "texturematrix") {
			if(this->cmap.count(header) > 0) {
				texturematrix* tm = (texturematrix*)this->cmap[header](*i);
				if( tm ) {
					this->textureMatrices[ tm->getName() ] = tm;
				}
			}	
			else {
				printf("Model::build(texturematrix): Skipping undefined object \"%s\"\n", header.c_str());
				this->unusedData.push_back(*i);
			}
		}
		
		// parse all other objects
		else {
			if(this->cmap.count(header) > 0) {
				this->_addObject((bz2object*)cmap[header](*i));
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				printf("data: \n%s\n", i->c_str());
				this->unusedData.push_back(*i);
			}
		}
	}
	// if there's no world, then there's no level
	if(!foundWorld)
		return false;
	
	return true;
}

// BZWB-specific API
world* Model::getWorldData() { return modRef->_getWorldData(); }
options* Model::getOptionsData() { return modRef->_getOptionsData(); }
waterLevel* Model::getWaterLevelData() { return modRef->_getWaterLevelData(); }

// plug-in specific API

/**
 * Register an object with the model.  It maps the object's header string with a function that will initialize the object
 * (usually, this is the static init() method in the built-in objects).
 * Returns true of the object was added; false if there's something already registered with that name
 */
bool Model::registerObject(string& name, DataEntry* (*init)(string&)) { return modRef->_registerObject(name, init); }
bool Model::registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(string&), ConfigurationDialog* (*config)(DataEntry*))
	{ return modRef->_registerObject(name, hierarchy, terminator, init, config); }
	
bool Model::_registerObject(string& name, DataEntry* (*init)(string&)) {
	return this->_registerObject( name.c_str(), "", "end", init, NULL);
}

bool Model::_registerObject(const char* _name, const char* _hierarchy, const char* _terminator, DataEntry* (*init)(string&), ConfigurationDialog* (*config)(DataEntry*)) {
	// catch NULLs
	if(_name == NULL)
		return false;
	if(_terminator == NULL)
		_terminator = "end";
	
	string name = _name;
	string hierarchy = (_hierarchy != NULL ? _hierarchy : "");
	string terminator = _terminator;
	
	// add support for this object
	if(!this->_addObjectSupport( _name ))
		return false;
		
	// add support for this terminator
	if(!this->_addTerminatorSupport( _name, _terminator ))
		return false;
	
	// add support for this hierarchy
	this->_addSupportedHierarchy( _hierarchy );
	
	// add the initializer for this object
	this->cmap[ name ] = init;
	
	// add the configuration dialog for this object
	this->configMap[ name ] = config;
	
	return true;
		
}

// is an object supported?
bool Model::isSupportedObject(const char* name) { return modRef->_isSupportedObject(name); }
bool Model::_isSupportedObject( const char* name ) {
	if( name == NULL || strlen(name) == 0)
		return false;
		
	return (this->supportedObjects.find( string("<") + name + ">", 0 ) != string::npos);	
}

// is the terminator supported?
bool Model::isSupportedTerminator(const char* name, const char* end) { return modRef->_isSupportedTerminator(name, end); }
bool Model::_isSupportedTerminator( const char* name, const char* end ) {
	if((name == NULL || strlen(name) == 0) || (end == NULL || strlen(end) == 0))
		return false;
		
	return (this->objectTerminators.find( string("<") + name + "|" + end + ">", 0 ) != string::npos);	
}

// is the hierarchy supported?
bool Model::isSupportedHierarchy(const char* name) { return modRef->_isSupportedHierarchy(name); }
bool Model::_isSupportedHierarchy( const char* name ) {
	if(name == NULL || strlen(name) == 0)
		return false;
		
	return (this->objectHierarchy.find( name, 0 ) != string::npos);	
}

/**************************
 * 
 * methods to manipulate Model::supportedObjects, Model::objectTerminators, and Model::objectHierarchy
 * 
 **************************/
 
// add the name of an object to the list of supported objects; return false if it's already there
bool Model::addObjectSupport(const char* name) { return modRef->_addObjectSupport(name); }
bool Model::_addObjectSupport(const char* name) {
	if(name == NULL || this->_isSupportedObject( name ))
		return false;
		
	this->supportedObjects += string("<") + name + ">";
	return true;
}

// remove the name of an object from the list of supported objects; return false if not found
bool Model::removeObjectSupport(const char* name) { return modRef->_removeObjectSupport(name); }
bool Model::_removeObjectSupport(const char* name) {
	if(!this->_isSupportedObject( name ))
		return false;
		
	// get the start of the object
	string::size_type index = this->supportedObjects.find( string("<") + name + ">", 0 );
	
	// get the end of the object
	string::size_type end = this->supportedObjects.find( ">", index+1 );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = this->supportedObjects.substr(0, index);
	
	if(end < supportedObjects.size() - 1)
		endChunk = this->supportedObjects.substr(end + 1);
		
	// regroup the string
	this->supportedObjects = frontChunk + endChunk;
	
	return true;
	
}

// add support for an object hierarchy
bool Model::addSupportedHierarchy(const char* name) { return modRef->_addSupportedHierarchy(name); }
bool Model::_addSupportedHierarchy(const char* name) {
	if(name == NULL || _isSupportedHierarchy( name ))
		return false;
	
	this->objectHierarchy += name;
	return true;
}

// remove support for an object hierarchy
bool Model::removeSupportedHierarchy(const char* name) { return modRef->_removeSupportedHierarchy(name); }
bool Model::_removeSupportedHierarchy(const char* name) {
	if(!this->_isSupportedHierarchy( name ))
		return false;
		
	// get the start of the hierarchy
	string::size_type index = this->objectHierarchy.find( name, 0 );
	
	// get the end of the hierarchy
	string::size_type end = this->objectHierarchy.find( "<", index + strlen(name) );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = this->objectHierarchy.substr(0, index);
	
	if(end < objectHierarchy.size() - 1)
		endChunk = this->objectHierarchy.substr(end + 1);
		
	// regroup the string
	this->objectHierarchy = frontChunk + endChunk;
	
	return true;
}


// add support for an object terminator
bool Model::addTerminatorSupport(const char* name, const char* end) { return modRef->_addTerminatorSupport(name, end); }
bool Model::_addTerminatorSupport(const char* name, const char* end) {
	if(name == NULL || this->_isSupportedTerminator( name, end ))
		return false;
		
	this->objectTerminators += string("<") + name + "|" + end + ">";
	return true;
}

// remove support for an object terminator
bool Model::removeTerminatorSupport(const char* name, const char* end) { return modRef->_removeTerminatorSupport(name, end); }
bool Model::_removeTerminatorSupport(const char* name, const char* end) {
	if(!this->_isSupportedTerminator(name, end))
		return false;
		
	string term = string(name) + "|" + string(end);
	
	// get the start of the object
	string::size_type index = this->objectTerminators.find( string("<") + term + ">", 0 );
	
	// get the end of the object
	string::size_type endIndex = this->objectTerminators.find( ">", index+1 );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = this->objectTerminators.substr(0, index);
	
	if(endIndex < this->objectTerminators.size() - 1)
		endChunk = this->objectTerminators.substr(endIndex + 1);
		
	// regroup the string
	this->objectTerminators = frontChunk + endChunk;
	
	return true;
}

// the universal getter--returns the entire working model as a string of BZW-formatted text
string& Model::toString() { return modRef->_toString(); }
string& Model::_toString() {
	// iterate through all objects and have them print themselves out in string format
	static string ret = "";
	ret.clear();
	
	string worldDataString = (this->worldData != NULL ? this->worldData->toString() : "\n");
	string optionsDataString = (this->optionsData != NULL ? this->optionsData->toString() : "\n");
	string waterLevelString = (this->waterLevelData != NULL && this->waterLevelData->getHeight() > 0.0 ? this->waterLevelData->toString() : "\n");
	
	// global data
	ret += "\n#--World-----------------------------------------\n\n" + worldDataString;
	ret += "\n#--Options---------------------------------------\n\n" + optionsDataString;
	ret += "\n#--Water Level-----------------------------------\n\n" + waterLevelString;
	
	// physics drivers
	ret += "\n#--Physics Drivers-------------------------------\n\n";
	if(this->phys.size() > 0) {
		for(map< string, physics* >::iterator i = this->phys.begin(); i != this->phys.end(); i++) {
			ret += i->second->toString() + "\n";
		}
	}
	
	// materials
	ret += "\n#--Materials-------------------------------------\n\n";
	if(this->materials.size() > 0) {
		for(map< string, material* >::iterator i = this->materials.begin(); i != this->materials.end(); i++) {
			ret += i->second->toString() + "\n";
		}	
	}
	
	// dynamic colors
	ret += "\n#--Dynamic Colors--------------------------------\n\n";
	if(this->dynamicColors.size() > 0) {
		for(map< string, dynamicColor* >::iterator i = this->dynamicColors.begin(); i != this->dynamicColors.end(); i++) {
			ret += i->second->toString() + "\n";	
		}
	}
	
	// texture matrices
	ret += "\n#--Texture Matrices------------------------------\n\n";
	if(this->textureMatrices.size() > 0) {
		for(map< string, texturematrix* >::iterator i = this->textureMatrices.begin(); i != this->textureMatrices.end(); i++) {
			ret += i->second->toString() + "\n";	
		}
	}
	
	// group defintions
	ret += "\n#--Group Definitions-----------------------------\n\n";
	if(this->groups.size() > 0) {
		for(map< string, define* >::iterator i = this->groups.begin(); i != this->groups.end(); i++) {
			ret += i->second->toString() + "\n";	
		}	
	}
	
	// all other objects
	ret += "\n#--Objects---------------------------------------\n\n";
	if(this->objects.size() > 0) {
		for(objRefList::iterator i = this->objects.begin(); i != this->objects.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}	
	}
	
	// links
	ret += "\n#--Teleporter Links------------------------------\n\n";
	if(this->links.size() > 0) {
		for(map< string, Tlink* >::iterator i = this->links.begin(); i != this->links.end(); i++) {
			ret += i->second->toString() + "\n";	
		}	
	}
	
	// unused dats
	ret += "\n#--Unused Data-----------------------------------\n\n";
	if(this->unusedData.size() > 0) {
		for(vector<string>::iterator i = this->unusedData.begin(); i != this->unusedData.end(); i++) {
			ret += (*i) + "\n";
		}
	}
	
	return ret;
}

// BZWB-specific API
Model::objRefList& 				Model::getObjects() 		{ return modRef->_getObjects(); }
map< string, material* >& 		Model::getMaterials() 		{ return modRef->_getMaterials(); }
map< string, texturematrix* >&	Model::getTextureMatrices() { return modRef->_getTextureMatrices(); }
map< string, physics* >& 		Model::getPhysicsDrivers() 	{ return modRef->_getPhysicsDrivers(); }
map< string, Tlink* >&		 	Model::getTeleporterLinks() { return modRef->_getTeleporterLinks(); }
map< string, define* >&			Model::getGroups() 			{ return modRef->_getGroups(); }
void					Model::addObject( bz2object* obj ) { modRef->_addObject( obj ); }
void					Model::removeObject( bz2object* obj ) { modRef->_removeObject( obj ); }
void					Model::setSelected( bz2object* obj ) { modRef->_setSelected( obj ); }
void					Model::setUnselected( bz2object* obj ) { modRef->_setUnselected( obj ); }
void					Model::unselectAll() { modRef->_unselectAll(); }
bool					Model::isSelected( bz2object* obj ) { return modRef->_isSelected( obj ); }

// add an object to the Model
void Model::_addObject( bz2object* obj ) {
	if( obj == NULL )
		return;
		
	this->objects.push_back( obj );
	
	// tell all observers
	ObserverMessage obs( ObserverMessage::ADD_OBJECT, obj );
	this->notifyObservers( &obs );
}

// remove an object by instance
void Model::_removeObject( bz2object* obj ) {
	if(objects.size() <= 0)
		return;
	
	objRefList::iterator itr = objects.begin();
	for(unsigned int i = 0; i < this->objects.size() && itr != this->objects.end(); i++, itr++) {
		if( this->objects[i] == obj ) {
			
			ObserverMessage obs( ObserverMessage::REMOVE_OBJECT, obj );
			this->notifyObservers( &obs );
			objects.erase( itr );
			
			break;
		}
	}
}

// set an object as selected and update it
void Model::_setSelected( bz2object* obj ) {
	if( selectedObjects.size() < 0 )
		return;
		
	for(objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		if( *i == obj ) {
			return;		// this object is already selected.
		}	
	}	
	
	obj->setSelected( true );
	obj->setChanged( true );
	
	this->selectedObjects.push_back( obj );	
	
	// tell the view to mark this object as selected
	ObserverMessage obs_msg( ObserverMessage::UPDATE_OBJECT, obj );
	
	this->notifyObservers( &obs_msg );
}

// set an object as unselected and update it
void Model::_setUnselected( bz2object* obj ) {
	if( this->selectedObjects.size() < 0)
		return;
		
	for(objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		if( *i == obj ) {
			obj->setSelected( false );
			obj->setChanged( true );
			this->selectedObjects.erase(i);
			break;
		}	
	}
	
	// tell the view to mark this object as unselected
	ObserverMessage obs_msg( ObserverMessage::UPDATE_OBJECT, obj );
	
	this->notifyObservers( &obs_msg );
}

// determine whether or not an object is selected
bool Model::_isSelected( bz2object* obj ) {
	if( this->selectedObjects.size() < 0)
		return false;
	
	if(obj == NULL)
		return false;
	
	for(objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		if( *i == obj ) {
			return true;
		}	
	}
	
	return false;
}

// unselect all objects
void Model::_unselectAll() {
	if( this->selectedObjects.size() <= 0)
		return;
		
	for(objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		(*i)->setSelected( false );
		(*i)->setChanged( true );
		// tell the view to mark this object as unselected
		ObserverMessage obs_msg( ObserverMessage::UPDATE_OBJECT, i->get() );
		this->notifyObservers( &obs_msg );
	}
	
	selectedObjects.clear();
	this->notifyObservers( NULL );
	
}

// get selection
Model::objRefList& Model::getSelection() { return modRef->_getSelection(); }

// build an object from the object registry
DataEntry* Model::buildObject( const char* header ) { return modRef->_buildObject( header ); }
DataEntry* Model::_buildObject( const char* header ) {
	string name = string(header);
	
	if( this->cmap.find( name ) == this->cmap.end() )
		return NULL;
		
	string blank = "";
	return this->cmap[name](blank);
}

// cut objects from the scene
bool Model::cutSelection() { return modRef->_cutSelection(); }
bool Model::_cutSelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	this->objectBuffer.clear();
	
	// remove objects from the scene, but move them into the cut/copy buffer first so they're still referenced
	for( objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->objectBuffer.push_back( *i );
	}
	for( objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->_removeObject( i->get() );
	}
	
	this->selectedObjects.clear();
	this->notifyObservers( NULL );
	
	return true;
}

// copy objects from the scene
bool Model::copySelection() { return modRef->_copySelection(); }
bool Model::_copySelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	this->objectBuffer.clear();
	
	// copy objects into the object buffer.
	for( objRefList::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->objectBuffer.push_back( *i );
	}
	
	return true;
}

// paste the objectBuffer to the scene
bool Model::pasteSelection() { return modRef->_pasteSelection(); }
bool Model::_pasteSelection() {
	if( this->objectBuffer.size() <= 0)
		return false;
	
	
	this->_unselectAll();
	
	// paste objects into the scene
	// create new instances; don't pass references
	for( vector< osg::ref_ptr<bz2object> >::iterator i = this->objectBuffer.begin(); i != this->objectBuffer.end(); i++) {
		bz2object* obj = dynamic_cast< bz2object* > (this->_buildObject( (*i)->getHeader().c_str() ));
		if(!obj) {
			printf("error! could not create new instance of \"%s\"\n", (*i)->getHeader().c_str() );
			continue;
		}
		
		string data = (*i)->toString();
		obj->update( data );
		obj->setPos( obj->getPos() + osg::Vec3(10.0, 10.0, 0.0) );
		
		this->_addObject( obj );
		this->_setSelected( obj );
	}
	
	this->notifyObservers(NULL);
	
	return true;
}

// delete a selection
bool Model::deleteSelection() { return modRef->_deleteSelection(); }
bool Model::_deleteSelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	// remove objects from the scene WITHOUT first referencing it (i.e. this will ensure it gets deleted)
	Model::objRefList::iterator itr = this->selectedObjects.begin();
	for(; itr != this->selectedObjects.end(); ) {
		bz2object* obj = itr->get();
		this->_removeObject( obj );
		
		this->selectedObjects.erase( itr );
		itr = this->selectedObjects.begin();
	}
	
	this->selectedObjects.clear();
	
	this->notifyObservers(NULL);
	
	return true;
}

// make a new world
bool Model::newWorld() { return modRef->_newWorld(); }
bool Model::_newWorld() {
	// make a fake list of world objects (to be fed to Model::build() )
	vector< string > theNewWorld = vector< string >();
	
	// add a default new "options" object
	options newOptions = options();
	theNewWorld.push_back( newOptions.toString() );
	
	// add a default new "world" object
	world newWorld = world();
	theNewWorld.push_back( newWorld.toString() );
	
	// build a blank world
	this->_build( theNewWorld );
	
	return true;
}

// assign a material and make sure the Model has a reference to it
void Model::assignMaterial( const string& matref, bz2object* obj ) { modRef->_assignMaterial( matref, obj ); }
void Model::assignMaterial( material* matref, bz2object* obj ) { modRef->_assignMaterial( matref, obj ); }

void Model::_assignMaterial( const string& matref, bz2object* obj ) {
	material* mat;
	
	// do we have this material?
	if( this->materials.count( matref ) > 0 )
		mat = this->materials[matref];	// then load it from our mapping
	else
		mat = this->defaultMaterial.get();	// otherwise, use the default material
	
	// give the material to the object (and it will update itself)
	UpdateMessage msg( UpdateMessage::UPDATE_MATERIAL, mat );
	obj->update( msg );
}

void Model::_assignMaterial( material* matref, bz2object* obj ) {
	
	// if the material reference was NULL, just use the normal default material
	if( matref == NULL ) {
		printf(" NULL material, using defaults\n" );
		SceneBuilder::assignBZMaterial( defaultMaterial.get(), obj );
		return;
	}
	
	// otherwise, make sure this material exists (if not, then add it)
	if( this->materials.count( matref->getName() ) == 0 )
		materials[ matref->getName() ] = matref;
		
	// give the material to the object (and it will update itself)
	UpdateMessage msg( UpdateMessage::UPDATE_MATERIAL, matref );
	obj->update( msg );
}

// link two teleporters together if they are not explicitly linked
// return TRUE if a new link was created; FALSE if it already exists
bool Model::linkTeleporters( teleporter* from, teleporter* to ) {
	return modRef->_linkTeleporters( from, to );
}

bool Model::_linkTeleporters( teleporter* from, teleporter* to ) {
	// make sure that we don't already have a link
	if( this->links.size() > 0 ) {
		for( map< string, Tlink* >::iterator i = this->links.begin(); i != this->links.end(); i++ ) {
			if( (i->second->getTo() == from && i->second->getFrom() == to) ||
				(i->second->getTo() == to && i->second->getFrom() == from) )
					return false;
		}
	}
	
	// no such link exists; create it
	Tlink* newLink = new Tlink();
	string newLinkName = SceneBuilder::makeUniqueName("link");
	
	newLink->setName( newLinkName );
	newLink->setFrom( from );
	newLink->setTo( to );
	
	printf("  linked %s to %s\n", from->getName().c_str(), to->getName().c_str() );
	
	// add the link to the database
	this->links[ newLinkName ] = newLink;
	
	// tell the view to add it
	// ObserverMessage msg( ObserverMessage::ADD_OBJECT, newLink );
	// notifyObservers( &msg );
	
	return true;
}

ConfigurationDialog* Model::configureObject( DataEntry* d) { return modRef->_configureObject( d ); }
// configure an object
ConfigurationDialog* Model::_configureObject( DataEntry* d ) {
	if( d == NULL )
		return NULL;
		
	if( this->configMap.count( d->getHeader() ) == 0 )
		return NULL;
		
	if( this->configMap[ d->getHeader() ] == NULL )
		return NULL;
		
	return this->configMap[ d->getHeader() ](d);
}

// group objects together
void Model::groupObjects( vector< osg::ref_ptr< bz2object > >& objects ) { modRef->_groupObjects( objects ); }
void Model::_groupObjects( vector< osg::ref_ptr< bz2object > >& objects ) {
	// create a "define" object for these objects if one does not exist yet
	define* def = new define();
	if( !def || objects.size() == 0 )
		return;
		
	// unselect all objects
	for( vector< osg::ref_ptr< bz2object > >::iterator i = this->objects.begin(); i != this->objects.end(); i++) {
		this->_setUnselected( i->get() );
	}
	
	string defName = SceneBuilder::makeUniqueName("define");
	
	// assign the objects
	def->setObjects( objects );
	
	// set the name
	def->setName( defName );
	
	// make a group
	group* grp = new group();
	
	// assign it to the define (the group will re-name itself automatically)
	grp->setDefine( def );
	
	// add this group
	this->_addObject( grp );
	
	// add this definition
	groups[ defName ] = def;
	
	// remove all the objects within the passed vector (they are now part of the define)
	for( vector< osg::ref_ptr< bz2object > >::iterator i = this->objects.begin(); i != this->objects.end(); i++ ) {
		this->_removeObject( i->get() );
	}
	
	// set the group as selected
	this->_setSelected( grp );
}

// ungroup objects
void Model::ungroupObjects( group* g ) { modRef->_ungroupObjects( g ); }
void Model::_ungroupObjects( group* g ) {
	// get the objects from the group
	vector< osg::ref_ptr< bz2object > > objs = g->getDefine()->getObjects();
	
	// get the group's position so the objects can be translated to their current position in the group relative to the world
	osg::Vec3 p = g->getPos();
	
	if( objects.size() > 0 ) {
		for( vector< osg::ref_ptr< bz2object > >::iterator i = objs.begin(); i != objs.end(); i++ ) {
			(*i)->setPos( (*i)->getPos() + p );
			this->_addObject( i->get() );		// add the object
			this->_setSelected( i->get() );	// select the object
		}
	}
	
	// see if we need to remove the associated define
	bool noRefs = true;	// set to false if other groups are referencing this group's "define"
	string defName = g->getDefine()->getName();
	for( vector< osg::ref_ptr< bz2object > >::iterator i = this->objects.begin(); i != this->objects.end(); i++ ) {
		group* grp = dynamic_cast< group* > ( i->get() );
		if( !grp )
			continue;
		
		// if the associated "define" has the same name as the define from this group, then we don't erase it
		if( grp->getDefine()->getName() == defName ) {
			noRefs = false;
			break;
		}
	}
	
	// remove the group itself
	this->_removeObject( g );
	
	// if no references to the define were found, then remove this define
	if( noRefs ) {
		this->groups.erase( defName );
	}
	
}
