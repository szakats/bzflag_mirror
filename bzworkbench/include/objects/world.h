#ifndef WORLDOPTIONSDATA_H_
#define WORLDOPTIONSDATA_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "../DataEntry.h"
#include "../BZWParser.h"

class world : public DataEntry {
	public:
		world() : DataEntry("world", "<name><size><flagHeight><noWalls>") {
			this->worldName = string("");
			this->size = 400.0;
			this->noWalls = false;
			this->flagHeight = 10.0f;
		}
		
		world(string& data) : DataEntry("world", "<name><size><flagHeight><noWalls>", data.c_str()) {
			this->worldName = string("");
			this->size = 400.0;
			this->noWalls = false;
			this->flagHeight = 10.0f;
			
			update(data);	
		}
		
		// send the data
		string get(void) {
			return this->toString();
		}
		
		// receive the data
		int update(string& data) {
			const char* header = this->getHeader().c_str();
			
			// get the world data objects
			vector<string> worldDatas = BZWParser::getSectionsByHeader(header, data.c_str());
			if(worldDatas[0] == BZW_NOT_FOUND)
				return 0;
				
			if(!hasOnlyOne(worldDatas, "world"))
				return 0;
			
			const char* worldData = worldDatas[0].c_str();
			
			// get the names, but only choose one
			vector<string> names = BZWParser::getValuesByKey("name", header, worldData);
			if(!hasOnlyOne(names, "name"))
				return 0;
			
			// get the sizes, but only choose one
			vector<string> sizes = BZWParser::getValuesByKey("size", header, worldData);
			if(!hasOnlyOne(sizes, "size"))
				return 0;
			
			// get the flagHeights, but only choose one
			vector<string> flagHeights = BZWParser::getValuesByKey("flagHeight", header, worldData);
			if(!hasOnlyOne(flagHeights, "flagHeight"))
				return 0;
			
			// get the noWalls value, but only choose one
			vector<string> noWallses = BZWParser::getValuesByKey("noWalls", header, worldData);
			
			// fill in the data
			if(!DataEntry::update(data))
				return 0;
			this->worldName = names[0];
			this->size = atof( sizes[0].c_str() );
			this->flagHeight = atof( flagHeights[0].c_str() );
	 		this->noWalls = (noWallses.size() == 0 ? false : true);
	 		
	 		return 1;
		}
		
		// toString method
		string toString(void) {
			string sizeString = string(ftoa(size));
			string flagHeightString = string(ftoa(flagHeight));
			return string(string("world\n") +
								 "  name " + worldName + "\n" +
								 "  size " + sizeString + "\n" +
								 "  flagHeight " + flagHeightString + "\n" +
									(noWalls == true ? "  noWalls\n" : "# noWalls\n") +
									this->getUnusedText() +
								 "end\n");
						  
		}
		
		// render method
		int render(void) { return 0; } 
		
	private:
		string worldName;
		float size;
		float flagHeight;
		bool noWalls;
};

#endif /*WORLDOPTIONSDATA_H_*/
