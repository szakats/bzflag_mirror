#include "../include/Model.h"

/**
 * This method builds the Model's built-in database of world-related data.
 * 
 */
void Model::buildDatabase() {
	
}

Model::Model()
{
	this->worldOptionsData = new WorldOptionsData();
	
	this->data = std::map<unsigned int, DataEntry*>();
	
	buildDatabase();
}

Model::~Model()
{
	if(this->worldOptionsData)
		delete worldOptionsData;
}

/**
 * This method allows other classes to query the Model class for world-specific data by passing in an integer key.
 * This method returns a void pointer to the data.
 * It is the responsibility of the caller to know in advance what they want.
 */
DataEntry* Model::query(unsigned int dataKey) {
	return this->data.at(dataKey);
}

