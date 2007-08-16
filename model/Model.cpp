#include "../include/model/Model.h"
#include "../include/windows/View.h"

// global reference to the model so the static call will work
Model* modelRef;

Model::Model() : Observable()
{
	worldData = new world();
	optionsData = new options();
	waterLevelData = new waterLevel();
	phys = map< string, physics* >();
	dynamicColors = map< string, dynamicColor* >();
	materials = map< string, material* >();
	links = map< string, Tlink* >();
	textureMatrices = map< string, texturematrix* >();
	groups = map< string, define* >();
	
	// make a default material
	defaultMaterial = new material();
	defaultMaterial->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	defaultMaterial->setDiffuse( osg::Vec4( 1, 1.0, 1.0, 1.0 ) );
	defaultMaterial->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0) );
	defaultMaterial->setEmissive( osg::Vec4( 0.0, 0.0, 0.0, 1.0) );
	
	vector< osg::ref_ptr<osg::Texture2D> > defaultTextures = vector< osg::ref_ptr< osg::Texture2D > >();
	defaultTextures.push_back( SceneBuilder::buildTexture2D( "share/tetrawall.png" ) );
	
	defaultMaterial->setTextures( defaultTextures );
	
	objects = vector< bz2object* >();
	modelRef = this;
	
	cmap = map<string, DataEntry* (*)(string&)>();
	
	this->supportedObjects = string("");
	this->objectHierarchy = string("");
	this->objectTerminators = string("");
	
	unusedData = vector<string>();
	
}

// constructor that takes information about which objects to support
Model::Model(const char* supportedObjects, const char* objectHierarchy, const char* objectTerminators) : Observable() {
	worldData = new world();
	optionsData = new options();
	waterLevelData = new waterLevel();
	phys = map< string, physics* >();
	dynamicColors = map< string, dynamicColor* >();
	materials = map< string, material* >();
	links = map< string, Tlink* >();
	textureMatrices = map< string, texturematrix* >();
	groups = map< string, define* >();
	
	// make a default material
	defaultMaterial = new material();
	defaultMaterial->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	defaultMaterial->setDiffuse( osg::Vec4( 1, 1.0, 1.0, 1.0 ) );
	defaultMaterial->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0) );
	defaultMaterial->setEmissive( osg::Vec4( 0.0, 0.0, 0.0, 1.0) );
	
	vector< osg::ref_ptr<osg::Texture2D> > defaultTextures = vector< osg::ref_ptr< osg::Texture2D > >();
	defaultTextures.push_back( SceneBuilder::buildTexture2D( "share/tetrawall.png" ) );
	
	defaultMaterial->setTextures( defaultTextures );
	
	objects = vector< bz2object* >();
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
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++)
			if((*i)) {
				delete *i;
				*i = NULL;	
			}
	
	if(groups.size() > 0)
		for(map<string, define*>::iterator i = groups.begin(); i != groups.end(); i++)
			if((i->second)) {
				delete i->second;
				i->second = NULL;	
			}
		
}

// getters specific to the model
const string Model::getSupportedObjects() { return modelRef->_getSupportedObjects(); }
const string Model::_getSupportedObjects() { return this->supportedObjects; }

const string Model::getSupportedHierarchies() { return modelRef->_getSupportedHierarchies(); }
const string Model::_getSupportedHierarchies() { return this->objectHierarchy; }

const string Model::getSupportedTerminators() { return modelRef->_getSupportedTerminators(); }
const string Model::_getSupportedTerminators() { return this->objectTerminators; }

// the query method
DataEntry* Model::command(const string& command, const string& object, const string& name, const string& data) { 
	return modelRef->_command(command, object, name, data);
}

