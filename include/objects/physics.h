#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../BZWParser.h"

class physics : public DataEntry {

public:

	physics() : DataEntry("physics", "<name><linear><angular><slide><death>") {
		name = string("");
		deathMessage = string("");
		slide = 0.0f;
		linear = Point3D(0.0f, 0.0f, 0.0f);
		angular = Point3D(0.0f, 0.0f, 0.0f);
	}
	
	physics(string& data) : DataEntry("physics", "<name><linear><angular><slide><death>", data.c_str()) {
		name = string("");
		deathMessage = string("");
		slide = 0.0f;
		linear = Point3D(0.0f, 0.0f, 0.0f);
		angular = Point3D(0.0f, 0.0f, 0.0f);
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		
		const char* header = this->getHeader().c_str();
		
		// get the chunk of string we need
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		if(!hasOnlyOne(lines, "physics"))
			return 0;
		
		const char* physicsData = lines[0].c_str();
		
		// get name
		vector<string> names = BZWParser::getValuesByKey("name", header, physicsData);
		if(!hasOnlyOne(names, "name"))
			return 0;
		
		// get linear velocity
		vector<string> linearVelocities = BZWParser::getValuesByKey("linear", header, physicsData);
		
		// get angular velocity
		vector<string> angularVelocities = BZWParser::getValuesByKey("angular", header, physicsData);
		
		// get slide
		vector<string> slides = BZWParser::getValuesByKey("slide", header, physicsData);
		
		// get death message
		vector<string> deathMessages = BZWParser::getValuesByKey("death", header, physicsData);
		
		// load the data in
		if(!DataEntry::update(data))
			return 0;
		this->name = names[0];
		this->linear = (linearVelocities.size() != 0 ? Point3D( linearVelocities[0].c_str() ) : Point3D( 0.0f, 0.0f, 0.0f ));
		this->angular = (angularVelocities.size() != 0 ? Point3D( angularVelocities[0].c_str() ): Point3D( 0.0f, 0.0f, 0.0f));
		this->slide = (slides.size() != 0 ? atof( slides[0].c_str() ) : 0.0f);
		
		if(deathMessages.size() >= 1)
			this->deathMessage = deathMessages[0];
			
		return 1;
	}
	
	// toString
	string toString(void) {
		return string("physics\n") +
					  "  name " + name + "\n" +
					  "  linear " + linear.toString() +
					  "  angular " + angular.toString() +
					  "  slide " + string(ftoa(slide)) + "\n" +
					  (deathMessage.length() != 0 ? "  death " : "# death") + deathMessage + "\n" +
					  this->getUnusedText() + 
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	
	Point3D linear, angular;
	float slide;
	string name, deathMessage;
};

#endif /*PHYSICS_H_*/
