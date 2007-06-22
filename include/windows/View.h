#ifndef VIEW_H_
#define VIEW_H_

// OSG view as an extension of RenderWindow
#include "RenderWindow.h"
#include "../model/Model.h"

// an extension of RenderWindow and osgViewer (this will be added to the main window)
class View : public osgViewer::Viewer, public RenderWindow
{
    public:
    	// constructor
        View(Model* m, int x, int y, int w, int h, const char *label=0);
        
    protected:
    
    	// draw method
        virtual void draw();
	
		// model reference
		Model* model;
};

#endif /*VIEW_H_*/
