#include "../include/objects/pyramid.h"

pyramid::pyramid() : bz2object("pyramid", "<position><rotation><size>", SceneBuilder::buildNode("share/pyramid/pyramid.obj")) {
	
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	
	this->setPos( osg::Vec3(0.0, 0.0, 0.0) );
	this->setSize( osg::Vec3(10.0, 10.0, 10.0) );
	SceneBuilder::markUnselected( this );
}

// constructor with binary data
pyramid::pyramid( osg::Vec3 position, float rotation, osg::Vec3 scale ) : bz2object("pyramid", "<position><rotation><size>", SceneBuilder::buildNode("share/pyramid/pyramid.obj")) {
	
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	
	this->setPos( position );
	this->setRotationZ( rotation );
	this->setSize( scale );
	SceneBuilder::markUnselected( this );
}

// constructor with string
pyramid::pyramid(string& data) : bz2object("pyramid", "<position><rotation><size>", data.c_str()) {
	this->addChild( SceneBuilder::buildNode("share/pyramid/pyramid.obj") );
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	SceneBuilder::markUnselected( this );
	
	if( data.length() <= 1 ) {
		this->setPos( osg::Vec3(0.0, 0.0, 0.0) );
		this->setSize( osg::Vec3(10.0, 10.0, 10.0) );
	}
	else 
		this->update(data);	
}	

// getter
string pyramid::get() { return this->toString(); }

// setter
int pyramid::update(string& data) {
	return bz2object::update(data);	
}

// setter with messaging
int pyramid::update(UpdateMessage& message) {
	
	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			this->setPos( *(message.getAsPosition()) );
			break;
			
		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			this->setPos( this->getPos() + *(message.getAsPositionFactor()) );
			break;
			
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			this->setRotationZ( message.getAsRotation()->z() );
			break;
			
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			this->setRotationZ( this->getRotation().z() + message.getAsRotationFactor()->z() );
			break;
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			this->updateGeometry( message );
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			this->updateGeometry( message );
			break;
			
		default:	// unknown event; don't handle
			return 0;
	}
	
	return 1;
}

// toString
string pyramid::toString(void) {
	return string("pyramid\n") +
				  this->BZWLines() +
				  "end\n";	
}

void pyramid::updateGeometry( UpdateMessage& message ) {
	// if we changed the scale, update the texture coordinates (i.e. the scale might have changed)
	// NOTE: it is expected that message.data will point to an osg::Vec3, which contains the scaling FACTOR
	if( message.type == UpdateMessage::SET_SCALE_FACTOR ) {
		
		// extract the scale factor from the message
		osg::Vec3* scaleFactor = message.getAsScaleFactor();
		
		// get the geometries from the box mesh
		GeometryExtractorVisitor geoExtractor = GeometryExtractorVisitor( this );
		vector< osg::Geometry* > geos = geoExtractor.getGeometries();
		
		// there should be 2 geometries (One Geometry makes up the 4 sides, the other the floor.)
		// if there isn't, then bail
		if( geos.size() != 2 ) {
			// printf(" error! %d geometries (expected 2)\n", geos.size());
			return;
		}
	}
}

