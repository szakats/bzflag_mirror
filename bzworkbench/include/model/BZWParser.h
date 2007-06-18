#ifndef BZWPARSER_H_
#define BZWPARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// generic "not found" return value
#define BZW_NOT_FOUND "NOT FOUND"

// a list of all the top-level objects
#define BZW_SUPPORTED_KEYS "<world><waterLevel><options><dynamicColor><texturematrix><material><physics><define>\
						    <group><box><pyramid><teleporter><base><link><mesh><meshbox><meshpyr><arc><cone><sphere>\
						    <tetra><weapon><zone>"

// hierarchy of direct sub-objects (if extant)
// format is:
// <objKey:<subobject1><subobject2>...<subobjectN>> for N subobjects
// Note: this is meant only to map objects to direct subobjects (i.e. mesh only maps to face and drawinfo, but drawinfo
// in a separate declaration maps to lod, and lod in a separate declaration maps to matref).
#define BZW_SUBOBJECTS "<define:<box><pyramid><base><mesh><meshbox><meshpyr><arc><sphere><tetra><teleporter>>\
					   <mesh:<face><drawinfo>>\
					   <drawinfo:<lod>>\
					   <lod:<matref>>"
					   
// unusual terminators (i.e. objects that don't end with "end")
#define BZW_TERMINATORS "<define|enddef><face|endface>"

class BZWParser {
public:
	
	// unused
	BZWParser() { }
	virtual ~BZWParser() { }
	
	// simplest method:  get a value from a non-repeatable key in a line
	static string value(const char* key, const char* text);
	
	// simpler method:  get the key from a line
	static string key(const char* text);
	
	// get the terminator token of a key
	static string terminatorOf(const char* key);
	
	// get the header of a chunk of BZW text
	static string headerOf(const char* text);
	
	// get the hierarchy of an object
	static string hierarchyOf(const char* text);
	
	// get the individual lines out of a section
	static vector<string> getLines(const char* header, const char* section);
	
	// get all lines in a section that begin with the given key
	static vector<string> getLinesByKey(const char* key, const char* header, const char* section);
	
	// get the first occurrence of a section from a chunk of text that start with header
	static string getSection(const char* header, const char* text);
	
	// get all sections from a chunk of text that start with header
	static const vector<string> getSectionsByHeader(const char* header, const char* text);
	static const vector<string> getSectionsByHeader(const char* header, const char* text, const char* footer);
	static const vector<string> getSectionsByHeader(const char* header, const char* text, const char* footer, const char* internalSectionKeys);
	
	// get all values from a key in a section
	static vector<string> getValuesByKey(const char* key, const char* header, const char* section);
	
	// get lines starting with a key from a set of keys keys in a section, preserving the order
	static vector<string> getLinesByKeys(vector<string> keys, const char* header, const char* section);
	
	// get all values from a line
	static vector<string> getLineElements(const char* line);
	static vector<string> getLineElements(const char* line, int count);
	
	// the big tamale: the top-level file loader
	static vector<string> loadFile(const char* filename);
	
private:

};

#endif /*BZWPARSER_H_*/
