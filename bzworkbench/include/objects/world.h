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
		
		world(string& data) : DataEntry("world", "<name><size><flagHeight><noWalls>") {
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
		void update(string& data) {
			const char* header = this->getHeader().c_str();
			
			// get the world data objects
			vector<string> worldDatas = BZWParser::getSectionsByHeader(header, data.c_str());
			if(worldDatas.size() < 1) {
				printf("world::update():  Warning! no world object defined!\n");
				return;	
			}
			else if(worldDatas.size() > 1) {
				printf("world::update():  Warning! multiple (%d) world objects; selecting the first\n", worldDatas.size());	
			}
			const char* worldData = worldDatas[0].c_str();
			
			// get the names, but only choose one
			vector<string> names = BZWParser::getValuesByKey("name", header, worldData);
			if(names.size() == 1)
				this->worldName = names[0];
			else if(names.size() > 1) {
				printf("world::update():  Warning! multiple (%d) names; selecting the first one...\n", names.size());
				this->worldName = names[0];	
			}
			
			// get the sizes, but only choose one
			vector<string> sizes = BZWParser::getValuesByKey("size", header, worldData);
			if(sizes.size() == 1)
				this->size = atof( sizes[0].c_str() );
			else if(sizes.size() > 1) {
				printf("world::update():  Warning! multiple (%d) sizes; selecting the first one...\n", sizes.size());
				this->size = atof( sizes[0].c_str() );
			}
			
			// get the flagHeights, but only choose one
			vector<string> flagHeights = BZWParser::getValuesByKey("flagHeight", header, worldData);
			if(flagHeights.size() == 1)
				this->flagHeight = atof( flagHeights[0].c_str() );
			else if(flagHeights.size() > 1) {
				printf("world::update():  Warning! multiple (%d) flagHeights; selecting the first one...\n", flagHeights.size());
				this->flagHeight = atof( flagHeights[0].c_str() );
			}
			
			// get the noWalls value, but only choose one
			vector<string> noWallses = BZWParser::getValuesByKey("noWalls", header, worldData);
			if(noWallses.size() == 1)
				this->noWalls = (noWallses[0].compare("noWalls") == 0 ? true : false);
			else if(noWallses.size() > 1) {
				printf("world::update():  Warning! multiple (%d) noWalls\n", noWallses.size());
				this->noWalls = (noWallses[0].compare("noWalls") == 0 ? true : false);
			}
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
