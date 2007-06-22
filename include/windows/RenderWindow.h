#ifndef RENDERWINDOW_H_
#define RENDERWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

// OpenSceneGraph stuff
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include "../model/Model.h"

class RenderWindow : public Fl_Gl_Window {
	
public:
	
	static const int DEFAULT_WIDTH = 980;
	static const int DEFAULT_HEIGHT = 680;
	
	// constructors
	// (needs a reference to the Model so it can render stuff)
	RenderWindow();
	RenderWindow(int x, int y, int width, int height);
	
	// OSG-specific methods
	osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
    const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }
	
	// FLTK-specific methods
	int handle(int);
	void resize(int x, int y, int w, int h);
	
private:

	// reference to an embedded OSG render window
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};

#endif /*RENDERWINDOW_H_*/
