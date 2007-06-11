#ifndef LINK_H_
#define LINK_H_

#include "../DataEntry.h"
#include "../BZWParser.h"

/**
 * Link object between teleporters.
 * Needs to be called "Tlink" because link() is a system call
 * which voids the effect of the constructor
 */
class Tlink : public DataEntry {
	
public:
	
	Tlink() : DataEntry("link", "<name><from><to>") {
		name = string("default_link");
		from = string("*");
		to = string("*");
	}
	
	Tlink(string& data) : DataEntry("link", "<name><from><to>", data.c_str()) {
		name = string("default_link");
		from = string("*");
		to = string("*");
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the data chunks
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
		
		if(!hasOnlyOne(lines, "link"))
			return 0;
			
		// get the data to the block
		const char* linkData = lines[0].c_str();
		
		// get the name
		vector<string> names = BZWParser::getValuesByKey("name", header, linkData);
		if(!hasOnlyOne(names, "name"))
			return 0;
			
		// get the from
		vector<string> froms = BZWParser::getValuesByKey("from", header, linkData);
		if(!hasOnlyOne(froms, "from"))
			return 0;
			
		// get the to
		vector<string> tos = BZWParser::getValuesByKey("to", header, linkData);
		if(!hasOnlyOne(tos, "to"))
			return 0;
			
		// superclass update
		if(!DataEntry::update(data))
			return 0;
		
		// load in the data
		this->name = names[0];
		this->from = froms[0];
		this->to = tos[0];
		
		return 1;
	}
	
	// toString
	string toString(void) {
		return string("link\n") +
					  "  name " + name + "\n" +
					  "  from " + from + "\n" +
					  "  to " + to + "\n" +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
private:
	string name, from, to;
};

#endif /*LINK_H_*/
