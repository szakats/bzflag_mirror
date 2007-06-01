#ifndef WORLDOPTIONSDATA_H_
#define WORLDOPTIONSDATA_H_

#include "DataEntry.h"

class WorldOptionsData : public DataEntry {
	public:
		WorldOptionsData(char* worldName, char* optionsString, float waterLevel, float flagHeight) {
			update(worldName, optionsString, waterLevel, flagHeight);	
		}
		
		// send the data
		void get(char* worldName, char* optionsString, float& waterLevel, float& flagHeight) {
			worldName = this->worldName;
			optionsString = this->optionsString;
			waterLevel = this->waterLevel;
			flagHeight = this->flagHeight;	
		}
		
		// receive the data
		void update(char* worldName, char* optionsString, float waterLevel, float flagHeight) {
			this->worldName = worldName;
			this->optionsString = optionsString;
			this->waterLevel = waterLevel;
			this->flagHeight = flagHeight;	
		}
		
		// methods to implement from DataEntry:
		void getData(Model* h) {
				
		}
		
		void updateData(Model* h) {
			
		}
		
	private:
		char* worldName;
		char* optionsString;
		float waterLevel;
		float flagHeight;
};

#endif /*WORLDOPTIONSDATA_H_*/
