#ifndef MODEL_H_
#define MODEL_H_

class Model;

#include "DataEntry.h"
#include "WorldOptionsData.h"

#include <map>

class Model
{
public:
	Model();
	virtual ~Model();
	
	// query pointers to private members
	DataEntry* query(unsigned int dataKey);
	
private:
// world options
	WorldOptionsData* worldOptionsData;

// mapping of int keys to void pointers (which point do data)
	std::map<unsigned int, DataEntry*> data;

// build the worldData
	void buildDatabase();
};


#endif /*MODEL_H_*/
