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

#include "model/BZWParser.h"
#include "model/Model.h"

Model* BZWParser::_modelRef = NULL;

/**
 * Helper method:  eliminate the whitespace on the ends of the line
 */
string cutWhiteSpace(string line) {
	// don't bother with empty strings
	if( line.length() <= 0 )
		return line;

	const char* text = line.c_str();
	unsigned long len = line.length();
	unsigned int startIndex = 0;
	unsigned int endIndex = len - 1;

	// cut any comments
	string::size_type commentIndex = line.find("#", 0);
	if(commentIndex != string::npos) {
		line = line.substr(0, commentIndex);
		if (line.length() == 0)
			return string("");
		endIndex = line.length() - 1;
	}

	// move the indexes into the string by skipping outside spacess
	while(startIndex < len && (text[startIndex] == ' ' || text[startIndex] == 9 || text[startIndex] == '\n' || text[startIndex] == '\r')) { startIndex++; }
	while(endIndex > startIndex && (text[endIndex] == ' ' || text[endIndex] == 9 || text[endIndex] == '\n' || text[endIndex] == '\r')) { endIndex--; }

	// return the line if there was no white space to cut
	if(startIndex == len)
		return line;

	if( endIndex < startIndex)
		return string(" ");
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
 * Helper method:  does a text chunk have another line in it?
 */

bool hasLine(string text) {
	string::size_type index = text.find("\n", 0);
	if(index == string::npos)
		return false;

	return true;
}

/**
 * Helper method:  determines whether or not the passed key is the key of the passed line
 */
 bool isKey(string key, string line) {
 	line = cutWhiteSpace(line);

	string::size_type index = line.find(key);
	if (index == 0) {
 		// sometimes the key IS the line, so there won't be a trailing space
 		if (line.size() == key.size())
 			return true;

		// ...but normally a key must be followed by whitespace
		string::size_type nextchar = index + key.size();
 		if (line[nextchar] == ' ')
 			return true;
		else if (line[nextchar] == '\t')
 			return true;
	}

	return false;
 }

/**
 * Helper method: replaces sequential spaces and tabs with a single space
 */
string removeWhiteSpace(const string& line) {
	// don't bother with empty strings
	if( line.length() <= 0 )
		return line;

	string ret;
	bool whitespace = false;
	string::const_iterator it;
	for ( it = line.begin(); it != line.end(); it++ ) {
		if (*it == ' ' || *it == '\t') {
			if (!whitespace) {
				// hit whitespace for the first time
				ret += ' ';
				whitespace = true;
			}
			else // don't add more whitespace
				continue;
		}
		else {
			// hit a non-whitespace char, reset bool
			ret += *it;
			whitespace = false;
		}
	}

	return ret;
}

/**
 * Get the value text from a line
 */
string BZWParser::value(const char* _key, const char* _text) {
	string line = cutWhiteSpace(string(_text));
	string key = string(_key);

	size_t startIndex = line.find(key, 0);

	// stop if we didn't find the key
	if(startIndex == string::npos)
		return string(BZW_NOT_FOUND);

	// advance to the start of the value
	startIndex += key.length();

	// stop if the first key is the value
	if(startIndex > line.length())
		return key;

	// get the value
	string value = line.substr(startIndex);

	// trunicate the value and return it
	return cutWhiteSpace(value);
}

/**
 * Get the key of a line
 */
string BZWParser::key(const char* _text) {
	string text = cutWhiteSpace(_text);
	string::size_type index = text.find(" ", 0);
	if(index == string::npos)
		return text;
	return text.substr(0, index);
}

/**
 * This method returns the terminator token of a key.  Usually, it's "end"
 */

string BZWParser::terminatorOf(const char* _text) {
	if( _modelRef == NULL )
		return "";
	string key = string(_text);
	string terms = _modelRef->_getSupportedTerminators();

	// see of there's an unusual terminator
	string::size_type start = terms.find("<" + key + "|", 0);
	if(start != string::npos) {
		start += 2 + key.length();
		string::size_type end = terms.find(">", start);
		if(end != string::npos)
			return terms.substr(start, end - (start));
		else
			return terms.substr(start);
	}

	return string("end");
}

/**
 * This method gets the immediate subobjects of an object (usually its "");
 * The format of the hierarchy is <subobject1><subobject2>...<subobjectN> for N subobjects
 */
string BZWParser::hierarchyOf(const char* _text) {
	if( _modelRef == NULL )
		return "";
	string key = string(_text);
	string terms = _modelRef->_getSupportedHierarchies();

	// find the hierarchy
	string::size_type start = terms.find("<" + key + ":", 0);
	if(start != string::npos) {
		string::size_type end = terms.find(">>", start+1);
		if(end != string::npos)
			return terms.substr(start, end+2 - start);
		else
			return terms.substr(start);
	}
	return string("");
}

/**
 * This method returns the header of a chunk of text
 */

string BZWParser::headerOf(const char* _text) {
	vector<string> elements = BZWParser::getLineElements(_text, 2);
	if(elements.size() >= 1)
		return elements[0];
	else
		return string(BZW_NOT_FOUND);
}
/**
 * This method will extract the key-value lines from a section, given the object name of the section and its text
 */
vector<string> BZWParser::getLines(const char* _start, const char* _text) {
	string start = string(_start);
	string text = string(_text);

	string header = cutWhiteSpace(start);
	string section = cutWhiteSpace(text);
	string terminator = BZWParser::terminatorOf(header.c_str());

	vector<string> lines = vector<string>();

	// find the start of the section
	while(true) {
		// make sure we have line enough to cut
		if(section.length() < header.length())
			break;

		// get a line
		string currLine = cutLine(section);

		// try to find the header in it
		string::size_type index = currLine.find(header, 0);

		if(index == 0)
			break;

		// advance the line
		section = section.substr(currLine.length() + 1);
	}

	// read in lines
	while(true) {
		// get the next line
		string currLine = cutLine(section);

		// break if we get to "end"
		if(currLine.compare(terminator) == 0)
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
	if( lines.size() > 0 ) {
		for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
			if(isKey(key, *i)) {
				ret.push_back(*i);
			}
		}
	}

	return ret;
}


/**
 * This method finds all occurences of a section (by looking for its header) in a chunk of text
 * Only use this method for sections with no sub-sections
 */
const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text, const char* _footer) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text)) + " \n";
	string footer = cutWhiteSpace(string(_footer));

	// printf("BZWParser: <%s|%s>\n", header.c_str(), footer.c_str());

	vector<string> sections = vector<string>();
	int cnt = 0;		// how many sections

	while(true) {
		string section = string("");

		string currLine, line;

		bool found = false;

		// find the header
		while(true) {

			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}

			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);

			// chunk up the line
			vector<string> lineElements = BZWParser::getLineElements(line.c_str());

			if(lineElements.size() == 0)
				lineElements.push_back(" ");

			// printf("BZWParser: first element is |%s|\n", lineElements[0].c_str());

			// see if it has the header
			if(lineElements[0].compare(0, header.length(), header) == 0) {
				section += line + "\n";
				text = text.substr(currLine.length() + 1);
				found = true;
				// printf("BZWParser: found header |%s|\n", line.c_str());
				break;
			}

			text = text.substr(currLine.length() + 1);
		}

		found = false;
		// find the footer
		while(true) {
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}

			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);

			// printf("BZWParser: line--> |%s|\n", line.c_str());
			// chunk up the line
			vector<string> lineElements = BZWParser::getLineElements(line.c_str());

			if(lineElements.size() == 0)
				lineElements.push_back(" ");

			// printf("BZWParser: first element is |%s|\n", lineElements[0].c_str());

			// add it to the section
			section += line + "\n";

			// see if it has the end
			if(lineElements[0].compare(0, footer.length(), footer) == 0) {
				found = true;
				// printf("BZWParser: found footer; breaking...\n");
				break;
			}


			text = text.substr(currLine.length() + 1);
		}

		if(!found)
			break;
		else {
			sections.push_back(section);
			cnt++;
		}
	}
	// add SOMETHING if we found nothing
	if(cnt == 0)
		sections.push_back(BZW_NOT_FOUND);

	return sections;
}

