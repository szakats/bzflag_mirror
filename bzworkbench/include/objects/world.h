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
			this->worldName = string("Brave_New_World");
			this->size = 400.0;
			this->noWalls = false;
			this->flagHeight = 10.0f;
		}
		
		world(string& data) : DataEntry("world", "<name><size><flagHeight><noWalls>") {
			update(data);	
		}
		
		// send the data
		void get(string& data) {
			data.assign(this->toString());
		}
		
		// receive the data
		void update(string& data) {
			string header = this->getHeader();
			
			// get the world data objects
			vector<string> worldDatas = BZWParser::getSectionsByHeader(header, data);
			if(worldDatas.size() < 1) {
				printf("world::update():  Warning! no world object defined!\n");
				return;	
			}
			else if(worldDatas.size() > 1) {
				printf("world::update():  Warning! multiple (%d) world objects; selecting the first\n", worldDatas.size());	
			}
			string worldData = worldDatas[0];
			
			// get the names, but only choose one
			string nameKey = string("name");
			vector<string> names = BZWParser::getValuesByKey(nameKey, header, worldData);
			if(names.size() == 1)
				this->worldName = names[0];
			else if(names.size() > 1) {
				printf("world::update():  Warning! multiple (%d) names; selecting the first one...\n", names.size());
				this->worldName = names[0];	
			}
			
			// get the sizes, but only choose one
			string sizeKey = string("size");
			vector<string> sizes = BZWParser::getValuesByKey(sizeKey, header, worldData);
			if(sizes.size() == 1)
				this->size = atof( sizes[0].c_str() );
			else if(sizes.size() > 1) {
				printf("world::update():  Warning! multiple (%d) sizes; selecting the first one...\n", sizes.size());
				this->size = atof( sizes[0].c_str() );
			}
			
			// get the flagHeights, but only choose one
			string flagHeightKey = string("flagHeight");
			vector<string> flagHeights = BZWParser::getValuesByKey(flagHeightKey, header, worldData);
			if(flagHeights.size() == 1)
				this->flagHeight = atof( flagHeights[0].c_str() );
			else if(flagHeights.size() > 1) {
				printf("world::update():  Warning! multiple (%d) flagHeights; selecting the first one...\n", flagHeights.size());
				this->flagHeight = atof( flagHeights[0].c_str() );
			}
			
			// get the noWalls value, but only choose one
			string noWallsKey = string("noWalls");
			vector<string> noWallses = BZWParser::getValuesByKey(noWallsKey, header, worldData);
			if(noWallses.size() == 1)
				this->noWalls = (noWallses[0].compare("noWalls") == 0 ? true : false);
			else if(noWallses.size() > 1) {
				printf("world::update():  Warning! multiple (%d) noWalls\n", noWallses.size());
				this->noWalls = (noWallses[0].compare("noWalls") == 0 ? true : false);
			}
		}
		
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
		
	private:
		string worldName;
		float size;
		float flagHeight;
		bool noWalls;
};

#endif /*WORLDOPTIONSDATA_H_*/
