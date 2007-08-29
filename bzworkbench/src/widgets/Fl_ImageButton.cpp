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

#include "widgets/Fl_ImageButton.h"

// constructor
Fl_ImageButton::Fl_ImageButton( int x, int y, int width, int height, const char* imageFile ) :
	Fl_Button( x, y, width, height ) {
		
	Fl_PNG_Image* tempImage = new Fl_PNG_Image( imageFile );
	if( image == NULL ) {
		copy_label( imageFile );	
	}
	else {
		image( tempImage );
	}
}