const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text) {
	return BZWParser::getSectionsByHeader(_header, _text, BZWParser::terminatorOf(_header).c_str());
}

/**
 * General case of getSectionsByHeader.
 * This method, unlike the others, anticipates subobjects listed in internalSectionKeys, and won't be
 * fooled by internal "end" statements.
 * Format of internalSectionKeys is "<key1><key2>...<keyN>" for N keys.
 * Use this method for sections with sub-objects.
 */

const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text, const char* _footer, const char* internalSectionKeys, const char* sectionsToIgnore) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text)) + " \n";
	string footer = cutWhiteSpace(string(_footer));
	string keyList = cutWhiteSpace(string(internalSectionKeys));
	string ignoreList = cutWhiteSpace(string(sectionsToIgnore));
	int sectionDepth = 0;	// determines the section depth (will be positive if inside a subobject)

	vector<string> sections = vector<string>();
	int cnt = 0;		// how many sections

	while(true) {
		string section = string("");

		string currLine, line;

		bool found = false;

		// find the header
		while(true) {

			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}

			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);

			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());

			if(lineElements.size() == 0)
				lineElements.push_back(" ");

			// see if it has the header
			if(lineElements[0].compare(header) == 0) {
				section += line + "\n";
				text = text.substr(currLine.length() + 1);
				found = true;
				break;
			}

			text = text.substr(currLine.length() + 1);
		}

		found = false;

		// footer stack
		vector<string> footerStack = vector<string>();
		vector<string> headerStack = vector<string>();
		vector<string> allowedHeaderStack = vector<string>();

		footerStack.push_back(footer);
		headerStack.push_back(header);
		allowedHeaderStack.push_back(header);

		string subobjects = BZWParser::hierarchyOf( header.c_str() );

		bool listening = true;

		// find the footer
		while(true) {
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}

			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);

			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());

			if(lineElements.size() == 0)
				lineElements.push_back(" ");

			// see if this is a key
			if(keyList.find("<" + lineElements[0] + ">") != string::npos || subobjects.find("<" + lineElements[0] + ">") != string::npos) {
				sectionDepth++;
				footerStack.push_back( BZWParser::terminatorOf(lineElements[0].c_str()) );
				headerStack.push_back(lineElements[0]);

				// see if we should listen to lines within this subobject
				if(ignoreList.find("<" + lineElements[0] + ">") == string::npos) {
					allowedHeaderStack.push_back(lineElements[0]);
				}
				else {
					listening = false;
				}

				subobjects = BZWParser::hierarchyOf(lineElements[0].c_str());
			}

			// add it to the section if its relevant
			if(listening) {
				section += line + "\n";
			}

			// see if this is the end of a subobject
			if(footerStack[ footerStack.size() - 1 ] == lineElements[0]) {
				sectionDepth--;
				if(sectionDepth < 0) {
					found = true;
					section += BZWParser::terminatorOf(lineElements[0].c_str()) + "\n";
					break;
				}
				footerStack.pop_back();
				headerStack.pop_back();
				if(headerStack.size() < allowedHeaderStack.size()) {
					allowedHeaderStack.pop_back();
					listening = true;
				}


				subobjects = BZWParser::hierarchyOf(headerStack[headerStack.size()-1].c_str());

			}

			text = text.substr(currLine.length() + 1);
		}

		if(!found)
			break;
		else {
			sections.push_back(section);
			cnt++;
		}
	}
	// add SOMETHING if we found nothing
	if(cnt == 0)
		sections.push_back(BZW_NOT_FOUND);

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
	if( lines.size() > 0 ) {
		for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
			string value = BZWParser::value(_key, i->c_str());
			ret.push_back( value );
		}
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
			if(isKey(*j, *i) || *j == *i) {
				ret.push_back(*i);
				break;
			}
		}
	}

	return ret;
}

