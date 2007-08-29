/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "objects/teleporter.h"

// constructor
teleporter::teleporter() :
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	
	border = 0.125;
	realSize = Point3D( 0.0, 0.0, 0.0 );
	
	leftLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	rightLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	topBeam = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_beam.obj" ) );
	frontPortal = new Renderable( SceneBuilder::buildNode( "share/teleporter/portal.obj" ) );
	rearPortal = new Renderable( SceneBuilder::buildNode( "share/teleporter/portal.obj" ) );
	
	// offset leftLeg so it's geometric data is to the left of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	leftLeg->setPosition( osg::Vec3(0, -1.0, 0) );
	
	// offset rightLeg so it's geometric data is to thr right of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	rightLeg->setPosition( osg::Vec3( 0, 1.0, 0 ) );
	
	// offset topBeam so it's geometric data is one unit above the geometry
	topBeam->setPosition( osg::Vec3( 0, 0, 1.0 ) );
	
	// double the width of topBeam so it rests atop leftLeg and rightLeg
	topBeam->setScale( osg::Vec3( 1.0, 2.0, 1.0 ) );
	
	// set the positions of the portals
	frontPortal->setPosition( osg::Vec3(border, 0, 0) );
	rearPortal->setPosition( osg::Vec3(-border, 0, 0) );
	
	// add the nodes to the teleporter
	theTeleporter = new osg::Group();
	theTeleporter->addChild( leftLeg.get() );
	theTeleporter->addChild( rightLeg.get() );
	theTeleporter->addChild( topBeam.get() );
	theTeleporter->addChild( frontPortal.get() );
	theTeleporter->addChild( rearPortal.get() );
	
	setThisNode(theTeleporter.get());
	addChild( theTeleporter.get() );
	
	// blow up the teleporter
	osg::Vec3 scale = osg::Vec3( border, 10, 20 );
	realSize = scale;
	
	UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE, &scale );
	setSize( scale );
	
	SceneBuilder::markUnselected( this );
}

// constructor with data
teleporter::teleporter(string& data) :	// don't pass the data to the parent class (this prevents super-sized teleporters)
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	
	border = 0.125;
	osg::Vec3 scale = osg::Vec3( border, 10, 20 );
	realSize = scale;
	
	leftLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	rightLeg = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_leg.obj" ) );
	topBeam = new Renderable( SceneBuilder::buildNode( "share/teleporter/teleporter_beam.obj" ) );
	frontPortal = new Renderable( SceneBuilder::buildNode( "share/teleporter/portal.obj" ) );
	rearPortal = new Renderable( SceneBuilder::buildNode( "share/teleporter/portal.obj" ) );
	
	// offset leftLeg so it's geometric data is to the left of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	leftLeg->setPosition( osg::Vec3(0, -1.0, 0) );
	
	// offset rightLeg so it's geometric data is to thr right of the local origin
	// NOTE: leftLeg's geometry is known in advance to be a 1x1x1 cube
	rightLeg->setPosition( osg::Vec3( 0, 1.0, 0 ) );
	
	// offset topBeam so it's geometric data is one unit above the geometry
	topBeam->setPosition( osg::Vec3( 0, 0, 1.0 ) );
	
	// double the width of topBeam so it rests atop leftLeg and rightLeg
	topBeam->setScale( osg::Vec3( 1.0, 2.0, 1.0 ) );
	
	// set the positions of the portals
	frontPortal->setPosition( osg::Vec3(border, 0, 0) );
	rearPortal->setPosition( osg::Vec3(-border, 0, 0) );
	
	// add the nodes to the teleporter
	// add the nodes to the teleporter
	theTeleporter = new osg::Group();
	theTeleporter->addChild( leftLeg.get() );
	theTeleporter->addChild( rightLeg.get() );
	theTeleporter->addChild( topBeam.get() );
	theTeleporter->addChild( frontPortal.get() );
	theTeleporter->addChild( rearPortal.get() );
	
	setThisNode( theTeleporter.get() );
	addChild( theTeleporter.get() );
	
	setSize( realSize );
	update(data);
	
	SceneBuilder::markUnselected( this );
}

// getter
string teleporter::get(void) {  return toString(); }

