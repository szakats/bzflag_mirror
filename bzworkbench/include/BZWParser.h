#ifndef BZWPARSER_H_
#define BZWPARSER_H_

#include <string>
#include <vector>

using namespace std;

class BZWParser {
public:
	
	// unused
	BZWParser() { }
	virtual ~BZWParser() { }
	
	// simplest method:  get a value from a non-repeatable key in a line
	static string value(string& key, string& text);
	
	// get the individual lines out of a section
	static vector<string> getLines(string& header, string& section);
	
	// get all lines in a section that begin with the given key
	static vector<string> getLinesByKey(string& key, string& header, string& section);
	
	// get the first occurrence of a section from a chunk of text that start with header
	static string getSection(string& header, string& text);
	
	// get all sections from a chunk of text that start with header
	static vector<string> getSectionsByHeader(string& header, string& text);
	
	// get the last occurrence of a section
	static string getLastSection(string& header, string& text) { return *getSectionsByHeader(header, text).rbegin(); }
	
	// get all values from a key in a section
	static vector<string> getValuesByKey(string& key, string& header, string& section);
	
private:
	
};

#endif /*BZWPARSER_H_*/
