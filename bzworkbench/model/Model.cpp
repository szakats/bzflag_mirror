#include "../include/model/Model.h"

// global reference to the model so the static call will worlk
Model* modelRef;

void Model::buildDatabase() {
	
	this->dataMap["world"] 			= this->worldData;
	this->dataMap["waterLevel"] 	= this->waterLevelData;
	this->dataMap["options"]		= this->optionsData;
}

Model::Model()
{
	this->worldData = new world();
	this->optionsData = new options();
	this->waterLevelData = new waterLevel();
	
	this->dataMap = map<string, DataEntry*>();
	
	buildDatabase();
	
	modelRef = this;
}

Model::~Model()
{
	if(this->worldData)
		delete worldData;
	
	if(this->optionsData)
		delete optionsData;
		
	if(this->waterLevelData)
		delete waterLevelData;
}

// the query method
DataEntry* Model::query(char* data) { return modelRef->_query(data); }
