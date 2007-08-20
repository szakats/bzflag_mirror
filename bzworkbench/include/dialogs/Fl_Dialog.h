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

#ifndef FL_DIALOG_H_
#define FL_DIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// This class is a simple dialog box with OK and CANCEL buttons, often used as the parent class of additional classes
class Fl_Dialog : public Fl_Window
{
public:
	
	// bit masks that can be |-ed together in the flags paramter of some constructors
	static const unsigned long Fl_OK = 0x1;
	static const unsigned long Fl_CANCEL = 0x2;
	
	// constructors (see Fl_Dialog.cpp for details)
	Fl_Dialog(const char* title);
	Fl_Dialog(const char* title, unsigned long flags);
	Fl_Dialog(const char* title, int width, int height, unsigned long flags);
	
	// destructor
	virtual ~Fl_Dialog();
	
	// methods (see Fl_Dialog.cpp for details)
	bool setOKEventHandler(void (*handler)(Fl_Widget* w, void* data), void* data);
	bool setCancelEventHandler(void (*handler)(Fl_Widget* w, void* data), void* data);

protected:
	// default window dimensions
	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 300;
	
private:
	
	// default dimensions of the OK button
	static const int OK_DEFAULT_WIDTH = 75;
	static const int OK_DEFAULT_HEIGHT = 20;
	
	// default dimensions of the CANCEL button
	static const int CANCEL_DEFAULT_WIDTH = 75;
	static const int CANCEL_DEFAULT_HEIGHT = 20; 
	
	// default coordinates of the OK/CANCEL button group
	static const int DEFAULT_GROUP_X = 0;
	static const int DEFAULT_GROUP_Y = DEFAULT_HEIGHT - 40;
	
	// default dimensions of the OK/CANCEL button group
	static const int DEFAULT_GROUP_WIDTH = DEFAULT_WIDTH;
	static const int DEFAULT_GROUP_HEIGHT = 40;
	
	// default group--contains the OK and CANCEL buttons
	Fl_Group* defaultGroup;
	
	// not always used; pointers to OK and CANCEL button widgets
	Fl_Button *OK_button, *CANCEL_button;
	
	// constructor helper methods
	void doConstructor(int width, int height);
	void finishConstructor(void);
	void setUpButtons(unsigned long flags);
};

#endif /*FL_DIALOG_H_*/
