#include "../include/objects/teleporter.h"

// constructor
teleporter::teleporter() :
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	
	border = 1.0;
	realSize = Point3D( 0.0, 0.0, 0.0 );
	
	this->leftLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	this->rightLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	this->topBeam = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_beam.obj" ) );
	
	// offset leftLeg so it's geometric data is to the left of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	this->leftLeg->setPosition( osg::Vec3(0, -1.0, 0) );
	
	// offset rightLeg so it's geometric data is to thr right of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	this->rightLeg->setPosition( osg::Vec3( 0, 1.0, 0 ) );
	
	// offset topBeam so it's geometric data is one unit above the geometry
	this->topBeam->setPosition( osg::Vec3( 0, 0, 1.0 ) );
	
	// double the width of topBeam so it rests atop leftLeg and rightLeg
	this->topBeam->setScale( osg::Vec3( 1.0, 2.0, 1.0 ) );
	
	// add the nodes to the teleporter
	this->addChild( leftLeg.get() );
	this->addChild( rightLeg.get() );
	this->addChild( topBeam.get() );
	
	
	// blow up the teleporter
	osg::Vec3 scale = osg::Vec3( 0.125, 10, 20 );
	
	UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE, &scale );
	
	this->updateGeometry( msg );
}

// constructor with data
teleporter::teleporter(string& data) :
	bz2object("teleporter", "<position><size><rotation><name><border>", data.c_str()) {
	
	border = 1.0;
	realSize = Point3D( 0.0, 0.0, 0.0 );
	
	this->leftLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	this->rightLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	this->topBeam = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_beam.obj" ) );
	
	// offset leftLeg so it's geometric data is to the left of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	this->leftLeg->setPosition( osg::Vec3(0, -1.0, 0) );
	
	// offset rightLeg so it's geometric data is to thr right of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	this->rightLeg->setPosition( osg::Vec3( 0, 1.0, 0 ) );
	
	// offset topBeam so it's geometric data is one unit above the geometry
	this->topBeam->setPosition( osg::Vec3( 0, 0, 1.0 ) );
	
	// double the width of topBeam so it rests atop leftLeg and rightLeg
	this->topBeam->setScale( osg::Vec3( 1.0, 2.0, 1.0 ) );
	
	// add the nodes to the teleporter
	this->addChild( leftLeg.get() );
	this->addChild( rightLeg.get() );
	this->addChild( topBeam.get() );
	
	
	// blow up the teleporter
	osg::Vec3 scale = osg::Vec3( 0.125, 10, 20 );
	
	UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE, &scale );
	
	this->updateGeometry( msg );
	
	this->update(data);
}

// getter
string teleporter::get(void) {  return this->toString(); }

// setter
int teleporter::update(string& data) {
	// get header
	const char* header = this->getHeader().c_str();
	
	// get the section
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "teleporter"))
		return 0;
		
	// get the data
	const char* teleporterData = lines[0].c_str();
	
	// get the border
	vector<string> borders = BZWParser::getValuesByKey("border", header, teleporterData);
	if(borders.size() > 1) {
		printf("teleporter::update(): Error! Defined \"border\" %d times!\n", borders.size());
	}	
	
	// get the linkage name
	vector<string> lnames = BZWParser::getValuesByKey("teleporter", header, data.c_str());
	
	// get the current scale
	osg::Vec3 scale = this->getScale();
	
	if(!bz2object::update(data))
		return 0;
	
	// see if the scale changed
	if( scale != this->getScale() ) {
		// the scale changed; compute the difference and update the geometry
		osg::Vec3 dscale = this->getScale() - scale;
		UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE_FACTOR, &dscale );
		this->updateGeometry( msg );	
	}
	
	// set the data
	this->border = (borders.size() != 0 ? atof( borders[0].c_str() ) : 0.0f);
	this->lname = (lnames.size() != 0 ? lnames[0] : "defaultLinkageName");
	
	return 1;
}

// update with binary message
int teleporter::update( string& data, UpdateMessage& message ) {
	
	this->updateGeometry( message );	
	
	return 1;
}

// tostring
string teleporter::toString(void) {
	// there's a different way of reporting size in teleporters, since the size isn't stored in the geometry
	// get the bz2object BZW lines
	string bzwlines = this->BZWLines();
	
	// find the "size" entry and replace it with the real size
	string::size_type sizeStart = bzwlines.find( "size", 0 );
	if( sizeStart != string::npos ) {	// if found...
		// find the end of the line
		string::size_type sizeEnd = bzwlines.find( "\n", sizeStart );
		
		if( sizeEnd != string::npos ) {  // if an end was found...
			// reformulate bzwlines
			bzwlines = bzwlines.substr( 0, sizeStart ) + "size " + this->realSize.toString() + bzwlines.substr( sizeEnd + 1 );
		}
		else {  // cut out the end
			bzwlines = bzwlines.substr( 0, sizeStart ) + "size " + this->realSize.toString();
		}
	}
	
	// finally, make that string
	return string("teleporter ") + lname + "\n" +
				  bzwlines +
				  "  border " + string(ftoa(border)) + "\n" +
				  "end\n";	
}

