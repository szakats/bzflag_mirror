#include "../include/objects/dynamicColor.h"

// default constructor
dynamicColor::dynamicColor() :
	DataEntry("dynamicColor", "<red><green><blue><alpha><name>") {
		
	this->name = string("");
	this->redCommands = vector<ColorCommand>();
	this->greenCommands = vector<ColorCommand>();
	this->blueCommands = vector<ColorCommand>();
	this->alphaCommands = vector<ColorCommand>();
}

// constructor with data
dynamicColor::dynamicColor(string& data) :
	DataEntry("dynamicColor", "<red><green><blue><alpha><name>", data.c_str()) {
	
	this->name = string("");
	this->redCommands = vector<ColorCommand>();
	this->greenCommands = vector<ColorCommand>();
	this->blueCommands = vector<ColorCommand>();
	this->alphaCommands = vector<ColorCommand>();
	
	this->update(data);
}

// getter
string dynamicColor::get(void) { return this->toString(); }

/**
 * Helper method:  check and see if the commands given are valid ColorCommands
 */
 
bool areValidCommands(vector<string>& commandList) {
	// an empty list is valid
	if(commandList.size() == 0)
		return true;
	
	// iterate through and use ColorCommand::isValid() to check each command	
	for(vector<string>::iterator i = commandList.begin(); i != commandList.end(); i++) {
		if(! ColorCommand::isValid( *i ) ) {
			printf("dynamicColor::update(): Error! Invalid color command \"%s\"\n", i->c_str());
			return false;	
		}
	}
	
	return true;
}

/**
 * Helper method: convert a vector of string-ified color commands into a vector of ColorCommand objects
 */
 
vector<ColorCommand> parseCommands(vector<string> commandList) {
	vector<ColorCommand> ret = vector<ColorCommand>();
	
	// return an empty list if there are no commands in commandList
	if(commandList.size() == 0)
		return ret;
	
	// iterate through the commandList and build ColorCommand objects from them
	for(vector<string>::iterator i = commandList.begin(); i != commandList.end(); i++) {
		ret.push_back( ColorCommand( *i ) );	
	}
	
	return ret;
}

/**
 * 	Helper method: convert a vector of ColorCommand objects into one huge BZW-formatted string
 */
string stringifyCommands(vector<ColorCommand>& commands, const char* color) {
	string ret = string("");
	
	// break if there are no commands
	if(commands.size() == 0)
		return ret;
		
	// iterate through the commands and concat their toString() output to ret
	for(vector<ColorCommand>::iterator i = commands.begin(); i != commands.end(); i++) {
		ret += string("  ") + color + " " + i->toString();	
	}
	
	return ret;
}

// setter
int dynamicColor::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the section
	vector<string> lines = BZWParser::getSectionsByHeader( header, data.c_str() );
	
	// break of there isn't only one
	if(!hasOnlyOne(lines, "header"))
		return 0;
		
	// get the data
	const char* dynamicColorData = lines[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey( "name", header, dynamicColorData );
	if(!hasOnlyOne(names, "name"))
		return 0;
		
	// get and validate the "red" commands
	vector<string> redCommandVals = BZWParser::getValuesByKey( "red", header, dynamicColorData );
	if(! areValidCommands(redCommandVals) )
		return 0;
	
	// get and validate the "green" commands
	vector<string> greenCommandVals = BZWParser::getValuesByKey( "green", header, dynamicColorData );
	if(! areValidCommands(greenCommandVals) )
		return 0;
	
	// get and validate the "blue" commands
	vector<string> blueCommandVals = BZWParser::getValuesByKey( "blue", header, dynamicColorData );
	if(! areValidCommands(blueCommandVals) )
		return 0;
	
	// get validate the "alpha" commands
	vector<string> alphaCommandVals = BZWParser::getValuesByKey( "alpha", header, dynamicColorData );
	if(! areValidCommands(alphaCommandVals) )
		return 0;
		
	// update superclass
	if(! DataEntry::update(data) )
		return 0;
		
	// commit the data
	this->redCommands = parseCommands( redCommandVals );
	this->greenCommands = parseCommands( greenCommandVals );
	this->blueCommands = parseCommands( blueCommandVals );
	this->alphaCommands = parseCommands( alphaCommandVals );
	this->name = names[0];
	
	return 1;
}

// toString
string dynamicColor::toString(void) {
	// string-ify the commands by color
	string redString = stringifyCommands( redCommands, "red" );
	string blueString = stringifyCommands( blueCommands, "blue" );
	string greenString = stringifyCommands( greenCommands, "green" );
	string alphaString = stringifyCommands( alphaCommands, "alpha" );
	
	return string("dynamicColor\n") +
				  (name.length() != 0 ? "  name " + name : string("# name")) + "\n" +
				  redString +
				  greenString +
				  blueString +
				  alphaString +
				  "end\n";
}

// render
int dynamicColor::render(void) {
	return 0;	
}
