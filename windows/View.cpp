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
   this->initGround( 100.0f );
   
   // add the ground to the root node
   this->root->addChild( this->ground.get() );

    osg::ref_ptr<osg::PositionAttitudeTransform> cow1 = SceneBuilder::transformable( SceneBuilder::buildNode( "share/cow.osg" ).get() );
	osg::ref_ptr<osg::PositionAttitudeTransform> cow2 = SceneBuilder::transformable( SceneBuilder::buildNode( "share/cow.osg" ).get() );
	osg::ref_ptr<osg::PositionAttitudeTransform> cow3 = SceneBuilder::transformable( SceneBuilder::buildNode( "share/cow.osg" ).get() );
	
	// move the cow up a bit
	cow1->setPosition( osg::Vec3(0, 0, 3) );
	cow2->setPosition( osg::Vec3(10, 10, 3) );
	cow3->setPosition( osg::Vec3(-10, 10, 3) );
	
	// add the cow model as a leaf to root
	this->root->addChild( cow1.get() );
	this->root->addChild( cow2.get() );
	this->root->addChild( cow3.get() );
	
   // add the root node to the scene
   this->setSceneData( this->root.get() );
   
   // make this a trackball manipulator
   this->setCameraManipulator(new osgGA::TrackballManipulator());
   
   // add the stats event handler
   this->addEventHandler(new osgViewer::StatsHandler);
	
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
   
   osg::ref_ptr<osg::PositionAttitudeTransform> pat = SceneBuilder::transformable( SceneBuilder::buildGeode( "ground", groundPoints, groundIndexes, groundTexCoords, "share/world/std_ground.png" ).get() );
   
	pat->setPosition( osg::Vec3( 0, 0, -30 ) );
	
	this->root->addChild(pat.get());
}

// destructor
View::~View() { }

float cnt = 0;

// draw method (really simple)
void View::draw(void) {
	this->frame();
}

// handle events
int View::handle(int event) {
    // pass other events to the base class
	return RenderWindow::handle(event);
}
