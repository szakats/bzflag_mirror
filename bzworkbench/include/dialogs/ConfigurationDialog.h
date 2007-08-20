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

#ifndef CONFIGURATIONDIALOG_H_
#define CONFIGURATIONDIALOG_H_

#include "Fl_Dialog.h"
#include "../DataEntry.h"

/**
 * This is the base class of all object configuration dialogs.
 */
 
class ConfigurationDialog : public Fl_Dialog {

public:

	// constructor
	ConfigurationDialog( DataEntry* data, const char* title, int width, int height ) :
		Fl_Dialog( title, width, height, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL )
		{ this->data = data; }
	
	// static initializer
	static ConfigurationDialog* init( DataEntry* data, const char* title, int width, int height ) { return new ConfigurationDialog( data, title, width, height ); }
	
	// destructor
	virtual ~ConfigurationDialog() { }
	
protected:

	// the data itself
	DataEntry* data;
};

#endif /*CONFIGURATIONDIALOG_H_*/
