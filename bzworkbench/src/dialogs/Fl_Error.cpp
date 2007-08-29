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

#include "dialogs/Fl_Error.h"

Fl_Error::Fl_Error(const char* message) :
	Fl_Dialog("BZWorkbench Error", DEFAULT_WIDTH, 200, Fl_Dialog::Fl_OK) {
		
	errorImage = new Fl_XPM_Image("share/error.xpm");
	
	errorMessage = new Fl_Multiline_Output(60, 20, DEFAULT_WIDTH - 60 - 10, 200 - 30 - 10, "An error has occurred:");
	errorMessage->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
	errorMessage->value(message);
	
	errorLabel = new QuickLabel("", 5, 100 - 24);
	errorLabel->image(errorImage);
	
	add(errorLabel);
	add(errorMessage);
	
	setOKEventHandler(Fl_ErrorCallback, this);
	
	show();
}

Fl_Error::~Fl_Error() {
	if(errorImage)
		delete errorImage;
}

void Fl_Error::setText(const char* message) { errorMessage->value(message); }

// default callbacks
void Fl_Error::Fl_ErrorCallback_real(Fl_Widget* w) {
	Fl::delete_widget(this);
}
