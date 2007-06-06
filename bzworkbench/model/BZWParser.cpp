#include "../include/BZWParser.h"

/**
 * Helper method:  eliminate the whitespace on the ends of the line
 */
string cutWhiteSpace(string line) {
	const char* text = line.c_str();
	unsigned long len = line.length();
	unsigned int startIndex = 0, endIndex = len - 1;
	
	// move the indexes into the string by skipping outside spacess
	while(startIndex < len && text[startIndex] == ' ') { startIndex++; }
	while(endIndex > startIndex && text[endIndex] == ' ') { endIndex--; }
	
	// return the line if there was no white space to cut
	if(startIndex == len)
		return line;
	
	// return the inner content
	return line.substr(startIndex, endIndex - startIndex + 1);
}

/**
 * Helper method:  return a substring from the beginning of the string to the next occurence of '\n'
 */
string cutLine(string text) {
	string::size_type index = text.find("\n", 0);
	return text.substr(0, index);	
}

/**
 * Helper method:  determines whether or not the passed key is the key of the passed line
 */
 bool isKey(string key, string line) {
 	line = cutWhiteSpace(line);
 	string::size_type index = line.find(key, 0);
 	if(index == 0)
 		return true;
 	return false;
 }

/**
 * Get the value text from a line
 */
string BZWParser::value(const char* _key, const char* _text) {
	string line = cutWhiteSpace(string(_text));
	string key = string(_key);
	
	unsigned int startIndex = line.find(key, 0);
	
	// stop if we didn't find the key
	if(startIndex == string::npos)
		return string("not found");
		
	// advance to the start of the value
	startIndex += key.length() + 1;
	
	// stop if the first key is the value
	if(startIndex > line.length())
		return key;
	
	// get the value
	string value = line.substr(startIndex, line.length() - startIndex);
	
	// trunicate the value and return it
	return cutWhiteSpace(value);
}

/**
 * Get the key of a line
 */
string BZWParser::key(const char* _text) {
	string text = cutWhiteSpace(_text);
	string::size_type index = text.find(" ", 0);
	return text.substr(0, index);
}

/**
 * This method will extract the key-value lines from a section, given the object name of the section and its text
 */
vector<string> BZWParser::getLines(const char* _start, const char* _text) {
	string start = string(_start);
	string text = string(_text);
	
	string header = cutWhiteSpace(start);
	string section = cutWhiteSpace(text);
	
	vector<string> lines = vector<string>();
	
	// find the start of the section
	while(true) {
		// get the next line
		string currLine = cutLine(section);
		
		// prevent an exception:  check to see if we can even get the next line (if we can't, then there is no section in this text)
		if(section.length() < currLine.length() + 1)
			return lines;
			
		// advance the section
		section = section.substr(currLine.length() + 1);
		
		// if we found the header then break
		if(currLine.compare(header) == 0)
			break;
	}
	
	// read in lines
	while(true) {
		// get the next line
		string currLine = cutLine(section);
		
		// break if we get to "end"
		if(currLine.compare("end") == 0)
			break;
		
		// add the line (remove whitespace)
		lines.push_back(cutWhiteSpace(currLine));
		
		// check to see if we ran out of room (then return if so)
		if(section.length() < currLine.length() + 1)
			return lines;
			
		// advance the section
		section = section.substr(currLine.length() + 1);
	}
	
	return lines;
}

/**
 * This method gets all the lines in a section starting with a given key
 */
vector<string> BZWParser::getLinesByKey(const char* _key, const char* _header, const char* _text) {
	
	string key = cutWhiteSpace(string(_key));
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	// get all lines from the section
	vector<string> lines = BZWParser::getLines(_header, _text);
	
	// the lines starting with key
	vector<string> ret = vector<string>();
	
	// find the lines with the matching key
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		if(isKey(key, *i)) {
			ret.push_back(*i);
		}
	}
	
	return ret;
}

/*
 * This method reads a section that starts with header from a chunk of text 
 */
string BZWParser::getSection(const char* _header, const char* _text) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	// find header occurence
	string::size_type index = text.find(header, 0);
	
	// if we didn't find anything then return an empty string
	if(index == string::npos)
		return string("");
	
	// find the end of the section
	string::size_type end = text.find("end\n", index);
	
	// if we didn't find anything then return the rest of the section
	if(end == string::npos)
		return text.substr(index);
	
	return text.substr(index, end - index);
}

/**
 * This method finds all occurences of a section (by looking for its header) in a chunk of text
 */
vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	vector<string> sections = vector<string>();
	
	string::size_type index = 0, end = 0;
	
	while(true) {
		// find the first occurence of the header
		index = text.find(header, index);
		
		// if we're at the end, then break
		if(index == string::npos)
			break;
			
		// find the end of the section
		end = text.find("end\n", index);
		
		// if there's no end, it's probably a syntax error so skip it
		if(end == string::npos) {
			printf("BZWParser:  WARNING! No \"end\" tag found for \"%s\" at char# %d; skipping...\n", header.c_str(), index);
			sections.push_back( text.substr(index) );
			break;
		}
		
		string section = text.substr( index, (end + 4) - index );
		// add the section
		sections.push_back(section);
		
		// advance the text
		text = text.substr(end + 1);
	}
	
	return sections;
}

/**
 * This method gets the list of all values referenced by a key in a segment of text (usually a section)
 */
vector<string> BZWParser::getValuesByKey(const char* _key, const char* _header, const char* _text) {
	string key = cutWhiteSpace(string(_key));
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	// get all lines with the key
	vector<string> lines = BZWParser::getLinesByKey(_key, _header, _text);
	
	// return value
	vector<string> ret = vector<string>();
	
	// get the values and load up ret
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		string value = BZWParser::value(_key, (char*)i->c_str());
		ret.push_back( value );
	}
	
	return ret;
}

/**
 * This method gets all values to a set of keys and preserves the order in which the occur
 */
 
vector<string> BZWParser::getLinesByKeys(vector<string> keys, const char* _header, const char* _section) {
	string header = cutWhiteSpace(_header);
	string section = cutWhiteSpace(_section);
	
	vector<string> lines = BZWParser::getLines(_header, _section);
	
	vector<string> ret = vector<string>();
	
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		for(vector<string>::iterator j = keys.begin(); j != keys.end(); j++) {
			if(isKey(*j, *i)) {
				ret.push_back(*i);
				break;	
			}	
		}	
	}
	
	return ret;
}

/**
 * This method gets all the elements in a line separated by one or more spaces
 */
vector<string> BZWParser::getLineElements(const char* data) {
	vector<string> ret = vector<string>();
	string line = cutWhiteSpace(string(data)) + " ";
	
	// separate all elements by finding the " "s
	while(true) {
		if(line.length() < 1)
			break;
			
		string::size_type spaceIndex = line.find(" ", 0);
		if(spaceIndex == string::npos)
			break;
			
		string element = line.substr(0, spaceIndex);
		
		ret.push_back(element);
		
		line = line.substr(spaceIndex + 1);
	}
	
	return ret;
}
