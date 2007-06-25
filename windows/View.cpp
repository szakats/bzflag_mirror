#include "../include/windows/View.h"

// view constructor
View::View(Model* m, int x, int y, int w, int h, const char *label) :
	RenderWindow(x,y,w,h) {
	
	// initialize the camera
	// this->camera = new OSGCamera( this->getCamera() );
	
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
   
   // load the cow model
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("share/cow.osg");
	
	// make a transformer
	osg::ref_ptr<osg::PositionAttitudeTransform> cowTransform = new osg::PositionAttitudeTransform();
	
	// add the cow model to the transformer
	cowTransform->addChild( loadedModel.get() );
	
	// move the cow up a bit
	cowTransform->setPosition( osg::Vec3( 0, 0, 3 ) );
	
	// add the cow model as a leaf to root
	this->root->addChild( cowTransform.get() );
	
   // add the root node to the scene
   this->setSceneData( this->root.get() );
	
}

// helper method: initialize the ground
void View::initGround( float size ) {
	// initialize the ground geode
   this->ground = new osg::Geode();
   
   // make a geometry
   osg::Geometry* groundGeometry = new osg::Geometry();
   
   // make a ground mesh (just a plane the size of the world)
   // ground points
   osg::Vec3Array* groundPoints = new osg::Vec3Array();
   groundPoints->push_back( osg::Vec3( -size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, size, 0 ) );
   groundPoints->push_back( osg::Vec3( -size, size, 0 ) );
   groundGeometry->setVertexArray( groundPoints );
   
   // ground indexes
   osg::DrawElementsUInt* groundIndexes = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
   groundIndexes->push_back( 0 );
   groundIndexes->push_back( 3 );
   groundIndexes->push_back( 2 );
   groundIndexes->push_back( 1 );
   groundGeometry->addPrimitiveSet( groundIndexes );
   
   // texture coordinates
   osg::Vec2Array* groundTexCoords = new osg::Vec2Array();
   groundTexCoords->push_back( osg::Vec2(0.0, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, size) );
   groundTexCoords->push_back( osg::Vec2(0.0, size) );
   groundGeometry->setTexCoordArray(0, groundTexCoords );
   
   // the texture itself
   osg::Texture2D* groundTexture = new osg::Texture2D();
   // keep the texture in VRAM
   groundTexture->setDataVariance( osg::Object::DYNAMIC );
   // load the image itself
   osg::Image* groundImage = osgDB::readImageFile( "share/world/std_ground.png" );
   // set the texture's image
   groundTexture->setImage( groundImage );
   // make a new state set for the texture (so we can manipulate the texture attributes)
   osg::StateSet* groundStateSet = new osg::StateSet();
   // assign the texture to the state set and activate it
   groundStateSet->setTextureAttributeAndModes( 0, groundTexture, osg::StateAttribute::ON );
   groundTexture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
   groundTexture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
   groundTexture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT ); 
   
   // make the member ground geode
   this->ground = new osg::Geode();
   // add the geometry data to the geode
   this->ground->addDrawable( groundGeometry );
   
   // assign the texture to the geode
   this->ground->setStateSet( groundStateSet );
   
}

// destructor (just free the camera)
View::~View() {
	/*
	if( this->camera )
		delete this->camera;	
	*/
}

float cnt = 0;

// draw method (really simple)
void View::draw(void) {
		
	this->frame();
}

// handle events
int View::handle(int event) {
	/*
	// forward FLTK events to OSG
	switch(event){
        
        case FL_KEYDOWN: {
            
            int key = Fl::event_key();
                
            switch (key) {
            	
            	case FL_Up: {
	            	
	            	this->camera->translate( 0.0, 1.0, 0.0 );
	           		this->redraw();
	           		return 1;
            	}
            	
            	case FL_Down: {
            		
	            	this->camera->translate( 0.0, -1.0, 0.0 );
	           		this->redraw();
	           		return 1;
            	}
            	
            	case FL_Right: {
            		this->camera->rotate_z( 1 );
            		this->redraw();
            		return 1;
            	}
            	
            	case FL_Left: {
            		this->camera->rotate_z( -1 );
            		this->redraw();
            		return 1;	
            	}
            	
            	default:
            		break;
            }
        }
        case FL_KEYUP:
            
            return 1;
        default:*/
            // pass other events to the base class
            return RenderWindow::handle(event);
    // }
}
