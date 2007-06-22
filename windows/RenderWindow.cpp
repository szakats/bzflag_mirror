#include "../include/windows/RenderWindow.h"

// constructor with model
RenderWindow::RenderWindow() :
	Fl_Gl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT) {
	
	this->end();
	
	// initialize the OSG embedded graphics window
	this->_gw = new osgViewer::GraphicsWindowEmbedded(this->x(), this->y(), DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	// make this resizable
	this->resizable(this);
}

RenderWindow::RenderWindow(int x, int y, int width, int height) :
	Fl_Gl_Window(x, y, width, height) {
	
	this->end();
	
	// initialize the OSG embedded graphics window
	this->_gw = new osgViewer::GraphicsWindowEmbedded(x,y,width,height);
	
	// make this resizable
	this->resizable(this);
	
}

// resize method
void RenderWindow::resize(int x, int y, int w, int h) {
	// resize the OSG render window
	_gw->getEventQueue()->windowResize(x, y, w, h );
    _gw->resized(x,y,w,h);
	
	// resize the FLTK window
    Fl_Gl_Window::resize(x,y,w,h);	
}

// event handler
int RenderWindow::handle(int event) {
	
	// forward FLTK events to OSG
	switch(event){
        case FL_PUSH:
            _gw->getEventQueue()->mouseButtonPress(Fl::event_x(), Fl::event_y(), Fl::event_button());
			this->redraw();
            return 1;
          
        case FL_DRAG:
            _gw->getEventQueue()->mouseMotion(Fl::event_x(), Fl::event_y());
        	this->redraw();
			return 1;
        case FL_RELEASE:
            _gw->getEventQueue()->mouseButtonRelease(Fl::event_x(), Fl::event_y(), Fl::event_button());
        	this->redraw();    
			return 1;
        case FL_KEYDOWN:
            _gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)Fl::event_key());
            this->redraw();
            return 1;
        case FL_KEYUP:
            _gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)Fl::event_key());
            this->redraw();
            return 1;
        default:
            // pass other events to the base class
            return Fl_Gl_Window::handle(event);
    }
}
