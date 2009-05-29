/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
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

#include "model/Primitives.h"

// constructor
teleporter::teleporter() :
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	
	setDefaults();

	// make sure geometry is built
	updateGeometry();
}

// constructor with data
teleporter::teleporter(string& data) :	// don't pass the data to the parent class (this prevents super-sized teleporters)
	bz2object("teleporter", "<position><size><rotation><name><border>") {
	
	setDefaults();

	update( data );

	// make sure geometry is built
	updateGeometry();
}

void teleporter::setDefaults() {
	osg::Group* teleporter = new osg::Group();
	osg::Geode* portal = new osg::Geode();
	osg::Geode* border = new osg::Geode();

	teleporter->addChild( portal );
	teleporter->addChild( border );

	setThisNode( teleporter );

	setBorder( 0.5f );
	
	osg::Vec3 scale = osg::Vec3( getBorder(), 10, 20 );
	realSize = scale;
	
	setSize( scale );
	
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
	vector<string> names = BZWParser::getValuesByKey( "teleporter", header, teleporterData );
	
	// get the border
	vector<string> borders = BZWParser::getValuesByKey( "border", header, teleporterData );
	if( borders.size() > 1 ) {
		printf("teleporter::update(): Error! Defined \"border\" %d times!\n", (int)borders.size());
	}

	// get the size, since the teleporter scales differently than other objects
	// the size must be parsed here
	vector<string> sizes = BZWParser::getValuesByKey( "size", header, teleporterData );
		
	// just go with the first size (only one should be defined)
	if(sizes.size() > 0)
		setSize( Point3D( sizes[0].c_str() ) );

	
	if( !bz2object::update( data ) )
		return 0;

	// reset the scale so that it doesn't effect the size of the object
	bz2object::setSize( osg::Vec3( 1, 1, 1 ) );
	
	if( names.size() > 0 ) setName( names[0] );
	
	// set the data
	setBorder( (borders.size() != 0 ? atof( borders[0].c_str() ) : 0.0f) );
	
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
			setSize( *(message.getAsScale()) );
			updateGeometry();
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			updateGeometry();
			break;
			
		default:	// unknown event; don't handle
			return result;
	}
	
	return 1;
}

// tostring
string teleporter::toString(void) {
	
	// get the bz2object BZW lines
	string bzwlines = BZWLines( this );
	
	// finally, make that string
	return string("teleporter") + "\n" +
				  bzwlines +
				  "  border " + string(ftoa(border)) + "\n" +
				  "end\n";	
}

void teleporter::setSize( const osg::Vec3d& newSize ) {
	realSize = (osg::Vec3)newSize;
}

void teleporter::setBorder( float newBorder ) { 
	border = newBorder;
}

// build a teleporter
void teleporter::updateGeometry() {
	osg::Group* teleporter = (osg::Group*)getThisNode();
	osg::Geode* portal = (osg::Geode*)teleporter->getChild( 0 );
	osg::Geode* border = (osg::Geode*)teleporter->getChild( 1 );

	// generate mesh and texture coordinates
	updateTeleporterMesh( teleporter, &getSize(), getBorder() );
	updateTeleporterUV( teleporter, &getSize(), getBorder() );

	// load textures
	osg::Texture2D* portalTexture = Primitives::loadTexture( "share/teleporter/telelink.png" );
	osg::Texture2D* borderTexture = Primitives::loadTexture( "share/teleporter/caution.png" );

	// assign the textures
	Primitives::setNodeTexture( portal, portalTexture );
	Primitives::setNodeTexture( border, borderTexture );
}

