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
		commands = vector<LODCommand>();
		matref = string("");
		pixelLength = 0;
	}
	
	// constructor with data
	LOD(string& data) : DataEntry("lod", "<matref><lengthPerPixel>", data.c_str()) {
		commands = vector<LODCommand>();
		matref = string("");
		pixelLength = 0;
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the data
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// make sure we only have one
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
		if(!hasOnlyOne(lines, header))
			return 0;
			
		// get the data
		const char* lodData = lines[0].c_str();
		
		// get the lengthPerPixel values
		vector<string> lppVals = BZWParser::getValuesByKey("lengthPerPixel", header, lodData);
		if(!hasOnlyOne(lppVals, "lengthPerPixel"))
			return 0;
			
		// get the sections beginning with matref and create LODCommand lists from them
		vector<string> matrefSections = BZWParser::getSectionsByHeader("matref", lodData);
		if(!hasOnlyOne(matrefSections, "matref"))
			return 0;
		
		// parse each LOD command
		vector<string> lodLines = BZWParser::getLines("matref", matrefSections[0].c_str());
		vector<LODCommand> cmds = vector<LODCommand>();
		for(vector<string>::iterator i = lodLines.begin(); i != lodLines.end(); i++) {
			if(LODCommand::isValidCommand( *i )) {
				printf("|%s|\n", i->c_str());
				LODCommand lc = LODCommand(*i);
				printf("<%s>\n", lc.toString().c_str());
				cmds.push_back(lc);
			}	
		}
		
		return 1;
	}
	
	// toString
	string toString(void) {
		return string("lod");
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	
	vector<LODCommand> commands;
	string matref;
	int pixelLength;
};

#endif /*LOD_H_*/
