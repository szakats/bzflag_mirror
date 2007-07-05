#ifndef BZEVENTHANDLER_H_
#define BZEVENTHANDLER_H_

#include "../View.h"
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/Viewer>
#include <string>

#include <FL/Fl.H>

using namespace std;

// This is the interface all event handlers that can be incorporated EventHandlerCollection
// must conform to

class BZEventHandler : public osgGA::GUIEventHandler {
	
public:

	BZEventHandler() {}
	~BZEventHandler() {}
	
	// the handler
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) = 0;
	
};

#endif /*BZEVENTHANDLER_H_*/
