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

#ifndef RENDERWINDOW_H_
#define RENDERWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

// OpenSceneGraph stuff
#include <osgViewer/Viewer>
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
	virtual int handle(int);
	void resize(int x, int y, int w, int h);
	
	// get the embedded view
	osgViewer::GraphicsWindowEmbedded* getOSGGraphicsWindow() { return _gw.get(); }
	
	// set the embedded view
	void setOSGGraphicsWindow( osgViewer::GraphicsWindowEmbedded* gw ) { _gw = gw; }
	
protected:

	// reference to an embedded OSG render window
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};

#endif /*RENDERWINDOW_H_*/
