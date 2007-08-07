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
	
	// getters
	string& getName() { return this->name; }
	vector<ColorCommand>& getRedCommands() { return this->redCommands; }
	vector<ColorCommand>& getGreenCommands() { return this->greenCommands; }
	vector<ColorCommand>& getBlueCommands() { return this->blueCommands; }
	vector<ColorCommand>& getAlphaCommands() { return this->alphaCommands; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	void setRedCommands( const vector<ColorCommand>& commands ) { this->redCommands = commands; }
	void setGreenCommands( const vector<ColorCommand>& commands ) { this->greenCommands = commands; }
	void setBlueCommands( const vector<ColorCommand>& commands ) { this->blueCommands = commands; }
	void setAlphaCommands( const vector<ColorCommand>& commands ) { this->alphaCommands = commands; }
	
	
private:
	string name;
	
	vector<ColorCommand> redCommands;
	vector<ColorCommand> greenCommands;
	vector<ColorCommand> blueCommands;
	vector<ColorCommand> alphaCommands;
};

#endif /*DYNAMICCOLOR_H_*/