// update the nodes that make up a teleporter
void teleporter::updateGeometry( UpdateMessage& message ) {
	
	// determine the type of message
	switch( message.type ) {
		case UpdateMessage::SET_SCALE_FACTOR: {		// handle a resize
			
			// get the scale factor from data
			osg::Vec3* scaleFactor = (osg::Vec3*)message.data;
			
			// undo the scale (since that was what was changed prior to this message being sent)
			// rather, we'll be changing the general shape of the teleporter
			this->setScale( this->getScale() - (*scaleFactor) );
			
			// when we scale by Y, we're really making the teleporter wider.
			// to do that, we'll just move the legs further apart by scaleFactor, and stretch the top beam by scaleFactor
			if( scaleFactor->y() != 0.0 ) {
				// scale the left leg's position
				this->leftLeg->setPosition( osg::Vec3(  this->leftLeg->getPosition().x(),
														this->leftLeg->getPosition().y() - scaleFactor->y(),
														this->leftLeg->getPosition().z() ) );
				
				// scale the right leg's position
				this->rightLeg->setPosition( osg::Vec3(  this->rightLeg->getPosition().x(),
														 this->rightLeg->getPosition().y() + scaleFactor->y(),
														 this->rightLeg->getPosition().z() ) );
				 
				 // scale the top beam's y dimension
				 this->topBeam->setScale( osg::Vec3(  this->topBeam->getScale().x(),
							 						  this->topBeam->getScale().y() + 2 * scaleFactor->y(),
							 						  this->topBeam->getScale().z() ) );
			}
			
			// when we scale by Z, we're really making the teleporter taller.
			// to do that, we'll just move the top beam higher, and scale the legs vertically
			if( scaleFactor->z() != 0.0 ) {
				// scale the left leg's height
				this->leftLeg->setScale( osg::Vec3(  this->leftLeg->getScale().x(),
													 this->leftLeg->getScale().y(),
													 this->leftLeg->getScale().z() + scaleFactor->z() ) );
										 
				// scale the right leg's height
				this->rightLeg->setScale( osg::Vec3(  this->rightLeg->getScale().x(),
													  this->rightLeg->getScale().y(),
													  this->rightLeg->getScale().z() + scaleFactor->z() ) );
				
				// increase the height of the top beam
				this->topBeam->setPosition( osg::Vec3(  this->topBeam->getPosition().x(),
														this->topBeam->getPosition().y(),
														this->topBeam->getPosition().z() + scaleFactor->z() ) );
			}
			
			// finally, update the real size
			this->realSize.set( this->realSize.x() + scaleFactor->x(),
								this->realSize.y() + scaleFactor->y(),
								this->realSize.z() + scaleFactor->z() );
			
			break;
		}
		
		case UpdateMessage::SET_SCALE: {		// handle a new scale
			
			// get the scale factor from data
			osg::Vec3* scaleFactor = (osg::Vec3*)message.data;
			
			// when we scale by Y, we're really making the teleporter wider.
			// to do that, we'll just move the legs further apart by scaleFactor, and stretch the top beam by scaleFactor
			if( scaleFactor->y() != 0.0 ) {
				// scale the left leg's position
				this->leftLeg->setPosition( osg::Vec3(  this->leftLeg->getPosition().x(),
														-scaleFactor->y(),
														this->leftLeg->getPosition().z() ) );
				
				// scale the right leg's position
				this->rightLeg->setPosition( osg::Vec3(  this->rightLeg->getPosition().x(),
														 scaleFactor->y(),
														 this->rightLeg->getPosition().z() ) );
				 
				 // scale the top beam's y dimension
				 this->topBeam->setScale( osg::Vec3(  this->topBeam->getScale().x(),
							 						  2 * scaleFactor->y() + 1,
							 						  this->topBeam->getScale().z()) );
			}
			
			// when we scale by Z, we're really making the teleporter taller.
			// to do that, we'll just move the top beam higher, and scale the legs vertically
			if( scaleFactor->z() != 0.0 ) {
				// scale the left leg's height
				this->leftLeg->setScale( osg::Vec3(  this->leftLeg->getScale().x(),
													 this->leftLeg->getScale().y(),
													 scaleFactor->z() ) );
										 
				// scale the right leg's height
				this->rightLeg->setScale( osg::Vec3(  this->rightLeg->getScale().x(),
													  this->rightLeg->getScale().y(),
													  scaleFactor->z() ) );
				
				// increase the height of the top beam
				this->topBeam->setPosition( osg::Vec3(  this->topBeam->getPosition().x(),
														this->topBeam->getPosition().y(),
														scaleFactor->z() ) );
			}
			
			// finally, update the real size
			this->realSize = (*scaleFactor);
			
			break;
		}
		
		default:	// all other messages
			break;	
	}
}
