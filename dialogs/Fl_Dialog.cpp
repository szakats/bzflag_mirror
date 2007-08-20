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

#include "../include/dialogs/Fl_Dialog.h"

// a helper method to build the default group and initialize member variables
void Fl_Dialog::doConstructor(int width, int height) {
	defaultGroup = new Fl_Group(DEFAULT_GROUP_X, height - DEFAULT_GROUP_HEIGHT, width, DEFAULT_GROUP_HEIGHT);
	
	defaultGroup->end();
	this->end();
	
	this->OK_button = NULL;
	this->CANCEL_button = NULL;
}

// default constructor--just the title is needed; default values used for everything else
Fl_Dialog::Fl_Dialog(const char* title) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT + DEFAULT_GROUP_HEIGHT, title) {
	
	doConstructor(DEFAULT_WIDTH, DEFAULT_HEIGHT + DEFAULT_GROUP_HEIGHT);
	finishConstructor();
}

// constructor with title and flags
Fl_Dialog::Fl_Dialog(const char* title, unsigned long flags) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT + DEFAULT_GROUP_HEIGHT, title) {
	
	doConstructor(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	setUpButtons(flags);
	
	finishConstructor();
}

// constructor with title, dimensions, and flags
Fl_Dialog::Fl_Dialog(const char* title, int width, int height, unsigned long flags) :
	Fl_Window(width, height + DEFAULT_GROUP_HEIGHT, title) {
	
	doConstructor(width, height + DEFAULT_GROUP_HEIGHT);
	
	setUpButtons(flags);
	
	finishConstructor();
}

// called at the end of each constructor, this mearly sets the window as resizable and highlights the box around the defaultGroup
void Fl_Dialog::finishConstructor(void) {
	this->resizable(this);
	defaultGroup->box(FL_UP_BOX);
}

// takes a bitfield of flags for common button configurations (i.e. OK, Cancel, etc) and builds and places their widgets
void Fl_Dialog::setUpButtons(unsigned long flags) {
	
	if(flags) {
		int width = defaultGroup->w();
		int height = defaultGroup->h();
		int x = defaultGroup->x();
		int y = defaultGroup->y();
		
		// both OK and CANCEL buttons
		if((flags & Fl_OK) && (flags & Fl_CANCEL)) {
			
			int OK_X = x + (width >> 2) - (OK_DEFAULT_WIDTH >> 1);
			int OK_Y = y + (height >> 1) - (OK_DEFAULT_HEIGHT >> 1);
			
			int CANCEL_X = x + 3 * (width >> 2) - (CANCEL_DEFAULT_WIDTH >> 1);
			int CANCEL_Y = y + (height >> 1) - (CANCEL_DEFAULT_HEIGHT >> 1);
			
			OK_button = new Fl_Button(OK_X, OK_Y, OK_DEFAULT_WIDTH, OK_DEFAULT_HEIGHT, "OK");
			CANCEL_button = new Fl_Button(CANCEL_X, CANCEL_Y, CANCEL_DEFAULT_WIDTH, CANCEL_DEFAULT_HEIGHT, "Cancel");
			
			OK_button->type(FL_NORMAL_BUTTON);
			CANCEL_button->type(FL_NORMAL_BUTTON);
			
			defaultGroup->add(OK_button);
			defaultGroup->add(CANCEL_button);
		}
		
		// only an OK button
		else if((flags & Fl_OK) && !(flags & Fl_CANCEL)) {
			
			int OK_X = x + (width >> 1) - (OK_DEFAULT_WIDTH >> 1);
			int OK_Y = y + (height >> 1) - (OK_DEFAULT_HEIGHT >> 1);
			
			OK_button = new Fl_Button(OK_X, OK_Y, OK_DEFAULT_WIDTH, OK_DEFAULT_HEIGHT, "OK");
			OK_button->type(FL_NORMAL_BUTTON);
			
			defaultGroup->add(OK_button);	
		}
		
		// only a CANCEL button
		else if(!(flags & Fl_OK) && (flags & Fl_CANCEL)) {
			
			int CANCEL_X = x + (width >> 1) - (CANCEL_DEFAULT_WIDTH >> 1);
			int CANCEL_Y = y + (height >> 1) - (CANCEL_DEFAULT_HEIGHT >> 1);
			
			CANCEL_button = new Fl_Button(CANCEL_X, CANCEL_Y, CANCEL_DEFAULT_WIDTH, CANCEL_DEFAULT_HEIGHT, "Cancel");
			CANCEL_button->type(FL_NORMAL_BUTTON);
			
			defaultGroup->add(CANCEL_button);	
		}
		
	}
}

// destructor (does nothing)
Fl_Dialog::~Fl_Dialog() {
	
}

// assign the OK button a user-defined click event handler (if it exists).
// Returns true if the handler is assigned; false if the OK button is uninitialized
bool Fl_Dialog::setOKEventHandler(void (*handler)(Fl_Widget* w, void* data), void* data) {
	
	if(this->OK_button == NULL)
		return false;
		
	this->OK_button->callback(handler, data);
	this->OK_button->when(FL_WHEN_RELEASE);
	
	return true;
}

// assign the CANCEL button a user-defined click event handler (if it exists).
// Returns true if the handler is assigned; false if the CANCEL button is uninitialized
bool Fl_Dialog::setCancelEventHandler(void (*handler)(Fl_Widget* w, void* data), void* data) {
	
	if(this->CANCEL_button == NULL)
		return false;
		
	this->CANCEL_button->callback(handler, data);
	this->CANCEL_button->when(FL_WHEN_RELEASE);
	
	return true;	
}
