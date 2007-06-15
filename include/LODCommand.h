#ifndef LODCOMMAND_H_
#define LODCOMMAND_H_

#include "DataEntry.h"

class LODCommand : public DataEntry {
	
public:
	
	// constructor
	LODCommand() : DataEntry("", "<dlist><sphere><points><lines><lineloop><linestrip><tris><tristrip><trifan><quads><quadstrip><polygon>") {
		name = string("");
		args = vector<int>();
		rad = 0;
		x = y = z = 0;
	}
	
	// constructor with data
	LODCommand(string& data) : DataEntry("", "<dlist><sphere><points><lines><lineloop><linestrip><tris><tristrip><trifan><quads><quadstrip><polygon>", data.c_str()) {
		name = string("");
		args = vector<int>();
		rad = 0;
		x = y = z = 0;
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		
		// assume its just one line
		// is the command valid?
		if(!isValidCommand(data))
			return 0;
		
		// get the name
		string newName = BZWParser::key( data.c_str() );
		
		// get the data
		string value = BZWParser::value( newName.c_str(), data.c_str() );
		
		// break up the data into line elements
		vector<string> values = vector<string>();
		if(value != newName)
			values = BZWParser::getLineElements( value.c_str() );
		
		// set the data in the superclass
		if(!DataEntry::update(data))
			return 0;
		
		// load the data in (first clear the arg list)
		args.clear();
		this->name = newName;
		
		// exception: sphere's last arg has to be a float
		if(newName == "sphere") {
			x = atof( values[0].c_str() );
			y = atof( values[1].c_str() );
			z = atof( values[2].c_str() );
			rad = atof( values[3].c_str() );
		}
		else if(values.size() > 0) {
			for(vector<string>::iterator i = values.begin(); i != values.end(); i++) {
				args.push_back( atoi( i->c_str() ) );	
			}
		}
		
		return 1;
		
	}
	
	/**
	 * Helper method:  determine whether or not the command is valid
	 */
	 
	static bool isValidCommand(string& command) {
		
		// non-existant commands are valid
		if(command.length() == 0)
			return true;
			
		// get the name and values
		string commandName = BZWParser::key( command.c_str() );
		
		// make sure its a valid command
		// (can't call isKey here--isKey isn't and can't be static)
		if(! (commandName == "dlist" ||
			  commandName == "sphere" ||
			  commandName == "points" ||
			  commandName == "lines" ||
			  commandName == "lineloop" ||
			  commandName == "linestrip" ||
			  commandName == "tris" ||
			  commandName == "tristrip" ||
			  commandName == "trifan" ||
			  commandName == "quads" ||
			  commandName == "quadstrip" ||
			  commandName == "polygon") )
			  		return false;
		
		// get the command values
		string val = BZWParser::value( commandName.c_str(), command.c_str() );
			  		
		vector<string> values = BZWParser::getLineElements( val.c_str() );
		
		// dlist gets 0 args (values[0] will be the same as the key in that case)
		if(commandName == "dlist" && values[0] != commandName)
			return false;
		
		// sphere gets four args
		if(commandName == "sphere" && values.size() != 4)
			return false;
			
		// points gets at least one arg
		if(commandName == "point" && values[0] != commandName)
			return false;
			
		// lines gets at least 2 args
		if(commandName == "lines" && values.size() < 2)
			return false;
			
		// lineloop gets at least 2 args
		if(commandName == "lineloop" && values.size() < 2)
			return false;
			
		// linestrip gets at least 2 args
		if(commandName == "linestrip" && values.size() < 2)
			return false;
			
		// tris gets at least 3 args
		if(commandName == "tris" && values.size() < 3)
			return false;
			
		// tristrip gets at least 3 args
		if(commandName == "tristrip" && values.size() < 3)
			return false;
			
		// trifan gets at least 3 args
		if(commandName == "trifan" && values.size() < 3)
			return false;
			
		// quads gets at least 4 args
		if(commandName == "quads" && values.size() < 4)
			return false;
		
		// quadstrip gets at least 4 args
		if(commandName == "quadStrip" && values.size() < 4)
			return false;
			
		// polygon gets at least 2
		if(commandName == "polygon" && values.size() < 2)
			return false;
			
		// test passed
		return true;
	}
	
	// toString
	string toString(void) {
		if(name == "sphere")
			return name + " " + string(ftoa(x)) + " " + string(ftoa(y)) + " " + string(ftoa(z)) + " " + string(ftoa(rad));
		else
			return name + " " + stringify(args);
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:

	// turn an array of ints into a string
	string stringify(vector<int>& values) {
		string ret = string("");
		
		if(values.size() > 0) {
			for(vector<int>::iterator i = values.begin(); i != values.end(); i++) {
				ret += string(itoa(*i)) + " ";	
			}	
		}
		
		return ret;
	}

	string name;
	vector<int> args;
	float x, y, z, rad;		// only used in sphere
};

#endif /*LODCOMMAND_H_*/
