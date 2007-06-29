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

#include "../objects/box.h"

#include "../Observer.h"

// forward declare the Picker class
class Picker;

// an extension of RenderWindow and osgViewer (this will be added to the main window), and Observer
class View : public osgViewer::Viewer, public RenderWindow, public Observer
{
    public:
    	// constructor
        View(Model* m, int x, int y, int w, int h, const char *label=0);
        
        // get the root node
        osg::Group* getRootNode() { return root.get(); }
        
        // FLTK event handler
        virtual int handle(int);
        
        // Observer update() method
        void update( Observable* obs, void* data );
        
        // OSG Picker event handler
        void handlePicker( Picker* picker, void* data );
        
        // set an object as selected
        void setSelected( bz2object& object );
        void setSelected( osg::PositionAttitudeTransform* node );
        
        // set an object as unselected
        void setUnselected( bz2object& object );
        void setUnselected( osg::PositionAttitudeTransform* node );
        
        // mark a node as selected
        static void markSelected( osg::PositionAttitudeTransform* node );
        
        // unselect the selected node(s)
        static void markUnselected( osg::PositionAttitudeTransform* node );
        
        // destructor
        virtual ~View();
        
        // is an object selected?
        bool isSelected( osg::PositionAttitudeTransform* node );
        
    protected:
    
    	// draw method
        virtual void draw();
	
		// model reference
		Model* model;
		
		// root node
		osg::ref_ptr< osg::Group > root;
		
		// ground (always present)
		osg::ref_ptr< osg::PositionAttitudeTransform > ground;
		
		// map of selected objects
		map< string, osg::ref_ptr< osg::PositionAttitudeTransform > > selectedObjects;
	
	private:
	
		// helper method: initialize the ground method from a given radius
		void initGround( float size );
};

#endif /*VIEW_H_*/
