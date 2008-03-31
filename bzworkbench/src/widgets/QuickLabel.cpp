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

#include "widgets/QuickLabel.h"
#include "defines.h"

// initializes the text
QuickLabel::QuickLabel(const char* labelText, int x, int y) : 
	Fl_Box(FL_UP_BOX, x - 3, y, 0, DEFAULT_TEXTSIZE, labelText) {
	
	align(FL_ALIGN_RIGHT);
	labelsize(DEFAULT_TEXTSIZE);
}

// does nothing
QuickLabel::~QuickLabel() { 
	
}
