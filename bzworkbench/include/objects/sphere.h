#ifndef SPHERE_H_
#define SPHERE_H_

#include "bz2object.h"

class sphere : public bz2object {
public:
	sphere() : 
		bz2object("sphere", "<position><rotation><size><radius><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
		
		// define some basic values
		this->divisions = 16;
		this->name = string("default_sphere");
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
	}
	
	sphere(string& data) :
		bz2object("sphere", "<position><rotation><size><radius><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
		
		// define some basic values
		this->divisions = 16;
		this->name = string("default_sphere");
		this->physicsDriver = string("");
		flatShading = false;
		smoothbounce = true;
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		// get the chunk we need
		
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// check and see if the proper data segment was found
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		if(!hasOnlyOne(lines, "sphere"))
			return 0;
		const char* sphereData = lines[0].c_str();
			
		// get the name
		vector<string> names = BZWParser::getValuesByKey("name", header, sphereData);
		if(!hasOnlyOne(names, "name"))
			return 0;
			
		// get the divisions
		vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, sphereData);
		if(!hasOnlyOne(vDivisions, "divisions"))
			return 0;
			
		// get the radius
		vector<string> radii = BZWParser::getValuesByKey("radius", header, sphereData);
		if(radii.size() > 1) {
			printf("sphere::update(): Error! Defined \"radius\" %d times!\n", radii.size());
			return 0;
		}
			
		// get flatshading
		vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, sphereData);
		
		// get smoothbounce
		vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, sphereData);
		
		// Before we call the superclass's update method, we need to see if "size" is defined.
		// It's perfectly legal to not define a size but to define a radius; if we have a radius,
		// we can add a size and forget the radius.
		vector<string> sizes = BZWParser::getValuesByKey("size", header, sphereData);
		if(sizes.size() < 1 && radii.size() >= 1) {
			// encapsulate the sphere data inside a string
			string sphData = string(sphereData);
			
			// find the line after the header	
			string::size_type index = sphData.find("\n", 0);
			
			// create an appropriate size string
			string size = string("  size ") + radii[0] + " " + radii[0] + " " + radii[0]+ "\n";
			
			// insert the size string into the object data
			sphData.insert(index+1, size.c_str());
			
			// make data the new sphere data
			data = sphData;
		}
		
		// do base class update
		if(!bz2object::update(data))
			return 0;
		
		// set the data
		this->name = names[0];
		this->divisions = atoi( vDivisions[0].c_str() );
		this->flatShading = (flatShadings.size() == 0 ? false : true);
		this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
		
		return 1;
	}
	
	// toString
	string toString(void) {
		return string("sphere\n") +
					  this->BZWLines() +
					  "  divisions " + string(itoa(divisions)) + "\n" +
					  (flatShading == true ? "  flatshading\n" : "") +
					  (smoothbounce == true ? "  smoothbounce\n" : "") + 
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	bool flatShading, smoothbounce;
	int divisions;
};

#endif /*SPHERE_H_*/
