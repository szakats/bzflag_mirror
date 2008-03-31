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

#ifndef FL_IMAGEBUTTON_H_
#define FL_IMAGEBUTTON_H_

#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_PNG_Image.H>

/**
 * Subclass of Fl_Button that will load a PNG image on top of the button as a label
 */

class Fl_ImageButton : public Fl_Button {

public:
	
	/** Identical constructor to Fl_Button's, but in this case instead of a text label,
	  * the image at that location will be loaded instead.  If the image isn't found,
	  * then a text label with that name will appear.
	  */
	Fl_ImageButton( int x, int y, int width, int height, const char* imageFile );
	
	// destructor
	virtual ~Fl_ImageButton() {}

};

#endif /*FL_IMAGEBUTTON_H_*/
