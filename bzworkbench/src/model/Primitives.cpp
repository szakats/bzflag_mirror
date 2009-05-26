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

#include "model/Primitives.h"

#include <osg/Vec3>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Group>
#include <osgDB/ReadFile>

// build a primitive given its name
osg::Node* Primitives::BuildByName( const char* name ) {
	string str(name);

	if (str == "box")
		return BuildBox( &osg::Vec3( 1, 1, 1 ) );
	if (str == "pyramid")
		return BuildPyramid( &osg::Vec3( 1, 1, 1 ) );
	if (str == "teleporter")
		return BuildTeleporter( &osg::Vec3( 0.1f, 1, 1 ), 0.1f );
	else
		return NULL;
}

// build a box
osg::Node* Primitives::BuildBox( const osg::Vec3* size ) {
	osg::Group* group = new osg::Group();
	// separate geometry nodes are needed so that each side
	// can have a separate material
	// array is in the order +x -x +y -y +z -z in bzflag coordinates
	osg::Geode* sideNodes[6];
	for ( int i = 0; i < 6; i++ )
		sideNodes[i] = new osg::Geode();

	// assign geometry nodes to group
	for ( int i = 0; i < 6; i++ )
		group->addChild( sideNodes[i] );

	// create geometry and assign it to the nodes
	osg::Geometry* sideGeometry[6];
	for ( int i = 0; i < 6; i++ ) {
		sideGeometry[i] = new osg::Geometry();
		sideNodes[i]->addDrawable(sideGeometry[i]);
	}

	// add vertices for all sides
	osg::Vec3Array* pxVerts = new osg::Vec3Array();
	pxVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	pxVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	pxVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pxVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	sideGeometry[0]->setVertexArray(pxVerts);

	osg::Vec3Array* nxVerts = new osg::Vec3Array();
	nxVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	nxVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	nxVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	nxVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	sideGeometry[1]->setVertexArray(nxVerts);

	osg::Vec3Array* pyVerts = new osg::Vec3Array();
	pyVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	pyVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pyVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	pyVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	sideGeometry[2]->setVertexArray(pyVerts);

	osg::Vec3Array* nyVerts = new osg::Vec3Array();
	nyVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	nyVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	nyVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	nyVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	sideGeometry[3]->setVertexArray(nyVerts);

	osg::Vec3Array* pzVerts = new osg::Vec3Array();
	pzVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	sideGeometry[4]->setVertexArray(pzVerts);

	osg::Vec3Array* nzVerts = new osg::Vec3Array();
	nzVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	sideGeometry[5]->setVertexArray(nzVerts);

	// generate UV coordinates
	RebuildBoxUV(group, size);

	// specify the vertex indices, this is the same for all sides
	osg::DrawElementsUInt* side =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	side->push_back( 0 );
	side->push_back( 1 );
	side->push_back( 2 );
	side->push_back( 3 );

	// add vertex indices for each side
	for ( int i = 0; i < 6; i++ ) {
		sideGeometry[i]->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
		sideGeometry[i]->addPrimitiveSet( side );
	}

	// load side texture
	osg::Texture2D* sideTexture = new osg::Texture2D();
	sideTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* sideImg = osgDB::readImageFile("share/box/boxwall.png");
	// only set image if it was loaded properly
	if (sideImg)
		sideTexture->setImage(sideImg);

	sideTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	sideTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	// load roof texture
	osg::Texture2D* roofTexture = new osg::Texture2D();
	sideTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* roofImg = osgDB::readImageFile("share/box/roof.png");
	// only set image if it was loaded properly
	if (roofImg)
		roofTexture->setImage(roofImg);

	roofTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	roofTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	// make a state set for associating the textures with the box
	osg::StateSet* state[6];
	for (int i = 0; i < 6; i++) {
		state[i] = new osg::StateSet();

		if (i < 4)
			state[i]->setTextureAttributeAndModes( 0, sideTexture, osg::StateAttribute::ON );
		else
			state[i]->setTextureAttributeAndModes( 0, roofTexture, osg::StateAttribute::ON );

		sideNodes[i]->setStateSet(state[i]);
	}

	return group;
}

