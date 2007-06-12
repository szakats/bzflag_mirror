#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class waterLevel : public DataEntry {
public:

	// default constructor
	waterLevel();
	
	// constructor with data
	waterLevel(string& data);
	
	// get method
	string get(void);
	
	// update method
	int update(string& data);
	
	// toString method
	string toString(void);
	
	// render method
	int render(void);
	
	virtual ~waterLevel();
	
private:
	string name;
	vector<string> materials;
	float height;
};

#endif /*WATERLEVEL_H_*/
