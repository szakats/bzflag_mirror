#ifndef WORLDOPTIONSDATA_H_
#define WORLDOPTIONSDATA_H_

#include <stdio.h>
#include <iostream>
#include "DataEntry.h"

inline char* ftoa(float f) {
	string ret;
	std::cout << f << ret;
	return (char*)ret.c_str();
}

class WorldOptionsData : public DataEntry {
	public:
		WorldOptionsData() {
			this->worldName = "Brave New World";
			this->waterMaterialName = NULL;
			this->optionsString = NULL;
			this->size = 400.0;
			this->noWalls = false;
			this->flagHeight = 10.0f;
			this->waterLevel = -1.0f;
		}
		
		WorldOptionsData(string& data) {
			update(data);	
		}
		
		// send the data
		void get(string& data) {
			data.assign(this->toString());
		}
		
		// receive the data
		void update(string& data) {
			this->worldName = this->parseWorldName(data);
			this->optionsString = this->parseOptionsString(data);
			this->waterLevel = this->parseWaterLevel(data);
			this->flagHeight = this->parseFlagHeight(data);
			this->waterMaterialName = this->parseWaterMaterial(data);
			this->noWalls = this->parseNoWalls(data);
			this->size = this->parseSize(data);
		}
		
		string toString(void) {
			
			return string(string("world\n") +
								 "  name " + worldName + "\n" +
								 "  size " + ftoa(size) + "\n" +
								 "  flagHeight" + ftoa(flagHeight) + "\n" +
									(noWalls == true ? "noWalls\n" : "# noWalls\n") +
								 "end\n\n" +
								  
								 "options\n" +
								 "  " + optionsString + "\n" +
								 "end\n\n" +
								  
								 "waterLevel\n" +
								 "  name defaultWaterLevel\n" +
								 "  height " + ftoa(waterLevel) + "\n" +
								 "  materials " + waterMaterialName + "\n" +
								 "end\n");
						  
		}
		
	private:
		char* worldName;
		char* optionsString;
		float waterLevel;
		float flagHeight;
		float size;
		bool noWalls;
		char* waterMaterialName;
		
		/**
		 * Parsers
		 * 
		 * These methods read in BZW-formatted text and pick out key values from them.
		 */
		
		// read in the BZW-formatted data and return the world's name
		char* parseWorldName(string& data) {
			// find the start of the world chunk
			string::size_type startIndex = data.find("world\n", 0) + 7;
			string::size_type endIndex = data.find("end\n", startIndex);
			string worldData = data.substr(startIndex, endIndex - startIndex);
			
			// return null if we couldn't find anything that started with "world"
			if(startIndex == string::npos || endIndex == string::npos)
				return NULL;
				
			// find the name key
			startIndex = worldData.find("name ", startIndex) + 5;
			endIndex = worldData.find("\n", startIndex);
			
			// return null if we couldn't find anything that started with "name"
			if(startIndex == string::npos || endIndex == string::npos)
				return NULL;
				
			return (char*)worldData.substr(startIndex, endIndex - startIndex).c_str();
		}
		
		// read in the BZW-formatted data and return the options string
		char* parseOptionsString(string& data) {
			string::size_type startIndex = data.find("options\n", 0) + 8;
			string::size_type endIndex = data.find("end\n", startIndex);
			
			// return null if we couldn't find anything that started with "options"
			if(startIndex == string::npos || endIndex == string::npos)
				return NULL;
				
			return (char*)data.substr(startIndex, endIndex - startIndex).c_str();
		}
		
		// read in the BZW-formatted data and return the waterlevel
		float parseWaterLevel(string& data) {
			// first find the waterLevel chuck
			string::size_type startIndex = data.find("waterLevel\n", 0) + 11;
			string::size_type endIndex = data.find("end\n", 0);
			string waterLevelData = data.substr(startIndex, endIndex - startIndex);
			
			// return -1.0f if waterLevel didn't show up
			if(startIndex == string::npos || endIndex == string::npos)
				return -1.0f;
			
			// now find the occurance of the height
			startIndex = waterLevelData.find("height ", startIndex) + 7;
			endIndex = waterLevelData.find("\n", startIndex);
			
			// return -1.0f if height didn't show up
			if(startIndex == string::npos || endIndex == string::npos)
				return -1.0f;
			
			return atof(waterLevelData.substr(startIndex, endIndex - startIndex).c_str());
		}
		
		// read in the BZW-formatted data and find the flag height
		float parseFlagHeight(string& data) {
			// first find the world chuck
			string::size_type startIndex = data.find("world\n", 0) + 7;
			string::size_type endIndex = data.find("end\n", startIndex);
			string worldData = data.substr(startIndex, endIndex - startIndex);
			
			// return 0 if we couldn't find anything that started with "world"
			if(startIndex == string::npos || endIndex == string::npos)
				return 0.0f;
				
			// now find the flagHeight key
			startIndex = worldData.find("flagHeight ", startIndex);
			endIndex = worldData.find("\n", startIndex);
			
			// return 0 if we couldn't find anything that started with "flagHeight"
			if(startIndex == string::npos || endIndex == string::npos)
				return 0.0f;
				
			return atof(worldData.substr(startIndex, endIndex - startIndex).c_str());
		}
		
		// read in BZW-formatted data and find the water material name
		char* parseWaterMaterial(string& data) {
			// first find the waterLevel chunk
			string::size_type startIndex = data.find("waterLevel\n", 0) + 11;
			string::size_type endIndex = data.find("end\n", 0);
			string waterLevelData = data.substr(startIndex, endIndex - startIndex);
			
			// return NULL if we couldn't find anything that started with "waterLevel"
			if(startIndex == string::npos || endIndex == string::npos)
				return NULL;
			
			// now find the materials key
			startIndex = waterLevelData.find("materials ", startIndex) + 10;
			endIndex = waterLevelData.find("\n", startIndex);
			
			// return NULL if we couldn't find anything that started with "materials"
			if(startIndex == string::npos || endIndex == string::npos)
				return NULL;
				
			return (char*)(waterLevelData.substr(startIndex, endIndex - startIndex).c_str());
			
		}
		
		// read in BZW-formatted data and find the noWalls keyword
		bool parseNoWalls(string& data) {
			// first find the world chuck
			string::size_type startIndex = data.find("world\n", 0) + 7;
			string::size_type endIndex = data.find("end\n", startIndex);
			string worldData = data.substr(startIndex, endIndex - startIndex);
			
			// return false if we couldn't find anything that started with "world"
			if(startIndex == string::npos || endIndex == string::npos)
				return false;
			
			// now find the noWalls keyword if we can
			startIndex = worldData.find("noWalls", startIndex);
			
			// if we found it, then return true
			if(startIndex != string::npos)
				return true;
			else
				return false;
		}
		
		// read in BZW-formatted data and find the size
		float parseSize(string& data) {
			// first find the world chuck
			string::size_type startIndex = data.find("world\n", 0) + 7;
			string::size_type endIndex = data.find("end\n", startIndex);
			string worldData = data.substr(startIndex, endIndex - startIndex);
			
			// return 0 if we couldn't find anything that started with "world"
			if(startIndex == string::npos || endIndex == string::npos)
				return 0.0f;
			
			startIndex = worldData.find("size ", startIndex) + 5;
			endIndex = worldData.find("\n", startIndex);
			
			// return 0 if we couldn't find anything that started with "size"
			if(startIndex == string::npos || endIndex == string::npos)
				return 0.0f;
			
			return atof(worldData.substr(startIndex, endIndex - startIndex).c_str());
		}
};

#endif /*WORLDOPTIONSDATA_H_*/