void Primitives::RebuildBoxUV(osg::Group* box, const osg::Vec3* size)
{
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	float xSideScale = size->x()*2/9.42f;
	float ySideScale = size->y()*2/9.42f;
	float zSideScale = size->z()/9.42f;
	float xTopScale = xSideScale*5;
	float yTopScale = ySideScale*5;

	// +x -x
	sideUVs[0]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[0]->push_back( osg::Vec2( 0, zSideScale ) );
	sideUVs[0]->push_back( osg::Vec2( ySideScale, zSideScale ) );
	sideUVs[0]->push_back( osg::Vec2( ySideScale, 0 ) );
	sideUVs[1]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[1]->push_back( osg::Vec2( 0, zSideScale ) );
	sideUVs[1]->push_back( osg::Vec2( ySideScale, zSideScale ) );
	sideUVs[1]->push_back( osg::Vec2( ySideScale, 0 ) );

	// +y -y
	sideUVs[2]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[2]->push_back( osg::Vec2( 0, zSideScale ) );
	sideUVs[2]->push_back( osg::Vec2( xSideScale, zSideScale ) );
	sideUVs[2]->push_back( osg::Vec2( xSideScale, 0 ) );
	sideUVs[3]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[3]->push_back( osg::Vec2( 0, zSideScale ) );
	sideUVs[3]->push_back( osg::Vec2( xSideScale, zSideScale ) );
	sideUVs[3]->push_back( osg::Vec2( xSideScale, 0 ) );

	// +z -z
	sideUVs[4]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[4]->push_back( osg::Vec2( 0, yTopScale ) );
	sideUVs[4]->push_back( osg::Vec2( xTopScale, yTopScale ) );
	sideUVs[4]->push_back( osg::Vec2( xTopScale, 0 ) );
	sideUVs[5]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[5]->push_back( osg::Vec2( 0, yTopScale ) );
	sideUVs[5]->push_back( osg::Vec2( xTopScale, yTopScale ) );
	sideUVs[5]->push_back( osg::Vec2( xTopScale, 0 ) );

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)box->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

// build a pyramid
osg::Node* Primitives::BuildPyramid( const osg::Vec3* size ) {
	osg::Geode* pyramid = new osg::Geode();
	osg::Geometry* geometry = new osg::Geometry();
	pyramid->addDrawable( geometry );

	// generate vertices for triangular sides
	osg::Vec3Array* verts = new osg::Vec3Array();
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	// generate verts for base
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );

	// generate texture coordinates
	RebuildPyramidUV( pyramid, size );

	// generate vertex indices for sides
	osg::DrawElementsUInt* side =
			new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	for (int i = 0; i < 4 * 3; i++) {
		side->push_back( i );
	}

	// generate vertex indices for base
	osg::DrawElementsUInt* bottom =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	bottom->push_back( 12 );
	bottom->push_back( 13 );
	bottom->push_back( 14 );
	bottom->push_back( 15 );

	geometry->setVertexArray( verts );
	geometry->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
	geometry->addPrimitiveSet( side );
	geometry->addPrimitiveSet( bottom );

	// load the texture from a file
	osg::Texture2D* sideTexture = new osg::Texture2D();
	sideTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* sideImg = osgDB::readImageFile("share/pyramid/pyrwall.png");
	// only set image if it was loaded properly
	if (sideImg)
		sideTexture->setImage(sideImg);

	// the texture needs to repeat
	sideTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	sideTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	// make a state set for assigning the texture
	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes( 0, sideTexture, osg::StateAttribute::ON );

	pyramid->setStateSet(state);

	return pyramid;
}

