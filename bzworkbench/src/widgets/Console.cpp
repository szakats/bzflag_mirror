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

#include "widgets/Console.h"

// empty constructor
Console::Console(int x, int y, int width, int height, int lineLimit) :
	Fl_Multiline_Input(x, y, width, height) { 
	
	lineLimit = lineLimit;
	writeOn = false;
}
	
// construct with text
Console::Console(int x, int y,int width, int height, const char* text, int lineLimit) :
	Fl_Multiline_Input(x, y, width, height) {

	lineLimit = lineLimit;
	Fl_Multiline_Input::value( text );
	writeOn = false;
}

// construct with a string
Console::Console(int x, int y,int width, int height, string& text, int lineLimit) :
	Fl_Multiline_Input(x, y, width, height) {

	lineLimit = lineLimit;
	value( text );
	writeOn = false;
}

// construct with a vector of strings
Console::Console(int x, int y,int width, int height, vector<string>& text, int lineLimit) :
	Fl_Multiline_Input(x, y, width, height) {
	
	lineLimit = lineLimit;
	value( text );
}

// set read only
void Console::setReadOnly(bool value) {
	writeOn = !value;
	Fl_Multiline_Input::readonly((value == true ? 1 : 0));	
}

// set read/write
void Console::setReadWrite(bool value) {
	writeOn = value;
	Fl_Multiline_Input::readonly((!value == true ? 1 : 0));	
}

// value() with string
void Console::value(string& data) {
	const char* currText = Fl_Multiline_Input::value();
	Fl_Multiline_Input::value( concat( currText, data.c_str() ).c_str() );	
}

// value with a vector of strings
void Console::value(vector<string>& text) {
	string data = "";
	if(text.size() > 0) {
		for(vector<string>::iterator i = text.begin(); i != text.end(); i++) {
			data += (*i) + "\n";
		}	
	}
	
	const char* currText = Fl_Multiline_Input::value();
	
	Fl_Multiline_Input::value( concat( currText, data.c_str() ).c_str() );
}

// add data to the current value (and keep it clamped)
void Console::add(const char* text) {
	string currValue = Fl_Multiline_Input::value();
	Fl_Multiline_Input::value( concat(currValue.c_str(), text).c_str() );	
}
void Console::add(string& text) { add( text.c_str() ); }
void Console::add(vector<string>& text) {
	string data = "";
	if(text.size() > 0) {
		for(vector<string>::iterator i = text.begin(); i != text.end(); i++) {
			data += (*i) + '\n';	
		}	
	}
	add(data.c_str());
}

// clamp the text to a certain number of lines (specifically, keep the last ones)
string Console::concat(const char* _currText, const char* _newText) {
	string currText = _currText;
	string newText = _newText;
	
	// get the number of newlines in newText
	int numNewLines = 0, numCurrNewLines = 0;
	string::size_type index = 0;
	while(true) {
		index = newText.find("\n", index);
		if(index == string::npos)
			break;
			
		numNewLines++;
		index++;
	}
	
	// get the number of newlines in the current text
	index = 0;
	while(true) {
		index = currText.find("\n", index);
		if(index == string::npos)
			break;
			
		numCurrNewLines++;
		index++;
	}
	
	// combine them
	string text = currText + newText;
	int numLines = numNewLines + numCurrNewLines;
	
	// remove that number of lines from the current text
	index = 0;
	while(numLines > lineLimit) {
		index = text.find("\n", index);
		if(index == string::npos) {
			break;
		}
		
		index++;
		numLines--;
	}
		
	return text.substr(index);
}
