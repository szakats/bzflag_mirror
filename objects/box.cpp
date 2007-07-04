#include "../include/objects/box.h"

// constructors
box::box() : bz2object("box", "<position><rotation><size>") {
	this->position = Point3D( 0.0, 0.0, 0.0 );
	this->size = Point3D( 10.0, 10.0, 10.0 );
}

box::box(string& data) : bz2object("box", "<position><rotation><size>", data.c_str()) {
	this->update(data);	
}

// nothing to destroy...
box::~box() { }

// getter
string box::get(void) {
	return this->toString();
}

// setter
int box::update(string& data) {
	return bz2object::update(data);		
}

// toString
string box::toString(void) {
	return string("box\n") +
				  this->BZWLines() +
				  "end\n";
}

// get renderable instance (will be invalid upon changing anything)
// the transform node stores the output of this->toString() as its name
osg::ref_ptr< Renderable > box::makeRenderable(void) {
	osg::ref_ptr< osg::Node > boxNode;
	
	if(!this->isSelected()) {
		boxNode = SceneBuilder::buildNode( "share/box/box.obj", true );
	}
	else {
		boxNode = SceneBuilder::buildSelectedNode( "share/box/box.obj" );
	}
	
	osg::ref_ptr< Renderable > boxTransform = SceneBuilder::renderable( boxNode.get() );
	
	boxTransform->setPosition( osg::Vec3( this->getPosition()->x(), this->getPosition()->y(), this->getPosition()->z() + this->getSize()->z() / 2.0 ) );
	boxTransform->setScale( osg::Vec3( this->getSize()->x(), this->getSize()->y(), this->getSize()->z() / 2.0 ) );
	boxTransform->setRotationZ( this->rotation );
	boxTransform->setBZWObject( this );
	
	return boxTransform;
}

// update the renderable
bool box::updateRenderable( Renderable* boxTransform ) {
	
	osg::ref_ptr< osg::Node > boxNode;
	
	// get the node based on selection
	// SceneBuilder flyweights all loaded nodes; 99% of the time all we're doing here
	// is just acquiring a reference to an already-loaded instance.
	if(!this->isSelected()) {
		boxNode = SceneBuilder::buildNode( "share/box/box.obj", true );
	}
	else {
		boxNode = SceneBuilder::buildSelectedNode( "share/box/box.obj" );
	}
	
	boxTransform->setNode( boxNode.get() );
	boxTransform->setPosition( osg::Vec3( this->getPosition()->x(), this->getPosition()->y(), this->getPosition()->z() + this->getSize()->z() / 2.0 ) );
	boxTransform->setScale( osg::Vec3( this->getSize()->x(), this->getSize()->y(), this->getSize()->z() / 2.0 ) );
	boxTransform->setRotationZ( this->rotation );
	boxTransform->setBZWObject( this );
	
	return true;
}