// rebuild UVs for a pyr, should be called whenever the box is scaled
void Primitives::RebuildPyramidUV( osg::Geode* pyr, const osg::Vec3* size ) {
	osg::Vec2Array* texcoords = new osg::Vec2Array();

	static const float Scale = 7.55f;

	float dist = size->length();
	float diagonalScale = dist/Scale;
	float xScale = size->x()*2/Scale;
	float yScale = size->y()*2/Scale;

	for ( int i = 0; i < 4; i++ ) {
		if ( i%2 ) {
			texcoords->push_back( osg::Vec2( 0, 0 ) );
			texcoords->push_back( osg::Vec2( yScale, 0 ) );
			texcoords->push_back( osg::Vec2( 0, diagonalScale ) );
		}
		else {
			texcoords->push_back( osg::Vec2( 0, 0 ) );
			texcoords->push_back( osg::Vec2( xScale, 0 ) );
			texcoords->push_back( osg::Vec2( 0, diagonalScale ) );
		}
	}

	texcoords->push_back( osg::Vec2( xScale, yScale ) );
	texcoords->push_back( osg::Vec2( 0, yScale ) );
	texcoords->push_back( osg::Vec2( 0, 0 ) );
	texcoords->push_back( osg::Vec2( xScale, 0 ) );

	osg::Geometry* geometry = (osg::Geometry*)pyr->getDrawable( 0 );
	geometry->setTexCoordArray( 0, texcoords );
}

// build a teleporter
osg::Group* Primitives::BuildTeleporter( const osg::Vec3* size, 
										const float borderSize ) {
	osg::Group* teleporter = new osg::Group();
	osg::Geode* portal = new osg::Geode();
	osg::Geode* border = new osg::Geode();

	teleporter->addChild( portal );
	teleporter->addChild( border );

	// generate mesh and texture coordinates
	RebuildTeleporterMesh( teleporter, size, borderSize );
	RebuildTeleporterUV( teleporter, size, borderSize );

	// load textures
	osg::Texture2D* portalTexture = new osg::Texture2D();
	portalTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* portalImg = osgDB::readImageFile("share/teleporter/telelink.png");
	// only set image if it was loaded properly
	if (portalImg)
		portalTexture->setImage(portalImg);

	// the texture needs to repeat
	portalTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	portalTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	osg::Texture2D* borderTexture = new osg::Texture2D();
	borderTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* borderImg = osgDB::readImageFile("share/teleporter/caution.png");
	// only set image if it was loaded properly
	if (borderImg)
		borderTexture->setImage(borderImg);

	// the texture needs to repeat
	borderTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	borderTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	// make state sets for assigning the textures
	osg::StateSet* portalState = new osg::StateSet();
	portalState->setTextureAttributeAndModes( 0, portalTexture, osg::StateAttribute::ON );
	osg::StateSet* borderState = new osg::StateSet();
	borderState->setTextureAttributeAndModes( 0, borderTexture, osg::StateAttribute::ON );

	portal->setStateSet( portalState );
	border->setStateSet( borderState );

	return teleporter;
}

// regenerate the UVs for a teleporter, call whenever the size is changed
void Primitives::RebuildTeleporterUV( osg::Group* tele, const osg::Vec3* size, 
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
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t4
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t5
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t7
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t8
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t9
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t7

		// +x faces
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t4
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t5
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t6
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t7
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t11
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t8
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb, 0.0f )) * texScale ); // t9
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zt )) * texScale ); // t10
		borderUVs->push_back( (xTexOffset + osg::Vec2( +xb,   zb )) * texScale ); // t7

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
void Primitives::RebuildTeleporterMesh( osg::Group* tele, const osg::Vec3* size, 
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

// build a blue base
osg::Geode* Primitives::BuildBlueBase() {
	return NULL; // FIXME: implement
}

// build a green base
osg::Geode* Primitives::BuildGreenBase() {
	return NULL; // FIXME: implement
}

// build a red base
osg::Geode* Primitives::BuildRedBase() {
	return NULL; // FIXME: implement
}

// build a purple base
osg::Geode* Primitives::BuildPurpleBase() {
	return NULL; // FIXME: implement
}
