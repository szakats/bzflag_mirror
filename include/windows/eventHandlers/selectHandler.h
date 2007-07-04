#ifndef SELECTHANDLER_H_
#define SELECTHANDLER_H_

#include "BZEventHandler.h"
#include <osgViewer/Viewer>

#include <string>

using namespace std;

class selectHandler : public BZEventHandler {

public:
	
	// constructor
	selectHandler();
	~selectHandler() {}
	
	// get the name
	static string getName() { return string("selectHandler"); }
	
	// handle an event
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	
	// pick an object an event
    bool pick(View* viewer, const osgGA::GUIEventAdapter& ea);
    
    Renderable* lastSelected;
};

#endif /*SELECTHANDLER_H_*/
