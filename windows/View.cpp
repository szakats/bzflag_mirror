#include "../include/windows/View.h"

// view constructor
View::View(Model* m, int x, int y, int w, int h, const char *label) :
	RenderWindow(x,y,w,h) {
	
	// set OSG viewport
    this->getCamera()->setViewport(new osg::Viewport(0,0,w,h));
    
    // get the graphics context
    this->getCamera()->setGraphicsContext(this->getGraphicsWindow());
    
    // do single-threaded view
   this->setThreadingModel(osgViewer::Viewer::SingleThreaded);
   
   // set the model
   this->model = m;
   
   // initialize the root node
   this->root = new osg::Group();
   
   // initialize the ground
   this->initGround( 400.0f );
   
   // add the ground to the root node
   this->root->addChild( this->ground.get() );
	
	
   // add the root node to the scene
   this->setSceneData( this->root.get() );
   
   // make this a trackball manipulator
   this->setCameraManipulator(new osgGA::TrackballManipulator());
   
   // add the stats event handler
   this->addEventHandler(new osgViewer::StatsHandler());
   
   // add our scene picker event handler
   this->addEventHandler(new Picker());
	
}

// helper method: initialize the ground
void View::initGround( float size ) {
	
   // make a ground mesh (just a plane the size of the world)
   // ground points
   osg::Vec3Array* groundPoints = new osg::Vec3Array();
   groundPoints->push_back( osg::Vec3( -size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, size, 0 ) );
   groundPoints->push_back( osg::Vec3( -size, size, 0 ) );
   
   // ground indexes
   osg::DrawElementsUInt* groundIndexes = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
   groundIndexes->push_back( 0 );
   groundIndexes->push_back( 3 );
   groundIndexes->push_back( 2 );
   groundIndexes->push_back( 1 );
   
   // texture coordinates
   osg::Vec2Array* groundTexCoords = new osg::Vec2Array();
   groundTexCoords->push_back( osg::Vec2(0.0, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, size) );
   groundTexCoords->push_back( osg::Vec2(0.0, size) );
   
   // make the member ground geode
   this->ground = SceneBuilder::buildGeode( "ground", groundPoints, groundIndexes, groundTexCoords, "share/world/std_ground.png" );
   
}

// destructor
View::~View() { }


// draw method (really simple)
void View::draw(void) {
	this->frame();
}

// handle events
int View::handle(int event) {
    // pass other events to the base class
	return RenderWindow::handle(event);
}

// update method (inherited from Observer)
void View::update( Observable* obs, void* data ) {
	
}

/**
 * Set an object as selected (i.e. tints it green)
 * Assume it's passed the output of bz2object.getRenderable(), meaning
 * it gets a PositionAttitudeTransform node containing a Group node containing
 * the node itself or other group nodes.
 */
void View::setSelected( osg::PositionAttitudeTransform* transformedNode ) {
	
	// get the child node (there should only be one)
	osg::Node* theNode = transformedNode->getChild( 0 );
	
	// try dynamic_cast-ing the node to a group
    osg::Group* theGroup = dynamic_cast< osg::Group* > (theNode);
    
    // the Geodes
    vector<osg::Geode*> theGeodes = vector<osg::Geode*>();
    
    // if the group dynamic_cast succeeded, then try dynamic_cast-ing its children into the geode array
    if(theGroup != NULL) {
    	// get the children of the group
    	vector< osg::ref_ptr< osg::Node > >* children = SceneBuilder::extractChildren( theGroup );
    	// see if any are geodes (if so, then add them to theGeodes)
    	if( children->size() > 0 ) {
    		for(vector< osg::ref_ptr< osg::Node > >::iterator i = children->begin(); i != children->end(); i++) {
    			// try to dynamic_cast the node to a geode
    			osg::Geode* geode = dynamic_cast< osg::Geode* > (i->get());
    			if(geode)
    				theGeodes.push_back( geode );	
    		}
    	}
    	// free the children memory
    	delete children;
    }
    else {
    	// if that didn't work, then try making the node a geode
    	osg::Geode* geode = dynamic_cast< osg::Geode* > (theNode);
    	if(geode)
    		theGeodes.push_back( geode );
    }
    
    // break if there are no geodes to select
    if( theGeodes.size() == 0) {
    	return;
    }
    
	// assign all geodes a different color
	for(vector< osg::Geode* >::iterator i = theGeodes.begin(); i != theGeodes.end(); i++) {
		osg::Vec4 color(0.0, 1.0, 0.0, 1.0);
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		
		colors->push_back( color );
		
		vector< osg::ref_ptr< osg::Drawable > > drawables = (*i)->getDrawableList();
		if(drawables.size() > 0) {
			osg::Geometry* geo;
			for( vector< osg::ref_ptr< osg::Drawable > >::iterator i = drawables.begin(); i != drawables.end(); i++ ) {
				geo = (*i)->asGeometry();
				if( geo ) {
					geo->setColorArray( colors.get() );
					geo->setColorBinding( osg::Geometry::BIND_OVERALL );
				}
			}	
		}
		
		osg::StateSet* states = (*i)->getOrCreateStateSet();
		
		osg::TexEnv* tec = new osg::TexEnv();
		tec->setMode( osg::TexEnv::BLEND );
		states->setTextureAttribute(0, tec, osg::StateAttribute::ON );
		
		(*i)->setStateSet( states );
	}
	
	// finally, push the selection to the selection array
	this->selectedNodes.clear();
	for(vector< osg::Geode* >::iterator i = theGeodes.begin(); i != theGeodes.end(); i++) {
		selectedNodes.push_back( osg::ref_ptr<osg::Node>( *i ) );	
	}
	
}

/**
 * This method does the same as setSelected, but instead of doing the selection, it undoes it.
 */
void View::setUnselected( osg::PositionAttitudeTransform* node ) {
	
}
