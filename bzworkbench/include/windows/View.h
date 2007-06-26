#ifndef VIEW_H_
#define VIEW_H_

#include "RenderWindow.h"
#include "../model/Model.h"
#include "../model/SceneBuilder.h"

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>

// an extension of RenderWindow and osgViewer (this will be added to the main window)
class View : public osgViewer::Viewer, public RenderWindow
{
    public:
    	// constructor
        View(Model* m, int x, int y, int w, int h, const char *label=0);
        
        // FLTK event handler
        virtual int handle(int);
        
        // destructor
        virtual ~View();
        
    protected:
    
    	// draw method
        virtual void draw();
	
		// model reference
		Model* model;
		
		// root node
		osg::ref_ptr< osg::Group > root;
		
		// ground (always present)
		osg::ref_ptr< osg::Geode > ground;
	
	private:
	
		// helper method: initialize the ground method from a given radius
		void initGround( float size );
};

#endif /*VIEW_H_*/
