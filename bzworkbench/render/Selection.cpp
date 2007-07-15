#include "../include/render/Selection.h"


const float Selection::CENTER_RADIUS = 2.0f;
const float Selection::SHAFT_LENGTH = 10.0f;
const float Selection::SHAFT_RADIUS = 0.5f;
const float Selection::TIP_LENGTH = 3.0f;
const float Selection::TIP_RADIUS = 1.0f;


Selection::Selection() {
	// build the map geode and add it
	this->axes = this->buildAxes( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	this->setName( Selection_NODE_NAME );
}

// build the axes geode
osg::ref_ptr< Renderable > Selection::buildAxes( osg::Vec3 localOrigin ) {
	// cone endings of the axes
	osg::ref_ptr< osg::Cone > x_tip, y_tip, z_tip;
	
	// the cylinders connecting the tips
	osg::ref_ptr< osg::Cylinder > x_shaft, y_shaft, z_shaft;
	
	// the center of the axes
	osg::ref_ptr< osg::Sphere > center = new osg::Sphere( localOrigin, Selection::CENTER_RADIUS );
	
	// create the shafts and translate them to their appropriate positions
	x_shaft = new osg::Cylinder( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH / 2.0, 0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	x_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH / 2.0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	y_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH / 2.0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	
	// create the tips
	x_tip = new osg::Cone( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH, 0, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	x_tip->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_tip = new osg::Cone( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	y_tip->setRotation( osg::Quat( osg::DegreesToRadians( -90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_tip = new osg::Cone( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	// z_tip->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	// make drawables
	osg::ShapeDrawable  *centerD	= new osg::ShapeDrawable( center.get() ),
						*x_shaftD	= new osg::ShapeDrawable( x_shaft.get() ),
						*y_shaftD	= new osg::ShapeDrawable( y_shaft.get() ),
						*z_shaftD	= new osg::ShapeDrawable( z_shaft.get() ),
						*x_tipD		= new osg::ShapeDrawable( x_tip.get() ),
						*y_tipD		= new osg::ShapeDrawable( y_tip.get() ),
						*z_tipD		= new osg::ShapeDrawable( z_tip.get() );
	
	// assign colors
	centerD->setColor( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	
	// x shaft is green
	x_shaftD->setColor( osg::Vec4( 0.0, 1.0, 0.0, 1.0 ) );
	x_tipD->setColor( osg::Vec4( 0.0, 1.0, 0.0, 1.0 ) );
	
	// y shaft is red
	y_shaftD->setColor( osg::Vec4( 1.0, 0.0, 0.0, 1.0 ) );
	y_tipD->setColor( osg::Vec4( 1.0, 0.0, 0.0, 1.0 ) );
	
	// z shaft is blue
	z_shaftD->setColor( osg::Vec4( 0.0, 0.0, 1.0, 1.0 ) );
	z_tipD->setColor( osg::Vec4( 0.0, 0.0, 1.0, 1.0 ) );
	
	// make geodes to contain the objects
	// We do this so we can tell in the selection handler which part of the object was selected
	// (i.e. which axis)
	osg::Geode *centerGeode = new osg::Geode(),
			   *xGeode		= new osg::Geode(),
			   *yGeode		= new osg::Geode(),
			   *zGeode		= new osg::Geode();
	
	// add the drawables
	centerGeode->addDrawable( centerD );
	xGeode->addDrawable( x_shaftD );
	xGeode->addDrawable( x_tipD );
	yGeode->addDrawable( y_shaftD );
	yGeode->addDrawable( y_tipD );
	zGeode->addDrawable( z_shaftD );
	zGeode->addDrawable( z_tipD );
	
	// give the geodes names
	centerGeode->setName( Selection_CENTER_NODE_NAME );
	xGeode->setName( Selection_X_AXIS_NODE_NAME );
	yGeode->setName( Selection_Y_AXIS_NODE_NAME );
	zGeode->setName( Selection_Z_AXIS_NODE_NAME );
	
	// add the geodes to a group
	osg::Group* axesGroup = new osg::Group();
	axesGroup->addChild( xGeode );
	axesGroup->addChild( yGeode );
	axesGroup->addChild( zGeode );
	axesGroup->addChild( centerGeode );
	
	// encase the group into a renderable
	osg::ref_ptr< Renderable > r = new Renderable( axesGroup );
	osg::StateSet* states = axesGroup->getOrCreateStateSet();
	
	// disable depth testing
	states->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF );
	states->setMode(GL_CULL_FACE, osg::StateAttribute::ON );
	
	axesGroup->setStateSet( states );
	
	return r;
}

// get the local origin
osg::Vec3 Selection::computeLocalOrigin( vector< bz2object* >& objects ) {
	double x = 0.0, y = 0.0, z = 0.0;
	
	// compute the average x, y, and z values of all renderables
	if( objects.size() > 0 ) {
		for( vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			x += (*i)->getPosition().x();
			y += (*i)->getPosition().y();
			z += (*i)->getPosition().z();
		}
		
		int numElements = objects.size();
		x /= numElements;
		y /= numElements;
		z /= numElements;
	}
	
	return osg::Vec3( x, y, z );
}

// rebuild the axes
void Selection::rebuildAxes( vector< bz2object*>& objects ) {
	this->axes->setPosition( this->computeLocalOrigin( objects ) );
}

// determine which child of this node was picked
// (used by selectHandler)
osg::Node* Selection::getPickedNode( Renderable* r, const osg::NodePath& nodes, unsigned int startIndex ) {
	if(nodes.size() <= 0)
		return NULL;
		
	for(unsigned int i = startIndex; i < nodes.size(); i++) {
		if( nodes[i]->getName().length() > 0 )
			return nodes[i];
	}
	
	return NULL;
}

// inhereted Observer method: update().
// This method queries the Model (i.e. the passed observer) for the selected objects
// and recomputes the axis location
void Selection::update( Observable* observable, void* data ) {
	// see if this was the Model
	Model* model = dynamic_cast< Model* >( observable );
	if(!model)
		return;
	
	// update the axes
	vector< bz2object* > selectedObjects = model->_getSelection();
	
	// remove the axes if there are no objects
	if( selectedObjects.size() <= 0 && this->containsNode( axes.get() ))
		this->removeChild( axes.get() );
	
	// add the axes if there are some objects
	if( selectedObjects.size() > 0 && !this->containsNode( axes.get() ))
		this->addChild( axes.get() );
		
	// recompute the center
	this->rebuildAxes( selectedObjects );
	
}