/**
 * Get values corresponding to keys in the order in which they appear
 */

vector<string> BZWParser::getValuesByKeys(vector<string> keys, const char* header, const char* text ) {
	vector<string> ret = vector<string>();

	for (vector<string>::iterator i = keys.begin(); i != keys.end(); i++) {
		vector<string> vals = getValuesByKey( (*i).c_str(), header, text );

		for (vector<string>::iterator v = vals.begin(); v != vals.end(); v++) {
			ret.push_back(*v);
		}
	}

	return ret;
}

/**
 * Get the values corresponding to a key and a list of faces
 */
vector<string> BZWParser::getValuesByKeyAndFaces(const char* key, vector<string> faces, const char* header, const char* section ) {
	// build a list of keys
	vector<string> keys;

	for (vector<string>::iterator itr = faces.begin(); itr != faces.end(); itr++) {
		keys.push_back(*itr + " " + key);
	}

	return getValuesByKeys( keys, header, section );
}

/**
 * This method gets all the elements in a line separated by one or more spaces
 * Pass -1 to count to get all elements
 */
vector<string> BZWParser::getLineElements(const char* data, int count) {
	vector<string> ret = vector<string>();

	string line = removeWhiteSpace(string(data));
	line = cutWhiteSpace(line) + " ";

	// separate all elements by finding the " "s
	while(count != 0) {
		if(line.length() < 1)
			break;

		string::size_type spaceIndex = line.find(" ", 0);
		string::size_type retIndex = line.find("\n", 0);
		if(spaceIndex == string::npos &&
		   retIndex == string::npos)
			break;

		string element = line.substr(0, min(spaceIndex, retIndex));

		ret.push_back(element);

		line = line.substr(spaceIndex + 1);

		count--;
	}

	return ret;
}
vector<string> BZWParser::getLineElements(const char* text) { return BZWParser::getLineElements(text, -1); }

