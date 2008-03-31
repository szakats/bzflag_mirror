/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "ftoa.h"
#include "model/BZWParser.h"
#include "UpdateMessage.h"

// the generic map object class
class DataEntry {
public:
	
	virtual string get(void) = 0;
	virtual int update(string& data) {
		text = data;
		setChanged();
		return 1;
	}
	
	virtual int update(UpdateMessage& msg) { return 1; }
	
	virtual string toString() {
		return string("# unknown DataEntry");
	}
	
	// initialize the header and keys
	DataEntry(const char* _header, const char* _keys) {
		header = _header;
		keys = _keys;
		text = "";
		setChanged();
	}
	
	// initialize the header and keys and data
	DataEntry(const char* _header, const char* _keys, const char* _data) {
		header = string(_header);
		keys = string(_keys);
		text = string(_data);	
		setChanged();
	}
	
	
	virtual ~DataEntry() { }
	
	// is the key supported?
	bool isKey(const string& key) {
		string::size_type index = keys.find("<" + key + ">", 0);
		if(index != string::npos)
			return true;
		return false;
	}
	// bool isKey(const char* key) { string k = string(key); return isKey(k); }
	
	
	// does a line have a key?
	bool hasKey(const string& line) {
		// get the keys
		vector<string> _keys = getKeys();
		
		// determine whether or not the line starts with one of them
		for(vector<string>::iterator i = _keys.begin(); i != _keys.end(); i++) {
			if(line.find(*i) == 0)
				return true;	
		}
		
		return false;
	}
	
	// helper method:  ensures that a key occurs only once (useful for calling from update())
	bool hasOnlyOne(vector<string> lines, const char* key) {
		if( lines.size() != 1) {
			printf("%s::update():  Error!  Defined \"%s\" %d times!\n", header.c_str(), key, (int)lines.size());
			return false;	
		}
		return true;
	}
	
	// helper method:  ensures that a line has the proper number of elements
	bool hasNumElements(string& data, unsigned int numElements) {
		if(BZWParser::getLineElements(data.c_str()).size() != numElements) {
			printf("Error!  Indecipherable line \"%s\"\n", data.c_str());
			return false;
		}
		return true;
	}
	bool hasNumElements(const char* data, unsigned int numElements) { string tmp = data; return hasNumElements(tmp, numElements); }
	
	// helper method:  prints out unused key/value pairs
	string getUnusedText() {
		string ret = string("");
		
		// get the section
		const char* hdr = header.c_str();
		
		// get the lines
		vector<string> sections = BZWParser::getSectionsByHeader(hdr, text.c_str());
		if(sections[0] == BZW_NOT_FOUND) {
			// printf("%s::update(): Error! Could not restore original text!\n", hdr);
			return ret;
		}
			
		const char* data = sections[0].c_str();
		
		// rip out the actual data lines
		vector<string> lines = BZWParser::getLines(hdr, data);
		
		// don't even bother iterating if there are no lines at all
		if(lines.size() <= 1)
			return ret;
		
		// iterate over the lines (skip the first one; that's the header line)
		// and find out which ones do NOT have supported keys
		for(vector<string>::iterator i = lines.begin() + 1; i != lines.end(); i++) {
			
			// check and see if the line does NOT have a supported key, and if not, add the line to ret
			if(!hasKey(*i)) {
				ret += "  " + (*i);
				// printf("%s: Warning! Indecipherable line \"%s\"\n", hdr, i->c_str());	
			}
		}
		
		// return the lines with unsupported keys
		return ret;
	}
	
	// get the supported keys
	string getKeyString(void) { return keys; }
	
	// get the keys as a vector of strings
	vector<string> getKeys(void) {
		vector<string> ret = vector<string>();
		
		// get the string
		string _keys = getKeyString();
		
		// parse the string
		while(true) {
			// find a <
			string::size_type start = _keys.find("<", 0);
			
			// find a >
			string::size_type end = _keys.find(">", 0);
			
			// get the stuff in between
			string key = _keys.substr(start+1, end - (start+1));
			
			// add it
			ret.push_back(key);
			
			// break if we're out of keys
			if(_keys.find("<", end) == string::npos)
				break;
			
			// remove the key from keys
			_keys = _keys.substr(end + 1);
			
		}
		
		// return the keys
		return ret;
	}
	
	// get the header
	string getHeader(void) { return header; }
	
	//  get the original text
	string getText() { return text; }
	
	// set the header
	void setHeader(const char* c) { header = string(c); }
	
	// set the keys
	void setKeys(const char* c) { keys = string(c); }
	
	// set changed
	void setChanged() { changed = true; }
	void setChanged(bool value) { changed = value; }
	
protected:
	string header;
	string keys;
	string text;
	bool changed;
};

#endif /*DATAENTRY_H_*/
