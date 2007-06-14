#ifndef LOD_H_
#define LOD_H_

#include "DataEntry.h"
#include "model/BZWParser.h"
#include "LODCommand.h"

#include <string>
#include <vector>

class LOD : public DataEntry {

public:

	// default constructor
	LOD() : DataEntry("lod", "<matref><lengthPerPixel>") {
		
	}
	
	// constructor with data
	LOD(string& data) : DataEntry("lod", "<matref><lengthPerPixel>", data.c_str()) {
		
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		
	}
	
	// toString
	string toString(void) {
		
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	
	vector<LODCommand> commands;
	int pixelLength;
};

#endif /*LOD_H_*/
