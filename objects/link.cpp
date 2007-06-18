#include "../include/objects/link.h"

// constructor
Tlink::Tlink() : DataEntry("link", "<name><from><to>") {
	name = string("default_link");
	from = string("*");
	to = string("*");
}

// constructor with data
Tlink::Tlink(string& data) : DataEntry("link", "<name><from><to>", data.c_str()) {
	name = string("default_link");
	from = string("*");
	to = string("*");
	
	this->update(data);	
}

// getter
string Tlink::get(void) { return this->toString(); }

// setter
int Tlink::update(string& data) {
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
	if(names.size() > 1) {
		printf("link::update(): Error! Defined \"name\" %d times!\n", names.size());
		return 0;
	}
		
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
	this->name = (names.size() != 0 ? names[0] : "");
	this->from = froms[0];
	this->to = tos[0];
	
	return 1;
}

// toString
string Tlink::toString(void) {
	return string("link\n") +
				  (name.length() != 0 ? "  name " + name : "# name") + "\n" +
				  "  from " + from + "\n" +
				  "  to " + to + "\n" +
				  "end\n";
}

// render
int Tlink::render(void) {
	return 0;	
}