// setter
int teleporter::update(string& data) {
	// get header
	const char* header = getHeader().c_str();
	
	// get the section
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "teleporter"))
		return 0;
		
	// get the data
	const char* teleporterData = lines[0].c_str();
	
	// get the name if it's in the title
	vector<string> names = BZWParser::getValuesByKey("teleporter", header, teleporterData );
	
	// get the border
	vector<string> borders = BZWParser::getValuesByKey("border", header, teleporterData);
	if(borders.size() > 1) {
		printf("teleporter::update(): Error! Defined \"border\" %d times!\n", borders.size());
	}	
	
	if(!bz2object::update(data))
		return 0;
	
	if( names.size() > 0 ) setName( names[0] );
	
	// set the data
	border = (borders.size() != 0 ? atof( borders[0].c_str() ) : 0.0f);
	
	return 1;
}

// update with binary message
int teleporter::update( UpdateMessage& message ) {
	
	// superclass event handler
	int result = bz2object::update( message );
	
	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;
			
		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;
			
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotationZ( message.getAsRotation()->z() );
			break;
			
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotationZ( getRotation().z() + message.getAsRotationFactor()->z() );
			break;
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			updateGeometry( message );
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			updateGeometry( message );
			break;
			
		default:	// unknown event; don't handle
			return result;
	}
	
	return 1;
}

