#ifndef DYNAMICCOLOR_H_
#define DYNAMICCOLOR_H_

#include "../DataEntry.h"
#include "../ColorCommand.h"

class dynamicColor : public DataEntry {

public:
	
	// default constructor
	dynamicColor();
	
	// constructor with data
	dynamicColor(string& data);
	
	static DataEntry* init() { return new dynamicColor(); }
	static DataEntry* init(string& data) { return new dynamicColor(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	string name;
	
	vector<ColorCommand> redCommands;
	vector<ColorCommand> greenCommands;
	vector<ColorCommand> blueCommands;
	vector<ColorCommand> alphaCommands;
};

#endif /*DYNAMICCOLOR_H_*/
