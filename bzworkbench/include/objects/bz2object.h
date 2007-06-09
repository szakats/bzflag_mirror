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
			
			if(lines[0] == BZW_NOT_FOUND)
				return;
			
			// just go with the first box definition we find (only one should be passed anyway)
			const char* objData = lines[0].c_str();
			
			// data
			vector<string> positions, rotations, sizes;
			
			// get the position
			if(this->isKey("position")) {
				positions = BZWParser::getValuesByKey("position", header, objData);
				
				// just go with the first position (only one should be defined)
				if(!hasOnlyOne(positions, "position"))
					return;
					
			}
			
			// get the rotation
			if(this->isKey("rotation")) {
				rotations = BZWParser::getValuesByKey("rotation", header, objData);
				
				// just go with the first rotation
				if(!hasOnlyOne(rotations, "rotation"))
					return;
					
			}
			
			if(this->isKey("size")) {
				// get the size
				sizes = BZWParser::getValuesByKey("size", header, objData);
				
				// just go with the first size (only one should be defined)
				if(!hasOnlyOne(sizes, "size"))
					return;
					
				
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
					transformations.push_back( Transform(*i) );	
				}
			}
			
			if(this->isKey("position"))
				this->position = Point3D( positions[0].c_str() );
			if(this->isKey("rotation"))
				this->rotation = atof( rotations[0].c_str() );
			if(this->isKey("size"))
				this->size = Point3D( sizes[0].c_str() );
		}
		
		// toString
		virtual string toString(void) {
			return this->getHeader() + "\n" + this->BZWLines() + "end\n";
		}
		
		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		string BZWLines(void) {
			string ret = string("");
			
			if(this->isKey("position"))
				ret += "  position " + position.toString();
				
			if(this->isKey("size"))
				ret += "  size " + size.toString();
			
			if(this->isKey("rotation"))
				ret += "  rotation " + string( ftoa(rotation) ) + "\n";
			
			for(vector<Transform>::iterator i = transformations.begin(); i != transformations.end(); i++) {
				if(this->isKey(i->getHeader().c_str()))
					ret += "  " + i->toString();
			}
			
			return ret;
		}
		
		// data getters (makes MasterConfigurationDialog code easier)
		Point3D getPosition() { return position; }
		Point3D getSize() { return size; }
		float getRotation() { return rotation; }
		vector<Transform> getTransformations() { return transformations; }
		string getOriginalData() { return originalData; }
		
		// data setters (makes MasterConfigurationDialog code easier)
		
	protected:
		string originalData;
	
	private:
		Point3D position;
		Point3D size;
		float rotation;
		
		vector<Transform> transformations;
};


#endif /*BZ2OBJECT_H_*/
