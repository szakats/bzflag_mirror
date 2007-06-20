#include "../include/windows/RenderWindow.h"

// constructor with model
RenderWindow::RenderWindow(Model* m) :
	Fl_Gl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT) {
	
	this->model = m;
}

RenderWindow::RenderWindow(int x, int y, int width, int height, Model* m) :
	Fl_Gl_Window(x, y, width, height) {
	
	this->model = m;
}

// draw the scene
void RenderWindow::draw(void) {
	
	if(!valid()) {
		//... set up projection, viewport, etc ...
    	//  ... window size is in w() and h().
   	    //  ... valid() is turned on by FLTK after draw() returns	
	}	
	// ... draw ...
}

// event handler
int RenderWindow::handle(int event) {
	
	switch(event) {
	  case FL_PUSH:
	  
	  //  ... mouse down event ...
	  //  ... position in Fl::event_x() and Fl::event_y()
	    return 1;
	  case FL_DRAG:
	  
	  //  ... mouse moved while down event ...
	    return 1;
	  case FL_RELEASE:   
	  		
	    return 1;
	  case FL_FOCUS :
	  case FL_UNFOCUS :
	  //  ... Return 1 if you want keyboard events, 0 otherwise
	    return 1;
	  case FL_KEYBOARD:
	   
	//    ... keypress, key is in Fl::event_key(), ascii in Fl::event_text()
	//    ... Return 1 if you understand/use the keyboard event, 0 otherwise..
	    return 1;
	  case FL_SHORTCUT:
	 	
	 //   ... shortcut, key is in Fl::event_key(), ascii in Fl::event_text()
	 //   ... Return 1 if you understand/use the shortcut event, 0 otherwise...
	    return 1;
	  default:
	    // pass other events to the base class...
	    return Fl_Gl_Window::handle(event);
	  }
}
