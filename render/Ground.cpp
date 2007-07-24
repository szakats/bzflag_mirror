#include "../include/render/Ground.h"


const float Ground::DEFAULT_SIZE = 400.0f;

// make the ground geode
Ground::Ground( float size ) : Renderable("ground") {
	 
	this->size = size;
	
	// make the geode
	
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
   osg::Geode* groundGeode = SceneBuilder::buildGeode( "ground", groundPoints, groundIndexes, groundTexCoords, "share/world/std_ground.png" );
   
   // add it
   this->addChild( groundGeode );
   
   // make the grid
   this->grid = this->buildGrid( this->size, 10.0f );
   
   // translate the grid upward a bit
   this->grid->setPosition( osg::Vec3( 0, 0, 0.1 ) );
   
   SceneBuilder::assignMaterial( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
   								 osg::Vec4( 0.2, 0.2, 0.2, 0.2 ),
   								 osg::Vec4( 0.0, 0.0, 0.0, 0.0 ),
   								 osg::Vec4( 0.0, 0.0, 0.0, 0.0 ),
   								 0.0f,
   								 1.0f,
   								 this->grid.get() );
   
   // add the grid
   this->addChild( grid.get() );
}

osg::ref_ptr< Renderable > Ground::buildGrid( float size, float unit ) {
   
   // ground vertexes
   osg::Vec3Array* gridPoints = new osg::Vec3Array();
   int numPoints = 0;
   for( float i = -size; i < size; i += unit ) {
   		// vertical line
   		gridPoints->push_back( osg::Vec3( i, -size, 0 ) );
   		gridPoints->push_back( osg::Vec3( i, size, 0 ) );
   		
   		// horizontal line
   		gridPoints->push_back( osg::Vec3( -size, i, 0 ) );
   		gridPoints->push_back( osg::Vec3( size, i, 0 ) );
   		
   		numPoints += 4;
   }
   
   // ground indexes
   osg::DrawElementsUInt* gridIndexes = new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
   
   for(int i = 0; i < numPoints; i+=4) {
   		gridIndexes->push_back( i );
   		gridIndexes->push_back( i+1 );
   		
   		gridIndexes->push_back( i+2 );
   		gridIndexes->push_back( i+3 );
   }
   
   return new Renderable( SceneBuilder::buildGeode( "grid", gridPoints, gridIndexes, NULL, NULL) );
}

// nothing to do when freeing this object
Ground::~Ground() { };

// get the size
float Ground::getSize() { return this->size; }

// set the size and update the ground
void Ground::setSize( float newSize ) {
	if(newSize <= 0.0f)
		return;
		
	float scaleFactor = Ground::DEFAULT_SIZE / newSize;
	this->setScale( osg::Vec3d( scaleFactor, scaleFactor, scaleFactor ) );
	
	this->size = size;
}