// tostring
string teleporter::toString(void) {
	
	// get the bz2object BZW lines
	string bzwlines = BZWLines();
	
	// finally, make that string
	return string("teleporter") + "\n" +
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
			osg::Vec3* scaleFactor = message.getAsScaleFactor();
			
			// when we scale by X, we're really moving the portals closer or further away from the teleporter
			if( scaleFactor->x() != 0.0 ) {
				
				// don't have the portals switch places
				if( frontPortal->getPosition().x() + scaleFactor->x() <= rearPortal->getPosition().x() - scaleFactor->x() )
					scaleFactor->set( 0, scaleFactor->y(), scaleFactor->z() );
				
				frontPortal->setPosition( osg::Vec3( frontPortal->getPosition().x() + scaleFactor->x(),
														   frontPortal->getPosition().y(),
														   frontPortal->getPosition().z() ) );
														   
				rearPortal->setPosition( osg::Vec3( rearPortal->getPosition().x() - scaleFactor->x(),
														   rearPortal->getPosition().y(),
														   rearPortal->getPosition().z() ) );
			
			}
			
			// when we scale by Y, we're really making the teleporter wider.
			// to do that, we'll just move the legs further apart by scaleFactor, and stretch the top beam by scaleFactor
			if( scaleFactor->y() != 0.0 ) {
				// we need to clamp the width of the teleporter to 2 * border, if necessary
				// to do so, we need to check and see if the resulting size will have negative width
				if( topBeam->getScale().y() + 2 * scaleFactor->y() <= 2 * border ) {
					
					scaleFactor->set( scaleFactor->x(), 0, scaleFactor->z() );	
				}
				// scale the left leg's position
				leftLeg->setPosition( osg::Vec3(  leftLeg->getPosition().x(),
														leftLeg->getPosition().y() - scaleFactor->y(),
														leftLeg->getPosition().z() ) );
				
				// scale the right leg's position
				rightLeg->setPosition( osg::Vec3(  rightLeg->getPosition().x(),
														 rightLeg->getPosition().y() + scaleFactor->y(),
														 rightLeg->getPosition().z() ) );
				 
				 // scale the top beam's y dimension
				 topBeam->setScale( osg::Vec3(  topBeam->getScale().x(),
							 						  topBeam->getScale().y() + 2 * scaleFactor->y(),
							 						  topBeam->getScale().z() ) );
							 						  
				// scale the front portal
				frontPortal->setScale( osg::Vec3( frontPortal->getScale().x(),
														frontPortal->getScale().y() + 2 * scaleFactor->y(),
														frontPortal->getScale().z() ) );
														
				// scale the rear portal
				rearPortal->setScale( osg::Vec3(  rearPortal->getScale().x(),
														rearPortal->getScale().y() + 2 * scaleFactor->y(),
														rearPortal->getScale().z() ) );
				
			}
			
			// when we scale by Z, we're really making the teleporter taller.
			// to do that, we'll just move the top beam higher, and scale the legs vertically
			if( scaleFactor->z() != 0.0 ) {
				// first, clamp the height of the teleporter to 0 if needed
				if( topBeam->getPosition().z() + scaleFactor->z() <= 0 )
					scaleFactor->set( scaleFactor->x(), scaleFactor->y(), 0 );
					
				// scale the left leg's height
				leftLeg->setScale( osg::Vec3(  leftLeg->getScale().x(),
													 leftLeg->getScale().y(),
													 leftLeg->getScale().z() + scaleFactor->z() ) );
										 
				// scale the right leg's height
				rightLeg->setScale( osg::Vec3(  rightLeg->getScale().x(),
													  rightLeg->getScale().y(),
													  rightLeg->getScale().z() + scaleFactor->z() ) );
				
				// increase the height of the top beam
				topBeam->setPosition( osg::Vec3(  topBeam->getPosition().x(),
														topBeam->getPosition().y(),
														topBeam->getPosition().z() + scaleFactor->z() ) );
														
				// scale the front portal
				frontPortal->setScale( osg::Vec3( frontPortal->getScale().x(),
														frontPortal->getScale().y(),
														frontPortal->getScale().z() + scaleFactor->z() ) );
														
				// scale the rear portal
				rearPortal->setScale( osg::Vec3(  rearPortal->getScale().x(),
														rearPortal->getScale().y(),
														rearPortal->getScale().z() + scaleFactor->z() ) );
			}
			
			// finally, update the real size
			realSize.set( realSize.x() + scaleFactor->x(),
								realSize.y() + scaleFactor->y(),
								realSize.z() + scaleFactor->z() );
			
			break;
		}
		
		case UpdateMessage::SET_SCALE: {		// handle a new scale
			
			// get the scale factor from data
			osg::Vec3* scaleFactor = message.getAsScale();
			
			
			// scale by X will increase the distance between the portals
			if( scaleFactor->x() != 0.0 ) {
				// don't have the portals switch places
				if( scaleFactor->x() < 0 )
					scaleFactor->set( 0, scaleFactor->y(), scaleFactor->z() );
				
				frontPortal->setPosition( osg::Vec3( scaleFactor->x(),
														   frontPortal->getPosition().y(),
														   frontPortal->getPosition().z() ) );
														   
				rearPortal->setPosition( osg::Vec3( -scaleFactor->x(),
														   rearPortal->getPosition().y(),
														   rearPortal->getPosition().z() ) );
			
			}
			
			// when we scale by Y, we're really making the teleporter wider.
			// to do that, we'll just move the legs further apart by scaleFactor, and stretch the top beam by scaleFactor
			if( scaleFactor->y() != 0.0 ) {
				// scale the left leg's position
				leftLeg->setPosition( osg::Vec3(  leftLeg->getPosition().x(),
														-scaleFactor->y(),
														leftLeg->getPosition().z() ) );
				
				// scale the right leg's position
				rightLeg->setPosition( osg::Vec3(  rightLeg->getPosition().x(),
														 scaleFactor->y(),
														 rightLeg->getPosition().z() ) );
				 
				 // scale the top beam's y dimension
				 topBeam->setScale( osg::Vec3(  topBeam->getScale().x(),
							 						  2 * scaleFactor->y() + 1,
							 						  topBeam->getScale().z()) );
							 						  
				 // scale the front portal
				frontPortal->setScale( osg::Vec3( frontPortal->getScale().x(),
														2 * scaleFactor->y(),
														frontPortal->getScale().z() ) );
														
				// scale the rear portal
				rearPortal->setScale( osg::Vec3(  rearPortal->getScale().x(),
														2 * scaleFactor->y(),
														rearPortal->getScale().z() ) );
			}
			
			// when we scale by Z, we're really making the teleporter taller.
			// to do that, we'll just move the top beam higher, and scale the legs vertically
			if( scaleFactor->z() != 0.0 ) {
				// scale the left leg's height
				leftLeg->setScale( osg::Vec3(  leftLeg->getScale().x(),
													 leftLeg->getScale().y(),
													 scaleFactor->z() ) );
										 
				// scale the right leg's height
				rightLeg->setScale( osg::Vec3(  rightLeg->getScale().x(),
													  rightLeg->getScale().y(),
													  scaleFactor->z() ) );
				
				// increase the height of the top beam
				topBeam->setPosition( osg::Vec3(  topBeam->getPosition().x(),
														topBeam->getPosition().y(),
														scaleFactor->z() ) );
														
				// scale the front portal
				frontPortal->setScale( osg::Vec3( frontPortal->getScale().x(),
														frontPortal->getScale().y(),
														scaleFactor->z() ) );
														
				// scale the rear portal
				rearPortal->setScale( osg::Vec3(  rearPortal->getScale().x(),
														rearPortal->getScale().y(),
														scaleFactor->z() ) );
			}
			
			// finally, update the real size
			realSize = (*scaleFactor);
			
			break;
		}
		
		default:	// all other messages
			break;	
	}
}
