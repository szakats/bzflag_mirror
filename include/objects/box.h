#ifndef BOX_H_
#define BOX_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../BZWParser.h"

/**
 * Box data (1.x box, not meshBox)
 */
class box : public DataEntry {

public:

	// constructors
	box() : DataEntry("box", "<position><rotation><size>") {
		this->position = Point3D(0.0, 0.0, 0.0);
		this->size = Point3D(10.0, 10.0, 10.0);
		this->rotation = 0.0f;
	}
	
	box(string& data) : DataEntry("box", "<position><rotation><size>") {
		this->update(data);	
	}
	
	// nothing to destroy...
	~box() { }
	
	// getter
	string get(void) {
		return this->toString();
	}
	
	// setter
	void update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the lines
		vector<string> boxSections = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// just go with the first box definition we find (only one should be passed anyway)
		const char* boxData = boxSections[0].c_str();
		
		// get the position
		vector<string> positions = BZWParser::getValuesByKey("position", header, boxData);
		
		// just go with the first position (only one should be defined)
		if(positions.size() > 1)
			printf("box::update():  Warning! Multiple positions defined; choosing the first one...\n");
		
		this->position = Point3D(positions[0].c_str());
		
		// get the size
		vector<string> sizes = BZWParser::getValuesByKey("size", header, boxData);
		
		// just go with the first size (only one should be defined)
		if(sizes.size() > 1)
			printf("box::update():  Warning! Multiple sizes defined; choosing the first one...\n");
			
		this->size = Point3D(sizes[0].c_str());
		
		// get the rotation
		vector<string> rotations = BZWParser::getValuesByKey("rotation", header, boxData);
		
		// just go with the first rotation
		if(rotations.size() > 1)
			printf("rotations::update():  Warning! Multiple rotations defined; choosing the first one...\n");
			
		this->rotation = atof( rotations[0].c_str() );
		
	}
	
	// toString
	string toString(void) {
		return string("box\n") +
					  "  position " + position.toString() +
					  "  rotation " + string(ftoa(rotation)) + "\n" +
					  "  size " + size.toString() +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;
	}
	
private:
	Point3D position;
	Point3D size;
	float rotation;
};

#endif /*BOX_H_*/
