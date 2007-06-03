#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "ftoa.h"

// the generic map object class
class DataEntry {
public:
	
	virtual void get(string&) = 0;
	virtual void update(string&) = 0;
	
	virtual string toString() {
		return string("# unknown DataEntry");
	}
	
	// initialize the header and keys
	DataEntry(const char* header, const char* keys) {
		this->header = string(header);
		this->keys = string(keys);
	}
	
	
	virtual ~DataEntry() { }
	
	// is the key supported?
	bool isKey(string& key) {
		string::size_type index = this->keys.find("<" + key + ">", 0);
		if(index != string::npos)
			return true;
		return false;
	}
	
	// get the supported keys
	string getKeys(void) { return keys; }
	
	// get the header
	string getHeader(void) { return header; }
	
	
private:
	string header;
	string keys;
};

#endif /*DATAENTRY_H_*/
