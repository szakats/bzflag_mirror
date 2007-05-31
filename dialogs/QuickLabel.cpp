#include "QuickLabel.h"

// initializes the text
QuickLabel::QuickLabel(const char* labelText, int x, int y) : 
	Fl_Box(FL_UP_BOX, x - 3, y, 0, DEFAULT_TEXTSIZE, labelText) {
	
	this->align(FL_ALIGN_RIGHT);
	this->labelsize(DEFAULT_TEXTSIZE);
}

// does nothing
QuickLabel::~QuickLabel() { 
	
}
