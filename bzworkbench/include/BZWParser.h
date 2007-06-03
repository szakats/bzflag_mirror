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
	static string value(const char* key, const char* text);
	
	// get the individual lines out of a section
	static vector<string> getLines(const char* header, const char* section);
	
	// get all lines in a section that begin with the given key
	static vector<string> getLinesByKey(const char* key, const char* header, const char* section);
	
	// get the first occurrence of a section from a chunk of text that start with header
	static string getSection(const char* header, const char* text);
	
	// get all sections from a chunk of text that start with header
	static vector<string> getSectionsByHeader(const char* header, const char* text);
	
	// get all values from a key in a section
	static vector<string> getValuesByKey(const char* key, const char* header, const char* section);
	
private:
	
};

#endif /*BZWPARSER_H_*/
