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