// the *real* query method
// TODO: add more features
DataEntry* Model::_command(const string& command, const string& object, const string& name, const string& data) {
	// see if this is a "get" command
	if( command == MODEL_GET ) {
		// determine which type of object
		
		// handle dynamicColor
		if( object == "dynamicColor" ) {
			return ( dynamicColors.count( name ) > 0 ? dynamicColors[name] : NULL );
		}
		
		// handle texturematrices
		else if( object == "texturematrix" ) {
			return ( textureMatrices.count( name ) > 0 ? textureMatrices[name] : NULL );
		}
		
		// handle physics drivers
		else if( object == "phydrv" ) {
			return ( phys.count( name ) > 0 ? phys[name] : NULL );
		}
		
		// handle materials
		else if( object == "material" ) {
			return (materials.count( name ) > 0 ? materials[name] : NULL );
		}
		
		// handle teleporter links
		else if( object == "link" ) {
			return (links.count( name ) > 0 ? links[name] : NULL );
		}
		
		// handle definitions
		else if( object == "define" ) {
			return (groups.count( name ) > 0 ? groups[name] : NULL );
		}
		
		// handle all other objects
		else {
			for( vector< bz2object* >::iterator i = this->objects.begin(); i != this->objects.end(); i++) {
				if( (*i)->getName() == name )
					return *i;
			}
		}
	}
	
	return NULL;
}

