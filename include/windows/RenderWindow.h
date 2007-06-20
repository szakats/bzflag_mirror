#ifndef RENDERWINDOW_H_
#define RENDERWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

#include "../model/Model.h"

class RenderWindow : public Fl_Gl_Window {
	
public:
	
	static const int DEFAULT_WIDTH = 980;
	static const int DEFAULT_HEIGHT = 680;
	
	// constructors
	// (needs a reference to the Model so it can render stuff)
	RenderWindow(Model* m);
	RenderWindow(int x, int y, int width, int height, Model* m);
	
	// FLTK-specific stuff
	int handle(int);
	void draw(void);
	
private:

	// model reference
	Model* model;
};

#endif /*RENDERWINDOW_H_*/
