#ifndef VIEW_H_
#define VIEW_H_

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgGA/EventQueue>

#include "RenderWindow.h"
#include "../model/Model.h"
#include "../model/SceneBuilder.h"

#include "../objects/bz2object.h"
#include "../objects/box.h"

#include "../Observer.h"
#include "../render/Renderable.h"

#include "EventHandlerCollection.h"
#include "eventHandlers/selectHandler.h"

#include "../render/Ground.h"

#include "../render/Selection.h"

// forward declare the Picker class
class BZEventHandler;

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
        
        // set an object as selected
        void setSelected( bz2object* object );
        void setSelected( Renderable* node );
        
        // set an object as unselected
        void setUnselected( bz2object* object );
        void setUnselected( Renderable* node );
        
        // tell the model an object changed
        void refresh( Renderable* node );
        
        // select all objects
        // void selectAll();
        
        // unselect all objects
        void unselectAll();
        
        // destructor
        virtual ~View();
        
        // is an object selected?
        bool isSelected( Renderable* node );
        
        // is a button down?
        bool isPressed( int value );
        
    protected:
    
    	// draw method
        virtual void draw();
	
		// model reference
		Model* model;
		
		// root node
		osg::ref_ptr< osg::Group > root;
		
		// ground node
		osg::ref_ptr< Renderable > ground;
		
		// modifier key map.
		// maps FLTK key values to bools
		map< int, bool > modifiers;
		
		// map bz2object pointers to renderables, so we can know
		// which objects are being rendered
		map< bz2object* , osg::ref_ptr< Renderable > > objectMap;
	
	private:
		
		// the collection of evnet handlers
		osg::ref_ptr< EventHandlerCollection > eventHandlers;
		
		// the current selection
		osg::ref_ptr< Selection > selection;
};

#endif /*VIEW_H_*/