/**
 * Helper method:
 * get a vector of subobjects from an object (modified hierarchyOf())
 */

vector<string> subobjectsOf(const char* obj) {
	string subobjects = BZWParser::hierarchyOf(obj);
	vector<string> ret = vector<string>();

	if(subobjects.length() == 0) {
		return ret;
	}

	// find all sub-objects
	string::size_type start = 0;
	string::size_type end = 1;

	while(true) {
		start = subobjects.find("<", end);
		end = subobjects.find(">", start+1);

		if(start == string::npos)
			break;

		string subobj = subobjects.substr(start+1, end - (start+1));

		ret.push_back( subobj );
	}

	return ret;
}

/**
 * Get all sections from a chunk of text
 */

const vector<string> BZWParser::getSections(const char* _text) {

	string text = string(_text);

	vector<string> ret = vector<string>();

	// don't bother if there is no model to query
	if( _modelRef == NULL )
		return ret;

	// start reading the stuff in
	string buff, key, objstr, line, supportedKeys = _modelRef->_getSupportedObjects(), hierarchy = _modelRef->_getSupportedHierarchies();
	vector<string> lineElements = vector<string>();

	while(true) {

		// read in lines until we find a key
		buff = cutWhiteSpace( cutLine( text ) );

		// advance the line
		string::size_type eol = text.find("\n", 0);
		if(eol == string::npos)
			break;

		text = text.substr(eol+1);

		// get the line elements
		lineElements = BZWParser::getLineElements(buff.c_str());

		// if there are no line elements, continue
		if(lineElements.size() == 0)
			continue;

		// get the key
		key = BZWParser::key( lineElements[0].c_str() );

		// is it a valid key?
		if( supportedKeys.find( "<" + key + ">", 0 ) != string::npos) {
			// if so, read in the object
			objstr = buff + "\n";

			// depth count--determines how deep the parser is in an object hierarchy
			// initialized to 1 now that we are in an object
			int depthCount = 1;

			// get any sub-object keys
			vector<string> subobjects = subobjectsOf( key.c_str() );

			// get number of subobjects
			int numSubobjects = subobjects.size();

			// terminator stack
			vector<string> terminatorStack = vector<string>();

			// header stack
			vector<string> headerStack = vector<string>();

			// add the header token of the base object
			headerStack.push_back( key );

			// add the terminator token of the base object
			terminatorStack.push_back( BZWParser::terminatorOf(key.c_str()) );

			// loop through the text until we have depth 0 (i.e. we exit the object) or we run out of text
			while(depthCount > 0) {
				// get a line
				buff = cutWhiteSpace( cutLine( text ) );

				// advance the line
				string::size_type eol = text.find("\n", 0);
				if(eol == string::npos)
					break;

				text = text.substr(eol+1);

				// trim buff
				line = buff;

				// don't continue of there was nothing to begin with
				if(line.length() == 0)
					continue;

				line += " ";

				// see if it's a subobject (if so, increase depthcount, save the subobject header, and load in new subobject keys)
				if(numSubobjects > 0) {
					for(int i = 0; i < numSubobjects; i++) {
						if( line.find( subobjects[i] + " ", 0 ) == 0 ) {
							depthCount++;
							terminatorStack.push_back( BZWParser::terminatorOf(subobjects[i].c_str()) );
							headerStack.push_back( subobjects[i] );
							subobjects = subobjectsOf( subobjects[i].c_str() );
							numSubobjects = subobjects.size();

							break;
						}
					}
				}

				// see if we're at an "end" (if so, decrease depthcount)
				if(line.find(terminatorStack[ terminatorStack.size() - 1 ] + " ", 0) != string::npos) {
					depthCount--;
					if(depthCount < 0) {
						objstr += line + "\n";
						break;
					}

					terminatorStack.pop_back();
					headerStack.pop_back();

					if(headerStack.size() > 0)
						subobjects = subobjectsOf( headerStack[ headerStack.size() - 1 ].c_str() );
					else
						subobjects.clear();

					numSubobjects = subobjects.size();
				}

				objstr += line + "\n";

			}

			// add the object to ret
			ret.push_back(objstr);
		}
	}

	return ret;
}