// regenerate the UVs for a teleporter, call whenever the size is changed
void teleporter::updateTeleporterUV( osg::Group* tele, const osg::Vec3* size, 
									 const float borderSize ) {
	// get portal and border geometries
	osg::Geode* portal = (osg::Geode*)tele->getChild( 0 );
	osg::Geode* border = (osg::Geode*)tele->getChild( 1 );
	osg::Geometry* portalGeometry = (osg::Geometry*)portal->getDrawable( 0 );
	osg::Geometry* borderGeometry = (osg::Geometry*)border->getDrawable( 0 );

	// make new UV arrays and assign them
	osg::Vec2Array* portalUVs = new osg::Vec2Array();
	osg::Vec2Array* borderUVs = new osg::Vec2Array();
	portalGeometry->setTexCoordArray( 0, portalUVs );
	borderGeometry->setTexCoordArray( 0, borderUVs );

	const bool wantBorder = (borderSize >= 0.001f);

	
	const float br = borderSize;
	const float hb = br * 0.5f;
	const float yo = size->y() + br;
	const float ym = size->y() + hb;
	const float yi = size->y();
	const float xl = size->x();
	const float xb = (size->x() > hb) ? size->x() : hb;
	const float zt = size->z() + br;
	const float zm = size->z() + hb;
	const float zb = size->z();

	const float ztxc = zb / (2.0f * yi);
	portalUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0
	portalUVs->push_back( osg::Vec2( 1.0f, 0.0f ) ); // t1
	portalUVs->push_back( osg::Vec2( 1.0f, ztxc ) ); // t2
	portalUVs->push_back( osg::Vec2( 0.0f, ztxc ) ); // t3
	portalUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0
	portalUVs->push_back( osg::Vec2( 1.0f, 0.0f ) ); // t1
	portalUVs->push_back( osg::Vec2( 1.0f, ztxc ) ); // t2
	portalUVs->push_back( osg::Vec2( 0.0f, ztxc ) ); // t3

	if ( wantBorder ) {
		float texScale = 1.0f / borderSize;

		const osg::Vec2 xTexOffset( +yo, -zt );
		const float xb2 = xb * 2.0f;
		const float yo2 = yo * 2.0f;
		const float b2yi = br + (yi * 2.0f);

		// -x faces
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo, 0.0f )) * texScale ); // t4
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi, 0.0f )) * texScale ); // t5
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi,   zb )) * texScale ); // t7
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi, 0.0f )) * texScale ); // t8
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo, 0.0f )) * texScale ); // t9
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi,   zb )) * texScale ); // t7

		// +x faces
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo, 0.0f )) * texScale ); // t4
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi, 0.0f )) * texScale ); // t5
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yi,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi,   zb )) * texScale ); // t7
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( -yo,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi, 0.0f )) * texScale ); // t8
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo, 0.0f )) * texScale ); // t9
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yo,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +yi,   zb )) * texScale ); // t7

		// +y outside
		borderUVs->push_back( osg::Vec2( 0.0f,   -zt ) * texScale );			  // t13
		borderUVs->push_back( osg::Vec2( +xb2,   -zt ) * texScale );			  // t14
		borderUVs->push_back( osg::Vec2( +xb2,  0.0f ) * texScale );			  // t12
		borderUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0

		// -y outside
		borderUVs->push_back( osg::Vec2( 0.0f,   -zt ) * texScale );			  // t13
		borderUVs->push_back( osg::Vec2( +xb2,   -zt ) * texScale );			  // t14
		borderUVs->push_back( osg::Vec2( +xb2,  0.0f ) * texScale );			  // t12
		borderUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0

		// +y inside
		borderUVs->push_back( osg::Vec2( 0.0f,   -zt ) * texScale );			  // t13
		borderUVs->push_back( osg::Vec2( +xb2,   -zt ) * texScale );			  // t14
		borderUVs->push_back( osg::Vec2( +xb2,   -br ) * texScale );			  // t16
		borderUVs->push_back( osg::Vec2( 0.0f,   -br ) * texScale );			  // t15

		// -y inside
		borderUVs->push_back( osg::Vec2( 0.0f,   -zt ) * texScale );			  // t13
		borderUVs->push_back( osg::Vec2( +xb2,   -zt ) * texScale );			  // t14
		borderUVs->push_back( osg::Vec2( +xb2,   -br ) * texScale );			  // t16
		borderUVs->push_back( osg::Vec2( 0.0f,   -br ) * texScale );			  // t15

		// crossbar top
		borderUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0
		borderUVs->push_back( osg::Vec2( 0.0f,  -xb2 ) * texScale );			  // t18
		borderUVs->push_back( osg::Vec2( +yo2,  -xb2 ) * texScale );			  // t19
		borderUVs->push_back( osg::Vec2( +yo2,  0.0f ) * texScale );			  // t17

		// crossbar bottom
		borderUVs->push_back( osg::Vec2( +br,   0.0f ) * texScale );			  // t20
		borderUVs->push_back( osg::Vec2( +br,   -xb2 ) * texScale );			  // t21
		borderUVs->push_back( osg::Vec2( +b2yi, -xb2 ) * texScale );			  // t23
		borderUVs->push_back( osg::Vec2( +b2yi, 0.0f ) * texScale );			  // t22

		// +y pillar cap
		borderUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0
		borderUVs->push_back( osg::Vec2( 0.0f,  -xb2 ) * texScale );			  // t18
		borderUVs->push_back( osg::Vec2( +br,   -xb2 ) * texScale );			  // t21
		borderUVs->push_back( osg::Vec2( +br,   0.0f ) * texScale );			  // t20

		// -y pillar cap
		borderUVs->push_back( osg::Vec2( 0.0f, 0.0f ) ); // t0
		borderUVs->push_back( osg::Vec2( 0.0f,  -xb2 ) * texScale );			  // t18
		borderUVs->push_back( osg::Vec2( +br,   -xb2 ) * texScale );			  // t21
		borderUVs->push_back( osg::Vec2( +br,   0.0f ) * texScale );			  // t20
	}
}

