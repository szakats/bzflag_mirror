#ifndef COLORCOMMAND_H_
#define COLORCOMMAND_H_

#include "DataEntry.h"
#include "model/BZWParser.h"
#include <string>
#include <vector>


/**
 * This is an assistant class to dynamicColor.
 * Like Transform assists bz2objects, ColorCommand assists
 * materials in that it provides the data for a command
 * that can be passed to a color channel.  Supported commands are
 * "limit", "sinusoid", "clampUp", "clampDown", and "sequence".
 * This class also stores the values that go with these keys
 */
 
 
class ColorCommand : public DataEntry {

public:

	// default constructor
	ColorCommand() : DataEntry("", "<limits><clampUp><clampDown><sinusoid><sequence>") {
		this->name = string("");
		this->commands = vector<float>();	
	}
	
	// constructor with data
	ColorCommand(string& data) :
		DataEntry("", "<limits><clampUp><clampDown><sinusoid><sequence>", data.c_str() ) {
			
		// init
		commands = vector<float>();
		
		// get the name of the colorcommand data
		this->name = BZWParser::key( data.c_str() );
		
		// get the value after the name
		string value = BZWParser::value( name.c_str(), data.c_str() );
		
		// get the value elements (if any exist)
		if(value != name) {
			vector<string> elements = BZWParser::getLineElements( value.c_str() );
			for(vector<string>::iterator i = elements.begin(); i != elements.end(); i++) {
				commands.push_back( atof( i->c_str() ) );	
			}
		}
	}
	
	// determine whether or not the line is a valid color command
	static bool isValid(string& data) {
		// simple check--does data have anything at all?
		if(data.length() < 1)
			return false;
			
		// get the name
		string cmdName = BZWParser::key( data.c_str() );
		
		// compare the name against supported commands (break if unrecognized)
		if(! (cmdName == "limits" ||
			  cmdName == "clampUp" ||
			  cmdName == "clampDown" ||
			  cmdName == "sinusoid" ||
			  cmdName == "sequence"))
			  return false;
			  		
		// check the number of elements after the command
		string valuesString = BZWParser::value( cmdName.c_str(), data.c_str() );
		if(valuesString == cmdName) 		// the value will be the same as the key if there are no other values
			return false;
			
		// count the number of commands
		unsigned int numCommands = BZWParser::getLineElements( valuesString.c_str() ).size();
		
		// check the number against the command name
		// "limit" should be followed by 2 commands
		if(cmdName == "limits" && numCommands != 2)
			return false;
		
		// "sinusoid", "clampUp", and "clampDown" should be followed by 3 commands
		if(numCommands != 3 &&
		   (cmdName == "sinusoid" ||
		    cmdName == "clampUp" ||
		    cmdName == "clampDown"))
				return false;
		
		// "sequence" needs at least three commands
		if(cmdName == "sequence" && numCommands < 3)
			return false;
			
		
		// it just might be valid
		return true;
	}
	
	
	// toString
	string toString(void) {
		string commandString = string("");
		
		// make sure we HAVE commands to parse :)
		if(commands.size() > 0) {
			if(name != "sequence") {
				// in all but "sequence", all commands are floats
				for(vector<float>::iterator i = commands.begin(); i != commands.end(); i++) {
					commandString += string( ftoa(*i) ) + " ";	
				}	
			}
			else {
				// if this is a "sequence", only the first two commands are floats
				for(vector<float>::iterator i = commands.begin(); i != commands.begin() + 2; i++) {
					commandString += string( ftoa(*i) ) + " ";	
				}	
				// the rest are ints
				for(vector<float>::iterator i = commands.begin() + 2; i != commands.end(); i++) {
					commandString += string( itoa((int)(*i)) ) + " ";	
				}
			}
			return name + " " + commandString + "\n";
		}
		else {
			return string("");	
		}
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// update (just to keep with the interface)
	int update(string& data) { return 1; }
	
	// render (just to keep with the interface)
	int render(void) { return 0; }
	
private:

	string name;
	vector<float> commands;	
};

#endif /*COLORCOMMAND_H_*/
