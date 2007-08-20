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

#ifndef CONSOLEWINDOW_H_
#define CONSOLEWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include "../widgets/Console.h"

class ConsoleWindow : public Fl_Window {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 100;
	
	// constructor
	ConsoleWindow(int lineLimit = -1);
	
	// destructor
	~ConsoleWindow();
	
	// static constructor
	static ConsoleWindow* init(int lineLimit = -1) { return new ConsoleWindow(lineLimit); }
	
	// static updater
	static void output(const char* text, ...);
	
private:

	// initialized flag
	static bool initialized;
	
	// the console widget
	static Console* console;
};

#endif /*CONSOLEWINDOW_H_*/
