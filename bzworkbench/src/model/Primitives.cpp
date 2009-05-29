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
#include <osg/StateSet>
#include <osg/Group>
#include <osgDB/ReadFile>

#include <cmath>

using namespace std;

// build a pyramid
osg::Node* Primitives::buildPyramid( const osg::Vec3* size ) {
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
	rebuildPyramidUV( pyramid, size );

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
	osg::Texture2D* sideTexture = loadTexture("share/pyramid/pyrwall.png");

	// assign the texture
	setNodeTexture( pyramid, sideTexture );

	return pyramid;
}

// rebuild UVs for a pyr, should be called whenever the box is scaled
void Primitives::rebuildPyramidUV( osg::Geode* pyr, const osg::Vec3* size ) {
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

void Primitives::rebuildBoxUV(osg::Group* box, const osg::Vec3* size)
{
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	float xSideUV = size->x()*2/9.42f;
	float ySideUV = size->y()*2/9.42f;
	float zSideUV = size->z()/9.42f;
	float xTopUV = xSideUV*5;
	float yTopUV = ySideUV*5;

	// +x -x
	for ( int i = 0; i < 2; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, 0 ) );
	}

	// +y -y
	for ( int i = 2; i < 4; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, 0 ) );
	}

	// +z -z
	for ( int i = 4; i < 6; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, 0 ) );
	}

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)box->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

osg::Group* Primitives::buildUntexturedBox( const osg::Vec3* size ) {
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
	
	return group;
}

osg::Texture2D* Primitives::loadTexture( const std::string& file ) {
	osg::Texture2D* texture = new osg::Texture2D();
	texture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* img = osgDB::readImageFile( file );
	// only set image if it was loaded properly
	if (img)
		texture->setImage(img);

	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	return texture;
}

void Primitives::setNodeTexture( osg::Node* node, osg::Texture2D* texture ) {
	osg::StateSet* state = new osg::StateSet();

	state->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
	node->setStateSet(state);
}