// the static build method
bool Model::build(vector<string>& bzworld) { return modelRef->_build(bzworld); }

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
		vector< bz2object* >::iterator itr = objects.begin();
		while( itr != objects.end() ) {
			
			ObserverMessage obs( ObserverMessage::REMOVE_OBJECT, *itr );
			notifyObservers( &obs );
			
			this->objects.erase( itr );
			itr = objects.begin();
			
		}
	}
	this->objects.clear();
	notifyObservers( NULL );
	
	// load the data in
	bool foundWorld = false;
	for(vector<string>::iterator i = bzworld.begin(); i != bzworld.end(); i++) {
		string header = BZWParser::headerOf(i->c_str());
		if(header == "world") {
			if(cmap[header] != NULL) {
				this->worldData->update(*i);
				foundWorld = true;		// there must be a world
				// tell the observers we have a different world
				ObserverMessage obs( ObserverMessage::UPDATE_WORLD, this->worldData );
				notifyObservers( &obs );
				continue;
			}
			else {
				printf("Model::build(world): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse waterLevel
		else if(header == "waterLevel") {
			if(cmap[header] != NULL) {
				this->waterLevelData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(waterLevel): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse options
		else if(header == "options") {
			if(cmap[header] != NULL) {
				this->optionsData->update(*i);	
				continue;
			}
			else {
				printf("Model::build(options): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse materials
		else if(header == "material") {
			if(cmap[header] != NULL) {
				material* mat = ((material*)cmap[header](*i));
				if( mat ) {
					materials[ mat->getName() ] = mat;
				}
			}
			else {
				printf("Model::build(material): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse physics
		else if(header == "physics") {
			if(cmap[header] != NULL) {
				physics* p = (physics*)cmap[header](*i);
				if( p ) {
					phys[ p->getName() ] = p;
				}
			}
			else {
				printf("Model::build(physics): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse dynamicColors
		else if(header == "dynamicColor") {
			if(cmap[header] != NULL) {
				dynamicColor* dynCol = (dynamicColor*)cmap[header](*i);
				if( dynCol ) {
					dynamicColors[ dynCol->getName() ] = dynCol;
				}
			}
			else {
				printf("Model::build(dynamicColor): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse group definitions
		else if(header == "define") {
			if(cmap[header] != NULL) {
				define* def = (define*)cmap[header](*i);
				if( def ) {
					groups[ def->getName() ] = def;
				}
			}
			else {
				printf("Model::build(define): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse links
		else if(header == "link") {
			if(cmap[header] != NULL) {
				Tlink* link = (Tlink*)cmap[header](*i);
				if( link ) {
					links[ link->getName() ] = link;
				}
			}
			else {
				printf("Model::build(link): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse texturematrices
		else if(header == "texturematrix") {
			if(cmap[header] != NULL) {
				texturematrix* tm = (texturematrix*)cmap[header](*i);
				if( tm ) {
					textureMatrices[ tm->getName() ] = tm;
				}
			}	
			else {
				printf("Model::build(texturematrix): Skipping undefined object \"%s\"\n", header.c_str());
				unusedData.push_back(*i);
			}
		}
		
		// parse all other objects
		else {
			if(cmap[header] != NULL) {
				this->_addObject((bz2object*)cmap[header](*i));
			}
			else {
				printf("Model::build(): Skipping undefined object \"%s\"\n", header.c_str());
				printf("data: \n%s\n", i->c_str());
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
 * methods to manipulate Model::supportedObjects, Model::objectTerminators, and Model::objectHierarchy
 * 
 **************************/
 
// add the name of an object to the list of supported objects; return false if it's already there
bool Model::addObjectSupport(const char* name) { return modelRef->_addObjectSupport(name); }
bool Model::_addObjectSupport(const char* name) {
	if(name == NULL || this->_isSupportedObject( name ))
		return false;
		
	this->supportedObjects += string("<") + name + ">";
	return true;
}

// remove the name of an object from the list of supported objects; return false if not found
bool Model::removeObjectSupport(const char* name) { return modelRef->_removeObjectSupport(name); }
bool Model::_removeObjectSupport(const char* name) {
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
bool Model::addSupportedHierarchy(const char* name) { return modelRef->_addSupportedHierarchy(name); }
bool Model::_addSupportedHierarchy(const char* name) {
	if(name == NULL || this->_isSupportedHierarchy( name ))
		return false;
	
	this->objectHierarchy += name;
	return true;
}

// remove support for an object hierarchy
bool Model::removeSupportedHierarchy(const char* name) { return modelRef->_removeSupportedHierarchy(name); }
bool Model::_removeSupportedHierarchy(const char* name) {
	if(!this->_isSupportedHierarchy( name ))
		return false;
		
	// get the start of the hierarchy
	string::size_type index = objectHierarchy.find( name, 0 );
	
	// get the end of the hierarchy
	string::size_type end = objectHierarchy.find( "<", index + strlen(name) );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = objectHierarchy.substr(0, index);
	
	if(end < objectHierarchy.size() - 1)
		endChunk = objectHierarchy.substr(end + 1);
		
	// regroup the string
	objectHierarchy = frontChunk + endChunk;
	
	return true;
}


// add support for an object terminator
bool Model::addTerminatorSupport(const char* name, const char* end) { return modelRef->_addTerminatorSupport(name, end); }
bool Model::_addTerminatorSupport(const char* name, const char* end) {
	if(name == NULL || this->_isSupportedTerminator( name, end ))
		return false;
		
	this->objectTerminators += string("<") + name + "|" + end + ">";
	return true;
}

// remove support for an object terminator
bool Model::removeTerminatorSupport(const char* name, const char* end) { return modelRef->_removeTerminatorSupport(name, end); }
bool Model::_removeTerminatorSupport(const char* name, const char* end) {
	if(!this->_isSupportedTerminator(name, end))
		return false;
		
	string term = string(name) + "|" + string(end);
	
	// get the start of the object
	string::size_type index = objectTerminators.find( string("<") + term + ">", 0 );
	
	// get the end of the object
	string::size_type endIndex = objectTerminators.find( ">", index+1 );
	
	// cut out the element
	string frontChunk = "", endChunk = "";
	
	if(index > 0)
		frontChunk = objectTerminators.substr(0, index);
	
	if(endIndex < objectTerminators.size() - 1)
		endChunk = objectTerminators.substr(endIndex + 1);
		
	// regroup the string
	objectTerminators = frontChunk + endChunk;
	
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
	string waterLevelString = (waterLevelData != NULL && waterLevelData->getHeight() > 0.0 ? waterLevelData->toString() : "\n");
	
	// global data
	ret += "\n#--World-----------------------------------------\n\n" + worldDataString;
	ret += "\n#--Options---------------------------------------\n\n" + optionsDataString;
	ret += "\n#--Water Level-----------------------------------\n\n" + waterLevelString;
	
	// physics drivers
	ret += "\n#--Physics Drivers-------------------------------\n\n";
	if(phys.size() > 0) {
		for(map< string, physics* >::iterator i = phys.begin(); i != phys.end(); i++) {
			ret += i->second->toString() + "\n";
		}
	}
	
	// materials
	ret += "\n#--Materials-------------------------------------\n\n";
	if(materials.size() > 0) {
		for(map< string, material* >::iterator i = materials.begin(); i != materials.end(); i++) {
			ret += i->second->toString() + "\n";
		}	
	}
	
	// dynamic colors
	ret += "\n#--Dynamic Colors--------------------------------\n\n";
	if(dynamicColors.size() > 0) {
		for(map< string, dynamicColor* >::iterator i = dynamicColors.begin(); i != dynamicColors.end(); i++) {
			ret += i->second->toString() + "\n";	
		}
	}
	
	// texture matrices
	ret += "\n#--Texture Matrices------------------------------\n\n";
	if(textureMatrices.size() > 0) {
		for(map< string, texturematrix* >::iterator i = textureMatrices.begin(); i != textureMatrices.end(); i++) {
			ret += i->second->toString() + "\n";	
		}
	}
	
	// group defintions
	ret += "\n#--Group Definitions-----------------------------\n\n";
	if(groups.size() > 0) {
		for(map< string, define* >::iterator i = groups.begin(); i != groups.end(); i++) {
			ret += i->second->toString() + "\n";	
		}	
	}
	
	// all other objects
	ret += "\n#--Objects---------------------------------------\n\n";
	if(objects.size() > 0) {
		for(vector< bz2object* >::iterator i = objects.begin(); i != objects.end(); i++) {
			ret += (*i)->toString() + "\n";	
		}	
	}
	
	// links
	ret += "\n#--Teleporter Links------------------------------\n\n";
	if(links.size() > 0) {
		for(map< string, Tlink* >::iterator i = links.begin(); i != links.end(); i++) {
			ret += i->second->toString() + "\n";	
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

// BZWB-specific API
vector< bz2object* >& 		Model::getObjects() 		{ return modelRef->_getObjects(); }
map< string, material* >& 		Model::getMaterials() 		{ return modelRef->_getMaterials(); }
map< string, texturematrix* >&	Model::getTextureMatrices() { return modelRef->_getTextureMatrices(); }
map< string, physics* >& 		Model::getPhysicsDrivers() 	{ return modelRef->_getPhysicsDrivers(); }
map< string, Tlink* >&		 	Model::getTeleporterLinks() { return modelRef->_getTeleporterLinks(); }
map< string, define* >&			Model::getGroups() 			{ return modelRef->_getGroups(); }
void					Model::addObject( bz2object* obj ) { modelRef->_addObject( obj ); }
void					Model::removeObject( bz2object* obj ) { modelRef->_removeObject( obj ); }
void					Model::setSelected( bz2object* obj ) { modelRef->_setSelected( obj ); }
void					Model::setUnselected( bz2object* obj ) { modelRef->_setUnselected( obj ); }
void					Model::unselectAll() { modelRef->_unselectAll(); }
bool					Model::isSelected( bz2object* obj ) { return modelRef->_isSelected( obj ); }

// add an object to the Model
void Model::_addObject( bz2object* obj ) {
	if( obj == NULL )
		return;
		
	this->objects.push_back( obj );
	
	// tell all observers
	ObserverMessage obs( ObserverMessage::ADD_OBJECT, obj );
	notifyObservers( &obs );
}

// remove an object by instance
void Model::_removeObject( bz2object* obj ) {
	if(this->objects.size() <= 0)
		return;
	
	vector< bz2object* >::iterator itr = objects.begin();
	for(unsigned int i = 0; i < this->objects.size() && itr != this->objects.end(); i++, itr++) {
		if( objects[i] == obj ) {
			
			ObserverMessage obs( ObserverMessage::REMOVE_OBJECT, obj );
			notifyObservers( &obs );
			this->objects.erase( itr );
			
			break;
		}
	}
}

// set an object as selected and update it
void Model::_setSelected( bz2object* obj ) {
	if( this->selectedObjects.size() < 0 )
		return;
		
	for(vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
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
		
	for(vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
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
	
	for(vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
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
		
	for(vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		(*i)->setSelected( false );
		(*i)->setChanged( true );
		// tell the view to mark this object as unselected
		ObserverMessage obs_msg( ObserverMessage::UPDATE_OBJECT, *i );
		this->notifyObservers( &obs_msg );
	}
	
	selectedObjects.clear();
	this->notifyObservers( NULL );
	
}

// get selection
vector< bz2object* >& Model::getSelection() { return modelRef->_getSelection(); }

// build an object from the object registry
DataEntry* Model::buildObject( const char* header ) { return modelRef->_buildObject( header ); }
DataEntry* Model::_buildObject( const char* header ) {
	string name = string(header);
	
	if( this->cmap.count( name ) <= 0 )
		return NULL;
		
	string blank = "";
	return cmap[name](blank);
}

// cut objects from the scene
bool Model::cutSelection() { return modelRef->_cutSelection(); }
bool Model::_cutSelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	this->objectBuffer.clear();
	
	// remove objects from the scene, but move them into the cut/copy buffer first so they're still referenced
	for( vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->objectBuffer.push_back( *i );
	}
	for( vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->_removeObject( *i );
	}
	
	this->selectedObjects.clear();
	this->notifyObservers( NULL );
	
	return true;
}

// copy objects from the scene
bool Model::copySelection() { return modelRef->_copySelection(); }
bool Model::_copySelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	this->objectBuffer.clear();
	
	// copy objects into the object buffer.
	for( vector< bz2object* >::iterator i = this->selectedObjects.begin(); i != this->selectedObjects.end(); i++) {
		this->objectBuffer.push_back( *i );
	}
	
	return true;
}

// paste the objectBuffer to the scene
bool Model::pasteSelection() { return modelRef->_pasteSelection(); }
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
		obj->setPos( osg::Vec3(0.0, 0.0, 0.0) );
		
		this->_addObject( obj );
		this->_setSelected( obj );
	}
	
	this->notifyObservers(NULL);
	
	return true;
}

// delete a selection
bool Model::deleteSelection() { return modelRef->_deleteSelection(); }
bool Model::_deleteSelection() {
	if( this->selectedObjects.size() <= 0)
		return false;
	
	// remove objects from the scene WITHOUT first referencing it (i.e. this will ensure it gets deleted)
	vector<bz2object*>::iterator itr = selectedObjects.begin();
	for(; itr != selectedObjects.end(); ) {
		bz2object* obj = *itr;
		this->_removeObject( obj );
		
		selectedObjects.erase( itr );
		itr = selectedObjects.begin();
	}
	
	this->selectedObjects.clear();
	
	this->notifyObservers(NULL);
	
	return true;
}

// make a new world
bool Model::newWorld() { return modelRef->_newWorld(); }
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
void Model::assignMaterial( const string& matref, bz2object* obj ) { modelRef->_assignMaterial( matref, obj ); }
void Model::assignMaterial( material* matref, bz2object* obj ) { modelRef->_assignMaterial( matref, obj ); }

void Model::_assignMaterial( const string& matref, bz2object* obj ) {
	material* mat;
	
	// do we have this material?
	if( materials.count( matref ) > 0 )
		mat = materials[matref];	// then load it from our mapping
	else
		mat = defaultMaterial.get();	// otherwise, use the default material
	
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
	if( materials.count( matref->getName() ) == 0 )
		materials[ matref->getName() ] = matref;
		
	// give the material to the object (and it will update itself)
	UpdateMessage msg( UpdateMessage::UPDATE_MATERIAL, matref );
	obj->update( msg );
}

// link two teleporters together if they are not explicitly linked
// return TRUE if a new link was created; FALSE if it already exists
bool Model::linkTeleporters( teleporter* from, teleporter* to ) {
	return modelRef->_linkTeleporters( from, to );
}

bool Model::_linkTeleporters( teleporter* from, teleporter* to ) {
	// make sure that we don't already have a link
	if( links.size() > 0 ) {
		for( map< string, Tlink* >::iterator i = links.begin(); i != links.end(); i++ ) {
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
	links[ newLinkName ] = newLink;
	
	// tell the view to add it
	// ObserverMessage msg( ObserverMessage::ADD_OBJECT, newLink );
	// notifyObservers( &msg );
	
	return true;
}

