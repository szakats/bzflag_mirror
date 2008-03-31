/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

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
		const char* _header = this->getHeader().c_str();
		
		// get the data
		vector<string> lines = BZWParser::getSectionsByHeader(_header, data.c_str());
		
		// make sure we only have one
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
		if(!hasOnlyOne(lines, _header))
			return 0;
			
		// get the data
		const char* lodData = lines[0].c_str();
		
		// get the lengthPerPixel values
		vector<string> lppVals = BZWParser::getValuesByKey("lengthPerPixel", _header, lodData);
			
		// get the sections beginning with matref and create LODCommand lists from them
		vector<string> matrefSections = BZWParser::getSectionsByHeader("matref", lodData);
		if(matrefSections.size() > 1) {
			printf("mesh::LOD::update(): Error! Defined \"matref\" %d times!\n", (int)matrefSections.size());
			return 0;	
		}
		
		// parse each LOD command
		vector<string> lodLines = BZWParser::getLines("matref", matrefSections[0].c_str());
		vector<LODCommand> cmds = vector<LODCommand>();
		if(lodLines.size() > 1) {
			for(vector<string>::iterator i = lodLines.begin() + 1; i != lodLines.end(); i++) {
				if(LODCommand::isValidCommand( *i )) {
					cmds.push_back( LODCommand(*i) );
				}
				else {
					printf("mesh::LOD::update(): Error! Unsupported LOD entry \"%s\"\n", i->c_str());
					return 0;
				}
			}
		}
		
		// do base class update
		if(!DataEntry::update(data))
			return 0;
			
		// set data
		this->pixelLength = (lppVals.size() != 0 ? atoi( lppVals[0].c_str() ) : 0);
		this->matref = (lodLines.size() > 0 ? BZWParser::value( "matref", lodLines[0].c_str() ) : string(""));
		this->commands = cmds;
		
		return 1;
	}
	
	// toString
	string toString(void) {
		string lods = string("");
		if(commands.size() > 0) {
			for( vector<LODCommand>::iterator i = commands.begin(); i != commands.end(); i++) {
				lods += string("        ") + i->toString().c_str() + "\n";	
			}
		}
		return string("lod\n") +
					  "      lengthPerPixel " + string(itoa(pixelLength)) + "\n" +
					  (matref.length() > 0 ? string("      matref ") + matref + "\n" + lods + "      end\n" : string("")) + "\n" +
					  "    end\n";
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
