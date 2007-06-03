#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class options : public DataEntry {
public:
	
	options() : DataEntry("options", "") {
		this->optionsString = string("");
	}
	
	options(string& data) : DataEntry("options", "") { this->update(data); }
	
	// get method
	void get(string& data) {
		data.assign(this->toString());	
	}
	
	// update method
	void update(string& data) {
		string header = this->getHeader();
		// get options objects
		vector<string> optionses = BZWParser::getSectionsByHeader(header, data);
		if(optionses.size() > 1) {
			printf("options::update():  Warning! multiple (%d) options objects; choosing the first one...\n", optionses.size());	
		}
		else if(optionses.size() < 1) {
			printf("options::update():  Warning! no options objects found!\n");
			return;	
		}
		
		string opts = optionses[0];
		
		// get the lines
		vector<string> options = BZWParser::getLines(header, opts);
		
		// concat the lines
		this->optionsString.clear();
		
		for(vector<string>::iterator i = options.begin(); i != options.end(); i++) {
			this->optionsString += *i + " ";
		}
	}
	
	// toString method
	string toString(void) {
		return string(string("options\n") +
							"  " + optionsString + "\n" + 
							"end\n");
	}
private:
	string optionsString;
};

#endif /*OPTIONS_H_*/