// rebuild the mesh for a teleporter, call whenever the border size is changed
void teleporter::updateTeleporterMesh( osg::Group* tele, const osg::Vec3* size, 
									   const float borderSize ) {
	osg::Geode* portal = (osg::Geode*)tele->getChild( 0 );
	osg::Geode* border = (osg::Geode*)tele->getChild( 1 );

	// remove any old drawables before we add new ones
	if (portal->getNumDrawables() > 0)
		portal->removeDrawables( 0 );
	if (border->getNumDrawables() > 0)
		border->removeDrawables( 0 );

	osg::Geometry* portalGeometry = new osg::Geometry();
	osg::Geometry* borderGeometry = new osg::Geometry();
	portal->addDrawable( portalGeometry );
	border->addDrawable( borderGeometry );

	osg::Vec3Array* portalVerts = new osg::Vec3Array();
	osg::Vec3Array* borderVerts = new osg::Vec3Array();
	portalGeometry->setVertexArray( portalVerts );
	borderGeometry->setVertexArray( borderVerts );

	osg::DrawElementsUInt* portalDE = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	osg::DrawElementsUInt* borderDE = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	portalGeometry->addPrimitiveSet( portalDE );
	borderGeometry->addPrimitiveSet( borderDE );

	const bool wantBorder = (borderSize >= 0.001f);
	const float br = borderSize;
	const float hb = br * 0.5f;
	const float yo = size->y() + br;
	const float ym = size->y() + hb;
	const float yi = size->y();
	const float xl = size->x();
	const float xb = (size->x() > hb) ? size->x() : hb;
	const float zt = size->z() + br;
	const float zm = size->z() + hb;
	const float zb = size->z();

    // back face vertices  (-x normal)
	portalVerts->push_back( osg::Vec3( -xl, +yi, 0.0f ) ); // v0
	portalVerts->push_back( osg::Vec3( -xl, -yi, 0.0f ) ); // v1
	portalVerts->push_back( osg::Vec3( -xl, -yi,   zb ) ); // v2
	portalVerts->push_back( osg::Vec3( -xl, +yi,   zb ) ); // v3
	// front face vertices (+x normal)
	portalVerts->push_back( osg::Vec3( +xl, -yi, 0.0f ) ); // v4
	portalVerts->push_back( osg::Vec3( +xl, +yi, 0.0f ) ); // v5
	portalVerts->push_back( osg::Vec3( +xl, +yi,   zb ) ); // v6
	portalVerts->push_back( osg::Vec3( +xl, -yi,   zb ) ); // v7

	for ( int i = 0; i < 8; i++ )
		portalDE->push_back( i );

	// only make the border if it is needed
	if (wantBorder) {
		// -x faces
		borderVerts->push_back( osg::Vec3( -xb, +yo, 0.0f ) ); // v8
		borderVerts->push_back( osg::Vec3( -xb, +yi, 0.0f ) ); // v9
		borderVerts->push_back( osg::Vec3( -xb, +yi,   zb ) ); // v10
		borderVerts->push_back( osg::Vec3( -xb, +yo,   zt ) ); // v15
		borderVerts->push_back( osg::Vec3( -xb, +yi,   zb ) ); // v10
		borderVerts->push_back( osg::Vec3( -xb, -yi,   zb ) ); // v11
		borderVerts->push_back( osg::Vec3( -xb, -yo,   zt ) ); // v14
		borderVerts->push_back( osg::Vec3( -xb, +yo,   zt ) ); // v15
		borderVerts->push_back( osg::Vec3( -xb, -yi, 0.0f ) ); // v12
		borderVerts->push_back( osg::Vec3( -xb, -yo, 0.0f ) ); // v13
		borderVerts->push_back( osg::Vec3( -xb, -yo,   zt ) ); // v14
		borderVerts->push_back( osg::Vec3( -xb, -yi,   zb ) ); // v11

		// +x faces
		borderVerts->push_back( osg::Vec3( +xb, -yo, 0.0f ) ); // v16
		borderVerts->push_back( osg::Vec3( +xb, -yi, 0.0f ) ); // v17
		borderVerts->push_back( osg::Vec3( +xb, -yi,   zb ) ); // v18
		borderVerts->push_back( osg::Vec3( +xb, -yo,   zt ) ); // v23
		borderVerts->push_back( osg::Vec3( +xb, -yi,   zb ) ); // v18
		borderVerts->push_back( osg::Vec3( +xb, +yi,   zb ) ); // v19
		borderVerts->push_back( osg::Vec3( +xb, +yo,   zt ) ); // v22
		borderVerts->push_back( osg::Vec3( +xb, -yo,   zt ) ); // v23
		borderVerts->push_back( osg::Vec3( +xb, +yi, 0.0f ) ); // v20
		borderVerts->push_back( osg::Vec3( +xb, +yo, 0.0f ) ); // v21
		borderVerts->push_back( osg::Vec3( +xb, +yo,   zt ) ); // v22
		borderVerts->push_back( osg::Vec3( +xb, +yi,   zb ) ); // v19

		// +y outside
		borderVerts->push_back( osg::Vec3( +xb, +yo, 0.0f ) ); // v21
		borderVerts->push_back( osg::Vec3( -xb, +yo, 0.0f ) ); // v8
		borderVerts->push_back( osg::Vec3( -xb, +yo,   zt ) ); // v15
		borderVerts->push_back( osg::Vec3( +xb, +yo,   zt ) ); // v22

		// -y outside
		borderVerts->push_back( osg::Vec3( -xb, -yo, 0.0f ) ); // v13
		borderVerts->push_back( osg::Vec3( +xb, -yo, 0.0f ) ); // v16
		borderVerts->push_back( osg::Vec3( +xb, -yo,   zt ) ); // v23
		borderVerts->push_back( osg::Vec3( -xb, -yo,   zt ) ); // v14

		// +y inside
		borderVerts->push_back( osg::Vec3( -xb, +yi, 0.0f ) ); // v9
		borderVerts->push_back( osg::Vec3( +xb, +yi, 0.0f ) ); // v20
		borderVerts->push_back( osg::Vec3( +xb, +yi,   zb ) ); // v19
		borderVerts->push_back( osg::Vec3( -xb, +yi,   zb ) ); // v10

		// -y inside
		borderVerts->push_back( osg::Vec3( +xb, -yi, 0.0f ) ); // v17
		borderVerts->push_back( osg::Vec3( -xb, -yi, 0.0f ) ); // v12
		borderVerts->push_back( osg::Vec3( -xb, -yi,   zb ) ); // v11
		borderVerts->push_back( osg::Vec3( +xb, -yi,   zb ) ); // v18

		// crossbar top
		borderVerts->push_back( osg::Vec3( -xb, -yo,   zt ) ); // v14
		borderVerts->push_back( osg::Vec3( +xb, -yo,   zt ) ); // v23
		borderVerts->push_back( osg::Vec3( +xb, +yo,   zt ) ); // v22
		borderVerts->push_back( osg::Vec3( -xb, +yo,   zt ) ); // v15

		// crossbar bottom
		borderVerts->push_back( osg::Vec3( -xb, +yi,   zb ) ); // v10
		borderVerts->push_back( osg::Vec3( +xb, +yi,   zb ) ); // v19
		borderVerts->push_back( osg::Vec3( +xb, -yi,   zb ) ); // v18
		borderVerts->push_back( osg::Vec3( -xb, -yi,   zb ) ); // v11

		// +y pillar cap
		borderVerts->push_back( osg::Vec3( -xb, +yo, 0.0f ) ); // v8
		borderVerts->push_back( osg::Vec3( +xb, +yo, 0.0f ) ); // v21
		borderVerts->push_back( osg::Vec3( +xb, +yi, 0.0f ) ); // v20
		borderVerts->push_back( osg::Vec3( -xb, +yi, 0.0f ) ); // v9

		// -y pillar cap
		borderVerts->push_back( osg::Vec3( -xb, -yi, 0.0f ) ); // v12
		borderVerts->push_back( osg::Vec3( +xb, -yi, 0.0f ) ); // v17
		borderVerts->push_back( osg::Vec3( +xb, -yo, 0.0f ) ); // v16
		borderVerts->push_back( osg::Vec3( -xb, -yo, 0.0f ) ); // v13

		for ( int i = 0; i < 56; i++ ) 
			borderDE->push_back( i );
	}
}
