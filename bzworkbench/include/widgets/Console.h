/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "../TextUtils.h"
#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Input.H>

#include <string>
#include <vector>

using namespace std;

/**
 * This class is an interactive multi-line text widget
 */
 
class Console : public Fl_Multiline_Input {

public:
	
	// constructors
	Console(int x, int y, int width, int height, int lineLimit = -1);
	Console(int x, int y, int width, int height, const char* text, int lineLimit = -1);
	Console(int x, int y, int width, int height, string& text, int lineLimit = -1);
	Console(int x, int y, int width, int height, vector<string>& text, int lineLimit = -1);
	
	// destructor
	~Console() { }
	
	// read-write enable/disable
	void setReadOnly(bool);
	void setReadWrite(bool);
	
	// modified value() methods
	void value(string& text);
	void value(vector<string>& text);
	
	// concat text to the current value
	void add(const char* text);
	void add(string& text);
	void add(vector<string>& text);
	
	// set the limit on the number of lines
	void setLineLimit(int numLines) { this->lineLimit = numLines; }
	
	// get the line limit
	int getLineLimit();
	
private:
	bool writeOn;
	int lineLimit;
	
	// cap the number of lines by lineLimit (only if lineLimit >= 0)
	string concat(const char* t1, const char* t2);
};

#endif /*CONSOLE_H_*/
