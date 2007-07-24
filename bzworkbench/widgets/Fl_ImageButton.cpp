#include "../include/widgets/Fl_ImageButton.h"

// constructor
Fl_ImageButton::Fl_ImageButton( int x, int y, int width, int height, const char* imageFile ) :
	Fl_Button( x, y, width, height ) {
		
	Fl_PNG_Image* image = new Fl_PNG_Image( imageFile );
	if( image == NULL ) {
		this->copy_label( imageFile );	
	}
	else {
		this->image( image );
	}
}
