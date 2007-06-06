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
	
	// simpler method:  get the key from a line
	static string key(const char* text);
	
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
	
	// get lines starting with a key from a set of keys keys in a section, preserving the order
	static vector<string> getLinesByKeys(vector<string> keys, const char* header, const char* section);
	
	// get all values from a line
	static vector<string> getLineElements(const char* line);
	
private:
	
};

#endif /*BZWPARSER_H_*/
