#ifndef BZ2OBJECT_H_
#define BZ2OBJECT_H_

#include "../DataEntry.h"
#include "../Transform.h"
#include "../render/Point3D.h"

#include <vector>
#include <string>

using namespace std;

/**
 * This is the base class of a BZW renderable object.  All of these support position, rotation, size,
 * and sets of shear, shift, scale, and spin key/value pairs.  box and pyramid are derived classes of this, 
 * but simply do not support transformations
 */

class bz2object : public DataEntry {
	
	public:
	
		bz2object(const char* name, const char* keys) : DataEntry(name, keys) {
			this->position = Point3D(0.0f, 0.0f, 0.0f);
			this->rotation = 0.0f;
			this->transformations = vector<Transform>();
			this->originalData = string("");
		};
		
		bz2object(const char* name, const char* keys, string& data) : DataEntry(name, keys) {
			this->position = Point3D(0.0f, 0.0f, 0.0f);
			this->rotation = 0.0f;
			this->transformations = vector<Transform>();
			this->originalData = data;
		}
		
		// getter
		string get(void) { return this->toString(); }
		
		// setter
		virtual void update(string& data) {
			// find the first occurence of the section in the data
			const char* header = this->getHeader().c_str();
		
			// get the lines
			vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
			
			// just go with the first box definition we find (only one should be passed anyway)
			const char* objData = lines[0].c_str();
			
			// get the position
			vector<string> positions = BZWParser::getValuesByKey("position", header, objData);
			
			// just go with the first position (only one should be defined)
			if(positions.size() > 1)
				printf("%s::update():  Warning! Multiple positions defined; choosing the first one...\n", header);
			
			this->position = Point3D(positions[0].c_str());
			
			// get the rotation
			vector<string> rotations = BZWParser::getValuesByKey("rotation", header, objData);
			
			// just go with the first rotation
			if(rotations.size() > 1)
				printf("%s::update():  Warning! Multiple rotations defined; choosing the first one...\n", header);
				
			this->rotation = atof( rotations[0].c_str() );
			
			// get the size
			vector<string> sizes = BZWParser::getValuesByKey("size", header, objData);
			
			// just go with the first size (only one should be defined)
			if(sizes.size() > 1)
				printf("%s::update():  Warning! Multiple sizes defined; choosing the first one...\n", header);
				
			this->size = Point3D(sizes[0].c_str());
			
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
					transformations.push_back( Transform(*i) );	
				}
			}
			
		}
		
		// toString
		virtual string toString(void) {
			return this->getHeader() + "\n" + this->BZWLines() + "end\n";
		}
		
		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		string BZWLines(void) {
			string ret = string("");
			
			ret += "  position " + position.toString();
			ret += "  size " + size.toString();
			ret += "  rotation " + string( ftoa(rotation) ) + "\n";
			
			for(vector<Transform>::iterator i = transformations.begin(); i != transformations.end(); i++) {
				ret += "  " + i->toString();
			}
			
			return ret;
		}
		
		// data getters (makes MasterConfigurationDialog code easier)
		Point3D getPosition() { return position; }
		Point3D getSize() { return size; }
		float getRotation() { return rotation; }
		vector<Transform> getTransformations() { return transformations; }
		
		// data setters (makes MasterConfigurationDialog code easier)
		
	
	
	private:
		Point3D position;
		Point3D size;
		float rotation;
		string originalData;
		
		vector<Transform> transformations;
};


#endif /*BZ2OBJECT_H_*/
