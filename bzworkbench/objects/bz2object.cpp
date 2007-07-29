#include "../include/objects/bz2object.h"

// default constructor
bz2object::bz2object(const char* name, const char* keys):
	Renderable(),
	DataEntry(name, keys) {
		
	this->transformations = vector<BZTransform>();
	this->materials = vector<string>();
	this->setSelected( false );
	this->setName( "(unknown bz2object)" );
	
};

// constructor with data
bz2object::bz2object(const char* name, const char* keys, const char* data):
	Renderable(),
	DataEntry(name, keys, data)
	{
		
	this->transformations = vector<BZTransform>();
	this->materials = vector<string>();
	this->setSelected( false );
	string d = string(data);
	this->update(d);
	
}
	
// getter
string bz2object::get(void) { return this->toString(); }

// setter
int bz2object::update(string& data) {
	
	// find the first occurence of the section in the data
	const char* header = this->getHeader().c_str();

	// get the lines
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(BZW_NOT_FOUND == lines[0]) {
		printf("bz2object::update(): Error! Could not find object data!\n");
		return 0;
	}
		
	if(!hasOnlyOne(lines, header))
		return 0;
	
	// just go with the first box definition we find (only one should be passed anyway)
	const char* objData = lines[0].c_str();
	
	// data
	vector<string> names, positions, rotations, sizes, physicsDrivers, matrefs;
	
	// get the name (break if there are more than one)
	if(this->isKey("name")) {
		names = BZWParser::getValuesByKey("name", header, objData);
		if(names.size() > 1) {
			printf("%s::update():  Error! Defined \"name\" %d times!\n", header, names.size());
			return 0;	
		}
	}
	
	// get the position
	if(this->isKey("position")) {
		positions = BZWParser::getValuesByKey("position", header, objData);
		
		// just go with the first position (only one should be defined)
		if(positions.size() > 1) {
			printf("%s::update(): Error! Defined \"position\" %d times!\n", header, positions.size());
			return 0;
		}
		if(positions.size() == 0) {
			positions.push_back(Point3D(0.0f, 0.0f, 0.0f).toString());
		}
			
	}
	
	// get the rotation
	if(this->isKey("rotation")) {
		rotations = BZWParser::getValuesByKey("rotation", header, objData);
		
		// just go with the first rotation
		if(rotations.size() > 1) {
			printf("%s::update(): Error! Defined \"rotation\" %d times!\n", header, rotations.size());
			return 0;
		}
		if(rotations.size() == 0) {
			string rot = "0.0";
			rotations.push_back(rot);	
		}
			
	}
	
	if(this->isKey("size")) {
		// get the size
		sizes = BZWParser::getValuesByKey("size", header, objData);
		
		// just go with the first size (only one should be defined)
		if(sizes.size() > 1) {
			printf("%s::update(): Error! Defined \"size\" %d times!\n", header, sizes.size());
			return 0;
		}
		if(sizes.size() == 0) {
			sizes.push_back(Point3D(-1.0f, -1.0f, -1.0f).toString());	
		}
		
	}
	// read in the transformations
	transformations.clear();
	
	// find out which transformations are valid
	vector<string> transformKeys = vector<string>();
	if(this->isKey("shear"))
		transformKeys.push_back(string("shear"));
	if(this->isKey("shift"))
		transformKeys.push_back(string("shift"));
	if(this->isKey("scale"))
		transformKeys.push_back(string("scale"));
	if(this->isKey("spin"))
		transformKeys.push_back(string("spin"));
		
	// get the transformation occurences
	if(transformKeys.size() > 0) {
		vector<string> transforms = BZWParser::getLinesByKeys(transformKeys, header, data.c_str());
		for(vector<string>::iterator i = transforms.begin(); i != transforms.end(); i++) {
			transformations.push_back( BZTransform(*i) );	
		}
	}
	
	// get the physics driver
	if(this->isKey("phydrv")) {
		physicsDrivers = BZWParser::getValuesByKey("phydrv", header, objData);
		if(physicsDrivers.size() > 1) {
			printf("%s::update(): Error! Defined \"phydrv\" %d times!\n", header, physicsDrivers.size());
			return 0;
		}
	}
	
	// get materials
	if(this->isKey("matref")) {
		matrefs = BZWParser::getValuesByKey("matref", header, objData);
	}
	
	// load in the data	
	if(this->isKey("name") && names.size() > 0)
		this->setName( names[0] );
	if(this->isKey("position") && positions.size() > 0)
		this->setPosition( Point3D( positions[0].c_str() ) );
	if(this->isKey("rotation") && rotations.size() > 0)
		this->setRotationZ( atof( rotations[0].c_str() ) );
	if(this->isKey("size") && sizes.size() > 0)
		this->setSize( Point3D( sizes[0].c_str() ) );
	if(this->isKey("phydrv") && physicsDrivers.size() > 0)
		this->physicsDriver = physicsDrivers[0];
	if(this->isKey("matref") && matrefs.size() > 0)
		this->materials = matrefs;
		
	return DataEntry::update(data);
}

// toString
string bz2object::toString(void) {
	return this->getHeader() + "\n" + this->BZWLines() + "end\n";
}

// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
string bz2object::BZWLines(void) {
	string ret = string("");
	
	// add name key/value to the string if supported
	if(this->isKey("name") && this->getName().length() > 0)
		ret += "  name " + this->getName() + "\n";
	
	// add position key/value to the string if supported
	if(this->isKey("position"))
		ret += "  position " + Point3D( this->getPosition() ).toString();
		
	// add size key/value to the string if supported
	if(this->isKey("size"))
		ret += "  size " + Point3D( this->getSize() ).toString();
	
	// add rotation key/value to the string if supported
	if(this->isKey("rotation"))
		ret += "  rotation " + string( ftoa(this->getRotation().z()) ) + "\n";
	
	// add all transformations to the string if they are supported
	for(vector<BZTransform>::iterator i = transformations.begin(); i != transformations.end(); i++) {
		if(this->isKey(i->getHeader().c_str()))
			ret += "  " + i->toString();
	}
	
	// add phydrv key/value to the string if supported and if defined
	if(this->isKey("phydrv") && physicsDriver.length() != 0)
		ret += "  phydrv " + physicsDriver + "\n";
	
	// add all matref key/value pairs to the string if supported and defined
	if(this->isKey("matref") && materials.size() != 0) {
		for(vector<string>::iterator i = materials.begin(); i != materials.end(); i++) {
			ret += "  matref " + (*i) + "\n";
		}	
	}
	
	// add unused text
	ret += this->getUnusedText();
	
	return ret;
}