/**
 * Find sections in a vector of sections by header
 */

const vector<string> BZWParser::findSections(const char* _header, vector<string>& sections) {
	vector<string> ret = vector<string>();

	// iterate over the passed sections
	if(sections.size() > 0) {
		for(vector<string>::iterator i = sections.begin(); i != sections.end(); i++) {
			vector<string> lineElements = BZWParser::getLineElements(i->c_str(), 1);
			if(lineElements.size() > 0) {
				if(lineElements[0] == _header) {
					ret.push_back(*i);
				}
			}
		}
	}

	if(ret.size() == 0)
		ret.push_back(BZW_NOT_FOUND);

	return ret;
}


/**
 * The top-level file loader.
 * Loads a .bzw file and returns a vector of strings, where each string contains an object's chunk of BZW text.
 * This is almost identical to BZWParser::getSections(); the only differences are file I/O-related.
 * The excuse for code duplication here is that this code executes MUCH faster than code that just loads
 * the entire file into RAM and calls BZWParser::getSections() to parse it.
 */

vector<string> BZWParser::loadFile(const char* filename) {

	vector<string> ret = vector<string>();

	if( _modelRef == NULL )
		return ret;

	ifstream fileInput(filename);

	// if its not open, its not there
	if(!fileInput.is_open()) {
		printf("BZWParser::loadFile(): Error! Could not open input stream\n");
		return vector<string>();
	}

	// start reading the stuff in
	string buff, key, objstr, line, supportedKeys = _modelRef->_getSupportedObjects(), hierarchy = _modelRef->_getSupportedHierarchies();
	vector<string> lineElements = vector<string>();

	while(!fileInput.eof()) {

		// read in lines until we find a key
		getline( fileInput, buff );

		buff = cutWhiteSpace( buff );

		// get the line elements
		lineElements = BZWParser::getLineElements(buff.c_str());

		// if there are no line elements, continue
		if(lineElements.size() == 0)
			continue;

		// get the key
		key = BZWParser::key( lineElements[0].c_str() );

		// is it a valid key?
		if( supportedKeys.find( "<" + key + ">", 0 ) != string::npos) {
			// if so, read in the object
			objstr = buff + "\n";

			// depth count--determines how deep the parser is in an object hierarchy
			// initialized to 1 now that we are in an object
			int depthCount = 1;

			// get any sub-object keys
			vector<string> subobjects = subobjectsOf( key.c_str() );

			// get number of subobjects
			int numSubobjects = subobjects.size();

			// terminator stack
			vector<string> terminatorStack = vector<string>();

			// header stack
			vector<string> headerStack = vector<string>();

			// add the header token of the base object
			headerStack.push_back( key );

			// add the terminator token of the base object
			terminatorStack.push_back( BZWParser::terminatorOf(key.c_str()) );

			// loop through the text until we have depth 0 (i.e. we exit the object) or we run out of text
			while(!fileInput.eof() && depthCount > 0) {
				// get a line
				getline( fileInput, buff );

				// trim buff
				buff = cutWhiteSpace(buff);
				line = buff;

				// don't continue of there was nothing to begin with
				if(line.length() == 0)
					continue;

				line += " ";

				// see if it's a subobject (if so, increase depthcount, save the subobject header, and load in new subobject keys)
				if(numSubobjects > 0) {
					for(int i = 0; i < numSubobjects; i++) {
						if( line.find( subobjects[i] + " ", 0 ) == 0 ) {
							depthCount++;
							terminatorStack.push_back( BZWParser::terminatorOf(subobjects[i].c_str()) );
							headerStack.push_back( subobjects[i] );
							subobjects = subobjectsOf( subobjects[i].c_str() );
							numSubobjects = subobjects.size();

							break;
						}
					}
				}

				// see if we're at an "end" (if so, decrease depthcount)
				if(line.find(terminatorStack[ terminatorStack.size() - 1 ] + " ", 0) != string::npos) {
					depthCount--;
					if(depthCount < 0) {
						objstr += line + "\n";
						break;
					}

					terminatorStack.pop_back();
					headerStack.pop_back();

					if(headerStack.size() > 0)
						subobjects = subobjectsOf( headerStack[ headerStack.size() - 1 ].c_str() );
					else
						subobjects.clear();

					numSubobjects = subobjects.size();
				}

				objstr += line + "\n";

			}

			// add the object to ret
			ret.push_back(objstr);

		}
	}

	return ret;
}

